#ifndef MIXOR_MIXIN_HPP
#define MIXOR_MIXIN_HPP
#include<type_traits>

#define REQUIRES(C) std::enable_if_t<(C), int> = 0

namespace mixor {

template<class X>
constexpr bool any(X const& x) {
  return (bool)x;
}

template<class X, class Y>
constexpr bool any(X const& x, Y const& y) {
  return x || y;
}

template<class X, class...Xs>
constexpr bool any(X const& x, Xs const&...xs) {
  return any(x, any(xs...) );
}

template<class B, class L, class R,
REQUIRES( (std::is_convertible<R,B>::value) )>
B either(L const&, R const& r) {
    return r;
}

template<class B, class L, class R,
REQUIRES( (!std::is_convertible<R,B>::value) )>
B either(L const& l, R const&) {
    return l;
}



template<class...>
struct Mixin
{};

template<class B>
struct Mixin<B>
  : B {

  Mixin(B const& b)
    : B{b}
  {}

  template<class...X>
  Mixin( Mixin<X...> const& rhs)
    : B{ either<B>(B{}, rhs) }
  {}

  Mixin()=default;
  Mixin(Mixin const&)=default;
};

template<class B, class...Bs>
struct Mixin<B, Bs...>
  : B
  , Mixin<Bs...> {

  Mixin(B const& b, Bs const&...bs)
    : B{b}, Mixin<Bs...>{bs...}
  {}

  template<class...X>
  Mixin( Mixin<X...> const& rhs)
    : B{ either<B>(B{}, rhs) }
    , Mixin<Bs...>{ either<Bs>(Bs{}, rhs)... }
  {}

  Mixin()=default;
  Mixin(Mixin const&)=default;
};


template<class...B>
constexpr auto mergeFragment(
  Mixin<B...> const& b
) {
  return b;
}

template<class X, class...B,
REQUIRES( !any(
  std::is_same<X,B>::value...
))>
constexpr auto mergeFragment(
  Mixin<B...> const& b,
  X const& x
) {
  return Mixin<B..., X>{
    (B const&)b..., x
  };
}

template<class X, class...B,
REQUIRES( any(
  std::is_same<X,B>::value...
))>
constexpr auto mergeFragment(Mixin<B...> const& b, X const& x) {
  return Mixin<B...> {
    either<B>(b, x)...
  };
}

template<class X, class...Xs, class...B>
constexpr auto mergeFragment(Mixin<B...> const& b, X const& x, Xs const&...xs) {
	return mergeFragment(mergeFragment(b,x), xs...);
}



template<class L>
constexpr auto merge(L const& l) {
  return Mixin<L>{l};
}

template<class...L>
constexpr auto merge(Mixin<L...> const& l) {
  return l;
}


template<class...L, class...R>
constexpr auto merge(Mixin<L...> const& l, Mixin<R...> const& r) {
  return mergeFragment(
    l, (R)r...
  );
}


template<class X, class...Xs>
constexpr auto merge(X const& x, Xs const&...xs) {
  return merge(
    merge(x),
    merge(xs...)
  );
}

static auto mix= [](auto const&...x) {
  return merge(x...);
};

template<class...B, class...X>
constexpr Mixin<B...> strip(Mixin<X...> const& m) {
  return {
  	either<B>(B{},m)...
  };
}

template<class...B, class...X>
constexpr Mixin<B...> strip(Mixin<B...> base, Mixin<X...> const& m) {
  return {
  	either<B>(base, m)...
  };
}

}

#endif
