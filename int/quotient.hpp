// x in [L, R), floor(N/x) == Q
template < class T, class F >
void for_each_quotient(T N, const F& f) {
    for(T L = 1; L <= N;) {
        T Q = N / L, R = N / Q + 1;
        f(L, R, Q);
        L = R;
    }
}