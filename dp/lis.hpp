#include "template.hpp"

template < class T, class Cmp >
tuple< vector< T >, vector<int>, vector<int> > LIS(const vector< T >& a, Cmp cmp) {
    int n = a.size();
    vector< T > l_s;
    vector<int> st(n, -1), nt(n, -1), rank(n);
    FOR(i, n) {
        int pos = lower_bound(l_s.begin(), l_s.end(), a[i], cmp) - l_s.begin();
        st[pos] = i;
        if(pos >= 1) nt[i] = st[pos - 1];
        if(pos == int(l_s.size())) l_s.push_back(a[i]); else l_s[pos] = a[i];
        rank[i] = pos + 1;
    }
    int len = l_s.size();
    vector<int> idx(len);
    for(int i = st[len - 1]; i >= 0; i = nt[i]) idx[--len] = i;
    return {l_s, idx, rank};
}