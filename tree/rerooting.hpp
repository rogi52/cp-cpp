#include "template.hpp"
#include "tree/hld.hpp"

/*
例: PAST20-L
struct S {
    int len, cnt;
};
auto id = [&]() {
    return S{-1, 0};
};
auto f_ee = [&](S a, S b) -> S {
    if(a.len > b.len) {
        return a;
    } else if(a.len < b.len) {
        return b;
    } else {
        return S{a.len, a.cnt + b.cnt};
    }
};
auto f_ev = [&](S x, int e_id) -> S {
    return x;
};
auto f_vp = [&](S x, int v_id) -> S {
    if(x.len < 0) return S{0, 1};
    return S{x.len + 1, x.cnt};
};
rerooting<S, decltype(f_ee), decltype(f_ev), decltype(f_vp), decltype(id)> rr(T, f_ee, f_ev, f_vp, id);
rr.solve(0);
rr.reroot();
*/

/*
edge_type: {to, e_id}
*/
template < class T, class EE, class EV, class VP, class ID > struct rerooting {
    int n;
    int r;
    vector< T > dp, dp_r, ans;
    vector<vector< T >> sub;
    vector<vector<pair<int, int>>> tree;
    EE f_ee;
    EV f_ev;
    VP f_vp;
    ID id;
    rerooting(
        const vector<vector<pair<int, int>>>& tree,
        const EE& f_ee,
        const EV& f_ev,
        const VP& f_vp,
        const ID& id
    ) : n(ssize(tree)), tree(tree), f_ee(f_ee), f_ev(f_ev), f_vp(f_vp), id(id) {}

    void solve(int r) {
        this->r = r;
        dp.assign(n, id());
        sub.resize(n);
        FOR(v, n) sub[v].resize(ssize(tree[v]));
        auto dfs = [&](auto&& dfs, int v, int p) -> T {
            FOR(i, ssize(tree[v])) {
                auto [to, e_id] = tree[v][i];
                if(to != p) {
                    sub[v][i] = dfs(dfs, to, v);
                    dp[v] = f_ee(dp[v], f_ev(sub[v][i], e_id));
                }
            }
            return dp[v] = f_vp(dp[v], v);
        }; dfs(dfs, r, -1);
    }

    void reroot() {
        dp_r.assign(n, id());
        auto dfs = [&](auto&& dfs, int v, int p, T s) -> void {
            const int m = ssize(tree[v]);
            FOR(i, m) {
                auto [to, e_id] = tree[v][i];
                if(to == p) sub[v][i] = s;
            }
            vector< T > R(m + 1);
            R[m] = id();
            REV(i, m) {
                auto [to, e_id] = tree[v][i];
                R[i] = f_ee(R[i + 1], f_ev(sub[v][i], e_id));
            }
            T L = id();
            FOR(i, m) {
                auto [to, e_id] = tree[v][i];
                if(to != p) dfs(dfs, to, v, f_vp(f_ee(L, R[i + 1]), v));
                dp_r[to] = f_vp(f_ee(L, R[i + 1]), v);
                L = f_ee(L, f_ev(sub[v][i], e_id));
            }
        }; dfs(dfs, r, -1, id());

        ans.assign(n, id());
        FOR(v, n) {
            FOR(i, ssize(tree[v])) {
                auto [to, e_id] = tree[v][i];
                ans[v] = f_ee(ans[v], f_ev(sub[v][i], e_id));
            }
            ans[v] = f_vp(ans[v], v);
        }
    }

    T get(int r, int v, treeHLD& hld) {
        if(r == v) return ans[v];
        if(not hld.in_subtree(v, r)) return dp[v] ;
        return dp_r[hld.jump(v, r, 1)];
    }
};