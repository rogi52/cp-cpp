#include "template.hpp"

template < class edge_type > struct cycle_detect;
namespace tag {
    struct directed;
    struct undirected;
}

struct cycle_detect<tag::directed> {
    struct edge {
        int from, to, id;
    };
    int n, m;
    vector<vector<edge>> g;
    cycle_detect(int n) : n(n), m(0), g(n) {}
    int add_edge(int from, int to) {
        assert(0 <= from and from < n);
        assert(0 <= to and to < n);
        g[from].push_back(edge{from, to, m});
        return m++;
    }
    vector<edge> detect() {

    }
};