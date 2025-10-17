#include "template.hpp"

namespace util {

// [0, N) の順列か判定する
bool is_perm0(const int N, const vector<int> P) {
    if(ssize(P) != N) return false;
    vector<int> A(N, 0);
    FOR(i, N) {
        if(0 <= P[i] and P[i] < N) {
            if(A[P[i]] == 1) return false;
            A[P[i]] = 1;
        } else {
            return false;
        }
    }
    return true;
}

// [1, N] の順列か判定する
bool is_perm1(const int N, const vector<int> P) {
    if(ssize(P) != N) return false;
    vector<int> A(N + 1, 0);
    FOR(i, N) {
        if(1 <= P[i] and P[i] <= N) {
            if(A[P[i]] == 1) return false;
            A[P[i]] = 1;
        } else {
            return false;
        }
    }
    return true;
}
}