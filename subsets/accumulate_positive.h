#include <unordered_map>
#include <cstdint>
// #include <concepts>
#include <ranges>
#include <functional>

namespace ss
{
  /*
   * requires: Only positive point values, and no duplicate values
   *
   * Find the number of occurrences of each sum of subset values from the given `set`.
   *
   * The returned map consists of ordered pairs of values
   *
   *   {s, k[s]}, where s = subset sum, and k[s] = number of subsets with sum s.
   *
   * The returned map always includes an entry {0, 1}, for the empty set.
   *
   * return: A map of subset sums
   */
  std::unordered_map<std::int64_t, std::uint64_t> accumulate_positive(std::ranges::range auto set)
  {
    if(std::cbegin(set) == std::cend(set))
      return {{0, 1}};

    using map = std::unordered_map<std::int64_t, std::uint64_t>;

    map counts{};
    counts[0] = 1;

    for(const auto& x : set)
    {
      map next_counts{};
      for(const auto& pair : counts)
      {
        auto sum = pair.first;
        next_counts[sum + x] += 1;
      }

      for(const auto& p : next_counts)
        counts[p.first] += p.second;
    }

    return counts;
  }

}


