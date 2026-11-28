// The integer types of the fuzzers that model C's integer rules
// (exprs.h and stmts.h), with C23's bit-precise types, `_BitInt(N)` and
// `unsigned _BitInt(N)` (§6.2.5), ported from fuzz/bit_precise_types.py.
//
// Each fuzzer keeps a table of its types by key, and a rank for each
// standard type left after the integer promotions. A seed that uses
// bit-precise types adds a few to the table with `add`, under keys that
// `is_bit_precise` recognizes. Those types aren't promoted (C23
// §6.3.1.1p2), and `common` gives the usual arithmetic conversions of
// two promoted types when either is one of them (§6.3.1.8).
#pragma once

#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "asdl/runtime.h"

namespace c6502::fuzz {

using asdl::Int;

struct IntType {
  std::string spelling;
  int width = 0;
  bool is_signed = false;
};

using TypeTable = std::map<std::string, IntType, std::less<>>;
// The conversion rank of each standard type left after the promotions.
using RankTable = std::map<std::string, int, std::less<>>;
// The unsigned type of each standard signed type.
using UnsignedOf = std::map<std::string, std::string, std::less<>>;

// Returns `v` converted to `t`: modulo 2**width, then read with `t`'s
// signedness.
Int wrap(Int v, const IntType& t);

// Returns `a * b` modulo 2**128, which holds the product's low 64 bits
// for any two operands of 64 bits.
Int wrapping_mul(Int a, Int b);

// Returns the key of `_BitInt(width)` or `unsigned _BitInt(width)`,
// added to `types`.
std::string add_bit_precise(TypeTable& types, int width, bool is_signed);

bool is_bit_precise(std::string_view key);

// Returns the keys of the bit-precise types seed `seed` uses, added to
// `types`: none in half the seeds, and `count` of random widths in the
// others, in the order drawn, a key twice when two draws agree. The
// choice has a random stream of its own, seeded with "bitint<seed>", so
// a seed without them is the program it was before they existed.
std::vector<std::string> choose_bit_precise(TypeTable& types, Int seed, int count);

// C23 §6.3.1.8 for the promoted types `a` and `b`, at least one of them
// bit-precise. Adds the unsigned bit-precise type it returns to `types`
// when that type is new.
std::string common_bit_precise(TypeTable& types, const RankTable& rank,
                               const UnsignedOf& unsigned_of, const std::string& a,
                               const std::string& b);

// Returns a constant expression of the bit-precise type `t` with value
// `v`: a cast of its bit pattern, which c99_to_tac folds.
std::string bit_precise_literal(const TypeTable& types, Int v, const std::string& t);

// Returns the decimal digits of `v`.
std::string decimal(Int v);

}  // namespace c6502::fuzz
