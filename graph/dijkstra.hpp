#include "template.hpp"

template < class T > vector< T > dijkstra(const vector<vector<pair<int, T>>>& g, int s) {
    const T INF = numeric_limits< T >::max();
    vector< T > d(ssize(g), INF);
    heap_min<pair< T, int>> q; q.push({d[s] = T(0), s});
    while(not q.empty()) {
        auto [uc, ui] = q.top(); q.pop();
        if(uc != d[ui]) continue;
        for(auto [vi, vc] : g[ui]) if(chmin(d[vi], uc + vc)) q.push({d[vi], vi});
    }
    return d;
}