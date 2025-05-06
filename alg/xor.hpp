#include "template.hpp"

namespace alg {
template < class Int > struct xor_g {
    using value_type = Int;
    static constexpr Int op(const Int& a, const Int& b) { return a ^ b; }
    static constexpr Int e() { return 0; }
    static constexpr Int inv(const Int& x) { return x; }
    static constexpr Int pow(const Int& x, const u64 n) { return n & 1 ? x : 0; }
    static constexpr bool comm() { return true; }
};
}