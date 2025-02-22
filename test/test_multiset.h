#include <iostream>
#include <cassert>

#define SS std::cout << __func__ << ", " << __LINE__ << ". "

#include "../utils/binomial.h"

#include "../subsets/multiset.h"

void test_multiset_N10_L3();
void test_multiset_N5_L3();
void test_multiset_N5_L8();

template<std::uint64_t N, std::uint64_t L>
void test_multiset();

void test_multiset()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  test_multiset_N10_L3();
  test_multiset_N5_L3();
  test_multiset<5, 8>();
  test_multiset<10, 13>();
  test_multiset<20, 13>();
  test_multiset<20, 4>();
  test_multiset<5, 44>();
}

template<std::uint64_t N, std::uint64_t L>
void test_multiset()
{
  std::cout << std::format("  {}<{}, {}>", __func__, N, L) << std::endl;

  using namespace ss::multi;

  // Multiindex on an N-point set
  index<N> mm;

  std::cout << "    Counting: ";
  begin(mm, L);
  std::uint64_t nsubsets{};
  while(mm.back() != 1)
  {
// SS << " mm = " << mm << std::endl;
    next(mm, L);
    ++nsubsets;
  }
  std::cout << "nsubsets = "     << nsubsets << std::endl;
  assert(nsubsets == ss::utils::C(N + L - 1, L));
}

void test_multiset_N5_L3()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  static constexpr std::uint64_t N{5};
  static constexpr std::uint64_t L{3};

  using namespace ss::multi;
  using index = ss::multi::index<N>; // Multiindex on a 5-point set

  index mm;

  // With length = L
  begin(mm, L);
  assert((mm == index{L, 0, 0, 0, 0, 0}));

  next(mm, L);
  assert((mm == index{L-1, 1, 0, 0, 0, 0}));

  next(mm, L);
  assert((mm == index{L-2, 2, 0, 0, 0, 0}));

  next(mm, L);
  assert((mm == index{0, L, 0, 0, 0, 0}));

  next(mm, L);
  assert((mm == index{2, 0, 1, 0, 0, 0}));

  next(mm, L);
  assert((mm == index{1, 1, 1, 0, 0, 0}));

  next(mm, L);
  assert((mm == index{0, 2, 1, 0, 0, 0}));

  std::cout << "    Counting: ";
  begin(mm, 3);
  int nsubsets{};
  while(mm.back() != 1)
  {
// SS << " mm = " << mm << std::endl;
    next(mm, 3);
    ++nsubsets;
  }
  std::cout << "nsubsets = "     << nsubsets << std::endl;
  assert(nsubsets == ss::utils::C(N + L - 1, L));
}

void test_multiset_N10_L3()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  static constexpr std::uint64_t N{10};
  static constexpr std::uint64_t L{3};

  using namespace ss::multi;

  // Multiindex on a 10-point set
  index<N> mm;

  // With length = L
  begin(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));

  next(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}));

  next(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}));

  next(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0}));

  next(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}));

  // Jump ahead
  mm = {0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0};
// SS << " mm = " << mm << std::endl;

  next(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0}));

  next(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0}));

  next(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0}));

  // Jump ahead
  mm = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0};
// SS << " mm = " << mm << std::endl;

  next(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0})); // last

  next(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1})); // end

  next(mm, 3);
// SS << " mm = " << mm << std::endl;
  assert((mm == index<10>{3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})); // begin

  std::cout << "    Counting: ";
  begin(mm, L);
  int nsubsets{};
  while(mm.back() != 1)
  {
// SS << " mm = " << mm << std::endl;
    next(mm, 3);
    ++nsubsets;
  }
  std::cout << "nsubsets = "     << nsubsets << std::endl;
  assert(nsubsets == ss::utils::C(N + L - 1, L));
}

