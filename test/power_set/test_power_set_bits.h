#include "../../subsets/power_set.h"

#include "test_power_set_bits_begin.h"
#include "test_power_set_bits_end.h"
#include "test_power_set_bits_rbegin.h"
#include "test_power_set_bits_rend.h"
#include "test_power_set_bits_next.h"
#include "test_power_set_bits_prev.h"

void test_power_set_bits()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  test_power_set_bits_begin();
  test_power_set_bits_end();
  test_power_set_bits_rbegin();
  test_power_set_bits_rend();
  test_power_set_bits_next();
  test_power_set_bits_prev();
}
