#include "template.hpp"

template < class F > void for_each_clique(const vector<vector<int>>& g, const F& f) {
    const int N = ssize(g);
    vector<int> deg(N, 0), S(N, 1);
    FOR(i, N) FOR(j, N) if(i < j and g[i][j]) deg[i]++, deg[j]++;

    FOR(N) {
        int v = -1, min_deg = N;
        FOR(i, N) if(S[i] and chmin(min_deg, deg[i])) v = i;
        vector<int> c = {v}, to;
        FOR(i, N) if(S[i] and g[v][i]) to.push_back(i);

        auto dfs = [&](auto dfs, int i) -> void {
            f(c);
            FOR(k, i, ssize(to)) {
                int ok = 1;
                for(int x : c) if(not g[x][to[k]]) { ok = 0; break; }
                if(ok) {
                    c.push_back(to[k]);
                    dfs(dfs, k + 1);
                    c.pop_back();
                }
            }
        }; dfs(dfs, 0);
        
        S[v] = 0;
        for(int i : to) deg[i]--;
    }
}