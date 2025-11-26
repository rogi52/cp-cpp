#include "template.hpp"

vector<int> zalgo(const vector<int>& s) {
    const int n = ssize(s);
    if(n == 0) return {};
    vector<int> z(n);
    z[0] = 0;
    for(int i = 1, j = 0; i < n; i++) {
        int& k = z[i];
        k = j + z[j] <= i ? 0 : min(j + z[j] - i, z[i - j]);
        while(i + k < n and s[k] == s[i + k]) k++;
        if(j + z[j] < i + z[i]) j = i;
    }
    z[0] = n;
    return z;
}

vector<int> zalgo(const string& s) {
    const int n = ssize(s);
    if(n == 0) return {};
    vector<int> z(n);
    z[0] = 0;
    for(int i = 1, j = 0; i < n; i++) {
        int& k = z[i];
        k = j + z[j] <= i ? 0 : min(j + z[j] - i, z[i - j]);
        while(i + k < n and s[k] == s[i + k]) k++;
        if(j + z[j] < i + z[i]) j = i;
    }
    z[0] = n;
    return z;
}