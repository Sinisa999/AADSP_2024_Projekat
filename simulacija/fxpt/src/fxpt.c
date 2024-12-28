#ifdef NSATURATION
#include <assert.h>
#endif
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "fxpt.h"

s131_t d_to_s131(const double value) {
#ifdef NSATURATION
  assert((value <= 1.0) && (value >= -1.0));
#endif
  const static s131_t s131_scale = UINT64_C(1) << 31;
  const static s131_t s131_sign_bit = UINT64_C(1) << 31;
  const static s131_t s131_max = UINT32_C(0x7fffffff); // 1 - 2^-31
  const static s131_t s131_min = UINT32_C(0x80000000); // -1
  const bool saturation_hi = value >= 1.0;
  const bool saturation_lo = value <= -1.0;
  const bool negative = value < 0.0;
  if (saturation_hi) {
    return s131_max;
  } else if (saturation_lo) {
    return s131_min;
  } else {
    if (negative) {
      return (uint32_t)(-1.0L + value * (long double)s131_scale) |
             s131_sign_bit;
    } else {
      return (uint32_t)(value * (long double)s131_scale);
    }
  }
}

#define S131_MAG(bit) [bit] = 1.0 / (double)(INT32_C(1) << (31 - bit))

double s131_to_d(const s131_t value) {
  static const double s131_magnitude[32] = {
      S131_MAG(0),  S131_MAG(1),  S131_MAG(2),  S131_MAG(3),  S131_MAG(4),
      S131_MAG(5),  S131_MAG(6),  S131_MAG(7),  S131_MAG(8),  S131_MAG(9),
      S131_MAG(10), S131_MAG(11), S131_MAG(12), S131_MAG(13), S131_MAG(14),
      S131_MAG(15), S131_MAG(16), S131_MAG(17), S131_MAG(18), S131_MAG(19),
      S131_MAG(20), S131_MAG(21), S131_MAG(22), S131_MAG(23), S131_MAG(24),
      S131_MAG(25), S131_MAG(26), S131_MAG(27), S131_MAG(28), S131_MAG(29),
      S131_MAG(30), [31] = -1.0,
  };
  double output = 0.0;
  for (size_t i = 0; i < 32; ++i) {
    const double bit_value[] = {[0] = 0.0, [1] = s131_magnitude[i]};
    const size_t index = (value >> i) & 1;
    output += bit_value[index];
  }
  return output;
}

s163_t ld_to_s163(const long double value) {
#ifdef NSATURATION
  assert((value <= 1.0L) && (value >= -1.0L));
#endif
  const static s163_t s163_scale = UINT64_C(1) << 63;
  const static s163_t s163_max = UINT64_C(0x7fffffffffffffff); // 1 - 2^-63
  const static s163_t s163_min = UINT64_C(0x8000000000000000); // -1
  const bool saturation_hi = value >= 1.0L;
  const bool saturation_lo = value <= -1.0L;
  const bool negative = value < 0.0L;
  if (saturation_hi) {
    return s163_max;
  } else if (saturation_lo) {
    return s163_min;
  } else {
    if (negative) {
      return (uint64_t)(-1.0L + value * (long double)s163_scale) | s163_scale;
    } else {
      return (uint64_t)(value * (long double)s163_scale);
    }
  }
}

#define S163_MAG(bit) [bit] = 1.0L / (long double)(INT64_C(1) << (63 - bit))

long double s163_to_ld(const s163_t value) {
  static const long double s163_magnitude[64] = {
      S163_MAG(0),  S163_MAG(1),  S163_MAG(2),  S163_MAG(3),  S163_MAG(4),
      S163_MAG(5),  S163_MAG(6),  S163_MAG(7),  S163_MAG(8),  S163_MAG(9),
      S163_MAG(10), S163_MAG(11), S163_MAG(12), S163_MAG(13), S163_MAG(14),
      S163_MAG(15), S163_MAG(16), S163_MAG(17), S163_MAG(18), S163_MAG(19),
      S163_MAG(20), S163_MAG(21), S163_MAG(22), S163_MAG(23), S163_MAG(24),
      S163_MAG(25), S163_MAG(26), S163_MAG(27), S163_MAG(28), S163_MAG(29),
      S163_MAG(30), S163_MAG(31), S163_MAG(32), S163_MAG(33), S163_MAG(34),
      S163_MAG(35), S163_MAG(36), S163_MAG(37), S163_MAG(38), S163_MAG(39),
      S163_MAG(40), S163_MAG(41), S163_MAG(42), S163_MAG(43), S163_MAG(44),
      S163_MAG(45), S163_MAG(46), S163_MAG(47), S163_MAG(48), S163_MAG(49),
      S163_MAG(50), S163_MAG(51), S163_MAG(52), S163_MAG(53), S163_MAG(54),
      S163_MAG(55), S163_MAG(56), S163_MAG(57), S163_MAG(58), S163_MAG(59),
      S163_MAG(60), S163_MAG(61), S163_MAG(62), [63] = -1.0,
  };
  long double output = 0.0L;
  for (size_t i = 0; i < 64; ++i) {
    const long double bit_value[] = {[0] = 0.0L, [1] = s163_magnitude[i]};
    const size_t index = (value >> i) & 1;
    output += bit_value[index];
  }
  return output;
}

// #include <assert.h> ///

// s1747_t d_to_s1747(const double value) {
//   // #ifdef NSATURATION
//   assert((value <= 65536.0) && (value >= -65536.0));
//   // #endif
//   const int64_t value_int =
//       (value >= 0.0) * floor(value) + (value < 0.0) * ceil(value);
//   const uint64_t value_bits = (*(uint64_t *)&value);
//   const uint64_t integer = (*(uint64_t *)&value_int) << 47;
//   const double value_rem = (value >= 0.0) * (value - (double)value_int) +
//                            (value < 0.0) * ((double)value_int - value);

//   const uint64_t remainder =
//       (*(uint64_t *)&value_rem); //& INT64_C(0xfffffffffffff) >> (52 - 47);
//   const uint64_t x = integer | remainder;
//   return 0;
// }

// double s1747_to_d(const s1747_t value) {
//   const uint64_t sign = (value >> 63) * ((UINT64_MAX >> 17) << 17);
//   const uint64_t integer_bits = sign | (value >> 47);
//   const uint64_t remainder_bits =
//       ((value >> 63) << 63) | (UINT64_C(1021) << 52) |
//       (value & UINT64_C(0x7fffffffffff)) << (52 - 47);
//   const double integer = *(int64_t *)&integer_bits;
//   const double remainder = *(double *)&remainder_bits;
//   const double x = integer + remainder;
//   return 0;
// }

s163_t s131_to_s163(const s131_t value) {
  const s163_t round_up = (value & UINT64_C(1)) * UINT64_C(0x77777777);
  return (((uint64_t)value) << 32) | round_up;
}

s131_t s163_to_s131(const s163_t value) {
  const s131_t round_up = (value >> 31) & UINT64_C(1);
  return (value >> 32) + round_up;
}
