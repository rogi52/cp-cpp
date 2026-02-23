#include "template.hpp"
#include "ds/bit_vector.hpp"
#include "ds/fwktree.hpp"

// [Key] * [Key] -> [Value]
template < class Key, class CMonoid, int LG > struct fwktree_on_wavemat {
    using M = CMonoid;
    using Value = typename M::value_type;
    int n;
    vector<int> mid;
    vector<bit_vector> buf;
    vector<fwktree<M>> bit;
    vector<pair<Key, Key>> ps;
    vector<Key> ys;

    fwktree_on_wavemat() {}

    void insert(Key x, Key y) {
        ps.push_back({x, y});
    }
    void build() {
        unique(ps);
        n = ssize(ps);
        for(const auto &[x, y] : ps) ys.push_back(y);
        unique(ys);
        vector<int> a(n);
        FOR(i, n) a[i] = LB(ys, ps[i].second);
        mid.resize(LG);
        buf.assign(LG, bit_vector(n));
        bit.assign(LG, fwktree<M>(n));
        REV(d, LG) {
            vector<int> n_a;
            n_a.reserve(n);
            FOR(i, n) if((a[i] >> d & 1) == 0) n_a.push_back(a[i]);
            mid[d] = ssize(n_a);
            FOR(i, n) if((a[i] >> d & 1) == 1) n_a.push_back(a[i]), buf[d].set(i);
            buf[d].build();
            a = move(n_a); 
        }
    }

    // [x][y] += v
    void add(Key x, Key y, Value w) {
        const pair<Key, Key> p = {x, y};
        int i = LB(ps, p);
        assert(0 <= i and i < n and ps[i] == p);
        REV(d, LG) {
            const int f = buf[d][i];
            i = buf[d].rank(i, f) + (f ? mid[d] : 0);
            bit[d].add(i, w);
        }
    }
    // [Lx, Rx) * [Ly, Ry)
    Value v(Key Lx, Key Rx, Key Ly, Key Ry) const {
        const int l = LB(ps, pair{Lx, Key()});
        const int r = LB(ps, pair{Rx, Key()});
        return M::op(M::inv(pv(l, r, LB(ys, Ly))), pv(l, r, LB(ys, Ry)));
    }
    // [Lx, Rx) * [, Ry)
    Value v(Key Lx, Key Rx, Key Ry) const {
        return pv(LB(ps, pair{Lx, Key()}), LB(ps, pair{Rx, Key()}), LB(ys, Ry));
    }

  private:
    Value pv(int l, int r, int y) const {
        Value res = M::e();
        REV(d, LG) {
            const int l0 = buf[d].rank(l, 0);
            const int r0 = buf[d].rank(r, 0);
            if(y >> d & 1) {
                res = M::op(res, bit[d].v(l0, r0));
                l = mid[d] + (l - l0);
                r = mid[d] + (r - r0);
            } else {
                l = l0;
                r = r0;
            }
        }
        return res;
    }
};