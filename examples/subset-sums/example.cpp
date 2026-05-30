/*
  Compiled with

    g++ --std=c++23 -I. examples/subset-sums/example.cpp

  from the ss repo root.
 */
#include "subsets/accumulate.h"
#include <iostream>
#include <algorithm>

int main()
{
  auto map = ss::accumulate({1, 4, 5, 7});

  std::vector<decltype(std::cbegin(map))> index(map.size());
  std::ranges::for_each(index, [i = std::cbegin(map)] (auto& j) mutable { j = i++;});
  std::ranges::sort(index, [](auto i, auto j) { return *i < *j; });
  for(auto i : index)
  {
    std::cout << std::format("{:>3} : {:>3}", i->first, i->second) << std::endl;
  }

  return 0;
}



