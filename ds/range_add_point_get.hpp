#include "template.hpp"
#include "ds/fwktree.hpp"
#include "alg/sum.hpp"

// Fenwick Tree を使って定数倍高速化
template < class T > struct range_add_point_get {
    int n;
    fwktree<alg::sum< T >> fwk;
    range_add_point_get(int n) : n(n), fwk(n + 1) {}

    // a[l, r) += x
    void add(int l, int r, T x) {
        assert(0 <= l and l <= r and r <= n);
        fwk.add(l,  x);
        fwk.add(r, -x);
    }
    T get(int i) {
        assert(0 <= i and i < n);
        return fwk.pv(i + 1);
    }
};