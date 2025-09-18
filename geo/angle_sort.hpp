#include "template.hpp"

// Sort Points by Argument
// https://judge.yosupo.jp/problem/sort_points_by_argument
// LongInt: Int*Intが収まる型
template < class Int, class LongInt > void angle_sort(vector<pair<Int, Int>>& P) {
    // Group 0: [y < 0] or [y == 0 and x > 0]
    // Group 1: (0, 0)
    // Group 2: [y > 0] or [y == 0 and x < 0]
    vector<pair<Int, Int>> G[3];
    for(const auto [x, y] : P) G[y < 0 or y == 0 and x > 0 ? 0 : x == 0 and y == 0 ? 1 : 2].emplace_back(x, y);
    sort(G[0].begin(), G[0].end(), [](const pair<Int, Int>& p, const pair<Int, Int>& q) { return LongInt(p.first) * q.second > LongInt(p.second) * q.first; });
    sort(G[2].begin(), G[2].end(), [](const pair<Int, Int>& p, const pair<Int, Int>& q) { return LongInt(p.first) * q.second > LongInt(p.second) * q.first; });
    move(G[0].begin(), G[0].end(), P.begin());
    move(G[1].begin(), G[1].end(), P.begin() + G[0].size());
    move(G[2].begin(), G[2].end(), P.begin() + G[0].size() + G[1].size());
}