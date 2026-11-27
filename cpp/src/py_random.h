// CPython 3.11's `random.Random`, so that a C++ port of code that draws
// from it (the fuzzers, and the tests that pick their operands at
// random) draws the same values from the same seed. The generator is
// MT19937 seeded with `init_by_array`, as CPython's _randommodule.c
// seeds it, and the methods follow Lib/random.py. cpp/tests/frozen/
// py_random.txt holds CPython's own results, which
// cpp/tests/test_py_random.cpp compares with these.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"

namespace c6502::py_random {

class Random {
 public:
  // `random.Random(seed)` for an integer seed, whose sign is ignored.
  explicit Random(asdl::Int seed);
  // `random.Random(seed)` for a string seed: the integer of the UTF-8
  // bytes followed by their SHA-512 digest, read big-endian (version 2
  // of `seed`).
  explicit Random(std::string_view seed);

  void seed(asdl::Int seed);
  void seed(std::string_view seed);

  // MT19937's next 32-bit output.
  std::uint32_t genrand_uint32();

  // `random()`: a double in [0, 1) made of 53 bits of two outputs.
  double random();
  // `getrandbits(k)` for 0 <= k <= 127, the low word drawn first.
  asdl::Int getrandbits(int k);
  // `_randbelow(n)` for n > 0, which draws `n.bit_length()` bits until
  // the value is below `n`.
  asdl::Int randbelow(asdl::Int n);
  // `randrange(stop)` and `randrange(start, stop)`, with a step of 1.
  asdl::Int randrange(asdl::Int stop);
  asdl::Int randrange(asdl::Int start, asdl::Int stop);
  // `randint(a, b)`, which is `randrange(a, b + 1)`.
  asdl::Int randint(asdl::Int a, asdl::Int b);

  // The index `choice(seq)` takes from a sequence of `n` items.
  std::size_t choice_index(std::size_t n);
  template <class T>
  const T& choice(const std::vector<T>& seq) {
    return seq[choice_index(seq.size())];
  }

  // `shuffle(x)`.
  template <class T>
  void shuffle(std::vector<T>& x) {
    for (std::size_t i = x.size(); i-- > 1;) {
      const std::size_t j = index_below(i + 1);
      std::swap(x[i], x[j]);
    }
  }

  // The indices `choices(population, weights=weights, k=k)` takes from
  // a population of `weights.size()` items. Integer weights are exact
  // in a double, so their sums are the integers Python adds.
  std::vector<std::size_t> choices_indices(const std::vector<double>& weights, std::size_t k);
  // The indices `choices(population, k=k)` takes from a population of
  // `n` items, without weights.
  std::vector<std::size_t> choices_indices(std::size_t n, std::size_t k);

  // The indices `sample(population, k)` takes from a population of `n`
  // items, in the order it returns them.
  std::vector<std::size_t> sample_indices(std::size_t n, std::size_t k);
  template <class T>
  std::vector<T> sample(const std::vector<T>& population, std::size_t k) {
    std::vector<T> out;
    for (std::size_t i : sample_indices(population.size(), k)) out.push_back(population[i]);
    return out;
  }

 private:
  void init_genrand(std::uint32_t s);
  void init_by_array(const std::vector<std::uint32_t>& key);
  std::size_t index_below(std::size_t n);

  std::array<std::uint32_t, 624> state_{};
  int index_ = 0;
};

}  // namespace c6502::py_random
