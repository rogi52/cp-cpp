#include "template.hpp"
#include "mod/binom.hpp"

// 参考: https://ferin-tech.hatenablog.com/entry/2019/08/11/ラグランジュ補間
// https://atcoder.jp/contests/arc033/submissions/39711101

// f(i) = y[i] (i = 0, 1, ..., n)
// f(T) を求める. O(n). 
template < class mint >
mint interpolate_point(const vector<mint>& y, const i64 T) {
    const int n = ssize(y) - 1;
    if(0 <= T and T <= n) return y[T];

    vector<mint> L(n + 1), R(n + 1);
    L[0] = R[n] = 1;
    for(int i = 0; i < n; i++) L[i + 1] = L[i] * (T - i);
    for(int i = n; i > 0; i--) R[i - 1] = R[i] * (T - i);
    mint ans = 0;
    for(int i = 0; i <= n; i++) {
        ans += y[i] * L[i] * R[i] * comb::fact_inv<mint>(i) * comb::fact_inv<mint>(n - i) * parity_sign(n - i);
    }
    return ans;
}