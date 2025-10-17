#pragma once
#include "template.hpp"

template < class Monoid > struct per_segtree {
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

    int n;
    per_segtree() {}
    per_segtree(int n) : n(n) {}
    
    ptr build() { return build(M::e()); }
    ptr build(const V& v) { return build(vector(n, v)); }
    ptr build(const vector< V >& v) {
        assert(ssize(v) == n);
        return build(0, n, v);
    }

    // a[i] <- v
    ptr set(ptr a, int i, const V& v) {
        assert(0 <= i and i < n);
        return set(i, v, a, 0, n);
    }
    // a[l, r)
    V v(ptr a, int l, int r) {
        assert(0 <= l and l <= r and r <= n);
        return prod(l, r, a, 0, n);
    }
    // a[i]
    V v(ptr a, int i) {
        assert(0 <= i and i < n);
        return prod(i, i + 1, a, 0, n);
    }
    // a[0, n)
    V av(ptr a) {
        return a->v;
    }
    // a[i] <- op(a[i], v)
    ptr o(ptr a, int i, const V& v) {
        assert(0 <= i and i < n);
        return apply(i, v, a, 0, n);
    }

  private:
    ptr merge(ptr l, ptr r) {
        return new node(M::op(l->v, r->v), l, r);
    }
    ptr build(int l, int r, const vector<V>& v) {
        if(l + 1 == r) return new node(v[l]);
        const int m = (l + r) / 2;
        return merge(build(l, m, v), build(m, r, v));
    }
    ptr set(int i, const V& v, ptr p, int l, int r) {
        if(r <= i or i + 1 <= l) return p;
        if(i <= l and r <= i + 1) return new node(v);
        const int m = (l + r) / 2;
        return merge(set(i, v, p->l, l, m), set(i, v, p->r, m, r));
    }
    ptr apply(int i, const V& v, ptr p, int l, int r) {
        if(r <= i or i + 1 <= l) return p;
        if(i <= l and r <= i + 1) return new node(M::op(p->v, v));
        const int m = (l + r) / 2;
        return merge(apply(i, v, p->l, l, m), apply(i, v, p->r, m, r));
    }
    V prod(int a, int b, ptr p, int l, int r) {
        if(r <= a or b <= l) return M::e();
        if(a <= l and r <= b) return p->v;
        const int m = (l + r) / 2;
        return M::op(prod(a, b, p->l, l, m), prod(a, b, p->r, m, r));
    }
};

namespace util {
// 個数を持った永続セグ木 (重み=+1) 用 kth 
// https://yukicoder.me/problems/no/924
// https://yukicoder.me/submissions/1127738
// (問題を知った場所: https://qiita.com/hotman78/items/9c643feae1de087e6fc5)
template < class ptr > int kth(ptr pl, ptr pr, int l, int r, int k) {
    if(l + 1 == r) return l;
    const int m = (l + r) / 2; 
    auto c = pr->l->v - pl->l->v;
    if(k < c)
        return kth(pl->l, pr->l, l, m, k);
    else
        return kth(pl->r, pr->r, m, r, k - c);
}
}