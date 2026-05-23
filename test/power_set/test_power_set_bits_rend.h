#include <ostream>
#include <cassert>

#include "../../subsets/power_set.h"

void test_power_set_bits_rend()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    ss::bits<16> bits{};
    ss::power_set::impl::rend(bits, 16);
    assert(bits[0] == 0b10'0000000'00000000);
  }

  {
    ss::bits<42> bits{};
    ss::power_set::impl::rend(bits, 42);
    assert(bits[0] == 0b100'00000'00000'00000'00000'00000'00000'00000'00000);
  }

  {
    ss::bits<77> bits{};
    ss::power_set::impl::rend(bits, 77);
    assert(bits[0] == 0);
    assert(bits[1] == 0b1000'00000'00000);
  }
}


