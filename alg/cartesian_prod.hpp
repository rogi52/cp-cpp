#include "template.hpp"

namespace alg {
template < class M, class N > struct cartesian_prod_m {
    using value_type = pair< typename M::value_type, typename N::value_type >;
    using T = value_type;
    static constexpr T op(const T& l, const T& r) {
        return T(M::op(l.first, r.first), N::op(l.second, r.second));
    }
    static constexpr T e() { return T(M::e(), N::e()); }
};
}