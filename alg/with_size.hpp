#include "template.hpp"

namespace alg {
template < class M, class Int > struct with_size {
    struct A {
        using V = typename M::value_type;
        mutable V value;
        Int size;
    };
    using value_type = A;
    static constexpr A op(const A& l, const A& r) { return A{M::op(l.value, r.value), l.size + r.size}; }
    static constexpr A e() { return A{M::e(), 0}; }
};
}