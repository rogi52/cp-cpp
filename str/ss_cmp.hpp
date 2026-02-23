#include "template.hpp"
#include "ds/disjoint_sparse_table.hpp"
#include "alg/minmax.hpp"
#include <atcoder/string>

struct ss_compare {
    int n;
    vector<int> sa;
    vector<int> rank;
    disjoint_sparse_table<alg::min_m<int>> dst;

    ss_compare(const string& s) : n(ssize(s)) {
        sa = atcoder::suffix_array(s);
        rank.resize(n);
        FOR(i, n) rank[sa[i]] = i;
        vector<int> lcp = atcoder::lcp_array(s, sa);
        dst = disjoint_sparse_table<alg::min_m<int>>(lcp);
    }
    int lcp(int i, int j) const {
        assert(0 <= i and i <= n);
        assert(0 <= j and j <= n);
        if(i == j) return n - i;
        if(i >= n or j >= n) return 0;
        int l = rank[i], r = rank[j];
        if(l > r) swap(l, r);
        return dst.prod(l, r);
    }
    int cmp(int l1, int r1, int l2, int r2) const {
        assert(0 <= l1 and l1 <= r1 and r1 <= n);
        assert(0 <= l2 and l2 <= r2 and r2 <= n);
        const int n1 = r1 - l1;
        const int n2 = r2 - l2;
        const int c = lcp(l1, l2);
        if(c < min(n1, n2)) return rank[l1] < rank[l2] ? -1 : +1;
        if(n1 == n2) return 0;
        return n1 < n2 ? -1 : +1;
    }
};