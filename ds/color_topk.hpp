#include "template.hpp"

// CodeFormula-D 映画の連続視聴 (https://atcoder.jp/contests/code-formula-2014-final/tasks/code_formula_2014_final_d)
template < class Value, class Color, int K, class Compare >
struct color_topk {
    using Data = pair<Value, Color>;
    array<Data, K> data;
    Compare cmp;
    color_topk(Value init_v, Color init_c) {
        data.fill({init_v, init_c});
    }
    void push(Value v, Color c) {
        int pos = -1;
        FOR(i, K) {
            if(data[i].second == c) {
                pos = i;
                break;
            }
        }
        if(pos != -1) {
            if(!cmp(v, data[pos].first)) return;
            while(pos > 0 and cmp(v, data[pos - 1].first)) {
                data[pos] = data[pos - 1];
                pos--;
            }
            data[pos] = {v, c};
        } else {
            if(!cmp(v, data[K - 1].first)) return;
            pos = K - 1;
            while(pos > 0 && cmp(v, data[pos - 1].first)) {
                data[pos] = data[pos - 1];
                pos--;
            }
            data[pos] = {v, c};
        }
    }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
};
template <class Value, class Color, int K>
using color_maxk = color_topk<Value, Color, K, greater<Value>>;
template <class Value, class Color, int K>
using color_mink = color_topk<Value, Color, K, less<Value>>;
