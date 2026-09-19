#include "template.hpp"
#if defined(__x86_64__)
#include <immintrin.h>
#endif

// ABC274-Ex XOR Sum of Arrays
// https://atcoder.jp/contests/abc274/tasks/abc274_h

struct GF2_64 {
    u64 v;
    GF2_64() : v(0) {}
    GF2_64(u64 v) : v(v) {}
    GF2_64& operator+=(const GF2_64& rhs) {
        v ^= rhs.v;
        return *this;
    }
    GF2_64& operator-=(const GF2_64& rhs) {
        v ^= rhs.v;
        return *this;
    }

#if defined(__x86_64__)
    __attribute__((target("pclmul")))
#endif
    GF2_64& operator*=(const GF2_64& rhs) {
#if defined(__x86_64__)
        __m128i a = _mm_cvtsi64_si128(this->v);
        __m128i b = _mm_cvtsi64_si128(rhs.v);
        __m128i prod = _mm_clmulepi64_si128(a, b, 0x00);

        u64 L = _mm_extract_epi64(prod, 0);
        u64 H = _mm_extract_epi64(prod, 1);

        u64 H_low = (H << 4) ^ (H << 3) ^ (H << 1) ^ H;
        u64 H_high = (H >> 60) ^ (H >> 61) ^ (H >> 63);
        u64 H_high_M = (H_high << 4) ^ (H_high << 3) ^ (H_high << 1) ^ H_high;

        this->v = L ^ H_low ^ H_high_M;
#else
        u64 a = v;
        u64 b = rhs.v;
        u64 p = 0;
        for(int i = 0; i < 64; i++) {
            if(b >> i & 1) p ^= a;
            bool carry = a >> 63 & 1;
            a <<= 1;
            if(carry) a ^= 0x1B;
        }
        this->v = p;
#endif
        return *this;
    }

    GF2_64 pow(u64 n) const {
        GF2_64 ans = 1;
        GF2_64 a = *this;
        while(n != 0) {
            if(n & 1) ans *= a;
            a *= a;
            n >>= 1;
        }
        return ans;
    }
    GF2_64 inv() const {
        assert(v != 0 and "Division by Zero");
        return pow(0xFFFFFFFFFFFFFFFEULL);
    }

    GF2_64& operator/=(const GF2_64& rhs) {
        return *this *= rhs.inv();
    }

    GF2_64 sqrt() const {
        return pow(1ULL << 63);
    }

    friend GF2_64 operator+(const GF2_64& lhs, const GF2_64& rhs) { return GF2_64(lhs) += rhs; }
    friend GF2_64 operator-(const GF2_64& lhs, const GF2_64& rhs) { return GF2_64(lhs) -= rhs; }
    friend GF2_64 operator*(const GF2_64& lhs, const GF2_64& rhs) { return GF2_64(lhs) *= rhs; }
    friend GF2_64 operator/(const GF2_64& lhs, const GF2_64& rhs) { return GF2_64(lhs) /= rhs; }

    friend bool operator==(const GF2_64& lhs, const GF2_64& rhs) { return lhs.v == rhs.v; }
    friend bool operator!=(const GF2_64& lhs, const GF2_64& rhs) { return lhs.v != rhs.v; }

    GF2_64 operator+() const { return *this; }
    GF2_64 operator-() const { return *this; }

    friend std::ostream& operator<<(std::ostream& os, const GF2_64& x) { return os << x.v; }
    friend std::istream& operator>>(std::istream& is, GF2_64& x) { return is >> x.v; }
};