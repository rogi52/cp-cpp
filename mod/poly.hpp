#include "template.hpp"
#include "mod/binom.hpp"
#include "mod/ntt.hpp"

namespace poly {

template < class mint > vector<mint> taylor_shift(const vector<mint>& f, const mint c) {
    const int n = ssize(f);
    vector<mint> p(n);
    FOR(i, n) p[i] = f[i] * comb::fact<mint>(i);

    vector<mint> q(n);
    mint c_pow = 1;
    FOR(i, n) {
        q[i] = c_pow * comb::fact_inv<mint>(i);
        c_pow *= c;
    }
    reverse(q);
    vector<mint> r = ntt::conv<mint>(p, q);
    r = {r.begin() + (n - 1), r.end()};
    FOR(i, n) r[i] *= comb::fact_inv<mint>(i);
    return r;
}

} // namespace poly