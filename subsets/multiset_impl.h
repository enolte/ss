#include "bits.h"

namespace ss
{
  namespace multi
  {
    namespace impl
    {
      constexpr void begin(auto& counts, std::uint64_t L)
      {
        for(auto l{0zu}; l != L; ++l)
        {
          ss::impl::arrays::zero(counts[l]);
          counts[l][0] = 1;
        }
      }

      constexpr void end(auto& counts, std::uint64_t L, std::uint64_t N)
      {
        for(auto l{0zu}; l != L; ++l)
        {
          ss::impl::arrays::zero(counts[l]);
          ss::impl::arrays::set1(counts[l], N-1, N);
        }
        // ss::impl::arrays::set1(counts[0], N, N+1);
        counts[0][N >> 6] |= 1ull << (N & 63);
      }

      constexpr void rbegin(auto& counts, std::uint64_t L, std::uint64_t N)
      {
        for(auto l{0zu}; l != L; ++l)
        {
          ss::impl::arrays::zero(counts[l]);
          ss::impl::arrays::set1(counts[l], N-1, N);
        }
      }

      constexpr void rend(auto& counts, std::uint64_t L, std::uint64_t N)
      {
        for(auto l{0zu}; l != L; ++l)
        {
          ss::impl::arrays::zero(counts[l]);
          counts[l][0] = 1;
        }
        // ss::impl::arrays::set1(counts[0], N, N+1);
        counts[0][N >> 6] |= 1ull << (N & 63);
      }

      constexpr bool is_begin(const auto& counts, std::size_t L)
      {
        for(auto l{0}; l < L; ++l)
        {
          if(counts[l][0] != 1)
            return false;

          for(auto i{1}; i != std::size(counts[0]); ++i)
            if(counts[l][i] != 0)
              return false;
        }

        return true;
      }

      // TODO 7:56 PM Sunday, October 12, 2025.
/*
      constexpr bool is_rbegin(const auto& counts, std::size_t L)
      {
        return
          counts.back() == L
          &&
          std::ranges::all_of(counts.cbegin(), counts.cend()-2, [](auto& m){ return m == 0; });
      }

      constexpr bool is_end(const auto& counts)
      {
        return counts.back() == 1;
      }

      constexpr bool is_rend(const auto& counts)
      {
        return counts.back() == 1;
      }
*/
      constexpr auto& next(auto& counts, std::size_t L)
      {
        constexpr auto L = std::size(counts);

        // Find a non-zero memory unit at lowest echelon.
        auto i{0zu};
        for(; i < std::size(counts[0]); ++i)
          if(counts[0][i] != 0)
            break;

        // Find the bit offset, j, in counts[0] where the first 1 occurs.
        // This is the first component we're about to change.
        const auto j = 64*i + std::countl_zero(counts[0][i]);

        // Get the highest echelon at offset j which is a one.
        auto l{L-1};
        for(; l != 0; --l)
          if(ss::impl::arrays::test(counts[l], j))
            break;

        // Get the highest echelon at offset j+1 which is a one.
        auto e1{L-1};
        for(; e1 != 0; --e1)
          if(ss::impl::arrays::test(counts[e1], j+1))
            break;

        if(e1 != 0 || ss::impl::arrays::test(counts[0], j+1))
          ++e1;

        // Toggle 2 bits
        ss::impl::arrays::set0(counts[e], j, j+1);
        ss::impl::arrays::set1(counts[e1], j+1, j+2);

        return counts;
      }
/*
      // TODO 6:58 PM Sunday, October 12, 2025.
      constexpr auto& prev(auto& mm, std::size_t L)
      {
        if(mm[0] > 0)
        {
          mm[0]--;
        }

        auto i{0};
        for(; i != std::size(mm) - 2; ++i)
        {
          if(mm[i] > 0)
          {
            mm[i]--;

            auto j{i+1};
            for(; j < std::size(mm) - 2 && mm[j] != 0; ++j);

            mm[j-1] = L;
            std::fill_n(std::begin(mm), i+1, 0);
            mm[std::size(mm) - 2] = L - std::accumulate(std::cbegin(mm), std::cbegin(mm) + i, 0);
            return mm;
          }
        }
      }
*/
    }
  }
}

