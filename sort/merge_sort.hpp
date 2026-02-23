#include "template.hpp"

namespace merge_sort {

template < class T > vector< T > merge(const vector< T >& a, const vector< T >& b) {
    const int n = ssize(a);
    const int m = ssize(b);
    vector< T > c;
    c.reserve(n + m);
    int i = 0, j = 0;
    while(i < n or j < m) {
        if(i < n and j < m) {
            if(a[i] < b[j]) {
                c.push_back(a[i++]);
            } else {
                c.push_back(b[j++]);
            }
        } else if(i < n) {
            c.push_back(a[i++]);
        } else {
            c.push_back(b[j++]);
        }
    }
    return c;
}

template < class T > void sort(vector< T >& a) {
    const int n = ssize(a);
    for(int len = 1; len < n; len <<= 1) {
        for(int L = 0; L < n; L += len + len) {
            const int M = min(L + len, n);
            const int R = min(L + len + len, n);
            if(M == n) break;
            vector< T > res = merge(
                vector<T>{a.begin() + L, a.begin() + M},
                vector<T>{a.begin() + M, a.begin() + R}
            );
            FOR(i, ssize(res)) a[L + i] = res[i];
        }
    }
}

// AWC0002-E (https://atcoder.jp/contests/awc0002/tasks/awc0002_e)
// [N]の全ての部分集合の和を，ソートした状態で， O(2^N) 時間で求める．
template < class T > vector< T > subset_sum(const vector< T >& a) {
    const int n = ssize(a);
    vector< T > ans = {T(0)};
    for(const T x : a) {
        vector< T > ans2 = ans;
        for(T& e : ans2) e += x;
        ans = merge(ans, ans2);
    }
    return ans;
}

}