#include "template.hpp"

template < class Cap > struct mf_graph {
    mf_graph() {}
    mf_graph(int n) : n(n), g(n) {}
    int add_edge(int from, int to, Cap cap) {
        assert(0 <= from and from < n);
        assert(0 <= to and to < n);
        assert(0 <= cap);
        const int m = ssize(pos);
        pos.push_back({from, ssize(g[from])});
        const int from_id = ssize(g[from]);
        const int to_id = ssize(g[to]) + (from == to);
        g[from].push_back(_edge{to, to_id, cap});
        g[to].push_back(_edge{from, from_id, 0});
        return m;
    }
    
    struct edge { int from, to; Cap cap, flow; };
    edge get_edge(int i) {
        assert(0 <= i and i < ssize(pos));
        _edge e = g[pos[i].first][pos[i].second];
        _edge r = g[e.to][e.rev];
        return edge{pos[i].first, e.to, e.cap + r.cap, r.cap};
    }
    vector<edge> edges() {
        vector<edge> res(ssize(pos));
        FOR(i, ssize(pos)) res[i] = get_edge(i);
        return res;
    }
    void change_edge(int i, Cap cap, Cap flow) {
        assert(0 <= i and i < ssize(pos));
        assert(0 <= flow and flow <= cap);
        _edge& e = g[pos[i].first][pos[i].second];
        _edge& r = g[e.to][e.rev];
        e.cap = cap - flow;
        r.cap = flow;
    }

    Cap flow(int s, int t, Cap flow_limit) {
        assert(0 <= s and s < n);
        assert(0 <= t and t < n);
        assert(s != t);

        vector<int> level(n), iter(n);
        auto bfs = [&]() {
            fill(level.begin(), level.end(), -1);
            level[s] = 0;
            queue<int> q;
            q.push(s);
            while(not q.empty()) {
                int v = q.front(); q.pop();
                for(_edge e : g[v]) {
                    if(e.cap == 0 or 0 <= level[e.to]) continue;
                    level[e.to] = level[v] + 1;
                    if(e.to == t) return;
                    q.push(e.to);
                }
            }
        };

        auto dfs = [&](auto self, int v, Cap up) {
            if(v == s) return up;
            Cap res = 0;
            int level_v = level[v];
            for(int& i = iter[v]; i < ssize(g[v]); i++) {
                _edge& e = g[v][i];
                if(level_v <= level[e.to] or g[e.to][e.rev].cap == 0) continue;
                Cap d = self(self, e.to, min(up - res, g[e.to][e.rev].cap));
                if(d <= 0) continue;
                g[v][i].cap += d;
                g[e.to][e.rev].cap -= d;
                res += d;
                if(res == up) return res;
            }
            level[v] = n;
            return res;
        };

        Cap flow = 0;
        while(flow < flow_limit) {
            bfs();
            if(level[t] == -1) break;
            fill(iter.begin(), iter.end(), 0);
            Cap f = dfs(dfs, t, flow_limit - flow);
            if(f == 0) break;
            flow += f;
        }
        return flow;
    }
    Cap flow(int s, int t) {
        return flow(s, t, numeric_limits<Cap>::max());
    }
    vector<int> min_cut(int s) {
        vector<int> visited(n, 0);
        queue<int> q;
        q.push(s);
        while(not q.empty()) {
            int p = q.front(); q.pop();
            visited[p] = 1;
            for(_edge e : g[p]) {
                if(e.cap && not visited[e.to]) {
                    visited[e.to] = 1;
                    q.push(e.to);
                }
            }
        }
        return visited;
    }

  private:
    int n;
    struct _edge { int to, rev; Cap cap; };
    vector<pair<int, int>> pos;
    vector<vector<_edge>> g;
};