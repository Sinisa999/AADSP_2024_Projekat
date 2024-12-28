/*
 * echo.cpp
 *
 *  Created on: Dec 2, 2024
 *      Author: Filip Parag
 */

#include "echo.h"
#include "haos.h"
#include <iostream>
#include <xtensa/tie/xt_hifi4.h>

struct {
  uint32_t enable;
  int32_t tap_count[MAX_NUM_CHANNELS];
  int32_t tap_delay_us[MAX_NUM_CHANNELS][ECHO_MAX_NTAP];
  ae_f32 tap_gain[MAX_NUM_CHANNELS][ECHO_MAX_NTAP];
  ae_f32 output_gain;
} Echo_mcv = {
    true, // enable
    {
        // tap_count
        1, // Front Left (FL)
        1, // Front Right (FR)
        1, // Front Center (FC)
        1, // LFE
        4, // Back Left (BL)
        6  // Back Right (BR)
    },
    {// tap_delay
     {
         // Front Left (FL)
         100000 // 1ms
     },
     {
         // Front Right (FR)
         200000 // 2ms
     },
     {
         // Front Center (FC)
         300000 // 3ms
     },
     {
         // LFE
         4000 // 4ms
     },
     {
         // Back Left (BL)
         500,  // 0.5ms
         1000, // 1ms
         1500, // 1.5ms
         2000  // 2ms
     },
     {
         // Back Right (BR)
         1000, // 1ms
         2000, // 2ms
         3000, // 3ms
         4000, // 4ms
         5000, // 5ms
         6000, // 6ms
     }},
    { // tap_gain
     {// Front Left (FL)
      0x7fffffff},
     {// Front Right (FR)
      0x7fffffff},
     {// Front Center (FC)
      0x7fffffff},
     {// LFE
      0x7fffffff},
     {// Back Left (BL)
      0x20000000, 0x20000000, 0x20000000, 0x20000000},
     {// Back Right (BR)
      0x15555555, 0x15555555, 0x15555555, 0x15555555, 0x15555555, 0x15555555}},
    0x7fffffff // output_gain
};

HAOS_Mct_t Echo_mct = {
    0, Echo_postKickFunction, 0, 0, Echo_brickFunction, 0, 0, 0, 0};

HAOS_Mif_t Echo_mif = {&Echo_mcv, &Echo_mct};

HAOS_Odt_t Echo_odt = {{&Echo_mif, 0x62}, ODT_END};

HAOS_OdtEntry_t *Echo_odtPtr = Echo_odt;

static int32_t tap_delay_n[MAX_NUM_CHANNELS][ECHO_MAX_NTAP];

static ae_int32x2 cb_values[MAX_NUM_CHANNELS][ECHO_CB_PAIRS];
static ae_int32x2 *cb_current[MAX_NUM_CHANNELS];

static inline void cb_prepare(const size_t ch) {
  AE_SETCBEGIN0(cb_values[ch]);
  AE_SETCEND0(cb_values[ch] + ECHO_CB_PAIRS);
}

static inline void cb_increment(const size_t ch, const size_t s) {
  AE_ADDCIRC32X2_XC(cb_current[ch], +(s & 1));
}

static inline void cb_write(const size_t ch, const size_t s,
                            const ae_f32 sample) {
  const ae_f32x2 pair = AE_F32x2(0, sample);
  AE_S32_L_I(*(ae_int32x2 *)&pair, ((ae_int32 *)cb_current[ch]) + (s & 1), 0);
}

static inline ae_f32 cb_read(const size_t ch, const int32_t delay) {
  ae_int32x2 *cb_target = cb_current[ch];
  AE_ADDCIRC32X2_XC(cb_target, -(delay / 2));
  const ae_int32x2 pair = AE_L32_I((ae_int32 *)cb_target, 0);
  return (*(ae_f32x2 *)&pair)[1 - (delay & 1)]; /////// ovo ne moze?
}

static inline void render_echo(const size_t ch, const size_t s,
                               ae_f32 *const output) {
  ae_f64 echo_sum_acc = 0;
  for (size_t t = 0; t < Echo_mcv.tap_count[ch]; ++t) {
    const ae_f32 gain = Echo_mcv.tap_gain[ch][t];
    const int32_t delay = tap_delay_n[ch][t];
    const ae_f32 tap = cb_read(ch, delay);
    AE_MULAF32S_LL(echo_sum_acc, tap, gain);
  }
  const ae_f32 echo_sum = AE_MOVF32_FROMF64(echo_sum_acc);
  *output = AE_F32X2_MULF_F32X2(echo_sum, Echo_mcv.output_gain);
}

void __fg_call Echo_postKickFunction() {
  std::cout << "start postKick ECHO" << std::endl;

  // calculate microseconds per sample
  const int32_t sample_us = (1.0 / (double)__HAOS_PPM_INPUT_FS) * 1000000.0;
  for (size_t ch = 0; ch < MAX_NUM_CHANNELS; ++ch) {
    // initialize circular buffer values
    for (size_t s = 0; s < ECHO_CB_PAIRS; ++s) {
      cb_values[ch / 2][s] = AE_INT32x2(0, 0);
    }
    // initialize circular buffer pointer
    cb_current[ch] = cb_values[ch];
    // set unused taps gain to zero
    for (size_t t = Echo_mcv.tap_count[ch]; t < ECHO_MAX_NTAP; ++t) {
      Echo_mcv.tap_gain[ch][t] = 0x00000000;
    }
    // calculate tap delays
    for (size_t t = 0; t < Echo_mcv.tap_count[ch]; ++t) {
      tap_delay_n[ch][t] = Echo_mcv.tap_delay_us[ch][t] / sample_us;
    }
  }

  std::cout << "end postKick ECHO" << std::endl;
}

void __fg_call Echo_brickFunction() {
  std::cout << "start brick ECHO" << std::endl;

  if (!Echo_mcv.enable) {
    return;
  }

  for (size_t ch = 0; ch < MAX_NUM_CHANNELS; ++ch) {
    if ((__HAOS_PPM_VALID_CHANNELS & (1 << ch)) == 0) {
      continue;
    }
    cb_prepare(ch);
    for (size_t s = 0; s < BRICK_SIZE; ++s) {
      cb_write(ch, s, __HAOS_IOBUFFER_PTRS[ch][s]);
      render_echo(ch, s, &__HAOS_IOBUFFER_PTRS[ch][s]);
      cb_increment(ch, s);
    }
  }

  std::cout << "end brick ECHO" << std::endl;
}
