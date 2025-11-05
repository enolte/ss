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

      constexpr bool is_end(auto& bits, std::uint64_t K, std::uint64_t N)
      {
        return
          ss::impl::arrays::ones(bits, 0, N-K) == 0 &&
          ss::impl::arrays::ones(bits, N-K, N) == K &&
          ss::impl::arrays::test(bits, N);
      }

      constexpr bool is_begin(auto& bits, std::uint64_t K, std::uint64_t N)
      {
        return
          ss::impl::arrays::ones(bits, 0, K) == K &&
          ss::impl::arrays::ones(bits, K, N+1) == 0;
      }

      constexpr bool is_rbegin(auto& bits, std::uint64_t K, std::uint64_t N)
      {
        return
          ss::impl::arrays::ones(bits, 0, N-K) == 0 &&
          ss::impl::arrays::ones(bits, N-K, N) == K &&
          !ss::impl::arrays::test(bits, N);
      }

      constexpr bool is_rend(auto& bits, std::uint64_t K, std::uint64_t N)
      {
        return
          ss::impl::arrays::ones(bits, 0, K) == K &&
          ss::impl::arrays::ones(bits, K, N) == 0 &&
          ss::impl::arrays::test(bits, N);
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

            // Find the first transition ..01.. if it exists.
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

            // Find the first transition ..10.. if it exists.
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

#include "../utils/binomial.h"

namespace ss
{
  namespace fixed_size
  {
    namespace impl
    {
      /*
       * Get the jth K-set from an N-set, in O(K(N-K)) time.
       *
       * TODO 9:40 PM Tuesday, November 04, 2025. This routines resets the
       *      given `bits` index to the `begin` state. General displacements
       *      are in progress.
       *
       * assumes: C(N, K) < 2**64. If this assumption is not satisfied, the result is undefined.
       *
       * return: the subset index stored in `bits`, modified to point to the next jth K-set
       */
      // TODO. 8:07 PM Monday, November 03, 2025. In progress.
      constexpr auto& get(auto& bits, std::uint64_t K, std::uint64_t N, std::uint64_t j)
      {
        using namespace ss::impl::arrays;

        // TODO 10:12 PM Saturday, November 01, 2025. Restarting from `begin` for now.
        begin(bits, K);
// SS << " bits = "; ss::impl::to_ostream(std::cout, bits, N) << std::endl;
        std::uint64_t count{};
        auto pos = K-1;

        j %= utils::C(N, K);

        // K iterations here
        while(true)
        {
// SS << "   pos = " << pos << std::endl;
          const auto k0 = pos;
          auto n0 = pos;

          // Number of subset indices with k0 at the current component, with all following
          // components already identified.
          auto next_stride = utils::C(n0, k0);

          // At most N-K iterations here.
          while(count + next_stride <= j)
          {
            count += next_stride;
// SS << "     [" << pos << "] next_stride = " << next_stride << std::endl;
// SS << "     [" << pos << "] count = " << count << std::endl;
// SS << "     [" << pos << "] n0 = " << n0 << std::endl;

            // Replace the point at the n0'th offset with the next point.
// SS << "     [" << pos << "] bits = "; ss::impl::to_ostream(std::cout, bits, N) << std::endl;
            set0(bits, n0, n0+1);
// SS << "     [" << pos << "] bits = "; ss::impl::to_ostream(std::cout, bits, N) << std::endl;
            set1(bits, n0+1, n0+2);
// SS << "     [" << pos << "] bits = "; ss::impl::to_ostream(std::cout, bits, N) << std::endl;


            // If we've reached the `end` state, reset to `begin` and continue.
            if(n0 == N-1)
            {
// SS << std::endl;
              begin(bits, K);
              pos = K;
              n0 = pos;
              break;
            }

            n0 += 1;
            next_stride = utils::C(n0, k0);
          }

          if(pos == 0)
            break;

          --pos;
        }

        return bits;
      }

    }
  }
}



