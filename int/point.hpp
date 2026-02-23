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