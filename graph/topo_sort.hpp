#include "template.hpp"

// 参考: https://algo-logic.info/topological-sort/
// graph g is DAG <-> ssize(topo) == n
vector<int> topo_sort(const vector<vector<int>>& g) {
    const int n = ssize(g);
    vector<int> deg(n, 0), topo;
    queue<int> que;
    FOR(v, n) for(int to : g[v]) deg[to] += 1;
    FOR(v, n) if(deg[v] == 0) que.push(v);
    while(not que.empty()) {
        const int v = que.front(); que.pop();
        topo.push_back(v);
        for(int to : g[v]) if(--deg[to] == 0) que.push(to);
    }
    return topo;
}