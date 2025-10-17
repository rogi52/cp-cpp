#include "template.hpp"

template < class Int > vector<pair<Int, pair<int, int>>> manhattan_edges(vector<pair<Int, Int>> P) {
    const int n = ssize(P);
    vector<pair<Int, pair<int, int>>> E; E.reserve(4 * n);
    vector<int> I = iota(n);
    FOR(2) {
        FOR(2) {
            sort(I.begin(), I.end(), [&](const int& i, const int& j) { return P[i].first + P[i].second < P[j].first + P[j].second; });
            map<Int, int> mp;
            for(const int i : I) {
                const Int key = -P[i].second;
                for(auto itr = mp.lower_bound(key); itr != mp.end(); itr = mp.erase(itr)) {
                    const int j = itr->second;
                    const Int dx = P[i].first - P[j].first, dy = P[i].second - P[j].second;
                    if(dy <= dx) E.push_back({abs(dx) + abs(dy), {i, j}}); else break;
                }
                mp[key] = i;
            }
            for(auto &[x, y] : P) swap(x, y);
        }
        for(auto &[x, y] : P) x = -x;
    }
    ranges::sort(E, {}, &pair<Int, pair<int, int>>::first);
    return E;
}