#ifndef __AE_INT32_H__
#define __AE_INT32_H__

#include <iostream>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t ae_int32;

class ae_int32x2 {
public:
  ae_int32x2(const ae_int32 lo = 0);
  ae_int32x2(const ae_int32 hi, const ae_int32 lo);
  ae_int32 &operator[](size_t index);
  const ae_int32 &operator[](size_t index) const;
  friend std::ostream &operator<<(std::ostream &os, const ae_int32x2 &obj);

private:
  ae_int32 hi;
  ae_int32 lo;
};

#define AE_INT32x2(hi, lo) ae_int32x2(hi, lo)

#endif /* __AE_INT32_H__ */
