#pragma once

#include <bit>

#include "bits.h"
#include "multiset_impl.h"

namespace ss
{
  namespace multi
  {
    // Sentinel types for construction and ordering comparison below
    static constexpr struct {} begin{};
    static constexpr struct {} end{};
    static constexpr struct {} rbegin{};
    static constexpr struct {} rend{};

    /*
     * A multi-index type for a set {0, ..., N-1}.
     */
    template<std::uint64_t N, std::uint64_t L>
    struct index
    {
      // bit width of a single entry in `bits`
      static constexpr std::uint64_t W{ std::bit_width(L) };

      // Bit-packed consecutive storage for each multiindex component.
      ss::bits<N*W> bits{};

      constexpr index(const auto state)
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
        impl::begin(bits, L);
        return *this;
      }

      index& operator=(decltype(end))
      {
        impl::end(bits, L, N);
        return *this;
      }

      index& operator=(decltype(rbegin))
      {
        impl::rbegin(bits, L, N);
        return *this;
      }

      index& operator=(decltype(rend))
      {
        impl::rend(bits, L, N);
        return *this;
      }

      constexpr std::array<std::uint64_t, N> components() const
      {
        std::array<std::uint64_t, N> c;
        for(auto j{0zu}; j != N; ++j)
        {
          c[j] = ss::impl::arrays::get_value(bits, j*W, W);
        }
        return c;
      }
    };


    template<typename ostream, std::uint64_t N, std::uint64_t L>
    ostream& operator<<(ostream& o, const index<N, L>& mm)
    {
      o << "(N * W(L)) = (" << N << " * W(" << L << ")) = (" << N << " * " << std::bit_width(L) << ") ";
      o << "[";
      ss::impl::to_ostream(o, mm.bits, N * mm.W - 1);
      o << "]" << std::flush;
      if(ss::impl::arrays::test(mm.bits, mm.W*N))
        o << " (end)" << std::flush;
      return o;
    }

  }


}