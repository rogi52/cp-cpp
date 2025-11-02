#include "template.hpp"
#include "ds/set64.hpp"

template < class Key, class Value > struct DIU {
    static constexpr Key MIN = -infty<Key>;
    static constexpr Key MAX = +infty<Key>;
    Value NONE;
    // 区間の個数
    int size;
    // 区間の長さの合計
    Key len;
    map<Key, Value> mp;

    DIU(Value NONE) : NONE(NONE), size(0), len(0) {
        mp[MIN] = mp[MAX] = NONE;
    }

    // x in [l, r) -> v
    tuple<Key, Key, Value> get(Key x, bool erase = false) {
        auto itr = mp.upper_bound(x);
        auto [r, vr] = *itr;
        auto [l, vl] = *prev(itr);
        if(vl != NONE and erase) {
            size -= 1;
            len -= r - l;
            mp[l] = NONE;
            merge(l);
            merge(r);
        }
        return {l, r, vl};
    }

    template < class F > void for_each_range(Key l, Key r, const F& f, bool erase = false) {
        assert(MIN <= l and l <= r and r <= MAX);
        if(erase) {
            auto p = prev(mp.upper_bound(l));
            if(p->first < l) {
                mp[l] = p->second;
                if(mp[l] != NONE) size += 1;
            }
            p = mp.lower_bound(r);
            if(r < p->first) {
                Value v = prev(p)->second;
                mp[r] = v;
                if(v != NONE) size += 1;
            }
            p = mp.lower_bound(l);
            while(p->first < r) {
                auto q = next(p);
                Value v = p->second;
                f(p->first, q->first, v);
                if (v != NONE) size -= 1, len -= q->first - p->first;
                p = mp.erase(p);
            }
            mp[l] = NONE;
        } else {
            auto itr = prev(mp.upper_bound(l));
            while(itr->first < r) {
                auto n_itr = next(itr);
                f(max(itr->first, l), min(n_itr->first, r), itr->second);
                itr = n_itr;
            }
        }
    }

    void set(Key l, Key r, Value v) {
        assert(l <= r);
        if(l == r) return;
        for_each_range(l, r, [](Key l, Key r, Value v){}, true);
        mp[l] = v;
        if(v != NONE) size += 1, len += r - l;
        merge(l);
        merge(r);
    }

    template < class F > void for_all_range(const F& f, bool erase = false) {
        for_each_range(MIN, MAX, f, erase);
    }

  private:
    void merge(Key p) {
        if(p == MIN or p == MAX) return;
        auto itp = mp.lower_bound(p);
        assert(itp->first == p);
        auto itq = prev(itp);
        if(itp->second == itq->second) {
            if(itp->second != NONE) size -= 1;
            mp.erase(itp);
        }
    }
};

/*
CF771(Div.2)-E
https://codeforces.com/contest/1638/problem/E

// 全体を -1 で初期化する
DIU<int, int> I(-1);
// [0, n) -> 0 の区間を追加する (初め a の色は 0)
I.set(0, n, 0);
// sum[c] := 色 c に足された値
vector<i64> sum(n, 0);
// a[i] = seg[i] + sum[a[i]->color] となるように管理する
lazytree<alg::range_add_range_min<i64>> seg(n, 0);
auto Q1 = [&] {
    // 区間 [l, r) の色を c に変更する
    I.for_each_range(l, r, [&](int l, int r, int c) { seg.o(l, r, sum[c]); }, true);
    seg.o(l, r, -sum[c]);
    I.set(l, r, c);
};
auto Q2 = [&]{
    // 色が c の要素 a[i] に x を足す
    sum[c] += x;
};
auto Q3 = [&]{
    // a[i] を出力する
    auto [l, r, c] = I.get(i);
    print(sum[c] + seg.v(i));
};

オフラインの実装: https://codeforces.com/contest/1638/submission/347045852
*/

template < class Value > struct DIU_o {
    const int MIN, MAX;
    Value NONE;
    // 区間の個数
    int size;
    // 区間の長さの合計
    int len;
    vector<Value> dat;
    set64 st;

    DIU_o(int N, Value NONE) : MIN(0), MAX(N), NONE(NONE), size(0), len(0), dat(N, NONE), st(N) {
        st.insert(0);
    }

    // x in [l, r) -> v
    tuple<int, int, Value> get(int x, bool erase = false) {
        int l = st.prev(x);
        int r = st.next(x + 1);
        Value v = dat[l];
        if(v != NONE and erase) {
            size -= 1;
            len -= r - l;
            dat[l] = NONE;
            merge(l);
            merge(r);
        }
        return {l, r, v};
    }

    template < class F > void for_each_range(int l, int r, const F& f, bool erase = false) {
        assert(MIN <= l and l <= r and r <= MAX);
        if(l == r) return;
        if(erase) {
            int p = st.prev(l);
            if(p < l) {
                st.insert(l);
                dat[l] = dat[p];
                if(dat[l] != NONE) size += 1;
            }
            p = st.next(r);
            if(r < p) {
                dat[r] = dat[st.prev(r)];
                st.insert(r);
                if(dat[r] != NONE) size += 1;
            }
            p = l;
            while(p < r) {
                int q = st.next(p + 1);
                Value v = dat[p];
                f(p, q, v);
                if(dat[p] != NONE) size -= 1, len -= q - p;
                st.erase(p);
                p = q;
            }
            st.insert(l);
            dat[l] = NONE;
        } else {
            int i = st.prev(l);
            while(i < r) {
                int ni = st.next(i);
                f(max(i, l), min(ni, r), dat[i]);
                i = ni;
            }
        }
    }

    void set(int l, int r, Value v) {
        if(l == r) return;
        for_each_range(l, r, [](int l, int r, Value v){}, true);
        st.insert(l);
        dat[l] = v;
        if(v != NONE) size += 1, len += r - l;
        merge(l);
        merge(r);
    }

    template < class F > void for_all_range(const F& f, bool erase = false) {
        for_each_range(MIN, MAX, f, erase);
    }

  private:
    void merge(int p) {
        if(p <= MIN or MAX <= p) return;
        int q = st.prev(p - 1);
        if(dat[p] == dat[q]) {
            if(dat[p] != NONE) size -= 1;
            st.erase(p);
        }
    }
};