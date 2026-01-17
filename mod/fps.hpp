#include "template.hpp"
#include "mod/ntt.hpp"
#include "mod/binom.hpp"

template < class mint >
void fmt(const int n, mint* const f, bool is_inv) {
    static constexpr u32 mod = mint::mod;
    static constexpr u32 mod2 = mod * 2;
    static const int L = 30;
    static mint g[L], ig[L], p2[L];
    if(g[0].v == 0) {
        FOR(i, L) {
            mint w = -pow(mint(mint::root), ((mod - 1) >> (i + 2)) * 3);
            g[i] = w;
            ig[i] = inv(w);
            p2[i] = inv(mint(1 << i));
        }
    }

    if(not is_inv) {
        int b = n;
        if(b >>= 1) {
            FOR(i, b) {
                u32 x = f[i + b].v;
                f[i + b].v = f[i].v + mod - x;
                f[i].v += x;
            }
        }
        if(b >>= 1) {
            mint p = 1;
            int k = 0;
            FOR(i, 0, n, b * 2) {
                FOR(j, i, i + b) {
                    u32 x = (f[j + b] * p).v;
                    f[j + b].v = f[j].v + mod - x;
                    f[j].v += x;
                }
                p *= g[bit::low(++k)];
            }
        }
        while(b) {
            if(b >>= 1) {
                mint p = 1;
                int k = 0;
                FOR(i, 0, n, b * 2) {
                    FOR(j, i, i + b) {
                        u32 x = (f[j + b] * p).v;
                        f[j + b].v = f[j].v + mod - x;
                        f[j].v += x;
                    }
                    p *= g[bit::low(++k)];
                }
            }
            if(b >>= 1) {
                mint p = 1;
                int k = 0;
                FOR(i, 0, n, b * 2) {
                    FOR(j, i, i + b) {
                        u32 x = (f[j + b] * p).v;
                        f[j].v = (f[j].v < mod2 ? f[j].v : f[j].v - mod2);
                        f[j + b].v = f[j].v + mod - x;
                        f[j].v += x;
                    }
                    p *= g[bit::low(++k)];
                }
            }
        }
    } else {
        int b = 1;
        if(b < n / 2) {
            mint p = 1;
            int k = 0;
            FOR(i, 0, n, b * 2) {
                FOR(j, i, i + b) {
                    u64 x = f[j].v + mod - f[j + b].v;
                    f[j].v += f[j + b].v;
                    f[j + b].v = x * p.v % mod;
                }
                p *= ig[bit::low(++k)];
            }
            b <<= 1;
        }
        for(; b < n / 2; b <<= 1) {
            mint p = 1;
            int k = 0;
            FOR(i, 0, n, b * 2) {
                for(int j = i; j < i + b / 2; j++) {
                    u64 x = f[j].v + mod2 - f[j + b].v;
                    f[j].v += f[j + b].v;
                    f[j].v = (f[j].v) < mod2 ? f[j].v : f[j].v - mod2;
                    f[j + b].v = x * p.v % mod;
                }
                for(int j = i + b / 2; j < i + b; j++) {
                    u64 x = f[j].v + mod - f[j + b].v;
                    f[j].v += f[j + b].v;
                    f[j + b].v = x * p.v % mod;
                }
                p *= ig[bit::low(++k)];
            }
        }
        if(b < n) {
            FOR(i, b) {
                u32 x = f[i + b].v;
                f[i + b].v = f[i].v + mod2 - x;
                f[i].v += x;
            }
        }
        const mint z = p2[__lg(n)];
        FOR(i, n) f[i] *= z;
    }
}

template < class mint > void fmt(vector<mint>& f) { fmt(ssize(f), f.data(), false); }
template < class mint > void fmt_inv(vector<mint>& f) { fmt(ssize(f), f.data(), true); }

template < class mint >
vector<mint> convolution(vector<mint> a, vector<mint> b) {
    const int n = ssize(a);
    const int m = ssize(b);
    const int k = n + m - 1;
    if(min(n, m) < 30) {
        vector<mint> c(k);
        FOR(i, n) FOR(j, m) c[i + j] += a[i] * b[j];
        return c;
    }

    const int k2 = bit::ceil(k);
    a.resize(k2); fmt(a);
    b.resize(k2); fmt(b);
    FOR(i, k2) a[i] *= b[i];
    fmt_inv(a); a.resize(k);
    return a;
}

namespace fps {
template < class mint >
vector<mint> prod(const vector<mint>& f, const vector<mint>& g) {
    return ntt::conv(f, g);
}
template < class mint >
vector<mint> prefix(const vector<mint>& f, int n) {
    vector<mint> g(n);
    FOR(i, min(n, (int)ssize(f))) g[i] = f[i]; 
    return g;
}
template < class mint >
vector<mint> inv(const vector<mint>& f, int n) {
    assert(1 <= ssize(f) and f[0] != 0);
    vector<mint> g(n);
    g[0] = mint(1) / f[0];
    for(int m = 1; m < n; m *= 2) {
        vector<mint> fm = prefix(f, 2 * m);
        fmt(fm);
        vector<mint> gm = prefix(g, 2 * m);
        fmt(gm);
        FOR(i, 2 * m) fm[i] *= gm[i];
        fmt_inv(fm);
        FOR(i, m) fm[i] = 0;
        fmt(fm);
        FOR(i, 2 * m) fm[i] *= gm[i];
        fmt_inv(fm);
        FOR(i, m, min(2 * m, n)) g[i] -= fm[i];
    }
    return g;
}
template < class mint >
vector<mint> derivative(const vector<mint>& f) {
    const int n = ssize(f);
    vector<mint> g(max(0, n - 1));
    FOR(i, 1, n) g[i - 1] = f[i] * i;
    return g;
}
template < class mint >
vector<mint> integral(const vector<mint>& f) {
    const int n = ssize(f);
    vector<mint> g(n + 1);
    FOR(i, n) g[i + 1] = f[i] * comb::inv<mint>(i + 1);
    return g;
}
template < class mint >
vector<mint> log(const vector<mint>& f, int n) {
    assert(1 <= ssize(f) and f[0] == 1);
    if(n == 1) return {0};
    return integral(prefix(prod(derivative(prefix(f, n)), inv(f, n - 1)), n - 1));
}
template < class mint >
vector<mint> exp(vector<mint> f, int n) {
    if(ssize(f) == 0) { vector<mint> e(n); e[0] = 1; return e; }
    assert(1 <= ssize(f) and f[0] == 0);

    vector<mint> g{1}, gg;
    f.resize(n);
    f[0] = 1;
    vector<mint> h = derivative(f);
    for(int m = 1; m < n; m *= 2) {
        vector<mint> ff = prefix(f, m);
        ff.resize(2 * m);
        fmt(ff);

        if(m != 1) {
            vector<mint> f2(m);
            FOR(i, m) f2[i] = ff[i] * gg[i];
            fmt_inv(f2);
            f2.erase(f2.begin(), f2.begin() + m / 2);
            f2.resize(m);
            fmt(f2);
            FOR(i, m) f2[i] *= gg[i];
            fmt_inv(f2);
            FOR(i, m / 2) f2[i] = -f2[i];
            g.insert(g.end(), f2.begin(), f2.begin() + m / 2);
        }

        vector<mint> t = derivative(prefix(f, m));
        t.resize(m);
        {
            vector<mint> r(h.begin(), h.begin() + (m - 1));
            r.resize(m);
            fmt(r);
            FOR(i, m) r[i] *= ff[i];
            fmt_inv(r);
            FOR(i, m) t[i] -= r[i];
            t.insert(t.begin(), t.back());
            t.pop_back();
        }

        t.resize(2 * m);
        fmt(t);
        gg = g;
        gg.resize(2 * m);
        fmt(gg);
        FOR(i, 2 * m) t[i] *= gg[i];
        fmt_inv(t);
        t.resize(m);

        vector<mint> v(f.begin() + m, f.begin() + min(n, 2 * m));
        v.resize(m);
        t.insert(t.begin(), m - 1, 0);
        t.push_back(0);
        t = integral(move(t));
        FOR(i, m) v[i] -= t[m + i];

        v.resize(2 * m);
        fmt(v);
        FOR(i, 2 * m) v[i] *= ff[i];
        fmt_inv(v);
        v.resize(m);
        FOR(i, min(n - m, m)) f[m + i] = v[i];
    }
    return f;
}
template < class mint >
vector<mint> pow(vector<mint> f, i64 e, int n) {
    if(e == 0) { vector<mint> g(n, 0); g[0] = 1; return g; }
    int i = 0;
    while(i < ssize(f) and f[i] == 0) i += 1;
    if(i == ssize(f) or n <= i128(e) * i) return vector<mint>(n, 0);
    const mint c = f[i];
    const mint ic = inv(c);
    const int o = e * i;
    vector<mint> g(f.begin() + i, f.end());
    FOR(i, ssize(g)) g[i] *= ic;
    g = log(move(g), n - o);
    const mint me = mint(e);
    FOR(i, ssize(g)) g[i] *= me;
    g = exp(move(g), n - o);
    const mint ce = pow(c, e);
    FOR(i, ssize(g)) g[i] *= ce;
    g.insert(g.begin(), o, 0);
    return g;
}
template < class mint > 
vector<mint> inv_sparse(const vector<pair<int, mint>>& f, int n) {
    assert(1 <= ssize(f) and f[0].first == 0 and f[0].second != 0);
    vector<mint> g(n);
    g[0] = inv(f[0].second);
    FOR(i, 1, n) {
        mint& x = g[i];
        for(auto [k, fk] : f) {
            if(i < k) break;
            x += fk * g[i - k];
        }
        g[i] *= -g[0];
    }
    return g;
}
template < class mint >
vector<mint> pow_sparse(vector<pair<int, mint>> f, i64 e, int n) {
    if(e == 0) { vector<mint> g(n, 0); g[0] = 1; return g; }
    if(ssize(f) == 0 or n <= i128(e) * f[0].first) return vector<mint>(n, 0);
    const mint c = f[0].second;
    const mint ic = inv(c);
    const mint me = mint(e);
    const int d = f[0].first, o = e * d, m = n - o;
    for(auto &[i, fi] : f) i -= d, fi *= ic;
    vector<mint> g(m);
    g[0] = 1;
    FOR(i, 1, m) {
        mint& x = g[i];
        for(auto [k, fk] : f) {
            if(i < k) break;
            x += fk * g[i - k] * (me * k - (i - k));
        }
        x *= comb::inv<mint>(i);
    }
    const mint ce = pow(c, e);
    FOR(i, m) g[i] *= ce;
    g.insert(g.begin(), o, 0);
    return g;
}
} // namespace fps