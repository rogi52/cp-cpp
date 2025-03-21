#include "template.hpp"

u64 modpow64(u64 a, u64 n, u64 mod) {
    a %= mod;
    u64 res = 1;
    for(; n; n >>= 1) { if(n & 1) res = u128(res) * a % mod; a = u128(a) * a % mod; }
    return res;
}
u64 modpow(u64 a, u64 n, u64 mod) {
    a %= mod;
    u64 res = 1;
    for(; n; n >>= 1) { if(n & 1) res = res * a % mod; a = a * a % mod; }
    return res;
}