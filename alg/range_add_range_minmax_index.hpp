#include "template.hpp"
#include "alg/minmax.hpp"
#include "alg/sum.hpp"
#include "alg/with_idx.hpp"

namespace alg {
template < class T > struct range_add_range_min_index {
    using value_structure = with_idx<min_m< T >>;
    using operator_structure = sum< T >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(S x, const F& f) {
        x.value += f;
        return x;
    }
};
template < class T > struct range_add_range_max_index {
    using value_structure = with_idx<max_m< T >>;
    using operator_structure = sum< T >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(S x, const F& f) {
        x.value += f;
        return x;
    }
};
}

