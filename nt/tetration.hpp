#include "template.hpp"
#include "nt/prime.hpp"
#include "bigint.hpp"

bint tetration(vector<bint> a, u64 mod) {
    for(bint x : a) assert(x > 0);
    vector<u64> m = {mod};
    while(m.back() != 1) m.push_back(euler_phi(m.back()));
    bint n = 1;
    REV(i, min(a.size(), m.size())) {
        bint x = 1, v = a[i];
        while(n > 0) {
            if(n % 2 == 1) {
                x *= v;
                if(x >= m[i]) x = x % m[i] + m[i];
            }
            v *= v;
            if(v >= m[i]) v = v % m[i] + m[i];
            n /= 2;
        }
        n = x;
    }
    return n % mod;
}

bint tetration(bint a, u64 b, u64 mod) {
    if(a == 0) return (b % 2 == 0) % mod;
    return tetration(vector<bint>(min<u64>(b, 64), a), mod);
}