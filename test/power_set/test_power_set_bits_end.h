#include <ostream>
#include <cassert>

#include "../../subsets/power_set.h"

void test_power_set_bits_end()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    ss::bits<16> bits{};
    ss::power_set::impl::end(bits, 16);
    assert(bits[0] == 0b1'11111111'11111111);
  }

  {
    ss::bits<42> bits{};
    ss::power_set::impl::end(bits, 42);
    assert(bits[0] == 0b111'11111'11111'11111'11111'11111'11111'11111'11111);
  }


  {
    ss::bits<77> bits{};
    ss::power_set::impl::end(bits, 77);
/*
    result:

          14 ones
             │
      ┌──────┴─────┐
    0011111111111111'1111111111111111111111111111111111111111111111111111111111111111
    9876543210987654 3210987654321098765432109876543210987654321098765432109876543210
    7777777777666666 6666555555555544444444443333333333222222222211111111110000000000
      ││
      │└ last
      └─ end
*/

    assert(bits[0] == 0xffff'ffff'ffff'ffff);
    assert(bits[1] == 0b01111'11111'11111);
  }

}