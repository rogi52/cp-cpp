#include "template.hpp"

// AGC023-F 01 on Tree
// https://atcoder.jp/contests/agc023/submissions/59018603
template < class tomd >
pair< typename tomd::value_type, vector<int> > choice_on_tree(vector<int> parent, vector<typename tomd::value_type> value) {
    const int n = parent.size();
    const int root = [&] {
        for(int v = 0; v < n; v++) if(parent[v] == -1) return v;
        assert(0);
    }();

    union_find uf(n);
    vector<int> head(n), tail(n), succ(n, -1);
    for(int v = 0; v < n; v++) head[v] = tail[v] = v;

    struct data {
        tomd::value_type x; int id, sz;
        bool operator < (const data& r) const { return tomd::cmp(x, r.x); }
    };
    priority_queue<data> pq;
    for(int v = 0; v < n; v++) if(v != root) pq.push(data{value[v], v, 1});

    while(not pq.empty()) {
        auto [_, v, sv] = pq.top(); pq.pop();
        if(uf.size(v) != sv) continue;
        int hv = head[uf.root(v)];
        int tv = tail[uf.root(v)];
        int p = uf.root(parent[hv]);
        int hp = head[p];
        int tp = tail[p];
        int x = uf.unite(p, v);
        value[x] = tomd::op(value[p], value[v]);
        head[x] = hp;
        tail[x] = tv;
        succ[tp] = hv;
        if(x != uf.root(root)) pq.push(data{value[x], x, uf.size(x)});
    }
    vector<int> ord = {root};
    while(succ[ord.back()] != -1) ord.push_back(succ[ord.back()]);
    return {value[uf.root(root)], ord};
}

struct i01 {
    struct value_type {
        i64 c0, c1, inv;
    };
    static value_type op(const value_type& l, const value_type& r) {
        value_type x;
        x.inv = l.inv + r.inv + l.c1 * r.c0;
        x.c0 = l.c0 + r.c0;
        x.c1 = l.c1 + r.c1;
        return x;
    }
    static value_type id() {
        return value_type{0, 0, 0};
    }
    static bool cmp(const value_type& l, const value_type& r) {
        return l.c0 * r.c1 < r.c0 * l.c1;
    }
};

// int main() {
//     int N = in();
//     vector<int> P(N, -1);
//     for(int i : rep(1, N)) P[i] = in(), P[i]--;
//     vector<i01::value_type> V(N);
//     for(int v : rep(N)) {
//         int x = in();
//         if(x == 0) V[v] = {1, 0, 0};
//         if(x == 1) V[v] = {0, 1, 0};
//     }
//     auto [ans, ord] = choice_on_tree<i01>(P, V);
//     print(ans.inv);
// }
