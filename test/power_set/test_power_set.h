#include <iostream>
#include <cassert>

#include "../../subsets/power_set.h"
#include "test_power_set_bits.h"

void test_power_set_subsets_4();
void test_power_set_subsets_187();

void test_power_set()
{
  test_power_set_subsets_4();
  test_power_set_subsets_187();
  test_power_set_bits();
}

void test_power_set_subsets_187()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  using namespace ss::power_set;

  subset<187> s{begin};

  assert(s.bits.size() == 3);
  s.bits[0] = 0xffff'ffff'ffff'ffff;
  s.bits[1] = 0xffff'ffff'ffff'ffff;
  s.bits[2] = 0x07ff'ffff'ffff'ffff;

  assert(s.size() == 187); assert(s == rbegin);

  --s; assert(s.bits[2] == 0x07ff'ffff'ffff'ffff); assert(s.bits[1] == 0xffff'ffff'ffff'ffff); assert(s.bits[0] == 0xffff'ffff'ffff'fffe); assert(s < rbegin);
  ++s; assert(s.bits[2] == 0x07ff'ffff'ffff'ffff); assert(s.bits[1] == 0xffff'ffff'ffff'ffff); assert(s.bits[0] == 0xffff'ffff'ffff'ffff); assert(s == rbegin);
  ++s; assert(s.bits[2] == 0x0fff'ffff'ffff'ffff); assert(s.bits[1] == 0xffff'ffff'ffff'ffff); assert(s.bits[0] == 0xffff'ffff'ffff'ffff); assert(s == end);
  ++s; assert(s.bits[2] == 0); assert(s.bits[1] == 0); assert(s.bits[0] == 0); assert(s == begin);
  ++s; assert(s.bits[2] == 0); assert(s.bits[1] == 0); assert(s.bits[0] == 1); assert(s > begin);
  --s; assert(s.bits[2] == 0); assert(s.bits[1] == 0); assert(s.bits[0] == 0); assert(s == begin);
  --s; assert(s.bits[2] == 0x0800'0000'0000'0000); assert(s.bits[1] == 0); assert(s.bits[0] == 0); assert(s == rend); assert(s < begin);
  --s; assert(s.bits[2] == 0x07ff'ffff'ffff'ffff); assert(s.bits[1] == 0xffff'ffff'ffff'ffff); assert(s.bits[0] == 0xffff'ffff'ffff'ffff); assert(s == rbegin);
}

void test_power_set_subsets_4()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  using namespace ss::power_set;

  int count{};
  subset<4> s{begin};
  for(; s != end; ++s)
  {
    SS << "[" << std::format("{:>2}", count) << "] " << s << std::endl;

    switch(using zu = std::size_t; count)
    {
    case 0: assert(s.size() == zu{0}); break;
    case 1:
    case 2:
    case 4:
    case 8: assert(s.size() == zu{1}); break;
    case 3:
    case 5:
    case 6:
    case 9:
    case 10:
    case 12: assert(s.size() == zu{2}); break;
    case 7:
    case 11:
    case 13:
    case 14: assert(s.size() == zu{3}); break;
    case 15: assert(s.size() == zu{4}); break;
    }

    ++count;

  }

  SS << "[" << std::format("{:>2}", count) << "] " << s << std::endl;
  assert(s == end);
  assert(count == 16);

  --s;
//SS << s << std::endl;
  assert(s == rbegin);

  for(; s != rend; --s)
  {
    --count;
    SS << "[" << std::format("{:>2}", count) << "] " << s << std::endl;
  }

  SS << "[" << std::format("{:>2}", count) << "] " << s << std::endl;
  
  assert(s == rend);
  assert(count == 0);

  ++s; assert(s.bits[0] == 0b0'0000); assert(s == begin);
  --s; assert(s.bits[0] == 0b1'0000); assert(s == rend);
  --s; assert(s.bits[0] == 0b0'1111); assert(s == rbegin);
  --s; assert(s.bits[0] == 0b0'1110); assert(s < rbegin);
  ++s; assert(s.bits[0] == 0b0'1111); assert(s == rbegin);
  ++s; assert(s.bits[0] == 0b1'1111); assert(s == end);
  ++s; assert(s.bits[0] == 0b0'0000); assert(s == begin);
  ++s; assert(s.bits[0] == 0b0'0001); assert(s > begin);
}


