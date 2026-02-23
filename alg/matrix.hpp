#include "template.hpp"

// 固定長の行列
namespace alg {
template < class T, int N >  struct matrix {
    using value_type = array<array< T, N >, N>;
    using S = value_type;
    static S op(const S& a, const S& b) {
        S c = {};
        FOR(k, N) FOR(i, N) FOR(j, N) c[i][j] += a[i][k] * b[k][j];
        return c;
    }
    static S e() {
        S I = {};
        FOR(i, N) I[i][i] = 1;
        return I;
    };
};
}