#pragma once
#include "template.hpp"
#include "mod/pow.hpp"

// return non-trivial divisor
u64 pollard_rho(u64 n) {
    if(n % 2 == 0) return 2;
    if(nt::prime_test(n)) return n;
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
u64 euler_phi(i64 n, const vector<pair<u64, i32>>& pf) {
    for(auto [p, e] : pf) n -= n / p;
    return n;
}

vector<u64> divisor(const vector<pair<u64, i32>>& pf) {
    vector<u64> ds = {1};
    for(auto [p, e] : pf) {
        FOR(i, ssize(ds)) {
            u64 x = 1;
            FOR(j, e) x *= p, ds.push_back(ds[i] * x);
        }
    }
    sort(ds);
    return ds;
}

u64 order(u64 x, u64 p, const vector<pair<u64, int>>& pf) {
    u64 ord = p - 1;
    for(auto [q, e] : pf) {
        FOR(e) {
            if(modpow64(x, ord / q, p) == 1) ord /= q;
            else break;
        }
    }
    return ord;
}

u64 euler_phi(u64 n, const vector<pair<u64, int>>& pf_n) {
    for(auto [p, e] : pf_n) n -= n / p;
    return n;
}
