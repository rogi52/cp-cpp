#include "template.hpp"

// 集合を扱う
template < class Int, int LG > struct binary_trie_set {
    static constexpr int ROOT = 0;
    struct node_type {
        array<int, 2> to;
        int cnt;
        node_type() : cnt(0) { to.fill(-1); }
    };
    vector<node_type> node;
    Int XOR;
    binary_trie_set() : node(1), XOR(0) {}

    void xor_all(Int x) { XOR ^= x; }
    int size() const { return node[ROOT].cnt; }
    bool empty() const { return size() == 0; }

    bool contains(Int x) const {
        x ^= XOR;
        int now = ROOT;
        REV(i, LG) {
            now = node[now].to[x >> i & 1];
            if(now == -1) return false;
        }
        return node[now].cnt != 0;
    }
    bool insert(Int x) {
        if(contains(x)) return false;
        x ^= XOR;
        int now = ROOT;
        node[now].cnt += 1;
        REV(i, LG) {
            const int k = x >> i & 1;
            if(node[now].to[k] == -1) {
                node[now].to[k] = node.size();
                node.push_back(node_type());
            }
            now = node[now].to[k];
            node[now].cnt += 1;
        }
        return true;
    }
    bool erase(Int x) {
        if(not contains(x)) return false;
        x ^= XOR;
        int now = ROOT;
        node[now].cnt -= 1;
        REV(i, LG) {
            const int k = x >> i & 1;
            now = node[now].to[k];
            node[now].cnt -= 1;
        }
        return true;
    }
    Int meeex() const {
        int now = ROOT;
        Int ans = 0;
        REV(i, LG) {
            if(now == -1) break;
            const int side = XOR >> i & 1;
            const int left = node[now].to[side];
            const Int cap = Int(1) << i;
            if(left != -1 and node[left].cnt >= cap) {
                ans |= Int(1) << i;
                now = node[now].to[side ^ 1];
            } else {
                now = left;
            }
        }
        return ans;
    }
    Int kth(int k) const {
        assert(0 <= k and k < size());
        int now = ROOT;
        Int ans = 0;
        REV(i, LG) {
            const int side = XOR >> i & 1;
            const int left = node[now].to[side];
            const int left_cnt = left != -1 ? node[left].cnt : 0;
            if(k < left_cnt) {
                now = left;
            } else {
                k -= left_cnt;
                ans |= Int(1) << i;
                now = node[now].to[side ^ 1];
            }
        }
        return ans;
    }
    Int max() const {
        assert(not empty());
        return kth(node[ROOT].cnt - 1);
    }
    Int min() const {
        assert(not empty());
        return kth(0);
    }
};