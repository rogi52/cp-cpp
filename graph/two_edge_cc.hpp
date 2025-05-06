#include "template.hpp"

// lowlink を計算して橋と関節点を求める
struct lowlink {
    int n;
    vector<vector<int>> g;
    vector<int> ord; // ord[v] := DFS で v を訪れた順番 (p -> v => ord[p] < ord[v])
    vector<int> low; // low[v] := (DFS-tree の辺)^* (後退辺) (DFS-tree の辺) で辿り着ける頂点 x について min_x ord[x]
    vector<pair<int, int>> bridge; // 橋
    vector<int> art; // 関節点
    vector<int> vis; // 計算用
    lowlink(int n) : n(n), g(n), ord(n, 0), low(n, 0), vis(n, 0) {}
    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    virtual void build() {
        int t = 0;
        auto dfs = [&](auto&& dfs, int u, int p) -> void {
            vis[u] = 1;
            ord[u] = low[u] = t++;
            int pu = 0, is_art = 0, ch = 0;
            for(int v : g[u]) {
                if(v == p and not pu) { pu = 1; continue; }
                if(not vis[v]) {
                    ch++;
                    dfs(dfs, v, u);
                    chmin(low[u], low[v]); // u から v へ下って, low[v] へ到達できる
                    if(ord[u] < low[v]) bridge.push_back({u, v}); // v から後退辺を使って u に到達できない (ord[u] = low[v] のときは橋でない)
                    is_art |= (p != -1 and ord[u] <= low[v]); // u が根でないとき, v から u より上に到達できない (ord[u] = low[v] のときも関節点)
                } else {
                    chmin(low[u], ord[v]); // u から v へ後退辺を使って到達できる
                }
            }
            is_art |= (p == -1 and 2 <= ch); // u が根のとき, 子が 2 つ以上あるとき関節点
            if(is_art) art.push_back(u);
        };
        for(int v = 0; v < n; v++) if(not vis[v]) dfs(dfs, v, -1);
    }
};

struct two_edge_connected_components : public lowlink {
    int sz; // 2辺連結成分の個数
    vector<int> idx; // idx[v] := v が属する2辺連結成分の番号
    vector<vector<int>> group; // group[i] := i 番目の2辺連結成分に属する頂点集合. v in group[idx[v]]
    vector<vector<int>> forest; // (普通の)連結成分ごとに木になる
    two_edge_connected_components(int n) : lowlink(n), sz(0), idx(n, -1) {}
    void build() override {
        lowlink::build();
        auto dfs = [&](auto&& dfs, int u, int p) -> void {
            idx[u] = (p == -1 or ord[p] < low[u] ? sz++ : idx[p]);
            for(int v : g[u]) if(idx[v] == -1) dfs(dfs, v, u);
        };
        for(int v = 0; v < n; v++) if(idx[v] == -1) dfs(dfs, v, -1);

        group.resize(sz);
        for(int v = 0; v < n; v++) group[idx[v]].push_back(v);

        forest.resize(sz);
        for(auto [u, v] : bridge) {
            u = idx[u], v = idx[v];
            forest[u].push_back(v);
            forest[v].push_back(u);
        }
    }
};