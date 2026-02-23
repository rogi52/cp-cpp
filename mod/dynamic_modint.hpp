#include "template.hpp"

template < int ID > struct dynamic_modint {
    inline static u32 mod = 998244353;
    using mint = dynamic_modint;
    u32 v;
    static void set_mod(u32 m) { assert(1 <= m); mod = m; }
    constexpr mint& s(u32 v) { this->v = v < mod ? v : v - mod; return *this; }
    dynamic_modint(i64 v = 0) { 
        i64 x = v % mod;
        if(x < 0) x += mod;
        this->v = static_cast<u32>(x);
    }
    mint operator - () const { return mint() - *this; }
    mint operator + () const { return *this; }

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
template < int ID > dynamic_modint<ID> pow(dynamic_modint<ID> x, u64 n) {
    dynamic_modint<ID> p(1);
    for(; n; n >>= 1) { if(n & 1) p *= x; x *= x; }
    return p;
}
template < int ID > dynamic_modint<ID> inv(dynamic_modint<ID> x) {
    int a = x.v, b = dynamic_modint<ID>::mod, u = 1, v = 0;
    while(b) { int t = a / b; swap(a -= t * b, b); swap(u -= t * v, v); }
    return dynamic_modint<ID>(u);
}
template < int ID > istream& operator >> (istream& is, dynamic_modint<ID>& x) { 
    i64 v; is >> v; x = dynamic_modint<ID>(v); return is; 
}
template < int ID > ostream& operator << (ostream& os, const dynamic_modint<ID>& x) { 
    return os << x.v; 
}
using modint = dynamic_modint<-1>;