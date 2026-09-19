#include "template.hpp"
#include "nt/prime.hpp"

// O(n^2)-judge
u64 primitive_root(u64 p) {
    // assert [p is prime]
    vector<u64> pf = factor(p - 1);
    unique(pf);
    for(u64& q : pf) q = (p - 1) / q;
    while(true) {
        const u64 g = rnd::i<u64>(1, p);
        if([&] {
            for(const u64 q : pf) if(modpow64(g, q, p) == 1) return false;
            return true;
        }()) return g;
    }
    return 0;
}

// O(nlogn)-judge
u64 primitive_root_dc(u64 p) {
    if(p == 2) return 1;
    vector<u64> pf = factor(p - 1);
    unique(pf);
    u64 Q = 1;
    for(u64 q : pf) Q *= q;

    auto dfs = [&](auto&& dfs, u64 now, int l, int r) -> bool {
        if(now == 1) return false;
        if(l + 1 == r) return true;
        const int m = (l + r) / 2;
        u64 xL = 1, xR = 1;
        FOR(i, l, m) xL *= pf[i];
        FOR(i, m, r) xR *= pf[i];
        if(not dfs(dfs, modpow64(now, xR, p), l, m)) return false;
        if(not dfs(dfs, modpow64(now, xL, p), m, r)) return false;
        return true;
    };

    while(true) {
        const u64 g = rnd::i<u64>(1, p);
        const u64 X = modpow64(g, (p - 1) / Q, p);
        if(dfs(dfs, X, 0, ssize(pf))) return g;
    }
    return 0;
}

// 全体 O(nlogn)
u64 primitive_root_expected_fast(u64 p) {
    if(p == 2) return 1;
    vector<pair<u64, i32>> pf = factor_pair(p - 1);
    
    vector<pair<u64, u64>> rem_pf;
    for(auto [q, e] : pf) {
        u64 q_e = 1;
        FOR(i, e) q_e *= q;
        rem_pf.push_back({q, q_e});
    }

    u64 ret = 1;
    u64 outer = 1;

    while(not rem_pf.empty()) {
        u64 a = rnd::i<u64>(1, p);
        
        u64 A = modpow64(a, outer, p);
        u64 inner = (p - 1) / outer;

        u64 Q_rem = 1;
        for(auto [q, q_e] : rem_pf) Q_rem *= q;
        u64 X = modpow64(A, inner / Q_rem, p);

        vector<u64> qs(rem_pf.size());
        FOR(i, ssize(rem_pf)) qs[i] = rem_pf[i].first;

        vector<bool> ok(qs.size(), false);
        auto dfs = [&](auto&& dfs, u64 now, int l, int r) -> void {
            if(now == 1) return;
            if(l + 1 == r) {
                ok[l] = true;
                return;
            }
            const int m = (l + r) / 2;
            u64 xL = 1, xR = 1;
            FOR(i, l, m) xL *= qs[i];
            FOR(i, m, r) xR *= qs[i];
            dfs(dfs, modpow64(now, xR, p), l, m);
            dfs(dfs, modpow64(now, xL, p), m, r);
        };
        dfs(dfs, X, 0, qs.size());

        vector<pair<u64, u64>> next_rem;
        FOR(i, ssize(rem_pf)) {
            auto [q, q_e] = rem_pf[i];
            if(ok[i]) {
                outer *= q_e;
            } else {
                A = modpow64(A, q_e, p);
                next_rem.push_back({q, q_e});
            }
        }
        ret = (u128)ret * A % p;
        rem_pf = next_rem;
    }

    return ret;
}

// g^i = x
// return {x_to_i, i_to_x}
pair<vector<int>, vector<int>> g_trans(int g, int p) {
    vector<int> x_to_i(p), i_to_x(p);
    int x = 1;
    FOR(i, p-1) {
        x_to_i[x] = i;
        i_to_x[i] = x;
        x = u64(x) * g % p;
    }
    return {x_to_i, i_to_x};
}

// O(nlogn)
u64 order_dc(u64 x, u64 p, const vector<pair<u64, i32>>& pf) {
    const int k = ssize(pf);
    if(k == 0) return 1;
    vector<u64> factors(k);
    FOR(i, k) {
        u64 f = 1;
        FOR(pf[i].second) f *= pf[i].first;
        factors[i] = f;
    }
    vector<u64> xi(k);
    auto dfs = [&](auto&& dfs, int L, int R, u64 v) -> void {
        if(L + 1 == R) {
            xi[L] = v;
            return;
        }
        const int M = std::midpoint(L, R);
        u64 Lv = v, Rv = v;
        FOR(i, M, R) Lv = modpow64(Lv, factors[i], p);
        FOR(i, L, M) Rv = modpow64(Rv, factors[i], p);
        dfs(dfs, L, M, Lv);
        dfs(dfs, M, R, Rv);
    };
    dfs(dfs, 0, k, x);

    u64 res = 1;
    FOR(i, k) {
        u64 q = pf[i].first;
        u64 y = xi[i];
        int c = 0;
        while(y != 1) {
            y = modpow64(y, q, p);
            c += 1;
        }
        FOR(c) res *= q;
    }
    return res;
}

