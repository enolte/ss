#include <iostream>
#include <cassert>

#include "binomial.h"
#include "array_ostream.h"

#include "../subsets/multiset.h"

template<std::uint64_t N, std::uint64_t L>
void test_multiset_components();

void test_multiset_N5_L3();
// TODO 6:41 PM Tuesday, October 28, 2025.
// void test_multiset_N10_L3();

template<std::uint64_t N, std::uint64_t L>
void test_multiset();

void test_multiset()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  test_multiset_components<3, 5>();
  test_multiset_components<4, 5>();
  test_multiset_components<5, 3>();

  test_multiset_N5_L3();
  // test_multiset_N10_L3();
  test_multiset<5, 8>();
  test_multiset<10, 13>();
  test_multiset<13, 10>();
  test_multiset<20, 4>();
  test_multiset<4, 20>();
  test_multiset<13, 8>();
  test_multiset<8, 13>();
  test_multiset<5, 44>();
  test_multiset<44, 5>();
}

// Basic component conversion from bit sequence
template<std::uint64_t N, std::uint64_t L>
void test_multiset_components()
{
  std::cout << std::format("  {}<{}, {}>", __func__, N, L) << std::endl;

  using namespace ss::multi;

  index<N, L> mm{begin};

  const std::array c = mm.components();
  static_assert(c.size() == N);
  assert(c[0] == L);
  for(int i{1}; i < N; ++i)
    assert(c[i] == 0);
}

template<std::uint64_t N, std::uint64_t L>
void test_multiset()
{
  std::cout << std::format("  {}<{}, {}>", __func__, N, L) << std::endl;

  using namespace ss::multi;

  // Multiindex on an N-point set
  index<N, L> mm{begin};

  std::cout << "    Counting: (expect " << ss::utils::C(N + L - 1, L) << ") " << std::flush;
  std::uint64_t nsubsets{};
  while(!impl::is_end(mm.bits, L, N))
  {
    impl::next(mm.bits, L, N);
    ++nsubsets;
  }
  std::cout << "nsubsets = "     << nsubsets << std::endl;
  assert(nsubsets == ss::utils::C(N + L - 1, L));
}

void test_multiset_N5_L3()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  // 5 = N = set size
  // L = multiindex length
  static constexpr std::uint64_t N{5};
  static constexpr std::uint64_t L{3};

  using namespace ss::multi;
  using components = std::array<std::uint64_t, N>;

  // Expect 35 multiindices, in this order:
  index<N, L> mm{begin};     std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{3, 0, 0, 0, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{2, 1, 0, 0, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 2, 0, 0, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 3, 0, 0, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{2, 0, 1, 0, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 1, 1, 0, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 2, 1, 0, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 2, 0, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 2, 0, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 3, 0, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{2, 0, 0, 1, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 1, 0, 1, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 2, 0, 1, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 1, 1, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 1, 1, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 2, 1, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 0, 2, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 0, 2, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 1, 2, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 0, 3, 0}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{2, 0, 0, 0, 1}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 1, 0, 0, 1}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 2, 0, 0, 1}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 1, 0, 1}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 1, 0, 1}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 2, 0, 1}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 0, 1, 1}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 0, 1, 1}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 1, 1, 1}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 0, 2, 1}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 0, 0, 2}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 0, 0, 2}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 1, 0, 2}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 0, 1, 2}));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 0, 0, 3}));

  // Expect transition into `end` state.
  assert(!impl::is_end(mm.bits, L, N));
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 0, 0, 3}));
  assert(impl::is_end(mm.bits, L, N));

  // Expect transition into `begin` state.
  impl::next(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{3, 0, 0, 0, 0}));
SS << "(mm.bits.size() = " << mm.bits.size() << ") " << mm.bits.back() << std::endl;
  assert(impl::is_begin(mm.bits, L));
  assert(!impl::is_end(mm.bits, L, N));

  std::cout << "    Counting: (expect " << ss::utils::C(N + L - 1, L) << ") " << std::flush;
  mm = begin;
  int nsubsets{};
  while(!impl::is_end(mm.bits, L, N))
  {
    impl::next(mm.bits, L, N);
    ++nsubsets;
  }
  std::cout << "nsubsets = "     << nsubsets << std::endl;
  assert(nsubsets == ss::utils::C(N + L - 1, L));
}


#if 0
void test_multiset_N10_L3()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  static constexpr std::uint64_t N{10};
  static constexpr std::uint64_t L{3};

  using namespace ss::multi;
  using components = std::array<std::uint64_t, N>;

  index<N, L> mm{begin};

  impl::begin(mm.bits, 3);     assert((mm.components() == components{3, 0, 0, 0, 0, 0, 0, 0, 0, 0}));
  impl::next(mm.bits, 3, 10);  assert((mm.components() == components{2, 1, 0, 0, 0, 0, 0, 0, 0, 0}));
  impl::next(mm.bits, 3, 10);  assert((mm.components() == components{1, 2, 0, 0, 0, 0, 0, 0, 0, 0}));
  impl::next(mm.bits, 3, 10);  assert((mm.components() == components{0, 3, 0, 0, 0, 0, 0, 0, 0, 0}));
  impl::next(mm.bits, 3, 10);  assert((mm.components() == components{2, 0, 1, 0, 0, 0, 0, 0, 0, 0}));

  // Jump ahead
  mm = index<N, L>{0, 0, 0, 0, 0, 0, 0, 0, 2, 1};

  impl::next(mm.bits, 3, 10);  assert((mm.components() == components{1, 0, 0, 0, 0, 0, 0, 0, 0, 2}));
  impl::next(mm.bits, 3, 10);  assert((mm.components() == components{0, 1, 0, 0, 0, 0, 0, 0, 0, 2}));
  impl::next(mm.bits, 3, 10);  assert((mm.components() == components{0, 0, 1, 0, 0, 0, 0, 0, 0, 2}));

  // Jump ahead
  mm = index<N, L>{0, 0, 0, 0, 0, 0, 0, 0, 1, 2};

  impl::next(mm.bits, 3, 10);  assert((mm.components() == components{0, 0, 0, 0, 0, 0, 0, 0, 0, 3})); // last
  impl::next(mm.bits, 3, 10);  assert((mm.components() == components{0, 0, 0, 0, 0, 0, 0, 0, 0, 3})); // end
  impl::next(mm.bits, 3, 10);  assert((mm.components() == components{3, 0, 0, 0, 0, 0, 0, 0, 0, 0})); // begin

  std::cout << "    Counting: (expect " << ss::utils::C(N + L - 1, L) << ") " << std::flush;
  mm = begin;
  int nsubsets{};
  while(!impl::is_end(mm.bits, L, N))
  {
    impl::next(mm, 3, 10);
    ++nsubsets;
  }
  std::cout << "nsubsets = "     << nsubsets << std::endl;
  assert(nsubsets == ss::utils::C(N + L - 1, L));
}

#endif
