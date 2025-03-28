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
}