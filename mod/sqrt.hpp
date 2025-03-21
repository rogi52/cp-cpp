#include "template.hpp"
#include "mod/pow.hpp"
#include "rnd.hpp"

// solve x^2 = a (mod p)
// return x
// or no solution (-1)
i32 modsqrt(i32 a, i32 p) {
    if(p == 2) return a;
    a %= p;
    if(a == 0) return 0;
    if(modpow(a, p >> 1, p) != 1) return -1;
    i32 q = p - 1, m = 0; while(q % 2 == 0) q >>= 1, m++;
    i32 z; do { z = rnd::i<i32>(1, p); } while(modpow(z, p >> 1, p) != p - 1);
    i64 c = modpow(z, q, p), t = modpow(a, q, p), r = modpow(a, (q + 1) >> 1, p);
    while(m > 1) {
        if(modpow(t, 1 << (m - 2), p) != 1) r = r * c % p, t = t * (c * c % p) % p;
        c = c * c % p;
        m--;
    }
    return r;
}