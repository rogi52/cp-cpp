#include "template.hpp"
#include "mat/prod.hpp"
#include "mod/modint.hpp"
using mint = modint998;
using matrix = vector<vector<mint>>;

mint det_gauss(matrix A) {
    const int N = ssize(A);
    mint res = 1;
    FOR(i, N) {
        if(A[i][i] == 0) {
            FOR(j, i + 1, N) if(A[j][i] != 0) {
                FOR(k, i, N) swap(A[i][k], A[j][k]);
                res = -res;
                break;
            }
        }
        if(A[i][i] == 0) return 0;
        res *= A[i][i];
        const mint x = mint(1) / A[i][i]; // division
        FOR(k, i, N) A[i][k] *= x;
        FOR(j, i + 1, N) {
            const mint y = A[j][i];
            FOR(k, i, N) A[j][k] -= A[i][k] * y;
        }
    }
    return res;
}

/*
Bird, R. S. (2011). A simple division-free algorithm for computing determinants. 
Information Processing Letters, 111(21-22), 1072-1074.
*/
mint det_bird(matrix A) {
    const int N = ssize(A);
    auto mu = [&](const matrix& X) -> matrix {
        matrix M(N, vector(N, mint(0)));
        FOR(i, N) {
            mint s = 0;
            FOR(k, i + 1, N) s += X[k][k];
            M[i][i] = -s;
            FOR(j, i + 1, N) M[i][j] = X[i][j];
        }
        return M;
    };
    matrix X = A;
    FOR(N - 1) X = mat::prod(mu(X), A);
    mint ans = X[0][0];
    if(N % 2 == 0) ans = -ans;;
    return ans;
}