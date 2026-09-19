#include "template.hpp"

template < class Sequence > Sequence LCS(const Sequence& a, const Sequence& b) {
    const int n = ssize(a);
    const int m = ssize(b);
    vector dp(n + 1, vector(m + 1, int(0)));
    FOR(i, n) FOR(j, m) {
        if(a[i] == b[j]) {
            dp[i + 1][j + 1] = dp[i][j] + 1;
        } else {
            dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);
        }
    }
    Sequence ans;
    int i = n, j = m;
    while(0 != i and 0 != j) {
        if(a[i - 1] == b[j - 1]) {
            ans.push_back(a[i - 1]);
            i--, j--;
        } else if(dp[i - 1][j] >= dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    reverse(ans.begin(), ans.end());
    return ans;
} 