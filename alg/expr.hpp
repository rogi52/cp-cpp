#include "template.hpp"
#include "mod/modint.hpp"
using mint = modint107;

// いろはちゃんコンテスト 2019 Day2 J ライ麦畑で待ちながら (https://atcoder.jp/contests/iroha2019-day2/tasks/iroha2019_day2_j)
// 提出: https://atcoder.jp/contests/iroha2019-day2/submissions/73816685

/*
(S, P) -> (S + A1*P + B1, C1*P + D1)
(S + A1*P + B1 + A2(C1*P+D1) + B2, C2(C1*P+D1) + D2)
= (S + (A1+A2C1)P + (B1+A2D1+B2), (C1C2)P + C2D1+D2)
*/
namespace alg {
struct iroha2019_day2_j {
    struct value_type {
        mint a, b, c, d;
    };
    using S = value_type;
    static S op(const S& x1, const S& x2) {
        return S{
            x1.a + x2.a * x1.c,
            x1.b + x2.a * x1.d + x2.b,
            x1.c * x2.c,
            x2.c * x1.d + x2.d
        };
    }
    static S e() {
        return S{0, 0, 1, 0};
    }
    static S make(char op, mint a) {
        if(op == '+') {
            return {1, 0, 0, a};
        } else {
            return {0, 0, a, 0};
        }
    }
};
}