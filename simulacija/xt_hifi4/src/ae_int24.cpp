#ifdef NSATURATION
#include <assert.h>
#endif

#include "xtensa/tie/ae_int24.hpp"
#include "xtensa/tie/ae_vector.hpp"

ae_int24::ae_int24(const uint32_t value) : value(value) { this->saturate(); }

ae_int24::operator int() const { return this->value; }

ae_int24 ae_int24::operator+(const ae_int24 &other) const {
  ae_int24 result;
  result.value = this->value + other.value;
  result.saturate();
  return result;
}

ae_int24 ae_int24::operator-(const ae_int24 &other) const {
  ae_int24 result;
  result.value = this->value - other.value;
  result.saturate();
  return result;
}

ae_int24 ae_int24::operator*(const ae_int24 &other) const {
  ae_int24 result;
  result.value = this->value * other.value;
  result.saturate();
  return result;
}

ae_int24 ae_int24::operator+(const unsigned int &other) const {
  ae_int24 result;
  result.value = this->value + other;
  result.saturate();
  return result;
}

ae_int24 ae_int24::operator-(const unsigned int &other) const {
  ae_int24 result;
  result.value = this->value - other;
  result.saturate();
  return result;
}

ae_int24 ae_int24::operator*(const unsigned int &other) const {
  ae_int24 result;
  result.value = this->value * other;
  result.saturate();
  return result;
}

ae_int24 &ae_int24::operator+=(const ae_int24 &other) {
  this->value += other.value;
  this->saturate();
  return *this;
}

ae_int24 &ae_int24::operator-=(const ae_int24 &other) {
  this->value -= other.value;
  this->saturate();
  return *this;
}

ae_int24 &ae_int24::operator*=(const ae_int24 &other) {
  this->value *= other.value;
  this->saturate();
  return *this;
}

ae_int24 &ae_int24::operator+=(const unsigned int &other) {
  this->value += other;
  this->saturate();
  return *this;
}

ae_int24 &ae_int24::operator-=(const unsigned int &other) {
  this->value -= other;
  this->saturate();
  return *this;
}

ae_int24 &ae_int24::operator*=(const unsigned int &other) {
  this->value *= other;
  this->saturate();
  return *this;
}

bool ae_int24::operator==(const ae_int24 &other) const {
  return this->value == other.value;
}

bool ae_int24::operator!=(const ae_int24 &other) const {
  return this->value != other.value;
}

bool ae_int24::operator<(const ae_int24 &other) const {
  return this->value < other.value;
}

bool ae_int24::operator>(const ae_int24 &other) const {
  return this->value > other.value;
}

bool ae_int24::operator<=(const ae_int24 &other) const {
  return this->value <= other.value;
}

bool ae_int24::operator>=(const ae_int24 &other) const {
  return this->value >= other.value;
}

bool ae_int24::operator==(const unsigned int &other) const {
  return this->value == other;
}

bool ae_int24::operator!=(const unsigned int &other) const {
  return this->value != other;
}

bool ae_int24::operator<(const unsigned int &other) const {
  return this->value < other;
}

bool ae_int24::operator>(const unsigned int &other) const {
  return this->value > other;
}

bool ae_int24::operator<=(const unsigned int &other) const {
  return this->value <= other;
}

bool ae_int24::operator>=(const unsigned int &other) const {
  return this->value >= other;
}

std::istream &operator>>(std::istream &is, ae_int24 &obj) {
  int32_t value;
  is >> std::hex >> value;
  obj.value = value;
  obj.saturate();
  return is;
}

std::ostream &operator<<(std::ostream &os, const ae_int24 &obj) {
  os << obj.value;
  return os;
}

void ae_int24::saturate() {
#ifdef NSATURATION
  assert(this->value >= UINT24_MIN);
  assert(this->value <= UINT24_MAX);
#else
  if (this->value > UINT24_MAX) {
    this->value = UINT24_MAX;
  }
#endif
}

ae_int24x2::ae_int24x2(const ae_int24 lo) : hi(0), lo(lo) {}

ae_int24x2::ae_int24x2(const ae_int24 hi, const ae_int24 lo) : hi(hi), lo(lo) {}

ae_int24 &ae_int24x2::operator[](size_t index) {
  if (index == AE_DR_H) {
    return this->hi;
  } else if (index == AE_DR_L) {
    return this->lo;
  } else {
    throw std::out_of_range(AE_DR_INDEX_ERROR);
  }
}

const ae_int24 &ae_int24x2::operator[](size_t index) const {
  if (index == AE_DR_H) {
    return this->hi;
  } else if (index == AE_DR_L) {
    return this->lo;
  } else {
    throw std::out_of_range(AE_DR_INDEX_ERROR);
  }
}

std::ostream &operator<<(std::ostream &os, const ae_int24x2 &obj) {
  os << std::dec << obj.hi << AE_DR_SEPARATOR << obj.lo;
  return os;
}
