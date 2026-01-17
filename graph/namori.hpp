#include "template.hpp"

// 参考: https://ei1333.github.io/algorithm/namori.html
struct namori_graph {
    int n;
    vector<vector<int>> g;
    vector<vector<int>> forest;
    vector<int> cycle;
    namori_graph(int n) : n(n), g(n), forest(n) {}
    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    void build() {
        vector<int> deg(n);
        FOR(v, n) deg[v] = ssize(g[v]);
        queue<int> Q;
        vector<int> vis(n, 0);
        FOR(v, n) if(deg[v] == 1) {
            Q.push(v);
            vis[v] = 1;
        }
        while(not Q.empty()) {
            const int v = Q.front(); Q.pop();
            for(int to : g[v]) if(not vis[to]) {
                forest[v].push_back(to);
                forest[to].push_back(v);
                if(--deg[to] == 1) {
                    Q.push(to);
                    vis[to] = 1;
                }
            }
        }
        auto dfs = [&](auto dfs, int v) -> void {
            cycle.push_back(v);
            for(int to : g[v]) if(not vis[to]) {
                vis[to] = 1;
                dfs(dfs, to);
            }
        };
        FOR(v, n) if(not vis[v]) {
            vis[v] = 1;
            dfs(dfs, v);
            break;
        }
    }
};