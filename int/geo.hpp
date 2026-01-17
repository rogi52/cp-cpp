#include "template.hpp"

template < class Int > struct point_int {
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