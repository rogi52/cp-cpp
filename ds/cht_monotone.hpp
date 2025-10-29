#include "template.hpp"


/*
追加する直線 ax+b について，a は広義単調増加 (or 広義単調減少) と仮定する．

直線の追加: amotized O(1)
単調なクエリ: amotized O(1)
任意のクエリ: O(log (直線の個数))

クエリ x は任意
*/
template < class T > class CHT_add_monotone {

    struct Line {
        T a, b;
        Line(T a, T b) : a(a), b(b) {}
        T eval(T x) { return a * x + b; }
    };

    deque< Line > H;
    T sgn;

    int sgn_of(T x) { return x == 0 ? 0 : (x < 0 ? -1 : 1); }

    using D = long double;
    bool check(Line &a, Line &b, Line &c) {
        if(a.b == b.b || b.b == c.b) return sgn_of(b.a - a.a) * sgn_of(c.b - b.b) >= sgn_of(c.a - b.a) * sgn_of(b.b - a.b);
        return D(b.a - a.a) * sgn_of(c.b - b.b) / D(abs(b.b - a.b)) >= D(c.a - b.a) * sgn_of(b.b - a.b) / D(abs(c.b - b.b));
    }

  public:
    CHT_add_monotone(bool is_min = true) { sgn = is_min ? +1 : -1; }

    bool empty() { return H.empty(); }

    void clear() { H.clear(); }

    void add(T a, T b) {
        a *= sgn, b *= sgn;
        Line l(a, b);
        if(empty()) {
            H.push_front(l);
        } else if(H.front().a <= a) {
            if(H.front().a == a) {
                if(H.front().b <= b) return;
                H.pop_front();
            }
            while(H.size() >= 2 && check(l, H.front(), H[1])) H.pop_front();
            H.push_front(l);
        } else {
            assert(a <= H.back().a);
            if(H.back().a == a) {
                if(H.back().b <= b) return;
                H.pop_back();
            }
            while(H.size() >= 2 && check(H[H.size() - 2], H.back(), l)) H.pop_back();
            H.push_back(l);
        }
    }

    T query(T x) {
        assert(!empty());
        int l = -1, r = H.size() - 1;
        while(l + 1 < r) {
            int m = (l + r) / 2;
            (H[m].eval(x) >= H[m + 1].eval(x) ? l : r) = m;
        }
        return sgn * H[r].eval(x);
    }

    T query_monotone_inc(T x) {
        assert(!empty());
        while(H.size() >= 2 && H.front().eval(x) >= H[1].eval(x)) H.pop_front();
        return sgn * H.front().eval(x);
    }

    T query_monotone_dec(T x) {
        assert(!empty());
        while(H.size() >= 2 && H.back().eval(x) >= H[H.size() - 2].eval(x)) H.pop_back();
        return sgn * H.back().eval(x);
    }
};