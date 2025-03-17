#include "template.hpp"
#include "alg/minmax.hpp"
#include "alg/set.hpp"

namespace alg {
template < class T, T none = T(-1) > struct range_set_range_min {
    using value_structure = min_m< T >;
    using operator_structure = set_m< T, none >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(const S& x, const F& f) { return f == none ? x : f; }
};
template < class T, T none = T(-1) > struct range_set_range_max {
    using value_structure = max_m< T >;
    using operator_structure = set_m< T, none >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(const S& x, const F& f) { return f == none ? x : f; }
};
}