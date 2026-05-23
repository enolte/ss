#define SS std::cout << __func__ << ", " << __LINE__ << ". "

#include "../utils/binomial.h"

#include "test_bits.h"
#include "test_accumulate.h"
#include "test_multiset.h"
#include "fixed_size/test_fixed_size.h"
#include "power_set/test_power_set.h"

int main()
{
  test_bits();
  test_power_set();
  test_accumulate();
  test_multiset();
  test_fixed_size();

  std::cout << "end tests" << std::endl;

  return 0;
}

