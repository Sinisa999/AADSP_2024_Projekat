#include "xtensa/tie/ae_intrinsics.hpp"
#include "xtensa/tie/ae_vector.hpp"

static inline ae_f64 ae_f64_saturate(const ae_f64 value, const double range) {
  const double inner = (double)value;
  const double minimum = -range;
  const double maximum = range;
  if (inner < minimum) {
    return ae_f64(minimum);
  } else if (inner > maximum) {
    return ae_f64(maximum);
  } else {
    return value;
  }
}

ae_int32 AE_SRLA32(ae_int32 d0, int32_t a0) {
  return (a0 >= 0) ? (d0 >> a0) : (d0 << -a0);
}

ae_int32x2 AE_SRLA32(ae_int32x2 d0, int32_t a0) {
  return ae_int32x2(AE_SRLA32(d0[AE_DR_H], a0), AE_SRLA32(d0[AE_DR_L], a0));
}

ae_int64 AE_SRLA64(ae_int64 d0, int32_t a0) {
  return (a0 >= 0) ? (d0 >> a0) : (d0 << -a0);
}

ae_f64 AE_CVT48F32_L(ae_f32 d0) { return ae_f64((double)d0); }

ae_f64 AE_CVT48F32_L(ae_f32x2 d0) { return ae_f64((double)d0[AE_DR_L]); }

ae_f64 AE_CVT48F32_H(ae_f32x2 d0) { return ae_f64((double)d0[AE_DR_H]); }

ae_int32 AE_MOVINT32_FROMINT64(ae_int64 i) { return i & UINT64_C(0xffffffff); }

ae_f32 AE_MOVF32_FROMINT32(ae_int32 i) { return ae_f32((s131_t)i); }

ae_f24 AE_MOVF24_FROMF32(ae_f32 i) { return ae_f24((double)i); }

ae_f24 AE_MOVF24_FROMF64(ae_f64 i) { return ae_f24((double)i); }

ae_f32 AE_MOVF32_FROMF24(ae_f24 i) { return ae_f32((double)i); }

ae_f32 AE_MOVF32_FROMF64(ae_f64 i) { return ae_f32((double)i); }

ae_f64 AE_MOVF64_FROMF24(ae_f24 i) { return ae_f64((double)i); }

ae_f64 AE_MOVF64_FROMF32(ae_f32 i) { return ae_f64((double)i); }

ae_int32 AE_MAX32(ae_int32 d0, ae_int32 d1) { return (d0 > d1) ? d0 : d1; }

ae_int32x2 AE_MAX32(ae_int32x2 d0, ae_int32x2 d1) {
  return ae_int32x2(AE_MAX32(d0[AE_DR_H], d1[AE_DR_H]),
                    AE_MAX32(d0[AE_DR_L], d1[AE_DR_L]));
}

ae_int32 AE_MIN32(ae_int32 d0, ae_int32 d1) { return (d0 < d1) ? d0 : d1; }

ae_int32x2 AE_MIN32(ae_int32x2 d0, ae_int32x2 d1) {
  return ae_int32x2(AE_MIN32(d0[AE_DR_H], d1[AE_DR_H]),
                    AE_MIN32(d0[AE_DR_L], d1[AE_DR_L]));
}

ae_int32 AE_ABS32(ae_int32 d0) {
  const int32_t d = (int32_t)d0;
  return (d >= 0) ? d : -d;
}

ae_int32x2 AE_ABS32(ae_int32x2 d0) {
  return ae_int32x2(AE_ABS32(d0[AE_DR_H]), AE_ABS32(d0[AE_DR_L]));
}

ae_f32 AE_ABS32S(ae_f32 d0) {
  return ((double)d0 >= 0) ? d0 : ae_f32(-1.0 * (double)d0);
}

ae_f32x2 AE_ABS32S(ae_f32x2 d0) {
  return ae_f32x2(AE_ABS32S(d0[AE_DR_H]), AE_ABS32S(d0[AE_DR_L]));
}

ae_f64 AE_ABS64S(ae_f64 d0) {
  return ((double)d0 >= 0) ? d0 : ae_f64(-1.0 * (double)d0);
}

ae_f32 AE_F32X2_NEGS(ae_f32 d0) { return ae_f32(-1.0 * (double)d0); }

ae_f32x2 AE_F32X2_NEGS(ae_f32x2 d0) {
  return ae_f32x2(AE_F32X2_NEGS(d0[AE_DR_H]), AE_F32X2_NEGS(d0[AE_DR_L]));
}

ae_f32 AE_F32X2_ADDS_F32(ae_f32 d0, ae_f32 d1) {
  return ae_f32((double)d0 + (double)d1);
}

ae_f32x2 AE_F32X2_ADDS_F32(ae_f32x2 d0, ae_f32 d1) {
  return ae_f32x2(AE_F32X2_ADDS_F32(d0[AE_DR_H], d1),
                  AE_F32X2_ADDS_F32(d0[AE_DR_L], d1));
}

ae_f32 AE_F32X2_ADDS_F32X2(ae_f32 d0, ae_f32 d1) {
  return ae_f32((double)d0 + (double)d1);
}

ae_f32x2 AE_F32X2_ADDS_F32X2(ae_f32x2 d0, ae_f32x2 d1) {
  return ae_f32x2(AE_F32X2_ADDS_F32X2(d0[AE_DR_H], d1[AE_DR_H]),
                  AE_F32X2_ADDS_F32X2(d0[AE_DR_L], d1[AE_DR_L]));
}

ae_f32 AE_F32X2_SUBS_F32(ae_f32 d0, ae_f32 d1) {
  return ae_f32((double)d0 - (double)d1);
}

ae_f32x2 AE_F32X2_SUBS_F32(ae_f32x2 d0, ae_f32 d1) {
  return ae_f32x2(AE_F32X2_SUBS_F32(d0[AE_DR_H], d1),
                  AE_F32X2_SUBS_F32(d0[AE_DR_L], d1));
}

ae_f32 AE_F32X2_SUBS_F32X2(ae_f32 d0, ae_f32 d1) {
  return ae_f32((double)d0 - (double)d1);
}

ae_f32x2 AE_F32X2_SUBS_F32X2(ae_f32x2 d0, ae_f32x2 d1) {
  return ae_f32x2(AE_F32X2_SUBS_F32X2(d0[AE_DR_H], d1[AE_DR_H]),
                  AE_F32X2_SUBS_F32X2(d0[AE_DR_L], d1[AE_DR_L]));
}

ae_f32 AE_F32X2_MULF_F32X2(ae_f32 d0, ae_f32 d1) {
  return ae_f32((double)d0 * (double)d1);
}

ae_f32x2 AE_F32X2_MULF_F32X2(ae_f32x2 d0, ae_f32x2 d1) {
  return ae_f32x2(AE_F32X2_MULF_F32X2(d0[AE_DR_H], d1[AE_DR_H]),
                  AE_F32X2_MULF_F32X2(d0[AE_DR_L], d1[AE_DR_L]));
}

xtbool AE_F32X2_EQ_F32X2(ae_f32 d0, ae_f32 d1) { return d0 == d1; }

xtbool2 AE_F32X2_EQ_F32X2(ae_f32x2 d0, ae_f32x2 d1) {
  return xtbool2(AE_F32X2_EQ_F32X2(d0[AE_DR_H], d1[AE_DR_H]),
                 AE_F32X2_EQ_F32X2(d0[AE_DR_L], d1[AE_DR_L]));
}

xtbool AE_F32X2_GE_F32X2(ae_f32 d0, ae_f32 d1) { return d0 >= d1; }

xtbool2 AE_F32X2_GE_F32X2(ae_f32x2 d0, ae_f32x2 d1) {
  return xtbool2(AE_F32X2_GE_F32X2(d0[AE_DR_H], d1[AE_DR_H]),
                 AE_F32X2_GE_F32X2(d0[AE_DR_L], d1[AE_DR_L]));
}

xtbool AE_F32X2_GT_F32X2(ae_f32 d0, ae_f32 d1) { return d0 > d1; }

xtbool2 AE_F32X2_GT_F32X2(ae_f32x2 d0, ae_f32x2 d1) {
  return xtbool2(AE_F32X2_GT_F32X2(d0[AE_DR_H], d1[AE_DR_H]),
                 AE_F32X2_GT_F32X2(d0[AE_DR_L], d1[AE_DR_L]));
}

xtbool AE_F32X2_LE_F32X2(ae_f32 d0, ae_f32 d1) { return d0 <= d1; }

xtbool2 AE_F32X2_LE_F32X2(ae_f32x2 d0, ae_f32x2 d1) {
  return xtbool2(AE_F32X2_LE_F32X2(d0[AE_DR_H], d1[AE_DR_H]),
                 AE_F32X2_LE_F32X2(d0[AE_DR_L], d1[AE_DR_L]));
}

xtbool AE_F32X2_LT_F32X2(ae_f32 d0, ae_f32 d1) { return d0 < d1; }

xtbool2 AE_F32X2_LT_F32X2(ae_f32x2 d0, ae_f32x2 d1) {
  return xtbool2(AE_F32X2_LT_F32X2(d0[AE_DR_H], d1[AE_DR_H]),
                 AE_F32X2_LT_F32X2(d0[AE_DR_L], d1[AE_DR_L]));
}

ae_f64 AE_F64_NEGS(ae_f64 d0) { return ae_f64(-1.0 * (double)d0); }

ae_f64 AE_F64_ADDS(ae_f64 d0, ae_f64 d1) {
  return ae_f64((double)d0 + (double)d1);
}

ae_f64 AE_F64_SUBS(ae_f64 d0, ae_f64 d1) {
  return ae_f64((double)d0 - (double)d1);
}

xtbool AE_F64_EQ(ae_f64 d0, ae_f64 d1) { return d0 == d1; }

xtbool AE_F64_GE(ae_f64 d0, ae_f64 d1) { return d0 >= d1; }

xtbool AE_F64_GT(ae_f64 d0, ae_f64 d1) { return d0 > d1; }

xtbool AE_F64_LE(ae_f64 d0, ae_f64 d1) { return d0 <= d1; }

xtbool AE_F64_LT(ae_f64 d0, ae_f64 d1) { return d0 < d1; }

void AE_MULAF32S_LL(ae_f64 &d, ae_f32 d0, ae_f32 d1) {
  d += ae_f64_saturate((double)d0 * (double)d1, 1.0);
}

void AE_MULAF32S_LL(ae_f64 &d, ae_f32x2 d0, ae_f32x2 d1) {
  AE_MULAF32S_LL(d, d0[AE_DR_L], d1[AE_DR_L]);
}

void AE_MULAF32S_HH(ae_f64 &d, ae_f32x2 d0, ae_f32x2 d1) {
  AE_MULAF32S_LL(d, d0[AE_DR_H], d1[AE_DR_H]);
}

void AE_MULAF32R_LL(ae_f64 &d, ae_f32 d0, ae_f32 d1) {
  d += (double)d0 * double(d1);
}

void AE_MULAF32R_LL(ae_f64 &d, ae_f32x2 d0, ae_f32x2 d1) {
  AE_MULAF32R_LL(d, d0[AE_DR_L], d1[AE_DR_L]);
}

void AE_MULAF32R_HH(ae_f64 &d, ae_f32x2 d0, ae_f32x2 d1) {
  AE_MULAF32R_LL(d, d0[AE_DR_H], d1[AE_DR_H]);
}

void AE_S32_L_I(ae_int32 d, ae_int32 *a, int32_t off) { *(a + off) = d; }

void AE_S32_L_I(ae_int32x2 d, ae_int32 *a, int32_t off) {
  *(a + off) = d[AE_DR_L];
}

ae_int32x2 AE_L32_I(const ae_int32 *a, int32_t off) {
  return ae_int32x2(*(a + off), *(a + off));
}

ae_int32x2 AE_L32X2_I(const ae_int32 *a, int64_t off) {
  return ae_int32x2(*(a + off));
}

ae_int32x2 AE_L32X2_I(const ae_int32x2 *a, int64_t off) {
  return ae_int32x2(*(a + off));
}

static void *AE_CBEGIN0 = nullptr;
static void *AE_CBEGIN1 = nullptr;
static void *AE_CEND0 = nullptr;
static void *AE_CEND1 = nullptr;

void AE_SETCBEGIN0(const void *a) { AE_CBEGIN0 = (void *)a; }

void AE_SETCBEGIN1(const void *a) { AE_CBEGIN1 = (void *)a; }

void AE_SETCEND0(const void *a) { AE_CEND0 = (void *)a; }

void AE_SETCEND1(const void *a) { AE_CEND1 = (void *)a; }

void *AE_GETCBEGIN0(void) { return AE_CBEGIN0; }

void *AE_GETCBEGIN1(void) { return AE_CBEGIN1; }

void *AE_GETCEND0(void) { return AE_CEND0; }

void *AE_GETCEND1(void) { return AE_CEND1; }

void AE_ADDCIRC32X2_XC(ae_int32x2 *&a, int32_t x) {
  const ae_int32x2 *a_original = a;
  a = a + x;
  if ((x > 0) && (a_original < AE_CEND0) && (a >= AE_CEND0)) {
    a = a + ((ae_int32x2 *)AE_CBEGIN0 - (ae_int32x2 *)AE_CEND0);
  }
  if ((x < 0) && (a_original >= AE_CBEGIN0) && (a < AE_CBEGIN0)) {
    a = a + ((ae_int32x2 *)AE_CEND0 - (ae_int32x2 *)AE_CBEGIN0);
  }
}

void AE_ADDCIRC32X2_XC1(ae_int32x2 *&a, int32_t x) {
  const ae_int32x2 *a_original = a;
  a = a + x;
  if ((x > 0) && (a_original < AE_CEND1) && (a >= AE_CEND1)) {
    a = a + ((ae_int32x2 *)AE_CBEGIN1 - (ae_int32x2 *)AE_CEND1);
  }
  if ((x < 0) && (a_original >= AE_CBEGIN1) && (a < AE_CBEGIN1)) {
    a = a + ((ae_int32x2 *)AE_CEND1 - (ae_int32x2 *)AE_CBEGIN1);
  }
}

// ae_valign AE_LA64_PP(void *a) {
//   const size_t address = (size_t)(a) & (SIZE_MAX ^ 0x7);
//   return *(ae_valign *)address;
// }

// void AE_LA24X2_IP(ae_int24x2 &d, ae_valign uu, const void *a) {
//   const ae_int24 *values = (ae_int24 *)a;
//   d[AE_DR_H] = values[AE_DR_H];
//   d[AE_DR_L] = values[AE_DR_L];
// }

// void AE_LP24F_IU(ae_p24x2s &d, const ae_p24f *a, immediate i32) {
//   d[AE_DR_H] = *a;
//   d[AE_DR_L] = *a;
// }

// void AE_SP24F_L_IU(ae_p24x2s d, ae_p24f *a, immediate i32) { *a = d[AE_DR_L];
// }

// static inline ae_q56s sign_extend_q56s(const ae_q56s value) {
//   const uint64_t mask = (UINT64_MAX >> (UINT64_WIDTH - 55)) << 55;
//   const uint64_t sign = (value >> 55) & UINT64_C(1);
//   const uint64_t ext = sign * mask;
//   return value | ext;
// }

// static inline ae_q56s saturate_q56s(const ae_q56s value) {
//   const uint64_t mask = UINT64_C(0xff) << 56;
//   const uint64_t sign = (value >> 55) & UINT64_C(1);
//   if ((value & mask) != 0) {
//     return (sign == 1) ? INT56_MIN : INT56_MAX;
//   } else {
//     return value;
//   }
// }

// ae_q56s AE_SLAASQ56S(ae_q56s d0, int32_t a0) {
//   const uint64_t d = d0 & UINT64_C(0x00ffffffffffffff);
//   return sign_extend_q56s((a0 >= 0) ? saturate_q56s(d << a0) : d >> -a0);
// }

// ae_q56s AE_ROUNDSQ32SYM(ae_q56s d0) {
//   //   Round symmetrically (away from 0), saturate the 17.47-bit value from
//   //   AE_DR register d0 to a 1.31-bit value, sign-
//   // extend it and store the result as 17.47-bit value in AE_DR register d.
//   In
//   // case of saturation, state AE_OVERFLOW is set to 1.

//   const uint64_t exponent = (d0 >> 47);
//   const uint64_t mantissa = d0 & (UINT64_MAX >> 17);
//   // const uint64_t d =

//   // uint64_t dr =

//   //
//   return 0;
// }

// ae_p24x2s AE_SUBSP24S(ae_p24x2s d0, ae_int24x2 d1) {

//   const double d0_H = d0[AE_DR_H];
//   const double d0_L = d0[AE_DR_L];
//   const double d1_H = d1[AE_DR_H];
//   const double d1_L = d1[AE_DR_L];

//   // d.H ← sext9 .23(saturate1 .23(d0.H9 .23 ± d1.H9 .23))
//   // d.L ← sext9 .23(saturate1 .23(d0.L9 .23 ± d1.L9 .23))
//   return ae_p24x2s(0, 0);
// }

// ae_p24x2s AE_ADDSP24S(ae_p24x2s d0, ae_int24x2 d1) {
//   //
//   return ae_p24x2s(0, 0);
// }

// ae_q56s AE_ADDQ56(ae_q56s d0, ae_q56s d1) { return d0 + d1; }

// ae_q56s AE_SUBQ56(ae_q56s d0, ae_q56s d1) { return d0 - d1; }

// ae_q56s AE_MULP24S_LL(ae_p24x2s d0, ae_p24x2s d1);

// // AE_MULP24S_HH

// void AE_MULAFS56P24S_LL(ae_q56s q, ae_p24x2s d0, ae_p24x2s d1);

// // AE_MULAFS56P24S_HH

// static void *AE_CBEGIN0 = nullptr;
// static void *AE_CEND0 = nullptr;

// void AE_SETCBEGIN0(const void *addr) { AE_CBEGIN0 = (void *)addr; }

// void AE_SETCEND0(const void *addr) { AE_CEND0 = (void *)addr; }

// void AE_ADDCIRC64_XC1(ae_int64 *a /* inout */, int x) {
//   const ae_int64 *a_original = a;
//   a = a + x;
//   if ((x > 0) && (a_original < AE_CEND0) && (a >= AE_CEND0)) {
//     a = a + (AE_CBEGIN0 - AE_CEND0);
//   } else if ((x < 0) && (a_original >= AE_CBEGIN0) && (a < AE_CEND0)) {
//     a = a + (AE_CEND0 - AE_CBEGIN0);
//   }
// }

// void AE_ADDCIRC32X2_XC1(ae_int32x2 *a /* inout */, int32 x);
// void AE_ADDCIRC16X4_XC1(ae_int16x4 *a /* inout */, int32 x);
