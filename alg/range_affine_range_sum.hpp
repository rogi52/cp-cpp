#include "template.hpp"
#include "alg/sum.hpp"
#include "alg/cartesian_prod.hpp"
#include "alg/affine.hpp"

namespace alg {
template < class T > struct range_affine_range_sum_lr {
    using value_structure = cartesian_prod_m< sum< T >, sum< T > >;
    using operator_structure = affine_lr< T >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(const S& l, const F& r) {
        return S(l.first * r.a + l.second * r.b, l.second);
    }
};
}