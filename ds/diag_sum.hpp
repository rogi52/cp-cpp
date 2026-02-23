#include "template.hpp"

// 2次元で斜め方向の和を取る
// ABC265-F Manhattan Cafe (https://atcoder.jp/contests/abc265/tasks/abc265_f)
template < class Value > struct diag_sum {
    int H, W;
    vector<vector<Value>> s1; // x - y = const.
    vector<vector<Value>> s2; // x + y = const.

    diag_sum(const vector<vector<Value>>& A) : H(ssize(A)), W(ssize(A[0])) {
        s1.assign(H + 1, vector(W + 1, Value(0)));
        s2.assign(H + 1, vector(W + 1, Value(0)));
        FOR(i, H) FOR(j, W) s1[i + 1][j + 1] = A[i][j] + (i > 0 and j > 0 ? s1[i][j] : 0);
        FOR(i, H) REV(j, W) s2[i + 1][j] = A[i][j] + (i > 0 and j + 1 < W ? s2[i][j + 1] : 0);
    }

    // (sx, sy) -- (tx, ty) の総和 (両端含む)
    Value query(int sx, int sy, int tx, int ty) const {
        if(sx - sy == tx - ty) {
            if(sx > tx) swap(sx, tx), swap(sy, ty);
            return get_s1(tx, ty) - get_s1(sx - 1, sy - 1);
        } else if(sx + sy == tx + ty) {
            if(sx > tx) swap(sx, tx), swap(sy, ty);
            return get_s2(tx, ty) - get_s2(sx - 1, sy + 1);
        }
        assert(0);
        return Value(0);
    }

  private:
    Value get_s1(int r, int c) const {
        const int k = r - c;
        const int i = min({r, H - 1, W - 1 + k});
        if(i < 0 or i < k) return 0;
        return s1[i + 1][i - k + 1];
    }
    Value get_s2(int r, int c) const {
        const int k = r + c;
        const int i = min({r, H - 1, k});
        const int i_min = max(0, k - (W - 1));
        if(i < i_min) return 0;
        return s2[i + 1][k - i];
    }
};