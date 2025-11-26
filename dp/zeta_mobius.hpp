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