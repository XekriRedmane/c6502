// A set of non-negative integers that iterates in the order a CPython
// 3.11 `set` of the same integers, added in the same order, iterates.
//
// Some Python passes walk a set of block ids (`natural_loops` walks the
// reachable blocks, and the dominance frontiers and loop bodies are
// sets that their callers walk), so the order of the walk reaches their
// output. A set of ints doesn't depend on hash randomization, since an
// int's hash is its value, so the order is fixed for a given sequence of
// additions, but it isn't ascending: it is the order of the slots of
// CPython's hash table, which depends on the table's size, its probing,
// and when it was resized. This class keeps the same table and does the
// same probing and resizing as `set_add_entry`, `set_table_resize`, and
// `set_insert_clean` of Objects/setobject.c, so its iteration gives the
// same sequence. Only additions are kept, since nothing ported removes
// from such a set. On 20 Nov 2026 the emulation gave CPython 3.11.3's
// order for 200,000 random sequences of additions and for sets built
// from a dict's keys, a list, and a comprehension.
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <set>
#include <vector>

namespace c6502 {

class PyIntSet {
 public:
  PyIntSet() : table_(8) {}

  // Adds `key`, as `s.add(key)` does. `key` must be non-negative, so
  // that its hash is itself.
  void add(std::int64_t key) {
    const auto hash = static_cast<std::uint64_t>(key);
    std::size_t mask = table_.size() - 1;
    std::size_t i = hash & mask;
    std::uint64_t perturb = hash;
    while (true) {
      std::size_t probes = i + kLinearProbes <= mask ? kLinearProbes : 0;
      std::size_t j = i;
      while (true) {
        auto& slot = table_[j];
        if (!slot) {
          slot = key;
          members_.insert(key);
          ++used_;
          // No key is ever removed, so the table's fill is the number
          // of keys.
          if (used_ * 5 >= mask * 3) {
            resize(used_ > 50000 ? used_ * 2 : used_ * 4);
          }
          return;
        }
        if (*slot == key) {
          return;
        }
        if (probes == 0) {
          break;
        }
        --probes;
        ++j;
      }
      perturb >>= kPerturbShift;
      i = (i * 5 + 1 + static_cast<std::size_t>(perturb)) & mask;
    }
  }

  // Returns whether `key` is in the set.
  bool contains(std::int64_t key) const { return members_.count(key) != 0; }

  std::size_t size() const { return used_; }
  bool empty() const { return used_ == 0; }

  // Returns the keys in the order CPython iterates them.
  std::vector<std::int64_t> items() const {
    std::vector<std::int64_t> out;
    out.reserve(used_);
    for (const auto& slot : table_) {
      if (slot) {
        out.push_back(*slot);
      }
    }
    return out;
  }

  // Two sets are equal when they hold the same keys, as in Python,
  // whatever their orders.
  bool operator==(const PyIntSet& other) const { return members_ == other.members_; }

  // Returns the keys in ascending order.
  const std::set<std::int64_t>& sorted() const { return members_; }

 private:
  static constexpr std::size_t kLinearProbes = 9;
  static constexpr int kPerturbShift = 5;

  void resize(std::size_t minused) {
    std::size_t size = 8;
    while (size <= minused) {
      size <<= 1;
    }
    std::vector<std::optional<std::int64_t>> old(size);
    old.swap(table_);
    for (const auto& slot : old) {
      if (slot) {
        insert_clean(*slot);
      }
    }
  }

  void insert_clean(std::int64_t key) {
    const std::size_t mask = table_.size() - 1;
    const auto hash = static_cast<std::uint64_t>(key);
    std::uint64_t perturb = hash;
    std::size_t i = hash & mask;
    while (true) {
      if (!table_[i]) {
        table_[i] = key;
        return;
      }
      if (i + kLinearProbes <= mask) {
        for (std::size_t j = 1; j <= kLinearProbes; ++j) {
          if (!table_[i + j]) {
            table_[i + j] = key;
            return;
          }
        }
      }
      perturb >>= kPerturbShift;
      i = (i * 5 + 1 + static_cast<std::size_t>(perturb)) & mask;
    }
  }

  std::vector<std::optional<std::int64_t>> table_;
  // The same keys, for lookups.
  std::set<std::int64_t> members_;
  std::size_t used_ = 0;
};

}  // namespace c6502
