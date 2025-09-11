#include "template.hpp"

template < class Monoid > struct swag_queue {
    using M = Monoid;
    using T = typename M::value_type;
    struct node {
        T v, s;
        node() : v(M::e()), s(M::e()) {}
        node(const T& v, const T& s) : v(v), s(s) {}
    };
    stack<node> F, B;
    swag_queue() { F.push(node()); B.push(node()); }
    int size() const { return ssize(F) + ssize(B) - 2; }
    bool empty() const { return size() == 0; }
    T prod() const { return M::op(F.top().s, B.top().s); }
    void push(const T& x) { B.push(node(x, M::op(B.top().s, x))); }
    void pop() {
        assert(not empty());
        if(ssize(F) == 1) while(ssize(B) > 1) F.push(node(B.top().v, M::op(B.top().v, F.top().s))), B.pop();
        F.pop();
    }
};

template < class Monoid > struct swag_deque {
    using M = Monoid;
    using T = typename M::value_type;
    struct node {
        T v, s;
        node() : v(M::e()), s(M::e()) {}
        node(const T& v, const T& s) : v(v), s(s) {}
    };
    stack<node> F, B;
    swag_deque() { F.push(node()); B.push(node()); }
    int size() const { return ssize(F) + ssize(B) - 2; }
    bool empty() const { return size() == 0; }
    T prod() const { return M::op(F.top().s, B.top().s); }
    void push_front(const T& x) { push(F, x, 1); }
    void push_back (const T& x) { push(B, x, 0); }
    void pop_front() { assert(not empty()); if(F.size() == 1) amortize(B, F, 1); F.pop(); }
    void pop_back () { assert(not empty()); if(B.size() == 1) amortize(F, B, 0); B.pop(); }

  private:
    void push(stack<node>& st, const T& x, int f) {
        st.push(node(x, f ? M::op(x, st.top().s) : M::op(st.top().s, x)));
    }
    void amortize(stack<node>& from, stack<node>& to, int f) {
        stack<T> st;
        while(ssize(st) < ssize(from) - 2) st.push(from.top().v), from.pop();
        while(ssize(from) > 1) push(to, from.top().v, f), from.pop();
        while(ssize(st) > 0) push(from, st.top(), 1 - f), st.pop();
    }
};



/*
#include "../../src/cp-template.hpp"

template < class monoid >
struct swag_queue {
  public:
    using T = typename monoid::set;
    class node {
      public:
        T value, sum;
        node() : value(monoid::id()), sum(monoid::id()) {}
        node(const T& value, const T& sum) : value(value), sum(sum) {}
    };
    std::stack<node> front_stack, back_stack;

  public:
    swag_queue() { front_stack.push(node()), back_stack.push(node()); }
    int size() const {
        return (int(front_stack.size()) - 1) + (int(back_stack.size()) - 1);
    }
    bool empty() const {
        return size() == 0;
    }
    T fold() const {
        return monoid::op(front_stack.top().sum, back_stack.top().sum);
    }
    void push(const T& x) {
        back_stack.push(node(x, monoid::op(back_stack.top().sum, x)));
    }
    void pop() {
        assert(not empty());
        if(front_stack.size() == 1) {
            while(back_stack.size() > 1) {
                front_stack.push(node(back_stack.top().value, monoid::op(back_stack.top().value, front_stack.top().sum)));
                back_stack.pop();
            }
        }
        front_stack.pop();
    }
};

template < class monoid >
struct swag_deque {
  private:
    using T = typename monoid::set;
    class node {
      public:
        T value, sum;
        node() : value(monoid::id()), sum(monoid::id()) {}
        node(const T& value, const T& sum) : value(value), sum(sum) {}
    };
    std::stack<node> front_stack, back_stack;

  public:
    swag_deque() { front_stack.push(node()), back_stack.push(node()); }
    int size() const { return (int(front_stack.size()) - 1) + (int(back_stack.size()) - 1); }
    bool empty() const { return size() == 0; }

    T fold() const { return monoid::op(front_stack.top().sum, back_stack.top().sum); }
    void push_front(const T& x) { push(front_stack, x, 1); }
    void push_back (const T& x) { push( back_stack, x, 0); }
    void pop_front() { assert(not empty()); if(front_stack.size() == 1) amortize( back_stack, front_stack, 1); front_stack.pop(); }
    void pop_back () { assert(not empty()); if( back_stack.size() == 1) amortize(front_stack,  back_stack, 0);  back_stack.pop(); }

  private:
    void push(std::stack<node>& st, const T& x, int f) {
        st.push(node(x, f ? monoid::op(x, st.top().sum) : monoid::op(st.top().sum, x)));
    }
    void amortize(std::stack<node>& from, std::stack<node>& to, int f) {
        std::stack< T > st;
        while(st.size() < from.size() - 2) st.push(from.top().value), from.pop();
        while(from.size() > 1) push(to, from.top().value, f), from.pop();
        while(not st.empty()) push(from, st.top(), 1 - f), st.pop();
        printer::flush();
    }
};
*/