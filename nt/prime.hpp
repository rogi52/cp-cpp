#include "template.hpp"
#include "mod/pow.hpp"
#include "rnd.hpp"

// n is prime? judge by witness. 
bool miller_rabin(u64 n, vector<u64> witness) {
    if(n == 1) return false;
    if(n % 2 == 0) return n == 2;
    u64 d = n - 1;
    while(d % 2 == 0) d /= 2;
    for(u64 a : witness) if(a < n) {
        u64 y = modpow64(a, d, n), t = d;
        while(t != n - 1 and y != 1 and y != n - 1) {
            y = u128(y) * y % n;
            t *= 2;
        }
        if(y != n - 1 and t % 2 == 0) return false;
    }
    return true;
}

// n is prime?
bool prime_test(u64 n) {
    if(n < (u64(1) << 32)) return miller_rabin(n, {2, 7, 61});
    return miller_rabin(n, {2, 325, 9375, 28178, 450775, 9780504, 1795265022});
}

// return non-trivial divisor
u64 pollard_rho(u64 n) {
    if(n % 2 == 0) return 2;
    if(prime_test(n)) return n;
    while(true) {
        u64 R = rnd::i<u64>(2, n), x, y = rnd::i<u64>(2, n), ys, q = 1, g = 1, m = 128;
        auto f = [&](u64 x) {
            return (u128(x) * x % n + R) % n;
        };
        for(int r = 1; g == 1; r *= 2) {
            x = y;
            FOR(i, r) y = f(y);
            for(int k = 0; g == 1 and k < r; k += m) {
                ys = y;
                for(int i = 0; i < m and i < r - k; i++) {
                    q = u128(q) * ((x - (y = f(y)) + n) % n) % n;
                }
                g = gcd(q, n);
            }
        }
        if(g == n) { do { g = gcd((x - (ys = f(ys))), n); } while(g == 1); }
        if(g != n) return g;
    }
    return 0;
}
// p, p, p, q, q, r, ...
// sorted
vector<u64> factor(u64 n) {
    auto dfs = [&](auto&& dfs, u64 n) -> vector<u64> {
        if(n <= 1) return vector<u64>{};
        u64 d = pollard_rho(n);
        if(d == n) return vector<u64>{n};
        vector<u64> L = dfs(dfs, d), R = dfs(dfs, n / d);
        L.insert(L.end(), R.begin(), R.end());
        return L;
    };
    vector<u64> res = dfs(dfs, n);
    sort(res.begin(), res.end());
    return res;
}
// {p, 3}, {q, 2}, {r, 1}, ...
vector<pair<u64, i32>> factor_pair(u64 n) {
    vector<u64> pf = factor(n);
    vector<pair<u64, i32>> res;
    if(pf.empty()) return res;
    res.push_back({pf[0], 1});
    FOR(i, 1, ssize(pf)) {
        if(res.back().first == pf[i]) res.back().second++;
        else res.push_back({pf[i], 1});
    }
    return res;
}
u64 euler_phi(u64 n) {
    vector<pair<u64,i32>> pf = factor_pair(n);
    for(auto [p, e] : pf) n -= n / p;
    return n;
}