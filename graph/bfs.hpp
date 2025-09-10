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