#include <ostream>
#include <cassert>

#include "../../subsets/fixed_size.h"

void test_fixed_size_bits_begin()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    ss::bits<16> bits{};
    ss::fixed_size::impl::begin(bits, 9);
    assert(bits[0] == 0b00'000000'111111111);
  }

  {
    ss::bits<42> bits{};
    ss::fixed_size::impl::begin(bits, 23);
    assert(bits[0] == 0b000'00000'00000'00000'00111'11111'11111'11111'11111);
  }

  {
    ss::bits<77> bits{};
    ss::fixed_size::impl::begin(bits, 12);
    assert(bits[0] == 0b011'11111'11111);
    assert(bits[1] == 0);
  }

  {
    ss::bits<77> bits{};
    ss::fixed_size::impl::begin(bits, 66);
    assert(bits[0] == 0xffff'ffff'ffff'ffff);
    assert(bits[1] == 0b11);
  }
}

