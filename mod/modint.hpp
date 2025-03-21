#include "template.hpp"

template < u32 mod_, u32 prime_, u32 root_ > struct static_modint {
    static constexpr u32 const & mod = mod_;
    static constexpr u32 const & prime = prime_;
    static constexpr u32 const & root = root_;
    u32 v;
    using mint = static_modint;
    constexpr mint& s(u32 v) { this->v = v < mod ? v : v - mod; return *this; }
    constexpr static_modint(i64 v = 0) { s(v % mod + mod); }
    mint operator - () const { return mint() - *this; }
    mint& operator += (const mint& r) { return s(v + r.v); }
    mint& operator -= (const mint& r) { return s(v + mod - r.v); }
    mint& operator *= (const mint& r) { v = u64(v) * r.v % mod; return *this; }
    mint& operator /= (const mint& r) { return *this *= inv(r); }
    mint operator + (const mint& r) const { return mint(*this) += r; }
    mint operator - (const mint& r) const { return mint(*this) -= r; }
    mint operator * (const mint& r) const { return mint(*this) *= r; }
    mint operator / (const mint& r) const { return mint(*this) /= r; }
    bool operator == (const mint& r) const { return v == r.v; }
    bool operator != (const mint& r) const { return v != r.v; }
};
// x^n
template < u32 mod, u32 prime, u32 root > static_modint<mod, prime, root> pow(static_modint<mod, prime, root> x, u64 n) {
    static_modint<mod, prime, root> p(1);
    for(; n; n >>= 1) { if(n & 1) p *= x; x *= x; }
    return p;
}
// x^{-1}
template < u32 mod, u32 prime, u32 root > static_modint<mod, prime, root> inv(static_modint<mod, prime, root> x) {
    int a = x.v, b = mod, u = 1, v = 0;
    while(b) { int t = a / b; swap(a -= t * b, b); swap(u -= t * v, v); }
    return static_modint<mod, prime, root>(u);
}
template < u32 mod, u32 prime, u32 root > istream& operator >> (istream& is, static_modint<mod, prime, root>& x) { i64 v; is >> v; x = static_modint<mod, prime, root>(v); return is; }
template < u32 mod, u32 prime, u32 root > ostream& operator << (ostream& os, const static_modint<mod, prime, root>& x) { return os << x.v; }

using modint998 = static_modint<  998'244'353, 1, 3>;
using modint107 = static_modint<1'000'000'007, 1, 5>;
