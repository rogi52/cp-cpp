#include "template.hpp"
#include "mod/modint.hpp"

namespace ntt {

template < class mint > void ntt(vector<mint>& a, bool inv) {
    const int n = ssize(a);
    if(n == 0) return;
    static u32 mod = mint::mod, root = mint::root;
    static bool init = true;
    static mint bw[30], ibw[30];
    if(init) {
        init = false;
        FOR(k, 30) ibw[k] = inv(bw[k] = pow(mint(root), (mod - 1) >> (k + 1)));
    }

    for(int i = 0, j = 1; j + 1 < n; j++) {
        for(int k = n >> 1; k > (i ^= k); k >>= 1);
        if(i > j) swap(a[i], a[j]);
    }
    for(int k = 0, t = 2; t <= n; k++, t <<= 1) {
        mint b = not inv ? bw[k] : ibw[k];
        for(int i = 0; i < n; i += t) {
            mint w = 1;
            for(int j = 0; j < t / 2; j++) {
                int j1 = i + j, j2 = i + j + t / 2;
                mint c1 = a[j1], c2 = a[j2] * w;
                a[j1] = c1 + c2;
                a[j2] = c1 - c2;
                w *= b;
            }
        }
    }
    if(inv) {
        mint x = inv(mint(n));
        FOR(i, n) a[i] *= x;
    }
}

template < class mint > vector<mint> naive(const vector<mint>& a, const vector<mint>& b) {
    const int n = ssize(a), m = ssize(b);
    if(n == 0 or m == 0) return {};
    vector<mint> c(n + m - 1, 0);
    FOR(i, n) FOR(j, m) c[i + j] += a[i] * b[j];
    return c;
}

using mint0 = static_modint<754'974'721, 1, 11>;
using mint1 = static_modint<167'772'161, 1,  3>;
using mint2 = static_modint<469'762'049, 1,  3>;
const mint1 imod0 ( 95'869'806); // m0^-1 mod m1
const mint2 imod1 (104'391'568); // m1^-1 mod m2
const mint2 imod01(187'290'749); // imod1 / m0

template < class mint > vector<mint> conv(vector<mint> a, vector<mint> b) {
    const int n = ssize(a), m = ssize(b);
    if(min(n, m) < 30) return naive(a, b);
    const u32 mod = mint::mod;
    const int sz = [&] {
        int n2 = 1; while(n2 < n) n2 <<= 1;
        int m2 = 1; while(m2 < n) m2 <<= 1;
        return max(n2, m2) << 1;
    }();

    if(mod == 998'244'353) {
        a.resize(sz); ntt(a, false);
        b.resize(sz); ntt(b, false);
        vector<mint> c(sz);
        FOR(i, sz) c[i] = a[i] * b[i];
        ntt(c, true); c.resize(n + m - 1);
        return c;
    }

    vector<mint0> a0(sz), b0(sz), c0(sz);
    vector<mint1> a1(sz), b1(sz), c1(sz);
    vector<mint2> a2(sz), b2(sz), c2(sz);
    FOR(i, n) a0[i].v = a1[i].v = a2[i].v = a[i].v;
    FOR(i, n) b0[i].v = b1[i].v = b2[i].v = b[i].v;
    ntt(a0, false); ntt(a1, false); ntt(a2, false);
    ntt(b0, false); ntt(b1, false); ntt(b2, false);
    FOR(i, sz) c0[i] = a0[i] * b0[i];
    FOR(i, sz) c1[i] = a1[i] * b1[i];
    FOR(i, sz) c2[i] = a2[i] * b2[i];
    ntt(c0, true); ntt(c1, true); ntt(c2, true);
    vector<mint> c(n + m - 1);
    const mint mod0 = mint0::mod;
    const mint mod01 = mod0 * mint1::mod;
    FOR(i, n + m - 1) {
        i64 y0 = c0[i].v;
        i64 y1 = (imod0 * (c1[i] - y0)).v;
        i64 y2 = (imod01 * (c2[i] - y0) - imod1 * y1).v;
        c[i] = mod01 * y2 + mod0 * y1 + y0;
    }
    return c;
}

// a^2
template < class mint > vector<mint> square(vector<mint> a) {
    const int n = ssize(a);
    if(n < 30) return naive(a, a);
    const u32 mod = mint::mod;
    const int sz = [&] {
        int n2 = 1; while(n2 < n) n2 <<= 1;
        return n2 << 1;
    }();

    if(mod == 998'244'353) {
        a.resize(sz); ntt(a, false);
        vector<mint> c(sz);
        FOR(i, sz) c[i] = a[i] * a[i];
        ntt(c, true); c.resize(n + n - 1);
        return c;
    }

    vector<mint0> a0(sz), c0(sz);
    vector<mint1> a1(sz), c1(sz);
    vector<mint2> a2(sz), c2(sz);
    FOR(i, n) a0[i].v = a1[i].v = a2[i].v = a[i].v;
    ntt(a0, false); ntt(a1, false); ntt(a2, false);
    FOR(i, sz) c0[i] = a0[i] * a0[i];
    FOR(i, sz) c1[i] = a1[i] * a1[i];
    FOR(i, sz) c2[i] = a2[i] * a2[i];
    ntt(c0, true); ntt(c1, true); ntt(c2, true);
    vector<mint> c(n + n - 1);
    const mint mod0 = mint0::mod;
    const mint mod01 = mod0 * mint1::mod;
    FOR(i, n + n - 1) {
        i64 y0 = c0[i].v;
        i64 y1 = (imod0 * (c1[i] - y0)).v;
        i64 y2 = (imod01 * (c2[i] - y0) - imod1 * y1).v;
        c[i] = mod01 * y2 + mod0 * y1 + y0;
    }
    return c;
}
}