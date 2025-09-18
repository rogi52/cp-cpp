#include "template.hpp"

// Static Convex Hull
// https://judge.yosupo.jp/problem/static_convex_hull
// LongInt: Int*Intが収まる型
template < class Int, class LongInt > vector<pair<Int, Int>> convex_hull(vector<pair<Int, Int>> P) {
    const int n = ssize(P);
    if(n <= 1) return P;
    sort(P.begin(), P.end());
    if(P[0] == P[n - 1]) return {P[0]};
    auto F = [&](const pair<Int, Int>& a, const pair<Int, Int>& b, const pair<Int, Int>& c) {
        const auto &[ax, ay] = a; const auto &[bx, by] = b; const auto &[cx, cy] = c;
        return LongInt(ax - bx) * (cy - by) >= LongInt(ay - by) * (cx - bx);
    };
    // Lower Hull
    vector<pair<Int, Int>> L; L.reserve(n);
    FOR(i, n) {
        while(true) {
            const int m = ssize(L);
            if(2 <= m and F(L[m - 2], L[m - 1], P[i])) L.pop_back(); else break;
        }
        L.emplace_back(P[i]);
    }
    // Upper Hull
    vector<pair<Int, Int>> U; U.reserve(n);
    REV(i, n) {
        while(true) {
            const int m = ssize(U);
            if(2 <= m and F(U[m - 2], U[m - 1], P[i])) U.pop_back(); else break;
        }
        U.emplace_back(P[i]);
    }
    vector<pair<Int, Int>> H;
    H.reserve(ssize(L) + ssize(U) - 2);
    H.insert(H.end(), make_move_iterator(L.begin()), make_move_iterator(L.end() - 1));
    H.insert(H.end(), make_move_iterator(U.begin()), make_move_iterator(U.end() - 1));
    return H;
}