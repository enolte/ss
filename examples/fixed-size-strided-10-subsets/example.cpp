/*
  Compiled with

    g++ --std=c++23 -I. examples/fixed-size-strided-10-subsets/example.cpp

  from the ss repo root.
 */

#include "subsets/fixed_size.h"
#include <iostream>

int main()
{
  int count{};

  // Select successive 1000th subsets of size 65 from a set of 89 points.
  // Stop after 10 subsets.
  for(ss::fixed_size::subset<89> subset{ss::fixed_size::begin, 65}; subset != ss::fixed_size::end; ++subset)
  {
    if(count % 1000 == 0)
      std::cout << subset << std::endl;

    if(++count == 20000)
      break;
  }

  return 0;
}



