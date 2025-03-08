#include "template.hpp"

struct union_find {
    vector<int> data;
    union_find(int n) : data(n, -1) {}

    int unite(int x, int y) {
        x = root(x), y = root(y);
        if(x == y) return -1;
        if(size(x) < size(y)) swap(x, y);
        data[x] += data[y];
        return data[y] = x;
    }
    int root(int x) { return data[x] < 0 ? x : data[x] = root(data[x]); }
    int size(int x) { return -data[root(x)]; }
    bool same(int x, int y) { return root(x) == root(y); }
};

template < class F > struct uf_sum : public union_find {
    uf_sum(int n) : union_find::union_find(n) {}
    int unite(int x, int y, F merge) {
        x = root(x), y = root(y);
        if(x == y) return -1;
        if(size(x) < size(y)) swap(x, y);
        data[x] += data[y];
        data[y] = x;
        merge(x, y); // s[x] <- s[x] + s[y]
        return x;
    }
};