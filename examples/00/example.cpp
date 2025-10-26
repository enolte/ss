/*
  Compiled with

    g++ --std=c++23 -I. examples/00/example.cpp

  from the ss repo root.
 */

#include "subsets/fixed_size.h"
#include <iostream>

int main()
{
  int count{};

  // Select subsets of size 65 from a set of 89 points.
  for(ss::fixed_size::subset<89> subset{ss::fixed_size::begin, 65}; subset != ss::fixed_size::end; ++subset)
  {
    std::cout << subset << std::endl;

    if(++count == 10)
      break;
  }

  return 0;
}



