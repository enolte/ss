/*
  Compiled with

    g++ --std=c++23 -I. examples/fixed-size-all-subsets/example.cpp

  from the ss repo root.
 */

#include "subsets/fixed_size.h"
#include <iostream>

int main()
{
  int count{};

  using namespace ss::fixed_size;

  // Write to stdout every 3-subset of a 6-set.
  for(subset<6> subset{begin, 3}; subset != end; ++subset)
  {
    std::cout << count << " " << subset << std::endl;
    ++count;
  }

  return 0;
}



