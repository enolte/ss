#include "../subsets/accumulate.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <set>
#include <cassert>

void test_accumulate_positive_unique();
void test_accumulate_nonnegative_unique();
void test_accumulate_nonnegative_nonunique();
void test_demo_case();
void test_accumulate_unique();
void test_accumulate_nonunique();

void test_accumulate()
{
  test_demo_case();
  test_accumulate_nonunique();
  test_accumulate_unique();
  test_accumulate_nonnegative_nonunique();
  test_accumulate_nonnegative_unique();
  test_accumulate_positive_unique();
}

void test_accumulate(std::ranges::range auto set, std::unordered_map<std::int64_t, std::uint64_t> ex = {});

void test_demo_case()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  using set = std::set<std::int64_t>;

  test_accumulate(set{1, 4, 5, 7},
    {
      {0, 1},
      {1, 1},
      {4, 1},
      {5, 2},
      {6, 1},
      {7, 1},
      {8, 1},
      {9, 1},
      {10, 1},
      {11, 1},
      {12, 2},
      {13, 1},
      {16, 1},
      {17, 1},
    }
   );
}

void test_accumulate_nonunique()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  using ilist = std::initializer_list<std::int64_t>;

  test_accumulate(ilist{-1, 0, -1, 4, 5, 7},
    {
               // TODO 10:33 Wednesday, May 27, 2026. Table incomplete.
               // 0       1                2                                 3                                         4                                  5                              6
               // ──  ──────────  ──────────────────  ─────────────────────────────────────────────────  ──────────────────────────────  ────────────────────────────────────  ─────────────────────
      {-2, 2}, //                 {-1, -1},           {-1,  0, -1}
      {-1, 4}, //     {-1}, {-1}, {-1,  0}, { 0, -1}
      { 0, 2}, // {}, {0}
      { 2, 2}, //                                     {-1, -1,  4},                                      {-1,  0, -1, 4}
      { 3, 6}, //                 {-1,  4},           { 0, -1,  4}, {-1, -1, 5}, {-1, 0, 5}, {-1, 0, 4}, {-1,  0, -1, 5}
      { 4, 6}, //     { 4},       { 0,  4}, {-1,  5}, {-1,  0,  5}, {0, -1, 5}
      { 5, 4}, //     { 5},       { 0,  5},           {-1, -1,  7},                                      {-1,  0, -1, 7}
      { 6, 4}, //                 {-1,  7},           {-1, 0, 7}, {0, -1, 7}
      { 7, 4}, //     { 7},       { 0,  7},                                                              {-1, -1,  4, 5},                {-1, 0, -1, 4, 5}
      { 8, 4}, //                                     {-1,  4,  5}, {-1, 4, 5},                          {-1,  0,  4, 5}, {-1, 0, 4, 5}
      { 9, 4}, //                 { 4,  5},           { 0,  4,  5}
      {10, 6}, //                                     {-1,  4,  7}, {-1, 4, 7},                          {-1,  0,  4, 7}, {0, -1, 4, 7}
      {11, 6}, //                 { 4,  7},           {-1,  5,  7}, {0, 4, 7}, {-1, 5, 7}
      {12, 2}, //                 { 5,  7},           { 0,  5,  7}
      {14, 2}, //                                                                                                                        {-1, -1, 4, 5, 7},                    {-1,  0, -1, 4, 5, 7},
      {15, 4}, //                                                                                        {-1,  4,  5, 7}, {-1, 4, 5, 7}, {-1,  0, 4, 5, 7}, { 0, -1, 4, 5, 7}
      {16, 2}, //                                     { 4,  5,  7},                                      { 0,  4,  5, 7}
    }
   );
}

void test_accumulate_unique()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  using set = std::set<std::int64_t>;

  test_accumulate(set{-1, 4, 5, 7},
    {
      {-1, 1}, // {-1}
      { 0, 1}, // {}
      { 3, 1}, // {-1, 4}
      { 4, 2}, // {4}, {-1, 5}
      { 5, 1}, // {5}
      { 6, 1}, // {-1, 7}
      { 7, 1}, // {7}
      { 8, 1}, // {-1, 4, 5}
      { 9, 1}, // {4, 5}
      {10, 1}, // {-1, 4, 7}
      {11, 2}, // {4, 7}, {-1, 5, 7}
      {12, 1}, // {5, 7}
      {15, 1}, // {-1, 4, 5, 7}
      {16, 1}, // {4, 5, 7}
    }
   );
}

void test_accumulate_nonnegative_nonunique()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  using ilist = std::initializer_list<std::int64_t>;

  test_accumulate(ilist{}, {{0, 1}});
  test_accumulate(ilist{0, 0}, {{0, 4}});
  test_accumulate(ilist{0, 0, 0, 0, 0, 0}, {{0, 64}});
  test_accumulate(ilist{0, 0, 1}, {{0, 4}, {1, 4}});
  test_accumulate(ilist{0, 0, 10}, {{0, 4}, {10, 4}});
  test_accumulate(ilist{0, 0, 3, 2}, {{0, 4}, {3, 4}, {2, 4}, {5, 4}});
  test_accumulate(ilist{0, 0, 3, 0, 2, 0, 0}, {{0, 32}, {3, 32}, {2, 32}, {5, 32}});
  test_accumulate(ilist{0, 1, 1}, {{0, 2}, {2, 2}, {1, 4}});
  test_accumulate(ilist{0, 1, 1, 1}, {{0, 2}, {2, 6}, {1, 6}, {3, 2}});
  test_accumulate(ilist{5, 5, 5, 5}, {{0, 1}, {5, 4}, {10, 6}, {15, 4}, {20, 1}});
}

void test_accumulate_nonnegative_unique()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  using set = std::set<std::int64_t>;

  test_accumulate(set{}, {{0, 1}});
  test_accumulate(set{0}, {{0, 2}});
  test_accumulate(set{0, 1}, {{0, 2}, {1, 2}});
  test_accumulate(set{0, 10}, {{0, 2}, {10, 2}});
  test_accumulate(set{0, 3, 2}, {{0, 2}, {3, 2}, {2, 2}, {5, 2}});
  test_accumulate(set{3, 2, 0}, {{0, 2}, {3, 2}, {2, 2}, {5, 2}});
  test_accumulate(set{0, 3, 6}, {{0, 2}, {3, 2}, {6, 2}, {9, 2}});
  test_accumulate(set{0, 3, 1, 6}, {{0, 2}, {1, 2}, {4, 2}, {7, 2}, {3, 2}, {6, 2}, {9, 2}, {10, 2}});
  test_accumulate(set{0, 1, 2, 4}, {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}});
  test_accumulate(set{1, 0, 2, 4}, {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}});
}

void test_accumulate_positive_unique()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  using set = std::set<std::int64_t>;

  test_accumulate(set{}, {{0, 1}});
  test_accumulate(set{  3}, {{0, 1}, {3, 1}});
  test_accumulate(set{  1}, {{0, 1}, {1, 1}});
  test_accumulate(set{  1}, {{0, 1}, {1, 1}});
  test_accumulate(set{ -1}, {{0, 1}, {-1, 1}});
  test_accumulate(set{-15}, {{0, 1}, {-15, 1}});


  test_accumulate(set{1, 2}, {{0, 1}, {1, 1}, {2, 1}, {3, 1}});
  test_accumulate(set{1, 3}, {{0, 1}, {1, 1}, {3, 1}, {4, 1}});

  test_accumulate(set{1, 2, 3}, {{0, 1}, {1, 1}, {2, 1}, {3, 2}, {4, 1}, {5, 1}, {6, 1}});
  test_accumulate(set{1, 2, 17});
  test_accumulate(set{1, 2, 5}, {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}});
  test_accumulate(set{2, 3, 5}, {{0, 1}, {2, 1}, {3, 1}, {5, 2}, {7, 1}, {8, 1}, {10, 1}});
  test_accumulate(set{2, 3, 10000}, {{0, 1}, {2, 1}, {3, 1}, {5, 1}, {10000, 1}, {10002, 1}, {10003, 1}, {10005, 1}});
  test_accumulate(set{9998, 9999, 10000},
    {
      {0, 1},
      {9998, 1},
      {9999, 1},
      {10000, 1},
      {9998 + 9999, 1},
      {9998 + 10000, 1},
      {9999 + 10000, 1},
      {9998 + 9999 + 10000, 1},
    });

  test_accumulate(set{1, 2, 3, 4});
  test_accumulate(set{4, 1, 2, 3});

  // test_accumulate(set{1, 2, 3, 4, 5});
  // test_accumulate(set{1, 2, 3, 4, 5, 6});
  test_accumulate(set{1, 2, 3, 4, 5, 6, 17});

  // Expect all ones
  test_accumulate(set{1, 2, 4, 8},
    {
      {0, 1},
      {1, 1},
      {2, 1},
      {3, 1},
      {4, 1},
      {5, 1},
      {6, 1},
      {7, 1},
      {8, 1},
      {9, 1},
      {10, 1},
      {11, 1},
      {12, 1},
      {13, 1},
      {14, 1},
      {15, 1},
    });
}

void test_accumulate(std::ranges::range auto set, std::unordered_map<std::int64_t, std::uint64_t> ex)
{
  std::cout << "{";
  if(set.begin() != set.end())
  {
    auto i{std::cbegin(set)};
    while(std::next(i) != std::cend(set)) { std::cout << *i << ", "; ++i; }
    std::cout << *i;
  }
  std::cout << "}" << std::endl;

  const auto ac = ss::accumulate(set);

  std::uint64_t acc{};
  for(const auto& p : ac)
    acc += p.second;
  assert(acc == 1ull << (set.size()));

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
      // const auto max_width = std::to_string(std::prev(ac_sorted.cend())->first).length();

      for(const auto& p : ac_sorted)
        std::cout << std::format("[{:>3}] {}\n", p.first, p.second);

      std::cout << std::endl;
    }

    assert(ac_sorted == ex_sorted);
  }

}
