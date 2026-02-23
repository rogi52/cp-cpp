#include "template.hpp"
#include "hld.hpp"

template < class Push, class Query, class Pop, class Reset >
void dsu_on_tree(const treeHLD& tree, const Push& push, const Query& query, const Pop& pop, const Reset& reset) {
    assert(tree.is_decomped());
    auto dfs = [&](auto dfs, int v, int p, bool keep) -> void {
        if(tree.subtree_size(v) != 1) {
            FOR(i, 1, ssize(tree.g[v])) {
                const auto& e = tree.g[v][i];
                if(e.to != p) dfs(dfs, e.to, v, false);
            }
            dfs(dfs, tree.g[v][0].to, v, true);
            FOR(i, tree.seq_end(tree.g[v][0].to), tree.seq_end(v)) push(tree.euler(i));
        }
        push(v);
        query(v);
        if(not keep) {
            const auto [l, r] = tree.seq_seg(v);
            FOR(i, l, r) pop(tree.euler(i));
            reset();
        }
    };
    dfs(dfs, tree.root, -1, false);
}

/*
参考: https://nyaannyaan.github.io/library/tree/dsu-on-tree.hpp.html

問題 AOJ-2995 Colorful Tree
+ 問題 URL: https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2995
+ 提出: https://onlinejudge.u-aizu.ac.jp/solutions/problem/2995/review/11270863/rogi_52/C++20?keyword=2995


次を実装する
*******************************************
// 頂点 v を追加する
auto push = [&](int v) {
    
};
// 頂点 v のクエリに答える (複数ある場合は，v ごとにまとめておけば OK)
auto query = [&](int v) {
};
// 頂点 v を削除する
auto pop = [&](int v) {
};
// 構造全体のリセット
auto reset = [&]() {
};
dsu_on_tree(tree, push, query, pop, reset);
*******************************************
*/