#include <unordered_map>
#include <cstdint>
#include <ranges>
#include <functional>

namespace ss
{
  namespace detail
  {
    /*
     * Find the number of occurrences of each sum of subset values from the given `set`.
     * The returned map consists of ordered pairs of values
     *
     *   {s, k[s]}, where s = subset sum, and k[s] = number of subsets with sum s.
     *
     * The returned map always includes an entry {0, 1}, for the empty set.
     *
     * return: A map of subset sums
     */
    std::unordered_map<std::int64_t, std::uint64_t> accumulate(std::ranges::range auto&& set)
    {
      using map = std::unordered_map<std::int64_t, std::uint64_t>;

      map counts{};
      counts[0] = 1;

      for(const auto& x : set)
      {
        map next_counts{};
        for(const auto& pair : counts)
        {
          auto sum = pair.first;
          next_counts[sum + x] += pair.second;
        }

        for(const auto& pair : next_counts)
          counts[pair.first] += pair.second;
      }

      return counts;
    }
  }

  std::unordered_map<std::int64_t, std::uint64_t> accumulate(std::ranges::range auto&& set)
  {
    return detail::accumulate(set);
  }

  template<typename T>
  std::unordered_map<std::int64_t, std::uint64_t> accumulate(std::initializer_list<T>&& list)
  {
    return detail::accumulate(std::forward<std::initializer_list<T>>(list));
  }
}




