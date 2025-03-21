#include "template.hpp"

// 一般の数列を三分探索すると極小点が求まるとは限らない
// https://noshi91.hatenablog.com/entry/2022/03/13/213114

// 極小点 f(i-1) >= f(i) <= f(i+1) を求める
// f: [0, n) -> T
template < class T, class Func > pair< int, T > find_local_min(Func f, int n) {
    int a = 0, b = (n + 1) / 2, c = n + 1;
    while(c - a > 2) {
        int l = (a + b) / 2, r = (b + c + 1) / 2;
        if(f(l) < f(b))
            tie(a, b, c) = make_tuple(a, l, b);
        else if(f(b) > f(r))
            tie(a, b, c) = make_tuple(b, r, c);
        else
            tie(a, b, c) = make_tuple(l, b, r);
    }
    return {b, f(b)};
};

// f: [0, p) -> T, f(x) = f(x + p)
template < class T, class Func > pair< int, T > find_min(Func f, int p) {
    return find_local_min< T, Func >(f, 2 * p - 1);
}