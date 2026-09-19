#include "template.hpp"

// 凸数列の min-plus 畳み込み, AtCoder Algorithm Lectures
// https://info.atcoder.jp/entry/algorithm_lectures/min_plus_convolution
// 参考: https://judge.yosupo.jp/submission/320371

// Min Plus Convolution (Convex and Convex), Library Checker
// https://judge.yosupo.jp/problem/min_plus_convolution_convex_convex
template < class T > 
vector< T > min_plus_convolution_convex_convex(const vector< T >& A, const vector< T >& B) {
    const int N = ssize(A);
    const int M = ssize(B);
    assert(1 <= N);
    assert(1 <= M);
    vector< T > C(N + M - 1);
    int i = 0, j = 0;
    FOR(k, N + M - 1) {
        C[k] = A[i] + B[j];
        if(j == M - 1 or (i + 1 < N and A[i + 1] + B[j] <= A[i] + B[j + 1])) {
            i += 1;
        } else {
            j += 1;
        }
    }
    return C;
}