#include "template.hpp"

// g[S] = sum_{T \subseteq S} f(T)
template < class T > void subset_zeta(vector< T >& f) {
    const int N = ssize(f);
    assert(1 <= N);
    const int n = bit::top(N);
    assert(N == (1 << n));
    FOR(i, n) FOR(S, 1 << n) if(S >> i & 1) f[S] += f[S ^ (1 << i)];
}
// f[S] = sum_{T \subseteq S} (-1)^{|S \setminus T|} g[T]
template < class T > void subset_mobius(vector< T >& g) {
    const int N = ssize(g);
    assert(1 <= N);
    const int n = bit::top(N);
    assert(N == (1 << n));
    FOR(i, n) FOR(S, 1 << n) if(S >> i & 1) g[S] -= g[S ^ (1 << i)];
}

// g[S] = sum_{S \supseteq T} f(T)
template < class T > void supset_zeta(vector< T >& f) {
    const int N = ssize(f);
    assert(1 <= N);
    const int n = bit::top(N);
    assert(N == (1 << n));
    FOR(i, n) FOR(S, 1 << n) if(not(S >> i & 1)) f[S] += f[S ^ (1 << i)];
}
// f[S] = sum_{S \supseteq T} (-1)^{|T \setminus S|} g[T]
template < class T > void supset_mobius(vector< T >& g) {
    const int N = ssize(g);
    assert(1 <= N);
    const int n = bit::top(N);
    assert(N == (1 << n));
    FOR(i, n) FOR(S, 1 << n) if(not(S >> i & 1)) g[S] -= g[S ^ (1 << i)];
}

template < class T > void and_fzt(vector< T >& f) { supset_zeta(f); }
template < class T > void and_fzt_inv(vector< T >& f) { supset_mobius(f); }
// h[S] = sum_{S = X ∩ Y} f[X] * g[Y]
template < class T > vector< T > and_convolution(vector< T > f, vector< T > g) {
    assert(ssize(f) == ssize(g));
    const int N = ssize(f);
    and_fzt(f);
    and_fzt(g);
    vector< T > h(N);
    FOR(S, N) h[S] = f[S] * g[S];
    and_fzt_inv(h);
    return h;
}

template < class T > void or_fzt(vector< T >& f) { subset_zeta(f); }
template < class T > void or_fzt_inv(vector< T >& f) { subset_mobius(f); }
// h[S] = sum_{S = X ∪ Y} f[X] * g[Y]
template < class T > vector< T > or_convolution(vector< T > f, vector< T > g) {
    assert(ssize(f) == ssize(g));
    const int N = ssize(f);
    or_fzt(f);
    or_fzt(g);
    vector< T > h(N);
    FOR(S, N) h[S] = f[S] * g[S];
    or_fzt_inv(h);
    return h;
}

// PAST18-M
// https://atcoder.jp/contests/past18-open/tasks/past18_m
// 提出: https://atcoder.jp/contests/past18-open/submissions/73015013
// subset_zeta した配列 g から，1 点だけを O(2^n) 時間で復元する
template < class T > T subset_point(const vector< T >& g, int S) {
    T ans = 0;
    FOR_SUBSET(X, S) ans += parity_sign(bit::parity(S ^ X)) * g[X];
    return ans;
}
template < class T > T supset_point(const vector< T >& g, int S) {
    T ans = 0;
    FOR_SUBSET(Y, (ssize(g) - 1) ^ S) {
        const int X = S | Y;
        ans += parity_sign(bit::parity(Y)) * g[X];
    }
    return ans;
}

// [x^S] f^0, f^1, f^2, ..., f^N を O(N 2^N) 時間で求める
template < class T > vector< T > or_enum_pow_coef(vector< T > f, int S) {
    const int N = ssize(f);
    assert(1 <= N);
    const int n = bit::top(N);
    assert(N == (1 << n));
    vector< T > now(N, 1), ans(n + 1);
    ans[0] = subset_point(now, S);
    or_fzt(f);
    FOR(k, 1, n + 1) {
        FOR(S, N) now[S] *= f[S];
        ans[k] = subset_point(now, S);
    }
    return ans;
}

/*
Subset Convolution
*/
template < class T, int n_max >
vector<array< T, n_max+1 >> rank_zeta(const vector< T >& f) {
    const int N = ssize(f);
    assert(1 <= N);
    const int n = bit::top(N);
    assert(N == (1 << n));
    assert(n <= n_max);
    vector<array< T, n_max+1 >> g(N);
    FOR(s, N) g[s][bit::pop(s)] = f[s];
    FOR(i, n) FOR(s, N) if(s >> i & 1) {
        const int t = s ^ (1 << i);
        FOR(d, n + 1) g[s][d] += g[t][d];
    }
    return g;
}
template < class T, int n_max >
vector< T > rank_mobius(vector<array< T, n_max+1 >> g) {
    const int N = ssize(g);
    assert(1 <= N);
    const int n = bit::top(N);
    assert(N == (1 << n));
    assert(n <= n_max);
    FOR(i, n) FOR(s, N) if(s >> i & 1) {
        const int t = s ^ (1 << i);
        FOR(d, n + 1) g[s][d] -= g[t][d];
    }
    vector< T > f(N);
    FOR(s, N) f[s] = g[s][bit::pop(s)];
    return f;
}
template < class T, int n_max >
vector< T > subset_convolution(const vector< T >& a, const vector< T >& b) {
    assert(ssize(a) == ssize(b));
    const int N = ssize(a);
    assert(1 <= N);
    const int n = bit::top(N);
    assert(N == (1 << n));
    auto ra = rank_zeta< T, n_max >(a);
    auto rb = rank_zeta< T, n_max >(b);
    vector<array< T, n_max+1 >> rc(N);
    FOR(s, N) FOR(k, n + 1) FOR(i, k + 1) {
        const int j = k - i;
        rc[s][k] += ra[s][i] * rb[s][j];
    }
    return rank_mobius< T, n_max >(rc);
}
template < class T, int n_max > T rank_point(const vector<array< T, n_max+1 >>& g, int S) {
    T ans = 0;
    const int k = bit::pop(S);
    FOR_SUBSET(X, S) ans += parity_sign(bit::parity(S ^ X)) * g[X][k];
    return ans;
}
template < class T, int n_max > vector< T > sps_exp(vector< T > f) {
    const int N = ssize(f);
    assert(1 <= N);
    const int n = bit::top(N);
    assert(N == (1 << n));
    assert(f[0] == T(0));
    vector< T > g(N);
    g[0] = T(1);
    FOR(i, n) {
        vector a(f.begin() + (1 << i), f.begin() + (1 << (i + 1)));
        vector b(g.begin(), g.begin() + (1 << i));
        vector c = subset_convolution< T, n_max >(a, b);
        copy(c.begin(), c.end(), g.begin() + (1 << i));
    }
    return g;
}

template < class T, int n_max >
vector< T > transposed_subset_convolution(vector< T > s, vector< T > x) {
    reverse(x);
    x = subset_convolution< T, n_max >(x, s);
    reverse(x);
    return x;
}

// PAST18-M
// https://atcoder.jp/contests/past18-open/submissions/73022329
// [x^S] s^1, s^2, ..., s^N を列挙したい
// -> x[S] = 1 かつ他は 0 として egf 合成． O(N^2 2^N)
template < class T, int n_max >
vector< T > transposed_sps_composition_egf(vector< T >& s, vector< T > x) {
    assert(ssize(s) == ssize(x));
    const int N = ssize(s);
    assert(1 <= N);
    const int n = bit::top(N);
    assert(N == (1 << n));
    assert(s[0] == T(0));
    vector< T > y(n + 1);
    y[0] = x[0];
    FOR(i, n) {
        vector< T > nx(1 << (n - 1 - i));
        FOR(j, n - i) {
            vector a(s.begin() + (1 << j), s.begin() + (2 << j));
            vector b(x.begin() + (1 << j), x.begin() + (2 << j));
            b = transposed_subset_convolution< T, n_max >(a, b);
            FOR(k, ssize(b)) nx[k] += b[k];
        }
        x = move(nx);
        y[i + 1] = x[0];
    }
    return y;
}