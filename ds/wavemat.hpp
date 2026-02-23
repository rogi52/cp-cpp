#include "template.hpp"
#include "ds/bit_vector.hpp"

// 0 <= Int < 2^LG
// O(N * LG)
// 座圧すれば O(N log N)
template < class Int, int LG > struct wavemat {
    int n;
    vector<int> mid;
    vector<bit_vector> buf;
    const Int INF;
    wavemat(vector<Int> a) : n(ssize(a)), mid(LG), buf(LG, bit_vector(n)), INF(Int(1) << LG) {
        REV(d, LG) {
            vector<Int> n_a;
            n_a.reserve(n);
            FOR(i, n) if((a[i] >> d & 1) == 0) n_a.push_back(a[i]);
            mid[d] = ssize(n_a);
            FOR(i, n) if((a[i] >> d & 1) == 1) n_a.push_back(a[i]), buf[d].set(i);
            buf[d].build();
            a = move(n_a);
        }
    }

    // [l, r) に値 x が何個あるか
    int rank(int l, int r, Int x) const {
        assert(0 <= l and l <= r and r <= n);
        assert(0 <= x and x < INF);
        REV(d, LG) {
            const int f = x >> d & 1;
            l = buf[d].rank(l, f) + (f ? mid[d] : 0);
            r = buf[d].rank(r, f) + (f ? mid[d] : 0);
        }
        return r - l;
    }
    // [l, r) で k 番目 (0-indexed) に小さい値
    Int quantile(int l, int r, int k) const {
        assert(0 <= l and l <= r and r <= n);
        assert(0 <= k and k < r - l);
        Int ans = 0;
        REV(d, LG) {
            const int c0 = buf[d].count(l, r, 0);
            if(k < c0) {
                l = buf[d].rank(l, 0);
                r = buf[d].rank(r, 0);
            } else {
                ans |= Int(1) << d;
                k -= c0;
                l = buf[d].rank(l, 1) + mid[d];
                r = buf[d].rank(r, 1) + mid[d];
            }
        }
        return ans;
    }
    // [l, r) で値が x 未満の要素数
    int freq(int l, int r, Int x) const {
        assert(0 <= l and l <= r and r <= n);
        assert(0 <= x and x <= INF);
        if(x == INF) return r - l;
        int cnt = 0;
        REV(d, LG) {
            const int f = x >> d & 1;
            if(f) cnt += buf[d].count(l, r, 0);
            l = buf[d].rank(l, f) + (f ? mid[d] : 0);
            r = buf[d].rank(r, f) + (f ? mid[d] : 0);
        }
        return cnt;
    }
    // [l, r) で値 x が [lx, rx) に含まれる要素数
    int freq(int l, int r, Int lx, Int rx) const {
        assert(0 <= l and l <= r and r <= n);
        assert(0 <= lx and lx <= rx and rx <= INF);
        return freq(l, r, rx) - freq(l, r, lx);
    }
    // x 未満で最大の要素 (存在しない場合 INF)
    Int prev(int l, int r, Int x) const {
        assert(0 <= l and l <= r and r <= n);
        assert(0 <= x and x <= INF);
        const int c = freq(l, r, x);
        return c != 0 ? quantile(l, r, c - 1) : INF;
    }
    // x 以上で最小の要素 (存在しない場合 INF)
    Int next(int l, int r, Int x) const {
        assert(0 <= l and l <= r and r <= n);
        assert(0 <= x and x <= INF);
        const int c = freq(l, r, x);
        return c != r - l ? quantile(l, r, c) : INF;
    }
};
