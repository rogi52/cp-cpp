#include "template.hpp"
#include "tree/hld.hpp"

struct static_top_tree_rr {
    int n;
    treeHLD &tree;
    vector<int> P, L, R, A, B, C;
    
    static_top_tree_rr(treeHLD& tree) : tree(tree) {
        n = tree.n;
        P.assign(n, -1);
        L.assign(n, -1);
        R.assign(n, -1);
        A.assign(n, -1);
        B.assign(n, -1);
        C.assign(n, 0);
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
            auto top1 = st.back(); st.pop_back();
            auto top2 = st.back(); st.pop_back();
            auto [h2, k2] = top1;
            auto [h1, k1] = top2;
            st.push_back({max(h1, h2) + 1, new_node(k1, k2, A[k1], B[k2], 1)});
        };

        FOR(i, 1, ssize(path)) {
            int k = path[i];
            
            vector<pair<int, int>> lights;
            for (auto e : tree.g[path[i - 1]]) {
                if (e.to != tree.parent(path[i - 1]) and e.to != path[i]) {
                    lights.push_back(dfs(e.to));
                }
            }
            
            if (!lights.empty()) {
                heap_min<pair<int, int>> pq;
                for(auto light : lights) pq.emplace(light);
                while(ssize(pq) >= 2) {
                    auto [h1, i1] = pq.top(); pq.pop();
                    auto [h2, i2] = pq.top(); pq.pop();
                    const int i3 = new_node(i1, i2, A[i1], B[i1], 0);
                    pq.emplace(max(h1, h2) + 1, i3);
                }
                auto [h_light, light_root] = pq.top(); pq.pop();
                k = new_node(k, light_root, A[k], B[k], 0);
            }
            
            st.push_back({0, k});
            while(true) {
                int sz = ssize(st);
                if(sz >= 3 and (st[sz - 3].first == st[sz - 2].first or st[sz - 3].first <= st[sz - 1].first)) {
                    auto top = st.back(); st.pop_back();
                    merge();
                    st.push_back(top);
                } else if(sz >= 2 and st[sz - 2].first <= st[sz - 1].first) {
                    merge();
                } else break;
            }
        }
        while(ssize(st) >= 2) merge();
        return st.back();
    }
};

template < class TreeDP >
struct dynamic_rerooting_tree_dp {
    using X = typename TreeDP::value_type;
    static_top_tree_rr stt;
    vector<pair<X, X>> dp;

    template < class Func >
    dynamic_rerooting_tree_dp(treeHLD& tree, const Func& f) : stt(tree) {
        const int n = tree.n;
        dp.resize(n + n - 1);
        dp[tree.root] = {TreeDP::unit(), TreeDP::unit()};
        FOR(i, n) if(i != tree.root) dp[i] = f(i);
        FOR(i, n, n + n - 1) update(i);
    }

    void update(int i) {
        const X& L1 = dp[stt.L[i]].first;
        const X& L2 = dp[stt.L[i]].second;
        const X& R1 = dp[stt.R[i]].first;
        const X& R2 = dp[stt.R[i]].second;
        if (stt.C[i]) {
            dp[i] = {TreeDP::compress(L1, R1), TreeDP::compress2(L2, R2)};
        } else {
            dp[i] = {TreeDP::rake(L1, R1), TreeDP::rake2(L2, R1)};
        }
    }

    void set(int v, pair<X, X> x) {
        assert(v > 0);
        dp[v] = x;
        for(int i = stt.P[v]; i != -1; i = stt.P[i]) update(i);
    }

    X prod_all(int v) {
        int i = v;
        X a = dp[i].second;
        X b = TreeDP::unit();
        X c = TreeDP::unit();
        while (true) {
            int p = stt.P[i];
            if (p == -1) break;
            int l = stt.L[p], r = stt.R[p];
            if (stt.C[p]) {
                if (l == i) {
                    b = TreeDP::compress(b, dp[r].first);
                } else {
                    a = TreeDP::compress2(dp[l].second, a);
                }
            } else {
                if (l == i) {
                    a = TreeDP::rake2(a, dp[r].first);
                } else {
                    a = TreeDP::rake3(a, b);
                    c = TreeDP::compress2(a, c);
                    a = TreeDP::unit();
                    b = dp[l].first;
                }
            }
            i = p;
        }
        a = TreeDP::rake3(a, b);
        return TreeDP::compress2(a, c);
    }
};

// LC: Point Set Tree Path Composite Sum
// 提出: https://judge.yosupo.jp/submission/358537
template < class mint >
struct point_set_tree_path_composite_sum_rr {
    struct X {
        mint a, b, cnt, ans;
    };
    using value_type = X;

    static X unit() { return {1, 0, 0, 0}; }
    static X rake(const X& L, const X& R) {
        return {L.a, L.b, L.cnt + R.cnt, L.ans + R.ans};
    }
    static X compress(const X& L, const X& R) {
        return {
            L.a * R.a,
            L.a * R.b + L.b,
            L.cnt + R.cnt,
            L.ans + L.a * R.ans + L.b * R.cnt
        };
    }
    static X rake2(const X& L, const X& R) {
        return {L.a, L.b, L.cnt + R.cnt, L.ans + L.a * R.ans + L.b * R.cnt};
    }
    static X rake3(const X& L, const X& R) { return rake(L, R); }
    static X compress2(const X& L, const X& R) { return compress(R, L); }
};
/*
using TreeDP = point_set_tree_path_composite_sum;
auto make = [&](int v) -> pair<TreeDP::X, TreeDP::X> {
    assert(v > 0);
    const int e = tree.v_to_e(v);
    TreeDP::X up = {B[e], C[e], 1, B[e] * A[v] + C[e]};
    TreeDP::X down = {B[e], C[e], 1, A[v]};
    return {up, down};
};
*/