#include "template.hpp"

struct cc_graph {
    int n;
    vector<vector<int>> g;
    vector<pair<int, int>> es;
    cc_graph(int n) : n(n), g(n) {}
    int add_edge(int u, int v)
};