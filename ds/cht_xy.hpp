template < class T > class CHT_xy {
  private:
    using Point = pair< T, T >;
    vector< Point > ps;
    bool is_prepared = false;

    T eval(int i, T x, T y) {
        i %= size(); if(i < 0) i += size();
        auto [a, b] = ps[i];
        return a * x + b * y;
    }

    struct rets {
        Point p; T v;
    };

  public:
    CHT_xy() {}
    CHT_xy(vector< Point > &ps) : ps(ps) {}

    int size() { return ps.size(); }

    void clear() { ps.clear(); is_prepared = false; }

    void add(T a, T b) {
        assert(!is_prepared);
        ps.push_back(Point(a, b));
    }

    void build() {
        assert(!is_prepared);
        is_prepared = true;
        ps = convex_hull< T >(ps).second;
    }

    rets get_min(T x, T y) {
        assert(is_prepared);
        auto f = [&](int i) -> T { return +eval(i, x, y); };
        auto [i, minimum] = find_min< T >(f, size());
        return {ps[i], +minimum};
    }

    rets get_max(T x, T y) {
        assert(is_prepared);
        auto f = [&](int i) -> T { return -eval(i, x, y); };
        auto [i, minimum] = find_min< T >(f, size());
        return {ps[i], -minimum};
    }
};