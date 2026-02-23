#include "template.hpp"

// ABC236-G (https://atcoder.jp/contests/abc236/tasks/abc236_g)
namespace mat {
template < class Semiring >
vector<vector<typename Semiring::value_type>> prod(const vector<vector<typename Semiring::value_type>>& A, const vector<vector<typename Semiring::value_type>>& B) {
    const int N = ssize(A);
    const int M = ssize(A[0]); assert(M == ssize(B));
    const int K = ssize(B[0]);
    vector C(N, vector(K, Semiring::add_e()));
    FOR(m, M) FOR(n, N) FOR(k, K) C[n][k] = Semiring::add(C[n][k], Semiring::mul(A[n][m], B[m][k]));
    return C;
}
template < class Semiring >
vector<vector<typename Semiring::value_type>> pow(vector<vector<typename Semiring::value_type>> A, u64 n) {
    const int N = ssize(A);
    vector ans(N, vector(N, Semiring::add_e()));
    FOR(i, N) ans[i][i] = Semiring::mul_e();
    for(; n; n >>= 1) { if(n & 1) ans = prod<Semiring>(ans, A); A = prod<Semiring>(A, A); }
    return ans;
}
}