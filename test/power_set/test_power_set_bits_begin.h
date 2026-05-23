#include <ostream>
#include <cassert>

#include "../../subsets/power_set.h"

void test_power_set_bits_begin()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    ss::bits<16> bits{};
    ss::power_set::impl::begin(bits);
    assert(bits[0] == 0);
  }

  {
    ss::bits<42> bits{};
    ss::power_set::impl::begin(bits);
    assert(bits[0] == 0);
  }

  {
    ss::bits<77> bits{};
    ss::power_set::impl::begin(bits);
    assert(bits[0] == 0);
    assert(bits[1] == 0);
  }

  {
    ss::bits<77> bits{};
    ss::power_set::impl::begin(bits);
    assert(bits[0] == 0);
    assert(bits[1] == 0);
  }
}

