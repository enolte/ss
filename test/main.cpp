#define SS std::cout << __func__ << ", " << __LINE__ << ". "

#include "../utils/binomial.h"

#include "test_bits.h"
#include "test_accumulate_positive.h"
#include "test_multiset.h"
#include "fixed_size/test_fixed_size_subsets.h"
#include "fixed_size/test_fixed_size_bits.h"

int main()
{
  test_bits();
  test_accumulate_positive();
  test_multiset();
  test_fixed_size_bits();
  test_fixed_size_subsets();

  std::cout << "end tests" << std::endl;

  return 0;
}

