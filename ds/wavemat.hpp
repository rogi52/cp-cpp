#include "template.hpp"


struct bit_vector {
    using u8 = unsigned char;
    vector<u64> buf;
    vector<int> sum;
    bit_vector() {}
    bit_vector(const vector<u8>& a) {
        const int n = ssize(a);
        const int m = (n + 63) >> 6;
        buf.assign(m, 0);
        sum.assign(m + 1, 0);
        FOR(i, n) if(a[i]) {
            buf[i >> 6] |= u64(1) << (i & 63);
            sum[(i >> 6) + 1] += 1;
        }
        FOR(i, m) sum[i + 1] += sum[i];
    }
    int rank(int k, int f = 1) {
        const int res = sum[k >> 6] + popcount(buf[k >> 6] & ((u64(1) << (k & 63)) - 1));
        return f ? res : k - res;
    }
};

// Wavelet Matrix
// max(Int) < 2^LG
template < class Int, int Lg > struct wavemat {
    using u8 = unsigned char;
    int n;
    vector<int> mid;
    vector<bit_vector> buf;
    Int INF;
    wavemat(vector<Int> a) : n(ssize(a)), mid(Lg), buf(Lg), INF(Int(1) << Lg) {
        vector<u8> b(n);
        REV(d, Lg) {
            vector<Int> nxt[2];
            FOR(i, n) nxt[b[i] = a[i] >> d & 1].push_back(a[i]);
            mid[d] = ssize(nxt[0]);
            buf[d] = bit_vector(b);
            a.clear();
            a.insert(a.end(), nxt[0].begin(), nxt[0].end());
            a.insert(a.end(), nxt[1].begin(), nxt[1].end());
        }
    }

    // count x
    int rank(int l, int r, Int x) {
        if(l == r) return 0;
        if(INF <= x) return 0;
        REV(d, Lg) {
            const int f = x >> d & 1;
            l = buf[d].rank(l, f) + (f ? mid[d] : 0);
            r = buf[d].rank(r, f) + (f ? mid[d] : 0);
        }
        return r - l;
    }
    // k-th smallest
    Int quantile(int l, int r, int k) {
        Int ans = 0;
        REV(d, Lg) {
            const int l2 = buf[d].rank(l, 0);
            const int r2 = buf[d].rank(r, 0);
            if(k < r2 - l2) {
                l = l2;
                r = r2;
            } else {
                k -= r2 - l2;
                ans |= Int(1) << d;
                l += mid[d] - l2;
                r += mid[d] - r2;
            }
        }
        return ans;
    }
    // count *<x
    int freq(int l, int r, Int x) {
        if(INF <= x) return r - l;
        Int ans = 0;
        REV(d, Lg) {
            const int f = x >> d & 1;
            if(f) ans += buf[d].rank(r, 0) - buf[d].rank(l, 0);
            l = buf[d].rank(l, f) + (f ? mid[d] : 0);
            r = buf[d].rank(r, f) + (f ? mid[d] : 0);
        }
        return ans;
    }
    int freq(int l, int r, Int lx, Int rx) {
        return freq(l, r, rx) - freq(l, r, lx);
    }
    Int prev(int l, int r, Int x) {
        const int c = freq(l, r, x);
        return c == r - l ? INF : quantile(l, r, c);
    }
    Int next(int l, int r, Int x) {
        const int c = freq(l, r, x);
        return c == 0 ? INF : quantile(l, r, c - 1);
    }
};