#pragma once

#include "int/rational.hpp"
#include "template.hpp"

// ================================================================
// Integer / Exact Rational 2D Geometry
//
// point_int<Int> stores exact coordinates in Int.
//
// Predicates (orientation, intersection, parallel, etc.) stay in
// integer arithmetic.
//
// Operations that require division (line intersection, projection,
// reflection, ...) return point_int<rational<LongInt>>.
//
// LongInt must be wide enough for all intermediate products.
// Examples:
//   Int = i64,  LongInt = i128
//   Int = i128, LongInt = boost::multiprecision::cpp_int
// ================================================================

template <class Int = i64> struct point_int {
  Int x, y;

  point_int() : x(0), y(0) {}
  point_int(Int x, Int y) : x(x), y(y) {}

  point_int &operator+=(const point_int &p) {
    x += p.x;
    y += p.y;
    return *this;
  }
  point_int &operator-=(const point_int &p) {
    x -= p.x;
    y -= p.y;
    return *this;
  }
  point_int &operator*=(const Int &r) {
    x *= r;
    y *= r;
    return *this;
  }

  point_int operator+(const point_int &p) const {
    return point_int(*this) += p;
  }
  point_int operator-(const point_int &p) const {
    return point_int(*this) -= p;
  }
  point_int operator*(const Int &r) const { return point_int(*this) *= r; }
  point_int operator-() const { return point_int(-x, -y); }

  bool operator==(const point_int &p) const { return x == p.x and y == p.y; }
  bool operator!=(const point_int &p) const { return not(*this == p); }
  bool operator<(const point_int &p) const {
    return x != p.x ? x < p.x : y < p.y;
  }
};

template <class Int> point_int<Int> operator*(const Int &r, point_int<Int> p) {
  return p *= r;
}

template <class Int> istream &operator>>(istream &is, point_int<Int> &p) {
  return is >> p.x >> p.y;
}

template <class Int> ostream &operator<<(ostream &os, const point_int<Int> &p) {
  return os << p.x << ' ' << p.y;
}

template <class Int> Int dot(const point_int<Int> &a, const point_int<Int> &b) {
  return a.x * b.x + a.y * b.y;
}

template <class Int> Int det(const point_int<Int> &a, const point_int<Int> &b) {
  return a.x * b.y - a.y * b.x;
}

template <class Int> Int norm(const point_int<Int> &p) { return dot(p, p); }

// Wider versions. Prefer these in geometric predicates when Int * Int
// may overflow Int.
template <class LongInt, class Int>
LongInt dot_wide(const point_int<Int> &a, const point_int<Int> &b) {
  return LongInt(a.x) * LongInt(b.x) + LongInt(a.y) * LongInt(b.y);
}

template <class LongInt, class Int>
LongInt det_wide(const point_int<Int> &a, const point_int<Int> &b) {
  return LongInt(a.x) * LongInt(b.y) - LongInt(a.y) * LongInt(b.x);
}

template <class LongInt, class Int> LongInt norm_wide(const point_int<Int> &p) {
  return dot_wide<LongInt>(p, p);
}

// det(b-a, c-a), with subtraction also promoted to LongInt.
template <class LongInt, class Int>
LongInt cross(const point_int<Int> &a, const point_int<Int> &b,
              const point_int<Int> &c) {
  const LongInt bax = LongInt(b.x) - LongInt(a.x);
  const LongInt bay = LongInt(b.y) - LongInt(a.y);
  const LongInt cax = LongInt(c.x) - LongInt(a.x);
  const LongInt cay = LongInt(c.y) - LongInt(a.y);
  return bax * cay - bay * cax;
}

// -1: clockwise, 0: collinear, +1: counter-clockwise.
template <class LongInt, class Int>
int orient(const point_int<Int> &a, const point_int<Int> &b,
           const point_int<Int> &c) {
  const LongInt z = cross<LongInt>(a, b, c);
  return (z > 0) - (z < 0);
}

// ================================================================
// Sort Points by Argument
// https://judge.yosupo.jp/problem/sort_points_by_argument
//
// Int * Int must fit in Int.
// ================================================================

template <class Int>
bool angle_sort_cmp(const point_int<Int> &p, const point_int<Int> &q) {
  const auto half = [&](const point_int<Int> &r) -> int {
    const auto &[x, y] = r;
    if (y < 0 or (y == 0 and x > 0))
      return 0;
    if (x == 0 and y == 0)
      return 1;
    return 2;
  };

  const int hp = half(p), hq = half(q);
  if (hp != hq)
    return hp < hq;
  return det(p, q) > 0;
}

// ================================================================
// Static Convex Hull
// https://judge.yosupo.jp/problem/static_convex_hull
//
// LongInt must hold Int * Int.
// Collinear points on hull edges are removed.
// ================================================================

template <class Int, class LongInt>
vector<point_int<Int>> convex_hull(vector<point_int<Int>> P) {
  using point = point_int<Int>;

  const int n = ssize(P);
  if (n <= 1)
    return P;

  sort(P);
  if (P[0] == P[n - 1])
    return {P[0]};

  auto non_left_turn = [&](const point &a, const point &b,
                           const point &c) -> bool {
    return cross<LongInt>(a, b, c) <= 0;
  };

  vector<point> L;
  L.reserve(n);
  FOR(i, n) {
    while (ssize(L) >= 2 and non_left_turn(L[ssize(L) - 2], L.back(), P[i])) {
      L.pop_back();
    }
    L.push_back(P[i]);
  }

  vector<point> U;
  U.reserve(n);
  REV(i, n) {
    while (ssize(U) >= 2 and non_left_turn(U[ssize(U) - 2], U.back(), P[i])) {
      U.pop_back();
    }
    U.push_back(P[i]);
  }

  vector<point> H;
  H.reserve(ssize(L) + ssize(U) - 2);
  H.insert(H.end(), make_move_iterator(L.begin()),
           make_move_iterator(L.end() - 1));
  H.insert(H.end(), make_move_iterator(U.begin()),
           make_move_iterator(U.end() - 1));
  return H;
}

// ================================================================
// Lines / Segments / Rays
// ================================================================

template <class Int> struct line_int {
  point_int<Int> a, b;

  line_int() = default;
  line_int(const point_int<Int> &a, const point_int<Int> &b) : a(a), b(b) {
    assert(a != b);
  }
};

template <class Int> struct segment_int {
  point_int<Int> a, b;

  segment_int() = default;
  segment_int(const point_int<Int> &a, const point_int<Int> &b) : a(a), b(b) {}
};

template <class Int> struct ray_int {
  point_int<Int> o, d; // o + t d, t >= 0

  ray_int() = default;
  ray_int(const point_int<Int> &o, const point_int<Int> &d) : o(o), d(d) {
    assert(d != point_int<Int>());
  }
};

template <class LongInt, class Int>
bool on_line(const line_int<Int> &l, const point_int<Int> &p) {
  return orient<LongInt>(l.a, l.b, p) == 0;
}

template <class LongInt, class Int>
bool on_segment(const segment_int<Int> &s, const point_int<Int> &p) {
  if (orient<LongInt>(s.a, s.b, p) != 0)
    return false;

  return min(s.a.x, s.b.x) <= p.x and p.x <= max(s.a.x, s.b.x) and
         min(s.a.y, s.b.y) <= p.y and p.y <= max(s.a.y, s.b.y);
}

template <class LongInt, class Int>
bool parallel(const line_int<Int> &l, const line_int<Int> &m) {
  const LongInt lx = LongInt(l.b.x) - LongInt(l.a.x);
  const LongInt ly = LongInt(l.b.y) - LongInt(l.a.y);
  const LongInt mx = LongInt(m.b.x) - LongInt(m.a.x);
  const LongInt my = LongInt(m.b.y) - LongInt(m.a.y);
  return lx * my - ly * mx == 0;
}

template <class LongInt, class Int>
bool parallel(const line_int<Int> &l, const segment_int<Int> &s) {
  if (s.a == s.b)
    return false;
  const LongInt lx = LongInt(l.b.x) - LongInt(l.a.x);
  const LongInt ly = LongInt(l.b.y) - LongInt(l.a.y);
  const LongInt sx = LongInt(s.b.x) - LongInt(s.a.x);
  const LongInt sy = LongInt(s.b.y) - LongInt(s.a.y);
  return lx * sy - ly * sx == 0;
}

template <class LongInt, class Int>
bool parallel(const segment_int<Int> &s, const segment_int<Int> &t) {
  if (s.a == s.b or t.a == t.b)
    return false;
  const LongInt sx = LongInt(s.b.x) - LongInt(s.a.x);
  const LongInt sy = LongInt(s.b.y) - LongInt(s.a.y);
  const LongInt tx = LongInt(t.b.x) - LongInt(t.a.x);
  const LongInt ty = LongInt(t.b.y) - LongInt(t.a.y);
  return sx * ty - sy * tx == 0;
}

template <class LongInt, class Int>
bool orthogonal(const line_int<Int> &l, const line_int<Int> &m) {
  const LongInt lx = LongInt(l.b.x) - LongInt(l.a.x);
  const LongInt ly = LongInt(l.b.y) - LongInt(l.a.y);
  const LongInt mx = LongInt(m.b.x) - LongInt(m.a.x);
  const LongInt my = LongInt(m.b.y) - LongInt(m.a.y);
  return lx * mx + ly * my == 0;
}

template <class LongInt, class Int>
bool same_line(const line_int<Int> &l, const line_int<Int> &m) {
  return parallel<LongInt>(l, m) and orient<LongInt>(l.a, l.b, m.a) == 0;
}

// Infinite line vs infinite line: true also for coincident lines.
template <class LongInt, class Int>
bool intersect_ll(const line_int<Int> &l, const line_int<Int> &m) {
  return not parallel<LongInt>(l, m) or same_line<LongInt>(l, m);
}

// Infinite line vs segment.
template <class LongInt, class Int>
bool intersect_ls(const line_int<Int> &l, const segment_int<Int> &s) {
  if (s.a == s.b)
    return on_line<LongInt>(l, s.a);

  const int x = orient<LongInt>(l.a, l.b, s.a);
  const int y = orient<LongInt>(l.a, l.b, s.b);
  return x == 0 or y == 0 or x != y;
}

// Segment vs segment, including endpoint touching and overlapping.
template <class LongInt, class Int>
bool intersect_ss(const segment_int<Int> &s, const segment_int<Int> &t) {
  const int a = orient<LongInt>(s.a, s.b, t.a);
  const int b = orient<LongInt>(s.a, s.b, t.b);
  const int c = orient<LongInt>(t.a, t.b, s.a);
  const int d = orient<LongInt>(t.a, t.b, s.b);

  if (a == 0 and on_segment<LongInt>(s, t.a))
    return true;
  if (b == 0 and on_segment<LongInt>(s, t.b))
    return true;
  if (c == 0 and on_segment<LongInt>(t, s.a))
    return true;
  if (d == 0 and on_segment<LongInt>(t, s.b))
    return true;

  const bool ab = (a < 0 and b > 0) or (a > 0 and b < 0);
  const bool cd = (c < 0 and d > 0) or (c > 0 and d < 0);
  return ab and cd;
}

// ================================================================
// Rational points
// ================================================================

template <class LongInt> using point_rat = point_int<rational<LongInt>>;

template <class LongInt, class Int>
point_rat<LongInt> to_rational(const point_int<Int> &p) {
  using Q = rational<LongInt>;
  return {Q(LongInt(p.x)), Q(LongInt(p.y))};
}

// ================================================================
// Exact intersections
// ================================================================

// Unique intersection of two non-parallel infinite lines.
template <class LongInt, class Int>
point_rat<LongInt> cross_point_ll(const line_int<Int> &l,
                                  const line_int<Int> &m) {
  using Q = rational<LongInt>;
  using RP = point_rat<LongInt>;

  const LongInt rx = LongInt(l.b.x) - LongInt(l.a.x);
  const LongInt ry = LongInt(l.b.y) - LongInt(l.a.y);
  const LongInt sx = LongInt(m.b.x) - LongInt(m.a.x);
  const LongInt sy = LongInt(m.b.y) - LongInt(m.a.y);

  const LongInt qpx = LongInt(m.a.x) - LongInt(l.a.x);
  const LongInt qpy = LongInt(m.a.y) - LongInt(l.a.y);

  const LongInt den = rx * sy - ry * sx;
  assert(den != 0);

  const LongInt num = qpx * sy - qpy * sx;
  const Q t(num, den);

  const RP a = to_rational<LongInt>(l.a);
  const RP d{Q(rx), Q(ry)};
  return a + d * t;
}

template <class LongInt, class Int>
point_rat<LongInt> cross_point_ls(const line_int<Int> &l,
                                  const segment_int<Int> &s) {
  assert(s.a != s.b);
  assert(intersect_ls<LongInt>(l, s));
  assert(not parallel<LongInt>(l, s));

  return cross_point_ll<LongInt>(l, line_int<Int>(s.a, s.b));
}

template <class LongInt, class Int>
point_rat<LongInt> cross_point_ss(const segment_int<Int> &s,
                                  const segment_int<Int> &t) {
  assert(s.a != s.b and t.a != t.b);
  assert(intersect_ss<LongInt>(s, t));
  assert(not parallel<LongInt>(s, t));

  return cross_point_ll<LongInt>(line_int<Int>(s.a, s.b),
                                 line_int<Int>(t.a, t.b));
}

// Ray o + t*d (t >= 0) vs segment a + s*(b-a) (0 <= s <= 1).
// For a unique non-parallel intersection, returns true and stores s.
template <class LongInt, class Int>
bool intersect_rs(const ray_int<Int> &r, const segment_int<Int> &s,
                  rational<LongInt> &s_pos) {
  using Q = rational<LongInt>;

  const LongInt dx = LongInt(r.d.x);
  const LongInt dy = LongInt(r.d.y);

  const LongInt ex = LongInt(s.b.x) - LongInt(s.a.x);
  const LongInt ey = LongInt(s.b.y) - LongInt(s.a.y);

  LongInt den = dx * ey - dy * ex;
  LongInt ux = LongInt(s.a.x) - LongInt(r.o.x);
  LongInt uy = LongInt(s.a.y) - LongInt(r.o.y);

  LongInt num_t = ux * ey - uy * ex;
  LongInt num_s = ux * dy - uy * dx;

  if (den == 0)
    return false;

  if (den < 0) {
    den = -den;
    num_t = -num_t;
    num_s = -num_s;
  }

  if (num_t < 0)
    return false;
  if (num_s < 0 or den < num_s)
    return false;

  s_pos = Q(num_s, den);
  return true;
}

template <class LongInt, class Int>
bool cross_point_rs(const ray_int<Int> &r, const segment_int<Int> &s,
                    point_rat<LongInt> &out) {
  using Q = rational<LongInt>;

  Q pos;
  if (not intersect_rs<LongInt>(r, s, pos))
    return false;

  const auto a = to_rational<LongInt>(s.a);
  const point_rat<LongInt> e{Q(LongInt(s.b.x) - LongInt(s.a.x)),
                             Q(LongInt(s.b.y) - LongInt(s.a.y))};

  out = a + e * pos;
  return true;
}

// ================================================================
// Projection / Reflection
// ================================================================

template <class LongInt, class Int>
point_rat<LongInt> projection(const line_int<Int> &l, const point_int<Int> &p) {
  using Q = rational<LongInt>;
  using RP = point_rat<LongInt>;

  const LongInt dx = LongInt(l.b.x) - LongInt(l.a.x);
  const LongInt dy = LongInt(l.b.y) - LongInt(l.a.y);

  const LongInt px = LongInt(p.x) - LongInt(l.a.x);
  const LongInt py = LongInt(p.y) - LongInt(l.a.y);

  const LongInt num = px * dx + py * dy;
  const LongInt den = dx * dx + dy * dy;

  assert(den != 0);

  const Q t(num, den);

  const RP a = to_rational<LongInt>(l.a);
  const RP d{Q(dx), Q(dy)};
  return a + d * t;
}

template <class LongInt, class Int>
point_rat<LongInt> reflection(const line_int<Int> &l, const point_int<Int> &p) {
  using Q = rational<LongInt>;

  const auto h = projection<LongInt>(l, p);
  const auto q = to_rational<LongInt>(p);
  return h * Q(2) - q;
}

// ================================================================
// Exact squared distances
// ================================================================

template <class LongInt, class Int>
LongInt distance2_pp(const point_int<Int> &a, const point_int<Int> &b) {
  const LongInt dx = LongInt(a.x) - LongInt(b.x);
  const LongInt dy = LongInt(a.y) - LongInt(b.y);
  return dx * dx + dy * dy;
}

template <class LongInt, class Int>
rational<LongInt> distance2_lp(const line_int<Int> &l,
                               const point_int<Int> &p) {
  const LongInt dx = LongInt(l.b.x) - LongInt(l.a.x);
  const LongInt dy = LongInt(l.b.y) - LongInt(l.a.y);

  const LongInt px = LongInt(p.x) - LongInt(l.a.x);
  const LongInt py = LongInt(p.y) - LongInt(l.a.y);

  const LongInt z = dx * py - dy * px;
  const LongInt den = dx * dx + dy * dy;

  return rational<LongInt>(z * z, den);
}

template <class LongInt, class Int>
rational<LongInt> distance2_sp(const segment_int<Int> &s,
                               const point_int<Int> &p) {
  using Q = rational<LongInt>;

  if (s.a == s.b) {
    return Q(distance2_pp<LongInt>(s.a, p));
  }

  const LongInt dx = LongInt(s.b.x) - LongInt(s.a.x);
  const LongInt dy = LongInt(s.b.y) - LongInt(s.a.y);

  const LongInt px = LongInt(p.x) - LongInt(s.a.x);
  const LongInt py = LongInt(p.y) - LongInt(s.a.y);

  const LongInt t_num = px * dx + py * dy;
  const LongInt t_den = dx * dx + dy * dy;

  if (t_num <= 0) {
    return Q(distance2_pp<LongInt>(s.a, p));
  }
  if (t_den <= t_num) {
    return Q(distance2_pp<LongInt>(s.b, p));
  }

  const LongInt z = dx * py - dy * px;
  return Q(z * z, t_den);
}

template <class LongInt, class Int>
rational<LongInt> distance2_ll(const line_int<Int> &l, const line_int<Int> &m) {
  using Q = rational<LongInt>;

  if (intersect_ll<LongInt>(l, m))
    return Q(0);
  return distance2_lp<LongInt>(l, m.a);
}

template <class LongInt, class Int>
rational<LongInt> distance2_ls(const line_int<Int> &l,
                               const segment_int<Int> &s) {
  using Q = rational<LongInt>;

  if (intersect_ls<LongInt>(l, s))
    return Q(0);

  const Q x = distance2_lp<LongInt>(l, s.a);
  const Q y = distance2_lp<LongInt>(l, s.b);
  return min(x, y);
}

template <class LongInt, class Int>
rational<LongInt> distance2_ss(const segment_int<Int> &s,
                               const segment_int<Int> &t) {
  using Q = rational<LongInt>;

  if (intersect_ss<LongInt>(s, t))
    return Q(0);

  Q ans = distance2_sp<LongInt>(s, t.a);
  ans = min(ans, distance2_sp<LongInt>(s, t.b));
  ans = min(ans, distance2_sp<LongInt>(t, s.a));
  ans = min(ans, distance2_sp<LongInt>(t, s.b));
  return ans;
}
