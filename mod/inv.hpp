#include "template.hpp"

i64 modinv(i64 a, i64 mod) {
    i64 b = mod, u = 1, v = 0;
    while(b) { i64 t = a / b; swap(a -= t * b, b); swap(u -= t * v, v); }
    u %= mod;
    if(u < 0) u += mod;
    return u;
}