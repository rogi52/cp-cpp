#include "template.hpp"

template < class Index, class Monoid > struct dyn_per_segtree {
    using M = Monoid;
    using V = typename M::value_type;
    struct node;
    using ptr = node*;
    struct node {
        V v;
        ptr l, r;
        node() {}
        node(const V& v) : v(v), l(nullptr), r(nullptr) {}
        node(const V& v, const ptr& l, const ptr& r) : v(v), l(l), r(r) {}
    };

    Index n;
    dyn_per_segtree(Index n) : n(n) {}
    
    ptr build() { return nullptr; }

    // a[i] <- v
    ptr set(ptr a, Index i, const V& v) {
        assert(0 <= i and i < n);
        return set(i, v, a, 0, n);
    }
    // a[l, r)
    V v(ptr a, Index l, Index r) {
        assert(0 <= l and l <= r and r <= n);
        return prod(l, r, a, 0, n);
    }
    // a[i]
    V v(ptr a, Index i) {
        assert(0 <= i and i < n);
        return prod(i, i + 1, a, 0, n);
    }
    // a[0, n)
    V av(ptr a) {
        return a->v;
    }
    // a[i] <- op(a[i], v)
    ptr o(ptr a, Index i, const V& v) {
        assert(0 <= i and i < n);
        return apply(i, v, a, 0, n);
    }

  private:
    ptr merge(ptr l, ptr r) {
        V lv = l ? l->v : M::e();
        V rv = r ? r->v : M::e();
        return new node(M::op(lv, rv), l, r);
    }
    ptr set(int i, const V& v, ptr p, int l, int r) {
        if(r <= i or i + 1 <= l) return p;
        if(i <= l and r <= i + 1) return new node(v);
        if(p == nullptr) p = new node(M::e());
        const int m = (l + r) / 2;
        return merge(set(i, v, p->l, l, m), set(i, v, p->r, m, r));
    }
    ptr apply(int i, const V& v, ptr p, int l, int r) {
        if(r <= i or i + 1 <= l) return p;
        if(p == nullptr) p = new node(M::e());
        if(i <= l and r <= i + 1) return new node(M::op(p->v, v));
        const int m = (l + r) / 2;
        return merge(apply(i, v, p->l, l, m), apply(i, v, p->r, m, r));
    }
    V prod(int a, int b, ptr p, int l, int r) {
        if(r <= a or b <= l) return M::e();
        if(!p) return M::e();
        if(a <= l and r <= b) return p->v;
        const int m = (l + r) / 2;
        return M::op(prod(a, b, p->l, l, m), prod(a, b, p->r, m, r));
    }
};

namespace util {
// 動的永続セグ木用 sum[l, r) <= k なる最大の r を求める
// https://contest.ucup.ac/submission/1512735
template < class ptr > int ksum(ptr pl, ptr pr, int l, int r, i64 k) {
    if(l + 1 == r) return l;
    const int m = (l + r) / 2;
    const i64 s = (pr and pr->l ? pr->l->v : 0) - (pl and pl->l ? pl->l->v : 0);
    if(s > k) {
        return ksum(pl ? pl->l : nullptr, pr ? pr->l : nullptr, l, m, k);
    } else {
        return ksum(pl ? pl->r : nullptr, pr ? pr->r : nullptr, m, r, k - s);
    }
};
}