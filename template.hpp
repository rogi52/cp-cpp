#pragma once
#include <bits/stdc++.h>
using namespace std;

using i32 = int;
using i64 = long long;
using i128 = __int128;
using u32 = unsigned int;
using u64 = unsigned long long;
using u128 = unsigned __int128;
using f32 = double;
using f64 = long double;

#define DMP(x) cout << "[" << __LINE__ << "]" << " " << #x << ":" << " " << x << endl;

#define FOR1(n)          for(int _ =  0 , n_ = (n); _ < n_; _++)
#define FOR2(i, n)       for(int i =  0 , n_ = (n); i < n_; i++)
#define FOR3(i, s, t)    for(int i = (s), t_ = (t); i < t_; i++)
#define FOR4(i, s, t, d) for(int i = (s), t_ = (t), d_ = (d); i < t_; i += d_)
#define OVERLOAD4(a, b, c, d, e, ...) e
#define FOR(...) OVERLOAD4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)

#define REV1(n)          for(int _ = (n) - 1; _ >=  0 ; _--)
#define REV2(i, n)       for(int i = (n) - 1; i >=  0 ; i--)
#define REV3(i, s, t)    for(int i = (t) - 1, s_ = (s); i >= s_; i--)
#define REV4(i, s, t, d) for(int i = (t) - 1, s_ = (s), d_ = (d); i >= s_; i -= d_)
#define OVERLOAD3(a, b, c, d, ...) d
#define REV(...) OVERLOAD4(__VA_ARGS__, REV4, REV3, REV2, REV1)(__VA_ARGS__)

#define FOR_SUBSET(T, S) for(int S_ = (S), T = S_; T >= 0; T = (T == 0 ? -1 : (T - 1) & S_))

#define MULTI for(int testcase_ = in(), testcase = 0; testcase < testcase_; testcase++) [&]

template < class T > using heap_max = priority_queue< T, vector< T >, less< T > >;
template < class T > using heap_min = priority_queue< T, vector< T >, greater< T >>;

template < class T, class U > bool chmin(T& a, const U& b) { return a > b ? a = b, 1 : 0; }
template < class T, class U > bool chmax(T& a, const U& b) { return a < b ? a = b, 1 : 0; }

i64 floor_div(const i64 n, const i64 d) { assert(d != 0); return n / d - ((n ^ d) <  0 && n % d != 0); }
i64  ceil_div(const i64 n, const i64 d) { assert(d != 0); return n / d + ((n ^ d) >= 0 && n % d != 0); }

template < class T, class F > T bin_search(T ok, T ng, const F& check) { while((ok > ng ? ok - ng : ng - ok) > 1) { T mid = (ok + ng) / 2; (check(mid) ? ok : ng) = mid; } return ok; }
template < class T, class F > T bin_search_real(T ok, T ng, const F& check, int step = 100) { FOR(step) { T mid = (ok + ng) / 2; (check(mid) ? ok : ng) = mid; } return ok; }

template < class T, class U > T accum(const vector< U >& a) { return accumulate(a.begin(), a.end(), T(0)); }
template < class T > void sort(vector< T >& a) { sort(a.begin(), a.end()); }
template < class T > void rsort(vector< T >& a) { sort(a.rbegin(), a.rend()); }
template < class T > void reverse(vector< T >& a) { reverse(a.begin(), a.end()); }
void sort(string& s) { sort(s.begin(), s.end()); }
void rsort(string& s) { sort(s.rbegin(), s.rend()); }
void reverse(string& s) { reverse(s.begin(), s.end()); }
template < class T, class Cmp > void sort(vector< T >& a, Cmp cmp) { sort(a.begin(), a.end(), cmp); }
template < class T > int LB(const vector< T >& a, T x) { return distance(a.begin(), lower_bound(a.begin(), a.end(), x)); }
template < class T > int UB(const vector< T >& a, T x) { return distance(a.begin(), upper_bound(a.begin(), a.end(), x)); }
template < class T > void unique(vector< T >& a) { sort(a.begin(), a.end()); a.erase(unique(a.begin(), a.end()), a.end()); }
vector<int> iota(int n) { vector<int> a(n); iota(a.begin(), a.end(), 0); return a; }

istream& operator >> (istream& is, i128& x) {
    string s; is >> s;
    int m = (s[0] == '-');
    x = 0;
    FOR(i, m, ssize(s)) x = x * 10 + (s[i] - '0');
    if(m) x *= -1;
    return is;
}
ostream& operator << (ostream& os, const i128& x) {
    if(x == 0) return os << '0';
    i128 y = x; if(y < 0) { os << '-'; y *= -1; }
    vector<int> ny;
    while(y) ny.push_back(y % 10), y /= 10;
    REV(i, ssize(ny)) os << ny[i];
    return os;
}
namespace scan {
struct x0 { template < class T > operator T() { T x; cin >> x; return x; } };
struct x1 { int n; x1(int n) : n(n) {} template < class T > operator vector< T >() { vector< T > a(n); for(T& x : a) cin >> x; return a; } };
struct x2 { int h, w; x2(int h, int w) : h(h), w(w) {} template < class T > operator vector< vector< T > >() { vector m(h, vector< T >(w)); for(vector< T >& a : m) for(T& x : a) cin >> x; return m; } };
struct cppio { cppio() { cin.tie(0); ios::sync_with_stdio(0); } } cppio_instance;
}
scan::x0 in() { return scan::x0(); }
scan::x1 in(int n) { return scan::x1(n); }
scan::x2 in(int h, int w) { return scan::x2(h, w); }

template < class T > ostream& operator << (ostream& os, const vector< T >& a) {
    const int n = a.size();
    FOR(i, n) { os << a[i]; if(i + 1 != n) os << ' '; }
    return os;
}
template < class T > int print_n(const vector< T >& a) { for(const T& x : a) cout << x << '\n'; return 0; }
int print() { cout << '\n'; return 0; }
template < class Head, class... Tail > int print(Head&& h, Tail&&... t) { cout << h; if(sizeof...(Tail)) cout << ' '; return print(forward<Tail>(t)...); }
namespace printer {
    void prec(int n) { cout << fixed << setprecision(n); }
    void flush() { cout.flush(); }
}

vector<int>& operator ++ (vector<int>& a) { for(auto& e : a) e++; return a; }
vector<int>& operator -- (vector<int>& a) { for(auto& e : a) e--; return a; }
vector<int>  operator ++ (vector<int>& a, int) { vector<int> b = a; ++a; return b; }
vector<int>  operator -- (vector<int>& a, int) { vector<int> b = a; --a; return b; }

template < class T > vector<pair< T, int>> RLE(const vector< T >& a) { vector<pair< T, int>> v; for(const T& x : a) { if(not v.empty() and v.back().first == x) v.back().second++; else v.emplace_back(x, 1); } return v; }
vector<pair<char, int>> RLE(const string& s) { vector<pair<char, int>> v; for(const char& c : s) { if(not v.empty() and v.back().first == c) v.back().second++; else v.emplace_back(c, 1); } return v; }
template < class String, class Same > vector<String> RLE(const String& a, const Same same) { vector<String> v; for(const auto& x : a) { if(not v.empty() and same(v.back().back(), x)) v.back().push_back(x); else v.push_back({x}); } return v; }

int YESNO(bool yes) { return print(yes ? "YES" : "NO"); }
int YesNo(bool yes) { return print(yes ? "Yes" : "No"); }
int Yes() { return print("Yes"); }
int No() { return print("No"); }

constexpr i32 INF32 = 1e9;
constexpr i64 INF64 = 1e18;
template < class T > constexpr T infty = 0;
template <> constexpr int infty<int> = 1e9;
template <> constexpr int infty<u32> = 1e9;
template <> constexpr i64 infty<i64> = 1e18;
template <> constexpr u64 infty<u64> = 1e18;

namespace bit {
int pop(int x) { return popcount<u32>(x); }
int pop(u32 x) { return popcount<u32>(x); }
int pop(i64 x) { return popcount<u64>(x); }
int pop(u64 x) { return popcount<u64>(x); }
int parity(int x) { return __builtin_parity(x); }
int parity(u32 x) { return __builtin_parity(x); }
int parity(i64 x) { return __builtin_parityll(x); }
int parity(u64 x) { return __builtin_parityll(x); }
int sgn(int x) { return parity(x) ? -1 : +1; }
int sgn(u32 x) { return parity(x) ? -1 : +1; }
int sgn(i64 x) { return parity(x) ? -1 : +1; }
int sgn(u64 x) { return parity(x) ? -1 : +1; }
int top(int x) { return x == 0 ? -1 : 31 - __builtin_clz(x); }
int top(u32 x) { return x == 0 ? -1 : 31 - __builtin_clz(x); }
int top(i64 x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int top(u64 x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int low(int x) { return x == 0 ? -1 : __builtin_ctz(x); }
int low(u32 x) { return x == 0 ? -1 : __builtin_ctz(x); }
int low(i64 x) { return x == 0 ? -1 : __builtin_ctzll(x); }
int low(u64 x) { return x == 0 ? -1 : __builtin_ctzll(x); }
int ceil(int x) { return bit_ceil<u32>(x); }
i64 ceil(i64 x) { return bit_ceil<u64>(x); }
int floor(int x) { return bit_floor<u32>(x); }
i64 floor(i64 x) { return bit_floor<u64>(x); }
}

// (-1)^n
int parity_sign(int n) { return n % 2 == 0 ? +1 : -1; }

// template < class T > pair< T, int > min(const vector< T >& a) { auto itr = min_element(a.begin(), a.end()); return {*itr, itr - a.begin()}; }
// template < class T > pair< T, int > max(const vector< T >& a) { auto itr = max_element(a.begin(), a.end()); return {*itr, itr - a.begin()}; }

template < class Key, class Value >
struct key_value {
    Key key;
    Value value;
};
template < class Value > key_value<int, Value> min(const vector<Value>& a) {
    assert(1 <= ssize(a));
    auto itr = min_element(a.begin(), a.end());
    return {static_cast<int>(distance(a.begin(), itr)), *itr};
}
template < class Value > key_value<int, Value> max(const vector<Value>& a) {
    assert(1 <= ssize(a));
    auto itr = max_element(a.begin(), a.end());
    return {static_cast<int>(distance(a.begin(), itr)), *itr};
}

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

#include "rnd.hpp"

namespace r52 {
int abs(int x) { return x >= 0 ? x : -x; }
i64 abs(i64 x) { return x >= 0 ? x : -x; }
i128 abs(i128 x) { return x >= 0 ? x : -x; }
}
