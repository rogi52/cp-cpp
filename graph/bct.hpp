#include "template.hpp"

// https://twitter.com/noshi91/status/1529858538650374144?s=20&t=eznpFbuD9BDhfTb4PplFUg
struct block_cut_tree {
    int n;
    vector<vector<int>> g;
    int sz; // 2点連結成分の個数
    vector<int> ord, low, vis, vs;

    // [0, n) => 元の頂点
    // [n, n + sz) => 2点連結成分に対応する頂点
    // forest[b] (b \in [n, n+sz)) = 2点連結成分内の頂点集合
    // 全体は連結成分ごとに木になる
    vector<vector<int>> forest;
    block_cut_tree(int n) : n(n), g(n), sz(0), forest(n), ord(n, 0), low(n, 0), vis(n, 0) { vs.reserve(n); }
    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    void build() {
        int b = n, t = 0;
        auto dfs = [&](auto&& dfs, int u, int p) -> void {
            vis[u] = 1;
            vs.push_back(u);
            ord[u] = low[u] = t++;
            int ch = 0;
            for(int v : g[u]) if(v != p) {
                if(not vis[v]) {
                    const int s = ssize(vs);
                    ch++;
                    dfs(dfs, v, u);
                    chmin(low[u], low[v]);
                    if(p == -1 and 2 <= ch or p != -1 and ord[u] <= low[v]) {
                        forest.push_back({});
                        forest[b].push_back(u);
                        forest[u].push_back(b);
                        while(s < ssize(vs)) {
                            const int x = vs.back(); vs.pop_back();
                            forest[b].push_back(x);
                            forest[x].push_back(b);
                        }
                        b++;
                    }
                } else {
                    chmin(low[u], ord[v]);
                }
            }
        };
        for(int v = 0; v < n; v++) if(not vis[v]) {
            dfs(dfs, v, -1);
            forest.push_back({});
            while(not vs.empty()) {
                const int x = vs.back(); vs.pop_back();
                forest[b].push_back(x);
                forest[x].push_back(b);
            }
            b++;
        }
        sz = b - n;
    }
    bool is_art(int v) {
        return 0 <= v and v < n and 2 <= ssize(forest[v]);
    }
};