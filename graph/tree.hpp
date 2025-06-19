#include "template.hpp"

template < class T > struct tree_graph {
    int n;
    struct edge { int from, to, id; T cost; };
    std::vector<std::vector< edge >> g, g_org;
    tree_graph(int n) : n(n), g(n), g_org(n) {}

    void add_edge(int u, int v, int i = 0, T c = 1) {
        g[u].push_back(edge{u, v, i, c});
        g[v].push_back(edge{v, u, i, c});
        g_org[u].push_back(edge{u, v, i, c});
        g_org[v].push_back(edge{v, u, i, c});
    }

    std::pair< std::pair<int,int>, T > diameter() {
        std::vector< T > dist(n);
        std::function<void(int,int)> dfs = [&](int v, int p) -> void {
            for(auto [to, c] : g[v]) if(to != p) {
                dist[to] = dist[v] + c;
                dfs(to, v);
            }
        };
        dist[0] = 0; dfs(0, -1);
        int u = max_of(dist).key;
        dist[u] = 0; dfs(u, -1);
        auto [v, d] = max_of(dist);
        return {{u, v}, d};
    }

    std::vector<int> path(int u, int v) {
        std::vector<int> parent(n, -1);
        std::function<void(int,int)> dfs = [&](int v, int p) -> void {
            parent[v] = p;
            for(auto e : g[v]) if(e.to != p) dfs(e.to, v);
        };
        dfs(v, -1);
        std::vector<int> res;
        while(u != -1) res.push_back(u), u = parent[u];
        return res;
    }

    int id, root, heavy_light_decomposed;
    std::vector<int> size, depth, down, up, nxt, par, tour;

    void heavy_light_decomposition(int root = 0) {
        id = 0;
        this->root = root;
        size .assign(n, 0);
        depth.assign(n, 0);
        down.assign(n, -1);
        up  .assign(n, -1);
        tour.assign(n, -1);
        nxt.assign(n, root);
        par.assign(n, root);
        dfs_size(root);
        dfs_hld(root);
        heavy_light_decomposed = 1;
    }

    void dfs_size(int v) {
        size[v] = 1;
        for(auto& e : g[v]) {
            if(e.to == par[v]) {
                if(int(g[v].size()) >= 2 and e.to == g[v][0].to) {
                    std::swap(g[v][0], g[v][1]);
                } else continue;
            }
            depth[e.to] = depth[v] + 1;
            par[e.to] = v;
            dfs_size(e.to);
            size[v] += size[e.to];
            if(size[e.to] > size[g[v][0].to]) std::swap(e, g[v][0]);
        }
    }
    void dfs_hld(int v) {
        down[v] = id++;
        tour[down[v]] = v;
        for(auto e : g[v]) if(e.to != par[v]) {
            nxt[e.to] = (e.to == g[v][0].to ? nxt[v] : e.to);
            dfs_hld(e.to);
        }
        up[v] = id;
    }

    std::vector<std::pair<int,int>> ascend(int u, int v) {
        assert(heavy_light_decomposed);
        std::vector<std::pair<int,int>> res;
        while(nxt[u] != nxt[v]) res.push_back({down[u] + 1, down[nxt[u]]}), u = par[nxt[u]];
        if(u != v) res.push_back({down[u] + 1, down[v] + 1});
        return res;
    }
    std::vector<std::pair<int,int>> descend(int u, int v) {
        assert(heavy_light_decomposed);
        if(u == v) return {};
        if(nxt[u] == nxt[v]) return {{down[u] + 1, down[v] + 1}};
        std::vector<std::pair<int,int>> res = descend(u, par[nxt[v]]);
        res.push_back({down[nxt[v]], down[v] + 1});
        return res;
    }
    std::pair<int,int> idx(int v) { 
        assert(heavy_light_decomposed);
        return {down[v], up[v]};
    }

    template < class func >
    void path_query_comm(int u, int v, bool vertex, const func& f) {
        assert(heavy_light_decomposed);
        int x = lca(u, v);
        for(auto [a, b] : ascend(u, x)) {
            std::tie(a, b) = std::minmax({a, b});
            f(a, b);
        }
        if(vertex) f(down[x], down[x] + 1);
        for(auto [a, b] : descend(x, v)) {
            std::tie(a, b) = std::minmax({a, b});
            f(a, b);
        }
    }
    template < class func >
    void path_query(int u, int v, bool vertex, const func& f) {
        assert(heavy_light_decomposed);
        int x = lca(u, v);
        for(auto [a, b] : ascend(u, x)) f(a, b);
        if(vertex) f(down[x], down[x] + 1);
        for(auto [a, b] : descend(x, v)) f(a, b);
    }
    template < class func >
    void subtree_query(int v, bool vertex, const func& f) {
        assert(heavy_light_decomposed);
        f(down[v] + !vertex, up[v]);
    }
    int parent(int v) {
        assert(heavy_light_decomposed);
        return v == root ? -1 : par[v];
    }
    int la(int v, int d) {
        assert(heavy_light_decomposed);
        while(v != -1) {
            int u = nxt[v];
            if(down[v] - d >= down[u]) return tour[down[v] - d];
            d -= down[v] - down[u] + 1;
            v = parent(u);
        }
        return v;
    }
    int lca(int u, int v) {
        assert(heavy_light_decomposed);
        while(nxt[u] != nxt[v]) {
            if(down[u] < down[v]) std::swap(u, v);
            u = par[nxt[u]];
        }
        return depth[u] < depth[v] ? u : v;
    }
    int dist(int u, int v) {
        assert(heavy_light_decomposed);
        return depth[u] + depth[v] - depth[lca(u, v)] * 2;
    }
    int jump(int u, int v, int d) {
        assert(heavy_light_decomposed);
        int x = lca(u, v);
        if(d <= depth[u] - depth[x]) return la(u, d);
        d -= depth[u] - depth[x];
        if(d <= depth[v] - depth[x]) return la(v, depth[v] - depth[x] - d);
        return -1;
    }
    int in_subtree(int r, int v) {
        return down[r] < down[v] and up[v] <= up[r];
    }
};