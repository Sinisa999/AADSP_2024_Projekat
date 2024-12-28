#ifdef NSATURATION
#include <assert.h>
#endif
#include <climits>

#include "xtensa/tie/ae_f32.hpp"
#include "xtensa/tie/ae_vector.hpp"

#define S131_MIN (-1.0)
#define S131_MAX (1.0)

ae_f32::ae_f32(const s131_t value) : value(s131_to_d(value)) {}

ae_f32::ae_f32(const s163_t value) : value(s131_to_d(s163_to_s131(value))) {}

ae_f32::ae_f32(const int value) : value((double)value / (double)INT_MAX) {
  this->saturate();
}

ae_f32::ae_f32(const double value) : value(value) { this->saturate(); }

ae_f32::operator int() const { return d_to_s131(this->value); }

ae_f32::operator double() const { return this->value; }

ae_f32::operator ae_int32() const { return (int)*this; }

ae_f32 ae_f32::operator+(const ae_f32 &other) const {
  ae_f32 result;
  result.value = this->value + other.value;
  result.saturate();
  return result;
}

ae_f32 ae_f32::operator-(const ae_f32 &other) const {
  ae_f32 result;
  result.value = this->value - other.value;
  result.saturate();
  return result;
}

ae_f32 ae_f32::operator*(const ae_f32 &other) const {
  ae_f32 result;
  result.value = this->value * other.value;
  result.saturate();
  return result;
}

ae_f32 &ae_f32::operator+=(const ae_f32 &other) {
  this->value += other.value;
  this->saturate();
  return *this;
}

ae_f32 &ae_f32::operator-=(const ae_f32 &other) {
  this->value -= other.value;
  this->saturate();
  return *this;
}

ae_f32 &ae_f32::operator*=(const ae_f32 &other) {
  this->value *= other.value;
  this->saturate();
  return *this;
}

bool ae_f32::operator==(const ae_f32 &other) const {
  return this->value == other.value;
}

bool ae_f32::operator!=(const ae_f32 &other) const {
  return this->value != other.value;
}

bool ae_f32::operator<(const ae_f32 &other) const {
  return this->value < other.value;
}

bool ae_f32::operator>(const ae_f32 &other) const {
  return this->value > other.value;
}

bool ae_f32::operator<=(const ae_f32 &other) const {
  return this->value <= other.value;
}

bool ae_f32::operator>=(const ae_f32 &other) const {
  return this->value >= other.value;
}

std::istream &operator>>(std::istream &is, ae_f32 &obj) {
  s131_t value;
  is >> std::hex >> value;
  obj.value = s131_to_d(value);
  return is;
}

std::ostream &operator<<(std::ostream &os, const ae_f32 &obj) {
  os << "0x" << std::hex << d_to_s131(obj.value);
  return os;
}

void ae_f32::saturate() {
#ifdef NSATURATION
  assert(this->value >= S131_MIN);
  assert(this->value <= S131_MAX);
#else
  if (this->value < S131_MIN) {
    this->value = S131_MIN;
  } else if (this->value > S131_MAX) {
    this->value = S131_MAX;
  }
#endif
}

ae_f32x2::ae_f32x2(const ae_f32 lo) : hi(0), lo(lo) {}

ae_f32x2::ae_f32x2(const ae_f32 hi, const ae_f32 lo) : hi(hi), lo(lo) {}

ae_f32x2::ae_f32x2(const ae_int32x2 pair)
    : hi(ae_f32(pair[AE_DR_H])), lo(ae_f32(pair[AE_DR_H])) {}

ae_f32 &ae_f32x2::operator[](size_t index) {
  if (index == AE_DR_H) {
    return this->hi;
  } else if (index == AE_DR_L) {
    return this->lo;
  } else {
    throw std::out_of_range(AE_DR_INDEX_ERROR);
  }
}

const ae_f32 &ae_f32x2::operator[](size_t index) const {
  if (index == AE_DR_H) {
    return this->hi;
  } else if (index == AE_DR_L) {
    return this->lo;
  } else {
    throw std::out_of_range(AE_DR_INDEX_ERROR);
  }
}

std::ostream &operator<<(std::ostream &os, const ae_f32x2 &obj) {
  os << obj.hi << AE_DR_SEPARATOR << obj.lo;
  return os;
}
