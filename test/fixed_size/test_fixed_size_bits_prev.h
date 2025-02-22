#include <iostream>
#include <cassert>

#include "../../subsets/fixed_size.h"

void test_fixed_size_bits_prev_16_bytes();

void test_fixed_size_bits_prev()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    using namespace ss::fixed_size::impl;
    ss::bits<7> bits{};

    // C(6, 4)
    ss::impl::arrays::set1(bits, 2, 6); assert(bits[0] == 0b011'1100); // rbegin
    prev(bits, 4, 6); assert(bits[0] == 0b011'1010);
    prev(bits, 4, 6); assert(bits[0] == 0b011'1001);
    prev(bits, 4, 6); assert(bits[0] == 0b011'0110);
    prev(bits, 4, 6); assert(bits[0] == 0b011'0101);
    prev(bits, 4, 6); assert(bits[0] == 0b011'0011);
    prev(bits, 4, 6); assert(bits[0] == 0b010'1110);
    prev(bits, 4, 6); assert(bits[0] == 0b010'1101);
    prev(bits, 4, 6); assert(bits[0] == 0b010'1011);
    prev(bits, 4, 6); assert(bits[0] == 0b010'0111);
    prev(bits, 4, 6); assert(bits[0] == 0b001'1110);
    prev(bits, 4, 6); assert(bits[0] == 0b001'1101);
    prev(bits, 4, 6); assert(bits[0] == 0b001'1011);
    prev(bits, 4, 6); assert(bits[0] == 0b001'0111);
    prev(bits, 4, 6); assert(bits[0] == 0b000'1111);
    prev(bits, 4, 6); assert(bits[0] == 0b100'1111); // rend

    // Now advance from rend state
    // Expect return to rbegin state
    prev(bits, 4, 6); assert(bits[0] == 0b011'1100); // rbegin
  }

  test_fixed_size_bits_prev_16_bytes();
}


void test_fixed_size_bits_prev_16_bytes()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  using namespace ss::fixed_size::impl;

  {
    ss::bits<67> bits{};

    // C(67, 3)
    // rbegin state
    ss::impl::arrays::set1(bits, 64, 67); assert(bits[0] == 0); assert(bits[1] == 0b00111);

    prev(bits, 3, 67); assert(bits[0] == 0x8000'0000'0000'0000); assert(bits[1] == 0b00110);
    prev(bits, 3, 67); assert(bits[0] == 0x4000'0000'0000'0000); assert(bits[1] == 0b00110);
    prev(bits, 3, 67); assert(bits[0] == 0x2000'0000'0000'0000); assert(bits[1] == 0b00110);
    prev(bits, 3, 67); assert(bits[0] == 0x1000'0000'0000'0000); assert(bits[1] == 0b00110);
    prev(bits, 3, 67); assert(bits[0] == 0x0800'0000'0000'0000); assert(bits[1] == 0b00110);
  }


  {
    using namespace ss::fixed_size::impl;
    ss::bits<67> bits{};

    // C(71, 58)
    // rbegin state with K = 58
    bits[0] = 0xffff'ffff'ffff'e000;          // 51 bits
    bits[1] = 0x7f;                           //  7 bits


    prev(bits, 58, 71); assert(bits[0] == 0xffff'ffff'ffff'd000); assert(bits[1] == 0x7f); //  51 + 7
    prev(bits, 58, 71); assert(bits[0] == 0xffff'ffff'ffff'c800); assert(bits[1] == 0x7f); //  51 + 7
    prev(bits, 58, 71); assert(bits[0] == 0xffff'ffff'ffff'c400); assert(bits[1] == 0x7f); //  51 + 7
    prev(bits, 58, 71); assert(bits[0] == 0xffff'ffff'ffff'c200); assert(bits[1] == 0x7f); //  51 + 7

    // Jump ahead
    bits[0] = 0xffff'ffff'ffff'c001;          // 51
    bits[1] = 0x7f;                           //  7

    prev(bits, 58, 71); assert(bits[0] == 0xffff'ffff'ffff'b000); assert(bits[1] == 0x7f); // 51 + 7

    // Jump ahead
    bits[0] = 0x0007'ffff'ffff'ffff;          // 51
    bits[1] = 0x7f;                           //  7

    prev(bits, 58, 71); assert(bits[0] == 0xffff'ffff'ffff'f000); assert(bits[1] == 0x7e); // 52 + 6
    prev(bits, 58, 71); assert(bits[0] == 0xffff'ffff'ffff'e800); assert(bits[1] == 0x7e); // 52 + 6

    // Jump ahead
    bits[0] = 0x0003'ffff'ffff'ffff;          // 52
    bits[1] = 0x7e;                           //  6

    prev(bits, 58, 71); assert(bits[0] == 0xffff'ffff'ffff'f000); assert(bits[1] == 0x7d); // 52 + 6
  }
}

