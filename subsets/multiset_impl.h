#include "bits.h"
#include "../test/array_ostream.h"
namespace ss
{
  namespace multi
  {
    namespace impl
    {
      inline std::uint64_t W(std::size_t L) { return std::bit_width(L); }

      constexpr void begin(auto& bits, std::uint64_t L)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set_value(bits, 0, W(L), L);
      }

      constexpr void end(auto& bits, std::uint64_t L, std::uint64_t N)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set1(bits, L*W(L) - L, L*W(L)+1);
      }

      // TODO 7:05 PM Tuesday, October 28, 2025. Test.
      constexpr void rbegin(auto& bits, std::uint64_t L, std::uint64_t N)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set1(bits, L*W(L) - L, L*W(L));
      }

      // TODO 7:05 PM Tuesday, October 28, 2025. Test.
      constexpr void rend(auto& bits, std::uint64_t L, std::uint64_t N)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set1(bits, 0, W(L));
        ss::impl::arrays::set1(bits, W(L), W(L)+1);
      }

      constexpr bool is_begin(const auto& bits, std::size_t L)
      {
        return
          ss::impl::arrays::test(bits, W(L)) == 0 &&
          ss::impl::arrays::get_value(bits, 0, W(L)) == L;
      }

      // TODO 7:56 PM Sunday, October 12, 2025.
/*
      constexpr bool is_rbegin(const auto& bits, std::size_t L)
      {
        return
          counts.back() == L
          &&
          std::ranges::all_of(counts.cbegin(), counts.cend()-2, [](auto& m){ return m == 0; });
      }
*/
      constexpr bool is_end(const auto& bits, std::size_t L, std::uint64_t N)
      {
        return
          ss::impl::arrays::test(bits, W(L)*N) != 0 &&
          ss::impl::arrays::get_value(bits, W(L)*(N - 1), W(L)) == L;
      }
/*
      constexpr bool is_rend(const auto& bits)
      {
        return counts.back() == 1;
      }
*/
      // sum of values in segments of `bits` of with width W, for segment offset i in [n, N).
      constexpr std::uint64_t sum(auto& bits, std::size_t W, std::size_t n, std::size_t N)
      {
        std::uint64_t acc{};
        for(auto j{n}; j < N; ++j)
          acc += ss::impl::arrays::get_value(bits, j*W, W);
        return acc;
      }

      inline auto& next(auto& bits, std::size_t L, std::size_t N)
      {
        const auto W{impl::W(L)};

        auto i{0zu};
        for(; i < N-1; ++i)
        {
          const auto component_i = ss::impl::arrays::get_value(bits, i*W, W);
          if(component_i > 0)
          {
            ss::impl::arrays::set_value(bits, i*W, W, component_i-1);

            const auto component_ip1 = ss::impl::arrays::get_value(bits, i*W+W, W);
            ss::impl::arrays::set_value(bits, i*W+W, W, component_ip1+1);

            ss::impl::arrays::set0(bits, 0, i*W+W);
            ss::impl::arrays::set_value(bits, 0, W, L - sum(bits, W, i+1, N));

            return bits;
          }
        }

        // We're in either `last` state or `end` state @entry
        // If `last`, transition to `end`.
        // If `end`, transition to `begin`.
        // if(bits.back() == 0)
        if(!ss::impl::arrays::test(bits, W*N))
        {
          ss::impl::arrays::set1(bits, W*N, W*N+1);
          return bits;
        }
        else
        {
          ss::impl::arrays::zero(bits);
          ss::impl::arrays::set_value(bits, 0, W, L);
          return bits;
        }
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

