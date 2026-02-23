#include "template.hpp"

struct bit_vector {
    int n;
    vector<pair<u64, u32>> data;
    bit_vector(int n) : n(n), built(false) {
        data.assign(((n + 63) >> 6) + 1, {0, 0});
    }
    void set(int i) {
        assert(not built);
        assert(0 <= i and i < n);
        data[i >> 6].first |= 1ULL << (i & 63);
    }
    void build() {
        FOR(i, ssize(data) - 1) data[i + 1].second = data[i].second + bit::pop(data[i].first);
        built = true;
    }
    int rank(int k, int f) const {
        assert(built);
        assert(0 <= k and k <= n);
        const auto [a, s] = data[k >> 6];
        const int res = s + bit::pop(a & (1ULL << (k & 63)) - 1);
        return f ? res : k - res;
    }
    int count(int l, int r, int f) const {
        assert(built);
        assert(0 <= l and l <= r and r <= n);
        return rank(r, f) - rank(l, f);
    }
    int operator[](int i) const {
        assert(0 <= i and i < n);
        return data[i >> 6].first >> (i & 63) & 1;
    }
  private:
    bool built;
};