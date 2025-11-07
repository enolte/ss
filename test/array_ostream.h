#pragma once

template<typename T>
concept array_like = requires(T t)
{
  (std::uint64_t)t[std::size_t{}];
};

template<typename ostream, array_like A>
constexpr ostream& to_ostream(ostream& o, const A& x)
{
  o << '[';
  if(std::size(x) != 0)
  {
    for(auto i{0zu}; i < std::size(x) - 1; ++i)
      o.operator<<(x[i]) << ",";
    o.operator<<(x[std::size(x) - 1]);
  }
  o << ']';
  return o;
}

template<typename ostream, array_like A>
constexpr ostream& operator<<(ostream& o, const A& x)
{
  return to_ostream(o, x);
}


