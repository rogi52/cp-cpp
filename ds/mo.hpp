#include "template.hpp"

template < class AddLeft, class AddRight, class DelLeft, class DelRight, class Answer >
void mo_algo(int n, vector<pair<int, int>> qs, AddLeft add_left, AddRight add_right, DelLeft del_left, DelRight del_right, Answer answer) {
    const int q = ssize(qs);
    const int B = max<int>(1, n / max<double>(1.0, sqrt(q * 2.0 / 3.0)));
    vector<int> ord = iota(q);
    sort(ord.begin(), ord.end(), [&](int i, int j) {
        auto [Li, Ri] = qs[i];
        auto [Lj, Rj] = qs[j];
        if(Li / B != Lj / B) return Li < Lj;
        return Li / B & 1 ? Ri < Rj : Ri > Rj;
    });
    int nL = 0, nR = 0;
    for(int i : ord) {
        auto [L, R] = qs[i];
        while(nL > L) add_left (--nL);
        while(nR < R) add_right(nR++);
        while(nL < L) del_left (nL++);
        while(nR > R) del_right(--nR);
        answer(i);
    }
}
