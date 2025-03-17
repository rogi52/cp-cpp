#include "template.hpp"
#include "alg/sum.hpp"
#include "alg/with_size.hpp"

namespace alg {
template < class T > struct range_add_range_sum {
    using value_structure = with_size< sum< T >, int >;
    using operator_structure = sum< T >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S  op(const S& x, const F& f) {
        return S{x.value + f * T(x.size), x.size};
    }
};
}