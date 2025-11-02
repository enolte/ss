#include <iostream>
#include <cassert>

#include "../../subsets/fixed_size.h"

void test_fixed_size_subsets_construct();
void test_fixed_size_subsets_ordering();
void test_fixed_size_subsets_states();
void test_fixed_size_subsets_state_transitions();
void test_fixed_size_subsets_random_access_N5_K3();
void test_fixed_size_subsets_random_access_N7_K4();

void test_fixed_size_subsets()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  test_fixed_size_subsets_construct();
  test_fixed_size_subsets_ordering();
  test_fixed_size_subsets_state_transitions();
  test_fixed_size_subsets_states();

  test_fixed_size_subsets_random_access_N5_K3();
  // test_fixed_size_subsets_random_access_N7_K4();
}

void test_fixed_size_subsets_random_access_N5_K3()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  using namespace ss::fixed_size;


  // TODO 3:56 PM Saturday, November 01, 2025. C(7, 4) = 35
  // C(5, 3) = 10
  subset<5> s{begin, 3};

  int count{};
  while(s != end)
  {
    std::cout << "[" << (count < 10 ? " " : "") << count << "] " << s << std::endl;
    ++count;
    ++s;
  }

  std::array<int, 3> offsets{};

  s.get( 0); assert(s.bits[0] == 0b000111);
  s.get( 1); assert(s.bits[0] == 0b001011);
  s.get( 2); assert(s.bits[0] == 0b001101);
  s.get( 3); assert(s.bits[0] == 0b001110);
  s.get( 4); assert(s.bits[0] == 0b010011);
  s.get( 5); assert(s.bits[0] == 0b010101);
  s.get( 6); assert(s.bits[0] == 0b010110);
  s.get( 7); assert(s.bits[0] == 0b011001);
  s.get( 8); assert(s.bits[0] == 0b011010);
  s.get( 9); assert(s.bits[0] == 0b011100);

  // Expect set 0
  s.get(10); assert(s.bits[0] == 0b000111);

  // Expect set 1
  // TODO 8:00 PM Monday, November 03, 2025. Not working yet.
SS << std::endl;
  s.get(11); //assert(s.bits[0] == 0b001011);
  {
    s.components(offsets.begin());
SS << s << "  " << offsets << std::endl;
    // assert((offsets == std::array{3, 4, 5}));
    // assert(s == end);
  }
}

void test_fixed_size_subsets_random_access_N7_K4()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  using namespace ss::fixed_size;


  // C(7, 4) = 35
  subset<7> s{begin, 4};

  int count{};
  while(s != end)
  {
    std::cout << "[" << (count < 10 ? " " : "") << count << "] " << s << std::endl;
    ++count;
    ++s;
  }

  std::array<int, 3> offsets{};

  s.get(0);   assert(s.bits[0] == 0b00001111);
  s.get(1);   assert(s.bits[0] == 0b00010111);
  s.get(2);   assert(s.bits[0] == 0b00011011);
  s.get(3);   assert(s.bits[0] == 0b00011101);
  s.get(4);   assert(s.bits[0] == 0b00011110);
  s.get(5);   assert(s.bits[0] == 0b00100111);
  s.get(6);   assert(s.bits[0] == 0b00101011);
  s.get(7);   assert(s.bits[0] == 0b00101101);
  s.get(8);   assert(s.bits[0] == 0b00101110);
  s.get(9);   assert(s.bits[0] == 0b00110011);
  int i = 9;
  s.get(++i); assert(s.bits[0] == 0b00110101);
  s.get(++i); assert(s.bits[0] == 0b00110110);
  s.get(++i); assert(s.bits[0] == 0b00111001);
  s.get(++i); assert(s.bits[0] == 0b00111010);
  s.get(++i); assert(s.bits[0] == 0b00111100);
  s.get(++i); assert(s.bits[0] == 0b01000111);
  s.get(++i); assert(s.bits[0] == 0b01001011);
  s.get(++i); assert(s.bits[0] == 0b01001101);
  s.get(++i); assert(s.bits[0] == 0b01001110);
  s.get(++i); assert(s.bits[0] == 0b01010011);
  s.get(++i); assert(s.bits[0] == 0b01010101);
  s.get(++i); assert(s.bits[0] == 0b01010110);
  s.get(++i); assert(s.bits[0] == 0b01011001);
  s.get(++i); assert(s.bits[0] == 0b01011010);
  s.get(++i); assert(s.bits[0] == 0b01011100);
  s.get(++i); assert(s.bits[0] == 0b01100011);
  s.get(++i); assert(s.bits[0] == 0b01100101);
  s.get(++i); assert(s.bits[0] == 0b01100110);
  s.get(++i); assert(s.bits[0] == 0b01101001);
  s.get(++i); assert(s.bits[0] == 0b01101010);
  s.get(++i); assert(s.bits[0] == 0b01101100);
  s.get(++i); assert(s.bits[0] == 0b01110001);
SS << std::endl;
  s.get(++i); assert(s.bits[0] == 0b01110010);
SS << std::endl;
  s.get(++i); assert(s.bits[0] == 0b01110100);
SS << std::endl;
  s.get(++i); assert(s.bits[0] == 0b01111000);


  // TODO 10:08 PM Saturday, November 01, 2025. Fix transition into `end` state.
SS << std::endl;
SS << s << "  " << offsets << std::endl;
  s.get(++i); // assert(s.bits[0] == 0b11111000);
  {
    s.components(offsets.begin());
SS << s << "  " << offsets << std::endl;
    // assert((offsets == std::array{3, 4, 5}));
    // assert(s == end);
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

