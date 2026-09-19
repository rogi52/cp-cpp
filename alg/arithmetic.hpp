#include "template.hpp"

// https://atcoder.jp/contests/pakencamp-2025-day2/tasks/pakencamp_2025_day2_d
namespace alg {

template < class T > struct arithmetic {
    T a, b;
    constexpr arithmetic() : a(0), b(0) {}
    constexpr arithmetic(const T& a, const T& b) : a(a), b(b) {}
    static constexpr arithmetic e() { return arithmetic(T(0), T(0)); }
};

template < class T > struct arithmetic_add {
    using F = arithmetic< T >;
    using value_type = F;
    static constexpr F op(const F& l, const F& r) {
        return F(l.a + r.a, l.b + r.b);
    }
    static constexpr F e() { return F::e(); }
};

template < class T > struct ap_sum_monoid {
    struct ap_sum {
        T sum, len, idx;
        constexpr ap_sum() : sum(0), len(0), idx(0) {}
        constexpr ap_sum(const T& sum, const T& len, const T& idx) : sum(sum), len(len), idx(idx) {}
    };
    using value_type = ap_sum;
    using S = value_type;
    static constexpr S op(const S& l, const S& r) {
        return S(l.sum + r.sum, l.len + r.len, l.idx + r.idx);
    }
    static constexpr S e() { return S(0, 0, 0); }
};

template < class T > struct range_arithmetic_range_sum {
    using value_structure = ap_sum_monoid< T >;
    using operator_structure = arithmetic_add< T >;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(const S& x, const F& f) {
        return S(x.sum + f.a * x.idx + f.b * x.len, x.len, x.idx);
    }
};

}