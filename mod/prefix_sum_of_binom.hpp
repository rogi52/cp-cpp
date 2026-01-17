#pragma once
#include "mod/binom.hpp"

// sum_{k=0 to m} binom{n}{k}
template < class mint > struct prefix_sum_of_binom {
    mint x = 1;
    int m = 0, n = 0;
    const mint inv2 = mint(1) / 2;
    mint inc_m() { return x += comb::comb<mint>(n, ++m); }
    mint dec_m() { return x -= comb::comb<mint>(n, m--); }
    mint inc_n() { return x += x - comb::comb<mint>(n++, m); }
    mint dec_n() { return x = (x + comb::comb<mint>(--n, m)) * inv2; }
    mint get() const { return x; }
};