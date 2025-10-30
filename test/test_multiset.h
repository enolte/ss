#include <iostream>
#include <cassert>

#include "binomial.h"
#include "array_ostream.h"

#include "../subsets/multiset.h"

template<std::uint64_t N, std::uint64_t L>
void test_multiset_components();

template<std::uint64_t N, std::uint64_t L>
void test_multiset();

template<std::uint64_t N, std::uint64_t L>
void test_iteration_cycles();

void test_multiset()
{

  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  test_multiset_components<3, 5>();
  test_multiset_components<4, 5>();
  test_multiset_components<5, 3>();

  test_iteration_cycles<31, 51>();
  test_iteration_cycles<41, 51>();
  test_iteration_cycles<51, 31>();

  test_multiset<2, 2>();
  test_multiset<4, 2>();
  test_multiset<2, 4>();
  test_multiset<3, 5>();
  test_multiset<5, 3>();
  test_multiset<5, 8>();
  test_multiset<6, 10>();
  test_multiset<10, 6>();
  test_multiset<10, 13>();
  test_multiset<13, 10>();
  test_multiset<20, 4>();
  test_multiset<4, 20>();
  test_multiset<13, 8>();
  test_multiset<8, 13>();
  test_multiset<5, 44>();
  test_multiset<44, 5>();
}

template<std::uint64_t N, std::uint64_t L>
void test_iteration_cycles()
{
  std::cout << std::format("  {}<{}, {}>", __func__, N, L) << std::endl;

  using namespace ss::multi;

  index<N, L> mm{begin};
  assert(mm == begin);
  assert(mm.is_begin());

  --mm;
  assert(mm == rend);
  assert(mm.is_rend());

  --mm;
  assert(mm == rbegin);
  assert(mm.is_rbegin());

  ++mm;
  assert(mm == end);
  assert(mm.is_end());

  ++mm;
  assert(mm == begin);
  assert(mm.is_begin());
}

// Basic component conversion from bit sequence
template<std::uint64_t N, std::uint64_t L>
void test_multiset_components()
{
  std::cout << std::format("  {}<{}, {}>", __func__, N, L) << std::endl;

  using namespace ss::multi;

  index<N, L> mm{begin};

  std::array c = mm.components();
  static_assert(c.size() == N);

  assert(c[0] == L);
  for(int i{1}; i < N; ++i)
    assert(c[i] == 0);

  ++mm;
  c = mm.components();
  assert(c[0] == L-1);
  assert(c[1] == 1);
  for(int i{2}; i < N; ++i)
    assert(c[i] == 0);

  mm = rbegin;
  c = mm.components();
  assert(c[N-1] == L);
  for(int i{0}; i < N-1; ++i)
    assert(c[i] == 0);

  --mm;
  c = mm.components();
  assert(c[N-1] == L-1);
  assert(c[N-2] == 1);
  for(int i{0}; i < N-2; ++i)
    assert(c[i] == 0);
}


template<std::uint64_t N, std::uint64_t L>
void test_multiset()
{
  std::cout << std::format("  {}<{}, {}>", __func__, N, L) << std::endl;

  using namespace ss::multi;

  constexpr bool write_to_stdout = ss::utils::C(N + L - 1, L) < 40;

  // Multiindex of length L on an N-point set
  index<N, L> mm{begin};
  if constexpr(write_to_stdout)
    std::cout << "    Counting forward: (expect " << ss::utils::C(N + L - 1, L) << ") " << std::endl;
  else
    std::cout << "    Counting forward: (expect " << ss::utils::C(N + L - 1, L) << ") " << std::flush;

// SS << mm << "  " << mm.components() << std::endl;
  std::uint64_t nsubsets{};
  while(!impl::is_end(mm.bits, L, N))
  {
    if constexpr(write_to_stdout)
    {
      std::cout << mm << "  " << mm.components() << std::endl;
    }

    impl::next(mm.bits, L, N);
    ++nsubsets;
  }
  std::cout << "nsubsets = "     << nsubsets << std::endl;
  assert(nsubsets == ss::utils::C(N + L - 1, L));

  if constexpr(write_to_stdout)
    std::cout << "    Counting backward: (expect " << ss::utils::C(N + L - 1, L) << ") " << std::endl;
  else
    std::cout << "    Counting backward: (expect " << ss::utils::C(N + L - 1, L) << ") " << std::flush;

  nsubsets = {};
  mm = rbegin;
// SS << mm << "  " << mm.components() << std::endl;
  while(!impl::is_rend(mm.bits, L, N))
  {
    if constexpr(write_to_stdout)
    {
      std::cout << mm << "  " << mm.components() << std::endl;
    }

    impl::prev(mm.bits, L, N);
    ++nsubsets;

    // Sanity check during initial dev.
    if(nsubsets == ss::utils::C(N + L - 1, L)+2)
      break;
  }
  std::cout << "nsubsets = "     << nsubsets << std::endl;
  assert(nsubsets == ss::utils::C(N + L - 1, L));
}


#if 0
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
  assert(impl::is_begin(mm.bits, L, N));
  assert(!impl::is_end(mm.bits, L, N));

  std::cout << "    Count forward: (expect " << ss::utils::C(N + L - 1, L) << ") " << std::flush;
  mm = begin;
  int nsubsets{};
  while(!impl::is_end(mm.bits, L, N))
  {
    impl::next(mm.bits, L, N);
    ++nsubsets;
  }
  std::cout << "nsubsets = "     << nsubsets << std::endl;
  assert(nsubsets == ss::utils::C(N + L - 1, L));



  // Expect 35 multiindices, in this order:
  mm = rbegin;               std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 0, 0, 3}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 0, 1, 2}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 1, 0, 2}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 0, 0, 2}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 0, 0, 2}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 0, 2, 1}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 1, 1, 1}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 0, 1, 1}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 0, 1, 1}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 2, 0, 1}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 1, 0, 1}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 1, 0, 1}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 2, 0, 0, 1}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 1, 0, 0, 1}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{2, 0, 0, 0, 1}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 0, 3, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 1, 2, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 0, 2, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 0, 2, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 2, 1, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 1, 1, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 1, 1, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 2, 0, 1, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 1, 0, 1, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{2, 0, 0, 1, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 3, 0, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 1, 2, 0, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 0, 2, 0, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 2, 1, 0, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 1, 1, 0, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{2, 0, 1, 0, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 3, 0, 0, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{1, 2, 0, 0, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{2, 1, 0, 0, 0}));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{3, 0, 0, 0, 0}));


  // Expect transition into `rend` state.
  assert(!impl::is_rend(mm.bits, L, N));
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{3, 0, 0, 0, 0}));
  assert(!impl::is_rbegin(mm.bits, L, N));
  assert(impl::is_rend(mm.bits, L, N));

  // Expect transition into `rbegin` state.
  impl::prev(mm.bits, L, N); std::cout << mm << "  " << mm.components() << std::endl; assert((mm.components() == components{0, 0, 0, 0, 3}));
  assert(impl::is_rbegin(mm.bits, L, N));
  assert(!impl::is_rend(mm.bits, L, N));

  std::cout << "    Count backward: (expect " << ss::utils::C(N + L - 1, L) << ") " << std::flush;
  mm = rbegin;
  nsubsets = 0;
  while(!impl::is_rend(mm.bits, L, N))
  {
    impl::prev(mm.bits, L, N);
    ++nsubsets;

    // Sanity check during initial dev.
    if(nsubsets == 35+2)
      break;
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
