template < class Sum > struct psum1D {
    int n;
    vector<Sum> s;
    psum1D() : n(0), s(1, Sum()) {}
    template < class Value >
    psum1D(const vector<Value>& a) : n(ssize(a)), s(n + 1, Sum()) {
        FOR(i, n) s[i + 1] = s[i] + static_cast<Sum>(a[i]);
    }
    // [l, r)
    Sum v(int l, int r) const {
        assert(0 <= l and l <= r and r <= n);
        return s[r] - s[l];
    }
    void push_back(const Sum& x) {
        s.push_back(s.back() + x);
        n += 1;
    }
};
template < class Value > struct psum2D {
    int H, W;
    vector<vector<Value>> A;
    bool built;
    psum2D(int H, int W) : H(H), W(W), A(H + 1, vector(W + 1, Value(0))), built(false) {}
    // A[x][y] += v
    void add(int x, int y, Value v) {
        assert(not built);
        assert(0 <= x and x < H);
        assert(0 <= y and y < W);
        A[x + 1][y + 1] += v;
    }
    void build() {
        FOR(x, H) FOR(y, W + 1) A[x + 1][y] += A[x][y];
        FOR(x, H + 1) FOR(y, W) A[x][y + 1] += A[x][y];
        built = true;
    }
    // [xL, xR) * [yL, yR)
    Value sum(int xL, int xR, int yL, int yR) {
        assert(built);
        assert(0 <= xL and xL <= xR and xR <= H);
        assert(0 <= yL and yL <= yR and yR <= W);
        return A[xR][yR] - A[xR][yL] - A[xL][yR] + A[xL][yL];
    }
    Value get(int x, int y) {
        assert(built);
        assert(0 <= x and x < H);
        assert(0 <= y and y < W);
        return sum(x, x + 1, y, y + 1);
    }
};
template < class Value > struct imos2D {
    int H, W;
    vector<vector<Value>> A;
    bool built;
    imos2D(int H, int W) : H(H), W(W), A(H + 1, vector(W + 1, Value(0))), built(false) {}
    void add(int xL, int xR, int yL, int yR, Value v) {
        assert(not built);
        assert(0 <= xL and xL <= xR and xR <= H);
        assert(0 <= yL and yL <= yR and yR <= W);
        A[xL][yL] += v;
        A[xR][yL] -= v;
        A[xL][yR] -= v;
        A[xR][yR] += v;
    }
    void build() {
        assert(not built);
        FOR(i, H + 1) FOR(j, W) A[i][j + 1] += A[i][j];
        FOR(i, H) FOR(j, W + 1) A[i + 1][j] += A[i][j];
        built = true;
    }
    Value get(int x, int y) {
        assert(built);
        assert(0 <= x and x < H);
        assert(0 <= y and y < W);
        return A[x][y];
    }
};