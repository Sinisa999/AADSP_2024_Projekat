#ifndef __AE_INT24_H__
#define __AE_INT24_H__

#include <iostream>
#include <stdint.h>

#define UINT24_C(c) ((uint32_t)c & UINT32_C(0x00ffffff))

#define UINT24_WIDTH (32)
#define UINT24_MIN (0)
#define UINT24_MAX UINT32_C(16777215)

#define INT24_C(c)                                                             \
  ((int32_t)(((uint32_t)c & UINT32_C(0x80ffffff)) |                            \
             (((uint32_t)c >> 31) * UINT32_C(0xff000000))))

#define INT24_WIDTH (32)
#define INT24_MIN INT32_C(-8388608)
#define INT24_MAX INT32_C(8388607)

#define AE_INT24_C(c) UINT24_C(UINT32_C(c))

class ae_int24 {
public:
  ae_int24(const uint32_t value = 0);
  operator int() const;
  ae_int24 operator+(const ae_int24 &other) const;
  ae_int24 operator-(const ae_int24 &other) const;
  ae_int24 operator*(const ae_int24 &other) const;
  ae_int24 operator+(const unsigned int &other) const;
  ae_int24 operator-(const unsigned int &other) const;
  ae_int24 operator*(const unsigned int &other) const;
  ae_int24 &operator+=(const ae_int24 &other);
  ae_int24 &operator-=(const ae_int24 &other);
  ae_int24 &operator*=(const ae_int24 &other);
  ae_int24 &operator+=(const unsigned int &other);
  ae_int24 &operator-=(const unsigned int &other);
  ae_int24 &operator*=(const unsigned int &other);
  bool operator==(const ae_int24 &other) const;
  bool operator!=(const ae_int24 &other) const;
  bool operator<(const ae_int24 &other) const;
  bool operator>(const ae_int24 &other) const;
  bool operator<=(const ae_int24 &other) const;
  bool operator>=(const ae_int24 &other) const;
  bool operator==(const unsigned int &other) const;
  bool operator!=(const unsigned int &other) const;
  bool operator<(const unsigned int &other) const;
  bool operator>(const unsigned int &other) const;
  bool operator<=(const unsigned int &other) const;
  bool operator>=(const unsigned int &other) const;
  friend std::istream &operator>>(std::istream &is, ae_int24 &obj);
  friend std::ostream &operator<<(std::ostream &os, const ae_int24 &obj);

private:
  void saturate();
  uint32_t value;
};

class ae_int24x2 {
public:
  ae_int24x2(const ae_int24 lo = 0);
  ae_int24x2(const ae_int24 hi, const ae_int24 lo);
  ae_int24 &operator[](size_t index);
  const ae_int24 &operator[](size_t index) const;
  friend std::ostream &operator<<(std::ostream &os, const ae_int24x2 &obj);

private:
  ae_int24 hi;
  ae_int24 lo;
};

#define AE_INT24x2(hi, lo) ae_int24x2(hi, lo)

#endif /* __AE_INT24_H__ */
