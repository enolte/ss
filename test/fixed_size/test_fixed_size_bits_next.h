#include <iostream>
#include <cassert>

#include "../../subsets/fixed_size.h"

void test_fixed_size_bits_next_16_bytes();

void test_fixed_size_bits_next()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    ss::bits<7> bits{};

    // C(6, 4)
    ss::impl::arrays::set1(bits, 0, 4);     assert(bits[0] == 0b000'1111);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b001'0111);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b001'1011);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b001'1101);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b001'1110);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b010'0111);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b010'1011);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b010'1101);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b010'1110);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b011'0011);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b011'0101);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b011'0110);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b011'1001);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b011'1010);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b011'1100);
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b111'1100);

    // Now advance from end state
    // Expect return to begin state
    ss::fixed_size::impl::next(bits, 4, 6); assert(bits[0] == 0b000'1111);
  }

  test_fixed_size_bits_next_16_bytes();
}

void test_fixed_size_bits_next_16_bytes()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    ss::bits<67> bits{};

    ss::impl::arrays::set1(bits, 0, 3);      assert(bits[0] == 0b000111); assert(bits[1] == 0);
    ss::fixed_size::impl::next(bits, 3, 66); assert(bits[0] == 0b001011); assert(bits[1] == 0);
    ss::fixed_size::impl::next(bits, 3, 66); assert(bits[0] == 0b001101); assert(bits[1] == 0);
    ss::fixed_size::impl::next(bits, 3, 66); assert(bits[0] == 0b001110); assert(bits[1] == 0);
    ss::fixed_size::impl::next(bits, 3, 66); assert(bits[0] == 0b010011); assert(bits[1] == 0);
    ss::fixed_size::impl::next(bits, 3, 66); assert(bits[0] == 0b010101); assert(bits[1] == 0);
  }


  {
    ss::bits<67> bits{};
    bits[0] = 0xffff'ffff'ffff'ffc0; bits[1] = 0; // 58 bits, state after begin

    ss::fixed_size::impl::next(bits, 58, 67); assert(bits[0] == 0x01ff'ffff'ffff'ffff); assert(bits[1] == 0b01); // 57 + 1
    ss::fixed_size::impl::next(bits, 58, 67); assert(bits[0] == 0x02ff'ffff'ffff'ffff); assert(bits[1] == 0b01);
    ss::fixed_size::impl::next(bits, 58, 67); assert(bits[0] == 0x037f'ffff'ffff'ffff); assert(bits[1] == 0b01);
    ss::fixed_size::impl::next(bits, 58, 67); assert(bits[0] == 0x03bf'ffff'ffff'ffff); assert(bits[1] == 0b01);

    // Jump ahead
    bits[0] = 0xffff'ffff'ffff'ff80;          // 57
    bits[1] = 0b01;                           //  1

    ss::fixed_size::impl::next(bits, 58, 67); assert(bits[0] == 0x01ff'ffff'ffff'ffff); assert(bits[1] == 0b10); // 57 + 1
    ss::fixed_size::impl::next(bits, 58, 67); assert(bits[0] == 0x02ff'ffff'ffff'ffff); assert(bits[1] == 0b10);
    ss::fixed_size::impl::next(bits, 58, 67); assert(bits[0] == 0x037f'ffff'ffff'ffff); assert(bits[1] == 0b10);
    ss::fixed_size::impl::next(bits, 58, 67); assert(bits[0] == 0x03bf'ffff'ffff'ffff); assert(bits[1] == 0b10);
  }
}

