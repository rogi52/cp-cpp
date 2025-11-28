#include "template.hpp"

template < class Value > struct rollback_var {
    Value x;
    vector<Value> history;
    int t;
    rollback_var(Value e) : x(e), t(0) {}
    Value set(Value v) {
        history.push_back(x);
        return x = v;
    }
    Value get() {
        return x;
    }
    void undo() {
        x = history.back();
        history.pop_back();
    }
    void snapshot() {
        t = ssize(history);
    }
    void rollback() {
        while(t < ssize(history)) undo();
    }
};

template < class Value > struct rollback_array {
    vector<Value> a;
    vector<pair<int, Value>> history;
    int t;
    rollback_array(int n) : a(n), t(0) {}
    rollback_array(int n, Value e) : a(n, e), t(0) {}
    Value set(int i, Value v) {
        history.push_back({i, a[i]});
        return a[i] = v;
    }
    Value get(int i) {
        return a[i];
    }
    void undo() {
        auto [i, v] = history.back(); history.pop_back();
        a[i] = v;
    }
    void snapshot() {
        t = ssize(history);
    }
    void rollback() {
        while(t < ssize(history)) undo();
    }
};



// TODO
/*
https://snuke.hatenablog.com/entry/2016/07/01/000000
https://ei1333.github.io/luzhiled/snippets/other/mo.html
https://ei1333.github.io/luzhiled/snippets/other/mo.html
https://maspypy.github.io/library/ds/offline_query/rollback_mo.hpp
https://github.com/rogi52/cp-library/blob/main/src/data_structure/rollback_array.hpp
https://github.com/rogi52/cp-library/blob/main/src/data_structure/rollback_union_find.hpp
https://maspypy.github.io/library/ds/unionfind/rollback_unionfind.hpp
https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum
https://github.com/rogi52/cp-library/blob/main/src/algorithm/offline_dynamic_data_structure.hpp
*/

struct rollback_unionfind {
    rollback_var<int> num_c;
    rollback_array<int> data;
    static constexpr int SKIP = -1;
    int time_num_c;
    int time_data;
    rollback_unionfind(int n) : num_c(n), data(n, -1), time_num_c(0), time_data(0) {}

    int unite(int x, int y) {
        x = root(x), y = root(y);
        if(x == y) return SKIP;
        num_c.set(num_c.get() - 1);
        if(size(x) < size(y)) swap(x, y);
        data.set(x, data.get(x) + data.get(y));
        return data.set(y, x);
    }
    int root(int x) {
        return data.get(x) < 0 ? x : data.set(x, root(data.get(x)));
    }
    int size(int x) {
        return -data.get(root(x));
    }
    bool same(int x, int y) {
        return root(x) == root(y);
    }
    void snapshot() {
        num_c.snapshot();
        data.snapshot();
    }
    void rollback() {
        num_c.rollback();
        data.rollback();
    }
};

struct rollback_uf_sum : public rollback_unionfind {
    rollback_uf_sum(int n) : rollback_unionfind::rollback_unionfind(n) {}
    // s[x] <- s[x] + s[y]
    template < class F > int unite(int x, int y, F merge) {
        x = root(x), y = root(y);
        if(x == y) return rollback_unionfind::SKIP;
        num_c.set(num_c.get() - 1);
        if(size(x) < size(y)) swap(x, y);
        data.set(x, data.get(x) + data.get(y));
        data.set(y, x);
        merge(x, y);
        return x;
    }
};