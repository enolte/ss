#pragma once

#include "bits.h"
#include "power_set_impl.h"
#include "fixed_size.h"

namespace ss
{
  namespace power_set
  {
    // Sentinel types for construction and ordering comparison below
    static constexpr struct {} begin{};
    static constexpr struct {} end{};
    static constexpr struct {} rbegin{};
    static constexpr struct {} rend{};

    template<std::size_t N>
    struct subset
    {
      // A dense binary indicator for the currently selected subset.
      ss::bits<N> bits{};

      // N.B. The `const` on this param is necessary here.
      consteval subset(const auto state)
      {
        if constexpr(std::same_as<decltype(state), decltype(begin)>)
          impl::begin(bits);
        else if constexpr(std::same_as<decltype(state), decltype(end)>)
          impl::end(bits, N);
        else if constexpr(std::same_as<decltype(state), decltype(rbegin)>)
          impl::rbegin(bits, N);
        else if constexpr(std::same_as<decltype(state), decltype(rend)>)
          impl::rend(bits, N);
        else
          static_assert(false, "unknown initial state type");
      }

      constexpr std::uint64_t size() const { return ss::impl::arrays::ones(bits, 0, N); }

      constexpr subset& operator=(decltype(begin))  { impl::begin(bits); return *this; }
      constexpr subset& operator=(decltype(end))    { impl::end(bits, N); return *this; }
      constexpr subset& operator=(decltype(rbegin)) { impl::rbegin(bits, N); return *this; }
      constexpr subset& operator=(decltype(rend))   { impl::rend(bits, N); return *this; }

      constexpr bool is_begin() const  { return impl::is_begin(bits, N); }
      constexpr bool is_end() const    { return impl::is_end(bits, N); }
      constexpr bool is_rbegin() const { return impl::is_rbegin(bits, N); }
      constexpr bool is_rend() const   { return impl::is_rend(bits, N); }

      constexpr auto components(auto begin) const
      {
        for(auto j{0zu}; j != N; ++j)
        {
          if(ss::impl::arrays::test(bits, j))
          {
            *begin = j;
            ++begin;
          }
        }
        return begin;
      }

      // TODO 20:24 Friday, May 22, 2026. Chain graph level-order traversal (Sets paritally ordered first by set size, then by points.)
      subset& next() { impl::next(bits, N); return *this; }
      subset& prev() { impl::prev(bits, N); return *this; }

      struct size_order
      {
        ss::fixed_size::subset<N> s;

        // N.B. The `const` on this param is necessary here.
        consteval size_order(const auto state)
        {
          if constexpr(std::same_as<decltype(state), decltype(begin)>)
            s = fixed_size::begin;
          else if constexpr(std::same_as<decltype(state), decltype(end)>)
            s = fixed_size::end;
          else if constexpr(std::same_as<decltype(state), decltype(rbegin)>)
            s = fixed_size::rbegin;
          else if constexpr(std::same_as<decltype(state), decltype(rend)>)
            s = fixed_size::rend;
          else
            static_assert(false, "unknown initial state type");
        }

        constexpr std::uint64_t size() const { return s.K; }

        constexpr subset& operator=(decltype(begin))  { impl::begin(s.bits); return *this; }
        constexpr subset& operator=(decltype(end))    { impl::end(s.bits, N); return *this; }
        constexpr subset& operator=(decltype(rbegin)) { impl::rbegin(s.bits, N); return *this; }
        constexpr subset& operator=(decltype(rend))   { impl::rend(s.bits, N); return *this; }

        constexpr bool is_begin() const  { return impl::is_begin(s.bits, N); }
        constexpr bool is_end() const    { return impl::is_end(s.bits, N); }
        constexpr bool is_rbegin() const { return impl::is_rbegin(s.bits, N); }
        constexpr bool is_rend() const   { return impl::is_rend(s.bits, N); }

        constexpr auto components(auto begin) const { return s.components(begin); }

        size_order& next()
        {
          if(s.is_rbegin())
          {
            if(s.K < N)
            {
              s.K += 1;
              s = fixed_size::begin;
            }
            else
            {
              s = fixed_size::end;
            }
          }
          else
          {
            s.next();
          }

          return *this;
        }
      };

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

    template<typename ostream, std::uint64_t N>
    inline ostream& operator<<(ostream& o, const subset<N>& s)
    {
      return ss::impl::to_ostream(o, s.bits, N);
    }

  }

}