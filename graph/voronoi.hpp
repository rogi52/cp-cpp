#include "template.hpp"

// Code Festival 2017 J (Tree MST) と 距離空間上のボロノイ図
// https://tokoharuland.hateblo.jp/entry/2018/04/01/155743

// ABC250 - Ex
// https://atcoder.jp/contests/abc250/tasks/abc250_h
// https://atcoder.jp/contests/abc250/submissions/70711826

// KEYENCE Programming Contest 2019 - E
// https://atcoder.jp/contests/keyence2019/tasks/keyence2019_e
// https://atcoder.jp/contests/keyence2019/submissions/70712047

// CODE FESTIVAL 2017 Final - J
// https://atcoder.jp/contests/cf17-final/tasks/cf17_final_j
// https://atcoder.jp/contests/cf17-final/submissions/70712294

// JOI春合宿2014 - E
// https://atcoder.jp/contests/joisc2014/tasks/joisc2014_e
// https://atcoder.jp/contests/joisc2014/submissions/70713298

// (u, v, w)
template < class Dist >
vector<tuple<int, int, Dist>> voronoi_dijkstra(const vector<vector<pair<int, Dist>>>& G, const vector<int>& S) {
    const int N = ssize(G);
    heap_min<tuple<Dist, int, int>> que; // (d, v, s)
    vector dist(N, pair{infty<Dist>, N});
    for(const int v : S) {
        const Dist d = 0;
        const int s = v;
        dist[v] = {d, s};
        que.push({d, v, s});
    }
    vector vis(N, false);
    vector<tuple<int, int, Dist>> E;
    while(not que.empty()) {
        const auto [d, v, s] = que.top(); que.pop();
        if(vis[v]) continue;
        vis[v] = true;
        for(auto [to, c] : G[v]) {
            if(vis[to]) {
                const auto [e, t] = dist[to];
                if(s != t) E.push_back({t, s, e + c + d});
            } else {
                const Dist nd = d + c;
                if(dist[to].first > nd) {
                    dist[to] = {nd, s};
                    que.push({nd, to, s});
                }
            }
        }
    }
    return E;
}

// (u, v, w)
vector<tuple<int, int, int>> voronoi_bfs(const vector<vector<int>>& G, const vector<int>& S) {
    const int N = ssize(G);
    queue<pair<int, int>> que; // (v, s)
    vector dist(N, pair{infty<int>, N});
    for(const int v : S) {
        const int d = 0;
        const int s = v;
        dist[v] = {d, s};
        que.push({v, s});
    }
    vector vis(N, false);
    vector<tuple<int, int, int>> E;
    while(not que.empty()) {
        const auto [v, s] = que.front(); que.pop();
        const int d = dist[v].first;
        if(vis[v]) continue;
        vis[v] = true;
        for(const int to : G[v]) {
            if(vis[to]) {
                const auto [e, t] = dist[to];
                if(s != t) E.push_back({t, s, e + 1 + d});
            } else {
                const int nd = d + 1;
                if(dist[to].first > nd) {
                    dist[to] = {nd, s};
                    que.push({to, s});
                }
            }
        }
    }
    return E;
}