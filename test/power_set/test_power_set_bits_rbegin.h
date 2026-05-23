#include <ostream>
#include <cassert>

#include "../../subsets/power_set.h"

void test_power_set_bits_rbegin()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    ss::bits<16> bits{};
    ss::power_set::impl::rbegin(bits, 16);
    assert(bits[0] == 0b00'11111111'11111111);
  }

  {
    ss::bits<77> bits{};
    ss::power_set::impl::rbegin(bits, 77);
    assert(bits[0] == 0xffff'ffff'ffff'ffff);
    assert(bits[1] == 0b00'1'1111'1111'1111);
  }
}

