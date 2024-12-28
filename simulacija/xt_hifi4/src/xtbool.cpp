#include "xtensa/tie/xtbool.hpp"
#include "xtensa/tie/ae_vector.hpp"

xtbool2::xtbool2(const xtbool lo) : hi(false), lo(lo) {}

xtbool2::xtbool2(const xtbool hi, const xtbool lo) : hi(hi), lo(lo) {}

xtbool &xtbool2::operator[](size_t index) {
  if (index == AE_DR_H) {
    return this->hi;
  } else if (index == AE_DR_L) {
    return this->lo;
  } else {
    throw std::out_of_range(AE_DR_INDEX_ERROR);
  }
}

const xtbool &xtbool2::operator[](size_t index) const {
  if (index == AE_DR_H) {
    return this->hi;
  } else if (index == AE_DR_L) {
    return this->lo;
  } else {
    throw std::out_of_range(AE_DR_INDEX_ERROR);
  }
}

std::ostream &operator<<(std::ostream &os, const xtbool2 &obj) {
  os << std::dec << obj.hi << AE_DR_SEPARATOR << obj.lo;
  return os;
}
