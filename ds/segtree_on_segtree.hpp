#include "template.hpp"
#include "ds/segtree.hpp"

// 0 <= Key
// [Key] * [Key] -> [Value]
template < class Key, class Monoid > struct segtree_on_segtree {
    using M = Monoid;
    using Value = typename M::value_type;
    int n;
    vector<pair<Key, Key>> ps;
    vector<vector<Key>> ys;
    vector<segtree<M>> seg;

    segtree_on_segtree() { assert(M::comm()); }

    void insert(Key x, Key y) {
        ps.push_back({x, y});
    }

    void build() {
        unique(ps);
        n = ssize(ps);
        ys.resize(n + n);
        seg.resize(n + n);
        FOR(i, n) {
            ys[i + n] = {ps[i].second};
            seg[i + n] = segtree<M>(1);
        }
        REV(i, 1, n) {
            vector<Key>& y0 = ys[i << 1 | 0];
            vector<Key>& y1 = ys[i << 1 | 1];
            ys[i].resize(ssize(y0) + ssize(y1));
            merge(y0.begin(), y0.end(), y1.begin(), y1.end(), ys[i].begin());
            ys[i].erase(unique(ys[i].begin(), ys[i].end()), ys[i].end());
            seg[i] = segtree<M>(ssize(ys[i]));
        }
    }

    // (x, y)
    Value v(Key x, Key y) const {
        const pair<Key, Key> p = {x, y};
        int i = LB(ps, p);
        assert(0 <= i and i < n and ps[i] == p);
        return seg[i + n].v(0);
    }

    // (x, y) += w
    void add(Key x, Key y, Value w) {
        const pair<Key, Key> p = {x, y};
        int i = LB(ps, p);
        assert(0 <= i and i < n and ps[i] == p);
        for(i += n; i; i >>= 1) {
            const int pos = LB(ys[i], y);
            seg[i].set(pos, M::op(seg[i].v(pos), w));
        }
    }

    // [Lx, Rx) * [Ly, Ry)
    Value v(Key Lx, Key Rx, Key Ly, Key Ry) const {
        Value s = M::e();
        int l = LB(ps, pair{Lx, Key()});
        int r = LB(ps, pair{Rx, Key()});
        for(l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if(l & 1) s = M::op(s, seg[l].v(LB(ys[l], Ly), LB(ys[l], Ry))), l++;
            if(r & 1) r--, s = M::op(s, seg[r].v(LB(ys[r], Ly), LB(ys[r], Ry)));
        }
        return s;
    }
};