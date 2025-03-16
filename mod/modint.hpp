#include "template.hpp"

template < u32 mod, u32 is_prime > struct static_modint {
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
    static u32 get_mod() { return mod; }
    static u32 prime_mod() { return is_prime; }
};
// x^n
template < u32 mod, u32 is_prime > static_modint<mod, is_prime> pow(static_modint<mod, is_prime> x, u64 n) {
    static_modint<mod, is_prime> p(1);
    for(; n; n >>= 1) { if(n & 1) p *= x; x *= x; }
    return p;
}
// x^{-1}
template < u32 mod, u32 is_prime > static_modint<mod, is_prime> inv(static_modint<mod, is_prime> x) {
    u32 a = x.v, b = mod, u = 1, v = 0;
    while(b) { u32 t = a / b; swap(a -= t * b, b); swap(u -= t * v, v); }
    return static_modint<mod, is_prime>(u);
}
template < u32 mod, u32 is_prime > istream& operator >> (istream& is, static_modint<mod, is_prime>& x) { i64 v; is >> v; x = static_modint<mod, is_prime>(v); return is; }
template < u32 mod, u32 is_prime > ostream& operator << (ostream& os, const static_modint<mod, is_prime>& x) { return os << x.v; }

using modint998 = static_modint<  998'244'353, 1>;
using modint107 = static_modint<1'000'000'007, 1>;
