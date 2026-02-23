#include "template.hpp"

template < class Key, class Value, int LG >
struct hash_map {
    static constexpr int N = 1 << LG;
    static constexpr u32 mask = N - 1;
    static constexpr int shift = 64 - LG;
    const u64 salt = []() {
        u64 s = rnd::i<u64>(1, u64(-1));
        return s | 1;
    }();
    vector<Key> key;
    vector<Value> value;
    bitset<N> used;
    hash_map() : key(N), value(N) {}
    inline u32 hash(u64 x) const {
        return (x * salt) >> shift;
    }
    void set(Key k, Value v) {
        u32 i = hash(k);
        while(used[i]) {
            if(key[i] == k) {
                value[i] = v;
                return;
            }
            i = (i + 1) & mask;
        }
        key[i] = k;
        value[i] = v;
        used[i] = 1;
    }
    Value v(Key k, Value default_value = Value(0)) const {
        u32 i = hash(k);
        while(used[i]) {
            if(key[i] == k) return value[i];
            i = (i + 1) & mask;
        }
        return default_value;
    }
    bool contains(Key k) const {
        u32 i = hash(k);
        while(used[i]) {
            if(key[i] == k) return true;
            i = (i + 1) & mask;
        }
        return false;
    }
    void clear() {
        used.reset();
    }
};