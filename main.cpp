#include <iostream>
#include "deque.h"

struct S {
  int x = 0;
  double y = 0.0;
};

int main() {
  Deque<int> d(5, 7);
  Deque<int> d2 = d;
  for (int i = 0; i < 5; ++i) {
    std::cout << d2[i] << ' ';
  } std::cout << '\n';
  for (int i = 0; i < 5; ++i) {
    d[i] = i;
  }
  Deque<int>::iterator it = d.begin() + 1;
  std::cout << *it << '\n';
  Deque<S> s(5, {1, 2.0});
  for (int i = 0; i < 5; ++i) {
    std::cout << '(' << s[i].x << ", " << s[i].y << ") ";
  } std::cout << "\n";
  const Deque<S>& cs = s;
  auto cs_it = *cs.begin();
  auto dd = cs;
  dd.pop_back();
  std::cout << dd.size() << "\n";
  dd.push_back({7, 7});
  std::cout << dd.size() << "\n";
  for (int i = 0; i < 5; ++i) {
    std::cout << '(' << dd[i].x << ", " << dd[i].y << ") ";
  } std::cout << "\n";
  dd.insert(dd.begin(), {3, 4.0});
  std::cout << dd.size() << "\n";
  for (int i = 0; i < 6; ++i) {
    std::cout << '(' << dd[i].x << ", " << dd[i].y << ") ";
  } std::cout << "\n";


  static_assert(std::is_convertible_v<
      decltype(std::declval<Deque<int>>().begin()),
      decltype(std::declval<Deque<int>>().cbegin())
  >, "should be able to construct const iterator from non const iterator");


  std::cout << "////////////////////////////////////////////////\n";

  return 0;
}
