#include "template.hpp"

namespace alg {
template < class M > struct with_idx {
    struct A {
        using V = typename M::value_type;
        mutable V value;
        int index;
    };
    using value_type = A;
    static constexpr A op(const A& l, const A& r) {
        return M::op(l.value, r.value) == l.value ? l : r;
    }
    static constexpr A e() {
        return A{M::e(), INF32};
    }
};
}