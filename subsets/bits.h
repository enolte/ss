#pragma once

#include <array>
#include <bit>
#include <cstdint>

namespace ss
{
  /*
   * Bitwise ops necessary to manipulate bit sequences in a std::array<uint64_t>.
   */
  namespace impl
  {
    // Number of 8-byte addressable units to store N many bits
    template<std::uint64_t N>
    constexpr std::uint64_t num_units = (N-1)/64 + 1;

    // For a set of size N, store a total of N+1 bits.
    template<std::uint64_t N>
    constexpr std::size_t array_size = num_units<N+1>;


    // Ops on a single 8-byte memory unit (block).
    namespace blocks
    {
      /*
       * Mask of a single bit.
       * If the bit-offset n >= 64, the zero mask is returned.
       */
      constexpr std::uint64_t bit_mask(std::uint64_t n)  { return (n < 64) * ((1ull << n)); }

      /*
       * Mask of [n, 64). Last 64 - n bits.
       * If n >= 64, the zero-mask is returned.
       * If n == 0, the full mask is returned.
       */
      constexpr std::uint64_t tail_mask(std::uint64_t n) { return (n < 64) * (~((1ull << n) - 1)); }

      /*
       * Mask of [0, n). First n bits.
       * If n > 64, the zero-mask is returned.
       * If n == 64, the full mask is returned.
       */
      constexpr std::uint64_t head_mask(std::uint64_t n) { return (n < 64) * ((1ull << n) - 1) + (n == 64) * 0xffff'ffff'ffff'ffff; }

      /*
       * Mask of [begin, end).
       * If begin >= end, the empty mask is returned.
       * If 64 <= begin or 64 <= end, the empty mask is returned.
       */
      constexpr std::uint64_t segment_mask(std::uint64_t begin, std::uint64_t end)
      {
        return tail_mask(begin) & head_mask(end);
      }

      /*
       * Bit segments in a block.
       * head: [0, n)
       * tail: [n, 64)
       * segment: [begin, end)
       */
      constexpr std::uint64_t head(std::uint64_t block, std::uint64_t n) { return head_mask(n) & block; }
      constexpr std::uint64_t tail(std::uint64_t block, std::uint64_t n) { return tail_mask(n) & block; }
      constexpr std::uint64_t segment(std::uint64_t block, std::uint64_t begin, std::uint64_t end)
      {
        return tail_mask(begin) & head_mask(end) & block;
      }

      /*
       * Get the count of ones.
       * For head_ones: return the count of ones in [0, n)
       * For tail_ones: return the count of ones in [n, 64)
       * For ones: return the count of ones in [begin, end)
       */
      constexpr std::uint64_t tail_ones(std::uint64_t block, std::uint64_t n) { return std::popcount(tail(block, n)); }
      constexpr std::uint64_t head_ones(std::uint64_t block, std::uint64_t n) { return std::popcount(head(block, n)); }
      constexpr std::uint64_t ones(std::uint64_t block, std::uint64_t begin, std::uint64_t end) { return std::popcount(segment(block, begin, end));}
    }

    // Ops on bit blocks of an array of bits
    namespace arrays
    {
      constexpr auto& zero(auto& blocks)
      {
        for(auto i{0zu}; i < std::size(blocks); ++i)
          blocks[i] = 0;
        return blocks;
      }

      constexpr std::uint64_t test(const auto& bits, std::uint64_t i)
      {
        // return ((i >> 6) < std::size(bits)) * (bits[i >> 6] & mask(i));
        return bits[i >> 6] & blocks::bit_mask(i & 63);
      }

      /*
       * Get the count of ones in [begin, end) in `blocks`.
       * If either of begin or end is OOR, zero is returned.
       * return: the count of ones in `blocks` in [begin, end)
       */
      constexpr std::uint64_t ones(const auto& blocks, std::uint64_t begin, std::uint64_t end)
      {
        const auto L = 64 * std::size(blocks);
        if(begin >= end || begin >= L || end > L)
        {
          return 0;
        }

        if((begin >> 6) == (end >> 6))
        {
          // TODO 9:14 AM Monday, October 13, 2025. Fix for the case of begin == 0, end == 64
          return blocks::ones(blocks[begin >> 6], begin & 63, end & 63);
        }

        auto i{begin >> 6};
        std::uint64_t acc = blocks::tail_ones(blocks[i], begin & 63);
        for(++i; i < (end >> 6); ++i)
          acc += blocks::ones(blocks[i], 0, 64);

        if((end >> 6) < std::size(blocks))
          acc += blocks::head_ones(blocks[i], end & 63);

        return acc;
      }

      /*
       * Count the 1-bits in [n, L) in `blocks`, where L = 64 * std::size(blocks).
       * If n >= L, zero is returned.
       * return: the count of ones in `blocks` in [n, L)
       */
      constexpr std::uint64_t tail_ones(const auto& blocks, std::uint64_t n)
      {
        if((n >> 6) >= std::size(blocks))
          return 0;

        auto i{n >> 6};
        std::uint64_t acc = blocks::tail_ones(blocks[i], n & 63);
        for(++i; i < std::size(blocks); ++i)
          acc += blocks::ones(blocks[i], 0, 64);
        return acc;
      }

      /*
       * Count the 1-bits in [0, n) in `blocks`.
       * If n > L, zero is returned, where L = 64 * std::size(blocks).
       * return: the count of ones in `blocks` in [0, n)
       */
      constexpr std::uint64_t head_ones(const auto& blocks, std::uint64_t n)
      {
        if((n >> 6) > std::size(blocks))
          return 0;

        auto i{0};
        std::uint64_t acc{};
        for(i; i < (n >> 6); ++i)
          acc += blocks::ones(blocks[i], 0, 64);

        if((n >> 6) < std::size(blocks))
          acc += blocks::head_ones(blocks[i], n & 63);

        return acc;
      }

      // Set the bits at bit-offsets [n, N) to one. Other bits are unchanged.
      constexpr void set1(auto& bits, std::uint64_t n, std::uint64_t N)
      {
        if((n >> 6) == (N >> 6))
        {
          bits[n >> 6] |= blocks::segment_mask(n & 63, N & 63);
          return;
        }

        bits[n >> 6] |= blocks::tail_mask(n & 63);
        for(std::size_t i{(n >> 6) + 1}; i < (N >> 6); ++i)
          bits[i] = 0xffff'ffff'ffff'ffff;
        if((N >> 6) < std::size(bits))
          bits[N >> 6] |= blocks::head_mask(N & 63);
      }

      // Set the bits at bit-offsets [n, N) to zeroes. Other bits are unchanged.
      constexpr void set0(auto& bits, std::uint64_t n, std::uint64_t N)
      {
        if((n >> 6) == (N >> 6))
        {
          bits[n >> 6] &= ~blocks::segment_mask(n & 63, N & 63);
          return;
        }

        // if((n >> 6) < std::size(bits))
          // bits[n >> 6] = blocks::tail_mask(n & 63);
        bits[n >> 6] &= ~blocks::tail_mask(n & 63);
        for(std::size_t i{(n >> 6) + 1}; i < (N >> 6); ++i)
          bits[i] = 0;
        if((N >> 6) < std::size(bits))
          bits[N >> 6] &= ~blocks::head_mask(N & 63);
      }
/*
      // TODO 8:46 PM Friday, October 03, 2025 Finish.
      // Shift bits up by `count`.
      constexpr auto& shift_up(auto& bits, std::uint64_t N, std::uint64_t count)
      {
        if(count == 0)
          return bits;

        if(count >= N)
          return zero(bits);

        for(auto i{0zu}; i < (N >> 6) - (count >> 6); ++i)
          bits[i + (count >> 6)] = bits[i];
        count &= 63;

        if((N >> 6) == 0)
        {
          bits[0] <<= count;
          return bits;
        }

        if(count != 0)
        {
          for(auto i{(N >> 6) - 1}; i != 0; ++i)
          {
            bits[i] <<= count;
            bits[i] |= (bits[i-1] >> (64 - count));
          }
          bits[0] <<= count;
        }

        bits[N >> 6] = blocks::head(bits[N >> 6], N & 63);

        return bits;
      }
*/

    }


    template<typename ostream, std::uint64_t L>
    inline ostream& to_ostream(ostream& o, const std::array<std::uint64_t, L>& bits, std::uint64_t N = 64*L-1)
    {
      // for(auto i{64*L-1}; i > 0; --i)
      for(auto i{N}; i > 0; --i)
      {
        o << (int)(bool)arrays::test(bits, i);
        if(i % 64 == 0)
          o << ' ';
      }
      o << (int)(bool)arrays::test(bits, 0);
      return o;
    }

  }

  /*
   * A bit-packed, dense, binary indicator type. Support ops are in namespace ss::impl.
   * Usages are in namespaces ss::fixed_size and ss::multi.
   */
  template<std::uint64_t N>
  using bits = std::array<std::uint64_t, impl::array_size<N>>;

  template<typename ostream, std::uint64_t N>
  inline ostream& operator<<(ostream& o, const bits<N>& bits)
  {
    return impl::to_ostream(o, bits, N);
  }
}


