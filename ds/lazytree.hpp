#include "template.hpp"

template < class A > struct lazytree {
    using V = typename A::value_structure;
    using T = typename V::value_type;
    using O = typename A::operator_structure;
    using F = typename O::value_type;

    int n, sz, lg;
    vector< T > a;
    vector< F > lz;

    int ce2(int n) {
        int x = 0;
        while((1 << x) < n) x++;
        return x;
    }
    void update(int k) {
        a[k] = V::op(a[2 * k], a[2 * k + 1]);
    }
    void all_apply(int k, F f) {
        a[k] = A::op(a[k], f);
        if(k < sz) lz[k] = O::op(lz[k], f);
    }
    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = O::e();
    }

    lazytree() : lazytree(0) {}
    lazytree(int n) : lazytree(vector< T >(n, V::e())) {}
    lazytree(int n, T x) : lazytree(vector< T >(n, x)) {}
    lazytree(const vector< T >& a_) : n(ssize(a_)) {
        lg = ce2(n);
        sz = 1 << lg;
        a = vector< T >(sz + sz, V::e());
        lz = vector< F >(sz, O::e());
        FOR(i, n) a[sz + i] = a_[i];
        REV(i, 1, sz) update(i);
    }
    void set(int i, T x) {
        assert(0 <= i and i < n);
        i += sz;
        REV(p, 1, lg + 1) push(i >> p);
        a[i] = x;
        FOR(p, 1, lg + 1) update(i >> p);
    }
    T v(int i) {
        assert(0 <= i and i < n);
        i += sz;
        REV(p, 1, lg + 1) push(i >> p);
        return a[i];
    }
    T v(int l, int r) {
        assert(0 <= l and l <= r and r <= n);
        if(l == r) return V::e();
        l += sz, r += sz;
        REV(i, 1, lg + 1) {
            if(((l >> i) << i) != l) push(l >> i);
            if(((r >> i) << i) != r) push((r - 1) >> i);
        }
        T sl = V::e(), sr = V::e();
        while(l < r) {
            if(l & 1) sl = V::op(sl, a[l++]);
            if(r & 1) sr = V::op(a[--r], sr);
            l >>= 1, r >>= 1;
        }
        return V::op(sl, sr);
    }
    T av() { return a[1]; }
    void o(int i, F f) {
        assert(0 <= i and i < n);
        i += sz;
        REV(p, 1, lg + 1) push(i >> p);
        a[i] = O::op(a[i], f);
        FOR(p, 1, lg + 1) update(i >> p);
    }
    void o(int l, int r, F f) {
        assert(0 <= l and l <= r and r <= n);
        if(l == r) return;
        l += sz, r += sz;
        REV(i, 1, lg + 1) {
            if(((l >> i) << i) != l) push(l >> i);
            if(((r >> i) << i) != r) push((r - 1) >> i);
        }
        {
            int l2 = l, r2 = r;
            while(l < r) {
                if(l & 1) all_apply(l++, f);
                if(r & 1) all_apply(--r, f);
                l >>= 1, r >>= 1;
            }
            l = l2, r = r2;
        }
        FOR(i, 1, lg + 1) {
            if(((l >> i) << i) != l) update(l >> i);
            if(((r >> i) << i) != r) update((r - 1) >> i);
        }
    }
    template < class G > int max_right(int l, G g) {
        assert(0 <= l and l <= n);
        assert(g(V::e()));
        if(l == n) return n;
        l += sz;
        REV(i, 1, lg + 1) push(l >> i);
        T s = V::e();
        do {
            while(l % 2 == 0) l >>= 1;
            if(not g(V::op(s, a[l]))) {
                while(l < sz) {
                    push(l);
                    l = 2 * l;
                    if(g(V::op(s, a[l]))) {
                        s = V::op(s, a[l]);
                        l++;
                    }
                }
                return l - sz;
            }
            s = V::op(s, a[l]);
            l++;
        } while((l & -l) != l);
        return n;
    }
    template < class G > int min_left(int r, G g) {
        assert(0 <= r and r <= n);
        if(r == 0) return 0;
        r += sz;
        REV(i, 1, lg + 1) push((r - 1) >> i);
        T s = V::e();
        do {
            r--;
            while(r > 1 && (r % 2)) r >>= 1;
            if(not g(V::op(a[r], s))) {
                while(r < sz) {
                    push(r);
                    r = 2 * r + 1;
                    if(g(V::op(a[r], s))) {
                        s = V::op(a[r], s);
                        r--;
                    }
                }
                return r + 1 - sz;
            }
            s = V::op(a[r], s);
        } while((r & -r) != r);
        return 0;
    }
};