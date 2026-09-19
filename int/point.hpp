#include "template.hpp"

template < class Int = i64 > struct point_int {
    Int x, y;
    point_int() : x(0), y(0) {}
    point_int(Int x, Int y) : x(x), y(y) {}
    point_int& operator+=(const point_int& p) { x += p.x, y += p.y; return *this; }
    point_int& operator-=(const point_int& p) { x -= p.x, y -= p.y; return *this; }
    point_int& operator*=(const Int& r) { x *= r, y *= r; return *this; }
    point_int operator+(const point_int& p) const { return point_int(*this) += p; }
    point_int operator-(const point_int& p) const { return point_int(*this) -= p; }
    point_int operator*(const Int& r) const { return point_int(*this) *= r; }
    point_int operator-() const { return point_int(-x, -y); }
    bool operator==(const point_int& p) const { return x == p.x and y == p.y; }
    bool operator!=(const point_int& p) const { return x != p.x or  y != p.y; }
    bool operator<(const point_int& p) const { return x != p.x ? x < p.x : y < p.y; }
};

template < class Int > Int dot(const point_int<Int>& a, const point_int<Int>& b) {
    return a.x * b.x + a.y * b.y;
}
template < class Int > Int det(const point_int<Int>& a, const point_int<Int>& b) {
    return a.x * b.y - a.y * b.x;
}
template < class Int > istream& operator>>(istream& is, point_int<Int>& p) {
    return is >> p.x >> p.y;
}
template < class Int > ostream& operator<<(ostream& os, point_int<Int>& p) {
    return os << p.x << ' ' << p.y;
}

// Sort Points by Argument
// https://judge.yosupo.jp/problem/sort_points_by_argument
// Int * Int が Int に収まる必要がある．
template < class Int > bool angle_sort_cmp(const point_int<Int>& p, const point_int<Int>& q) {
    const auto f = [&](const point_int<Int>& r) -> bool {
        const auto &[x, y] = r;
        return y < 0 or y == 0 and x > 0 ? 0 : (x == 0 and y == 0 ? 1 : 2);
    };
    const int fp = f(p), fq = f(q);
    if(fp != fq) return fp < fq;
    return det(p, q) > 0;
}


// Static Convex Hull
// https://judge.yosupo.jp/problem/static_convex_hull
// LongInt: Int*Intが収まる型
template < class Int, class LongInt > vector<point_int<Int>> convex_hull(vector<point_int<Int>> P) {
    using point = point_int<Int>;
    const int n = ssize(P);
    if(n <= 1) return P;
    sort(P);
    if(P[0] == P[n - 1]) return {P[0]};
    auto F = [&](const point& a, const point& b, const point& c) {
        const auto &[ax, ay] = a; const auto &[bx, by] = b; const auto &[cx, cy] = c;
        return LongInt(ax - bx) * (cy - by) >= LongInt(ay - by) * (cx - bx);
    };
    // Lower Hull
    vector<point> L; L.reserve(n);
    FOR(i, n) {
        while(true) {
            const int m = ssize(L);
            if(2 <= m and F(L[m - 2], L[m - 1], P[i])) L.pop_back(); else break;
        }
        L.emplace_back(P[i]);
    }
    // Upper Hull
    vector<point> U; U.reserve(n);
    REV(i, n) {
        while(true) {
            const int m = ssize(U);
            if(2 <= m and F(U[m - 2], U[m - 1], P[i])) U.pop_back(); else break;
        }
        U.emplace_back(P[i]);
    }
    vector<point> H;
    H.reserve(ssize(L) + ssize(U) - 2);
    H.insert(H.end(), make_move_iterator(L.begin()), make_move_iterator(L.end() - 1));
    H.insert(H.end(), make_move_iterator(U.begin()), make_move_iterator(U.end() - 1));
    return H;
}