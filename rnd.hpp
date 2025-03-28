#pragma once
#include "template.hpp"

namespace rnd {
    u32 seed; mt19937 mt;
    struct gen_seed { gen_seed() { seed = random_device()(); mt = mt19937(seed); } } gen_seed_instance;
    // [L, R)
    template < class Int > Int i(Int L, Int R) { assert(L < R); return uniform_int_distribution<Int>(L, R - 1)(mt); }
    template < class Real > Real r(Real L, Real R) { assert(L <= R); return uniform_real_distribution<Real>(L, R)(mt); }
}

template < int n, array<u32, n> mod > struct hash_vector {
    array<u32, n> a;
    using hvec = hash_vector;
    hvec& s(array<u32, n> a) { FOR(i, n) this->a[i] = a[i] < mod[i] ? a[i] : a[i] - mod[i]; return *this; }
    hash_vector(u32 v = 0) { FOR(i, n) a[i] = v % mod[i] + mod[i]; s(a); }
    hvec operator - () const { return hvec() - *this; }
    hvec& operator += (const hvec& r) { FOR(i, n) a[i] += r.a[i]; return s(a); }
    hvec& operator -= (const hvec& r) { FOR(i, n) a[i] += mod[i] - r.a[i]; return s(a); }
    hvec& operator *= (const hvec& r) { FOR(i, n) a[i] = u64(a[i]) * r.a[i] % mod[i]; return *this; }
    hvec& operator /= (const hvec& r) { return *this *= inv(r); }
    hvec operator + (const hvec& r) const { return hvec(*this) += r; }
    hvec operator - (const hvec& r) const { return hvec(*this) -= r; }
    hvec operator * (const hvec& r) const { return hvec(*this) *= r; }
    hvec operator / (const hvec& r) const { return hvec(*this) /= r; }
    bool operator == (const hvec& r) const { return a == r.a; }
    bool operator != (const hvec& r) const { return a != r.a; }
};
template < int n, array<u32, n> mod > hash_vector<n, mod> pow(hash_vector<n, mod> x, u64 m) {
    hash_vector<n, mod> p(1);
    for(; m; m >>= 1) { if(m & 1) p *= x; x *= x; }
    return p;
}
template < int n, array<u32, n> mod > hash_vector<n, mod> inv(hash_vector<n, mod> x) {
    hash_vector<n, mod> res;
    FOR(i, n) {
        u32 a = x.a[i], b = mod[i], u = 1, v = 0;
        while(b) { u32 t = a / b; swap(a -= t * b, b); swap(u -= t * v, v); }
        res[i] = u;
    }
    return res;
}
template < int n, array<u32, n> mod > ostream& operator << (ostream& os, const hash_vector< n, mod >& x) { FOR(i, n) { if(i) os << ' '; os << x.a[i]; } return os; }
using hvec1 = hash_vector< 1, array<u32, 1>{999999937} >;
using hvec2 = hash_vector< 2, array<u32, 2>{999999937, 1000000007} >;
using hvec3 = hash_vector< 3, array<u32, 3>{999999937, 1000000007, 1000000009} >;
using hvec4 = hash_vector< 4, array<u32, 4>{999999937, 1000000007, 1000000009, 1000000021} >;