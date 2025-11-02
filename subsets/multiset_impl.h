#include "bits.h"

namespace ss
{
  namespace multi
  {
    namespace impl
    {
      constexpr std::uint64_t W(std::size_t L) { return std::bit_width(L); }

      constexpr void begin(auto& bits, std::uint64_t L)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set_value(bits, 0, W(L), L);
      }

      constexpr void end(auto& bits, std::uint64_t L, std::uint64_t N)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set_value(bits, (N-1)*W(L), W(L), L);
        ss::impl::arrays::set1(bits, N*W(L), N*W(L)+1);
      }

      constexpr void rbegin(auto& bits, std::uint64_t L, std::uint64_t N)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set_value(bits, (N-1)*W(L), W(L), L);
      }

      constexpr void rend(auto& bits, std::uint64_t L, std::uint64_t N)
      {
        ss::impl::arrays::zero(bits);
        ss::impl::arrays::set_value(bits, 0, W(L), L);
        ss::impl::arrays::set1(bits, N*W(L), N*W(L)+1);
      }

      constexpr bool is_begin(const auto& bits, std::size_t L, std::uint64_t N)
      {
        return
          ss::impl::arrays::test(bits, W(L)*N) == 0 &&
          ss::impl::arrays::get_value(bits, 0, W(L)) == L;
      }

      constexpr bool is_rbegin(const auto& bits, std::size_t L, std::uint64_t N)
      {
        return
          ss::impl::arrays::test(bits, W(L)*N) == 0 &&
          ss::impl::arrays::get_value(bits, W(L)*(N - 1), W(L)) == L;
      }

      constexpr bool is_end(const auto& bits, std::size_t L, std::uint64_t N)
      {
        return
          ss::impl::arrays::test(bits, W(L)*N) != 0 &&
          ss::impl::arrays::get_value(bits, W(L)*(N - 1), W(L)) == L;
      }

      // TODO 9:36 PM Thursday, October 30, 2025. Test
      constexpr bool is_rend(const auto& bits, std::size_t L, std::uint64_t N)
      {
        return
          ss::impl::arrays::test(bits, W(L)*N) != 0 &&
          ss::impl::arrays::get_value(bits, 0, W(L)) == L;
      }

      constexpr auto& next(auto& bits, std::size_t L, std::size_t N)
      {
        const auto W{impl::W(L)};
        std::uint64_t initial_count{};

        auto i{0zu};
        for(; i < N-1; ++i)
        {
          const auto component_i = ss::impl::arrays::get_value(bits, i*W, W);
          initial_count += component_i;
          if(component_i > 0)
          {
            ss::impl::arrays::set_value(bits, i*W, W, component_i-1);

            const auto component_ip1 = ss::impl::arrays::get_value(bits, i*W+W, W);
            ss::impl::arrays::set_value(bits, i*W+W, W, component_ip1+1);

            ss::impl::arrays::set0(bits, 0, i*W+W);
            ss::impl::arrays::set_value(bits, 0, W, initial_count - 1);

            return bits;
          }
        }

        // We're in either `last` state or `end` state @entry
        // If `last`, transition to `end`.
        // If `end`, transition to `begin`.
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

      constexpr auto& prev(auto& bits, std::size_t L, std::size_t N)
      {
        const auto W{impl::W(L)};

        std::uint64_t initial_count{};
        const auto component_0 = ss::impl::arrays::get_value(bits, 0, W);
        if(component_0 > 0)
        {
          initial_count += component_0;

          // Find the next non-zero component if there is one.
          for(auto i{1zu}; i < N; ++i)
          {
            const auto component_i = ss::impl::arrays::get_value(bits, i*W, W);
            if(component_i > 0)
            {
              initial_count += component_i;
              ss::impl::arrays::set_value(bits, i*W, W, component_i-1);
              ss::impl::arrays::set0(bits, 0, i*W);
              ss::impl::arrays::set_value(bits, i*W-W, W, initial_count-(component_i-1));
              return bits;
            }
          }

          // No next non-zero. We're in either `begin` or `rend` at entry.
          if(ss::impl::arrays::test(bits, N*W) == 0)
          {
            // `begin` --> `rend`
            ss::impl::arrays::set1(bits, N*W, N*W+1);
          }
          else
          {
            // `rend` --> `rbegin`
            rbegin(bits, L, N);
          }

          return bits;
        }

        auto i{1zu};
        for(; i < N; ++i)
        {
          const auto component_i = ss::impl::arrays::get_value(bits, i*W, W);
          if(component_i > 0)
          {
            ss::impl::arrays::set_value(bits, i*W, W, component_i-1);
            const auto component_im1 = ss::impl::arrays::get_value(bits, i*W-W, W);
            ss::impl::arrays::set_value(bits, i*W-W, W, component_im1+1);
            break;
          }

        }

        return bits;
      }
    }
  }
}

