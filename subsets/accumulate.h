#pragma once

#include <unordered_map>
#include <cstdint>
#include <concepts>
#include <ranges>
#include <algorithm>

#if 1
#include <unordered_set>
namespace ss
{
  //                                          subset
  std::unordered_map<std::uint64_t, std::unordered_set<std::uint64_t>> accumulate(std::ranges::range auto set, std::predicate auto p)
  {
    // TODO return: The set of subsets which satisfies the predicate
    return {};
  }
}
#endif

namespace ss
{
  /*
   * Like the above, but assume `set` has only non-negative integer values.
   */
  std::unordered_map<std::int64_t, std::uint64_t> accumulate_unsigned(std::ranges::range auto);


  /*
   * Find the number of occurrences of each sum of subset values from the given `set`.
   *
   * The returned map consists of ordered pairs of values
   *
   *   {s, k[s]}, where s = subset sum, and k[s] = number of subsets with sum s.
   *
   * The returned map always includes an entry with s == 0, for the empty set.
   *
   * Time complexity: Θ(n * s * log(s)),
   *
   *  n = number of points in set
   *  s = number of distinct subset sums to compute
   *
   *
   * return: A map of subset sums
   */
#define VERSION 2
#if VERSION == 0
  std::unordered_map<std::int64_t, std::uint64_t> accumulate(std::ranges::range auto set)
  {
    const auto size = std::distance(std::cbegin(set), std::cend(set));

    if(std::cbegin(set) == std::cend(set))
      return {{0, 1}};

    using map = std::unordered_map<std::int64_t, std::uint64_t>;

    map counts{};
    counts[0] = 1;

    using X = std::remove_reference_t<decltype(*set.begin())>;

    int zero_count{};

    for(auto x : set)
    {
      map next_counts{};
      for(auto& pair : counts)
      {
        auto sum = pair.first;
        next_counts[sum + x] += 1;
      }

      for(const auto& p : next_counts)
        counts[p.first] += p.second;
    }

    return counts;
  }

#elif VERSION == 1
  std::unordered_map<std::int64_t, std::uint64_t> accumulate(std::ranges::range auto set)
  {
    const auto size = std::distance(std::cbegin(set), std::cend(set));

    if(std::cbegin(set) == std::cend(set))
      return {{0, 1}};

    using map = std::unordered_map<std::int64_t, std::uint64_t>;

    map counts{};
    counts[0] = 1;

    using X = std::remove_reference_t<decltype(*set.begin())>;

    int zero_count{0};

    for(X x : set)
    {
      if(x == X{})
      {
        ++zero_count;
        std::cout << " ** zero_count = " << zero_count << std::endl;
        continue;
      }

      map next_counts{};
      for(auto& pair : counts)
      {
        auto sum = pair.first;
        next_counts[sum + x] += 1;
      }

      for(const auto& p : next_counts)
        counts[p.first] += p.second;
    }

    for(auto& pair : counts)
    {
      pair.second += (1 << zero_count) - 1;
    }

    return counts;
  }
#elif VERSION == 2
/*
Reduce by multiplicities of input values
*/
  std::unordered_map<std::int64_t, std::uint64_t> accumulate(std::ranges::range auto set)
  {
    const auto size = std::distance(std::cbegin(set), std::cend(set));

    if(std::cbegin(set) == std::cend(set))
      return {{0, 1}};

    using map = std::unordered_map<std::int64_t, std::uint64_t>;

    using X = std::remove_reference_t<decltype(*set.begin())>;

    map value_counts{};
    for(X x : set)
      value_counts[x] += 1;

    for(auto v : value_counts)
      std::cout << " ** [" << v.first << "] " << v.second << std::endl;

    map counted{};

    map counts{};
    counts[0] = 1 << value_counts[0];
    counted.emplace(0, 0);
    // for(X x : set)
      // counts[x] += 1 << value_counts[x];

    for(X x : set)
    {
      if(counted.contains(x))
      {
        continue;
      }

      counted.emplace(x, 0);

      std::cout << " **  x = " << x << std::endl;

      map next_counts{};
      for(auto& pair : counts)
      {
        auto sum = pair.first;
        next_counts[sum + x] += (1 << value_counts[sum]);
        // next_counts[sum + x] += counts[x];
        // next_counts[sum + x] += value_counts[x];
        // counts[sum + x] += counts[x];
      }

      for(const auto& p : next_counts)
        counts[p.first] += p.second;
    }

    // if(counts[0] > 1)
      // counts[0] -= 1;

    return counts;
  }
#endif

/*

  7:01 PM Wednesday, July 30, 2025 Hand-worked test case:

  S = {1, 2, 3}
  sums(S):

  counts                       [0] = 1 inserted (initial)

  outer loop iterations:
      x = 1
      inner loop iterations:
          after  1:  [1 + 0] = [1] = 1 inserted

      result: { [0] = 1, [1] = 1 }

      x = 2
      inner loop iterations:
          after  1:  [2 + 0] = [2] = 1 inserted
          after  2:  [2 + 1] = [3] = 1 inserted

      result: { [0] = 1, [1] = 1, [2] = 1, [3] = 1 }

      x = 3
      inner loop iterations:
          after  1:  [3 + 0] = [3] = 2
          after  2:  [3 + 1] = [4] = 1 inserted
          after  3:  [3 + 2] = [5] = 1 inserted
          after  4:  [3 + 3] = [6] = 1 inserted



      result: { [0] = 1, [1] = 1, [2] = 1, [3] = 2, [4] = 1, [5] = 1, [6] = 1}

                {}       {1}      {2}      {3}      {1, 3}   {2, 3}   {1, 2, 3}
                                           {1, 2}

*/

}


