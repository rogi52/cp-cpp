#include "template.hpp"

// sum_{i=0}^{N-1} floor_div(Ai+B, M)
// https://qiita.com/sounansya/items/51b39e0d7bf5cc194081
template < class T > T floor_sum(T n, T m, T a, T b) {
    T ans = 0;
    if(not(0 <= a and a < m)) {
        const T q = floor_div(a, m);
        ans += n * (n - 1) / 2 * q;
        a -= q * m;
    }
    if(not(0 <= b and b < m)) {
        const T q = floor_div(b, m);
        ans += n * q;
        b -= q * m;
    }
    assert(0 <= a and a < m);
    assert(0 <= b and b < m);
    if(a != 0) {
        const T k = (a * (n - 1) + b) / m;
        return ans + k * n - floor_sum(k, a, m, m + a - b - 1);
    }
    return ans;
}