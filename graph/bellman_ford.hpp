#include "template.hpp"

template < class T > vector< T > bellman_ford(int n, const vector<tuple<int, int, T>>& es, int s) {
    const T INF = numeric_limits< T >::max();
    vector< T > d(n, INF); d[s] = 0;
    FOR(n - 1) {
        for(auto [u, v, c] : es) {
            if(d[u] != INF) chmin(d[v], d[u] + c);
        }
    }
    for(auto [u, v, c] : es) if(d[u] != INF and chmin(d[v], d[u] + c)) return {};
    return d;
}