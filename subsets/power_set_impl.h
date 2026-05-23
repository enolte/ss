#include "bits.h"
#include <algorithm>

namespace ss
{
  namespace power_set
  {
    namespace impl
    {
      using namespace ss::impl::arrays;

      constexpr void begin(auto& bits)
      {
        zero(bits);
      }

      constexpr void end(auto& bits, std::uint64_t N)
      {
        // end state == all data bits ones + end-state bit
        set1(bits, 0, N+1);
      }

      constexpr void rbegin(auto& bits, std::uint64_t N)
      {
        // rbegin state == all data bits ones + end-state bit == zero
        zero(bits);
        set1(bits, 0, N);
      }

      constexpr void rend(auto& bits, std::uint64_t N)
      {
        // rend state == all data bits zero + end-state bit.
        zero(bits);
        set1(bits, N, N+1);
      }

      constexpr bool is_end(auto& bits, std::uint64_t N)
      {
        return
          ones(bits, 0, N) == N &&
          test(bits, N);
      }

      constexpr bool is_begin(auto& bits, std::uint64_t N)
      {
        return
          ones(bits, 0, N) == 0 &&
          !test(bits, N);
      }

      constexpr bool is_rbegin(auto& bits, std::uint64_t N)
      {
        return
          ones(bits, 0, N) == N &&
          !test(bits, N);
      }

      constexpr bool is_rend(auto& bits, std::uint64_t N)
      {
        return
          ones(bits, 0, N) == 0 &&
          test(bits, N);
      }

      constexpr auto& next(auto& bits, std::uint64_t N)
      {
        using ss::operator<<;
        using namespace ss::impl;

        std::size_t i{};
        for(; i < (N>>6); ++i)
          if(bits[i] != 0xffff'ffff'ffff'ffff)
            break;

        if(i < (N>>6))
        {
          // rend --> begin
          if(is_rend(bits, N))
          {
            impl::begin(bits);
          }
          else
          {
            ++bits[i];
            std::ranges::fill_n(std::begin(bits), i, 0);
          }
        }
        else
        {
          // end --> begin
          // rend --> begin
          if(test(bits, N))
          {
            impl::begin(bits);
          }
          else
          {
            // rbegin --> end
            if(ones(bits, 64*i, N) == N - 64*i)
            {
              set1(bits, N, N+1);
            }
            else
            {
              auto value = get_value(bits, 64*i, N - 64*i);
              ++value;
              set_value(bits, 64*i, N - 64*i, value);
              std::ranges::fill_n(std::begin(bits), i, 0);
            }
          }
        }

        return bits;
      }

      constexpr auto& prev(auto& bits, std::uint64_t N)
      {
        using namespace ss::impl;

        auto i{0zu};
        for(; i < N>>6; ++i)
          if(bits[i] != 0)
            break;

        if(bits[i] != 0)
        {
          // rbegin <-- end
          if(is_end(bits, N))
          {
            set0(bits, N, N+1);
          }
          else
          {
            --bits[i];
            std::ranges::fill_n(std::begin(bits), i, 0xffff'ffff'ffff'ffff);
          }
        }
        else
        {
          // subset@entry == empty set == first subset in iteration order or else rend state@entry
          if(ones(bits, 64*i, N) == 0)
          {
            // rbegin <-- rend
            if(test(bits, N))
            {
              set1(bits, 0, N);
            }
            // rend <-- begin
            else
            {
              rend(bits, N);
            }
          }
          else
          {
            auto value = get_value(bits, 64*i, N - 64*i);
            --value;
            set_value(bits, 64*i, N - 64*i, value);
            std::ranges::fill_n(std::begin(bits), i, 0xffff'ffff'ffff'ffff);
          }
        }

        return bits;
      }
    }
  }
}




