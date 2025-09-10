#include "template.hpp"

// https://noshi91.hatenablog.com/entry/2023/04/07/165310
// https://judge.yosupo.jp/submission/313847
template < class Monoid > struct disjoint_sparse_table {
    using M = Monoid;
    using T = typename M::value_type;
    int n;
    vector<vector<T>> t;
    disjoint_sparse_table(const vector<T>& a) : n(ssize(a)) {
        const int m = 32 - __builtin_clz(n + 1);
        t.assign(m, vector(n + 2, M::e()));
        FOR(k, 1, m) {
            vector<T>& s = t[k];
            const int w = 1 << k;
            for(int i = w; i < n + 2; i += w << 1) {
                REV(j, i-w+1, i) s[j - 1] = M::op(a[j - 1], s[j]);
                FOR(j, i, min(i+w-1, n+1)) s[j + 1] = M::op(s[j], a[j - 1]);
            }
        }
    }
    T get(int i) const {
        assert(0 <= i and i < n);
        return prod(i, i + 1);
    }
    T prod(int l, int r) const {
        assert(0 <= l and l <= r and r <= n);
        r++;
        const vector<T>& s = t[31 - __builtin_clz(l ^ r)];
        return M::op(s[l], s[r]);
    }
};