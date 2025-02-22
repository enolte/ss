#include <ostream>
#include <cassert>

#include "../../subsets/fixed_size.h"

void test_fixed_size_bits_rbegin()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    ss::bits<16> bits{};
    ss::fixed_size::impl::rbegin(bits, 9, 16);
    assert(bits[0] == 0b0'111111111'0000000);
  }

  {
    ss::bits<77> bits{};
    ss::fixed_size::impl::rbegin(bits, 12, 77);
    assert(bits[0] == 0);
    assert(bits[1] == 0b0111'11111'11110);
  }

  {
    ss::bits<77> bits{};
    ss::fixed_size::impl::rbegin(bits, 66, 77);
    assert(bits[0] == 0xffff'ffff'ffff'f800);
    assert(bits[1] == 0b0111'11111'11111);
  }
}

