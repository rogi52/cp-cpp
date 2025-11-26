template < class T > struct point {
    T x, y;
    point() : x(0), y(0) {}
    point(T x, T y) : x(x), y(y) {}
    point(std::pair< T, T > p) : x(p.first), y(p.second) {}
    point& operator+=(const point& p) { x += p.x, y += p.y; return *this; }
    point& operator-=(const point& p) { x -= p.x, y -= p.y; return *this; }
    point& operator*=(const T r) { x *= r, y *= r; return *this; }
    point& operator/=(const T r) { x /= r, y /= r; return *this; }
    point operator+(const point& p) const { return point(*this) += p; }
    point operator-(const point& p) const { return point(*this) -= p; }
    point operator*(const T r) const { return point(*this) *= r; }
    point operator/(const T r) const { return point(*this) /= r; }
    point operator-() const { return {-x, -y}; }
    bool operator==(const point& p) const { return x == p.x and y == p.y; }
    bool operator!=(const point& p) const { return x != p.x or  y != p.y; }
    bool operator<(const point& p) const { return x == p.x ? y < p.y : x < p.x; }
    point< T > rot(f64 theta) {
        static_assert(is_floating_point_v< T >);
        f64 cos_ = std::cos(theta), sin_ = std::sin(theta);
        return {cos_ * x - sin_ * y, sin_ * x + cos_ * y};
    }
};
template < class T > istream& operator>>(istream& is, point< T >& p) { return is >> p.x >> p.y; }
template < class T > ostream& operator<<(ostream& os, point< T >& p) { return os << p.x << " " << p.y; }
template < class T > T dot(const point< T >& a, const point< T >& b) { return a.x * b.x + a.y * b.y; }
template < class T > T det(const point< T >& a, const point< T >& b) { return a.x * b.y - a.y * b.x; }
template < class T > T norm(const point< T >& p) { return p.x * p.x + p.y * p.y; }
template < class T > f64 abs(const point< T >& p) { return std::sqrt(norm(p)); }
template < class T > f64 angle(const point< T >& p) { return std::atan2(p.y, p.x); }
template < class T > int sign(const T x) {
    T e = (is_integral_v< T > ? 1 : 1e-9);
    if(x <= -e) return -1;
    if(x >= +e) return +1;
    return 0;
}
template < class T > bool equals(const T& a, const T& b) { return sign(a - b) == 0; }
template < class T > int ccw(const point< T >& a, point< T > b, point< T > c) {
    b -= a, c -= a;
    if(sign(det(b, c)) == +1) return +1; // counter clockwise
    if(sign(det(b, c)) == -1) return -1; //         clockwise
    if(sign(dot(b, c)) == -1) return +2; // c-a-b
    if(norm(b) < norm(c)) return -2;     // a-b-c
    return 0;                            // a-c-b
}

template < class T > struct line {
    point< T > a, b;
    line() {}
    line(point< T > a, point< T > b) : a(a), b(b) {}
};
template < class T > point< T > projection(const line< T >& l, const point< T >& p) {
    static_assert(is_floating_point_v< T >);
    return l.a + (l.a - l.b) * dot(p - l.a, l.a - l.b) / norm(l.a - l.b);
}
template < class T > point< T > reflection(const line< T >& l, const point< T >& p) {
    static_assert(is_floating_point_v< T >);
    return p + (projection(l, p) - p) * T(2);
}
template < class T > bool orthogonal(const line< T >& a, const line< T >& b) { return equals(dot(a.b - a.a, b.b - b.a), T(0)); }
template < class T > bool parallel  (const line< T >& a, const line< T >& b) { return equals(det(a.b - a.a, b.b - b.a), T(0)); }
template < class T > point< T > cross_point_ll(const line< T >& l, const line< T >& m) {
    static_assert(is_floating_point_v< T >);
    T A = det(l.b - l.a, m.b - m.a);
    T B = det(l.b - l.a, l.b - m.a);
    if(equals(abs(A), T(0)) and equals(abs(B), T(0))) return m.a;
    return m.a + (m.b - m.a) * B / A;
}

template < class T > using segment = line< T >;
template < class T > bool intersect_ss(const segment< T >& s, const segment< T >& t) {
    return ccw(s.a, s.b, t.a) * ccw(s.a, s.b, t.b) <= 0 and ccw(t.a, t.b, s.a) * ccw(t.a, t.b, s.b) <= 0;
}
template < class T > f64 distance_sp(const segment< T >& s, const point< T >& p) {
    static_assert(is_floating_point_v< T >);
    if(sign(abs(s.a - p)) == 0) return 0;
    if(sign(abs(s.b - p)) == 0) return 0;
    if(sign(abs(s.a - s.b)) == 0) return abs(s.a - p);
    point r = projection(s, p);
    if(ccw(s.a, s.b, r) == 0) return abs(r - p);
    return std::min(abs(s.a - p), abs(s.b - p));
}
template < class T > f64 distance_ss(const segment< T >& a, const segment< T >& b) {
    if(intersect_ss(a, b)) return 0;
    return std::min({ distance_sp(a, b.a), distance_sp(a, b.b), distance_sp(b, a.a), distance_sp(b, a.b) });
}

template < class T > using polygon = std::vector< point< T > >;
template < class T > T area2(const polygon< T >& p) {
    T s = 0;
    int n = p.size();
    for(int i = 0; i < n; i++) s += det(p[i], p[(i + 1) % n]);
    return s;
}
template < class T > T area(const polygon< T >& p) { return area2(p) / T(2); }

template < class T > bool is_convex(const polygon< T >& p) {
    int n = p.size();
    for(int i = 0; i < n; i++) if(ccw(p[(i - 1 + n) % n], p[i], p[(i + 1) % n]) == -1) return false;
    return true;
}
template < class T > int contains(const polygon< T >& g, const point< T >& p) {
    int n = g.size();
    bool in = false;
    for(int i = 0; i < n; i++) {
        point a = g[i] - p, b = g[(i + 1) % n] - p;
        if(sign(a.y - b.y) == +1) std::swap(a, b);
        if(sign(a.y) <= 0 and sign(b.y) ==+1 and sign(det(a, b)) == -1) in = !in;
        if(sign(det(a, b)) == 0 and sign(dot(a, b)) <= 0) return 1; // ON
    }
    return in ? 2 : 0;
}
template < class T > polygon< T > convex_cut(const polygon< T >& p, const line< T >& l) {
    int n = p.size();
    polygon< T > res;
    for(int i = 0; i < n; i++) {
        point now = p[i], nxt = p[(i + 1) % n];
        if(ccw(l.a, l.b, now) != -1) res.push_back(now);
        if(ccw(l.a, l.b, now) * ccw(l.a, l.b, nxt) < 0) res.push_back(cross_point_ll(line(now, nxt), l));
    }
    return res;
}
template < class T > polygon< T > convex_hull(polygon< T >& p) {
    int n = p.size(), k = 0;
    if(n <= 2) return p;
    std::sort(p.begin(), p.end());
    polygon< T > ch(n + n);
    for(int i = 0; i < n; ch[k++] = p[i++])
        while(k >= 2 and sign(det(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1])) == -1) k--;
    for(int i = n - 2, t = k + 1; i >= 0; ch[k++] = p[i--])
        while(k >= t and sign(det(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1])) == -1) k--;
    ch.resize(k - 1);
    return ch;
}
template < class T > T diameter2(const polygon< T >& p) {
    static_assert(is_floating_point_v< T >);
    int n = p.size(), is = 0, js = 0;
    for(int i = 1; i < n; i++) {
        if(sign(p[i].y - p[is].y) == +1) is = i;
        if(sign(p[i].y - p[js].y) == -1) js = i;
    }
    T dist_max = norm(p[is] - p[js]);
    int maxi = is, i = is, maxj = js, j = js;
    do {
        if(sign(det(p[(i + 1) % n] - p[i], p[(j + 1) % n] - p[j])) >= 0) j = (j + 1) % n; else i = (i + 1) % n;
        if(norm(p[i] - p[j]) > dist_max) {
            dist_max = norm(p[i] - p[j]);
            maxi = i, maxj = j;
        }
    } while(i != is or j != js);
    return dist_max;
}
template < class T > f64 diameter(const polygon< T >& p) {
    static_assert(is_floating_point_v< T >);
    return std::sqrt(diameter2(p));
}

template < class T > struct circle {
    point< T > p;
    T r;
    circle() = default;
    circle(point< T > p, T r) : p(p), r(r) {}
};
template < class T > istream& operator>>(istream& is, circle< T >& c) { return is >> c.p >> c.r; }
template < class T > int intersect_cc(circle< T > c1, circle< T > c2) {
    if(c1.r < c2.r) std::swap(c1, c2);
    T d = abs(c1.p - c2.p);
    if(sign(c1.r + c2.r - d) == -1) return 4;
    if(equals(c1.r + c2.r, d)) return 3;
    if(sign(c1.r - c2.r - d) == -1) return 2;
    if(equals(c1.r - c2.r, d)) return 1;
    return 0;
}
template < class T > std::pair<point< T >, point< T >> cross_point_cc(const circle< T >& c1, const circle< T >& c2) {
    T d = abs(c1.p - c2.p);
    T a = std::acos((c1.r * c1.r + d * d - c2.r * c2.r) / (2 * c1.r * d));
    T t = angle(c2.p - c1.p);
    point< T > p1 = c1.p + point< T >(std::cos(t + a), std::sin(t + a)) * c1.r;
    point< T > p2 = c1.p + point< T >(std::cos(t - a), std::sin(t - a)) * c1.r;
    return {p1, p2};
}