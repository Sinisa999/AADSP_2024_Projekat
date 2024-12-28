#include <assert.h>
#include <iostream>

#include <xtensa/tie/xt_hifi4.h>

// void test_AE_LA24X2_IP(void) {
//   ae_int24x2 d = AE_INT24x2(123, 456);
//   ae_int24 mem[] = {1, 2, 3, 4, 5};
//   AE_LA24X2_IP(d, 0, (void *)mem);
//   assert(d[0] == ae_int24(1));
//   assert(d[1] == ae_int24(2));
// }

// void test_AE_LP24F_IU(void) {
//   ae_f24 a = UINT32_C(0xdeadbeefabbacafe);
//   ae_p24x2s d = AE_F24x2(0x12345678, 0x87654321);
//   AE_LP24F_IU(d, &a, 0);
//   assert(d[0] == ae_f24(0xdeadbeefU & 0xffffff00));
//   assert(d[1] == ae_f24(0xdeadbeefU & 0xffffff00));
// }

// void test_AE_SP24F_L_IU(void) {
//   char mem[16] = {0};
//   ae_p24x2s d = AE_F24x2(0, 0x123456);
//   AE_SP24F_L_IU(d, (ae_p24f *)&mem[2], 0);
//   assert(mem[1] == 0);
//   assert(*(ae_f24 *)&mem[2] == ae_f24(0x123456));
//   assert(mem[2 + sizeof(ae_f24)] == 0);
// }

// void test_AE_SLAASQ56S(void) {
//   assert(AE_SLAASQ56S(UINT32_C(0x0000000001234000), 12) ==
//   0x0000001234000000); assert(AE_SLAASQ56S(UINT32_C(0x0000000001234000), -12)
//   == 0x0000000000001234);
// }

// void test_AE_ROUNDSQ32SYM(void) {
// std::cout << "AE_ROUNDSQ32SYM = " << std::hex <<
// AE_ROUNDSQ32SYM(UINT32_C(1))
//           << std::endl;
// }

int main(const int argc, const char **argv) {

  // test_AE_LA24X2_IP();
  // test_AE_LP24F_IU();
  // test_AE_SP24F_L_IU();
  // test_AE_SLAASQ56S();
  // test_AE_ROUNDSQ32SYM();
  // std::cout << "v = " << b << std::endl;

  // ae_f24x2 c = AE_F24x2(a, b);
  // std::cout << "c = " << c << std::endl;

  // const ae_f32 x = 0.1f;
  // const ae_f32 y = 0.2f;
  // const ae_f32 z = x * y;
  // const ae_f32x2 vect = AE_F32x2_C(x, y);

  // std::cout << "vect = " << vect << std::endl;

  ae_f32 a = 0.3;
  ae_f32 b;

  std::cout << "a := " << a << std::endl;
  std::cout << "b := ";
  std::cin >> b;
  std::cout << "a := " << (double)a << std::endl;
  std::cout << "b := " << (double)b << std::endl;
  std::cout << "a + b = " << (double)(a + b) << std::endl;
  std::cout << "a - b = " << (double)(a - b) << std::endl;
  std::cout << "a * b = " << (double)(a * b) << std::endl;
  std::cout << "a == b = " << (a == b) << std::endl;
  std::cout << "a != b = " << (a != b) << std::endl;
  std::cout << "a < b = " << (a < b) << std::endl;
  std::cout << "a > b = " << (a > b) << std::endl;
  std::cout << "a <= b = " << (a <= b) << std::endl;
  std::cout << "a >= b = " << (a >= b) << std::endl;

  return 0;
}
