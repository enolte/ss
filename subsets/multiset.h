#pragma once


#include "bits.h"
#include "multiset_impl.h"

namespace ss
{
  namespace multi
  {
    // TODO 7:49 PM Sunday, October 12, 2025.
    // Sentinel types for construction and ordering comparison below
    static constexpr struct {} begin{};
    static constexpr struct {} end{};
    static constexpr struct {} rbegin{};
    static constexpr struct {} rend{};







    template<std::uint64_t N>
    struct index
    {
      std::uint64_t L;

      // N+1 bits in each `bits`. For all but the 0th, use only the first N bits.
      std::array<bits<N>, L> counts;

      index(const auto state, std::uint64_t L): L{L}
      {
        if constexpr(std::same_as<decltype(state), decltype(begin)>)
          impl::begin(bits, L);
        // TODO 9:20 AM Monday, October 13, 2025.
        // else if constexpr(std::same_as<decltype(state), decltype(end)>)
          // impl::end(bits, L, N);
        // else if constexpr(std::same_as<decltype(state), decltype(rbegin)>)
          // impl::rbegin(bits, L, N);
        // else if constexpr(std::same_as<decltype(state), decltype(rend)>)
          // impl::rend(bits, L, N);
        else
          static_assert(false, "unknown initial state type");
      }

      index& operator=(decltype(begin))
      {
        begin(counts, K);
        return *this;
      }

      index& operator=(decltype(end))
      {
        end(counts, K, N);
        return *this;
      }

      index& operator=(decltype(rbegin))
      {
        rbegin(counts, K, N);
        return *this;
      }

      index& operator=(decltype(rend))
      {
        rend(counts, K, N);
        return *this;
      }
    };


    template<typename ostream, std::uint64_t N, std::uint64_t L>
    ostream& operator<<(ostream& o, const index<K, L>& mm)
    {
      o << "[\n";
      if(mm.counts[0].size() > 0)
      {
        for(auto l{0zu}; l < L; ++l)
          o << mm.counts[l] << '\n';
      }
      o << "\n]" << st::flush;
      return o;
    }

  }


}