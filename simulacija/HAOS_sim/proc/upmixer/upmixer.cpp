/*
 * upmixer.cpp
 *
 *  Created on: Nov 26, 2024
 *      Author: Filip Parag
 */

#include "upmixer.h"
#include "haos.h"
#include <iostream>
#include <xtensa/tie/xt_hifi4.h>

struct {
  uint32_t enable;
  int32_t in_channels;
  int32_t out_channels;
  ae_f32 mix_coeffs[MAX_NUM_CHANNELS][MAX_NUM_CHANNELS];
} Upmixer_mcv = {true, // enable
                 2,    // in_channels
                 6,    // out_channels
                 {
                     // mix_coeffs
                     {0x7fffffff, 0x00000000}, // Front Left (FL)
                     {0x00000000, 0x7fffffff}, // Front Right (FR)
                     {0x40000000, 0x40000000}, // Front Center (FC)
                     {0x40000000, 0x40000000}, // LFE
                     {0x7fffffff, 0x00000000}, // Back Left (BL)
                     {0x00000000, 0x7fffffff}  // Back Right (BR)
                 }};

HAOS_Mct_t Upmixer_mct = {0, 0, 0, 0, Upmixer_brickFunction, 0, 0, 0, 0};

HAOS_Mif_t Upmixer_mif = {&Upmixer_mcv, &Upmixer_mct};

HAOS_Odt_t Upmixer_odt = {{&Upmixer_mif, 0x61}, ODT_END};

HAOS_OdtEntry_t *Upmixer_odtPtr = Upmixer_odt;

static inline void render_mix(void) {
  for (size_t o_ch = 0; o_ch < Upmixer_mcv.out_channels; ++o_ch) {
    for (size_t s = 0; s < BRICK_SIZE; s++) {
      ae_f64 mix = 0x00000000;
      for (size_t i_ch = 0; i_ch < Upmixer_mcv.in_channels; ++i_ch) {
        const ae_f32 sample = __HAOS_IOBUFFER_PTRS[i_ch][s];
        const ae_f32 coefficient = Upmixer_mcv.mix_coeffs[o_ch][i_ch];
        AE_MULAF32S_LL(mix, sample, coefficient);
      }
      __HAOS_TEMPBUFFER_PTRS[o_ch][s] = AE_MOVF32_FROMF64(mix);
    }
  }
}

static inline void write_mix(void) {
  for (size_t o_ch = 0; o_ch < Upmixer_mcv.out_channels; ++o_ch) {
    for (size_t s = 0; s < BRICK_SIZE; s++) {
      __HAOS_IOBUFFER_PTRS[o_ch][s] = __HAOS_TEMPBUFFER_PTRS[o_ch][s];
    }
  }
}

void __fg_call Upmixer_brickFunction() {
  std::cout << "start brick UPMIX" << std::endl;

  if (!Upmixer_mcv.enable) {
    return;
  }

  render_mix();
  write_mix();

  std::cout << "end brick UPMIX" << std::endl;
}
