#include "template.hpp"

struct binary_lifting {
    int N;
    int LG_K;
    vector<vector<int>> doubling;
    binary_lifting(const vector<int>& P, i64 MAX_K) {
        N = ssize(P);
        LG_K = 1;
        while((1LL << LG_K) <= MAX_K) LG_K++;
        doubling.assign(LG_K, vector<int>(N));
        FOR(i, N) doubling[0][i] = P[i];
        FOR(k, LG_K - 1) {
            FOR(i, N) {
                doubling[k + 1][i] = doubling[k][doubling[k][i]];
            }
        }
    }
    int jump(int v, i64 k) {
        FOR(d, LG_K) if(k >> d & 1) v = doubling[d][v];
        return v;
    }
};