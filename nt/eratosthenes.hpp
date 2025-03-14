#include "template.hpp"

struct eratosthenes {
    int N;
    vector<int> isprime, primes, spf, mobius;
    eratosthenes(int N) : N(N), isprime(N + 1, 1), spf(N + 1, -1), mobius(N + 1, 1) {
        isprime[1] = 0;
        spf[1] = 1;
        for(int n = 2; n <= N; n++) if(isprime[n]) {
            primes.push_back(n);
            spf[n] = n;
            mobius[n] = -1;
            for(int m = n + n; m <= N; m += n) {
                isprime[m] = 0;
                if(spf[m] == -1) spf[m] = n;
                mobius[m] = ((m / n) % n == 0 ? 0 : -mobius[m]);
            }
        }
    }
    vector<pair<int, int>> factor(int n) {
        assert(1 <= n and n <= N);
        vector<pair<int, int>> pe;
        while(n > 1) {
            int p = spf[n], e = 0;
            while(spf[n] == p) n /= p, e++;
            pe.emplace_back(p, e);
        }
        return pe;
    }
    vector<int> divisor(int n) {
        vector<int> ds = {1};
        for(auto [p, e] : factor(n)) {
            FOR(i, ssize(ds)) {
                int x = 1;
                FOR(j, e) x *= p, ds.push_back(ds[i] * x);
            }
        }
        return ds;
    }
    int euler_phi(int n) {
        assert(1 <= n and n <= N);
        int phi = n;
        for(auto [p, e] : factor(n)) phi /= p, phi *= p - 1;
        return phi;
    }
    // a[n] <- sum_{n divides m} a[m]
    template < class T > void m_zeta(vector< T >& a) {
        const int n = ssize(a) - 1;
        assert(1 <= n and n <= N);
        for(int p : primes) { if(p <= n) for(int k = n / p; k >  0; k--) a[k] += a[k * p]; else break; }
    }
    template < class T > void m_mobius(vector< T > &a) {
        const int n = ssize(a) - 1;
        assert(1 <= n and n <= N);
        for(int p : primes) { if(p <= n) for(int k = 1; k * p <= n; k++) a[k] -= a[k * p]; else break; }
    }
    // a[n] <- sum_{d divides n} a[d]
    template < class T > void d_zeta(vector< T >& a) {
        const int n = ssize(a) - 1;
        assert(1 <= n and n <= N);
        for(int p : primes) { if(p <= n) for(int k = 1; k * p <= n; k++) a[k * p] += a[k]; else break; }
    }
    template < class T > void d_mobius(vector< T >& a) {
        const int n = ssize(a) - 1;
        assert(1 <= n and n <= N);
        for(int p : primes) { if(p <= n) for(int k = n / p; k >  0; k--) a[k * p] -= a[k]; else break; }
    }
    template < class T > vector< T > gcd_conv(vector< T > a, vector< T > b) {
        const int n = max(ssize(a), ssize(b));
        a.resize(n), b.resize(n);
        m_zeta(a); m_zeta(b);
        vector< T > c(n); FOR(i, n) c[i] = a[i] * b[i];
        m_mobius(c);
        return c;
    }
    template < class T > vector< T > lcm_conv(vector< T > a, vector< T > b) {
        const int n = max(ssize(a), ssize(b));
        a.resize(n, 0), b.resize(n, 0);
        d_zeta(a), d_zeta(b);
        vector< T > c(n); FOR(i, n) c[i] = a[i] * b[i];
        d_mobius(c);
        return c;
    }
};