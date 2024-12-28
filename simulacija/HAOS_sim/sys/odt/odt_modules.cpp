/*
 * odt_modules.h
 *
 *  Created on: Nov 7, 2024
 *      Author: katarinac
 */

#include "haos.h"

extern HAOS_Mif_t Upmixer_mif;
extern HAOS_Mif_t Echo_mif;
extern HAOS_Mif_t Obrada_mif;

HAOS_Odt_t odt = {
    //{&Upmixer_mif, 0x61},
    //{&Echo_mif, 0x62},
    {&Obrada_mif, 0x20},
    ODT_END
    };
