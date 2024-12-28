#include <inttypes.h>
#include <stdio.h>

#include "fxpt.h"

void to_fixed(const char *arg) {
  long double decimal;
  sscanf(arg, "%Lf", &decimal);
  printf("0x%08" PRIx32 "\n", d_to_s131(decimal));
}

void to_decimal(const char *arg) {
  uint64_t fixed;
  sscanf(arg, "0x%" SCNx64, &fixed);
  printf("%g\n", s131_to_d((s131_t)fixed));
}

int main(const int argc, const char **argv) {
  const char *const input_buffer[64];
  char *arg = (char *)input_buffer;
  if (argc == 2) {
    arg = (char *)argv[1];
  } else {
    scanf("%s\n", arg);
  }
  if (arg[0] == '0' && arg[1] == 'x') {
    to_decimal(arg);
  } else {
    to_fixed(arg);
  }
  return 0;
}
