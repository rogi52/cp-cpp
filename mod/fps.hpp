#include "template.hpp"
#include "mod/ntt.hpp"

namespace fps {
template < class mint > vector<mint> prod(vector<mint> f, vector<mint> g) {
    return ntt::conv(f, g);
}
template < class mint > vector<mint> square(vector<mint> f) {
    return ntt::square(f);
}
template < class mint > vector<mint> prefix(const vector<mint>& f, int n) {
    return vector<mint>(f.begin(), f.begin() + min(ssize(f), n));
}
template < class mint > void up(vector<mint>& f, int n) {
    if(ssize(f) < n) f.resize(n);
}
template < class mint > vector<mint> inv(const vector<mint>& f, int n) {
    assert(f[0] != mint(0));
    vector<mint> g = {mint(1) / f[0]};
    for(int k = 1; k < n; k <<= 1) {
        vector<mint> ggf = prod(square(g), prefix(f, k << 1));
        vector<mint> h(k << 1);
        FOR(i, k << 1) h[i] = g[i] + g[i] - ggf[i];
        g = move(h);
    }
    g.resize(n);
    return g;
}
template < class mint > vector<mint> log(const vector<mint>& f, int n) {
    assert(f[0] == mint(1));
    const int s = ssize(f);
    vector<mint> df(s - 1);
    FOR(i, 1, s) df[i - 1] = f[i] * i;
    vector<mint> g = prod(df, inv(f, n));
    g.resize(n);
    return g;
}
template < class mint > vector<mint> exp(const vector<mint>& f, int n) {
    assert(f[0] == mint(0));
    vector<mint> g = {1};
    for(int k = 1; k < n; k <<= 1) {
        vector<mint> h = log(g, k << 1);
        up(f, k << 1);
        FOR(i, k << 1) h[i] = f[i] - h[i];
        h[0] += 1;
        g = prod(g, h);
    }
    g.resize(n);
    return g;
}
template < class mint > vector<mint> pow(const vector<mint>& f, u64 e, int n) {
    if(e == 0) {
        vector<mint> g(n, 0);
        g[0] = 1;
        return g;
    }
    int s = 0; while(s < ssize(f) and f[s] == 0) s++;
    if(s == ssize(f) or n <= u128(s) * e) return vector<mint>(n, 0);
    const mint k = f[s];
    const mint ik = mint(1) / k;
    vector<mint> g = {f.begin() + s, f.end()};
    FOR(i, ssize(g)) g[i] *= ik;
    g = log(g, n);
    const mint me = e;
    FOR(i, n) g[i] *= me;
    g = exp(g, n);
    const mint pk = pow(k, e);
    FOR(i, n) g[i] *= e;
    g.insert(g.begin(), s * e, 0);
    g.resize(n);
    return g;
}
template < class mint > vector<mint> sqrt(const vector<mint>& f, int n) {
    const int s = ssize(f), d = n;
    REV(i, n) if(f[i] != mint(0)) d = i;
    if(d == n) return f;
    if(d % 2 == 1) { assert(0); }
    // TODO
}
}