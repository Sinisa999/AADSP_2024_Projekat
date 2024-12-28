#ifndef __AE_INTRINSICS_H__
#define __AE_INTRINSICS_H__

#include <stdint.h>
#include <unistd.h>

#include "ae_f24.hpp"
#include "ae_f32.hpp"
#include "ae_f64.hpp"
#include "ae_int32.hpp"
#include "ae_int64.hpp"
#include "xtbool.hpp"

// #define UINT56_C(c) ((uint64_t)c & UINT64_C(0x00ffffffffffffff))

// #define UINT56_WIDTH (64)
// #define UINT56_MIN (0)
// #define UINT56_MAX UINT64_C(72057594037927935)

// #define INT56_C(c)                                                             \
//   ((int64_t)(((uint64_t)c & UINT64_C(0x80ffffffffffffff)) |                    \
//              (((uint64_t)c >> 63) * UINT64_C(0xff00000000000000))))

// #define INT56_WIDTH (64)
// #define INT56_MIN UINT64_C(-36028797018963968)
// #define INT56_MAX UINT64_C(36028797018963967)

// #define AE_INT56_C(c) UINT56_C(UINT64_C(c))

// typedef uint64_t ae_valign;
// typedef ssize_t immediate;
// typedef ae_f24x2 ae_p24x2s;
// typedef ae_f24 ae_p24f;
// typedef ae_int64 ae_q56s;

// 2-way bidirectional shift, right (logical zero-extending) by AR register
ae_int32 AE_SRLA32(ae_int32, int32_t);
ae_int32x2 AE_SRLA32(ae_int32x2, int32_t);

// 64-bit logical bidirectional shift right by AR register
ae_int64 AE_SRLA64(ae_int64, int32_t);

// Convert a 1.31 to 17.47
ae_f64 AE_CVT48F32_L(ae_f32);
ae_f64 AE_CVT48F32_L(ae_f32x2);
ae_f64 AE_CVT48F32_H(ae_f32x2);

// Copy AE_DR register
ae_int32 AE_MOVINT32_FROMINT64(ae_int64);
ae_f32 AE_MOVF32_FROMINT32(ae_int32);
ae_f24 AE_MOVF24_FROMF32(ae_f32);
ae_f24 AE_MOVF24_FROMF64(ae_f64);
ae_f32 AE_MOVF32_FROMF24(ae_f24);
ae_f32 AE_MOVF32_FROMF64(ae_f64);
ae_f64 AE_MOVF64_FROMF24(ae_f24);
ae_f64 AE_MOVF64_FROMF32(ae_f32);

// 2-way 32-bit maximum
ae_int32 AE_MAX32(ae_int32, ae_int32);
ae_int32x2 AE_MAX32(ae_int32x2, ae_int32x2);

// 2-way 32-bit minimum
ae_int32 AE_MIN32(ae_int32, ae_int32);
ae_int32x2 AE_MIN32(ae_int32x2, ae_int32x2);

// 2-way 32-bit absolute value
ae_int32 AE_ABS32(ae_int32);
ae_int32x2 AE_ABS32(ae_int32x2);

// 2-way 32-bit absolute value with saturation
ae_f32 AE_ABS32S(ae_f32);
ae_f32x2 AE_ABS32S(ae_f32x2);

// 64-bit absolute value with saturation
ae_f64 AE_ABS64S(ae_f64);

// 2-way 32-bit negation with saturation
ae_f32 AE_F32X2_NEGS(ae_f32);
ae_f32x2 AE_F32X2_NEGS(ae_f32x2);

// 2-way 32-bit signed, saturating addition
ae_f32 AE_F32X2_ADDS_F32(ae_f32, ae_f32);
ae_f32x2 AE_F32X2_ADDS_F32(ae_f32x2, ae_f32);
ae_f32 AE_F32X2_ADDS_F32X2(ae_f32, ae_f32);
ae_f32x2 AE_F32X2_ADDS_F32X2(ae_f32x2, ae_f32x2);

// 2-way 32-bit signed, saturating subtraction
ae_f32 AE_F32X2_SUBS_F32(ae_f32, ae_f32);
ae_f32x2 AE_F32X2_SUBS_F32(ae_f32x2, ae_f32);
ae_f32 AE_F32X2_SUBS_F32X2(ae_f32, ae_f32);
ae_f32x2 AE_F32X2_SUBS_F32X2(ae_f32x2, ae_f32x2);

// Two-way SIMD 32x32-bit signed fractional (1.31) multiply
// with asymmetric rounding down to 1.31-bits
ae_f32 AE_F32X2_MULF_F32X2(ae_f32, ae_f32);
ae_f32x2 AE_F32X2_MULF_F32X2(ae_f32x2, ae_f32x2);

// 2-way 32-bit equality comparison
xtbool AE_F32X2_EQ_F32X2(ae_f32, ae_f32);
xtbool2 AE_F32X2_EQ_F32X2(ae_f32x2, ae_f32x2);

// 2-way 32-bit signed greater-than-or-equal comparison
xtbool AE_F32X2_GE_F32X2(ae_f32, ae_f32);
xtbool2 AE_F32X2_GE_F32X2(ae_f32x2, ae_f32x2);

// 2-way 32-bit signed greater-than comparison
xtbool AE_F32X2_GT_F32X2(ae_f32, ae_f32);
xtbool2 AE_F32X2_GT_F32X2(ae_f32x2, ae_f32x2);

// 2-way 32-bit signed less-than-or-equal comparison
xtbool AE_F32X2_LE_F32X2(ae_f32, ae_f32);
xtbool2 AE_F32X2_LE_F32X2(ae_f32x2, ae_f32x2);

// 2-way 32-bit signed less-than comparison
xtbool AE_F32X2_LT_F32X2(ae_f32, ae_f32);
xtbool2 AE_F32X2_LT_F32X2(ae_f32x2, ae_f32x2);

// 64-bit negation with saturation
ae_f64 AE_F64_NEGS(ae_f64);

// 64-bit signed addition with saturation
ae_f64 AE_F64_ADDS(ae_f64, ae_f64);

// 64-bit signed subtraction with saturation
ae_f64 AE_F64_SUBS(ae_f64, ae_f64);

// 64-bit signed equality
xtbool AE_F64_EQ(ae_f64, ae_f64);

// 64-bit signed greater-than-or-equal comparison
xtbool AE_F64_GE(ae_f64, ae_f64);

// 64-bit signed greater-than comparison
xtbool AE_F64_GT(ae_f64, ae_f64);

// 64-bit signed less-than-or-equal comparison
xtbool AE_F64_LE(ae_f64, ae_f64);

// 64-bit signed less-than comparison
xtbool AE_F64_LT(ae_f64, ae_f64);

// 32x32-bit signed fractional (1.31) MAC with 64-bit (1.63) result,
//  with saturation
void AE_MULAF32S_LL(ae_f64 &, ae_f32, ae_f32);
void AE_MULAF32S_LL(ae_f64 &, ae_f32x2, ae_f32x2);
void AE_MULAF32S_HH(ae_f64 &, ae_f32x2, ae_f32x2);

// 32x32-bit signed fractional (1.31) MAC
// with symmetric rounding down to 17.47-bits
void AE_MULAF32R_LL(ae_f64 &, ae_f32, ae_f32);
void AE_MULAF32R_LL(ae_f64 &, ae_f32x2, ae_f32x2);
void AE_MULAF32R_HH(ae_f64 &, ae_f32x2, ae_f32x2);

// Store a 32-bit value from the L entry of the AE_DR register
void AE_S32_L_I(ae_int32, ae_int32 *, int32_t);
void AE_S32_L_I(ae_int32x2, ae_int32 *, int32_t);

// Load a 32-bit value and replicate into the AE_DR register
ae_int32x2 AE_L32_I(const ae_int32 *, int32_t);

// Load two 32-bit values into the AE_DR register
ae_int32x2 AE_L32X2_I(const ae_int32 *, int64_t);
ae_int32x2 AE_L32X2_I(const ae_int32x2 *, int64_t);

// Write User Register CBEGIN0
void AE_SETCBEGIN0(const void *);

// Write User Register CBEGIN1
void AE_SETCBEGIN1(const void *);

// Write User Register CEND0
void AE_SETCEND0(const void *);

// Write User Register CEND1
void AE_SETCEND1(const void *);

// Read User Register CBEGIN0
void *AE_GETCBEGIN0(void);

// Read User Register CBEGIN1
void *AE_GETCBEGIN1(void);

// Read User Register CEND0
void *AE_GETCEND0(void);

// Read User Register CEND1
void *AE_GETCEND1(void);

// Circular addition
void AE_ADDCIRC32X2_XC(ae_int32x2 *&, int32_t);
void AE_ADDCIRC32X2_XC1(ae_int32x2 *&, int32_t);

// ae_valign AE_LA64_PP(void *);
// void AE_LA24X2_IP(ae_int24x2 &, ae_valign, const void *);
// void AE_LP24F_IU(ae_p24x2s &, const ae_p24f *, immediate);
// void AE_SP24F_L_IU(ae_p24x2s, ae_p24f *, immediate);

// ae_q56s AE_SLAASQ56S(ae_q56s, int32_t);

// ae_q56s AE_ROUNDSQ32SYM(ae_q56s);

// ae_p24x2s AE_SUBSP24S(ae_p24x2s, ae_int24x2);
// ae_p24x2s AE_ADDSP24S(ae_p24x2s, ae_int24x2);
// ae_q56s AE_ADDQ56(ae_q56s, ae_q56s);
// ae_q56s AE_SUBQ56(ae_q56s, ae_q56s);

#endif /* __AE_INTRINSICS_H__ */
