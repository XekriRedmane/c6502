#include "py_random.h"

#include <cmath>
#include <set>
#include <string>

#include "sha512.h"

namespace c6502::py_random {

namespace {

using asdl::Int;

constexpr int kN = 624;
constexpr int kM = 397;
constexpr std::uint32_t kMatrixA = 0x9908b0dfU;
constexpr std::uint32_t kUpperMask = 0x80000000U;
constexpr std::uint32_t kLowerMask = 0x7fffffffU;

// Returns the 32-bit words of the integer whose bytes, least
// significant first, are `bytes`, least significant word first, with
// at least one word, as CPython's `random_seed` passes a seed to
// `init_by_array`.
std::vector<std::uint32_t> key_words(std::vector<unsigned char> bytes) {
  while (!bytes.empty() && bytes.back() == 0) bytes.pop_back();
  std::vector<std::uint32_t> key((bytes.size() + 3) / 4);
  for (std::size_t i = 0; i < bytes.size(); ++i) {
    key[i / 4] |= std::uint32_t{bytes[i]} << (8 * (i % 4));
  }
  if (key.empty()) key.push_back(0);
  return key;
}

int bit_length(Int n) {
  int bits = 0;
  while (n > 0) {
    n >>= 1;
    ++bits;
  }
  return bits;
}

}  // namespace

Random::Random(Int seed) { this->seed(seed); }

Random::Random(std::string_view seed) { this->seed(seed); }

void Random::seed(Int seed) {
  Int n = seed < 0 ? -seed : seed;
  std::vector<unsigned char> bytes;
  while (n > 0) {
    bytes.push_back(static_cast<unsigned char>(static_cast<int>(n & 0xFF)));
    n >>= 8;
  }
  init_by_array(key_words(std::move(bytes)));
}

void Random::seed(std::string_view seed) {
  const std::array<unsigned char, 64> digest = sha512(seed);
  // The integer is big-endian: the string's bytes, then the digest's.
  std::vector<unsigned char> bytes(digest.rbegin(), digest.rend());
  for (auto it = seed.rbegin(); it != seed.rend(); ++it) {
    bytes.push_back(static_cast<unsigned char>(*it));
  }
  init_by_array(key_words(std::move(bytes)));
}

void Random::init_genrand(std::uint32_t s) {
  state_[0] = s;
  for (int i = 1; i < kN; ++i) {
    state_[i] = 1812433253U * (state_[i - 1] ^ (state_[i - 1] >> 30)) + static_cast<std::uint32_t>(i);
  }
  index_ = kN;
}

void Random::init_by_array(const std::vector<std::uint32_t>& key) {
  init_genrand(19650218U);
  int i = 1;
  std::size_t j = 0;
  const std::size_t length = key.size();
  for (std::size_t k = kN > length ? kN : length; k != 0; --k) {
    state_[i] = (state_[i] ^ ((state_[i - 1] ^ (state_[i - 1] >> 30)) * 1664525U)) + key[j] +
                static_cast<std::uint32_t>(j);
    ++i;
    ++j;
    if (i >= kN) {
      state_[0] = state_[kN - 1];
      i = 1;
    }
    if (j >= length) j = 0;
  }
  for (int k = kN - 1; k != 0; --k) {
    state_[i] = (state_[i] ^ ((state_[i - 1] ^ (state_[i - 1] >> 30)) * 1566083941U)) -
                static_cast<std::uint32_t>(i);
    ++i;
    if (i >= kN) {
      state_[0] = state_[kN - 1];
      i = 1;
    }
  }
  state_[0] = 0x80000000U;
}

std::uint32_t Random::genrand_uint32() {
  if (index_ >= kN) {
    int kk = 0;
    for (; kk < kN - kM; ++kk) {
      const std::uint32_t y = (state_[kk] & kUpperMask) | (state_[kk + 1] & kLowerMask);
      state_[kk] = state_[kk + kM] ^ (y >> 1) ^ ((y & 1U) ? kMatrixA : 0U);
    }
    for (; kk < kN - 1; ++kk) {
      const std::uint32_t y = (state_[kk] & kUpperMask) | (state_[kk + 1] & kLowerMask);
      state_[kk] = state_[kk + (kM - kN)] ^ (y >> 1) ^ ((y & 1U) ? kMatrixA : 0U);
    }
    const std::uint32_t y = (state_[kN - 1] & kUpperMask) | (state_[0] & kLowerMask);
    state_[kN - 1] = state_[kM - 1] ^ (y >> 1) ^ ((y & 1U) ? kMatrixA : 0U);
    index_ = 0;
  }
  std::uint32_t y = state_[index_++];
  y ^= y >> 11;
  y ^= (y << 7) & 0x9d2c5680U;
  y ^= (y << 15) & 0xefc60000U;
  y ^= y >> 18;
  return y;
}

double Random::random() {
  const std::uint32_t a = genrand_uint32() >> 5;
  const std::uint32_t b = genrand_uint32() >> 6;
  return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

Int Random::getrandbits(int k) {
  if (k < 0 || k > 127) throw std::invalid_argument("getrandbits takes 0 to 127 bits");
  if (k == 0) return 0;
  if (k <= 32) return Int(genrand_uint32() >> (32 - k));
  Int out = 0;
  for (int shift = 0; k > 0; shift += 32, k -= 32) {
    std::uint32_t r = genrand_uint32();
    if (k < 32) r >>= 32 - k;
    out |= Int(r) << shift;
  }
  return out;
}

Int Random::randbelow(Int n) {
  if (n <= 0) throw std::invalid_argument("_randbelow takes a positive bound");
  const int k = bit_length(n);
  Int r = getrandbits(k);
  while (r >= n) r = getrandbits(k);
  return r;
}

Int Random::randrange(Int stop) {
  if (stop <= 0) throw std::invalid_argument("empty range for randrange()");
  return randbelow(stop);
}

Int Random::randrange(Int start, Int stop) {
  if (stop - start <= 0) throw std::invalid_argument("empty range for randrange()");
  return start + randbelow(stop - start);
}

Int Random::randint(Int a, Int b) { return randrange(a, b + 1); }

std::size_t Random::index_below(std::size_t n) {
  return static_cast<std::size_t>(randbelow(Int(static_cast<std::uint64_t>(n))));
}

std::size_t Random::choice_index(std::size_t n) {
  if (n == 0) throw std::out_of_range("Cannot choose from an empty sequence");
  return index_below(n);
}

std::vector<std::size_t> Random::choices_indices(const std::vector<double>& weights,
                                                 std::size_t k) {
  std::vector<double> cum;
  double sum = 0;
  for (double w : weights) cum.push_back(sum += w);
  if (cum.empty() || !(cum.back() > 0.0) || !std::isfinite(cum.back())) {
    throw std::invalid_argument("Total of weights must be greater than zero");
  }
  const double total = cum.back();
  const std::size_t hi = cum.size() - 1;
  std::vector<std::size_t> out;
  for (std::size_t n = 0; n < k; ++n) {
    // bisect_right(cum, x, 0, hi)
    const double x = random() * total;
    std::size_t lo = 0, top = hi;
    while (lo < top) {
      const std::size_t mid = (lo + top) / 2;
      if (x < cum[mid]) {
        top = mid;
      } else {
        lo = mid + 1;
      }
    }
    out.push_back(lo);
  }
  return out;
}

std::vector<std::size_t> Random::choices_indices(std::size_t n, std::size_t k) {
  std::vector<std::size_t> out;
  const double size = static_cast<double>(n);
  for (std::size_t i = 0; i < k; ++i) {
    out.push_back(static_cast<std::size_t>(std::floor(random() * size)));
  }
  return out;
}

std::vector<std::size_t> Random::sample_indices(std::size_t n, std::size_t k) {
  if (k > n) throw std::invalid_argument("Sample larger than population or is negative");
  std::vector<std::size_t> out(k);
  // The size of a small set less that of an empty list, and from six
  // picks on the size of a set's table, which decide between the two
  // ways of picking.
  double setsize = 21;
  if (k > 5) {
    setsize += std::pow(4.0, std::ceil(std::log(static_cast<double>(k * 3)) / std::log(4.0)));
  }
  if (static_cast<double>(n) <= setsize) {
    std::vector<std::size_t> pool(n);
    for (std::size_t i = 0; i < n; ++i) pool[i] = i;
    for (std::size_t i = 0; i < k; ++i) {
      const std::size_t j = index_below(n - i);
      out[i] = pool[j];
      pool[j] = pool[n - i - 1];
    }
  } else {
    std::set<std::size_t> selected;
    for (std::size_t i = 0; i < k; ++i) {
      std::size_t j = index_below(n);
      while (selected.count(j) != 0) j = index_below(n);
      selected.insert(j);
      out[i] = j;
    }
  }
  return out;
}

}  // namespace c6502::py_random
