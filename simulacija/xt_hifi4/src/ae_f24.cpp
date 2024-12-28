#ifdef NSATURATION
#include <assert.h>
#endif
#include <climits>

#include "xtensa/tie/ae_f24.hpp"
#include "xtensa/tie/ae_vector.hpp"

#define S124_MIN (-1.0)
#define S124_MAX (1.0)

ae_f24::ae_f24(const s131_t value)
    : value(s131_to_d(value & UINT32_C(0xffffff00))) {}

ae_f24::ae_f24(const s163_t value)
    : value(s131_to_d(s163_to_s131(value & UINT32_C(0xffffff0000000000)))) {}

ae_f24::ae_f24(const int value)
    : value((double)(value << 8) / (double)INT_MAX) {
  this->saturate();
}

ae_f24::ae_f24(const double value) : value(value) { this->saturate(); }

ae_f24::operator int() const {
  return d_to_s131(this->value) & (0xffffff << (UINT_WIDTH - 24));
}

ae_f24::operator double() const { return this->value; }

ae_f24::operator ae_int32() const { return (int)*this; }

ae_f24 ae_f24::operator+(const ae_f24 &other) const {
  ae_f24 result;
  result.value = this->value + other.value;
  result.saturate();
  return result;
}

ae_f24 ae_f24::operator-(const ae_f24 &other) const {
  ae_f24 result;
  result.value = this->value - other.value;
  result.saturate();
  return result;
}

ae_f24 ae_f24::operator*(const ae_f24 &other) const {
  ae_f24 result;
  result.value = this->value * other.value;
  result.saturate();
  return result;
}

ae_f24 &ae_f24::operator+=(const ae_f24 &other) {
  this->value += other.value;
  this->saturate();
  return *this;
}

ae_f24 &ae_f24::operator-=(const ae_f24 &other) {
  this->value -= other.value;
  this->saturate();
  return *this;
}

ae_f24 &ae_f24::operator*=(const ae_f24 &other) {
  this->value *= other.value;
  this->saturate();
  return *this;
}

bool ae_f24::operator==(const ae_f24 &other) const {
  return this->value == other.value;
}

bool ae_f24::operator!=(const ae_f24 &other) const {
  return this->value != other.value;
}

bool ae_f24::operator<(const ae_f24 &other) const {
  return this->value < other.value;
}

bool ae_f24::operator>(const ae_f24 &other) const {
  return this->value > other.value;
}

bool ae_f24::operator<=(const ae_f24 &other) const {
  return this->value <= other.value;
}

bool ae_f24::operator>=(const ae_f24 &other) const {
  return this->value >= other.value;
}

std::istream &operator>>(std::istream &is, ae_f24 &obj) {
  s131_t value;
  is >> std::hex >> value;
  obj.value = s131_to_d(value);
  return is;
}

std::ostream &operator<<(std::ostream &os, const ae_f24 &obj) {
  os << "0x" << std::hex << (d_to_s131(obj.value) >> 8);
  return os;
}

void ae_f24::saturate() {
#ifdef NSATURATION
  assert(this->value >= S124_MIN);
  assert(this->value <= S124_MAX);
#else
  if (this->value < S124_MIN) {
    this->value = S124_MIN;
  } else if (this->value > S124_MAX) {
    this->value = S124_MAX;
  }
#endif
}

ae_f24x2::ae_f24x2(const ae_f24 lo) : hi(0), lo(lo) {}

ae_f24x2::ae_f24x2(const ae_f24 hi, const ae_f24 lo) : hi(hi), lo(lo) {}

ae_f24 &ae_f24x2::operator[](size_t index) {
  if (index == AE_DR_H) {
    return this->hi;
  } else if (index == AE_DR_L) {
    return this->lo;
  } else {
    throw std::out_of_range(AE_DR_INDEX_ERROR);
  }
}

const ae_f24 &ae_f24x2::operator[](size_t index) const {
  if (index == AE_DR_H) {
    return this->hi;
  } else if (index == AE_DR_L) {
    return this->lo;
  } else {
    throw std::out_of_range(AE_DR_INDEX_ERROR);
  }
}

std::ostream &operator<<(std::ostream &os, const ae_f24x2 &obj) {
  os << obj.hi << AE_DR_SEPARATOR << obj.lo;
  return os;
}
