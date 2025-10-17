#pragma once
#include "template.hpp"

namespace alg {
template < class T > struct prod {
    using value_type = T;
    static constexpr T op(const T& a, const T& b) { return a * b; }
    static constexpr T e() { return T(1); }
    static constexpr T inv(const T& a) { return T(1) / a; }
    static constexpr bool comm() { return true; }
};
}