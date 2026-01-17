#include "template.hpp"
#include "mod/ntt.hpp"

template < class mint > mint one_coeff(vector<mint> P, vector<mint> Q, u64 N) {
    const int d = Q.size() - 1;
    assert(P.size() <= d);

    auto even = [&](vector<mint> F) {
        const int n = F.size();
        vector<mint> G;
        G.reserve((n + 1) / 2);
        for(int i = 0; i < n; i += 2) G.push_back(F[i]);
        return G;
    };

    auto odd  = [&](vector<mint> F) {
        const int n = F.size();
        vector<mint> G;
        G.reserve(n / 2);
        for(int i = 1; i < n; i += 2) G.push_back(F[i]);
        return G;
    };

    for(; N > 0; N /= 2) {
        vector<mint> Qm = Q;
        const int n = Qm.size();
        for(int i = 1; i < n; i += 2) Qm[i] = - Qm[i];
        vector<mint> U = ntt::conv(P, Qm);
        tie(P, Q) = make_pair(N % 2 == 0 ? even(move(U)) : odd(move(U)), even(ntt::conv(Q, move(Qm))));
    }
    return P[0] / Q[0];
}

template < class mint > mint one_term(const vector<mint>& a, const vector<mint>& c, u64 k) {
    const int d = c.size();
    vector<mint> Q(d + 1);
    Q[0] = 1;
    for(int i = 0; i < d; i++) Q[i + 1] = - c[i];
    vector<mint> P = ntt::conv(a, Q);
    P.resize(d);
    return one_coeff(P, Q, k);
}