#include <array>
#include <numeric>
#include <algorithm>

namespace ss
{
  namespace multi
  {
    namespace impl
    {
      constexpr void zero(auto& mm)
      {
        // TODO 2:59 PM Monday, September 29, 2025 Just do this for now.
        std::fill_n(std::begin(mm), std::size(mm), 0);
      }

      constexpr auto& next(auto& mm, std::size_t L)
      {
        auto i{0zu};
        for(; i < std::size(mm) - 2; ++i)
        {
          if(mm[i] > 0)
          {
            mm[i]--;
            mm[i+1]++;
            std::fill_n(std::begin(mm), i+1, 0);
            mm[0] = L - std::accumulate(std::cbegin(mm) + (i+1), std::cend(mm) - 1, 0);
            return mm;
          }
        }

        // We're in either `last` state or `end` state @entry
        // If `last`, transition to `end`.
        // If `end`, transition to `begin`.
        if(mm[std::size(mm) - 1] == 0)
        {
// SS << std::endl;
          zero(mm);
          mm[std::size(mm) - 2] = L;
          mm[std::size(mm) - 1] = 1;
          return mm;
        }
        else
        {
          zero(mm);
          mm[0] = L;
          return mm;
        }
      }

      // TODO 9:19 PM Monday, September 29, 2025 Finish
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
    }

    template<std::uint64_t N>
    using index = std::array<std::uint64_t, N+1>;

    /*
     * Set `begin` state for a traversal over C(N+L-1, L) many multi-subsets
     */
    constexpr void begin(auto& index, std::size_t L)
    {
      impl::zero(index);
      index[0] = L;
    }

    constexpr void end(auto& index, std::size_t L)
    {
      impl::zero(index);
      index[std::size(index)-2] = L;
      index[std::size(index)-1] = 1;
    }

    constexpr auto& next(auto& index, std::size_t L)
    {
      return impl::next(index, L);
    }

    /*
     * Set `rbegin` state for a traversal over C(N+L-1, L) many multi-subsets
     */
    constexpr void rbegin(auto& index, std::size_t L)
    {
      impl::zero(index);
      index[std::size(index) - 2] = L;
    }

    constexpr bool is_begin(const auto& index, std::size_t L)
    {
      return
        index.front() == L
        &&
        std::ranges::all_of(index.cbegin()+1, index.cend()-1, [](auto& m){ return m == 0; });
    }

    constexpr bool is_rbegin(const auto& index, std::size_t L)
    {
      return
        index.back() == L
        &&
        std::ranges::all_of(index.cbegin(), index.cend()-2, [](auto& m){ return m == 0; });
    }

    constexpr bool is_end(const auto& index)
    {
      return index.back() == 1;
    }

    constexpr bool is_rend(const auto& index)
    {
      return index.back() == 1;
    }

    // template<typename ostream, std::uint64_t N>
    // ostream& operator<<(ostream& o, const index<N>& mm)
    template<typename ostream>
    ostream& operator<<(ostream& o, const auto& mm)
    {
      o << '[';
      if(mm.size() > 0)
      {
        for(auto i{0zu}; i < mm.size() - 1; ++i)
          o << mm[i] << ", ";
        o << mm.back();
      }
      o << ']';
      return o;
    }

  }


}