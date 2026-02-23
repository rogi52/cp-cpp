#include "template.hpp"

// S contains T ?
// T is subsequence of S
template < class Value > bool is_subseq(const vector<Value>& S, const vector<Value>& T) {
    if(T.empty()) return true;
    if(ssize(S) < ssize(T)) return false;
    int j = 0;
    FOR(i, ssize(S)) {
        if(S[i] == T[j]) {
            j += 1;
            if(j == ssize(T)) return true;
        }
    }
    return false;
}