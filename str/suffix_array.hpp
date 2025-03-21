#include "template.hpp"
#include "ds/segtree.hpp"
#include "alg/minmax.hpp"

struct suffix_array {
    int n;
    string s;
    vector<int> sa, rank, tmp;
    segtree<alg::min_m<int>> RmQ;
    segtree<alg::max_m<int>> RMQ;
    suffix_array(const string& s) : s(s), n(ssize(s)), sa(n + 1), rank(n + 1), tmp(n + 1) {
        for(int i = 0; i <= n; i++) {
            sa[i] = i;
            rank[i] = i < n ? s[i] : -1;
        }
        for(int k = 1; k <= n; k *= 2) {
            auto cmp = [&](int i, int j) {
                if(rank[i] != rank[j]) return rank[i] < rank[j];
                int ri = i + k <= n ? rank[i + k] : -1;
                int rj = j + k <= n ? rank[j + k] : -1;
                return ri < rj;
            };
            sort(sa, cmp);
            tmp[sa[0]] = 0;
            for(int i = 1; i <= n; i++)
                tmp[sa[i]] = tmp[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
            for(int i = 0; i <= n; i++) rank[i] = tmp[i];
        }
        RmQ = segtree<alg::min_m<int>>(sa);
        RMQ = segtree<alg::max_m<int>>(sa);
    }
    int first(const string& t) {
        int L = 0, R = n;
        while(R - L > 1) {
            int M = (L + R) / 2;
            (s.compare(sa[M], t.size(), t) < 0 ? L : R) = M;
        }
        if(s.compare(sa[R], t.size(), t) == 0) {
            return R;
        } else {
            return -1;
        }
    }
    int last(const string& t) {
        int L = 1, R = n + 1;
        while(R - L > 1) {
            int M = (L + R) / 2;
            (s.compare(sa[M], t.size(), t) <= 0 ? L : R) = M;
        }
        if(s.compare(sa[L], t.size(), t) == 0) {
            return L;
        } else {
            return -1;
        }
    }
    int leftest(const string& t) {
        int L = first(t);
        if(L == -1) return -1;
        int R = last(t);
        return RmQ.v(L, R + 1);
    }
    int rightest(const string& t) {
        int L = first(t);
        if(L == -1) return -1;
        int R = last(t);
        return RMQ.v(L, R + 1);
    }
};