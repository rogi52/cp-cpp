#include "template.hpp"

namespace alg {
template < class M > struct with_cnt {
    struct A {
        using V = typename M::value_type;
        mutable V value;
        int count;
    };
    using value_type = A;
    static constexpr A op(const A& l, const A& r) {
        if(l.value == r.value) {
            return A{l.value, l.count + r.count};
        } else {
            return M::op(l.value, r.value) == l.value ? l : r;
        }
    }
    static constexpr A e() {
        return A{M::e(), 0};
    }
};
}