#ifndef __AE_F24_H__
#define __AE_F24_H__

#include <iostream>

#include "ae_int32.hpp"

extern "C" {
#include <fxpt.h>
}

#define AE_F24_C(c) ((s131_t)(UINT32_C(c) & UINT32_C(0xffffff00)))

class ae_f24 {
public:
  ae_f24(const s131_t value = 0);
  ae_f24(const s163_t value);
  ae_f24(const int value);
  ae_f24(const double value);
  operator int() const;
  operator double() const;
  operator ae_int32() const;
  ae_f24 operator+(const ae_f24 &other) const;
  ae_f24 operator-(const ae_f24 &other) const;
  ae_f24 operator*(const ae_f24 &other) const;
  ae_f24 &operator+=(const ae_f24 &other);
  ae_f24 &operator-=(const ae_f24 &other);
  ae_f24 &operator*=(const ae_f24 &other);
  bool operator==(const ae_f24 &other) const;
  bool operator!=(const ae_f24 &other) const;
  bool operator<(const ae_f24 &other) const;
  bool operator>(const ae_f24 &other) const;
  bool operator<=(const ae_f24 &other) const;
  bool operator>=(const ae_f24 &other) const;
  friend std::istream &operator>>(std::istream &is, ae_f24 &obj);
  friend std::ostream &operator<<(std::ostream &os, const ae_f24 &obj);

private:
  void saturate();
  float value;
};

class ae_f24x2 {
public:
  ae_f24x2(const ae_f24 lo = 0);
  ae_f24x2(const ae_f24 hi, const ae_f24 lo);
  ae_f24 &operator[](size_t index);
  const ae_f24 &operator[](size_t index) const;
  friend std::ostream &operator<<(std::ostream &os, const ae_f24x2 &obj);

private:
  ae_f24 hi;
  ae_f24 lo;
};

#define AE_F24x2(hi, lo) ae_f24x2(hi, lo)

#endif /* __AE_F24_H__ */
