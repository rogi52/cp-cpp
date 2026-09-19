#pragma once
#include "template.hpp"

namespace alg {
template < class Int > struct and_m {
    using value_type = Int;
    static constexpr Int op(const Int& a, const Int& b) { return a & b; }
    static constexpr Int e() { return ~static_cast<Int>(0); }
};
template < class Int > struct or_m {
    using value_type = Int;
    static constexpr Int op(const Int& a, const Int& b) { return a | b; }
    static constexpr Int e() { return static_cast<Int>(0); }
};
}