#include "template.hpp"

// [LC: Enumerate Quotients]
// https://judge.yosupo.jp/problem/enumerate_quotients


// 区間と同時に列挙
// https://judge.yosupo.jp/submission/364384
// (0, N] を (L, R] に分割 (開閉に注意!)
// f(L, R, Q) := [L, R) → Q
template < class Int, class Func >
void for_each_quotient(const Int N, const Func& F) {
    for(Int L = 0; L < N; ) {
        const Int Q = N / (L + 1);
        const Int R = N / Q;
        F(L, R, Q);
        L = R;
    }
}

// 商のみ列挙
// https://judge.yosupo.jp/submission/364380
vector<u64> enum_quotient(const u64 N) {
    assert(1 <= N and N <= 1'000'000'000'000);
    const u32 n = sqrt(N);
    const u32 f = N / n == n;
    vector<u64> ans(n + n - f);
    int idx = 0;
    for(u32 i = 1; i <= n; i++) ans[idx++] = i;
    for(u32 i = n - f; i >= 1; i--) ans[idx++] = N / i;
    return ans;
}