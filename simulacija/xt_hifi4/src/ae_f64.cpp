#ifdef NSATURATION
#include <assert.h>
#endif
#include <climits>

#include "xtensa/tie/ae_f64.hpp"

#define S163_MIN (-1.0)
#define S163_MAX (1.0)

#define S1747_MIN (-65536.0)
#define S1747_MAX (65536.0)

ae_f64::ae_f64(const s163_t value) : value(s163_to_ld(value)) {}

ae_f64::ae_f64(const s131_t value) : value(s163_to_ld(s131_to_s163(value))) {}

ae_f64::ae_f64(const int value) : value((double)value / (double)INT_MAX) {
  this->saturate();
}

ae_f64::ae_f64(const long value) : value((double)value / (double)LONG_MAX) {
  this->saturate();
}

ae_f64::ae_f64(const double value) : value(value) { this->saturate(); }

ae_f64::operator double() const { return this->value; }

ae_f64 ae_f64::operator+(const ae_f64 &other) const {
  ae_f64 result;
  result.value = this->value + other.value;
  result.saturate();
  return result;
}

ae_f64 ae_f64::operator-(const ae_f64 &other) const {
  ae_f64 result;
  result.value = this->value - other.value;
  result.saturate();
  return result;
}

ae_f64 ae_f64::operator*(const ae_f64 &other) const {
  ae_f64 result;
  result.value = this->value * other.value;
  result.saturate();
  return result;
}

ae_f64 &ae_f64::operator+=(const ae_f64 &other) {
  this->value += other.value;
  this->saturate();
  return *this;
}

ae_f64 &ae_f64::operator-=(const ae_f64 &other) {
  this->value -= other.value;
  this->saturate();
  return *this;
}

ae_f64 &ae_f64::operator*=(const ae_f64 &other) {
  this->value *= other.value;
  this->saturate();
  return *this;
}

bool ae_f64::operator==(const ae_f64 &other) const {
  return this->value == other.value;
}

bool ae_f64::operator!=(const ae_f64 &other) const {
  return this->value != other.value;
}

bool ae_f64::operator<(const ae_f64 &other) const {
  return this->value < other.value;
}

bool ae_f64::operator>(const ae_f64 &other) const {
  return this->value > other.value;
}

bool ae_f64::operator<=(const ae_f64 &other) const {
  return this->value <= other.value;
}

bool ae_f64::operator>=(const ae_f64 &other) const {
  return this->value >= other.value;
}

std::istream &operator>>(std::istream &is, ae_f64 &obj) {
  s163_t value;
  is >> std::hex >> value;
  obj.value = s163_to_ld(value);
  return is;
}

std::ostream &operator<<(std::ostream &os, const ae_f64 &obj) {
  os << "0x" << std::hex << d_to_s131(obj.value);
  return os;
}

void ae_f64::saturate() {
#ifdef NSATURATION
  assert(this->value >= S1747_MIN);
  assert(this->value <= S1747_MAX);
#else
  if (this->value < S1747_MIN) {
    this->value = S1747_MIN;
  } else if (this->value > S1747_MAX) {
    this->value = S1747_MAX;
  }
#endif
}
