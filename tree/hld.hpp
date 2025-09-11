#include "template.hpp"

struct treeHLD {
    int n, root;
    struct edge { int to, i; };
    vector<vector<edge>> g;
    treeHLD(int n) : n(n), g(n), S(n, 0), D(n, 0), L(n, -1), R(n, -1), E(n, -1), id(0), decomped(0) {}
    void add_edge(int u, int v, int i = 0) {
        g[u].push_back({v, i});
        g[v].push_back({u, i});
    }
    void decomp(int r = 0) {
        root = r;
        nxt.assign(n, r);
        par.assign(n, r);
        dfs0(r);
        dfs1(r);
        decomped = true;
    }

    template < class Func > void path_query_comm(int u, int v, bool vertex, const Func& f) {
        assert(decomped);
        const int x = lca(u, v);
        for(auto [a, b] :  ascend(u, x)) tie(a, b) = minmax({a, b}), f(a, b);
        if(vertex) f(L[x], L[x] + 1);
        for(auto [a, b] : descend(x, v)) tie(a, b) = minmax({a, b}), f(a, b);
    }
    template < class Func > void path_query(int u, int v, bool vertex, const Func& f) {
        assert(decomped);
        const int x = lca(u, v);
        for(auto [a, b] :  ascend(u, x)) f(a, b);
        if(vertex) f(L[x], L[x] + 1);
        for(auto [a, b] : descend(x, v)) f(a, b);
    }
    template < class Func > void subtree_query(int v, bool vertex, const Func& f) {
        assert(decomped);
        f(L[v] + !vertex, R[v]);
    }

    int parent(int v) {
        assert(decomped);
        return v == root ? -1 : par[v];
    }
    int la(int v, int d) {
        assert(decomped);
        while(v != -1) {
            const int u = nxt[v];
            if(L[u] <= L[v] - d) return E[L[v] - d];
            d -= L[v] - L[u] + 1;
            v = parent(u);
        }
        return v;
    }
    int lca(int u, int v) {
        assert(decomped);
        for(; nxt[u] != nxt[v]; u = par[nxt[u]]) if(L[u] < L[v]) swap(u, v);
        return D[u] < D[v] ? u : v;
    }
    // 辺の本数
    int dist(int u, int v) {
        assert(decomped);
        return D[u] + D[v] - D[lca(u, v)] * 2;
    }
    int jump(int u, int v, int d) {
        assert(decomped);
        const int D_x = D[lca(u, v)];
        if(d <= D[u] - D_x) return la(u, d);
        d -= D[u] - D_x;
        if(d <= D[v] - D_x) return la(v, D[v] - D_x - d);
        return -1;
    }
    int in_subtree(int r, int v) {
        assert(decomped);
        return L[r] < L[v] and R[v] <= R[r];
    }
    pair<int, int> seq_seg(int v) {
        assert(decomped);
        return {L[v], R[v]};
    }
    int seq_pos(int v) {
        assert(decomped);
        return L[v];
    }

  private:
    int id, decomped;
    vector<int> S, D, L, R, nxt, par, E;
    void dfs0(int v) {
        S[v] = 1;
        for(edge& e : g[v]) {
            if(e.to == par[v]) {
                if(ssize(g[v]) >= 2 and e.to == g[v][0].to) swap(g[v][0], g[v][1]);
                else continue;
            }
            D[e.to] = D[v] + 1;
            par[e.to] = v;
            dfs0(e.to);
            S[v] += S[e.to];
            if(S[e.to] > S[g[v][0].to]) swap(e, g[v][0]);
        }
    }
    void dfs1(int v) {
        L[v] = id++;
        E[L[v]] = v;
        for(edge e : g[v]) if(e.to != par[v]) {
            nxt[e.to] = (e.to == g[v][0].to ? nxt[v] : e.to);
            dfs1(e.to);
        }
        R[v] = id;
    }
    vector<pair<int, int>> ascend(int u, int v) {
        assert(decomped);
        vector<pair<int, int>> res;
        for(; nxt[u] != nxt[v]; u = par[nxt[u]]) res.push_back({L[u] + 1, L[nxt[u]]});
        if(u != v) res.push_back({L[u] + 1, L[v] + 1});
        return res;
    }
    vector<pair<int, int>> descend(int u, int v) {
        assert(decomped);
        if(u == v) return {};
        if(nxt[u] == nxt[v]) return {{L[u] + 1, L[v] + 1}};
        vector<pair<int, int>> res = descend(u, par[nxt[v]]);
        res.push_back({L[nxt[v]], L[v] + 1});
        return res;
    }

};