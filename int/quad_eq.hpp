#include "template.hpp"
#include "nt/kth_root_int.hpp"

// いろはちゃんコンテスト Day1 J ヌクレオチド (https://atcoder.jp/contests/iroha2019-day1/tasks/iroha2019_day1_j)
// 提出: https://atcoder.jp/contests/iroha2019-day1/submissions/73805451
vector<i64> quad_eq_int(i64 a, i64 b, i64 c) {
    assert(a != 0);
    const i64 D = b * b - 4 * a * c;
    if(D < 0) {
        return {};
    } else if(D == 0) {
        if((-b) % (2*a) != 0) return {};
        return {(-b) / (2*a)};
    } else {
        const i64 sqD = kth_root(D, 2);
        if(sqD * sqD != D) return {};
        vector<i64> r;
        if((-b + sqD) % (2*a) == 0) r.push_back((-b + sqD) / (2*a));
        if((-b - sqD) % (2*a) == 0) r.push_back((-b - sqD) / (2*a));
        return r;
    }
}