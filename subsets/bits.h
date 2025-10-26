#pragma once

#include "bits_impl.h"

namespace ss
{
  /*
   * A bit-packed, dense, binary indicator type. This is an implemetor for other types.
   * Support ops are in namespace ss::impl. Usages are in namespaces ss::fixed_size and ss::multi.
   * For a given N, `bits` contains N+1 bits. The last bit is an end / rend state indicator.
   */
  template<std::uint64_t N>
  using bits = std::array<std::uint64_t, impl::array_size<N>>;

  template<typename ostream, std::uint64_t N>
  inline ostream& operator<<(ostream& o, const bits<N>& bits)
  {
    return impl::to_ostream(o, bits, N);
  }
}


