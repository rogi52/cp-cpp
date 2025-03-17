#include "template.hpp"

namespace alg {
template < class Int > struct xor_g {
    using value_type = Int;
    static constexpr Int op(const T& a, const T& b) { return a ^ b; }
    static constexpr Int e() { return 0; }
    static constexpr Int inv(const T& x) { return x; }
    static constexpr Int pow(const T& x, const u64 n) { return n & 1 ? x : 0; }
    static constexpr bool comm() { return true; }
};
}