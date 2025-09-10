#include "template.hpp"

namespace mat {
template < class T > vector<vector< T >> prod(const vector<vector< T >>& A, const vector<vector< T >>& B) {
    const int N = ssize(A);
    const int M = ssize(A[0]); assert(M == ssize(B));
    const int K = ssize(B[0]);
    vector C(N, vector(K, T(0)));
    FOR(m, M) FOR(n, N) FOR(k, K) C[n][k] += A[n][m] * B[m][k];
    return C;
}
// A^n
template < class T > vector<vector< T >> pow(vector<vector< T >> A, u64 n) {
    const int N = ssize(A);
    vector ans(N, vector(N, T(0)));
    FOR(i, N) ans[i][i] = 1;
    for(; n; n >>= 1) { if(n & 1) ans = prod(ans, A); A = prod(A, A); }
    return ans;
}
}