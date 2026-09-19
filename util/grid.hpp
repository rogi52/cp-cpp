struct grid {
    int H, W;
    grid(int H, int W) : H(H), W(W) {}
    static constexpr pair<int, int> dir4[] = {
                  {-1,  0}, 
        { 0, -1},           { 0, +1}, 
                  {+1,  0}
    };
    static constexpr pair<int, int> dir8[] = {
        {-1, -1}, {-1,  0}, {-1, +1},
        { 0, -1},           { 0, +1},
        {+1, -1}, {+1,  0}, {+1, +1}
    };
    bool contains(int i, int j) const {
        return 0 <= i and i < H and 0 <= j and j < W;
    }
    template < class F > 
    void for_each_dir4(int i, int j, const F& f) const {
        for(const auto [di, dj] : dir4) {
            const int ni = i + di, nj = j + dj;
            if(contains(ni, nj)) f(ni, nj);
        }
    }
    template < class F >
    void for_each_dir8(int i, int j, const F& f) const {
        for(const auto [di, dj] : dir8) {
            const int ni = i + di, nj = j + dj;
            if(contains(ni, nj)) f(ni, nj);
        }
    }
};