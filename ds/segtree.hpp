#include "template.hpp"

template < class Monoid > struct segtree {
    using M = Monoid;
    using T = typename M::value_type;

    segtree() : segtree(0) {}
    segtree(int n) : segtree(n, M::e()) {}
    segtree(int n, T x) : segtree(vector(n, x)) {}
    segtree(const vector< T >& a_) : n(ssize(a_)) {
        lg = ce2(n);
        sz = 1 << lg;
        a = vector(sz + sz, M::e());
        FOR(i, n) a[sz + i] = a_[i];
        REV(i, 1, sz) update(i);
    }

    // a[i] <- x
    void set(int i, T x) {
        assert(0 <= i and i < n);
        i += sz;
        a[i] = x;
        FOR(p, 1, lg + 1) update(i >> p);
    }
    // a[i]
    T v(int i) {
        assert(0 <= i and i < n);
        return a[i + sz];
    }
    // [l, r)
    T v(int l, int r) {
        assert(0 <= l and l <= r and r <= n);
        T sl = M::e(), sr = M::e();
        for(l += sz, r += sz; l < r; l >>= 1, r >>= 1) {
            if(l & 1) sl = M::op(sl, a[l++]);
            if(r & 1) sr = M::op(a[--r], sr);
        }
        return M::op(sl, sr);
    }
    // [0, n)
    T av() { return a[1]; }

    template < class F > int max_right(int l, F f) {
        assert(0 <= l and l <= n);
        assert(f(M::e()));
        if(l == n) return n;
        l += sz;
        T s = M::e();
        do {
            while(l % 2 == 0) l >>= 1;
            if(not f(M::op(s, a[l]))) {
                while(l < sz) {
                    l = 2 * l;
                    if(f(M::op(s, a[l]))) s = M::op(s, a[l++]);
                }
                return l - sz;
            }
            s = M::op(s, a[l]);
            l++;
        } while((l & -l) != l);
        return n;
    }
    template < class F > int min_left(int r, F f) {
        assert(0 <= r and r <= n);
        assert(f(M::e()));
        if(r == 0) return 0;
        r += sz;
        T s = M::e();
        do {
            r--;
            while(r > 1 and r % 2 == 1) r >>= 1;
            if(not f(M::op(a[r], s))) {
                while(r < sz) {
                    r = 2 * r + 1;
                    if(f(M::op(a[r], s))) s = M::op(a[r--], s);
                }
                return r + 1 - sz;
            }
            s = M::op(a[r], s);
        } while((r & -r) != r);
        return 0;
    }

  private:
    int n, sz, lg;
    vector< T > a;
    int ce2(int n) { int e = 0; while((1 << e) < n) e++; return e; }
    void update(int k) { a[k] = M::op(a[2 * k], a[2 * k + 1]); }
};