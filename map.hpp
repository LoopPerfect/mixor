#ifndef MIXOR_MIXIN_HPP
#define MIXOR_MIXIN_HPP
#include<type_traits>

namespace mixor {
  template<class F>
  auto map(F const& f) {
    return [=](auto const& v){
      std::vector<decltype(f(v[0]))> w.reserve(v.size());
      for(auto const& x : v) {
        w.push_back(f(x));
      }
      return w;
    };
  }
}

#endif
