#pragma once
#include <compare>

#include "bits.h"
#include "fixed_size_impl.h"

namespace ss
{
  namespace fixed_size
  {
    // Sentinel types for construction and ordering comparison below
    static constexpr struct {} begin{};
    static constexpr struct {} end{};
    static constexpr struct {} rbegin{};
    static constexpr struct {} rend{};

    /*
     * A cursor type which implements a subset of size `K` in a set of size `N`.
     *
     * Each iterated subset defines positional values in the set of integers {0, ..., N-1}.
     * The ordering of subsets is essentially a left-to-right iteration
     * order in a standard tree, without constructing the actual tree.
     */
    template<std::uint64_t N>
    struct subset
    {
      template<std::uint64_t K>
      static consteval subset choose(const auto state)
      {
        static_assert(K <= N);
        return {state, K};
      }

      std::uint64_t K{};

      // A dense binary indicator for the currently selected subset.
      ss::bits<N> bits{};

      // N.B. The `const` on this param is necessary here.
      consteval subset(const auto state, std::uint64_t K) : K{K}
      {
        if constexpr(std::same_as<decltype(state), decltype(begin)>)
          impl::begin(bits, K);
        else if constexpr(std::same_as<decltype(state), decltype(end)>)
          impl::end(bits, K, N);
        else if constexpr(std::same_as<decltype(state), decltype(rbegin)>)
          impl::rbegin(bits, K, N);
        else if constexpr(std::same_as<decltype(state), decltype(rend)>)
          impl::rend(bits, K, N);
        else
          static_assert(false, "unknown initial state type");
      }

      subset& operator=(decltype(begin))
      {
        begin(bits, K);
        return *this;
      }

      subset& operator=(decltype(end))
      {
        end(bits, K, N);
        return *this;
      }

      subset& operator=(decltype(rbegin))
      {
        rbegin(bits, K, N);
        return *this;
      }

      subset& operator=(decltype(rend))
      {
        rend(bits, K, N);
        return *this;
      }

      constexpr bool is_end() const
      {
        return
          ss::impl::arrays::ones(bits, 0, N-K) == 0 &&
          ss::impl::arrays::ones(bits, N-K, N) == K &&
          ss::impl::arrays::test(bits, N);
      }

      constexpr bool is_begin() const
      {
        return
          ss::impl::arrays::ones(bits, 0, K) == K &&
          ss::impl::arrays::ones(bits, K, N+1) == 0;
      }

      constexpr bool is_rbegin() const
      {
        return
          ss::impl::arrays::ones(bits, 0, N-K) == 0 &&
          ss::impl::arrays::ones(bits, N-K, N) == K &&
          !ss::impl::arrays::test(bits, N);
      }

      constexpr bool is_rend() const
      {
        return
          ss::impl::arrays::ones(bits, 0, K) == K &&
          ss::impl::arrays::ones(bits, K, N) == 0 &&
          ss::impl::arrays::test(bits, N);
      }

      subset& next()
      {
        impl::next(bits, K, N);
        return *this;
      }

      subset& prev()
      {
        impl::prev(bits, K, N);
        return *this;
      }

      // N.B. `friend` is the only way to declare a non-member function in
      // a class-type scope, and a function template can not be `= default`,
      // so this symbol can not be declared default outside this class.
      // This decl is resolved after template parameter substitution, so it's
      // not a function template by then, which means it can be parsed if declared here.
      // Therefore, `friend` is necessary here.
      friend auto operator<=>(const subset&, const subset&) = default;
    };

    // N.B. Pre-increment only
    template<std::uint64_t N>
    subset<N>& operator++(subset<N>& s) { return s.next(); }

    template<std::uint64_t N>
    subset<N>& operator--(subset<N>& s) { return s.prev(); }

    template<std::uint64_t N>
    constexpr bool operator==(const auto state, const subset<N>& s)
    {
      if constexpr(std::same_as<decltype(state), decltype(begin)>)
        return s.is_begin();
      else if constexpr(std::same_as<decltype(state), decltype(end)>)
        return s.is_end();
      else if constexpr(std::same_as<decltype(state), decltype(rbegin)>)
        return s.is_rbegin();
      else if constexpr(std::same_as<decltype(state), decltype(rend)>)
        return s.is_rend();
      else
        static_assert(false, "unknown state comparison type");
    }

    /*
     * Ordering:
     *  rend < begin < ... < rbegin < end
     */
    template<std::uint64_t N>
    constexpr auto operator<=>(const auto state, const subset<N>& s)
    {
      // TODO 7:04 PM Thursday, October 02, 2025
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
/*
    template<std::uint64_t N>
    constexpr bool operator<(const auto state, const subset<N>& s)
    {
      if constexpr(std::same_as<decltype(state), decltype(begin)>)
        return s != begin && s != rend;
      else if constexpr(std::same_as<decltype(state), decltype(end)>)
        return false;
      else if constexpr(std::same_as<decltype(state), decltype(rbegin)>)
        return s == end;
      else if constexpr(std::same_as<decltype(state), decltype(rend)>)
        return s != rend;
      else
        static_assert(false, "unknown state comparison type");
    }

    template<std::uint64_t N>
    constexpr bool operator<=(const auto state, const subset<N>& s) { return state < s || state == s; }

    template<std::uint64_t N>
    constexpr bool operator>(const auto state, const subset<N>& s) { return !(state <= s); }

    template<std::uint64_t N>
    constexpr bool operator>=(const auto state, const subset<N>& s) { return !(state < s); }

    template<std::uint64_t N>
    constexpr bool operator<=(const subset<N>& s, const auto state) { return state >= s; }

    template<std::uint64_t N>
    constexpr bool operator>(const subset<N>& s, const auto state) { return state < s; }

    template<std::uint64_t N>
    constexpr bool operator>=(const subset<N>& s, const auto state) { return state <= s; }

    template<std::uint64_t N>
    constexpr bool operator<(const subset<N>& s, const auto state) { return state > s; }
*/

    template<std::uint64_t N>
    inline std::ostream& operator<<(std::ostream& o, const subset<N>& s)
    {
      return ss::impl::to_ostream(o, s.bits, N);
    }
  }

}
