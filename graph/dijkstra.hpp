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

template < class T > vector< T > dijkstra(const vector<vector<pair<int, T>>>& g, vector<int> ss) {
    const T INF = numeric_limits< T >::max();
    vector< T > d(ssize(g), INF);
    heap_min<pair< T, int>> q;
    for(int s : ss) q.push({d[s] = T(0), s});
    while(not q.empty()) {
        auto [uc, ui] = q.top(); q.pop();
        if(uc != d[ui]) continue;
        for(auto [vi, vc] : g[ui]) if(chmin(d[vi], uc + vc)) q.push({d[vi], vi});
    }
    return d;
}

template < class T > vector< T > dijkstra_dense(const vector<vector< T >>& a) {
    const T INF = numeric_limits< T >::max();
    const int n = ssize(a);
    vector< T > d(n, INF);
    vector<int> check(n, 0);
    FOR(n) {
        pair< T, int > min = {INF, INF32};
        FOR(v, n) if(not check[v]) chmin(min, pair{d[v], v});
        const auto [du, u] = min;
        FOR(v, n) if(not check[v]) chmin(d[v], du + a[u][v]);
    }
    return d;
}