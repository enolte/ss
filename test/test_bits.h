#include "../subsets/bits.h"

#include <iostream>
#include <cassert>

void test_bits_array_size();
void test_bits_mask_1();
void test_bits_segment_mask();
void test_bits_tail_ones();
void test_bits_set1();

void test_bits()
{
  std::cout << "────── " << __func__ << " ─────────────────────────────────────────\n";

  test_bits_array_size();
  test_bits_mask_1();
  test_bits_segment_mask();
  test_bits_tail_ones();
  test_bits_set1();
}

void test_bits_set1()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  using namespace ss;
  using namespace ss::impl::arrays;

  {
    ss::bits<7> bits{};
    set1(bits, 2, 6);
    assert(bits[0] == 0b011'1100); // rbegin
  }

  {
    bits<16> bits{};
    set1(bits, 9, 16);
    assert(bits[0] == 0b00'11111110'00000000);
  }

  {
    bits<42> bits{};
    set1(bits, 23, 42);
    assert(bits[0] == 0b011'11111'11111'11111'11000'00000'00000'00000'00000);
  }

  {
    bits<42> bits{};
    set1(bits, 37, 42);
    assert(bits[0] == 0b011'11100'00000'00000'00000'00000'00000'00000'00000);
  }

  {
    bits<64> bits{};
    set1(bits, 0, 2);
    assert(bits[0] == 0b011);
    assert(bits[1] == 0);
  }

  {
    bits<65> bits{};
    set1(bits, 0, 2);
    assert(bits[0] == 0b011);
    assert(bits[1] == 0);
  }

  {
    bits<77> bits{};
    set1(bits, 0, 12);
    assert(bits[0] == 0b011'11111'11111);
    assert(bits[1] == 0);
  }

  {
    bits<77> bits{};
    set1(bits,0,  66);
    assert(bits[0] == 0xffff'ffff'ffff'ffff);
    assert(bits[1] == 0b011);
  }

  {
    bits<77> bits{};
    set1(bits, 65, 77);
    assert(bits[0] == 0);
    assert(bits[1] == 0b111'11111'11110);
  }

  {
    bits<77> bits{};
    set1(bits, 11, 77);
    assert(bits[0] == 0b11111111'11111111'11111111'11111111'11111111'11111111'11111000'00000000); // 53 bits
    assert(bits[1] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00011111'11111111); // 13 bits
/*                      76543210 98765432 10987654 32109876 54321098 76543210 98765432 10987654
                        22222222 11111111 11000000 00009999 99999988 88888888 77777777 77666666
                        11111111 11111111 11111111 1111                          |
                                                                                 last
*/

  }

  {
    bits<177> bits{};
    set1(bits, 103, 177);
    assert(bits[0] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
/*                      32109876 54321098 76543210 98765432 10987654 32109876 54321098 76543210
                        66665555 55555544 44444444 33333333 33222222 22221111 111111
*/
    assert(bits[1] == 0b11111111'11111111'11111111'10000000'00000000'00000000'00000000'00000000); // 25 bits
/*                      76543210 98765432 10987654 32109876 54321098 76543210 98765432 10987654
                        22222222 11111111 11000000 00009999 99999988 88888888 77777777 77666666
                        11111111 11111111 11111111 1111
*/
    assert(bits[2] == 0b00000000'00000001'11111111'11111111'11111111'11111111'11111111'11111111); // 49 bits
/*                      10987654 32109876 54321098 76543210 98765432 10987654 32109876 54321098
                        99888888 88887777 77777766 66666666 55555555 55444444 44443333 33333322
                        11111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111
                                        |
                                        last
*/
  }

  {
    bits<177> bits{};
    set1(bits, 3, 177);
    assert(bits[0] == 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111000); // 61 bits
/*                      32109876 54321098 76543210 98765432 10987654 32109876 54321098 76543210
                        66665555 55555544 44444444 33333333 33222222 22221111 111111
*/
    assert(bits[1] == 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111); // 64 bits
/*                      76543210 98765432 10987654 32109876 54321098 76543210 98765432 10987654
                        22222222 11111111 11000000 00009999 99999988 88888888 77777777 77666666
                        11111111 11111111 11111111 1111
*/
    assert(bits[2] == 0b00000000'00000001'11111111'11111111'11111111'11111111'11111111'11111111); // 49 bits
/*                      10987654 32109876 54321098 76543210 98765432 10987654 32109876 54321098
                        99888888 88887777 77777766 66666666 55555555 55444444 44443333 33333322
                        11111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111
                                        |
                                        last
*/
  }

  {
    bits<177> bits{};
    set1(bits, 103, 121);
    assert(bits[0] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
/*                      32109876 54321098 76543210 98765432 10987654 32109876 54321098 76543210
                        66665555 55555544 44444444 33333333 33222222 22221111 111111
*/
    assert(bits[1] == 0b00000001'11111111'11111111'10000000'00000000'00000000'00000000'00000000); // 25 bits
/*                      76543210 98765432 10987654 32109876 54321098 76543210 98765432 10987654
                        22222222 11111111 11000000 00009999 99999988 88888888 77777777 77666666
                        11111111 11111111 11111111 1111
                              │└───────────────────┘
                              │
                              end
*/
    assert(bits[2] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
  }

  {
    bits<199> bits{};
    set1(bits, 0, 196);
    assert(bits[0] == 0xffff'ffff'ffff'ffff);
    assert(bits[1] == 0xffff'ffff'ffff'ffff);
    assert(bits[2] == 0xffff'ffff'ffff'ffff);
    assert(bits[3] == 0b01111);
  }
}

void test_bits_tail_ones()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  {
    using namespace ss::impl;
    ss::bits<77> bits{};

    // bits[0] = 0b01111010'00111100'11010010'01001001'11111111'00101110'00001100'00111110;
    bits[0] = 0b11111111'00000000'11111111'00000000'11111111'00000000'11111111'00000000;
/*              32109876 54321098 76543210 98765432 10987654 32109876 54321098 76543210
                66665555 55555544 44444444 33333333 33222222 22221111 11111100 00000000
*/
    // bits[1] = 0b01101110'01100110'00010111'01111111'11110011'00110111'00110100'00001111;
    bits[1] = 0b00000000'00000000'00000000'00000000'00000000'00000000'00001111'00001111;
/*              76543210 98765432 10987654 32109876 54321098 76543210 98765432 10987654
                22222222 11111111 11000000 00009999 99999988 88888888 77777777 77666666
                11111111 11111111 11111111 1111                          |
                                                                         last
*/

    assert(arrays::tail_ones(bits, 71) == 4);
/*
    result:

            4 ones
              │
           ┌──┴───┐
        0000'1111'0000'1111'1111111100000000111111110000000011111111000000001111111100000000
mask:   0001'1111'1000'0000
        9876 5432 1098 7654 3210987654321098765432109876543210987654321098765432109876543210
        7777 7777 7766 6666 6666555555555544444444443333333333222222222211111111110000000000
          ││      │
          ││      └ 71
          │└ last = 76
          └─ end  = 77
*/

    assert(arrays::tail_ones(bits, 70) == 4);
    assert(arrays::tail_ones(bits, 69) == 4);
    assert(arrays::tail_ones(bits, 68) == 4);
    assert(arrays::tail_ones(bits, 67) == 5);
    assert(arrays::tail_ones(bits, 66) == 6);
    assert(arrays::tail_ones(bits, 65) == 7);
    assert(arrays::tail_ones(bits, 64) == 8);
    assert(arrays::tail_ones(bits, 60) == 12);


    assert(arrays::tail_ones(bits, 59) == 13);
/*
    result:

                  13 ones
                     │
           ┌─────────┴──────────┐
        0000'1111'0000'1111'1111111100000000111111110000000011111111000000001111111100000000
mask 1: 0001'1111'1111'1111
mask 0:                     1111100000000000000000000000000000000000000000000000000000000000
        9876 5432 1098 7654 3210987654321098765432109876543210987654321098765432109876543210
        7777 7777 7766 6666 6666555555555544444444443333333333222222222211111111110000000000
          ││                    │
          ││                    └ 59
          │└ last = 76
          └─ end  = 77
*/

  }

  {
    ss::bits<42> bits{};
    bits[0] = 0b011'11111'11111'11111'11111'10000'00000'00000'00000;

    assert(ss::impl::arrays::tail_ones(bits, 10) == 23);
    assert(ss::impl::arrays::tail_ones(bits, 30) == 12);
  }
}

void test_bits_mask_1()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  using namespace ss::impl;

  assert(blocks::bit_mask(0) == 1ull);
  assert(blocks::bit_mask(2) == 1ull << 2);
  assert(blocks::bit_mask(29) == 1ull << 29);
  assert(blocks::bit_mask(62) == 1ull << 62);
  assert(blocks::bit_mask(63) == 1ull << 63);

  // Bit-offset >= 64 --> empty mask.
  assert(blocks::bit_mask(64) == 0);
  assert(blocks::bit_mask(65) == 0);
  assert(blocks::bit_mask(77) == 0);
  assert(blocks::bit_mask(200305587) == 0);
}

void test_bits_segment_mask()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  using namespace ss::impl;

  // Spot checks
  assert(blocks::segment_mask( 2,  6) == 0b0111100);
  assert(blocks::segment_mask(11, 20) == 0b0'11111'11110'00000'00000);
  assert(blocks::segment_mask( 0, 63) == 0x7fff'ffff'ffff'ffff);
  assert(blocks::segment_mask( 7, 63) == 0x7fff'ffff'ffff'ff80);
  assert(blocks::segment_mask( 0, 59) == 0x07ff'ffff'ffff'ffff);
  assert(blocks::segment_mask(23, 59) == 0x07ff'ffff'ff80'0000);
  assert(blocks::segment_mask(29, 39) == 0x0000'007f'e000'0000);
  assert(blocks::segment_mask( 2, 7)  == 0b1111100);
  assert(blocks::segment_mask( 7, 13) == 0b01111110000000);

  assert(blocks::segment_mask(45, 64)               == 0xffff'e000'0000'0000);
  assert(blocks::segment_mask(45 + 1088, 64 + 1088) == 0);
  assert(blocks::segment_mask(301, 320)             == 0);

  // [n, 64), n < 64
  assert(blocks::segment_mask( 0, 64) == 0xffff'ffff'ffff'ffff);
  assert(blocks::segment_mask( 5, 64) == 0xffff'ffff'ffff'ffe0);
  assert(blocks::segment_mask(60, 64) == 0xf000'0000'0000'0000);
  assert(blocks::segment_mask(23, 64) == 0xffff'ffff'ff80'0000);

  // [0, n), n < 64
  assert(blocks::segment_mask(  0,   0)    == 0);
  assert(blocks::segment_mask(  0,   5)    == 0b11111);
  assert(blocks::segment_mask(  0,   7)    == 0x0000'0000'0000'007f);
  assert(blocks::segment_mask(  0,  13)    == 0x0000'0000'0000'1fff);
  assert(blocks::segment_mask(  0,  21)    == 0x0000'0000'001f'ffff);

  // [0, n), n >= 64
  assert(blocks::segment_mask(  0,  77)    == 0);
  assert(blocks::segment_mask(  0, 192)    == 0);
  assert(blocks::segment_mask(128, 192)    == 0);
  assert(blocks::segment_mask(640, 704)    == 0);
  assert(blocks::segment_mask(640, 960)    == 0);

  // [n, n)
  assert(blocks::segment_mask(33, 33)   == 0);
  assert(blocks::segment_mask(607, 607) == 0);
  assert(blocks::segment_mask(608, 608) == 0);

  // b > e --> empty mask
  assert(blocks::segment_mask(29, 13)      == 0);
  assert(blocks::segment_mask(64, 69)      == 0);
  assert(blocks::segment_mask(128, 133)    == 0);
  assert(blocks::segment_mask(1024, 1029)  == 0);

  // [b, e), b < 64 < e
  assert(blocks::segment_mask(29, 77) == 0);
  assert(blocks::segment_mask(59, 77) == 0);

  // [b, e), 64 < b <= e
  assert(blocks::segment_mask(71, 77)  == 0);
  assert(blocks::segment_mask(81, 81)  == 0);
  assert(blocks::segment_mask(93, 77)  == 0);
  assert(blocks::segment_mask(93, 141) == 0);
  assert(blocks::segment_mask(130, 135)         == 0);
  assert(blocks::segment_mask(2 + 704, 7 + 704) == 0);
}

// This function is just a name to write to stdout for these static_asserts
void test_bits_array_size()
{
  std::cout << std::format("  {}", __func__) << std::endl;

  static_assert(ss::bits<1>{}.size() == 1);
  static_assert(ss::bits<62>{}.size() == 1);
  static_assert(ss::bits<63>{}.size() == 1);
  static_assert(ss::bits<64>{}.size() == 2);
  static_assert(ss::bits<65>{}.size() == 2);
  static_assert(ss::bits<66>{}.size() == 2);
  static_assert(ss::bits<127>{}.size() == 2);
  static_assert(ss::bits<128>{}.size() == 3);
  static_assert(ss::bits<129>{}.size() == 3);
  static_assert(ss::bits<130>{}.size() == 3);
  static_assert(ss::bits<191>{}.size() == 3);
  static_assert(ss::bits<192>{}.size() == 4);
  static_assert(ss::bits<193>{}.size() == 4);
  static_assert(ss::bits<712>{}.size() == 12);
}

