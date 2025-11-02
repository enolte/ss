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
      // Total bit offset range is [0, NW+1]. The extra bit is for
      // `end` / `rend` state.
      ss::bits<N*W> bits{};

      // N.B. The `const` on this param is necessary here.
      constexpr index(const auto state)
      {
        if constexpr(std::same_as<decltype(state), decltype(begin)>)
          impl::begin(bits, L);
        else if constexpr(std::same_as<decltype(state), decltype(end)>)
          impl::end(bits, L, N);
        else if constexpr(std::same_as<decltype(state), decltype(rbegin)>)
          impl::rbegin(bits, L, N);
        else if constexpr(std::same_as<decltype(state), decltype(rend)>)
          impl::rend(bits, L, N);
        else
          static_assert(false, "unknown initial state type");
      }

      constexpr index& operator=(decltype(begin))  { impl::begin(bits, L); return *this; }
      constexpr index& operator=(decltype(end))    { impl::end(bits, L, N); return *this; }
      constexpr index& operator=(decltype(rbegin)) { impl::rbegin(bits, L, N); return *this; }
      constexpr index& operator=(decltype(rend))   { impl::rend(bits, L, N); return *this; }

      constexpr bool is_begin() const  { return impl::is_begin(bits, L, N); }
      constexpr bool is_end() const    { return impl::is_end(bits, L, N); }
      constexpr bool is_rbegin() const { return impl::is_rbegin(bits, L, N); }
      constexpr bool is_rend() const   { return impl::is_rend(bits, L, N); }

      constexpr std::array<std::uint64_t, N> components() const
      {
        std::array<std::uint64_t, N> c;
        for(auto j{0zu}; j != N; ++j)
        {
          c[j] = ss::impl::arrays::get_value(bits, j*W, W);
        }
        return c;
      }

      index& next() { impl::next(bits, L, N); return *this; }
      index& prev() { impl::prev(bits, L, N); return *this; }
    };

    // N.B. Pre-increment only
    template<std::uint64_t N, std::size_t L>
    index<N, L>& operator++(index<N, L>& ii) { return ii.next(); }

    template<std::uint64_t N, std::size_t L>
    index<N, L>& operator--(index<N, L>& ii) { return ii.prev(); }

    template<std::uint64_t N, std::size_t L>
    constexpr bool operator==(const auto state, const index<N, L>& ii)
    {
      if constexpr(std::same_as<decltype(state), decltype(begin)>)
        return ii.is_begin();
      else if constexpr(std::same_as<decltype(state), decltype(end)>)
        return ii.is_end();
      else if constexpr(std::same_as<decltype(state), decltype(rbegin)>)
        return ii.is_rbegin();
      else if constexpr(std::same_as<decltype(state), decltype(rend)>)
        return ii.is_rend();
      else
        static_assert(false, "unknown state comparison type");
    }

    /*
     * Ordering:
     *  rend < begin < ... < rbegin < end
     */
    template<std::uint64_t N, std::size_t L>
    constexpr auto operator<=>(const auto state, const index<N, L>& s)
    {
      // TODO 1:14 PM Saturday, November 01, 2025
      // if constexpr(s == state)
      if(s == state)
        return std::strong_ordering::equal;
      else if(s == rend)
        return std::strong_ordering::greater;
      else if(s == end)
        return std::strong_ordering::less;

      if constexpr(std::same_as<decltype(state), decltype(begin)>)
        return std::strong_ordering::less;
      else if constexpr(std::same_as<decltype(state), decltype(end)>)
        return std::strong_ordering::greater;
      else if constexpr(std::same_as<decltype(state), decltype(rbegin)>)
        return std::strong_ordering::greater;
      else if constexpr(std::same_as<decltype(state), decltype(rend)>)
        return std::strong_ordering::less;
      else
        static_assert(false, "unknown state comparison type");
    }

    template<typename ostream, std::uint64_t N, std::uint64_t L>
    ostream& operator<<(ostream& o, const index<N, L>& mm)
    {
      // o << "(N * W(L)) = (" << N << " * W(" << L << ")) = (" << N << " * " << std::bit_width(L) << ") ";
      o << "[";
      ss::impl::to_ostream(o, mm.bits, N * mm.W - 1);
      o << "]" << std::flush;
      if(ss::impl::arrays::test(mm.bits, mm.W*N))
        o << " (end)" << std::flush;
      return o;
    }

  }


}