#include "template.hpp"

/*
LISの長さ: m
idx = {i1, ..., im}
a[i1] < ... < a[im]
*/
template < class T, class Cmp >
pair< vector<int>, vector<int> > LIS(const vector< T >& a, const Cmp& cmp) {
    const int n = ssize(a);
    vector< T > dp;
    vector<int> st(n, -1), nt(n, -1), rank(n);
    FOR(i, n) {
        const int p = lower_bound(dp.begin(), dp.end(), a[i], cmp) - dp.begin();
        st[p] = i;
        if(1 <= p) nt[i] = st[p - 1];
        if(p == ssize(dp)) {
            dp.push_back(a[i]);
        } else {
            dp[p] = a[i];
        }
        rank[i] = p + 1;
    }
    int m = ssize(dp);
    vector<int> idx(m);
    for(int i = st[m - 1]; i >= 0; i = nt[i]) idx[--m] = i;
    return {idx, rank};
}