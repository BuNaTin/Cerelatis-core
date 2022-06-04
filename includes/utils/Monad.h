#pragma once

#include <utility>

namespace utils { namespace monad {

// wrap state
template<typename Arg>
inline auto wrap(Arg x) {
    return [=](auto acess) {
        return acess(x);
    };
};

// return state
template<class State, class Func>
auto operator>(State s, Func f) {
    return s(f);
}

// change state
template<class State, class Func>
auto operator>=(State s, Func f) {
    return wrap(s(f));
}

}} // namespace utils::monad