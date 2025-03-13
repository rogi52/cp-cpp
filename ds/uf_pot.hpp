#include "template.hpp"

template < class Group > struct uf_pot {
    using G = Group;
    using T = typename G::value_type;
    vector<int> data;
    vector< T > pot;
    static constexpr int SKIP  = -1;
    static constexpr int ERROR = -2;
    uf_pot(int n) : data(n, -1), pot(n, G::e()) {}

    // p[x] = p[y] * x
    int unite(int x, int y, T w) {
        w = G::op(G::inv(p(y)), G::op(w, p(x)));
        x = root(x), y = root(y);
        if(x == y) return d(x, y) == w ? SKIP : ERROR;
        if(size(x) < size(y)) w = G::inv(w), swap(x, y);
        data[x] += data[y];
        pot[y] = w;
        return data[y] = x;
    }
    int root(int x) { compress(x); return data[x] < 0 ? x : data[x]; }
    int size(int x) { return -data[root(x)]; }
    bool same(int x, int y) { return root(x) == root(y); }
    T p(int x) { compress(x); return pot[x]; }
    T d(int x, int y) { return G::op(p(y), G::inv(p(x))); }

  private:
    void compress(int x) {
        if(data[x] < 0) return;
        const int p = data[x];
        compress(p);
        pot[x] = G::op(pot[x], pot[p]);
        data[x] = data[p] < 0 ? p : data[p];
    }
};