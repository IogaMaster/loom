#include <cassert>
#include <iostream>
#include <loom/utils/sparse_set.hpp>

// NOLINTBEGIN

using loom::utils::SparseSet;

int main() {
  SparseSet<float> set;

  // --- Insert tests ---
  set.insert(0, 1.5f);
  set.insert(2, 2.5f);
  set.insert(5, 5.5f);

  // Access inserted values
  auto a = set.get(0);
  auto b = set.get(2);
  auto c = set.get(5);

  assert(a.has_value());
  assert(b.has_value());
  assert(c.has_value());
  assert(**a == 1.5f);
  assert(**b == 2.5f);
  assert(**c == 5.5f);

  std::cout << "✅ Insert and get\n";

  // --- get_ref() edit test ---
  if (auto ref = set.get_ref(2)) {
    ref->get() = 42.0f;
  }
  assert(**set.get(2) == 42.0f);
  std::cout << "✅ get_ref modification\n";

  // --- Invalid get ---
  auto missing = set.get(999);
  assert(!missing.has_value());
  std::cout << "✅ Invalid get \n";

  // --- Remove test ---
  set.remove(2);
  assert(!set.get(2).has_value());
  std::cout << "✅ Remove\n";

  // --- Clear test ---
  set.clear();
  assert(!set.get(0).has_value());
  assert(!set.get(5).has_value());
  std::cout << "✅ Clear \n";

  std::cout << "✅ test_sparse_set passed!\n";
  return 0;
}
// NOLINTEND
