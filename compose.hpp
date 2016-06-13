#ifndef MIXOR_COMPOSE_HPP
#define MIXOR_COMPOSE_HPP

#include<type_traits>

namespace mixor {

template<class...F>
auto compose(F...) {
  return [](auto const& x) {
    return x;
  };
}


template<class F, class...Fs>
auto compose(F const& f, Fs const&...fs) {
  return [=](auto const& x) {
    return compose(fs...)(f(x));
  };
}

}

#endif
