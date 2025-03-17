#include "template.hpp"

namespace kronecker {
template < class T > void mul(vector< T >& x, T a, T b, T c, T d) {
    const int n = ssize(x);
    for(int j = 1; j < n; j <<= 1) FOR(i, n) if((i & j) == 0) {
        T s = a * x[i] + b * x[i + j];
        T t = c * x[i] + d * x[i + j];
        x[i] = s;
        x[i + j] = t;
    }
}
template < class T > void fwht(vector< T >& x, bool inv_) {
    mul< T >(x, 1, 1, 1, -1);
    if(inv_) { const T sz = inv(T(ssize(x))); for(T& e : x) e *= sz; }
}
template < class T > vector< T > xor_conv(vector< T > a, vector< T > b) {
    const int n = ssize(a); fwht(a, false); fwht(b, false);
    vector< T > c(n); FOR(i, n) c[i] = a[i] * b[i]; fwht(c, true); return c;
}
template < class T > vector< T > and_conv(vector< T > a, vector< T > b) {
    const int n = ssize(a); mul< T >(a, 1, 1, 0, 1); mul< T >(b, 1, 1, 0, 1);
    vector< T > c(n); FOR(i, n) c[i] = a[i] * b[i]; mul< T >(c, 1, -1, 0, 1); return c;
}
template < class T > vector< T > or_conv(vector< T > a, vector< T > b) {
    const int n = ssize(a); mul< T >(a, 1, 0, 1, 1); mul< T >(b, 1, 0, 1, 1);
    vector< T > c(n); FOR(i, n) c[i] = a[i] * b[i]; mul< T >(c, 1, 0, -1, 1); return c;
}
}