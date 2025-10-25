#include "bits.h"

namespace ss
{
  namespace fixed_size
  {
    namespace impl
    {
      constexpr void begin(auto& bits, std::uint64_t K)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set1(bits, 0, K);
      }

      constexpr void end(auto& bits, std::uint64_t K, std::uint64_t N)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set1(bits, N-K, N+1);
      }

      constexpr void rbegin(auto& bits, std::uint64_t K, std::uint64_t N)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set1(bits, N-K, N);
      }

      constexpr void rend(auto& bits, std::uint64_t K, std::uint64_t N)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set1(bits, 0, K);
        bits[N >> 6] |= 1ull << (N & 63);
      }

      constexpr auto& next(auto& bits, std::uint64_t K, std::uint64_t N)
      {
        using namespace ss::impl;
        using namespace ss::impl::arrays;

        std::uint64_t j{}; // bit counter
        bool prev{false};
        for(std::size_t i{}; i != bits.size(); ++i)
        {
          const auto q = bits[i];
          if(prev && ((q & 1) == 0))
          {
            break;
          }
          else
          {
            j += (i != 0);

            // If we're in either `rbegin` or `end` state @entry, we're done with the loop.
            if(j == N-1 && test(bits, j))
              break;

            std::uint64_t m{};
            for(; m < 63 && j != N-1; ++m, ++j)
            {
              if(((q >> m) & 3) == 1)
                break;
            }

            if(m < 63 || j == N-1)
              break;

            prev = (q >> 63) & 1;
          }
        }

        // If the first transition ..01.. was found at K-1, then we're either in
        // `begin` or `rend` state@entry. Bit[N] determines which.
        //   If `rend`, move to `begin` here.
        //   If `begin` state, move to next state below this branch.
        if(j == K-1)
        {
          if(test(bits, N))
          {
            // `rend`@entry --> `begin`
            bits[N >> 6] &= ~blocks::bit_mask(N & 63);
            return bits;
          }
        }

        // If no transition ...01... was found in bits@entry, we're in the `rbegin` state
        // or `end` state. The next bit determines which.
        //   If at `rbegin` subset @entry, move to `end` state.
        //   If already in `end` state@entry, move to `begin` state. This case is done below this branch.
        if(j == N-1)
        {
          if(test(bits, j) && !test(bits, N))
          {
            // `rbegin` --> `end`
            bits[N >> 6] |= blocks::bit_mask(N & 63);
            return bits;
          }
        }

        // Toggle bits j and j+1
        bits[j >> 6] ^= blocks::bit_mask(j & 63);
        bits[(j+1) >> 6] ^= blocks::bit_mask((j+1) & 63);

        const auto ones_from_j = tail_ones(bits, j);
        set1(bits, 0, K - ones_from_j);
        set0(bits, K - ones_from_j, j);

        return bits;
      }

      constexpr auto& prev(auto& bits, std::uint64_t K, std::uint64_t N)
      {
        using namespace ss::impl;
        using namespace ss::impl::arrays;

        std::uint64_t j{}; // bit counter
        bool prev{true};
        for(std::size_t i{}; i != bits.size(); ++i)
        {
          const auto q = bits[i];
          if(!prev && ((q & 1) == 1))
          {
            break;
          }
          else
          {
            j += (i != 0);

            if(j == K-1 && test(bits, j))
              break;

            std::uint64_t m{};
            for(; m < 63 && j != N-1; ++m, ++j)
            {
              if(((q >> m) & 3) == 2)
              {
                break;
              }
            }

            if(m < 63 || j == N-1)
              break;

            prev = (q >> 63) & 1;
          }
        }

        // If the first bit change 0 to 1 is at N-K-1, we're in either
        // `rbegin` or `end` state@entry. Bit[N] determines which.
        // If `rbegin`, move to next state below this branch.
        // If `end`, transition to `rbegin` here.
        if(j == N-K-1)
        {
          if(test(bits, N))
          {
            bits[N >> 6] &= ~blocks::bit_mask(N & 63);
            return bits;
          }
        }

        if(j == N-1)
        {
          if(!test(bits, j) && !test(bits, N))
          {
            bits[N >> 6] |= blocks::bit_mask(N & 63);
            return bits;
          }
        }

        bits[j >> 6] ^= blocks::bit_mask(j & 63);
        bits[(j+1) >> 6] ^= blocks::bit_mask((j+1) & 63);

        const auto ones_from_j = tail_ones(bits, j);
        set1(bits, j - (K - ones_from_j), j);
        set0(bits, 0, j - (K - ones_from_j));

        return bits;
      }

    }
  }
}



