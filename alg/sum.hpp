namespace alg {
template < class T > struct sum {
    using value_type = T;
    static constexpr T op(const T& a, const T& b) { return a + b; }
    static constexpr T e() { return T(0); }
    static constexpr T inv(const T& a) { return -a; }
    static constexpr bool comm() { return true; }
};
}