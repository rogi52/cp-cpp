#include "template.hpp"

vector<int> bfs(const vector<vector<int>>& g, int s) {
    vector<int> d(ssize(g), INF32);
    queue<int> q; d[s] = 0, q.push(s);
    while(not q.empty()) {
        const int u = q.front(); q.pop();
        for(const int v : g[u]) if(d[v] == INF32) d[v] = d[u] + 1, q.push(v);
    }
    return d;
}

vector<int> bfs(const vector<vector<int>>& g, vector<int> ss) {
    vector<int> d(ssize(g), INF32);
    queue<int> q; for(int s : ss) d[s] = 0, q.push(s);
    while(not q.empty()) {
        const int u = q.front(); q.pop();
        for(const int v : g[u]) if(d[v] == INF32) d[v] = d[u] + 1, q.push(v);
    }
    return d;
}

vector<int> shortest_path_bfs(const vector<vector<int>>& g, int s, int t) {
    const int n = ssize(g);
    vector<int> d(n, INF32), p(n, -1);
    queue<int> q;
    d[s] = 0;
    q.push(s);
    while(not q.empty()) {
        const int u = q.front(); q.pop();
        if(u == t) break;
        for(const int v : g[u]) {
            if(d[v] == INF32) {
                d[v] = d[u] + 1;
                p[v] = u;
                q.push(v);
            }
        }
    }
    vector<int> path;
    if(d[t] != INF32) {
        for(int v = t; v != -1; v = p[v]) path.push_back(v);
        reverse(path);
    }
    return move(path);
}

vector<int> bfs01(const vector<vector<pair<int, int>>>& g, int s) {
    vector<int> d(ssize(g), INF32);
    deque<int> q; d[s] = 0, q.push_back(s);
    while(not q.empty()) {
        const int u = q.front(); q.pop_front();
        for(const auto [v, c] : g[u]) {
            if(chmin(d[v], d[u] + c)) {
                if(c == 0) {
                    q.push_front(v);
                } else {
                    q.push_back(v);
                }
            }
        }
    }
    return d;
}

vector<int> bfs01(const vector<vector<pair<int, int>>>& g, vector<int> ss) {
    vector<int> d(ssize(g), INF32);
    deque<int> q; for(int s : ss) d[s] = 0, q.push_back(s);
    while(not q.empty()) {
        const int u = q.front(); q.pop_front();
        for(const auto [v, c] : g[u]) {
            if(chmin(d[v], d[u] + c)) {
                if(c == 0) {
                    q.push_front(v);
                } else {
                    q.push_back(v);
                }
            }
        }
    }
    return d;
}