#include "template.hpp"
#include "flow/maxflow.hpp"

template < class Cost > struct sm_min {
    int n, s, t;
    mf_graph<Cost> g;
    Cost c0;
    vector<array<Cost, 2>> c1;
    vector<map<int, array<array<Cost, 2>, 2>>> c2;

    sm_min(int n) : n(n), s(n), t(n + 1), g(n + 2), c0(0), c1(n), c2(n) {}
    void add0(Cost cost) { c0 += cost; }
    void add1(int i, bool f, Cost cost) {
        assert(0 <= i and i < n);
        c1[i][f] += cost;
    }
    void add2(int i, bool f, int j, bool g, Cost cost) {
        assert(0 <= i and i < n);
        assert(0 <= j and j < n);
        c2[i][j][f][g] += cost;
    }
    Cost ans() {
        FOR(i, n) for(const auto &[j, c] : c2) {
            assert(c[0][0] + c[1][1] <= c[0][1] + c[1][0]);
            add0(c[0][0]);
            add1(i, 1, c[1][1] - c[0][1]);
            add2(j, 1, c[0][1] - c[0][0]);
            g.add_edge(i, j, c[0][1] + c[1][0] - c[0][0] - c[1][1]);
        }
        FOR(i, n) {
            auto& c = c1[i];
            if(c[1] <= c[0]) {
                add0(c[1]);
                g.add_edge(s, i, c[0] - c[1]);
            } else {
                add0(c[0]);
                g.add_edge(i, t, c[1] - c[0]);
            }
        }
        return c0 + g.flow(s, t);
    }
};

template < class Cost, Cost INF > struct k_sm_min {
    int n;
    vector<int> k;
    vector<vector<int>> id;
    Cost c0;
    vector<vector<Cost>> c1;
    vector<map<int, vector<vector<Cost>>>> c2;
    sm_min<Cost> sm;
    k_sm_min(int n, vector<int> k) : n(n), k(k), id(n), c0(0), c1(n), c2(n) {
        int m = 0;
        FOR(i, n) {
            id[i].resize(k[i] - 1);
            FOR(f, k[i] - 1) id[i][f] = m++;
        }
        sm = sm_min<Cost>(m);
        FOR(i, n) c1[i].resize(k[i], 0);
    }
    void add0(Cost cost) { c0 += cost; }
    void add1(int i, int f, Cost cost) {
        assert(0 <= i and i < n); assert(0 <= f and f < k[i]);
        c1[i][f] += cost;
    }
    void add2(int i, int f, int j, int g, Cost cost) {
        assert(0 <= i and i < n); assert(0 <= f and f < k[i]);
        assert(0 <= j and j < n); assert(0 <= g and g < k[j]);
        if(not c2[i].contains(j)) c2[i][j] = vector(k[i], vector(k[j], Cost(0)));
        c2[i][j][f][g] += cost;
    }
    Cost ans() {
        FOR(i, n) for(auto [j, c] : c2[i]) {
            FOR(f, k[i]) {
                const Cost d = c[f][0];
                add1(i, f, d);
                FOR(g, k[j]) c[f][g] -= d;
            }
            FOR(g, k[j]) {
                const Cost d = c[0][g];
                add1(j, g, d);
                FOR(f, k[i]) c[f][g] -= d;
            }
            FOR(f, k[i] - 1) FOR(g, k[j] - 1)
                sm.add2(id[i][f], 0, id[j][g], 0, c[f + 1][g + 1] - c[f + 1][g] - c[f][g + 1] + c[f][g]);
        }
        FOR(i, n) {
            auto& c = c1[i];
            FOR(f, 1, k[i] - 1) sm.add2(id[i][f - 1], 1, id[i][f], -, INF);
            FOR(f, 1, k[i]) sm.add1(id[i][f - 1], 1, c[f - 1] - c[f]);
            add0(c[k[i] - 1]);
        }
        sm.add0(c0);
        return sm.ans();
    }
};