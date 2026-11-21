/* Splits a 2-byte ordering compare and an equality compare of the same
two values into a compare of each byte.

What it does. A lookup in a sorted table tests the key against an entry
twice, once for order and once for equality:

    if (key < e) return 0;
    if (key == e) return found;

which reaches `tac_to_asm` as

    JumpIfCmp(GreaterOrEqual, key, e, .if_end@0)
    Ret(0)
  .if_end@0:
    JumpIfCmp(NotEqual, key, e, .if_end@1)
    <found>
  .if_end@1:

and lowers to a 2-byte subtract for the first compare and a 2-byte
equality test for the second, 22 bytes with zero-page operands. Both
results come from the high bytes unless the high bytes are equal, and
then from the low bytes, which is how the 6502 code a person writes
does it:

    LDA key+1; CMP e+1; BCC less; BNE greater
    LDA key;   CMP e;   BCC less; BNE greater
    <equal>

That's 16 bytes, and most entries take only the first two branches.
Zork's `match_dictionary_word` (examples/zork/input.c) compares each of
a word's two 2-byte halves with an entry's this way.

The pass writes the byte compares in TAC. Each of the three outcomes,
less, equal, and greater, goes where the two compares send it.

  * Two compares in a row, `JumpIfCmp(op1, a, b, T1); JumpIfCmp(op2, a,
    b, T2)` with `op2` an equality. An outcome goes to T1 if `op1` is
    true of it, else to T2 if `op2` is true of it, else to the
    instruction after the second compare. The two become, for the high
    bytes and then the low ones, a `<` to the less outcome's place and
    a `!=` to the greater one's, except for the low bytes'
    fall-through.
  * An ordering compare whose target starts with the equality compare,
    `JumpIfCmp(>=, a, b, T1); X; T1: JumpIfCmp(op2, a, b, T2)`, where
    X, the less outcome, ends in a `Ret` or `Jump`. This is the two
    `if`s above. The first compare becomes

        JumpIfCmp(<, a_hi, b_hi, .less)
        JumpIfCmp(!=, a_hi, b_hi, <greater's place>)
        JumpIfCmp(>=, a_lo, b_lo, .low_same_or_more)
      .less:
        X
      .low_same_or_more:
        JumpIfCmp(op2, a_lo, b_lo, T2)

    and the second compare goes, when nothing else jumps to it. The low
    bytes keep a branch over X so that the asm `tail_merge` can send it
    to another copy of X, as the less outcome of every such pair in a
    function shares one `return 0`. The second compare of the low bytes
    is the first's, which `redundant_compare` drops in the asm. `<=` is
    the same with the operands swapped.

The pattern.

  * The ordering compare is unsigned (an operand is unsigned or a
    pointer, the rule `tac_to_asm` uses) and both operands are 2 bytes.
    Neither is volatile, and they aren't both constants.
  * The equality compare has the same two operands, in either order.
  * The three outcomes go to three different places.
  * The ordering compare doesn't follow a subtract of its operands,
    which `tac_to_asm` would branch on instead (compare_subtract.py).

A byte of an operand is a byte constant for a constant, the two bytes
for a value built as `(ZeroExtend(h) << 8) | ZeroExtend(l)` of two
1-byte temporaries, and 0 and the byte for `ZeroExtend(l)`. Each of
those temporaries and the instructions that build the value must have
one definition each, in one basic block, so the bytes hold at the
compare what they held when the value was built. Any other operand's
bytes are new `Truncate(a)` and `Truncate(a >> 8)` temporaries, which
`tac_to_asm` lowers to reads of the one byte
(`significant_bytes.low_byte_only`). A high byte of 0 makes the less
outcome `b_hi != 0` and the greater one impossible there (and the other
way around for `b`).

Where it runs. Once, after SSA destruction. Its new edges would require
Phi arguments in SSA form, and the labels it adds split blocks that a
Phi names as predecessors.

The port. Python finds a name's definitions and use counts afresh in
each helper. Every helper of one rewrite reads the same list, so the
C++ pass computes them once per candidate compare. A load moved down to
its compare is left out of the instructions before the compare only;
when its definition comes after the compare, Python keeps it in place
as well, and so does the port.
*/
#include "passes/optimization/three_way_compare.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "passes/recursion_saves.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;
using framework::Instrs;
using framework::PassContext;
using IK = ir::Kind_instruction;

// The outcomes of comparing `a` with `b`, as bits of a mask.
constexpr unsigned kLess = 1;
constexpr unsigned kEqual = 2;
constexpr unsigned kGreater = 4;
constexpr unsigned kOutcomes[] = {kLess, kEqual, kGreater};

// Returns the outcomes for which `op` is true (Python's `_HOLDS`), or 0
// for an operator that isn't a comparison.
unsigned holds(const ir::Type_binary_operator& op) {
  switch (ir::kind(op)) {
    case ir::Kind_binary_operator::LessThan:
      return kLess;
    case ir::Kind_binary_operator::LessOrEqual:
      return kLess | kEqual;
    case ir::Kind_binary_operator::GreaterThan:
      return kGreater;
    case ir::Kind_binary_operator::GreaterOrEqual:
      return kEqual | kGreater;
    case ir::Kind_binary_operator::Equal:
      return kEqual;
    case ir::Kind_binary_operator::NotEqual:
      return kLess | kGreater;
    default:
      return 0;
  }
}

bool is_ordering(const ir::Type_binary_operator& op) {
  return asdl::isa<ir::LessThan, ir::GreaterThan, ir::LessOrEqual, ir::GreaterOrEqual>(op);
}

bool is_equality(const ir::Type_binary_operator& op) {
  return asdl::isa<ir::Equal, ir::NotEqual>(op);
}

// Returns the target of a jump (Python's `_JUMPS`), or null.
const std::string* jump_target(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case IK::Jump:
      return &std::get<ir::Jump>(instr).target;
    case IK::JumpIfTrue:
      return &std::get<ir::JumpIfTrue>(instr).target;
    case IK::JumpIfFalse:
      return &std::get<ir::JumpIfFalse>(instr).target;
    case IK::JumpIfCmp:
      return &std::get<ir::JumpIfCmp>(instr).target;
    case IK::JumpIfMasked:
      return &std::get<ir::JumpIfMasked>(instr).target;
    default:
      return nullptr;
  }
}

// Returns the value of an integer constant. A floating constant has
// none, where Python's `.value` raises.
std::optional<Int> int_value(const ir::Type_const& c) {
  switch (ir::kind(c)) {
    case ir::Kind_const::ConstChar:
      return std::get<ir::ConstChar>(c).value;
    case ir::Kind_const::ConstUChar:
      return std::get<ir::ConstUChar>(c).value;
    case ir::Kind_const::ConstInt:
      return std::get<ir::ConstInt>(c).value;
    case ir::Kind_const::ConstLong:
      return std::get<ir::ConstLong>(c).value;
    case ir::Kind_const::ConstLongLong:
      return std::get<ir::ConstLongLong>(c).value;
    case ir::Kind_const::ConstUInt:
      return std::get<ir::ConstUInt>(c).value;
    case ir::Kind_const::ConstULong:
      return std::get<ir::ConstULong>(c).value;
    case ir::Kind_const::ConstULongLong:
      return std::get<ir::ConstULongLong>(c).value;
    case ir::Kind_const::ConstInt24:
      return std::get<ir::ConstInt24>(c).value;
    case ir::Kind_const::ConstUInt24:
      return std::get<ir::ConstUInt24>(c).value;
    case ir::Kind_const::ConstInt40:
      return std::get<ir::ConstInt40>(c).value;
    case ir::Kind_const::ConstUInt40:
      return std::get<ir::ConstUInt40>(c).value;
    case ir::Kind_const::ConstInt48:
      return std::get<ir::ConstInt48>(c).value;
    case ir::Kind_const::ConstUInt48:
      return std::get<ir::ConstUInt48>(c).value;
    case ir::Kind_const::ConstInt56:
      return std::get<ir::ConstInt56>(c).value;
    case ir::Kind_const::ConstUInt56:
      return std::get<ir::ConstUInt56>(c).value;
    default:
      return std::nullopt;
  }
}

// Returns whether `v` is a constant whose value is `n`.
bool is_const_value(const ir::Type_val& v, Int n) {
  const auto* c = asdl::as<ir::Constant>(v);
  if (c == nullptr) {
    return false;
  }
  const std::optional<Int> value = int_value(c->const_);
  return value && *value == n;
}

bool is_zero(const ir::Type_val& v) { return is_const_value(v, 0); }

bool is_temp(const ir::Type_val& v) {
  const auto* var = asdl::as<ir::Var>(v);
  return var != nullptr && var->name.starts_with('%');
}

// Returns whether the sets {a1, a2} and {b1, b2} are equal, as Python
// compares the sets of the operands' keys. Two values have equal keys
// exactly when they are equal.
bool same_pair(const ir::Type_val& a1, const ir::Type_val& a2, const ir::Type_val& b1,
               const ir::Type_val& b2) {
  return (a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1);
}

ir::Type_val byte_const(Int value) {
  return ir::Constant{.const_ = ir::ConstUChar{.value = value & 0xFF}};
}

ir::Type_instruction jcmp(ir::Type_binary_operator op, ir::Type_val src1, ir::Type_val src2,
                          std::string target) {
  return ir::JumpIfCmp{
      .op = std::move(op), .src1 = std::move(src1), .src2 = std::move(src2),
      .target = std::move(target)};
}

// Returns the type of the symbol `v` names, or null.
const c99::Type_data_type* type_of(const ir::Type_val& v, const SymbolTable& symbols) {
  const auto* var = asdl::as<ir::Var>(v);
  if (var == nullptr) {
    return nullptr;
  }
  const Symbol* sym = symbols.find(var->name);
  return sym == nullptr ? nullptr : &sym->type;
}

// Fresh temporary and label names for one function (Python's `_Names`).
// One counter numbers both.
struct Names {
  std::string fn;
  SymbolTable* symbols;
  std::set<std::string, std::less<>> labels;
  int n = 0;

  Names(const ir::Function& f, SymbolTable* syms) : fn(f.name), symbols(syms) {
    for (const auto& ins : f.instructions) {
      if (const auto* lab = asdl::as<ir::Label>(ins)) {
        labels.insert(lab->name);
      }
    }
  }

  // Returns a new `unsigned char` temporary, entered in the symbols.
  ir::Var byte() {
    std::string name;
    while (true) {
      ++n;
      name = "%" + fn + ".cmp3." + std::to_string(n);
      if (symbols->find(name) == nullptr) {
        break;
      }
    }
    symbols->insert_or_assign(name, Symbol{.type = c99::UChar{}, .attrs = LocalAttr{}});
    return ir::Var{.name = name};
  }

  // Returns a new `unsigned int` temporary, which takes its name and its
  // place in the symbols from `byte`.
  ir::Var word() {
    ir::Var v = byte();
    symbols->insert_or_assign(v.name, Symbol{.type = c99::UInt{}, .attrs = LocalAttr{}});
    return v;
  }

  std::string label() {
    while (true) {
      ++n;
      std::string name = "." + fn + "@cmp3@" + std::to_string(n);
      if (!labels.contains(name)) {
        labels.insert(name);
        return name;
      }
    }
  }
};

// The definitions and the use counts of one list of instructions, which
// every helper of a rewrite reads.
struct Analysis {
  const Instrs& instrs;
  std::map<std::string, std::vector<std::size_t>, std::less<>> defs;
  OrderedMap<std::string, std::size_t> uses;

  explicit Analysis(const Instrs& in) : instrs(in), uses(count_uses(in)) {
    for (std::size_t k = 0; k < in.size(); ++k) {
      for (const std::string_view d : recursion_saves::defs_in(in[k])) {
        defs[std::string(d)].push_back(k);
      }
    }
  }

  // Returns the index of the one definition of the Var `v`, or none when
  // `v` isn't a Var or has no definition or several (Python's
  // `_only_def`).
  std::optional<std::size_t> only_def(const ir::Type_val& v) const {
    const auto* var = asdl::as<ir::Var>(v);
    if (var == nullptr) {
      return std::nullopt;
    }
    const auto it = defs.find(var->name);
    if (it == defs.end() || it->second.size() != 1) {
      return std::nullopt;
    }
    return it->second.front();
  }

  std::size_t use_count(std::string_view name) const {
    const std::size_t* n = uses.find(name);
    return n == nullptr ? 0 : *n;
  }
};

// Returns the variable `v` is a copy of, through temporaries that have
// one definition each, which all hold the same value.
ir::Type_val root(ir::Type_val v, const Analysis& a) {
  for (int step = 0; step < 8; ++step) {
    const std::optional<std::size_t> d = is_temp(v) ? a.only_def(v) : std::nullopt;
    if (!d) {
      return v;
    }
    const auto* copy = asdl::as<ir::Copy>(a.instrs[*d]);
    if (copy == nullptr) {
      return v;
    }
    // A variable that isn't a single-definition temporary may change
    // between the two copies.
    if (!(is_temp(copy->src) && a.only_def(copy->src))) {
      return v;
    }
    v = copy->src;
  }
  return v;
}

// Returns whether `ins` is an equality compare of the values `j1`
// compares, each the same variable or a copy of the same one.
bool same_equality(const ir::Type_instruction* ins, const ir::JumpIfCmp& j1, const Analysis& a) {
  const auto* j2 = ins == nullptr ? nullptr : asdl::as<ir::JumpIfCmp>(*ins);
  if (j2 == nullptr || !is_equality(j2->op)) {
    return false;
  }
  const ir::Type_val r1 = root(j2->src1, a);
  const ir::Type_val r2 = root(j2->src2, a);
  if (r1 == r2) {
    return false;
  }
  return same_pair(r1, r2, root(j1.src1, a), root(j1.src2, a));
}

bool subtracts(const ir::Type_instruction& ins, const ir::JumpIfCmp& j1) {
  const auto* bin = asdl::as<ir::Binary>(ins);
  return bin != nullptr && asdl::isa<ir::Subtract>(bin->op) &&
         same_pair(bin->src1, bin->src2, j1.src1, j1.src2);
}

bool qualifies(const ir::Type_val& a, const ir::Type_val& b, const SymbolTable& symbols) {
  if (asdl::isa<ir::Constant>(a) && asdl::isa<ir::Constant>(b)) {
    return false;
  }
  bool is_unsigned = false;
  for (const ir::Type_val* v : {&a, &b}) {
    if (const auto* c = asdl::as<ir::Constant>(*v)) {
      if (!asdl::isa<ir::ConstInt, ir::ConstUInt>(c->const_)) {
        return false;
      }
      is_unsigned |= asdl::isa<ir::ConstUInt>(c->const_);
      continue;
    }
    const c99::Type_data_type* t = type_of(*v, symbols);
    if (t == nullptr || !asdl::isa<c99::Int, c99::UInt, c99::Pointer>(*t)) {
      return false;
    }
    is_unsigned |= asdl::isa<c99::UInt, c99::Pointer>(*t);
  }
  return is_unsigned;
}

// The bytes of the two operands (Python's `_Split`). `high_pre` and
// `low_pre` compute any new temporaries for the high and low bytes, and
// `high_loads` and `low_loads` are the loads of bytes moved from
// `moved` (their indices) to just before the compares of those bytes.
struct Split {
  Instrs high_pre;
  Instrs low_pre;
  std::vector<std::pair<ir::Type_val, ir::Type_val>> pairs;
  Instrs high_loads;
  Instrs low_loads;
  std::set<std::size_t> moved;
};

bool byte_temp(const ir::Type_val& v, const SymbolTable& symbols) {
  if (!is_temp(v)) {
    return false;
  }
  const c99::Type_data_type* t = type_of(v, symbols);
  return t != nullptr && asdl::isa<c99::UChar, c99::Char>(*t);
}

// Returns `(hi, lo)` when `v` is `(ZeroExtend(h) << 8) | ZeroExtend(l)`,
// or `ZeroExtend(l)` with a high byte of 0, of 1-byte temporaries that
// hold at every read of `v` what they held when `v` was built.
std::optional<std::pair<ir::Type_val, ir::Type_val>> built_from_bytes(
    const ir::Type_val& v0, const Analysis& a, const SymbolTable& symbols) {
  const ir::Type_val v = root(v0, a);
  const std::optional<std::size_t> top = a.only_def(v);
  if (!top) {
    return std::nullopt;
  }
  std::vector<std::size_t> block = {*top};
  const ir::Type_instruction& top_ins = a.instrs[*top];
  std::optional<ir::Type_val> hi;
  std::optional<ir::Type_val> lo;
  std::vector<ir::Type_val> sources;
  if (const auto* ze = asdl::as<ir::ZeroExtend>(top_ins)) {
    if (!byte_temp(ze->src, symbols)) {
      return std::nullopt;
    }
    lo = ze->src;
    hi = byte_const(0);
    sources = {ze->src};
  } else if (const auto* bin = asdl::as<ir::Binary>(top_ins);
             bin != nullptr && asdl::isa<ir::BitwiseOr>(bin->op)) {
    bool found = false;
    const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {
        {&bin->src1, &bin->src2}, {&bin->src2, &bin->src1}};
    for (const auto& [s, z] : orders) {
      const std::optional<std::size_t> shift = a.only_def(*s);
      const std::optional<std::size_t> ext = a.only_def(*z);
      if (!shift || !ext) {
        continue;
      }
      const auto* si = asdl::as<ir::Binary>(a.instrs[*shift]);
      const auto* zi = asdl::as<ir::ZeroExtend>(a.instrs[*ext]);
      if (!(si != nullptr && asdl::isa<ir::LeftShift>(si->op) && is_const_value(si->src2, 8) &&
            zi != nullptr)) {
        continue;
      }
      const std::optional<std::size_t> hext = a.only_def(si->src1);
      if (!hext) {
        continue;
      }
      const auto* hi_ext = asdl::as<ir::ZeroExtend>(a.instrs[*hext]);
      if (hi_ext == nullptr) {
        continue;
      }
      hi = hi_ext->src;
      lo = zi->src;
      block.push_back(*shift);
      block.push_back(*ext);
      block.push_back(*hext);
      found = true;
      break;
    }
    if (!found) {
      return std::nullopt;
    }
    if (!(byte_temp(*hi, symbols) && byte_temp(*lo, symbols))) {
      return std::nullopt;
    }
    sources = {*hi, *lo};
  } else {
    return std::nullopt;
  }
  for (const ir::Type_val& s : sources) {
    const std::optional<std::size_t> d = a.only_def(s);
    if (!d) {
      return std::nullopt;
    }
    block.push_back(*d);
  }
  // Every definition in one basic block, so the bytes are redefined only
  // when the value is rebuilt.
  std::size_t first = block.front();
  std::size_t last = block.front();
  for (const std::size_t k : block) {
    first = std::min(first, k);
    last = std::max(last, k);
  }
  if (last != *top) {
    return std::nullopt;
  }
  for (std::size_t k = first + 1; k <= last; ++k) {
    const ir::Type_instruction& ins = a.instrs[k];
    if (asdl::isa<ir::Label, ir::Ret>(ins) || jump_target(ins) != nullptr) {
      return std::nullopt;
    }
  }
  return std::pair{std::move(*hi), std::move(*lo)};
}

// What `bytes` finds of one operand: the instructions that compute new
// temporaries for each byte, the two bytes, and the indices of the
// instructions that define the bytes where the operand is built from
// them.
struct Bytes {
  Instrs high_pre;
  Instrs low_pre;
  ir::Type_val hi;
  ir::Type_val lo;
  std::optional<std::size_t> hi_def;
  std::optional<std::size_t> lo_def;
};

std::optional<Bytes> bytes(const ir::Type_val& v, const Analysis& a, SymbolTable& symbols,
                           Names& names) {
  if (const auto* c = asdl::as<ir::Constant>(v)) {
    const Int value = *int_value(c->const_) & 0xFFFF;
    return Bytes{.hi = byte_const(value >> 8), .lo = byte_const(value)};
  }
  if (auto built = built_from_bytes(v, a, symbols)) {
    Bytes out{.hi = std::move(built->first), .lo = std::move(built->second)};
    out.hi_def = a.only_def(out.hi);
    out.lo_def = a.only_def(out.lo);
    return out;
  }
  const c99::Type_data_type* t = type_of(v, symbols);
  if (t != nullptr && asdl::isa<c99::Pointer>(*t)) {
    return std::nullopt;
  }
  ir::Var lo = names.byte();
  ir::Var shifted = names.word();
  ir::Var hi = names.byte();
  Bytes out{.hi = hi, .lo = lo};
  // `v` is read where the compare read it, so the low byte may be read
  // after the jumps on the high bytes.
  out.high_pre.push_back(ir::Binary{
      .op = ir::RightShift{},
      .src1 = v,
      .src2 = ir::Constant{.const_ = ir::ConstInt{.value = 8}},
      .dst = shifted,
  });
  out.high_pre.push_back(ir::Truncate{.src = shifted, .dst = hi});
  out.low_pre.push_back(ir::Truncate{.src = v, .dst = lo});
  return out;
}

// Returns the destination of an instruction that writes nothing but its
// destination (Python's `_PURE`), or null for any other instruction.
const ir::Type_val* pure_dst(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case IK::Binary:
      return &std::get<ir::Binary>(ins).dst;
    case IK::Unary:
      return &std::get<ir::Unary>(ins).dst;
    case IK::Copy:
      return &std::get<ir::Copy>(ins).dst;
    case IK::ZeroExtend:
      return &std::get<ir::ZeroExtend>(ins).dst;
    case IK::SignExtend:
      return &std::get<ir::SignExtend>(ins).dst;
    case IK::Truncate:
      return &std::get<ir::Truncate>(ins).dst;
    case IK::GetAddress:
      return &std::get<ir::GetAddress>(ins).dst;
    case IK::Load:
      return &std::get<ir::Load>(ins).dst;
    case IK::IndirectIndexedLoad:
      return &std::get<ir::IndirectIndexedLoad>(ins).dst;
    case IK::IndexedLoad:
      return &std::get<ir::IndexedLoad>(ins).dst;
    default:
      return nullptr;
  }
}

// Returns whether `ins` is a volatile load (the only instructions of
// Python's `_PURE` with an `is_volatile` field are the loads).
bool volatile_load(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case IK::Load:
      return std::get<ir::Load>(ins).is_volatile;
    case IK::IndirectIndexedLoad:
      return std::get<ir::IndirectIndexedLoad>(ins).is_volatile;
    case IK::IndexedLoad:
      return std::get<ir::IndexedLoad>(ins).is_volatile;
    default:
      return false;
  }
}

// Returns whether the load at `instrs[d]` reads the same byte at the
// compare `instrs[i]`: it isn't volatile, the value it builds is its
// only reader, and nothing between it and the compare writes memory or
// the load's operands.
bool movable_load(const Analysis& a, std::size_t d, std::size_t i) {
  const ir::Type_instruction& load = a.instrs[d];
  if (!asdl::isa<ir::Load, ir::IndirectIndexedLoad, ir::IndexedLoad>(load) ||
      volatile_load(load)) {
    return false;
  }
  const auto* dst = asdl::as<ir::Var>(*pure_dst(load));
  if (dst == nullptr || a.use_count(dst->name) != 1) {
    return false;
  }
  std::set<std::string, std::less<>> read;
  for (const std::string_view name : uses_in(load)) {
    read.emplace(name);
  }
  for (std::size_t k = d + 1; k < i; ++k) {
    const ir::Type_instruction& ins = a.instrs[k];
    const ir::Type_val* out = pure_dst(ins);
    if (out == nullptr || volatile_load(ins)) {
      return false;
    }
    const auto* var = asdl::as<ir::Var>(*out);
    if (var == nullptr || !var->name.starts_with('%') || read.contains(var->name)) {
      return false;
    }
  }
  return true;
}

// Returns the `Split` of `a` and `b` at the compare `instrs[i]`, or
// none. The temporaries made for `a` stay in the symbols when `b` has no
// bytes.
std::optional<Split> split(const ir::Type_val& a, const ir::Type_val& b, const Analysis& an,
                           std::size_t i, SymbolTable& symbols, Names& names) {
  Split out;
  for (const ir::Type_val* v : {&a, &b}) {
    std::optional<Bytes> found = bytes(*v, an, symbols, names);
    if (!found) {
      return std::nullopt;
    }
    for (auto& ins : found->high_pre) {
      out.high_pre.push_back(std::move(ins));
    }
    for (auto& ins : found->low_pre) {
      out.low_pre.push_back(std::move(ins));
    }
    out.pairs.emplace_back(found->hi, found->lo);
    const std::pair<std::optional<std::size_t>, Instrs*> sources[] = {
        {found->hi_def, &out.high_loads}, {found->lo_def, &out.low_loads}};
    for (const auto& [d, loads] : sources) {
      if (d && movable_load(an, *d, i)) {
        loads->push_back(an.instrs[*d]);
        out.moved.insert(*d);
      }
    }
  }
  return out;
}

// Returns the jumps on the high bytes: to `less` when a's is below b's,
// to `greater` when it's above.
Instrs high_jumps(const ir::Type_val& ah, const ir::Type_val& bh, const std::string& less,
                  const std::string& greater) {
  if (is_zero(ah)) {
    return {jcmp(ir::NotEqual{}, bh, ah, less)};
  }
  if (is_zero(bh)) {
    return {jcmp(ir::NotEqual{}, ah, bh, greater)};
  }
  return {jcmp(ir::LessThan{}, ah, bh, less), jcmp(ir::NotEqual{}, ah, bh, greater)};
}

// Returns the instructions before the compare at `i`, less the loads
// moved down to it.
Instrs before(const Instrs& instrs, std::size_t i, const Split& s) {
  Instrs out;
  for (std::size_t k = 0; k < i; ++k) {
    if (!s.moved.contains(k)) {
      out.push_back(instrs[k]);
    }
  }
  return out;
}

void append(Instrs& out, const Instrs& more) { out.insert(out.end(), more.begin(), more.end()); }

std::optional<Instrs> rewrite_adjacent(const Analysis& an, std::size_t i, SymbolTable& symbols,
                                       Names& names) {
  const Instrs& instrs = an.instrs;
  const auto& j1 = std::get<ir::JumpIfCmp>(instrs[i]);
  const auto& j2 = std::get<ir::JumpIfCmp>(instrs[i + 1]);
  // Where each outcome goes, none for the fall-through.
  std::optional<std::string> dest[3];
  for (int o = 0; o < 3; ++o) {
    if (holds(j1.op) & kOutcomes[o]) {
      dest[o] = j1.target;
    } else if (holds(j2.op) & kOutcomes[o]) {
      dest[o] = j2.target;
    }
  }
  if (dest[0] == dest[1] || dest[0] == dest[2] || dest[1] == dest[2]) {
    return std::nullopt;
  }
  std::optional<Split> s = split(j1.src1, j1.src2, an, i, symbols, names);
  if (!s) {
    return std::nullopt;
  }
  const auto& [ah, al] = s->pairs[0];
  const auto& [bh, bl] = s->pairs[1];
  std::optional<std::string> fall;
  const auto place = [&](const std::optional<std::string>& d) -> std::string {
    if (!d) {
      if (!fall) {
        fall = names.label();
      }
      return *fall;
    }
    return *d;
  };
  const std::string less_place = place(dest[0]);
  const std::string greater_place = place(dest[2]);
  Instrs out = s->high_pre;
  append(out, s->high_loads);
  append(out, high_jumps(ah, bh, less_place, greater_place));
  append(out, s->low_pre);
  append(out, s->low_loads);
  if (!dest[1]) {
    out.push_back(jcmp(ir::LessThan{}, al, bl, *dest[0]));
    out.push_back(jcmp(ir::NotEqual{}, al, bl, *dest[2]));
  } else if (!dest[0]) {
    out.push_back(jcmp(ir::Equal{}, al, bl, *dest[1]));
    out.push_back(jcmp(ir::GreaterOrEqual{}, al, bl, *dest[2]));
  } else {
    out.push_back(jcmp(ir::LessThan{}, al, bl, *dest[0]));
    out.push_back(jcmp(ir::Equal{}, al, bl, *dest[1]));
  }
  if (fall) {
    out.push_back(ir::Label{.name = *fall});
  }
  Instrs result = before(instrs, i, *s);
  append(result, out);
  result.insert(result.end(), instrs.begin() + static_cast<std::ptrdiff_t>(i + 2), instrs.end());
  return result;
}

std::optional<Instrs> rewrite_at_target(const Analysis& an, std::size_t i, SymbolTable& symbols,
                                        Names& names) {
  const Instrs& instrs = an.instrs;
  const auto& j1 = std::get<ir::JumpIfCmp>(instrs[i]);
  const ir::Type_val* a = &j1.src1;
  const ir::Type_val* b = &j1.src2;
  if (asdl::isa<ir::LessOrEqual>(j1.op)) {
    std::swap(a, b);
  } else if (!asdl::isa<ir::GreaterOrEqual>(j1.op)) {
    return std::nullopt;
  }
  std::optional<std::size_t> t;
  for (std::size_t k = i + 1; k < instrs.size(); ++k) {
    const auto* lab = asdl::as<ir::Label>(instrs[k]);
    if (lab != nullptr && lab->name == j1.target) {
      t = k;
      break;
    }
  }
  // X runs from the compare to T1 and ends in a terminator, so nothing
  // falls into the block that goes in front of T1.
  if (!t || *t == i + 1 || !asdl::isa<ir::Ret, ir::Jump>(instrs[*t - 1])) {
    return std::nullopt;
  }
  std::size_t k = *t;
  while (k < instrs.size() && asdl::isa<ir::Label>(instrs[k])) {
    ++k;
  }
  // Copies that only the equality compare reads, such as the `int` to
  // `unsigned` cast of a widened byte, go with it.
  while (k < instrs.size()) {
    const auto* copy = asdl::as<ir::Copy>(instrs[k]);
    if (copy == nullptr) {
      break;
    }
    const auto* dst = asdl::as<ir::Var>(copy->dst);
    if (dst == nullptr || !dst->name.starts_with('%') || an.use_count(dst->name) != 1) {
      break;
    }
    ++k;
  }
  if (k >= instrs.size() || !same_equality(&instrs[k], j1, an)) {
    return std::nullopt;
  }
  const auto& j2 = std::get<ir::JumpIfCmp>(instrs[k]);
  std::optional<Split> s = split(*a, *b, an, i, symbols, names);
  if (!s) {
    return std::nullopt;
  }
  const auto& [ah, al] = s->pairs[0];
  const auto& [bh, bl] = s->pairs[1];
  std::map<std::string, std::size_t, std::less<>> refs;
  for (const auto& ins : instrs) {
    if (const std::string* target = jump_target(ins)) {
      ++refs[*target];
    }
  }
  bool removable = true;
  for (std::size_t m = *t; m < k; ++m) {
    const auto* lab = asdl::as<ir::Label>(instrs[m]);
    if (lab == nullptr) {
      continue;
    }
    const auto it = refs.find(lab->name);
    const std::size_t count = it == refs.end() ? 0 : it->second;
    if (count != (lab->name == j1.target ? 1u : 0u)) {
      removable = false;
    }
  }
  // Where the second compare sends the equal and greater outcomes.
  const std::size_t after = k + 1;
  std::optional<std::string> after_label;
  std::string greater;
  ir::Type_binary_operator low_op;
  if (asdl::isa<ir::Equal>(j2.op)) {
    const auto* lab = after < instrs.size() ? asdl::as<ir::Label>(instrs[after]) : nullptr;
    after_label = lab != nullptr ? lab->name : names.label();
    greater = *after_label;
    low_op = ir::Equal{};
  } else {
    greater = j2.target;
    low_op = ir::NotEqual{};
  }
  const std::string less = names.label();
  const std::string same_or_more = names.label();
  const Instrs high = high_jumps(ah, bh, less, greater);
  Instrs out = s->high_pre;
  append(out, s->high_loads);
  append(out, high);
  append(out, s->low_pre);
  append(out, s->low_loads);
  out.push_back(jcmp(ir::GreaterOrEqual{}, al, bl, same_or_more));
  bool jumps_to_less = false;
  for (const auto& ins : high) {
    const std::string* target = jump_target(ins);
    jumps_to_less |= target != nullptr && *target == less;
  }
  if (jumps_to_less) {
    out.push_back(ir::Label{.name = less});
  }
  out.insert(out.end(), instrs.begin() + static_cast<std::ptrdiff_t>(i + 1),
             instrs.begin() + static_cast<std::ptrdiff_t>(*t));
  out.push_back(ir::Label{.name = same_or_more});
  out.push_back(jcmp(low_op, al, bl, j2.target));
  if (!removable) {
    out.insert(out.end(), instrs.begin() + static_cast<std::ptrdiff_t>(*t),
               instrs.begin() + static_cast<std::ptrdiff_t>(k + 1));
  }
  Instrs result = before(instrs, i, *s);
  append(result, out);
  if (after_label) {
    const auto* first = after < instrs.size() ? asdl::as<ir::Label>(instrs[after]) : nullptr;
    if (!(first != nullptr && first->name == *after_label)) {
      result.push_back(ir::Label{.name = *after_label});
    }
  }
  if (after < instrs.size()) {
    result.insert(result.end(), instrs.begin() + static_cast<std::ptrdiff_t>(after), instrs.end());
  }
  return result;
}

// Returns the instruction list with the compares at `instrs[i]` split,
// or none.
std::optional<Instrs> rewrite_at(const Instrs& instrs, std::size_t i, SymbolTable& symbols,
                                 Names& names) {
  const auto* j1 = asdl::as<ir::JumpIfCmp>(instrs[i]);
  if (!(j1 != nullptr && is_ordering(j1->op) && qualifies(j1->src1, j1->src2, symbols))) {
    return std::nullopt;
  }
  if (i > 0 && subtracts(instrs[i - 1], *j1)) {
    return std::nullopt;
  }
  const Analysis an(instrs);
  const ir::Type_instruction* nxt = i + 1 < instrs.size() ? &instrs[i + 1] : nullptr;
  if (same_equality(nxt, *j1, an)) {
    return rewrite_adjacent(an, i, symbols, names);
  }
  return rewrite_at_target(an, i, symbols, names);
}

}  // namespace

ir::Function split_three_way_compares(ir::Function fn, const PassContext& ctx) {
  if (ctx.symbols == nullptr) {
    return fn;
  }
  Names names(fn, ctx.symbols);
  Instrs instrs = fn.instructions;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::optional<Instrs> rewritten = rewrite_at(instrs, i, *ctx.symbols, names);
    if (rewritten) {
      instrs = std::move(*rewritten);
      changed = true;
    }
    ++i;
  }
  if (!changed) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(instrs));
}

}  // namespace c6502::passes
