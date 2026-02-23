#include "template.hpp"

/*
長さ N の数列 A がある．
要素 A[i] をコスト C[i] で削除できる．
コストの総和 K 以下のとき，辞書順最小の A を求める．

A が順列の場合，C は任意で解ける．
C が均一の (つまり K は削除できる個数を表す) 場合，A は任意で解ける．

A が任意かつ C が任意だと解けない
[> の撃墜ケース]
A = [1, 1, 1]
C = [2, 1, 2]
K = 1

[>= の撃墜ケース]
A = [7, 7, 6]
C = [6, 2, 5]
K = 7

*/

// P: 順列
// C: 削除するコスト
// K: コストの総和
template < class Char, class Cost >
vector<Char> subseq_erase_cost(const vector<Char>& P, const vector<Cost>& C, Cost K) {
    assert(ssize(P) == ssize(C));
    const int N = ssize(P);
    vector<int> I;
    FOR(i, N) {
        while(not I.empty() and P[I.back()] > P[i] and C[I.back()] <= K) K -= C[I.back()], I.pop_back();
        I.push_back(i);
    }
    while(not I.empty() and C[I.back()] <= K) K -= C[I.back()], I.pop_back();
    const int m = ssize(I);
    vector<Char> ans(m);
    FOR(i, m) ans[i] = P[I[i]];
    return ans;
}

// A: 順列である必要はない
// K: 削除できる個数
template < class Char >
vector<int> subseq_erase(const vector<Char>& A, int K) {
    return subseq_erase_cost(A, vector(ssize(A), 1), K);
}

// template < class Char, class Cost >
// vector<int> naive(const vector<Char>& A, const vector<Cost>& C, Cost K) {
//     const int N = ssize(A);
//     vector<int> argMax;
//     vector<Char> Max;
//     bool init = false;
//     FOR(S, 1 << N) {
//         Cost cost = 0;
//         vector<int> idx;
//         vector<Char> value;
//         FOR(i, N) {
//             if(S >> i & 1) {
//                 idx.push_back(i);
//                 value.push_back(A[i]);
//             } else {
//                 cost += C[i];
//             }
//         }
//         if(cost <= K) {
//             if(not init or value < Max) {
//                 argMax = idx;
//                 Max = value;
//                 init = true;
//             }
//         }
//     }
//     return argMax;
// }