#include "../subsets/accumulate_positive.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <set>
#include <cassert>

void test_accumulate_positive(std::ranges::range auto set, std::unordered_map<std::int64_t, std::uint64_t> ex = {});

void test_accumulate_positive()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  using set = std::set<std::int64_t>;
  using ilist = std::initializer_list<std::int64_t>;

  test_accumulate_positive(set{}, {{0, 1}});
  test_accumulate_positive(set{  3}, {{0, 1}, {3, 1}});
  test_accumulate_positive(set{  1}, {{0, 1}, {1, 1}});
  test_accumulate_positive(set{  1}, {{0, 1}, {1, 1}});
  test_accumulate_positive(set{ -1}, {{0, 1}, {-1, 1}});
  test_accumulate_positive(set{-15}, {{0, 1}, {-15, 1}});


  test_accumulate_positive(set{1, 2}, {{0, 1}, {1, 1}, {2, 1}, {3, 1}});
  test_accumulate_positive(set{1, 3}, {{0, 1}, {1, 1}, {3, 1}, {4, 1}});

  test_accumulate_positive(set{1, 2, 3}, {{0, 1}, {1, 1}, {2, 1}, {3, 2}, {4, 1}, {5, 1}, {6, 1}});
  test_accumulate_positive(set{1, 2, 17});
  test_accumulate_positive(set{1, 2, 5}, {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}});
  test_accumulate_positive(set{2, 3, 5}, {{0, 1}, {2, 1}, {3, 1}, {5, 2}, {7, 1}, {8, 1}, {10, 1}});
  test_accumulate_positive(set{2, 3, 10000}, {{0, 1}, {2, 1}, {3, 1}, {5, 1}, {10000, 1}, {10002, 1}, {10003, 1}, {10005, 1}});
  test_accumulate_positive(set{9998, 9999, 10000});

  test_accumulate_positive(set{1, 2, 3, 4});
  test_accumulate_positive(set{4, 1, 2, 3});

  // test_accumulate_positive(set{1, 2, 3, 4, 5});
  // test_accumulate_positive(set{1, 2, 3, 4, 5, 6});
  test_accumulate_positive(set{1, 2, 3, 4, 5, 6, 17});

  // Expect all ones
  // test_accumulate_positive(set{1, 2, 4, 8, 16, 32});
}

void test_accumulate_positive(std::ranges::range auto set, std::unordered_map<std::int64_t, std::uint64_t> ex)
{
  // std::cout << "________________________________________\n" << std::endl;

  

  std::cout << "{";
  if(set.begin() != set.end())
  {
    auto i{std::cbegin(set)};
    while(std::next(i) != std::cend(set)) { std::cout << *i << ", "; ++i; }
    std::cout << *i;
  }
  std::cout << "}" << std::endl;

  const auto ac = ss::accumulate_positive(set);
  using ac_pair = decltype(ac)::value_type;

  using pair = std::pair<std::int64_t, std::uint64_t>;
  std::vector<pair> ac_sorted(ac.cbegin(), ac.cend());
  std::ranges::sort(ac_sorted, {}, &pair::first);

  std::vector<pair> ex_sorted(ex.cbegin(), ex.cend());
  std::ranges::sort(ex_sorted, {}, &pair::first);

  if(!ex.empty())
  {
    if(ac_sorted != ex_sorted)
    {
      const auto num_subsets_opt = std::ranges::fold_left_first(ac_sorted, [](const auto& p, auto acc) { return pair{{}, acc.second + p.second}; });
      const auto num_subsets = num_subsets_opt->second;

      std::cout << "[FAIL] actual: " << num_subsets << " subsets, " << ac.size() << " sums:\n";
      const auto max_width = std::to_string(std::prev(ac_sorted.cend())->first).length();

      for(const auto& p : ac_sorted)
        std::cout << std::format("[{:>3}] {}\n", p.first, p.second);

      std::cout << std::endl;
    }

    // assert(ac_sorted == ex_sorted);
  }
}
