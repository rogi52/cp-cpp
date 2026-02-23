#include "template.hpp"
#include "tree/rerooting.hpp"

// edge_type: {to, e_id}
// e_id はダミー，両方向の辺，根は 0 とする．
pair<int, vector<int>> subtree_hash(const vector<vector<pair<int, int>>>& tree) {
    const int n = ssize(tree);
    using hash_type = hvec2;
    vector<hash_type> h(n);
    FOR(i, n) h[i] = hash_type(rnd::i<int>(0, INF32));

    struct data_type {
        hash_type hash;
        int depth;
        bool operator < (const data_type& x) const {
            return hash < x.hash;
        }
        bool operator == (const data_type& x) const {
            return hash == x.hash;
        }
    };
    auto id = [&]() {
        return data_type{hash_type(1), 0};
    };
    auto f_ee = [&](data_type a, data_type b) -> data_type {
        return data_type{a.hash * b.hash, max(a.depth, b.depth + 1)};
    };
    auto f_ev = [&](data_type x, int e_id) -> data_type {
        return x;
    };
    auto f_vp = [&](data_type x, int v_id) -> data_type {
        return data_type{x.hash + h[x.depth], x.depth};
    };
    rerooting<data_type, decltype(f_ee), decltype(f_ev), decltype(f_vp), decltype(id)> rr(tree, f_ee, f_ev, f_vp, id);
    rr.solve(0);

    vector<data_type> key = rr.dp;
    unique(key);
    vector<int> idx(n);
    FOR(i, n) idx[i] = LB(key, rr.dp[i]);
    return {ssize(key), idx};
}