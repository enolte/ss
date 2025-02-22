#include "../../subsets/fixed_size.h"

#include "test_fixed_size_bits_begin.h"
#include "test_fixed_size_bits_end.h"
#include "test_fixed_size_bits_rbegin.h"
#include "test_fixed_size_bits_rend.h"
// #include "test_fixed_size_bits_predicates.h"
#include "test_fixed_size_bits_next.h"
#include "test_fixed_size_bits_prev.h"
// #include "test_fixed_size_bits_random_access.h"

void test_fixed_size_bits()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  test_fixed_size_bits_begin();
  test_fixed_size_bits_end();
  test_fixed_size_bits_rbegin();
  test_fixed_size_bits_rend();
  // test_fixed_size_bits_predicates();
  test_fixed_size_bits_next();
  test_fixed_size_bits_prev();
  // test_fixed_size_bits_random_access();
}
