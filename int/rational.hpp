#include "template.hpp"

template < class Int > struct rational {
    using Q = rational;
    Int sign, upper, lower;
    rational() : sign(+1), upper(0), lower(1) {}
    rational(Int sign, Int upper, Int lower) : sign(sign), upper(upper), lower(lower) { reduce(); }
    rational(Int upper, Int lower) {
        if(upper == 0 and lower == 0) cerr << "0/0" << endl, exit(1);
        else if(upper == 0) sign = s(lower), this->upper = 0, this->lower = 1;
        else if(lower == 0) sign = s(upper), this->upper = 1, this->lower = 0;
        else sign = s(upper) * s(lower), this->upper = s(upper) * upper, this->lower = s(lower) * lower, reduce();
    }
    rational(Int n) : rational(n, 1) {}
    static Q raw(Int sign, Int upper, Int lower) { Q q; q.sign = sign; q.upper = upper; q.lower = lower; return q; }
    Q operator - () const { return raw(+1, 0, 1) - *this; }
    Q operator + (const Q& r) { return Q((sign * upper) * r.lower + lower * (r.sign * r.upper), lower * r.lower); }
    Q operator - (const Q& r) { return Q((sign * upper) * r.lower - lower * (r.sign * r.upper), lower * r.lower); }
    Q operator * (const Q& r) { return Q(sign * r.sign, upper * r.upper, lower * r.lower); }
    Q operator / (const Q& r) { return Q(sign * r.sign, upper * r.lower, lower * r.upper); }
    bool operator == (const Q& r) { return sign == r.sign and upper == r.upper and lower == r.lower; }
    bool operator != (const Q& r) { return sign != r.sign or  upper != r.upper or  lower != r.lower; }
    bool operator <  (const Q& r) { return sign != r.sign ? sign == -1 : (sign == -1) ^ (upper * r.lower < r.upper * lower); }
    Q& operator += (const Q& r) { return *this = *this + r; }
    Q& operator -= (const Q& r) { return *this = *this - r; }
    Q& operator *= (const Q& r) { return *this = *this * r; }
    Q& operator /= (const Q& r) { return *this = *this / r; }
  private:
    Int s(Int x) { if(x == 0) return 0; return x > 0 ? +1 : -1; }
    Int gcd(Int a, Int b) { if(b == 0) return a; return gcd(b, a % b); }
    void reduce() { Int g = gcd(upper, lower); upper /= g, lower /= g; }
};
template < class Int > ostream& operator << (ostream& os, const rational<Int>& q) { os << (q.sign * q.upper) << "/" << q.lower; return os; }