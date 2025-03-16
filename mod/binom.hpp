#include "template.hpp"

template < class mint > mint fact(int n) {
    static const u32 mod = mint::get_mod();
    assert(0 <= n); assert(n < mod); assert(mint::prime_mod);
    static vector<mint> data = {1, 1};
    while(ssize(data) <= n) { const int i = ssize(data); data.push_back(data.back() * i); }
    return data[n];
}
template < class mint > mint inv(int n) {
    static const u32 mod = mint::get_mod();
    assert(0 <= n); assert(n < mod); assert(mint::prime_mod);
    static vector<mint> data = {1, 1};
    while(ssize(data) <= n) { const int i = ssize(data); data.push_back(-data[mod % i] * (mod / i)); }
    return data[n];
}
template < class mint > mint fact_inv(int n) {
    static const u32 mod = mint::get_mod();
    assert(0 <= n); assert(n < mod); assert(mint::prime_mod);
    static vector<mint> data = {1, 1};
    while(ssize(data) <= n) { const int i = ssize(data); data.push_back(data.back() * inv<mint>(i)); }
    return data[n];
}
template < class mint > mint comb(int n, int k) {
    return 0 <= k and k <= n ? fact<mint>(n) * fact_inv<mint>(k) * fact_inv<mint>(n - k) : 0;
}

template < class mint > struct powers {
    mint a; vector<mint> data;
    powers(const mint a) : a(a), data({1}) {}
    // a^n
    mint get(int n) {
        assert(0 <= n);
        while(ssize(data) <= n) data.push_back(data.back() * a);
        return data[n];
    }
};