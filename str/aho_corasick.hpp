#include "template.hpp"

// ABC419-F All Included (https://atcoder.jp/contests/abc419/tasks/abc419_f)
template < int C_SIZE = 26, int C_BEGIN = 'a', class data_type = int > struct aho_corasick {
    static constexpr int ROOT = 0;
    struct node_type {
        array<int, C_SIZE> to;
        vector<int> ids;
        int fail;
        int drct; // node[v] の suffix でマッチするもののうち最長のもの
        data_type data;
        node_type() : fail(ROOT), drct(ROOT), data() { to.fill(-1); }
    };

    int n_str; // 追加した文字列の数
    vector<node_type> node;
    bool built;
    aho_corasick() : node(1), n_str(0), built(false) {}

    vector<int> insert(const string& s) {
        assert(not built);
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

    // オートマトンにする
    void build() {
        assert(not built);
        queue<int> Q;
        FOR(k, C_SIZE) {
            if(node[ROOT].to[k] == -1) {
                node[ROOT].to[k] = ROOT;
            } else {
                Q.push(node[ROOT].to[k]);
            }
        }
        while(not Q.empty()) {
            const int now = Q.front(); Q.pop();
            const int fail = node[now].fail;
            node[now].drct = node[fail].ids.empty() ? node[fail].drct : fail;
            FOR(k, C_SIZE) {
                int& nxt = node[now].to[k];
                if(nxt == -1) {
                    nxt = node[fail].to[k];
                } else {
                    node[nxt].fail = node[fail].to[k];
                    Q.push(nxt);
                }
            }
        }
        built = true;
    }

    vector<int> match(int v) {
        assert(built);
        vector<int> res;
        for(int now = v; now != ROOT; now = node[now].drct) {
            for(int i : node[now].ids) res.push_back(i);
        }
        return res;
    }
};