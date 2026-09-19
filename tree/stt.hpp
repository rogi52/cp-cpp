#include "template.hpp"
#include "tree/hld.hpp"

struct static_top_tree {
    int n;
    treeHLD &tree;
    vector<int> P, L, R, A, B, C;
    static_top_tree(treeHLD& tree) : tree(tree) {
        n = tree.n;
        P.assign(n, -1);
        L.assign(n, -1);
        R.assign(n, -1);
        A.assign(n, -1);
        B.assign(n, -1);
        C.assign(n, -1); // compress=1, rake=0
        FOR(v, n) {
            A[v] = tree.parent(v);
            B[v] = v;
        }
        dfs(tree.root);
        assert(ssize(P) == n + n - 1);
    }

    int new_node(int l, int r, int a, int b, int c) {
        const int u = ssize(P);
        P.push_back(-1);
        L.push_back(l);
        R.push_back(r);
        A.push_back(a);
        B.push_back(b);
        C.push_back(c);
        P[l] = P[r] = u;
        return u;
    }

    pair<int, int> dfs(int v) {
        vector<int> path;
        int curr = v;
        while(curr != -1) {
            path.push_back(curr);
            int heavy = -1;
            if(not tree.g[curr].empty()) {
                const int child = tree.g[curr][0].to;
                if(child != tree.parent(curr)) heavy = child;
            }
            curr = heavy;
        }

        vector<pair<int, int>> st;
        st.push_back({0, path[0]});
        auto merge = [&] {
            auto [h2, k2] = st.back(); st.pop_back();
            auto [h1, k1] = st.back(); st.pop_back();
            st.push_back({max(h1, h2) + 1, new_node(k1, k2, A[k1], B[k2], 1)});
        };

        FOR(i, 1, ssize(path)) {
            heap_min<pair<int, int>> pq;
            int k = path[i];
            pq.emplace(0, k);
            for (auto e : tree.g[path[i - 1]]) {
                if (e.to != tree.parent(path[i - 1]) and e.to != path[i]) {
                    pq.emplace(dfs(e.to));
                }
            }
            while (ssize(pq) >= 2) {
                auto [h1, i1] = pq.top(); pq.pop();
                auto [h2, i2] = pq.top(); pq.pop();
                if (i2 == k) swap(i1, i2);
                int i3 = new_node(i1, i2, A[i1], B[i1], 0);
                if (k == i1) k = i3;
                pq.emplace(max(h1, h2) + 1, i3);
            }
            st.push_back(pq.top()); pq.pop();

            while (true) {
                int sz = ssize(st);
                if (sz >= 3 and (st[sz - 3].first == st[sz - 2].first or st[sz - 3].first <= st[sz - 1].first)) {
                    auto top = st.back(); st.pop_back();
                    merge();
                    st.push_back(top);
                } else if (sz >= 2 and st[sz - 2].first <= st[sz - 1].first) {
                    merge();
                } else break;
            }
        }
        while(ssize(st) >= 2) merge();
        return st.back();
    }
};

template < class TreeDP >
struct dynamic_tree_dp {
    using X = typename TreeDP::value_type;
    static_top_tree stt;
    vector< X > dp;

    template < class Func >
    dynamic_tree_dp(treeHLD& tree, const Func& f) : stt(tree) {
        const int n = tree.n;
        dp.resize(n + n - 1);
        FOR(i, n) dp[i] = f(i);
        FOR(i, n, n + n - 1) update(i);
    }
    void update(int i) {
        X& L = dp[stt.L[i]];
        X& R = dp[stt.R[i]];
        dp[i] = stt.C[i] ? TreeDP::compress(L, R) : TreeDP::rake(L, R);
    }

    void set(int v, X x) {
        dp[v] = x;
        for(int i = stt.P[v]; i != -1; i = stt.P[i]) update(i);
    }
    X get() { return dp.back(); }
};


// LC: Point Set Tree Path Composite Sum (Fixed Root)
// https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum_fixed_root
/*
auto make = [&](int v) -> TreeDP::value_type {
    if(v == tree.root) return {1, 0, 1, A[v]};
    const int e = tree.v_to_e(v);
    return {B[e], C[e], 1, B[e] * A[v] + C[e]};
};
*/
template < class mint >
struct point_set_tree_path_composite_sum {
    struct X {
        mint a, b, cnt, ans;
    };
    using value_type = X;
    static X rake(const X& L, const X& R) {
        return {
            L.a,
            L.b,
            L.cnt + R.cnt,
            L.ans + R.ans
        };
    }
    static X compress(const X& L, const X& R) {
        return {
            L.a * R.a,
            L.a * R.b + L.b,
            L.cnt + R.cnt,
            L.ans + L.a * R.ans + L.b * R.cnt
        };
    }
};