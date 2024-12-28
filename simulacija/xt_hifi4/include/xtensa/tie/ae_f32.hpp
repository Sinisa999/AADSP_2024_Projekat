#ifndef __AE_F32_H__
#define __AE_F32_H__

#include <iostream>

#include "ae_int32.hpp"

extern "C" {
#include <fxpt.h>
}

#define AE_F32_C(c) ((s131_t)UINT32_C(c))

class ae_f32 {
public:
  ae_f32(const s131_t value = 0);
  ae_f32(const s163_t value);
  ae_f32(const int value);
  ae_f32(const double value);
  operator int() const;
  operator double() const;
  operator ae_int32() const;
  ae_f32 operator+(const ae_f32 &other) const;
  ae_f32 operator-(const ae_f32 &other) const;
  ae_f32 operator*(const ae_f32 &other) const;
  ae_f32 &operator+=(const ae_f32 &other);
  ae_f32 &operator-=(const ae_f32 &other);
  ae_f32 &operator*=(const ae_f32 &other);
  bool operator==(const ae_f32 &other) const;
  bool operator!=(const ae_f32 &other) const;
  bool operator<(const ae_f32 &other) const;
  bool operator>(const ae_f32 &other) const;
  bool operator<=(const ae_f32 &other) const;
  bool operator>=(const ae_f32 &other) const;
  friend std::istream &operator>>(std::istream &is, ae_f32 &obj);
  friend std::ostream &operator<<(std::ostream &os, const ae_f32 &obj);

private:
  void saturate();
  float value;
};

class ae_f32x2 {
public:
  ae_f32x2(const ae_f32 lo = 0);
  ae_f32x2(const ae_f32 hi, const ae_f32 lo);
  ae_f32x2(const ae_int32x2 pair);
  ae_f32 &operator[](size_t index);
  const ae_f32 &operator[](size_t index) const;
  friend std::ostream &operator<<(std::ostream &os, const ae_f32x2 &obj);

private:
  ae_f32 hi;
  ae_f32 lo;
};

#define AE_F32x2(hi, lo) ae_f32x2(hi, lo)

#endif /* __AE_F32_H__ */
