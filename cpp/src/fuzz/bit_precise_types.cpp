#include "fuzz/bit_precise_types.h"

#include <cstdint>
#include <cstdio>
#include <utility>

#include "py_random.h"

namespace c6502::fuzz {

namespace {

Int mask(int width) { return (Int(1) << width) - 1; }

// Orders the promoted types by conversion rank (C23 §6.3.1.1p1): by
// width, with a standard type above a bit-precise one of the same
// width.
std::pair<int, int> rank_key(const TypeTable& types, const RankTable& rank, const std::string& t) {
  const int w = types.find(t)->second.width;
  return is_bit_precise(t) ? std::pair{w, 0} : std::pair{w, rank.find(t)->second};
}

}  // namespace

Int wrap(Int v, const IntType& t) {
  v &= mask(t.width);
  if (t.is_signed && ((v >> (t.width - 1)) & 1) != 0) v -= Int(1) << t.width;
  return v;
}

Int wrapping_mul(Int a, Int b) {
#if defined(__SIZEOF_INT128__)
  return static_cast<Int>(static_cast<unsigned __int128>(a) * static_cast<unsigned __int128>(b));
#else
  // MSVC's `std::_Signed128` multiplies its two words modulo 2**128.
  return a * b;
#endif
}

std::string add_bit_precise(TypeTable& types, int width, bool is_signed) {
  const std::string w = std::to_string(width);
  std::string key = (is_signed ? "bi" : "ubi") + w;
  types[key] = IntType{(is_signed ? "_BitInt(" : "unsigned _BitInt(") + w + ")", width, is_signed};
  return key;
}

bool is_bit_precise(std::string_view key) { return key.find("bi") != std::string_view::npos; }

std::vector<std::string> choose_bit_precise(TypeTable& types, Int seed, int count) {
  py_random::Random rng("bitint" + decimal(seed));
  if (rng.random() < 0.5) return {};
  std::vector<std::string> keys;
  for (int i = 0; i < count; ++i) {
    const bool is_signed = rng.random() < 0.5;
    const int width = static_cast<int>(rng.randint(is_signed ? 2 : 1, 64));
    keys.push_back(add_bit_precise(types, width, is_signed));
  }
  return keys;
}

std::string common_bit_precise(TypeTable& types, const RankTable& rank,
                               const UnsignedOf& unsigned_of, const std::string& a,
                               const std::string& b) {
  if (a == b) return a;
  const bool sa = types.find(a)->second.is_signed;
  const bool sb = types.find(b)->second.is_signed;
  if (sa == sb) return rank_key(types, rank, a) >= rank_key(types, rank, b) ? a : b;
  const std::string& u = sa ? b : a;
  const std::string& s = sa ? a : b;
  if (rank_key(types, rank, u) >= rank_key(types, rank, s)) return u;
  const int sw = types.find(s)->second.width;
  if (sw - 1 >= types.find(u)->second.width) return s;
  if (is_bit_precise(s)) return add_bit_precise(types, sw, false);
  return unsigned_of.find(s)->second;
}

std::string bit_precise_literal(const TypeTable& types, Int v, const std::string& t) {
  const IntType& type = types.find(t)->second;
  char hex[24];
  std::snprintf(hex, sizeof hex, "%llX",
                static_cast<unsigned long long>(static_cast<std::uint64_t>(v & mask(type.width))));
  return "((" + type.spelling + ")0x" + hex + "ULL)";
}

std::string decimal(Int v) { return asdl::int_to_string(v); }

}  // namespace c6502::fuzz
