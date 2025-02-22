#include <ostream>
#include <cassert>

#include "../../subsets/fixed_size.h"

void test_fixed_size_bits_rend()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    ss::bits<16> bits{};
    ss::fixed_size::impl::rend(bits, 9, 16);
    assert(bits[0] == 0b10'000000'111111111);
  }

  {
    ss::bits<42> bits{};
    ss::fixed_size::impl::rend(bits, 23, 42);
    assert(bits[0] == 0b100'00000'00000'00000'00111'11111'11111'11111'11111);
  }

  {
    ss::bits<77> bits{};
    ss::fixed_size::impl::rend(bits, 12, 77);
    assert(bits[0] == 0b011'11111'11111);
    assert(bits[1] == 0b1000'00000'00000);
  }

  {
    ss::bits<77> bits{};
    ss::fixed_size::impl::rend(bits, 66, 77);
    assert(bits[0] == 0xffff'ffff'ffff'ffff);
    assert(bits[1] == 0b1000'00000'00011);
  }
}
