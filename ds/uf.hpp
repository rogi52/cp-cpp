#include "template.hpp"

struct union_find {
    int num_c; // 連結成分の個数
    vector<int> data;
    static constexpr int SKIP = -1;
    union_find() {}
    union_find(int n) : num_c(n), data(n, -1) {}

    int unite(int x, int y) {
        x = root(x), y = root(y);
        if(x == y) return SKIP;
        num_c--;
        if(size(x) < size(y)) swap(x, y);
        data[x] += data[y];
        return data[y] = x;
    }
    int root(int x) { return data[x] < 0 ? x : data[x] = root(data[x]); }
    int size(int x) { return -data[root(x)]; }
    bool same(int x, int y) { return root(x) == root(y); }

    // merge(x, y): s[x] <- s[x] + s[y]
    template < class F >
    int unite(int x, int y, const F& merge) {
        x = root(x), y = root(y);
        if(x == y) return SKIP;
        num_c--;
        if(size(x) < size(y)) swap(x, y);
        data[x] += data[y];
        data[y] = x;
        merge(x, y); // s[x] <- s[x] + s[y]
        return x;
    }
};
