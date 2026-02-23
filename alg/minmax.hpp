#pragma once
#include "template.hpp"

namespace alg {
template < class T > struct min_m {
    using value_type = T;
    static constexpr T op(const T& a, const T& b) { return min(a, b); }
    static constexpr T e() { return numeric_limits< T >::max(); }
    static constexpr bool comm() { return true; }
};
template < class T > struct max_m {
    using value_type = T;
    static constexpr T op(const T& a, const T& b) { return max(a, b); }
    static constexpr T e() { return numeric_limits< T >::min(); }
    static constexpr bool comm() { return true; }
};
}