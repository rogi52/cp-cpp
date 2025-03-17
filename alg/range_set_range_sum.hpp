#include "template.hpp"
#include "alg/sum.hpp"
#include "alg/with_size.hpp"
#include "alg/set.hpp"

namespace alg {
template < class T, T none = T(-1) > struct range_set_range_sum {
    using value_structure = with_size< sum< T >, int >;
    using operator_structure = set_m< T, none >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(const S& x, const F& f) {
        if(f != none) x.value = f * x.size;
        return x;
    }
};
}