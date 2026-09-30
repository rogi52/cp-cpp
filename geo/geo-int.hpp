#include "geo/base.hpp"
#include "int/rational.hpp"

/*
ucup5-1H
https://contest.ucup.ac/submission/3063521
https://contest.ucup.ac/contest/4126/problem/20255
*/

using Int = i128;
using P = point<Int>;
using Q = rational<Int>;
/*
ray: o + td (t >= 0)
segment: u + s(v-u) (0<=s<=1)
交差するなら pos <- s
*/
bool ray_segment(const P &o, const P &d, const P &u, const P &v, Q &pos) {
  const P e = v - u;
  Int den = det(d, e);
  Int num_t = det(u - o, e);
  Int num_s = det(u - o, d);
  if (den == 0)
    return false;
  if (den < 0) {
    den = -den;
    num_t = -num_t;
    num_s = -num_s;
  }
  if (num_t < 0)
    return false;
  if (num_s < 0 or den < num_s)
    return false;
  pos = Q(num_s, den);
  return true;
}