#include "template.hpp"
#include "mod/ntt.hpp"
#include "mod/fps.hpp"

// template < class mint > mint one_coeff(vector<mint> P, vector<mint> Q, u64 N) {
//     const int d = Q.size() - 1;
//     assert(P.size() <= d);

//     auto even = [&](vector<mint> F) {
//         const int n = F.size();
//         vector<mint> G;
//         G.reserve((n + 1) / 2);
//         for(int i = 0; i < n; i += 2) G.push_back(F[i]);
//         return G;
//     };

//     auto odd  = [&](vector<mint> F) {
//         const int n = F.size();
//         vector<mint> G;
//         G.reserve(n / 2);
//         for(int i = 1; i < n; i += 2) G.push_back(F[i]);
//         return G;
//     };

//     for(; N > 0; N /= 2) {
//         vector<mint> Qm = Q;
//         const int n = Qm.size();
//         for(int i = 1; i < n; i += 2) Qm[i] = - Qm[i];
//         vector<mint> U = ntt::conv(P, Qm);
//         tie(P, Q) = make_pair(N % 2 == 0 ? even(move(U)) : odd(move(U)), even(ntt::conv(Q, move(Qm))));
//     }
//     return P[0] / Q[0];
// }

template < class mint > mint one_coeff(vector<mint> P, vector<mint> Q, u64 N) {
    const int d = ssize(Q) - 1;
    assert(ssize(P) <= d + 1);
    int K = 1;
    while(K <= 2 * d) K *= 2;
    vector<mint> nP(K), nQ(K);
    vector<mint> next_P(d + 1), next_Q(d + 1);
    for(; N != 0; N >>= 1) {
        P.resize(K, 0);
        Q.resize(K, 0);
        fmt(P);
        fmt(Q);
        FOR(i, K) {
            const int j = i ^ 1;
            nP[i] = P[i] * Q[j];
            nQ[i] = Q[i] * Q[j];
        }
        fmt_inv(nP);
        fmt_inv(nQ);
        int offset = N % 2;
        FOR(i, d + 1) {
            next_P[i] = nP[i * 2 + offset];
            next_Q[i] = nQ[i * 2];
        }
        FOR(i, d + 1) {
            P[i] = next_P[i];
            Q[i] = next_Q[i];
        }
        P.resize(d + 1);
        Q.resize(d + 1);
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