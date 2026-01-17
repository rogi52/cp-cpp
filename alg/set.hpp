#pragma once
#include "template.hpp"

namespace alg {
template < class T, T none = T(-1) > struct set_m {
    using value_type = T;
    static constexpr T op(const T& l, const T& r) { return r == none ? l : r; }
    static constexpr T e() { return none; }
};
}