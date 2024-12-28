////////////////////////////////////////////////////////////////////////////////
//
// Author.......: Milica Tadic (tmilica)
// Contact......: milica.tadic@rt-rk.com
// Date.........: 24.04.2024.
//
////////////////////////////////////////////////////////////////////////////////

#include "am_sim.h"
#include "haos.h"
#include <iostream>
#include <xtensa/tie/xt_hifi4.h>

#define NO_SOURCE 0x08000000

int nChannels = 0;
ae_f32 AudioManager_gainAndTrim[MAX_NUM_CHANNELS] = {0};

struct {
  ae_f32 gain;
  unsigned int mute;
  ae_f32 channelTrim[16];
  unsigned int channelRemap[16];
  unsigned int reserved[32];
} AudioManager_mcv = {0x7fffffff, // gain
                      0,          // mute - 0 - not muted, 1 - muted
                      {
                          0x7fffffff, // l_trim
                          0x7fffffff, // c_trim
                          0x7fffffff, // r_trim
                          0x7fffffff, // ls_trim
                          0x7fffffff, // rs_trim
                          0x7fffffff, // lb_trim
                          0x7fffffff, // rb_trim
                          0x7fffffff, // lfe0_trim
                          0x7fffffff, // lh_trim
                          0x7fffffff, // rh_trim
                          0x7fffffff, // lw_trim
                          0x7fffffff, // rw_trim
                          0x7fffffff, // lt_trim
                          0x7fffffff, // rt_trim
                          0x7fffffff, // lfe1_trim
                          0x7fffffff, // res_trim
                      },
                      {
                          0,         // IOBUFFER[0] --> output wave channel 0
                          1,         // IOBUFFER[2] --> output wave channel 1
                          2,         // IOBUFFER[1] --> output wave channel 2
                          3,         // IOBUFFER[3] --> output wave channel 3
                          4,         // IOBUFFER[4] --> output wave channel 4
                          5,         // IOBUFFER[5] --> output wave channel 5
                          NO_SOURCE, // IOBUFFER[6] --> output wave channel 6
                          NO_SOURCE, // IOBUFFER[7] --> output wave channel 7
                          NO_SOURCE, // IOBUFFER[8] --> output wave channel 8
                          NO_SOURCE, // IOBUFFER[9] --> output wave channel 9
                          NO_SOURCE, // IOBUFFER[10] --> output wave channel 10
                          NO_SOURCE, // IOBUFFER[11] --> output wave channel 11
                          NO_SOURCE, // IOBUFFER[12] --> output wave channel 12
                          NO_SOURCE, // IOBUFFER[13] --> output wave channel 13
                          NO_SOURCE, // IOBUFFER[14] --> output wave channel 14
                          NO_SOURCE  // IOBUFFER[15] --> output wave channel 15
                      }};

HAOS_Mct_t AudioManager_mct = {0, 0, 0, 0, AudioManager_brickFunction,
                               0, 0, 0, 0};

HAOS_Mif_t AudioManager_mif = {&AudioManager_mcv, &AudioManager_mct};

HAOS_Odt_t AudioManager_odt = {{&AudioManager_mif, 0x60}, ODT_END};

HAOS_OdtEntry_t *AudioManager_odtPtr = AudioManager_odt;

//==============================================================================

void AudioManager_combineGainMuteAndTrims() {
  // apply mute if needed
  ae_f32 gain = AudioManager_mcv.gain;
  if (AudioManager_mcv.mute == 1) {
    gain = 0;
  }

  // combine gain and trims into one buffer
  for (int channel = 0; channel < MAX_NUM_CHANNELS; channel++) {
    AudioManager_gainAndTrim[channel] =
        gain * AudioManager_mcv.channelTrim[channel];
  }
}
//==============================================================================

void __fg_call AudioManager_brickFunction() {
  std::cout << "start brick AM" << std::endl;
  static bool initDone = false;

  if (!initDone) {
    AudioManager_combineGainMuteAndTrims();
    initDone = true;
  }

  int mask = __HAOS_PPM_VALID_CHANNELS;
  int maskOut = 0;
  int samplesToWrite = BRICK_SIZE;
  int iobufferChannel;
  int channel = 0;
  // int inpCh = 0;
  // int currChannel = 0;

  // apply gain, trim and mute and write the result to TEMPBUFFERs
  while (mask != 0) {
    if (mask & 1) {
      for (int sample = 0; sample < samplesToWrite; sample++) {
        __HAOS_TEMPBUFFER_PTRS[channel][sample] =
            __HAOS_IOBUFFER_PTRS[channel][sample] *
            AudioManager_gainAndTrim[channel];
      }
    }
    mask >>= 1;
    channel++;
  }

  // refresh channels mask based on AudioManager MCV
  for (int i = 0; i < MAX_NUM_CHANNELS; i++) {
    if (AudioManager_mcv.channelRemap[i] != NO_SOURCE) {
      maskOut |= 1 << i;
    }
  }
  __HAOS_PPM_VALID_CHANNELS = maskOut;

  channel = 0;
  // apply remap, move samples to IOBUFFERs
  while (maskOut != 0) {
    if (maskOut & 1) {
      iobufferChannel = AudioManager_mcv.channelRemap[channel];
      for (int sample = 0; sample < samplesToWrite; sample++) {
        __HAOS_IOBUFFER_PTRS[channel][sample] =
            __HAOS_TEMPBUFFER_PTRS[iobufferChannel][sample];
      }
      channel++;
    }

    maskOut >>= 1;
  }

  std::cout << "end brick AM" << std::endl;
}
//==============================================================================
