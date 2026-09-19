#pragma once
#include "template.hpp"
#include "mod/pow.hpp"

// [yukicoder] No.8030 ミラー・ラビン素数判定法のテスト
// https://yukicoder.me/problems/no/8030

// [MojaCoder] 素数判定 (64bit)
// https://mojacoder.app/users/mizar/problems/isprime_64bit


namespace nt {

struct m64 {
    u64 n, inv, r2, one;
    m64(u64 n) : n(n) {
        u64 x = n;
        FOR(i, 5) x *= 2 - n * x;
        inv = -x;
        r2 = -u128(n) % n;
        one = reduce(r2);
    }
    u64 reduce(u128 x) const {
        u64 q = u64(x) * inv;
        u128 m = u128(q) * n;
        u64 x_hi = u64(x >> 64);
        u64 m_hi = u64(m >> 64);
        u64 carry = u64(x) != 0; 
        u64 res = x_hi + m_hi + carry;
        if (res < x_hi || res >= n) res -= n;
        return res;
    }
    u64 mul(u64 a, u64 b) const {
        return reduce(u128(a) * b);
    }
    u64 pow(u64 a, u64 b) const {
        u64 res = one;
        a = mul(a, r2);
        for(; b; b >>= 1) {
            if(b & 1) res = mul(res, a);
            a = mul(a, a);
        }
        return res;
    }
};

// n is prime?
// 0 <= n < 2^64
bool prime_test(const u64 n) {
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    if (n % 5 == 0) return n == 5;
    if (n % 7 == 0) return n == 7;
    if (n < 121) return n > 1;

    m64 m(n);
    const u64 e = n - 1;
    const int z = bit::low(e);
    const u64 d = e >> z;
    const u64 one = m.one;
    const u64 minus_one = n - one;

    static constexpr u64 bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

    auto check = [&](u64 y) {
        if(y == one or y == minus_one) return true;
        FOR(z - 1) {
            y = m.mul(y, y);
            if(y == minus_one) return true;
        }
        return false;
    };
    for(u64 a : bases) {
        u64 a_mod = a % n;
        if(a_mod == 0) continue;
        if(not check(m.pow(a_mod, d))) return false;
    }
    return true;
}

} // namespace nt
