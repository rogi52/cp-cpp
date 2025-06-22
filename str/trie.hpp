#include "template.hpp"

// O(sigma) per move
// ARC201-C https://atcoder.jp/contests/arc201/submissions/67011187
template < int C_SIZE = 26, int C_BEGIN = 'a', class data_type = int > struct prefix_tree {
    static constexpr int ROOT = 0;
    struct node_type {
        array<int, C_SIZE> to;
        vector<int> ids;
        data_type data;
        node_type() : data() { to.fill(-1); }
    };
    int n_str; // 追加した文字列の数
    vector<node_type> node;
    prefix_tree() : node(1), n_str(0) {}

    vector<int> insert(const string& s) {
        vector<int> path;
        int now = ROOT;
        path.push_back(now);
        for(const char c : s) {
            const int k = c - C_BEGIN;
            if(node[now].to[k] == -1) {
                node[now].to[k] = ssize(node);
                node.push_back(node_type());
            }
            path.push_back(now = node[now].to[k]);
        }
        node[now].ids.push_back(n_str++);
        return path;
    }
};