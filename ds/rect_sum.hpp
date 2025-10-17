#include "template.hpp"
#include "ds/persistent_segtree.hpp"

// 点とその重み: オフライン
// 長方形: オンライン
template < class Index, class Monoid > struct rect_sum {
    using I = Index;
    using M = Monoid;
    using V = typename M::value_type;
    per_segtree< M > pst;
    vector<typename per_segtree< M >::ptr> seg;
    vector< I > X, Y;
    vector< V > W;

    rect_sum(const vector< I >& x, const vector< I >& y, const vector< V >& w) {
        const int n = ssize(x);
        assert(ssize(y) == n);
        assert(ssize(w) == n);

        vector<int> ord = iota(n);
        sort(ord, [&](int i, int j) { return y[i] < y[j]; });
        X.reserve(n);
        Y.reserve(n);
        W.reserve(n);
        for(int i : ord) {
            X.push_back(x[i]);
            Y.push_back(y[i]);
            W.push_back(w[i]);
        }

        sort(ord, [&](int i, int j) { return X[i] < X[j]; });
        pst = per_segtree< M >(n);
        seg.resize(n + 1); seg[0] = pst.build();
        vector< I > X2;
        X2.reserve(n);
        FOR(t, n) {
            const int i = ord[t];
            seg[t + 1] = pst.o(seg[t], i, W[i]);
            X2.push_back(X[i]);
        }
        X = move(X2);
    }

    // [x1, x2) * [y1, y2)
    V sum(I xL, I xR, I yL, I yR) {
        assert(xL <= xR);
        assert(yL <= yR);
        const int l = LB(Y, yL), r = LB(Y, yR);
        return pst.v(seg[LB(X, xR)], l, r) - pst.v(seg[LB(X, xL)], l, r);
    }
};