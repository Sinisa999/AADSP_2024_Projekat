#ifndef __AE_F64_H__
#define __AE_F64_H__

#include <iostream>

extern "C" {
#include <fxpt.h>
}

#define AE_F64_C(c) ((s163_t)UINT64_C(c))

class ae_f64 {
public:
  ae_f64(const s163_t value = 0);
  ae_f64(const s131_t value);
  ae_f64(const int value);
  ae_f64(const long value);
  ae_f64(const double value);
  operator double() const;
  ae_f64 operator+(const ae_f64 &other) const;
  ae_f64 operator-(const ae_f64 &other) const;
  ae_f64 operator*(const ae_f64 &other) const;
  ae_f64 &operator+=(const ae_f64 &other);
  ae_f64 &operator-=(const ae_f64 &other);
  ae_f64 &operator*=(const ae_f64 &other);
  bool operator==(const ae_f64 &other) const;
  bool operator!=(const ae_f64 &other) const;
  bool operator<(const ae_f64 &other) const;
  bool operator>(const ae_f64 &other) const;
  bool operator<=(const ae_f64 &other) const;
  bool operator>=(const ae_f64 &other) const;
  friend std::istream &operator>>(std::istream &is, ae_f64 &obj);
  friend std::ostream &operator<<(std::ostream &os, const ae_f64 &obj);

private:
  void saturate();
  double value;
};

#endif /* __AE_F64_H__ */
