#include <iostream>
#include <cassert>

#include "../../subsets/fixed_size.h"

void test_fixed_size_subsets_construct();
void test_fixed_size_subsets_ordering();
void test_fixed_size_subsets_states();
void test_fixed_size_subsets_state_transitions();
void test_fixed_size_subsets_random_access();

void test_fixed_size_subsets()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  test_fixed_size_subsets_construct();
  test_fixed_size_subsets_ordering();
  test_fixed_size_subsets_state_transitions();
  test_fixed_size_subsets_states();

  test_fixed_size_subsets_random_access();
}

// TODO 8:25 AM Thursday, October 09, 2025
void test_fixed_size_subsets_random_access()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  using namespace ss::fixed_size;

  subset<7> s{begin, 4};

  while(s != end)
  {
SS << s << std::endl;
    ++s;
  }
}

// TODO 3:56 PM Saturday, October 04, 2025 This could be more thorough.
void test_fixed_size_subsets_states()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  ss::fixed_size::subset<89> subset{ss::fixed_size::begin, 65};

  assert(!subset.is_end());
  assert(subset.is_begin());

  assert(subset.bits[0] == 0xffff'ffff'ffff'ffff);
  assert(subset.bits[1] == 0x0000'0000'0000'0001);

  subset.bits[0] = 1;
  assert(!subset.is_begin());

  subset.bits[0] = 0xffff'ffff'ffff'ffff;
  assert(subset.is_begin());
}

void test_fixed_size_subsets_construct()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  ss::fixed_size::subset<89> s1{ss::fixed_size::begin, 65};
  ss::fixed_size::subset s2{ss::fixed_size::subset<89>::choose<65>(ss::fixed_size::begin)};

  assert(s1.bits == s2.bits);
  assert(s1.bits.size() == 2);

  using namespace ss::fixed_size;


  // Construct from sentinel type:

  {
    subset<8> s3{begin, 4};
    assert(s3 == begin);

    subset s4{subset<8>::choose<4>(begin)};
    assert(s4 == begin);
  }

  {
    subset<8> s3{end, 4};
    assert(s3 == end);

    subset s4{subset<8>::choose<4>(end)};
    assert(s4 == end);
  }

  {
    subset<8> s3{rend, 4};
    assert(s3 == rend);

    subset s4{subset<8>::choose<4>(rend)};
    assert(s4 == rend);
  }

  {
    subset<8> s3{rbegin, 4};
    assert(s3 == rbegin);

    subset s4{subset<8>::choose<4>(rbegin)};
    assert(s4 == rbegin);
  }
}

void test_fixed_size_subsets_state_transitions_forward();
void test_fixed_size_subsets_state_transitions_backward();

void test_fixed_size_subsets_state_transitions()
{
  test_fixed_size_subsets_state_transitions_forward();
  test_fixed_size_subsets_state_transitions_backward();

  using namespace ss::fixed_size;

  {
    auto s = subset<771>::choose<500>(rend);
    assert(s == rend);
    ++s; assert(s == begin);    // rend --> begin
    --s; assert(s == rend);     // rend <-- begin
    ++s; assert(s == begin);    // rend --> begin
  }


  {
    auto s = subset<784>::choose<517>(end);
    assert(s == end);
    ++s; assert(s == begin);    //    end --> begin
    --s; assert(s == rend);     //   rend <-- begin
    ++s; assert(s == begin);    //   rend --> begin
    --s; assert(s == rend);     //   rend <-- begin
    --s; assert(s == rbegin);   // rbegin <-- rend
    ++s; assert(s == end);      // rbegin --> end
    ++s; assert(s ==  begin);   //    end --> begin
    --s; assert(s == rend);     //   rend <-- begin
    ++s; assert(s == begin);    //   rend --> begin
    --s; assert(s == rend);     //   rend <-- begin
    --s; assert(s == rbegin);   // rbegin <-- rend
    ++s; assert(s ==  end);     // rbegin --> end
    ++s; assert(s ==  begin);   //    end --> begin

  }
}

void test_fixed_size_subsets_state_transitions_forward()
{
  using namespace ss::fixed_size;

  auto s = ss::fixed_size::subset<7>::choose<5>(rend);

  auto state = [](const auto& s)
  {
      if (s == begin)
        return "begin  ";
      else if (s == end)
        return "end    ";
      else if (s == rbegin)
        return "rbegin ";
      else if (s == rend)
        return "rend   ";
      else
        return "ELSE   ";
  };


  // rbegin --> end --> begin
  {
    auto s = ss::fixed_size::subset<7>::choose<5>(rbegin);

    std::cout << "    rbegin -->  end" << std::endl;

    assert(s == rbegin);
    ++s;
    assert(s == end);

    // s = subset<7>::choose<5>(end);

    std::cout << "    end    -->  begin" << std::endl;
    assert(s == end);
    ++s;
    assert(s == begin);
  }


  // rend   -->  begin
  {
    s = subset<7>::choose<5>(rend);

    std::cout << "    rend   --> begin" << std::endl;
    assert(s == rend);
    ++s;
    assert(s == begin);
  }
}



void test_fixed_size_subsets_state_transitions_backward()
{
  using namespace ss::fixed_size;

  auto state = [](const auto& s)
  {
      if (s == begin)
        return "begin  ";
      else if (s == end)
        return "end    ";
      else if (s == rbegin)
        return "rbegin ";
      else if (s == rend)
        return "rend   ";
      else
        return "ELSE   ";
  };

  {
    auto s = subset<5>::choose<2>(rbegin);
    assert(     s.bits[0] == 0b011000); assert(s == rbegin);
    --s; assert(s.bits[0] == 0b010100); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b010010); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b010001); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b001100); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b001010); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b001001); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b000110); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b000101); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b000011); assert(s ==  begin);
    --s; assert(s.bits[0] == 0b100011); assert(s == rend);
    --s; assert(s.bits[0] == 0b011000); assert(s == rbegin);
  }

  // TODO 4:41 PM Saturday, October 04, 2025. impl::arrays::prev doesn't handle K == 1 correctly.
/*
  {
    auto s = subset<6>::choose<1>(rbegin);
    assert(     s.bits[0] == 0b0100000); assert(s == rbegin);
    --s; assert(s.bits[0] == 0b0010000); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b0001000); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b0000100); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b0000010); assert(s != rbegin);
    --s; assert(s.bits[0] == 0b0000001); assert(s ==  begin);
SS << s << std::endl;
    --s;
SS << s << std::endl;
    assert(s.bits[0] == 0b1000001); assert(s == rend);
    --s; assert(s.bits[0] == 0b0100000); assert(s == rbegin);
  }
*/

  // rbegin <-- rend <-- begin
  {
    // rend   <-- begin
    auto s = subset<7>::choose<5>(begin);
    std::cout << "    rend   <-- begin" << std::endl;

    assert(s == begin);
    --s;
    assert(s == rend);


    // rbegin <-- rend
    std::cout << "    rbegin <-- rend" << std::endl;

    assert(s == rend);
    --s;
    assert(s == rbegin);
  }


  // rbegin <-- end
  {
    auto s = subset<7>::choose<5>(end);
    std::cout << "    rbegin <-- end" << std::endl;

    assert(s == end);
    --s;
    assert(s == rbegin);
  }
}

void test_fixed_size_subsets_ordering()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  using namespace ss::fixed_size;

  subset<89> subset{begin, 65};

  assert(subset == subset);
  assert(subset == begin);
  assert(begin == subset);

  assert(subset <= subset);
  assert(subset <= begin);
  assert(begin <= subset);

  assert(subset >= subset);
  assert(subset >= begin);
  assert(begin >= subset);

  assert(!(subset < subset));
  assert(!(subset < begin));
  assert(!(begin < subset));

  assert(!(subset > subset));
  assert(!(subset > begin));
  assert(!(begin > subset));

  assert(!(subset != subset));
  assert(subset != end);
  assert(end != subset);
  assert(subset != rbegin);
  assert(rbegin != subset);
  assert(subset != rend);
  assert(rend!= subset);

  ++subset;

  assert(!(subset > subset));
  assert(subset > begin);
  assert(!(begin > subset));

  assert(subset <= subset);
  assert(!(subset <= begin));
  assert(begin <= subset);

  assert(subset >= subset);
  assert(subset >= begin);
  assert(!(begin >= subset));

  assert(!(subset < subset));
  assert(!(subset < begin));
  assert(begin < subset);

  assert(subset != begin);
  assert(begin != subset);
  assert(subset != end);
  assert(end != subset);
  assert(subset != rbegin);
  assert(rbegin != subset);
  assert(subset != rend);
  assert(rend!= subset);
}

