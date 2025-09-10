#include "template.hpp"

// 黄金分割探索 (離散)
// 参考: https://twitter.com/noshi91/status/1399003086362865673
// [min, max]
template <typename T, bool get_min = true>
pair<i64, T> golden_section_search(const function<T(i64)>& f, i64 min,
                                   i64 max) {
  assert(min <= max);
  i64 a = min - 1, x, b;
  {
    i64 s = 1, t = 2;
    while (t < max - min + 2) swap(s += t, t);
    x = a + t - s, b = a + t;
  }
  T fx = f(x), fy;
  while (a + b != 2 * x) {
    i64 y = a + b - x;
    if (max < y || (fy = f(y), get_min ? fx < fy : fx > fy)) {
      b = a;
      a = y;
    } else {
      a = x;
      x = y;
      fx = fy;
    }
  }
  return {x, fx};
}