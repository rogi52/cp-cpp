#include "template.hpp"

template < class T > pair<bool, vector< T >> bellman_ford(int n, const vector<tuple<int, int, T>>& es, int s) {
    const T INF = numeric_limits< T >::max();
    vector< T > d(n, INF); d[s] = 0;
    while(n--) {
        bool updated = false;
        for(const auto &[u, v, c] : es) if(d[u] != INF) {
            if(chmin(d[v], d[u] + c)) updated = true;
        }
        if(not updated) break;
    }
    return pair{n == 0, d};
}