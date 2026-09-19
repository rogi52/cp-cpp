#include "template.hpp"

// N 以下
template < class T > vector<vector< T >> binom_table(int N) {
    vector C(N + 1, vector(N + 1, T(0)));
    FOR(i, N + 1) FOR(j, i + 1) {
        if(j == 0 or j == i) {
            C[i][j] = 1;
        } else {
            C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
        }
    }
    return C;
}