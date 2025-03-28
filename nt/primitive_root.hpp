#include "template.hpp"
#include "nt/prime.hpp"

u64 primitive_root(u64 p) {
    vector<u64> pf = factor(p - 1);
    unique(pf);
    for(u64& q : pf) q = (p - 1) / q;
    while(true) {
        const u64 g = rnd::i<u64>(1, p);
        if([&] {
            for(const u64 q : pf) if(modpow64(g, q, p) == 1) return false;
            return true;
        }()) return g;
    }
    return 0;
}