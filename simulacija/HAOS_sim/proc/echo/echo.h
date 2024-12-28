/*
 * echo.h
 *
 *  Created on: Dec 2, 2024
 *      Author: Filip Parag
 */

#ifndef __ECHO_H__
#define __ECHO_H__

#define __fg_call

#define ECHO_MAX_NTAP (16)

#define ECHO_MAX_FS (48000.0)
#define ECHO_MAX_DELAY_S (1.0)

#define ECHO_CB_SAMPLES ((int)(ECHO_MAX_FS * ECHO_MAX_DELAY_S))
#define ECHO_CB_PAIRS ((ECHO_CB_SAMPLES + 1) / 2)

void __fg_call Echo_postKickFunction();
void __fg_call Echo_brickFunction();

#endif /* __ECHO_H__ */
