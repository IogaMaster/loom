#pragma once

#include <cstdint>
#include <limits>
#include <optional>
#include <vector>

namespace loom::utils {

class ISparseSet {
public:
  virtual ~ISparseSet() = default;
  virtual auto remove(uint64_t idx) -> void = 0;
};

/**
 * @brief Sparse data structure that maps indexes into a densely packed vector
 * of data, for cache locality
 */
template <typename T> class SparseSet : public ISparseSet {
  static constexpr size_t tombstone = std::numeric_limits<size_t>::max();

  std::vector<uint64_t> m_sparse;
  std::vector<T> m_dense;
  std::vector<uint64_t> m_reverse;

  auto getDenseIndex(uint64_t idx) -> uint64_t {
    if (idx >= m_sparse.size()) {
      return tombstone;
    }

    auto dIndex = m_sparse[idx];

    if (dIndex >= m_dense.size()) {
      return tombstone;
    }

    return dIndex;
  };

  auto setSparseIndex(uint64_t idx, uint64_t d_idx) -> void {
    if (m_sparse.size() - 1 < idx) {
      return;
    }
    m_sparse[idx] = d_idx;
  };

public:
  SparseSet() = default;

  /**
   * @brief Insert data into the sparse set at idx
   *
   * @param idx The sparse index
   * @param data The data to add to dense
   */
  auto insert(uint64_t idx, T data) -> void {
    m_dense.push_back(data);
    m_reverse.push_back(idx);

    if (idx >= m_sparse.size()) {
      m_sparse.resize(idx + 1, tombstone);
    }
    m_sparse[idx] = m_dense.size() - 1;
  };

  /**
   * @brief Remove data from the sparse set
   *
   * @param idx Sparse index to remove data of
   */
  auto remove(uint64_t idx) -> void {
    uint64_t deletedIndex = getDenseIndex(idx);

    if (m_dense.empty() || deletedIndex == tombstone) {
      return;
    }

    setSparseIndex(m_reverse.back(), deletedIndex);
    setSparseIndex(idx, tombstone);

    std::swap(m_dense.back(), m_dense[deletedIndex]);
    std::swap(m_reverse.back(), m_reverse[deletedIndex]);

    m_dense.pop_back();
    m_reverse.pop_back();
  };

  /**
   * @brief Get data at index
   *
   * @param idx Index to get
   * @return Optional pointer to data
   */
  auto get(uint64_t idx) -> std::optional<T *> {
    uint64_t index = getDenseIndex(idx);
    if (index != tombstone) {
      return std::optional<T *>{&m_dense[index]};
    }
    return std::nullopt;
  }

  /**
   * @brief Get data at index
   *
   * @param idx Index to get
   * @return Optional Reference to data
   */
  auto get_ref(uint64_t idx) -> std::optional<std::reference_wrapper<T>> {
    uint64_t index = getDenseIndex(idx);
    if (index != tombstone) {
      return std::ref(m_dense[index]);
    }
    return std::nullopt;
  }

  auto get_dense() const -> std::vector<T> & { return m_dense; }
  auto size() -> size_t { return m_dense.size(); }

  /**
   * @brief Clear and deallocate the set
   */
  auto clear() -> void {
    m_dense.clear();
    m_sparse.clear();
    m_reverse.clear();

    m_dense.shrink_to_fit();
    m_sparse.shrink_to_fit();
    m_reverse.shrink_to_fit();
  };
};

}; // namespace loom::utils
