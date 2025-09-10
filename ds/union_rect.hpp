#include "template.hpp"
#include "ds/lazytree.hpp"
#include "alg/sum.hpp"
#include "alg/minmax.hpp"

// Library Checker: https://judge.yosupo.jp/problem/area_of_union_of_rectangles
namespace area_of_union_of_rectangles {
template < class T > struct min_count {
    struct S { int min; T cnt; };
    using value_type = S;
    static constexpr S op(const S& a, const S& b) {
        if(a.min < b.min) return a;
        if(a.min > b.min) return b;
        return S{a.min, a.cnt + b.cnt};
    }
    static constexpr S e() { return S{numeric_limits<T>::max(), 0}; }
};
template < class T > struct range_add_range_min_count {
    using value_structure = min_count< T >;
    using operator_structure = alg::sum<int>;
    using F = typename operator_structure::value_type;
    using S = typename value_structure::value_type;
    static constexpr S op(const S& x, const F& f) {
        return S{x.min + f, x.cnt};
    }
};

template < class T > struct rect { T lx, rx, ly, ry; };
template < class T, class Ans > Ans solve(const vector<rect<T>>& a) {
    const int N = ssize(a);
    vector<pair<T, int>> Y(N + N);
    FOR(i, N) Y[i    ] = {a[i].ly, i    };
    FOR(i, N) Y[i + N] = {a[i].ry, i + N};
    ranges::sort(Y, {}, &pair<T, int>::first); 
    vector<int> ly_idx(N), ry_idx(N);
    FOR(i, N + N) {
        const auto &[_, j] = Y[i];
        (j < N ? ly_idx[j] : ry_idx[j - N]) = i;
    }

    struct query { T x; int l, r, c; };
    vector<query> Q(N + N);
    FOR(i, N) {
        const int l = ly_idx[i];
        const int r = ry_idx[i];
        Q[i    ] = {a[i].lx, l, r, +1};
        Q[i + N] = {a[i].rx, l, r, -1};
    }
    ranges::sort(Q, {}, &query::x);

    using A = range_add_range_min_count< T >;
    using S = typename A::value_structure::value_type;
    vector<S> V(N + N - 1);
    FOR(i, N + N - 1) V[i] = {0, Y[i + 1].first - Y[i].first};
    lazytree<A> seg(V);

    Ans ans = 0;
    const T Y_cnt = Y[N + N - 1].first - Y[0].first;
    T x = 0;
    for(const query& q : Q) {
        const auto p = seg.av();
        ans += Ans(q.x - x) * (Y_cnt - (p.min == 0 ? p.cnt : 0));
        seg.o(q.l, q.r, q.c);
        x = q.x;
    }
    return ans;
}
}
