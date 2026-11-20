// A map that iterates in insertion order, as a Python dict does. The
// compile's tables that Python keeps in dicts (the symbol table, the
// calling conventions in `abi`, the colorings) feed passes whose output
// follows the dict's order, so their C++ form keeps it.
#pragma once

#include <cstddef>
#include <initializer_list>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

namespace c6502 {

template <class K, class V>
class OrderedMap {
 public:
  using value_type = std::pair<K, V>;
  using iterator = typename std::vector<value_type>::iterator;
  using const_iterator = typename std::vector<value_type>::const_iterator;

  OrderedMap() = default;
  OrderedMap(std::initializer_list<value_type> init) {
    for (const auto& kv : init) {
      insert_or_assign(kv.first, kv.second);
    }
  }

  // Returns the value under `key`, or null. A key of another type that
  // compares with K, such as a `std::string_view` for a `std::string`
  // key, is looked up without a copy.
  template <class Key>
  const V* find(const Key& key) const {
    const auto it = index_.find(key);
    return it == index_.end() ? nullptr : &items_[it->second].second;
  }
  template <class Key>
  V* find(const Key& key) {
    const auto it = index_.find(key);
    return it == index_.end() ? nullptr : &items_[it->second].second;
  }
  // Returns the value under `key`, and throws `std::out_of_range` when
  // there is none, as `d[key]` raises `KeyError`.
  template <class Key>
  const V& at(const Key& key) const {
    if (const V* v = find(key)) {
      return *v;
    }
    throw std::out_of_range("OrderedMap::at: no such key");
  }

  template <class Key>
  bool contains(const Key& key) const {
    return index_.find(key) != index_.end();
  }

  // Sets the value under `key`. A new key goes at the end, and a key
  // already present keeps its place, as assigning to a dict's key does.
  V& insert_or_assign(K key, V value) {
    if (V* v = find(key)) {
      *v = std::move(value);
      return *v;
    }
    index_.emplace(key, items_.size());
    items_.emplace_back(std::move(key), std::move(value));
    return items_.back().second;
  }

  // Returns the value under `key`, inserting a default one at the end
  // when there is none, as `dict.setdefault(key, V())` does.
  V& operator[](const K& key) {
    if (V* v = find(key)) {
      return *v;
    }
    return insert_or_assign(key, V{});
  }

  // Removes `key`, keeping the order of the others, as `del d[key]`
  // does. Returns whether it was present. This takes time linear in
  // the size of the map.
  template <class Key>
  bool erase(const Key& key) {
    const auto it = index_.find(key);
    if (it == index_.end()) {
      return false;
    }
    const std::size_t pos = it->second;
    index_.erase(it);
    items_.erase(items_.begin() + static_cast<std::ptrdiff_t>(pos));
    for (auto& [k, i] : index_) {
      if (i > pos) {
        --i;
      }
    }
    return true;
  }

  std::size_t size() const { return items_.size(); }
  bool empty() const { return items_.empty(); }
  iterator begin() { return items_.begin(); }
  iterator end() { return items_.end(); }
  const_iterator begin() const { return items_.begin(); }
  const_iterator end() const { return items_.end(); }

  // Compares the entries in order, so two maps with the same entries in
  // different orders differ, where Python's dicts would be equal. The
  // checks compare encodings, which keep the order too.
  bool operator==(const OrderedMap& other) const { return items_ == other.items_; }

 private:
  std::vector<value_type> items_;
  std::map<K, std::size_t, std::less<>> index_;
};

}  // namespace c6502
