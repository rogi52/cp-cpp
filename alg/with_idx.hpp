#include "template.hpp"

namespace alg {
template < class M, class Int > struct with_idx {
    struct A {
        using V = typename M::value_type;
        mutable V value;
        Int index;
    };
    using value_type = A;
    static constexpr A op(const A& l, const A& r) {
        return M::op(l.value, r.value) == l.value ? l : r;
    }
    static constexpr A e() {
        return A{M::e(), T(1e9)};
    }
};
}