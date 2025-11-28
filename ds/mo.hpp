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

template < class AddLeft, class AddRight, class Reset, class Snapshot, class Rollback, class Answer >
void rollback_mo(int n, vector<pair<int, int>> qs, AddLeft add_left, AddRight add_right, Reset reset, Snapshot snapshot, Rollback rollback, Answer answer) {
    const int q = ssize(qs);
    if(q == 0) return;
    const int b_num = sqrt(q);
    const int b_sz = ceil_div(n, b_num);
    vector<vector<int>> qid((n - 1) / b_sz + 1);
    FOR(qi, q) {
        auto [L, R] = qs[qi];
        const int L_id = L / b_sz;
        const int R_id = R / b_sz;
        if(L_id == R_id) {
            snapshot();
            FOR(i, L, R) add_right(i);
            answer(qi);
            rollback();
        } else {
            qid[L_id].push_back(qi);
        }
    }

    FOR(L_id, ssize(qid)) {
        vector<int>& I = qid[L_id];
        if(I.empty()) continue;
        sort(I, [&](const int i, const int j) { return qs[i].second < qs[j].second; });
        int Lmax = 0;
        for(int i : I) chmax(Lmax, qs[i].first);
        reset();
        int l = Lmax, r = Lmax;
        for(int i : I) {
            auto [L, R] = qs[i];
            while(r < R) add_right(r++);
            snapshot();
            while(L < l) add_left(--l);
            answer(i);
            rollback();
            l = Lmax;
        }
    }
}