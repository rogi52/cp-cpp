#include "template.hpp"

template < class CMonoid > struct fwktree {
  private:
    using M = CMonoid;
    using T = typename M::value_type;
    int n, n2;
    vector< T > a;

  public:
    fwktree() : fwktree(0) {}
    fwktree(int n) : n(n), a(n + 1, M::e()) { assert(M::comm()); }
    fwktree(const vector< T >& a_) : n(ssize(a_)), a(a_) {
        assert(M::comm());
        a.insert(a.begin(), {M::e()});
        for(int i = 1; i <= n; i++) {
            const int p = i + (i & -i);
            if(p <= n) a[p] = M::op(a[i], a[p]);
        }
    }
    // a[i] <- a[i] + x
    void add(int i, T x) {
        for(int p = i + 1; p <= n; p += p & -p) a[p] = M::op(a[p], x);
    }
    // [0, r)
    T pv(int r) {
        T s = M::e();
        for(int p = r; p > 0; p -= p & -p) s = M::op(a[p], s);
        return s;
    }
    // [l, r)
    T v(int l, int r) { return M::op(M::inv(pv(l)), pv(r)); }
    // a[i]
    T v(int i) { return v(i, i + 1); }
    // a[i] <- x
    void set(int i, T x) { add(i, M::op(M::inv(v(i)), x)); }
};