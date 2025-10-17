#include "template.hpp"

namespace treap {

u64 xor_shift() {
    static u64 seed = 52;
    seed ^= seed << 7;
    seed ^= seed >> 9;
    return seed;
}

struct node;
using ptr = unique_ptr<node>;
struct node {
    int v;
    ptr l, r;
    u64 p;
    int c;
    i64 s;
    bool rev;
    node(int v) : v(v), l(), r(), p(xor_shift()), c(1), s(v), rev(false) {}
};

int c(const ptr& t) { return t == nullptr ? 0 : t->c; }
i64 s(const ptr& t) { return t == nullptr ? 0 : t->s; }
void rev(ptr& p) { if(p != nullptr) p->rev ^= 1; }
void push(ptr& p) {
    if(p->rev) {
        p->rev = false;
        swap(p->l, p->r);
        rev(p->l); rev(p->r);
    }
}
ptr& update(ptr& t) {
    t->c = c(t->l) + c(t->r) + 1;
    t->s = s(t->l) + s(t->r) + t->v;
    return t;
}
ptr merge(ptr l, ptr r) {
    if(l == nullptr) return r;
    if(r == nullptr) return l;
    if(l->p > r->p) {
        push(l);
        l->r = merge(move(l->r), move(r));
        return move(update(l));
    } else {
        push(r);
        r->l = merge(move(l), move(r->l));
        return move(update(r));
    }
}
// [0, k) [k, n)
pair<ptr, ptr> split(ptr t, int k) {
    if(t == nullptr) return {nullptr, nullptr};
    push(t);
    if(k <= c(t->l)) {
        auto [a, b] = split(move(t->l), k);
        t->l = move(b);
        return {move(a), move(update(t))};
    } else {
        auto [a, b] = split(move(t->r), k - c(t->l) - 1);
        t->r = move(a);
        return {move(update(t)), move(b)};
    }
}

struct range_reverse_range_sum {
    int n;
    ptr root;
    range_reverse_range_sum(const vector<int>& a) : n(ssize(a)), root() {
        for(int x : a) root = merge(move(root), make_unique<node>(x));
    }
    void reverse(int l, int r) {
        auto [ab, c] = split(move(root), r);
        auto [a, b] = split(move(ab), l);
        rev(b);
        ab = merge(move(a), move(b));
        root = merge(move(ab), move(c));
    }
    i64 sum(int l, int r) {
        auto [ab, c] = split(move(root), r);
        auto [a, b] = split(move(ab), l);
        const i64 res = s(b);
        ab = merge(move(a), move(b));
        root = merge(move(ab), move(c));
        return res;
    }
};

}