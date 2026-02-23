#include "template.hpp"

namespace alg {
// ABC236-G (https://atcoder.jp/contests/abc236/tasks/abc236_g)
template < class T > struct min_max_semiring {
    using value_type = T;
    static constexpr T add(const T& a, const T& b) { return min(a, b); }
    static constexpr T mul(const T& a, const T& b) { return max(a, b); }
    static constexpr T add_e() { return numeric_limits<T>::max(); }
    static constexpr T mul_e() { return T(0); }
};
template < class T > struct max_plus_semiring {
    using value_type = T;
    static constexpr T add(const T& a, const T& b) { return max(a, b); }
    static constexpr T mul(const T& a, const T& b) { return a + b; }
    static constexpr T add_e() { return numeric_limits<T>::min(); }
    static constexpr T mul_e() { return T(0); }
};
// T = unsigned **
template < class T > struct or_and_semiring {
    using value_type = T;
    static constexpr T add(const T& a, const T& b) { return a | b; }
    static constexpr T mul(const T& a, const T& b) { return a & b; }
    static constexpr T add_e() { return T(0); }
    static constexpr T mul_e() { return ~T(0); } // 111
};
// T = unsigned **
template < class T > struct xor_and_semiring {
    using value_type = T;
    static constexpr T add(const T& a, const T& b) { return a ^ b; }
    static constexpr T mul(const T& a, const T& b) { return a & b; }
    static constexpr T add_e() { return T(0); }
    static constexpr T mul_e() { return ~T(0); }
};
template < class T > struct min_plus_semiring {
    using value_type = T;
    static constexpr T add(const T& a, const T& b) { return min(a, b); }
    static constexpr T mul(const T& a, const T& b) { return a + b; }
    static constexpr T add_e() { return numeric_limits<T>::max() / 2; }
    static constexpr T mul_e() { return T(0); }
};
}