#include "template.hpp"
#include "graph/scc.hpp"

struct two_sat {
    int n;
    scc_graph scc;
    vector<bool> answer;
    two_sat(int n) : n(n), scc(n + n), answer(n) {}

    void add_clause(int i, bool f, int j, bool g) {
        assert(0 <= i and i < n);
        assert(0 <= j and j < n);
        scc.add_edge(2 * i + (f ? 0 : 1), 2 * j + (g ? 1 : 0));
        scc.add_edge(2 * j + (g ? 0 : 1), 2 * i + (f ? 1 : 0));
    }

    // https://atcoder.jp/contests/abc210/tasks/abc210_f
    void add_variable(int k) {
        assert(0 <= k);
        scc.add_vertex(k + k);
        answer.resize(n + k);
        n += k;
    }
    void at_most_one(const vector<pair<int, bool>>& vs) {
        const int k = vs.size(), o = n;
        add_variable(k);
        const auto y = [&](const int i) { return o + i; };
        for(int i = 0; i < k; i++) {
            auto [x, f] = vs[i];
            add_clause(x, !f, y(i), true);
            if(0 <= i - 1) {
                add_clause(y(i - 1), false, y(i), true);
                add_clause(y(i - 1), false, x, !f);
            }
        }
    }

    bool satisfiable() {
        scc.build();
        for(int i = 0; i < n; i++) {
            if(scc.ids[2 * i] == scc.ids[2 * i + 1]) return false;
            answer[i] = scc.ids[2 * i] < scc.ids[2 * i + 1];
        }
        return true;
    }
};