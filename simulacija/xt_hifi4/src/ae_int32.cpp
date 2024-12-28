#include "xtensa/tie/ae_int32.hpp"
#include "xtensa/tie/ae_vector.hpp"

ae_int32x2::ae_int32x2(const ae_int32 lo) : hi(0), lo(lo) {}

ae_int32x2::ae_int32x2(const ae_int32 hi, const ae_int32 lo) : hi(hi), lo(lo) {}

ae_int32 &ae_int32x2::operator[](size_t index) {
  if (index == AE_DR_H) {
    return this->hi;
  } else if (index == AE_DR_L) {
    return this->lo;
  } else {
    throw std::out_of_range(AE_DR_INDEX_ERROR);
  }
}

const ae_int32 &ae_int32x2::operator[](size_t index) const {
  if (index == AE_DR_H) {
    return this->hi;
  } else if (index == AE_DR_L) {
    return this->lo;
  } else {
    throw std::out_of_range(AE_DR_INDEX_ERROR);
  }
}

std::ostream &operator<<(std::ostream &os, const ae_int32x2 &obj) {
  os << std::dec << obj.hi << AE_DR_SEPARATOR << obj.lo;
  return os;
}
