namespace alg {
template < class T > struct affine {
    T a, b;
    constexpr affine() : a(1), b(0) {}
    constexpr affine(const T& a, const T& b) : a(a), b(b) {}
    constexpr T eval(const T& x) { return a * x + b; }
    constexpr affine composite(const affine& r) const {
        return affine(a * r.a, b * r.a + r.b);
    }
    static constexpr affine e() { return affine(T(1), T(0)); }
};

// f_r(f_l(x))
template < class T > struct affine_lr {
    using F = affine< T >;
    using value_type = affine< T >;
    static constexpr F op(const F& l, const F& r) { return l.composite(r); }
    static constexpr F e() { return F::e(); }
};

// f_l(f_r(x))
template < class T > struct affine_rl {
    using F = affine< T >;
    using value_type = affine< T >;
    static constexpr F op(const F& l, const F& r) { return r.composite(l); }
    static constexpr F e() { return F::e(); }
};
}