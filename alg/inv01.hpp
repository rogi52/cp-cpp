#include "template.hpp"

// APC-L Lazy Segment Tree
// https://atcoder.jp/contests/practice2/tasks/practice2_l
// https://atcoder.jp/contests/practice2/submissions/68451260

struct i01 {
    struct value_type {
        i64 c0, c1, inv;
    };
    static value_type op(const value_type& l, const value_type& r) {
        value_type x;
        x.inv = l.inv + r.inv + l.c1 * r.c0;
        x.c0 = l.c0 + r.c0;
        x.c1 = l.c1 + r.c1;
        return x;
    }
    static value_type id() {
        return value_type{0, 0, 0};
    }
    static bool cmp(const value_type& l, const value_type& r) {
        return l.c0 * r.c1 < r.c0 * l.c1;
    }
    static value_type e() {
        return value_type{0, 0, 0};
    }
};

#include "alg/xor.hpp"

struct range_xor_range_inversion {
    using value_structure = i01;
    using operator_structure = alg::xor_g<int>;
    using S = typename value_structure::value_type;
    using F = typename operator_structure::value_type;
    static constexpr S op(const S& x, const F& f) {
        if(f == 0) {
            return x;
        } else {
            return S{x.c1, x.c0, x.c0 * x.c1 - x.inv};
        }
    }
};

// int main() {
//     int N = in(), Q = in();
//     vector<int> A = in(N);
//     vector<i01::value_type> V(N);
//     FOR(i, N) {
//         if(A[i] == 0) {
//             V[i] = i01::value_type{1, 0, 0};
//         } else {
//             V[i] = i01::value_type{0, 1, 0};
//         }
//     }
//     lazytree<range_xor_range_inversion> lz(V);
//     FOR(Q) {
//         int t = in(), L = in(), R = in(); L--;
//         if(t == 1) {
//             lz.o(L, R, 1);
//         } else {
//             print(lz.v(L, R).inv);
//         }
//     }
// }