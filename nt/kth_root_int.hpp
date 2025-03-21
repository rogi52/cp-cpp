#include "template.hpp"

// floor(a^{1/k})
// | 0 <= a < 2^64
// | 1 <= k
u64 kth_root(u64 a, u64 k) {
    if(a == 0) return 0;
    if(k == 1) return a;
    if(k >= 64) return 1;

    const static u64 ub[] = { 0,
        18446744073709551615ULL, 4294967295, 2642245, 65535, 7131, 1625, 565, 255,
        138, 84, 56, 40, 30, 23, 19, 15,
        13, 11, 10, 9, 8, 7, 6, 6,
        5, 5, 5, 4, 4, 4, 4, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 1
    };

    if(k == 2) { for(u64 x = min<u64>(sqrtl(a), ub[2]); ; x--) if(x * x <= a) return x; }
    if(k == 3) { for(u64 x = min<u64>(cbrtl(a), ub[3]); ; x--) if(x * x * x <= a) return x; }

    const static auto pow = [](u64 x, u64 e) -> u64 {
        u64 res = 1;
        for(u64 i = 0; i < e; i++) res *= x;
        return res;
    };
    u64 l = 0, r = ub[k] + 1;
    while(l + 1 < r) {
        u64 m = (l + r) / 2;
        (pow(m, k) <= a ? l : r) = m;
    }
    return l;
}