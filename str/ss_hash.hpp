#include "template.hpp"
#include "rnd.hpp"

template < class hvec > struct ss_hash {
    static const hvec b;
    static vector<hvec> p;
    vector<hvec> h;
    ss_hash() {}
    ss_hash(const string& s) {
        const int n = s.size();
        h.resize(n + 1); h[0] = hvec(0);
        FOR(i, n) h[i + 1] = h[i] * b + hvec(s[i]);
    }
    // [l, r)
    hvec get(int l, int r) const { while(ssize(p) <= r - l) p.push_back(p.back() * b); return h[r] - h[l] * p[r - l]; }
    hvec cat(hvec h1, hvec h2, int h2_len) { while(ssize(p) <= h2_len) p.push_back(p.back() * b); return h1 * p[h2_len] + h2; }
    static int lcp(const ss_hash<hvec>& sh1, int l1, int r1, const ss_hash<hvec>& sh2, int l2, int r2) {
        return bin_search<int>(-1, min(r1 - l1, r2 - l2) + 1, [&](int x) { return sh1.get(l1, l1 + x) == sh2.get(l2, l2 + x); });
    }
    static int cmp(const string& s1, const ss_hash<hvec>& sh1, int l1, int r1, const string& s2, const ss_hash<hvec>& sh2, int l2, int r2) {
        const int x = lcp(sh1, l1, r1, sh2, l2, r2);
        if(x == r1 - l1 and x == r2 - l2) return 0;
        if(x == r1 - l1) return -1;
        if(x == r2 - l2) return +1;
        return s1[l1 + x] < s2[l2 + x] ? -1 : +1;
    }
};
template < class hvec > const hvec ss_hash<hvec>::b = hvec(rnd::i<u32>(0, -1));
template < class hvec > vector<hvec> ss_hash<hvec>::p = {1};