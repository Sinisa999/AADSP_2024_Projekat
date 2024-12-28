#ifndef __XTBOOL_H__
#define __XTBOOL_H__

#include <iostream>
#include <stdbool.h>
#include <stddef.h>

typedef bool xtbool;

class xtbool2 {
public:
  xtbool2(const xtbool lo = false);
  xtbool2(const xtbool hi, const xtbool lo);
  xtbool &operator[](size_t index);
  const xtbool &operator[](size_t index) const;
  friend std::ostream &operator<<(std::ostream &os, const xtbool2 &obj);

private:
  bool hi;
  bool lo;
};

#define XTBOOL2(hi, lo) xtbool2(hi, lo)

#endif /* __XTBOOL_H__ */
