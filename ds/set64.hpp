#include "template.hpp"

// 参考: https://judge.yosupo.jp/submission/170327 by maspy
struct set64 {
    static constexpr u32 B = 64;
    int n, lg;
    vector<vector<u64>> a;
    set64() {}
    set64(int n) : n(n) {
        do { a.emplace_back(n = (n + B - 1) / B); } while(n > 1);
        lg = ssize(a);
    }
    // i in S <=> b[i] = 1
    set64(const vector<int>& b) : set64(ssize(b)) {
        FOR(i, n) a[0][i / B] |= u64(b[i]) << (i % B);
        FOR(h, lg - 1) FOR(i, ssize(a[h])) {
            a[h + 1][i / B] |= u64(bool(a[h][i])) << (i % B);
        }
    }
    void insert(int i) {
        assert(0 <= i and i < n);
        FOR(h, lg) a[h][i / B] |= u64(1) << (i % B), i /= B;
    }
    void erase(int i) {
        assert(0 <= i and i < n);
        u64 x = 0;
        FOR(h, lg) {
            a[h][i / B] &= ~(u64(1) << (i % B));
            a[h][i / B] |= x << (i % B);
            x = bool(a[h][i / B]);
            i /= B;
        }
    }
    bool contains(int i) {
        return a[0][i / B] >> (i % B) & 1;
    }
    int next(int i) {
        assert(i <= n);
        chmax(i, 0);
        FOR(h, lg) {
            if(i / B == ssize(a[h])) break;
            u64 d = a[h][i / B] >> (i % B);
            if(!d) { i = i / B + 1; continue; }
            i += bit::low(d);
            REV(g, h) {
                i *= B;
                i += bit::low(a[g][i / B]);
            }
            return i;
        }
        return n;
    }
    int prev(int i) {
        assert(-1 <= i);
        if(n <= i) i = n - 1;
        FOR(h, lg) {
            if(i == -1) break;
            u64 d = a[h][i / B] << (63 - i % B);
            if(!d) { i = i / B - 1; continue; }
            i -= __builtin_clzll(d);
            REV(g, h) {
                i *= B;
                i += bit::top(a[g][i / B]);
            }
            return i;
        }
        return -1;
    }
};