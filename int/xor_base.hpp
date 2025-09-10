#include "template.hpp"

template < class Int > vector<Int> xor_base_of_array(const vector<Int>& A) {
    vector<Int> B;
    for(Int a : A) {
        for(const Int b : B) chmin(a, a ^ b);
        if(a != 0) B.push_back(a);
    }
    return B;
}

template < class Int > struct xor_base {
    vector<Int> B;
    xor_base() {}
    void insert(Int x) {
        for(const Int b : B) chmin(x, x ^ b);
        if(x != 0) B.push_back(x);
    }
    bool contains(Int x) {
        for(const Int b : B) chmin(x, x ^ b);
        return x == 0;
    }
};