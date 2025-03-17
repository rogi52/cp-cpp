#include "template.hpp"
#include "alg/minmax.hpp"
#include "alg/sum.hpp"

namespace alg {
template < class T > struct range_add_range_min {
    using value_structure = min_m< T >;
    using operator_structure = sum< T >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(const S& x, const F& f) { return S(x + f); }
};
template < class T > struct range_add_range_max {
    using value_structure = max_m< T >;
    using operator_structure = sum< T >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(const S& x, const F& f) { return S(x + f); }
};
}

