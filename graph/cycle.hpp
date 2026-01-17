#include "template.hpp"

// https://judge.yosupo.jp/problem/cycle_detection
struct cycle_detect_directed {
    int n;
    vector<vector<pair<int, int>>> g; // v -> {to, e_id}
    cycle_detect_directed(int n) : n(n), g(n) {}
    void add_edge(int u, int v, int i) {
        g[u].push_back({v, i});
    }
    vector<int> detect() {
        vector<int> used(n, 0);
        vector<pair<int, int>> prev(n);
        vector<int> ans;
        auto dfs = [&](auto dfs, int v) -> bool {
            used[v] = 1;
            for(auto [to, e_id] : g[v]) {
                if(used[to] == 0) {
                    prev[to] = {v, e_id};
                    if(dfs(dfs, to)) return true;
                } else if(used[to] == 1) {
                    ans = {e_id};
                    for(int x = v; x != to; x = prev[x].first) ans.push_back(prev[x].second);
                    reverse(ans);
                    return true;
                }
            }
            used[v] = 2;
            return false;
        };
        FOR(v, n) if(used[v] == 0) {
            if(dfs(dfs, v)) return ans;
        }
        return {};
    }
};