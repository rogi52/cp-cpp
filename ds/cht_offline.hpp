#include "template.hpp"

template < class T > class CHT_offline_get_min {
  private:
    struct Line {
        T a, b;
        Line(T a, T b) : a(a), b(b) {}
        T eval(T x) { return a * x + b; }
    };

    T sgn;
    int n;
    vector< Line > ls;
    vector< T > xs;

  public:
    T inf = numeric_limits< T >::max();

    CHT_offline_get_min(vector< T > &x, bool is_min = true) : xs(x) {
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        n = xs.size();
        ls.assign(n << 1, Line(0, inf));
        sgn = is_min ? +1 : -1;
    }

    void add_line(T a, T b) { update(a, b, 0, n); }

    void add_segment(T a, T b, T l, T r) {
        int xl = distance(xs.begin(), lower_bound(xs.begin(), xs.end(), l));
        int xr = distance(xs.begin(), lower_bound(xs.begin(), xs.end(), r));
        update(a, b, xl, xr);
    }

    T query(T x) {
        int i = distance(xs.begin(), lower_bound(xs.begin(), xs.end(), x));
        assert(i != n && x == xs[i]);
        T v = inf;
        for(i += n; i > 0; i >>= 1) v = min(v, ls[i].eval(x));
        return sgn * v;
    }

  private:
    void update(T a, T b, int l, int r) {
        a *= sgn, b *= sgn;
        Line f(a, b);
        for(l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if(l & 1) descend(f, l++);
            if(r & 1) descend(f, --r);
        }
    }

    void descend(Line g, int i) {
        int l = i, r = i + 1;
        while(l < n) l <<= 1, r <<= 1;
        while(l < r) {
            int m = (l + r) >> 1;
            T xl = xs[l - n], xm = xs[m - n], xr = xs[r - 1 - n];
            Line &f = ls[i];
            if(f.eval(xl) <= g.eval(xl) && f.eval(xr) <= g.eval(xr)) return;
            if(f.eval(xl) >= g.eval(xl) && f.eval(xr) >= g.eval(xr)) { f = g; return; }
            if(f.eval(xm) >  g.eval(xm)) swap(f, g);
            if(f.eval(xl) >  g.eval(xl)) i = i << 1 | 0, r = m; else i = i << 1 | 1, l = m;
        }
    }
};