#pragma once
#include "template.hpp"

struct scc_graph {
    int n;
    vector<vector<int>> g;

    int sz; // # of scc
    vector<int> ids; // v in scc[ids[v]]
    vector<vector<int>> scc;
    vector<vector<int>> dag;

    scc_graph(int n) : n(n), g(n), sz(0), ids(n) {}
    void add_edge(int from, int to) {
        assert(0 <= from and from < n);
        assert(0 <= to and to < n);
        g[from].push_back(to);
    }
    void add_vertex(const int k) {
        assert(0 <= k);
        n += k;
        g.resize(n);
        ids.resize(n);
    }

    void build() {
        int now_ord = 0;
        vector<int> vis, low(n), ord(n, -1);
        vis.reserve(n);
        auto dfs = [&](auto self, int v) -> void {
            low[v] = ord[v] = now_ord++;
            vis.push_back(v);
            for(int to : g[v]) {
                if(ord[to] == -1) {
                    self(self, to);
                    low[v] = min(low[v], low[to]);
                } else {
                    low[v] = min(low[v], ord[to]);
                }
            }
            if(low[v] == ord[v]) {
                while(true) {
                    int u = vis.back(); vis.pop_back();
                    ord[u] = n;
                    ids[u] = sz;
                    if(u == v) break;
                }
                sz++;
            }
        };
        for(int i = 0; i < n; i++) if(ord[i] == -1) dfs(dfs, i);
        for(int& x : ids) x = sz - 1 - x;

        scc.resize(sz);
        for(int i = 0; i < n; i++) scc[ids[i]].push_back(i);

        dag.resize(sz);
        for(int from = 0; from < n; from++) {
            for(int to : g[from]) {
                int from_id = ids[from], to_id = ids[to];
                if(from_id != to_id) dag[from_id].push_back(to_id);
            }
        }
    }
};