#include <cassert>
#include <iostream>

auto main() -> int {
  static_assert(1 == 1); // NOLINT
  std::cout << "✅ test_trivial passed!" << '\n';
  return 0;
}
