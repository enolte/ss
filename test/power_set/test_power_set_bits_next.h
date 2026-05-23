#include <iostream>
#include <cassert>

#include "../../subsets/power_set.h"

void test_power_set_bits_next_16_bytes();
void test_power_set_bits_next_24_bytes();
void test_power_set_bits_next_32_bytes();

void test_power_set_bits_next()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  // 32 subsets to iterate
  ss::bits<5> bits{};
  using namespace ss::power_set::impl;

  {
    assert(bits[0] == 0b00'0000); // 0 : begin state

    next(bits, 5); assert(bits[0] == 0b0'00001);
    next(bits, 5); assert(bits[0] == 0b0'00010);
    next(bits, 5); assert(bits[0] == 0b0'00011);
    next(bits, 5); assert(bits[0] == 0b0'00100);

    // Jump ahead
    set1(bits, 0, 4); assert(bits[0] == 0b0'01111); // 15
    next(bits, 5); assert(bits[0] == 0b0'10000); // 16
    next(bits, 5); assert(bits[0] == 0b0'10001); // 17 ...
    next(bits, 5); assert(bits[0] == 0b0'10010);
    next(bits, 5); assert(bits[0] == 0b0'10011);
    next(bits, 5); assert(bits[0] == 0b0'10100);
    next(bits, 5); assert(bits[0] == 0b0'10101);
    next(bits, 5); assert(bits[0] == 0b0'10110);
    next(bits, 5); assert(bits[0] == 0b0'10111);
    next(bits, 5); assert(bits[0] == 0b0'11000);
    next(bits, 5); assert(bits[0] == 0b0'11001);
    next(bits, 5); assert(bits[0] == 0b0'11010);
    next(bits, 5); assert(bits[0] == 0b0'11011);
    next(bits, 5); assert(bits[0] == 0b0'11100); // 28
    next(bits, 5); assert(bits[0] == 0b0'11101); // 29
    next(bits, 5); assert(bits[0] == 0b0'11110); // 30
    next(bits, 5); assert(bits[0] == 0b0'11111); // 31 = rbegin
    next(bits, 5); assert(bits[0] == 0b1'11111); // rbegin --> end
   
    // Cyclic iteration: Now advance from end state. Expect return to begin state.
    next(bits, 5); assert(bits[0] == 0b0'00000); // end --> begin
  }

  // Check next(`rend)
  {
    bits[0] = 0b1'00000;
    assert(is_rend(bits, 5));
    next(bits, 5); assert(bits[0] == 0b0'00000); assert(is_begin(bits, 5));
  }

  test_power_set_bits_next_16_bytes();
  test_power_set_bits_next_24_bytes();
  test_power_set_bits_next_32_bytes();
}

void test_power_set_bits_next_32_bytes()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  ss::bits<200> bits{};

  const auto next = [&bits]{ ss::power_set::impl::next(bits, 200); };

  using namespace ss::power_set::impl;
  assert(is_begin(bits, 200));

  {
    ss::impl::arrays::set1(bits, 0, 3); assert(bits[2] == 0); assert(bits[1] == 0);  assert(bits[0] == 0b000111);
    next(); assert(bits[2] == 0); assert(bits[1] == 0);  assert(bits[0] == 0b001000);
    next(); assert(bits[2] == 0); assert(bits[1] == 0);  assert(bits[0] == 0b001001);
    next(); assert(bits[2] == 0); assert(bits[1] == 0);  assert(bits[0] == 0b001010);
    next(); assert(bits[2] == 0); assert(bits[1] == 0);  assert(bits[0] == 0b001011);
    next(); assert(bits[2] == 0); assert(bits[1] == 0);  assert(bits[0] == 0b001100);
  }

  // Jump ahead: check intermediate case impl branch (bytss 8-15 change --> zero all lower order bits)
  {
    bits[1] = 0; bits[0] = 0xffff'ffff'ffff'fffc;

    next(); assert(bits[2] == 0); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'fffd);
    next(); assert(bits[2] == 0); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'fffe);
    next(); assert(bits[2] == 0); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'ffff);
    next(); assert(bits[2] == 0); assert(bits[1] == 1); assert(bits[0] == 0);
    next(); assert(bits[2] == 0); assert(bits[1] == 1); assert(bits[0] == 1);
    next(); assert(bits[2] == 0); assert(bits[1] == 1); assert(bits[0] == 2);
    next(); assert(bits[2] == 0); assert(bits[1] == 1); assert(bits[0] == 3);
  }

  // Jump ahead: check intermediate case impl branch (bytes 16-23 change --> zero all lower order bits)
  {
    bits[2] = 6; bits[1] = 0xffff'ffff'ffff'ffff; bits[0] = 0xffff'ffff'ffff'fffc;

    next(); assert(bits[2] == 6); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'fffd);
    next(); assert(bits[2] == 6); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'fffe);
    next(); assert(bits[2] == 6); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'ffff);
    next(); assert(bits[2] == 7); assert(bits[1] == 0); assert(bits[0] == 0);
    next(); assert(bits[2] == 7); assert(bits[1] == 0); assert(bits[0] == 1);
    next(); assert(bits[2] == 7); assert(bits[1] == 0); assert(bits[0] == 2);
    next(); assert(bits[2] == 7); assert(bits[1] == 0); assert(bits[0] == 3);
  }
}


void test_power_set_bits_next_24_bytes()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  ss::bits<150> bits{};

  const auto next = [&bits]{ ss::power_set::impl::next(bits, 150); };

  using namespace ss::power_set::impl;
  assert(is_begin(bits, 150));

  {
    ss::impl::arrays::set1(bits, 0, 3);  assert(bits[1] == 0);  assert(bits[0] == 0b000111);
    next(); assert(bits[1] == 0);  assert(bits[0] == 0b001000);
    next(); assert(bits[1] == 0);  assert(bits[0] == 0b001001);
    next(); assert(bits[1] == 0);  assert(bits[0] == 0b001010);
    next(); assert(bits[1] == 0);  assert(bits[0] == 0b001011);
    next(); assert(bits[1] == 0);  assert(bits[0] == 0b001100);
  }

  // Jump ahead: check intermediate case impl branch (middle 8 bytes change --> zero all lower order bits)
  {
    bits[1] = 0; bits[0] = 0xffff'ffff'ffff'fffc;

    next(); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'fffd);
    next(); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'fffe);
    next(); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'ffff);
    next(); assert(bits[1] == 1); assert(bits[0] == 0);
    next(); assert(bits[1] == 1); assert(bits[0] == 1);
    next(); assert(bits[1] == 1); assert(bits[0] == 2);
    next(); assert(bits[1] == 1); assert(bits[0] == 3);
  }
}

void test_power_set_bits_next_16_bytes()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  ss::bits<67> bits{};
  using namespace ss::power_set::impl;
  assert(is_begin(bits, 67));

  {
    ss::impl::arrays::set1(bits, 0, 3);  assert(bits[1] == 0);  assert(bits[0] == 0b000111);
    next(bits, 67); assert(bits[1] == 0);  assert(bits[0] == 0b001000);
    next(bits, 67); assert(bits[1] == 0);  assert(bits[0] == 0b001001);
    next(bits, 67); assert(bits[1] == 0);  assert(bits[0] == 0b001010);
    next(bits, 67); assert(bits[1] == 0);  assert(bits[0] == 0b001011);
    next(bits, 67); assert(bits[1] == 0);  assert(bits[0] == 0b001100);
  }

  // Jump ahead: check carry
  {
    bits[1] = 0; bits[0] = 0xffff'ffff'ffff'fffc;

    next(bits, 67); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'fffd);
    next(bits, 67); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'fffe);
    next(bits, 67); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'ffff);
    next(bits, 67); assert(bits[1] == 1); assert(bits[0] == 0);
    next(bits, 67); assert(bits[1] == 1); assert(bits[0] == 1);
    next(bits, 67); assert(bits[1] == 1); assert(bits[0] == 2);
    next(bits, 67); assert(bits[1] == 1); assert(bits[0] == 3);
  }

  // Jump ahead: increment bit field value in the last offset
  {
    bits[1] = 1; bits[0] = 0xffff'ffff'ffff'fffd;
    next(bits, 67); assert(bits[1] == 1); assert(bits[0] == 0xffff'ffff'ffff'fffe);
    next(bits, 67); assert(bits[1] == 1); assert(bits[0] == 0xffff'ffff'ffff'ffff);
    next(bits, 67); assert(bits[1] == 2); assert(bits[0] == 0);
    next(bits, 67); assert(bits[1] == 2); assert(bits[0] == 1);
  }


  // Jump ahead: check cyclic iteration
  {
    bits[1] = 0b111; bits[0] = 0xffff'ffff'ffff'fffc;

    next(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'fffd);
    next(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'fffe);
    next(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'ffff); assert(is_rbegin(bits, 67));
    next(bits, 67); assert(bits[1] == 0b01'111); assert(bits[0] == 0xffff'ffff'ffff'ffff); assert(is_end(bits, 67));
    next(bits, 67); assert(bits[1] == 0); assert(bits[0] == 0); assert(is_begin(bits, 67));
    next(bits, 67); assert(bits[1] == 0); assert(bits[0] == 1);
    next(bits, 67); assert(bits[1] == 0); assert(bits[0] == 2);
    next(bits, 67); assert(bits[1] == 0); assert(bits[0] == 3);
  }
}

