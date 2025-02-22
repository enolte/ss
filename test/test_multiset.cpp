#include <iostream>
#include <cassert>

#include <chrono>

#include "../../subsets/multiset.h"

#define SS_TRACE_ON
#include "../trace.h"
#undef SS_TRACE_ON

void test(std::size_t length, std::size_t npoints);

int main()
{
  // test(2, 2);
  // test(4, 3);
  // test(4, 5);
  test(3, 7);
  // test(7, 7);
  // test(9, 7);
  // test(9, 6);
  test(9, 10);
  // test(10, 10);
  // test(10, 6);
  // test(10, 7);
  test(10, 17);
  // test(8, 17);
  // // test(20, 13);
  // test(14, 19);
  // // test(12, 21);
  // // test(12, 20);
  // // test(13, 19);
  // // test(18, 17);
  // // test(40, 5);
  // // test(31, 20); // (long run)

  // TODO (23 May 2021)
  // test_multi_generator(3, 5);


  return 0;
}


void test(std::size_t npoints, std::size_t length)
{
  using clock = std::chrono::high_resolution_clock;

  fe::multiset ms{npoints, length};

  const std::size_t ex_count = fe::utils::C(npoints-1 + length, length);

  std::stringstream ss;
  ss << " -- npoints = " << npoints << ", length = " << length << ", ex. count = " << ex_count << " ------------";
  SS_TRACE_MSG(ss.str().c_str());

  assert(ms.count() == ex_count);

  {
    auto ii = *ms.begin();
    assert(ii.npoints() == npoints);

    // Verify `begin` state: Verify that `ii` == {length, 0, ..., 0};
    assert(ii == *ms.cbegin());
    assert(ii[0] == length);
    assert(ii.nnz() == 1);
  }

/*
  {
    SS_TRACE_MSG(" -- individual iteration test ------------------");

    fe::multiset::iterator i = ms.cbegin();
    assert(i->npoints() == npoints);

    SS_TRACE0(*i);
    SS_TRACE0(i->npoints());
    SS_TRACE0(i->length());
    ++i;
    SS_TRACE0(*i);
    SS_TRACE0(i->npoints());
    SS_TRACE0(i->length());
    ++i;
    SS_TRACE0(*i);
    SS_TRACE0(i->npoints());
    SS_TRACE0(i->length());
  }
*/
  {
    SS_TRACE_MSG("iterator while-loop test");

    const clock::time_point start = clock::now();

    std::size_t ac_count = 0;
    auto i = ms.begin();
    while(i != ms.cend())
    {
      ++ac_count;
      ++i;
    }

    const clock::time_point stop = clock::now();
    const clock::duration loop_time__ns = stop - start;
    SS_TRACE0(loop_time__ns.count());

    auto ii = *i;

    // Verify `end` state
    assert(ii == *ms.cend());
    assert(ii[ii.npoints() - 1] == length);
    assert(ii.nnz() == 1);

    // Verify that `next` has no effect on end state.
    ++ii;
    assert(ii == *ms.cend());
    assert(ii[ii.npoints() - 1] == length);
    assert(ii.nnz() == 1);

    // Verify loop iteration count
    if(ac_count != ex_count)
    {
      SS_TRACE0(ac_count);
      SS_TRACE0(ex_count);
      SS_TRACE0(ii);
    }

    assert(ac_count == ex_count);
  }

  {
    SS_TRACE_MSG("iterator for-loop test");

    const clock::time_point start = clock::now();

    std::size_t ac_count = 0;
    for(auto i = ms.cbegin(); i != ms.cend(); ++i)
    {
      // std::stringstream ss;
      // ss << "[ " << ac_count << " ] " << *i;
      // SS_TRACE_MSG(ss.str().c_str());

      assert(npoints == i->npoints());
      assert(length == i->length());

      ++ac_count;
    }

    const clock::time_point stop = clock::now();
    const clock::duration loop_time__ns = stop - start;
    SS_TRACE0(loop_time__ns.count());

    // Verify loop iteration count
    if(ac_count != ex_count)
    {
      SS_TRACE0(ac_count);
      SS_TRACE0(ex_count);
      // SS_TRACE0(*i);
    }

    assert(ac_count == ex_count);
  }

  {
    SS_TRACE_MSG("range-based for-loop test");

    const clock::time_point start = clock::now();

    std::size_t ac_count = 0;
    for(const auto& ii : ms)
    {
      // std::stringstream ss;
      // ss << "[ " << ac_count << " ] " << ii;
      // SS_TRACE_MSG(ss.str().c_str());

      assert(npoints == ii.npoints());
      assert(length == ii.length());

      ++ac_count;
    }

    const clock::time_point stop = clock::now();
    const clock::duration loop_time__ns = stop - start;
    SS_TRACE0(loop_time__ns.count());

    const auto ii = *ms.cend();

    // Verify loop iteration count
    if(ac_count != ex_count)
    {
      SS_TRACE0(ac_count);
      SS_TRACE0(ex_count);
      SS_TRACE0(ii);
    }

    assert(ac_count == ex_count);

  }

  {
    SS_TRACE_MSG("in-place multiindex iteration loop test");

    const clock::time_point start = clock::now();

    std::size_t ac_count = 0;
    for(fe::multiindex ii(npoints, length); !ii.end; ++ii)
    {
      // std::stringstream ss;
      // ss << "[ " << ac_count << " ] " << ii;
      // SS_TRACE_MSG(ss.str().c_str());

      assert(npoints == ii.npoints());
      assert(length == ii.length());

      ++ac_count;
    }

    const clock::time_point stop = clock::now();
    const clock::duration loop_time__ns = stop - start;
    SS_TRACE0(loop_time__ns.count());

    const auto ii = *ms.cend();

    // Verify loop iteration count
    if(ac_count != ex_count)
    {
      SS_TRACE0(ac_count);
      SS_TRACE0(ex_count);
      SS_TRACE0(ii);
    }

    assert(ac_count == ex_count);

  }



}






/*

// Constant-time look up
void test_get(std::size_t npoints, std::size_t length)
{
  SS_TRACE0(npoints);
  SS_TRACE0(length);

  // TODO (11 January 2021) A test.
  {
    fe::multiset m{npoints, length};
    fe::multiindex ii;

    m.begin(ii);
    // const std::size_t j = fe::subsets::multi::count(npoints, length);
    const std::size_t j = 8;
    for(std::size_t k = 0; k < j; ++k)
    {
      SS_TRACE0(k);

      const fe::multiindex& ex = ii;
      const fe::multiindex ac = m.get(k);

      // assert(ex == ac);

      if(ex != ac)
      {
        SS_TRACE0(ex);
        SS_TRACE0(ac);
      }

      m.next(ii);
    }

  }



  // TODO (11 January 2021) Experimenting.
  {
    fe::multiset m{npoints, length};
    fe::multiindex ii;

    std::size_t ac_count = 0;

    std::size_t cycle = length;
    std::size_t prev_cycle_start = length;

    for(m.begin(ii); !m.is_end(ii); m.next(ii))
    {
      SS_PRINT0(ac_count);
      if(ac_count < 10)
        std::cout << "   " << ii << std::endl;
      else
        std::cout << "  " << ii << std::endl;

      --cycle;
      if(cycle == (std::size_t)-1)
      {
        std::cout << "\n";
        prev_cycle_start--;
        // if(prev_cycle_start == 0)
        cycle = prev_cycle_start;
      }

      ++ac_count;
    }
  }

}

*/