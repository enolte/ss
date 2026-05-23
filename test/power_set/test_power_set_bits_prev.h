#include <iostream>
#include <cassert>

#include "../../subsets/power_set.h"

void test_power_set_bits_prev_16_bytes();
void test_power_set_bits_prev_24_bytes();

void test_power_set_bits_prev()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  // 32 subsets
  ss::bits<5> bits{};
  using namespace ss::power_set::impl;

  {
    rbegin(bits, 5); assert(bits[0] == 0b0'11111); // 31 = rbegin
    prev(bits, 5); assert(bits[0] == 0b0'11110); // 30
    prev(bits, 5); assert(bits[0] == 0b0'11101); // 29
    prev(bits, 5); assert(bits[0] == 0b0'11100); // 28
    prev(bits, 5); assert(bits[0] == 0b0'11011); // 27...
    prev(bits, 5); assert(bits[0] == 0b0'11010);
    prev(bits, 5); assert(bits[0] == 0b0'11001);
    prev(bits, 5); assert(bits[0] == 0b0'11000);
    prev(bits, 5); assert(bits[0] == 0b0'10111);
    prev(bits, 5); assert(bits[0] == 0b0'10110);
    prev(bits, 5); assert(bits[0] == 0b0'10101);
    prev(bits, 5); assert(bits[0] == 0b0'10100);
    prev(bits, 5); assert(bits[0] == 0b0'10011);
    prev(bits, 5); assert(bits[0] == 0b0'10010);
    prev(bits, 5); assert(bits[0] == 0b0'10001);
    prev(bits, 5); assert(bits[0] == 0b0'10000); // 16

    // Jump ahead
    bits[0] = 5; assert(bits[0] == 0b00'0101);
    prev(bits, 5); assert(bits[0] == 0b0'00100);
    prev(bits, 5); assert(bits[0] == 0b0'00011);
    prev(bits, 5); assert(bits[0] == 0b0'00010);
    prev(bits, 5); assert(bits[0] == 0b0'00001);
    prev(bits, 5); assert(bits[0] == 0b0'00000); // 0 = begin
    prev(bits, 5); assert(bits[0] == 0b1'00000); // 32 = rend <-- begin
    prev(bits, 5); assert(bits[0] == 0b0'11111); // 31 = rbegin <-- rend
  }

  // Check prev(`end`)
  {
    bits[0] = 0b1'11111; assert(is_end(bits, 5));
    prev(bits, 5); assert(bits[0] == 0b0'11111); assert(is_rbegin(bits, 5)); // 31 = rbegin <-- end
  }

  test_power_set_bits_prev_16_bytes();
  test_power_set_bits_prev_24_bytes();
}

void test_power_set_bits_prev_24_bytes()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  using namespace ss::power_set::impl;
  using ss::operator<<;

  ss::bits<145> bits{};
  assert(bits.size() == 3);

  const auto prev = [&bits]{ ss::power_set::impl::prev(bits, 145); };

  // Decrement from rbegin
  {
    rbegin(bits, 145);
    assert(is_rbegin(bits, 145));
    assert(bits[2] == 0x01'ffff); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'ffff);

    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'fffe);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'fffd);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'fffc);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'fffb);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'fffa);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'fff9);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'fff8);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0xffff'ffff'ffff'ffff); assert(bits[0] == 0xffff'ffff'ffff'fff7);
  }

  // Jump backward: check middle case branch
  {
    bits[2] = 0x01'ffff; bits[1] = 0b1011; bits[0] = 3;
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0b1011 && bits[0] == 2);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0b1011 && bits[0] == 1);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0b1011 && bits[0] == 0);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0b1010 && bits[0] == 0xffff'ffff'ffff'ffff);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0b1010 && bits[0] == 0xffff'ffff'ffff'fffe);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0b1010 && bits[0] == 0xffff'ffff'ffff'fffd);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0b1010 && bits[0] == 0xffff'ffff'ffff'fffc);
  }

  // Jump backward: check last segment branch
  {
    bits[2] = 0x01'ffff; bits[1] = 0; bits[0] = 3;
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0 && bits[0] == 2);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0 && bits[0] == 1);
    prev(); assert(bits[2] == 0x01'ffff); assert(bits[1] == 0 && bits[0] == 0);
    prev(); assert(bits[2] == 0x01'fffe); assert(bits[1] == 0xffff'ffff'ffff'ffff && bits[0] == 0xffff'ffff'ffff'ffff);
    prev(); assert(bits[2] == 0x01'fffe); assert(bits[1] == 0xffff'ffff'ffff'ffff && bits[0] == 0xffff'ffff'ffff'fffe);
    prev(); assert(bits[2] == 0x01'fffe); assert(bits[1] == 0xffff'ffff'ffff'ffff && bits[0] == 0xffff'ffff'ffff'fffd);
    prev(); assert(bits[2] == 0x01'fffe); assert(bits[1] == 0xffff'ffff'ffff'ffff && bits[0] == 0xffff'ffff'ffff'fffc);
  }
}

void test_power_set_bits_prev_16_bytes()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  using namespace ss::power_set::impl;

  ss::bits<67> bits{};

  // Decrement from rbegin
  {
    rbegin(bits, 67);
    assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'ffff); assert(is_rbegin(bits, 67));

    prev(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'fffe);
    prev(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'fffd);
    prev(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'fffc);
    prev(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'fffb);
    prev(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'fffa);
    prev(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'fff9);
    prev(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'fff8);
    prev(bits, 67); assert(bits[1] == 0b00'111); assert(bits[0] == 0xffff'ffff'ffff'fff7);
  }

  // Jump backward: check cyclic iteration
  {
    bits[1] = 0; bits[0] = 3;
    prev(bits, 67); assert(bits[1] == 0 && bits[0] == 2);
    prev(bits, 67); assert(bits[1] == 0 && bits[0] == 1);
    prev(bits, 67); assert(bits[1] == 0 && bits[0] == 0); assert(is_begin(bits, 67));                             // begin
    prev(bits, 67); assert(bits[1] == 0b1'000 && bits[0] == 0); assert(is_rend(bits, 67));                        // rend <-- begin
    prev(bits, 67); assert(bits[1] == 0b0'111 && bits[0] == 0xffff'ffff'ffff'ffff); assert(is_rbegin(bits, 67));  // rbegin <-- rend
    prev(bits, 67); assert(bits[1] == 0b0'111 && bits[0] == 0xffff'ffff'ffff'fffe);
    prev(bits, 67); assert(bits[1] == 0b0'111 && bits[0] == 0xffff'ffff'ffff'fffd);
  }

  // Jump forward: Check carry
  {
    bits[1] = 1; bits[0] = 2;
    prev(bits, 67); assert(bits[1] == 1); assert(bits[0] == 1);
    prev(bits, 67); assert(bits[1] == 1); assert(bits[0] == 0);
    prev(bits, 67); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'ffff);
    prev(bits, 67); assert(bits[1] == 0); assert(bits[0] == 0xffff'ffff'ffff'fffe);
  }
}

