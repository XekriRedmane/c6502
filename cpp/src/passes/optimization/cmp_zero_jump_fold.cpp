/* TAC rules that fold `Binary(cmp_op, src1, src2, cond);
JumpIfTrue/False(cond, t)`, with a single-use `cond`, into one direct
conditional jump.

There are two kinds of fold, by the shape of the comparison.

Equal and NotEqual against zero are the pattern that motivated the pass.
C99 `if (a == 0) ...` for a `uint8_t a` produces this TAC.

    ZeroExtend(a, %0)              # a is uint8_t, promoted to int
    Binary(Equal, %0, ConstInt(0), %cond)
    JumpIfFalse(%cond, .else)

Without the fold, `tac_to_asm` lowers the Binary as a multi-byte CMP and
a select of 0 or 1, and then JumpIfFalse loads %cond and branches on it,
about 13 instructions for what should be `LDA a; BNE .else`. The rule
rewrites it to `JumpIfTrue(a, .else)`, with the sense flipped, and traces
through the ZeroExtend so the test happens at the source's narrow width.

Any other comparison, and `==` or `!=` against a constant other than 0,
becomes the TAC instruction `JumpIfCmp(op, src1, src2, t)`, which
`tac_to_asm` lowers as a per-byte compare chain ending in one `Branch`,
with no 0 or 1 to materialize. The op is inverted when the original jump
was a JumpIfFalse (`<` becomes `>=`, `==` becomes `!=`, and so on), so a
JumpIfCmp always jumps when its op is true.

The operands narrow through a ZeroExtend. When one operand is the dst of
a `ZeroExtend(narrow_var)` upstream (SSA-renamed, or else with no other
use, see `narrowable_use`) and `narrow_var` has a 1-byte unsigned type
(`char` or `unsigned char`), and the other operand either traces to the
same kind of narrow Var or is an integer constant from 0 to 255, both
operands narrow to 1 byte. A signed-int `(int)uint8 < 105` then lowers as
a 1-byte unsigned CMP, `LDA a; CMP #105; BCS .end` for the JumpIfFalse
sense, in place of the 16-bit SBC chain.

The operands narrow through a SignExtend too. If one operand is the dst
of a `SignExtend(narrow_var)` whose source has the 1-byte signed type
`signed char`, and the other is the same kind of value or an integer
constant from -128 to 127, both narrow to 1 byte, the constant as a
`ConstChar`. A SignExtend keeps the value, so the signed comparison of
the bytes has the same result. Against 0, `JumpIfCmp(GE/LT, schar_var,
ConstChar(0), t)` lowers through the asm path for a relational compare
with zero (no SBC and no correction for overflow) into a bare `LDA
schar_var; B<PL|MI> t`, which is what the tail test of a rotated signed
countdown for-loop becomes, `DEC mem; LDA mem; BPL .top`.

The compare's dst becomes dead either way, and dead-store elimination
removes it along with any ZeroExtend before it whose dst only it read.

A comparison whose result is kept as a value, `return x < 10;` or `n +=
c < 20;`, narrows its operands the same way (`narrow_value_compare_at`),
and its dst keeps its width, since it holds 0 or 1 whatever the
operands' width. `(int)x < 10` on a `uint8_t x` then lowers to `LDA x;
CMP #$0A; BCC` where it had lowered to `SEC; SBC #$0A; LDA #$00; SBC
#$00; BVC; EOR #$80; BMI`. That rule comes after `CMP_ZERO_RULE` in the
same rule set, so a compare that a jump tests becomes a `JumpIfCmp`
first.

The use count of `cond` must be exactly 1, the jump's read, since
removing the Binary would break any other read, such as a return of it.
`cond` also can't be a static or a local whose address is taken, since
other functions or a load through a pointer read those without a use in
this function (`framework::single_use`). The rules run inside the
fixed-point loop in SSA form, so each SSA name has one definition.

Only a Binary immediately followed by the jump is taken. The shapes
c99_to_tac writes for `if (...)`, `while (...)`, and `?:` are adjacent,
and other cases wait for copy propagation and dead-store elimination to
close the gap in later rounds of the fixed point. */
#include "passes/optimization/cmp_zero_jump_fold.h"

#include <string>
#include <utility>

#include "bit_precise.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using framework::Instrs;
using framework::PassContext;
using framework::RuleEnv;
using K = ir::Kind_instruction;
using BK = ir::Kind_binary_operator;
using UseCount = OrderedMap<std::string, std::size_t>;
using DefIdx = std::map<std::string, std::size_t, std::less<>>;

// Returns whether `op` is one of the six comparisons (`_CMP_OPS`).
bool is_compare_op(const ir::Type_binary_operator& op) {
  switch (ir::kind(op)) {
    case BK::Equal:
    case BK::NotEqual:
    case BK::LessThan:
    case BK::GreaterThan:
    case BK::LessOrEqual:
    case BK::GreaterOrEqual:
      return true;
    default:
      return false;
  }
}

// Returns whether `instr` only writes the Vars that `defs_in` returns
// (`_PURE_DEF_INSTRS`). A store or a call can write a static or an
// address-taken local without naming it, and a label or a jump means
// that the order of the list between two instructions doesn't cover
// every path between them, so `extend_source_unchanged` refuses on
// anything else.
bool is_pure_def(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::SignExtend:
    case K::ZeroExtend:
    case K::Truncate:
    case K::IntToFloat:
    case K::IntToDouble:
    case K::FloatToInt:
    case K::DoubleToInt:
    case K::FloatToDouble:
    case K::DoubleToFloat:
    case K::GetAddress:
    case K::Load:
    case K::IndexedLoad:
    case K::IndexedConstLoad:
    case K::IndirectIndexedLoad:
    case K::Unary:
    case K::Binary:
    case K::Copy:
      return true;
    default:
      return false;
  }
}

const ir::Type_instruction* entry(const Instrs& instrs, std::size_t k) { return &instrs[k]; }

const ir::Type_instruction* entry(const std::vector<std::optional<ir::Type_instruction>>& instrs,
                                  std::size_t k) {
  return instrs[k] ? &*instrs[k] : nullptr;
}

template <class List>
bool source_unchanged(const ir::Type_val& src, const ir::Type_val& dst, std::ptrdiff_t ext_idx,
                      std::ptrdiff_t use_idx, const List& instrs, const NameSet* ssa_dsts) {
  const auto* s = std::get_if<ir::Var>(&src);
  if (s == nullptr) {
    return false;
  }
  if (ssa_dsts != nullptr) {
    const auto* d = std::get_if<ir::Var>(&dst);
    if (d == nullptr || !ssa_dsts->contains(d->name)) {
      return false;
    }
    if (ssa_dsts->contains(s->name)) {
      return true;
    }
  }
  if (ext_idx >= use_idx) {
    return false;
  }
  for (std::ptrdiff_t k = ext_idx + 1; k < use_idx; ++k) {
    const ir::Type_instruction* ins = entry(instrs, static_cast<std::size_t>(k));
    if (ins == nullptr || !is_pure_def(*ins)) {
      return false;
    }
    for (const auto name : defs_in(*ins)) {
      if (name == s->name) {
        return false;
      }
    }
  }
  return true;
}

// Returns how many times `use_count` counts `name`, 0 when it isn't
// there.
std::size_t count_of(const UseCount& use_count, std::string_view name) {
  const std::size_t* n = use_count.find(name);
  return n == nullptr ? 0 : *n;
}

// Returns the index of `name`'s definition, or none.
std::optional<std::size_t> index_of(const DefIdx& def_idx, std::string_view name) {
  const auto it = def_idx.find(name);
  if (it == def_idx.end()) {
    return std::nullopt;
  }
  return it->second;
}

// Returns whether `c` is an integer constant (`_INTEGER_CONSTS`, which
// takes the odd widths of the bit-precise types too), with its value.
std::optional<asdl::Int> integer_value(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c == nullptr) {
    return std::nullopt;
  }
  return bit_precise::raw_value(c->const_);
}

// Returns whether a compare's read of the extend dst `val` may be
// replaced by a read of the extend's source (Python's
// `_narrowable_use`). An SSA-renamed dst has one definition, so the
// index of definitions finds the extend that gives every use its value,
// and the extend stays for the other uses until dead-store elimination
// drops it once none are left. Without SSA names, the dst must have this
// one use.
bool narrowable_use(const ir::Var& val, const UseCount& use_count, const NameSet* ssa_dsts) {
  if (ssa_dsts != nullptr && ssa_dsts->contains(val.name)) {
    return true;
  }
  return count_of(use_count, val.name) == 1;
}

// An operand narrowed through an extend: the extend's source and the
// source's c99 type.
struct Narrowed {
  ir::Var src;
  const c99::Type_data_type* type;
};

// The analyses the narrowing reads: the instructions, the index of
// definitions, the use counts, the symbols, the compare's position, and
// the SSA names.
struct Scope {
  const Instrs& instrs;
  const DefIdx& def_idx;
  const UseCount& use_count;
  const SymbolTable* symbols;
  std::ptrdiff_t use_idx;
  const NameSet* ssa_dsts;
};

// Returns the source of the ZeroExtend that defines `val` and its type,
// when the source holds at the compare the value it held at the extend
// (Python's `_try_narrow_through_zero_extend`), or none. The dst may
// have other uses (`narrowable_use`), as a switch's controlling value
// has one per case.
std::optional<Narrowed> narrow_through_zero_extend(const ir::Type_val& val, const Scope& s) {
  const auto* v = std::get_if<ir::Var>(&val);
  if (v == nullptr || !narrowable_use(*v, s.use_count, s.ssa_dsts)) {
    return std::nullopt;
  }
  std::optional<std::size_t> def_idx = index_of(s.def_idx, v->name);
  if (!def_idx) {
    return std::nullopt;
  }
  const ir::Type_instruction* defining = &s.instrs[*def_idx];
  // A `Copy` between an `int` and an `unsigned int` is how a promoted
  // byte meets an unsigned operand, as in `i < sizeof a`, and copy
  // propagation keeps it, since the two differ in signedness. A Copy
  // doesn't change the width, and the extend's value is 0 to 255 either
  // way, so the compare reads the extend's source just the same.
  int seen = 0;
  while (seen < 4) {
    const auto* copy = std::get_if<ir::Copy>(defining);
    if (copy == nullptr) {
      break;
    }
    const auto* src = std::get_if<ir::Var>(&copy->src);
    if (src == nullptr) {
      break;
    }
    ++seen;
    if (!narrowable_use(*src, s.use_count, s.ssa_dsts)) {
      return std::nullopt;
    }
    def_idx = index_of(s.def_idx, src->name);
    if (!def_idx) {
      return std::nullopt;
    }
    defining = &s.instrs[*def_idx];
  }
  const auto* ext = std::get_if<ir::ZeroExtend>(defining);
  if (ext == nullptr) {
    return std::nullopt;
  }
  const auto* src = std::get_if<ir::Var>(&ext->src);
  if (src == nullptr) {
    return std::nullopt;
  }
  if (!source_unchanged(ext->src, ext->dst, static_cast<std::ptrdiff_t>(*def_idx), s.use_idx,
                        s.instrs, s.ssa_dsts)) {
    return std::nullopt;
  }
  if (s.symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = s.symbols->find(src->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  return Narrowed{*src, &sym->type};
}

// The mirror of `narrow_through_zero_extend` for a SignExtend, with no
// Copy looked through (Python's `_try_narrow_through_sign_extend`).
std::optional<Narrowed> narrow_through_sign_extend(const ir::Type_val& val, const Scope& s) {
  const auto* v = std::get_if<ir::Var>(&val);
  if (v == nullptr || !narrowable_use(*v, s.use_count, s.ssa_dsts)) {
    return std::nullopt;
  }
  const std::optional<std::size_t> def_idx = index_of(s.def_idx, v->name);
  if (!def_idx) {
    return std::nullopt;
  }
  const auto* ext = std::get_if<ir::SignExtend>(&s.instrs[*def_idx]);
  if (ext == nullptr) {
    return std::nullopt;
  }
  const auto* src = std::get_if<ir::Var>(&ext->src);
  if (src == nullptr) {
    return std::nullopt;
  }
  if (!source_unchanged(ext->src, ext->dst, static_cast<std::ptrdiff_t>(*def_idx), s.use_idx,
                        s.instrs, s.ssa_dsts)) {
    return std::nullopt;
  }
  if (s.symbols == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = s.symbols->find(src->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  return Narrowed{*src, &sym->type};
}

// Returns an integer constant from 0 to 255 as a `ConstUChar`, or none.
std::optional<ir::Type_val> narrow_const_to_unsigned_byte(const ir::Type_val& val) {
  const std::optional<asdl::Int> c = integer_value(val);
  if (!c || *c < 0 || *c > 255) {
    return std::nullopt;
  }
  return ir::Constant{.const_ = ir::ConstUChar{.value = *c}};
}

// Returns an integer constant from -128 to 127 as a `ConstChar`, which
// tac_to_asm compares as signed, or none.
std::optional<ir::Type_val> narrow_const_to_signed_byte(const ir::Type_val& val) {
  const std::optional<asdl::Int> c = integer_value(val);
  if (!c || *c < -128 || *c > 127) {
    return std::nullopt;
  }
  return ir::Constant{.const_ = ir::ConstChar{.value = *c}};
}

using Pair = std::pair<ir::Type_val, ir::Type_val>;

// Narrows both operands of a compare to 1-byte unsigned (Python's
// `_try_narrow_compare`). At least one operand is a ZeroExtend dst
// whose source is `char` or `unsigned char`, and the other is either
// such a Var too or an integer constant from 0 to 255. Returns the
// narrowed pair, or none.
std::optional<Pair> try_narrow_compare(const ir::Type_val& src1, const ir::Type_val& src2,
                                       const Scope& s) {
  if (s.symbols == nullptr) {
    return std::nullopt;
  }
  const auto info1 = narrow_through_zero_extend(src1, s);
  const auto info2 = narrow_through_zero_extend(src2, s);
  const bool n1_ok = info1 && cmp_zero_jump_fold::is_narrow_unsigned_type(*info1->type);
  const bool n2_ok = info2 && cmp_zero_jump_fold::is_narrow_unsigned_type(*info2->type);
  if (n1_ok && n2_ok) {
    return Pair{info1->src, info2->src};
  }
  if (n1_ok) {
    if (auto nc = narrow_const_to_unsigned_byte(src2)) {
      return Pair{info1->src, std::move(*nc)};
    }
  }
  if (n2_ok) {
    if (auto nc = narrow_const_to_unsigned_byte(src1)) {
      return Pair{std::move(*nc), info2->src};
    }
  }
  return std::nullopt;
}

// Narrows a signed comparison whose operands are each a
// `SignExtend(signed char)` dst or an integer constant from -128 to 127,
// at least one of them an extend (Python's
// `_try_narrow_signed_compare`). Returns the narrowed pair, each
// constant a `ConstChar`, or none. A SignExtend keeps the value, and so
// does a constant in that range, so the 1-byte signed comparison has the
// same result. `(int8_t)(a - b) > 0` had compared at 16 bits.
std::optional<Pair> try_narrow_signed_compare(const ir::Type_val& src1,
                                              const ir::Type_val& src2, const Scope& s) {
  if (s.symbols == nullptr) {
    return std::nullopt;
  }
  const auto info1 = narrow_through_sign_extend(src1, s);
  const auto info2 = narrow_through_sign_extend(src2, s);
  const bool n1_ok = info1 && std::holds_alternative<c99::SChar>(*info1->type);
  const bool n2_ok = info2 && std::holds_alternative<c99::SChar>(*info2->type);
  if (n1_ok && n2_ok) {
    return Pair{info1->src, info2->src};
  }
  if (n1_ok) {
    if (auto nc = narrow_const_to_signed_byte(src2)) {
      return Pair{info1->src, std::move(*nc)};
    }
  }
  if (n2_ok) {
    if (auto nc = narrow_const_to_signed_byte(src1)) {
      return Pair{std::move(*nc), info2->src};
    }
  }
  return std::nullopt;
}

// Returns whether `val` is an integer constant whose value is 0.
bool is_constant_zero(const ir::Type_val& val) {
  const std::optional<asdl::Int> c = integer_value(val);
  return c && *c == 0;
}

// Returns the other operand when exactly one of `a` and `b` is an
// integer constant 0, or null.
const ir::Type_val* zero_compare_other_operand(const ir::Type_val& a, const ir::Type_val& b) {
  const bool a_zero = is_constant_zero(a);
  const bool b_zero = is_constant_zero(b);
  if (a_zero && !b_zero) {
    return &b;
  }
  if (b_zero && !a_zero) {
    return &a;
  }
  return nullptr;
}

// Returns the source of the ZeroExtend that defines `x`, looking through
// a chain of them, while each extend's source holds at the compare the
// value it held at the extend, and `x` itself otherwise (Python's
// `_trace_through_zero_extend`).
ir::Type_val trace_through_zero_extend(ir::Type_val x, const Scope& s) {
  for (;;) {
    const auto* v = std::get_if<ir::Var>(&x);
    if (v == nullptr || !narrowable_use(*v, s.use_count, s.ssa_dsts)) {
      break;
    }
    const std::optional<std::size_t> def_idx = index_of(s.def_idx, v->name);
    if (!def_idx) {
      break;
    }
    const auto* ext = std::get_if<ir::ZeroExtend>(&s.instrs[*def_idx]);
    if (ext == nullptr) {
      break;
    }
    if (!source_unchanged(ext->src, ext->dst, static_cast<std::ptrdiff_t>(*def_idx), s.use_idx,
                          s.instrs, s.ssa_dsts)) {
      break;
    }
    ir::Type_val next = ext->src;
    x = std::move(next);
  }
  return x;
}

// Returns the op a JumpIfCmp tests for the compare `op` and the jump
// that tested its result (Python's `_adjusted_op_for_jumpif`). A
// JumpIfCmp jumps when its op is true, so the op of a JumpIfFalse is
// inverted.
ir::Type_binary_operator adjusted_op_for_jumpif(const ir::Type_binary_operator& op,
                                                bool jump_if_true) {
  if (jump_if_true) {
    return op;
  }
  switch (ir::kind(op)) {
    case BK::Equal:
      return ir::NotEqual{};
    case BK::NotEqual:
      return ir::Equal{};
    case BK::LessThan:
      return ir::GreaterOrEqual{};
    case BK::GreaterOrEqual:
      return ir::LessThan{};
    case BK::GreaterThan:
      return ir::LessOrEqual{};
    case BK::LessOrEqual:
      return ir::GreaterThan{};
    default:
      throw std::logic_error("adjusted_op_for_jumpif: not a comparison");
  }
}

// Returns the jump on `x` for an Equal or NotEqual against zero that
// the jump `jump_if_true` tested (Python's `_build_replacement_jump`).
// NotEqual keeps the jump's sense and Equal flips it.
ir::Type_instruction replacement_jump(bool is_equal, bool jump_if_true, ir::Type_val x,
                                      const std::string& target) {
  const bool new_is_true = is_equal ? !jump_if_true : jump_if_true;
  if (new_is_true) {
    return ir::JumpIfTrue{.condition = std::move(x), .target = target};
  }
  return ir::JumpIfFalse{.condition = std::move(x), .target = target};
}

// The jump of a `producer_then_jump` window: a JumpIfTrue or
// JumpIfFalse whose condition is `on`, with its sense and target.
struct WindowJump {
  bool jump_if_true;
  const std::string* target;
};

std::optional<WindowJump> jump_on(const ir::Type_instruction& instr, const ir::Var& on) {
  if (const auto* t = std::get_if<ir::JumpIfTrue>(&instr)) {
    const auto* c = std::get_if<ir::Var>(&t->condition);
    if (c != nullptr && c->name == on.name) {
      return WindowJump{true, &t->target};
    }
    return std::nullopt;
  }
  if (const auto* f = std::get_if<ir::JumpIfFalse>(&instr)) {
    const auto* c = std::get_if<ir::Var>(&f->condition);
    if (c != nullptr && c->name == on.name) {
      return WindowJump{false, &f->target};
    }
  }
  return std::nullopt;
}

}  // namespace

bool extend_source_unchanged(const ir::Type_val& src, const ir::Type_val& dst,
                             std::ptrdiff_t ext_idx, std::ptrdiff_t use_idx,
                             const Instrs& instrs, const NameSet* ssa_dsts) {
  return source_unchanged(src, dst, ext_idx, use_idx, instrs, ssa_dsts);
}

bool extend_source_unchanged(const ir::Type_val& src, const ir::Type_val& dst,
                             std::ptrdiff_t ext_idx, std::ptrdiff_t use_idx,
                             const std::vector<std::optional<ir::Type_instruction>>& instrs,
                             const NameSet* ssa_dsts) {
  return source_unchanged(src, dst, ext_idx, use_idx, instrs, ssa_dsts);
}

namespace cmp_zero_jump_fold {

bool is_narrow_unsigned_type(const c99::Type_data_type& t) {
  return std::holds_alternative<c99::Char>(t) || std::holds_alternative<c99::UChar>(t);
}

// Python's `_build_cmp_zero_jump`. `== 0` and `!= 0` trace through
// ZeroExtends to a bare jump with the sense flipped as needed, and
// anything else becomes a `JumpIfCmp` (the op inverted for the
// JumpIfFalse sense), with both operands narrowed to 1 byte where the
// ZeroExtend or SignExtend chain permits.
std::optional<Instrs> fold_cmp_zero_jump_at(const Instrs& instrs, std::size_t i, RuleEnv& env,
                                            const PassContext& ctx) {
  const auto* binop = std::get_if<ir::Binary>(&instrs[i]);
  if (binop == nullptr || !is_compare_op(binop->op)) {
    return std::nullopt;
  }
  const auto* cond = std::get_if<ir::Var>(&binop->dst);
  if (cond == nullptr) {
    return std::nullopt;
  }
  const std::optional<WindowJump> jmp = jump_on(instrs[i + 1], *cond);
  if (!jmp || !framework::single_use(binop->dst, env, ctx)) {
    return std::nullopt;
  }
  // The compare's own position. Each narrowing below replaces an
  // extend's dst with the extend's source here, so the source must hold
  // the same value here as at the extend. A non-SSA cond dst written
  // again later has its last definition elsewhere, and then -1 lets only
  // an SSA-renamed extend source qualify.
  std::ptrdiff_t use_idx = static_cast<std::ptrdiff_t>(env.def_idx.at(cond->name));
  if (use_idx != static_cast<std::ptrdiff_t>(i)) {
    use_idx = -1;
  }
  const Scope s{instrs, env.def_idx, env.use_count, ctx.symbols, use_idx, ctx.ssa_dsts};

  const BK op = ir::kind(binop->op);
  if (op == BK::Equal || op == BK::NotEqual) {
    if (const ir::Type_val* x = zero_compare_other_operand(binop->src1, binop->src2)) {
      return Instrs{replacement_jump(op == BK::Equal, jmp->jump_if_true,
                                     trace_through_zero_extend(*x, s), *jmp->target)};
    }
  }
  ir::Type_val src1 = binop->src1;
  ir::Type_val src2 = binop->src2;
  if (auto narrowed = try_narrow_compare(src1, src2, s)) {
    src1 = std::move(narrowed->first);
    src2 = std::move(narrowed->second);
  } else if (auto signed_pair = try_narrow_signed_compare(src1, src2, s)) {
    src1 = std::move(signed_pair->first);
    src2 = std::move(signed_pair->second);
  }
  return Instrs{ir::JumpIfCmp{.op = adjusted_op_for_jumpif(binop->op, jmp->jump_if_true),
                              .src1 = std::move(src1),
                              .src2 = std::move(src2),
                              .target = *jmp->target}};
}

// Python's `_build_narrow_value_compare`: `a op b` with each operand
// narrowed to 1-byte unsigned through its ZeroExtend, into the same dst.
std::optional<Instrs> narrow_value_compare_at(const Instrs& instrs, std::size_t i, RuleEnv& env,
                                              const PassContext& ctx) {
  const auto* binop = std::get_if<ir::Binary>(&instrs[i]);
  if (binop == nullptr || !is_compare_op(binop->op)) {
    return std::nullopt;
  }
  const auto* dst = std::get_if<ir::Var>(&binop->dst);
  if (dst == nullptr) {
    return std::nullopt;
  }
  const std::optional<std::size_t> at = index_of(env.def_idx, dst->name);
  const std::ptrdiff_t use_idx = at && *at == i ? static_cast<std::ptrdiff_t>(i) : -1;
  const Scope s{instrs, env.def_idx, env.use_count, ctx.symbols, use_idx, ctx.ssa_dsts};
  auto narrowed = try_narrow_compare(binop->src1, binop->src2, s);
  if (!narrowed) {
    return std::nullopt;
  }
  return Instrs{ir::Binary{.op = binop->op,
                           .src1 = std::move(narrowed->first),
                           .src2 = std::move(narrowed->second),
                           .dst = binop->dst}};
}

// Python's `_build_narrow_jump_compare`. A `JumpIfCmp` whose operands
// narrow to 1-byte unsigned compares the narrow operands. The compare
// was folded into the jump before one operand's `ZeroExtend` reached it:
// `i < 40U` on a `uint8_t i` converts the promoted `int` to `unsigned
// int` with a `Copy`, which copy propagation removes only after the
// fold. Python finds the jump's position by identity, and this takes it
// from the scan, which lets a global extend source, read earlier in the
// same straight run, narrow too. `T < M` with a `constexpr uint8_t M`
// became a compare of `T` and 99 only after the fold, since `M` was a
// variable until its read was folded.
std::optional<Instrs> narrow_jump_compare_at(const Instrs& instrs, std::size_t i, RuleEnv& env,
                                             const PassContext& ctx) {
  const auto* jmp = std::get_if<ir::JumpIfCmp>(&instrs[i]);
  if (jmp == nullptr || !is_compare_op(jmp->op)) {
    return std::nullopt;
  }
  const Scope s{instrs,      env.def_idx, env.use_count, ctx.symbols, static_cast<std::ptrdiff_t>(i),
                ctx.ssa_dsts};
  auto narrowed = try_narrow_compare(jmp->src1, jmp->src2, s);
  if (!narrowed) {
    return std::nullopt;
  }
  return Instrs{ir::JumpIfCmp{.op = jmp->op,
                              .src1 = std::move(narrowed->first),
                              .src2 = std::move(narrowed->second),
                              .target = jmp->target}};
}

}  // namespace cmp_zero_jump_fold

}  // namespace c6502::passes
