#ifndef __FXPT_H__
#define __FXPT_H__

#include <stdint.h>

typedef uint32_t s131_t;
typedef uint64_t s163_t;

s131_t d_to_s131(const double);
double s131_to_d(const s131_t);

s163_t ld_to_s163(const long double);
long double s163_to_ld(const s163_t);

s163_t s131_to_s163(const s131_t);
s131_t s163_to_s131(const s163_t);

#endif /* __FXPT_H__ */
