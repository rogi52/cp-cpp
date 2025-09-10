#include "template.hpp"

/*

Negative Index Vector

int N = 5;
vector<int> _vec(N + 1 + N); // [-N, +N]
iota(_vec.begin(), _vec.end(), -N);
auto vec = _vec.begin() + N;

添字 i によるアクセスはできる
範囲forしたい時は for(auto e : _vec) とする必要がある
for(int i = -N; i <= +N; i++) cout << vec[i] << " \n"[i == N];

*/