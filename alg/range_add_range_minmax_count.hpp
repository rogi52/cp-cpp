#include "template.hpp"
#include "alg/minmax.hpp"
#include "alg/with_cnt.hpp"
#include "alg/sum.hpp"

namespace alg {
template < class T > struct range_add_range_min_cnt {
    using value_structure = with_cnt<min_m< T >>;
    using operator_structure = sum< T >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(const S& x, const F& f) {
        return S{x.value + f, x.count};
    }
};
template < class T > struct range_add_range_max_cnt {
    using value_structure = with_cnt<max_m< T >>;
    using operator_structure = sum< T >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(const S& x, const F& f) {
        return S{x.value + f, x.count};
    }
};
}
