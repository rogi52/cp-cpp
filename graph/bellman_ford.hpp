#include "template.hpp"

// edge = {{from, to}, cost}
// return {sから到達できる負閉路がある?, dist}
template < class T > 
pair<bool, vector< T >> bellman_ford(int n, const vector<pair<pair<int, int>, i64>>& es, int s) {
    const T INF = numeric_limits< T >::max();
    vector< T > dist(n, INF); dist[s] = 0;
    int cnt = 0;
    while(cnt < n) {
        bool upd = false;
        for(const auto [uv, c] : es) {
            auto [u, v] = uv;
            if(dist[u] != INF and chmin(dist[v], dist[u] + c)) upd = true;
        }
        if(not upd) break;
        cnt += 1;
    }
    return {cnt == n, dist};
}