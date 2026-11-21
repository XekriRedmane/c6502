/* TAC loop rotation before SSA, for signed count-down `for` loops.

The pass recognizes the canonical `c99_to_tac` shape of a `for` loop

    [<init>]                                ; ends with x_var := nonneg-const
    Label(<L>_start)
    [<entry cond>]                          ; emits cond_var
    JumpIfFalse(cond_var, <L>_break)
    [<body>]
    Label(<L>_continue)
    [<post>]                                ; ends with x_var := x_var - 1
    Jump(<L>_start)
    Label(<L>_break)

and rewrites it to test at the bottom

    [<init>]
    Label(<L>_start)
    [<body, possibly with SignExtend(x_var) → ZeroExtend rewrite>]
    Label(<L>_continue)
    [<post>]
    [<entry cond>]                          ; same insns, NEW position
    JumpIfTrue(cond_var, <L>_start)         ; replaces both old jumps
    Label(<L>_break)

which drops the entry test, since it can be shown to pass on the first
iteration, and folds the unconditional `Jump(<L>_start)` into the back
edge, which is now conditional.

SignExtend to ZeroExtend. Given the rotation's preconditions (the init
is `>= 0`, the post is `x -= 1`, and the test is `x >= 0` at the
bottom), the loop body always sees `x_var` in `[0, init_value]` and
never negative. For a byte that isn't negative, `SignExtend(byte)` and
`ZeroExtend(byte)` give the same bits (the byte with a zero high byte).
So every `SignExtend(Var(x_var.name), _)` in the rotated body is
rewritten to a `ZeroExtend` of the same source and destination.

The gain comes later. `tac_to_asm` lowers `ZeroExtend` as the two
instructions `LDA src; LDA #0; STA dst.hi` with no branch, where a
SignExtend takes the six of `LDA src; BMI .neg; LDA #0; JMP .done;
.neg: LDA #$FF; .done: STA dst.hi`. More importantly, the IndexedStore
and IndexedConstLoad recognizers require a ZeroExtend of the index, so
this rewrite lets them fire for loops with a signed counter. The body
of `for (int8_t x = N; x >= 0; x--)` that uses `x` to index a static
array then becomes absolute,X stores, as the unsigned counter's does.

The gate on the rewrite. The rewrite is sound only when the loop body
never writes `x_var`. Any write of `x_var` in the body could make it
negative (`for (int8_t i = 5; i >= 0; i--) i--;`), and then SignExtend
and ZeroExtend differ where x is read later. The pass declines the
rewrite, but still rotates, when the body writes `x_var`.

Eligibility. The C source the pass targets is

    for (T x = N; x >= 0; x--) { ... }

with N a literal that isn't negative and T a signed integer type
(`int8_t`, `int`, `long`, `long long`). In terms of the matcher:

  * The test computes `x_var >= 0`, either as `Binary(GE, x_var,
    Constant(0), cond_var)` directly or as a SignExtend or Copy chain
    of `x_var` followed by the GE. The chain comes from C99's integer
    promotion of narrower operands (`SChar` to `Int` for the
    comparison).
  * Every instruction of the test computes a `%` temporary with no
    load, store, or call (`computes_temporary`). The rotated loop skips
    the first test, so a test with a side effect, such as the decrement
    in `i-- >= 0`, would lose it.
  * The post block ends with `Binary(Subtract, x_var, Constant(1),
    %t); Copy(%t, x_var)`. A postfix `x--` puts a `Copy(x_var, %old)`
    of the value before the decrement in front of those. That Copy is
    dead in a for loop's post clause and is dropped later, so the
    instructions before the last two aren't inspected.
  * The c99 type of `x_var` is `SChar`, `Int`, `Long`, or `LongLong`.
    `Char` and the unsigned types are declined, since `>= 0` is always
    true for an unsigned type and the rotated loop would never end.
  * The last write of `x_var` before `Label(<L>_start)` comes to a
    constant in `[0, max_signed_for_x_type]`, so that the first
    iteration would have entered the original loop, which tests at the
    top. That is the condition for putting off the test until after one
    run of the body.
  * That write, and every write in its Copy or cast chain, comes after
    the last label before `Label(<L>_start)`. Control enters the
    straight run of instructions between that label and the loop only
    at its top, so the write reaches the loop on every path. A write
    before the label, such as `i = 5` in `if (c) i = 5;`, may be
    skipped.
  * The address of `x_var` is never taken, since a store through a
    pointer or a call could write it between the init and the loop.

Why before SSA. Before SSA, `x_var` has one name across the init, the
body, and the post block, so the rewrite moves ranges of instructions
and renames nothing. After SSA, the entry test reads the Phi's name
(the join of the init and the decrement), and the decrement writes a
new SSA name, so the same move would also have to retag a Phi argument,
which isn't worth the complexity for a rewrite of the same shape.

The pass runs once per function, before `to_ssa`. A second run does
nothing, since a rotated loop has no test and JumpIfFalse before its
body for the matcher to find.

Why not `while` and `do`-`while` too. A `do`-`while` loop already tests
at the bottom. A `while (cond) body` could be rotated by copying the
test in front of the loop, but showing that the first test passes is
harder for an arbitrary `cond` than for a `for` loop bounded by a
literal. The narrow target keeps the soundness gate tight, and widening
it is left for later. */
#include "passes/optimization/loop_rotate.h"

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "errors.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using K = ir::Kind_instruction;
using Instrs = std::vector<ir::Type_instruction>;

// Returns the largest value that an init constant of the signed type
// `t` may have, or none for a type the pass doesn't take. A value in
// `[0, max]` truncates or sign-extends to a bit pattern that isn't
// negative at the width of `x_var`, which is the property the rotation
// requires at run time. `Char` is c6502's plain char, which is unsigned
// (0..255), so only `SChar` of the char types is taken.
std::optional<asdl::Int> max_positive(const c99::Type_data_type& t) {
  switch (c99::kind(t)) {
    case c99::Kind_data_type::SChar:
      return asdl::Int{0x7F};
    case c99::Kind_data_type::Int:
      return asdl::Int{0x7FFF};
    case c99::Kind_data_type::Long:
      return asdl::Int{0x7FFFFFFF};
    case c99::Kind_data_type::LongLong:
      return asdl::Int{0x7FFFFFFFFFFFFFFF};
    default:
      return std::nullopt;
  }
}

// Returns the integer value of a constant of an integer type, or none
// for the floating constants, whose `bits` field is an IEEE 754 bit
// pattern and not a value this pass compares with 0, and for the odd
// widths, which Python's `_INT_CONST_VARIANTS` leaves out.
std::optional<asdl::Int> const_int_value(const ir::Type_const& c) {
  switch (ir::kind(c)) {
    case ir::Kind_const::ConstChar:
      return std::get<ir::ConstChar>(c).value;
    case ir::Kind_const::ConstUChar:
      return std::get<ir::ConstUChar>(c).value;
    case ir::Kind_const::ConstInt:
      return std::get<ir::ConstInt>(c).value;
    case ir::Kind_const::ConstUInt:
      return std::get<ir::ConstUInt>(c).value;
    case ir::Kind_const::ConstLong:
      return std::get<ir::ConstLong>(c).value;
    case ir::Kind_const::ConstULong:
      return std::get<ir::ConstULong>(c).value;
    case ir::Kind_const::ConstLongLong:
      return std::get<ir::ConstLongLong>(c).value;
    case ir::Kind_const::ConstULongLong:
      return std::get<ir::ConstULongLong>(c).value;
    default:
      return std::nullopt;
  }
}

// Returns whether `instr` ends a straight run of the test. The test and
// the post block must each be straight-line code. Labels, jumps, and
// calls in either aren't what `c99_to_tac` emits for the shapes the pass
// targets, so declining them is a soundness rail: such a block could
// never have been a `>= 0` test. A `JumpIfFalse` doesn't end the test,
// since the scan stops at the one to the break label.
bool breaks_cond_block(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Label:
    case K::Jump:
    case K::JumpIfTrue:
    case K::JumpIfCmp:
    case K::JumpIfMasked:
    case K::Ret:
    case K::FunctionCall:
    case K::IndirectCall:
      return true;
    default:
      return false;
  }
}

// Returns whether `instr` ends the straight run of a post block, which
// is the test's set and a `JumpIfFalse`.
bool breaks_post_block(const ir::Type_instruction& instr) {
  return breaks_cond_block(instr) || asdl::isa<ir::JumpIfFalse>(instr);
}

// Returns the source of a `Copy`, `SignExtend`, `ZeroExtend`, or
// `Truncate`, with `truncate` false leaving a Truncate out, or null for
// any other instruction.
const ir::Type_val* cast_source(const ir::Type_instruction& instr, bool truncate = true) {
  switch (ir::kind(instr)) {
    case K::Copy:
      return &std::get<ir::Copy>(instr).src;
    case K::SignExtend:
      return &std::get<ir::SignExtend>(instr).src;
    case K::ZeroExtend:
      return &std::get<ir::ZeroExtend>(instr).src;
    case K::Truncate:
      return truncate ? &std::get<ir::Truncate>(instr).src : nullptr;
    default:
      return nullptr;
  }
}

// Returns the names of the Var operands `instr` writes (Python's
// `_defs_of`, a local copy of var_visit's helper). It counts the same
// destinations as `framework::dst_name` except an
// `IndirectIndexedLoad`'s.
std::optional<std::string_view> defs_of(const ir::Type_instruction& instr) {
  if (asdl::isa<ir::IndirectIndexedLoad>(instr)) {
    return std::nullopt;
  }
  return framework::dst_name(instr);
}

// The index ranges and the test's variable of a loop the pass can
// rotate.
struct Match {
  std::size_t start_idx;      // Label(L_start)
  std::size_t cond_jif_idx;   // JumpIfFalse(cond_var, L_break)
  std::size_t cont_idx;       // Label(L_continue)
  std::size_t jump_back_idx;  // Jump(L_start)
  std::size_t break_idx;      // Label(L_break)
  std::string cond_var_name;
  std::string start_label;
  std::string x_name;  // the counter's name
  // Whether the body writes no definition of x_name, which makes the
  // SignExtend to ZeroExtend rewrite safe.
  bool body_iv_immutable;
};

// Returns the definition of `cond_var` in the test, when it is one
// `Binary(GreaterOrEqual, ?, ?, cond_var)`, or null for any other shape.
// The last Binary that writes `cond_var` is the one tested.
const ir::Binary* find_ge_producer(const Instrs& cond_block, const std::string& cond_var) {
  const ir::Binary* producer = nullptr;
  for (const auto& ins : cond_block) {
    const auto* b = asdl::as<ir::Binary>(ins);
    if (b == nullptr) {
      continue;
    }
    const auto* d = asdl::as<ir::Var>(b->dst);
    if (d == nullptr || d->name != cond_var) {
      continue;
    }
    producer = b;
  }
  if (producer == nullptr || !asdl::isa<ir::GreaterOrEqual>(producer->op)) {
    return nullptr;
  }
  return producer;
}

// Returns whether `val` is 0, either as an integer Constant or through
// a SignExtend, ZeroExtend, Truncate, or Copy chain in the test that
// starts at an integer Constant(0). c99_to_tac emits the chain when the
// literal 0's type is narrower than the comparison's (`long x; x >= 0`
// gives a SignExtend(ConstInt(0), %t) before the GE). Only the first
// definition of a name in the test is followed.
bool val_resolves_to_zero(const ir::Type_val& val, const Instrs& cond_block) {
  if (const auto* c = asdl::as<ir::Constant>(val)) {
    const auto v = const_int_value(c->const_);
    return v && *v == 0;
  }
  const auto* var = asdl::as<ir::Var>(val);
  if (var == nullptr) {
    return false;
  }
  for (const auto& ins : cond_block) {
    const auto d = framework::dst_name(ins);
    if (!d || *d != var->name) {
      continue;
    }
    const ir::Type_val* src = cast_source(ins);
    return src != nullptr && val_resolves_to_zero(*src, cond_block);
  }
  return false;
}

// Returns whether `ins` only computes a `%` temporary from values, with
// no store, load, or call. The first test of a rotated loop is skipped,
// so a test that does more is left alone. The test `i-- >= 0`
// decrements `i`, and `(n++, i) >= 0` increments `n`, and skipping
// either changes what the loop computes.
bool computes_temporary(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case K::Copy:
    case K::SignExtend:
    case K::ZeroExtend:
    case K::Truncate:
    case K::Unary:
    case K::Binary: {
      const auto d = framework::dst_name(ins);
      return d && d->starts_with('%');
    }
    default:
      return false;
  }
}

// Returns the variable on the left of the GE comparison: `val` itself
// when it is a Var that isn't a temporary, or the source of the
// SignExtend, ZeroExtend, or Copy chain in the test that defines the
// temporary `val`. Returns none for anything that doesn't end at a Var
// that isn't a temporary.
std::optional<std::string> trace_to_var(const ir::Type_val& val, const Instrs& cond_block) {
  const auto* var = asdl::as<ir::Var>(val);
  if (var == nullptr) {
    return std::nullopt;
  }
  if (!var->name.starts_with('%')) {
    // Names that start with `@` (locals and parameters) or with neither
    // (statics) are storage names, where the trace stops.
    return var->name;
  }
  for (const auto& ins : cond_block) {
    const auto d = framework::dst_name(ins);
    if (!d || *d != var->name) {
      continue;
    }
    const ir::Type_val* src = cast_source(ins, /*truncate=*/false);
    if (src == nullptr) {
      return std::nullopt;
    }
    return trace_to_var(*src, cond_block);
  }
  return std::nullopt;
}

// Returns whether the last two instructions of `post` are
// `Binary(Subtract, x_var, Constant(1), %t); Copy(%t, x_var)` with
// `x_var` named `x_name`. The instructions before them, such as a
// postfix decrement's `Copy(x, %old)` of the value before it, are
// allowed and not inspected, since the last two make the decrement.
bool post_decrements_x_by_one(const Instrs& post, std::size_t begin, std::size_t end,
                              const std::string& x_name) {
  if (end - begin < 2) {
    return false;
  }
  const auto* sub = asdl::as<ir::Binary>(post[end - 2]);
  if (sub == nullptr || !asdl::isa<ir::Subtract>(sub->op)) {
    return false;
  }
  const auto* s1 = asdl::as<ir::Var>(sub->src1);
  if (s1 == nullptr || s1->name != x_name) {
    return false;
  }
  const auto* s2 = asdl::as<ir::Constant>(sub->src2);
  if (s2 == nullptr) {
    return false;
  }
  const auto one = const_int_value(s2->const_);
  if (!one || *one != 1) {
    return false;
  }
  const auto* sub_dst = asdl::as<ir::Var>(sub->dst);
  if (sub_dst == nullptr) {
    return false;
  }
  const auto* cp = asdl::as<ir::Copy>(post[end - 1]);
  if (cp == nullptr) {
    return false;
  }
  const auto* cs = asdl::as<ir::Var>(cp->src);
  if (cs == nullptr || cs->name != sub_dst->name) {
    return false;
  }
  const auto* cd = asdl::as<ir::Var>(cp->dst);
  return cd != nullptr && cd->name == x_name;
}

// Returns whether any instruction takes the address of `name`.
bool address_taken(const Instrs& instrs, const std::string& name) {
  for (const auto& ins : instrs) {
    const auto* ga = asdl::as<ir::GetAddress>(ins);
    if (ga == nullptr) {
      continue;
    }
    const auto* v = asdl::as<ir::Var>(ga->operand);
    if (v != nullptr && v->name == name) {
      return true;
    }
  }
  return false;
}

// Returns the index of the first instruction after the last `Label`
// before `instrs[start_idx]`, or 0 when there is no such label. Control
// reaches `instrs[start_idx]` only by running every instruction from
// that index on.
std::size_t straight_line_start(const Instrs& instrs, std::size_t start_idx) {
  for (std::size_t k = start_idx; k-- > 0;) {
    if (asdl::isa<ir::Label>(instrs[k])) {
      return k + 1;
    }
  }
  return 0;
}

// Returns the last index below `end_excl` whose instruction defines
// `name`, or none.
std::optional<std::size_t> last_def_pos(const Instrs& instrs, std::size_t end_excl,
                                        std::string_view name) {
  for (std::size_t k = end_excl; k-- > 0;) {
    const auto d = defs_of(instrs[k]);
    if (d && *d == name) {
      return k;
    }
  }
  return std::nullopt;
}

// Returns whether the definition at `instrs[def_idx]`, which writes
// `name`, comes through Copy and cast chains to an integer constant in
// `[low, high]`. Every definition in the chain must sit at index
// `floor` or later, in the same straight run as the loop's init write,
// since a temporary such as the result of `?:` is written on more than
// one path.
bool resolves_to_int_in_range(const Instrs& instrs, std::size_t def_idx, asdl::Int low,
                              asdl::Int high, std::size_t floor, int depth = 0) {
  if (depth > 4) {
    return false;
  }
  const ir::Type_val* src = cast_source(instrs[def_idx]);
  if (src == nullptr) {
    return false;
  }
  if (const auto* c = asdl::as<ir::Constant>(*src)) {
    const auto v = const_int_value(c->const_);
    return v && low <= *v && *v <= high;
  }
  const auto* inner = asdl::as<ir::Var>(*src);
  if (inner == nullptr) {
    return false;
  }
  const auto inner_def = last_def_pos(instrs, def_idx, inner->name);
  if (!inner_def || *inner_def < floor) {
    return false;
  }
  return resolves_to_int_in_range(instrs, *inner_def, low, high, floor, depth + 1);
}

// Returns whether the last definition of `x_name` before
// `instrs[start_idx]` comes to an integer constant in `[0, max]` for
// the counter's type, that definition comes after the last label before
// the loop, and the address of `x_name` is never taken. The walk goes
// backward through Copy and cast chains of temporaries, and declines
// the first step it can't follow, since the only shape taken is
// `c99_to_tac`'s straight-line lowering of a constant init expression.
bool init_value_is_nonnegative(const Instrs& instrs, std::size_t start_idx,
                               const std::string& x_name, const c99::Type_data_type& x_type) {
  const auto max_pos = max_positive(x_type);
  if (!max_pos) {
    return false;
  }
  // A Store through a pointer or a call can write an address-taken
  // x_var, and neither shows up as a definition of x_name.
  if (address_taken(instrs, x_name)) {
    return false;
  }
  // The init write must sit in the straight run that ends at
  // Label(L_start). A label is the only place where control enters the
  // middle of a TAC instruction list, so every path into the loop's
  // first test runs that whole run, and the last write in it is the
  // value the test sees. A write before an earlier label may be on a
  // conditional path that another path skips, as with `if (c) i = 5;`
  // just before the loop.
  const std::size_t floor = straight_line_start(instrs, start_idx);
  const auto def_idx = last_def_pos(instrs, start_idx, x_name);
  if (!def_idx || *def_idx < floor) {
    return false;
  }
  return resolves_to_int_in_range(instrs, *def_idx, 0, *max_pos, floor);
}

// Returns the loop the pass can rotate whose `Label(L_start)` is at
// `instrs[i]`, or none when any condition fails.
std::optional<Match> try_match(const Instrs& instrs, std::size_t i, const SymbolTable* symbols) {
  const auto* label = asdl::as<ir::Label>(instrs[i]);
  if (label == nullptr) {
    return std::nullopt;
  }
  const std::string& l_start = label->name;
  // c99_to_tac's lowering of a for statement names the loop's labels
  // `<base>_start`, `<base>_continue`, and `<base>_break`. Other labels
  // (a do-while's start, SSA's preheader) end otherwise.
  if (!l_start.ends_with("_start")) {
    return std::nullopt;
  }
  const std::string base = l_start.substr(0, l_start.size() - 6);
  const std::string l_continue = base + "_continue";
  const std::string l_break = base + "_break";

  const std::size_t n = instrs.size();
  // Find the JumpIfFalse(_, L_break) that ends the entry test.
  std::optional<std::size_t> cond_jif_idx;
  std::string cond_var_name;
  for (std::size_t j = i + 1; j < n; ++j) {
    const auto& instr = instrs[j];
    if (const auto* jf = asdl::as<ir::JumpIfFalse>(instr); jf != nullptr && jf->target == l_break) {
      const auto* cond = asdl::as<ir::Var>(jf->condition);
      if (cond == nullptr) {
        return std::nullopt;
      }
      cond_var_name = cond->name;
      cond_jif_idx = j;
      break;
    }
    if (breaks_cond_block(instr)) {
      return std::nullopt;
    }
  }
  if (!cond_jif_idx) {
    return std::nullopt;
  }

  const Instrs cond_block(instrs.begin() + static_cast<std::ptrdiff_t>(i + 1),
                          instrs.begin() + static_cast<std::ptrdiff_t>(*cond_jif_idx));
  // The rotated loop skips the test the first time, so the test may
  // only compute temporaries.
  for (const auto& ins : cond_block) {
    if (!computes_temporary(ins)) {
      return std::nullopt;
    }
  }
  const ir::Binary* ge = find_ge_producer(cond_block, cond_var_name);
  if (ge == nullptr || !val_resolves_to_zero(ge->src2, cond_block)) {
    return std::nullopt;
  }
  const auto x_name = trace_to_var(ge->src1, cond_block);
  if (!x_name) {
    return std::nullopt;
  }

  if (symbols == nullptr) {
    // Python reads `symbols[x_name]` of None here.
    throw InternalError("TypeError: 'NoneType' object is not subscriptable");
  }
  const Symbol* sym = symbols->find(*x_name);
  if (sym == nullptr || !max_positive(sym->type)) {
    return std::nullopt;
  }

  // Find Label(L_continue) after the body. The body is whatever sits
  // between the JumpIfFalse and the continue label. The scan doesn't
  // look inside it, so labels, nested loops, and calls are all fine.
  std::optional<std::size_t> cont_idx;
  for (std::size_t j = *cond_jif_idx + 1; j < n; ++j) {
    const auto* cur = asdl::as<ir::Label>(instrs[j]);
    if (cur != nullptr && cur->name == l_continue) {
      cont_idx = j;
      break;
    }
  }
  if (!cont_idx) {
    return std::nullopt;
  }

  // Find Jump(L_start) after the post block. The post block must be
  // straight-line code: the c99_to_tac lowering of a for loop emits the
  // post clause's expression for its side effects only, and `x--` is
  // straight-line code.
  std::optional<std::size_t> jump_back_idx;
  for (std::size_t j = *cont_idx + 1; j < n; ++j) {
    const auto& instr = instrs[j];
    if (const auto* jmp = asdl::as<ir::Jump>(instr); jmp != nullptr && jmp->target == l_start) {
      jump_back_idx = j;
      break;
    }
    if (breaks_post_block(instr)) {
      return std::nullopt;
    }
  }
  if (!jump_back_idx || *jump_back_idx + 1 >= n) {
    return std::nullopt;
  }
  const auto* after_jump = asdl::as<ir::Label>(instrs[*jump_back_idx + 1]);
  if (after_jump == nullptr || after_jump->name != l_break) {
    return std::nullopt;
  }
  const std::size_t break_idx = *jump_back_idx + 1;

  if (!post_decrements_x_by_one(instrs, *cont_idx + 1, *jump_back_idx, *x_name)) {
    return std::nullopt;
  }
  if (!init_value_is_nonnegative(instrs, i, *x_name, sym->type)) {
    return std::nullopt;
  }

  bool body_iv_immutable = true;
  for (std::size_t k = *cond_jif_idx + 1; k < *cont_idx; ++k) {
    const auto d = defs_of(instrs[k]);
    if (d && *d == *x_name) {
      body_iv_immutable = false;
      break;
    }
  }

  return Match{
      .start_idx = i,
      .cond_jif_idx = *cond_jif_idx,
      .cont_idx = *cont_idx,
      .jump_back_idx = *jump_back_idx,
      .break_idx = break_idx,
      .cond_var_name = cond_var_name,
      .start_label = l_start,
      .x_name = *x_name,
      .body_iv_immutable = body_iv_immutable,
  };
}

// Appends the matched range `instrs[m.start_idx .. m.break_idx]` to
// `out` in its rotated order. The init instructions before
// `m.start_idx` are passed through by the caller.
void emit_rotated(const Instrs& instrs, const Match& m, Instrs& out) {
  // Label(<L>_start)
  out.push_back(instrs[m.start_idx]);
  // The body, with each `SignExtend(Var(x_name), dst)` made a
  // ZeroExtend of the same operands when the body doesn't write x. That
  // is sound only while x_name can't be negative anywhere in the body,
  // which holds for the rotated count-down's counter because its init
  // is `>= 0`, its post is `x -= 1`, and the test leaves the loop on a
  // negative value. A SignExtend of any other operand stays.
  for (std::size_t k = m.cond_jif_idx + 1; k < m.cont_idx; ++k) {
    const auto& ins = instrs[k];
    if (m.body_iv_immutable) {
      if (const auto* se = asdl::as<ir::SignExtend>(ins)) {
        const auto* src = asdl::as<ir::Var>(se->src);
        if (src != nullptr && src->name == m.x_name) {
          out.push_back(ir::ZeroExtend{.src = se->src, .dst = se->dst});
          continue;
        }
      }
    }
    out.push_back(ins);
  }
  // Label(<L>_continue)
  out.push_back(instrs[m.cont_idx]);
  // The post block, between Label(<L>_continue) and Jump(<L>_start).
  for (std::size_t k = m.cont_idx + 1; k < m.jump_back_idx; ++k) {
    out.push_back(instrs[k]);
  }
  // The test's instructions, between Label(<L>_start) and the
  // JumpIfFalse, moved here. They are the same instructions with the
  // same temporaries: the producer of `cond_var` and any casts before
  // it read x_var, which holds the decremented value at this point.
  for (std::size_t k = m.start_idx + 1; k < m.cond_jif_idx; ++k) {
    out.push_back(instrs[k]);
  }
  // JumpIfTrue(cond_var, <L>_start), in place of both the JumpIfFalse
  // to the break label and the Jump back to the start.
  out.push_back(ir::JumpIfTrue{.condition = ir::Var{.name = m.cond_var_name},
                               .target = m.start_label});
  // Label(<L>_break)
  out.push_back(instrs[m.break_idx]);
}

}  // namespace

ir::Function rotate_signed_countdown_loops(ir::Function fn, const framework::PassContext& ctx) {
  const Instrs& instrs = fn.instructions;
  Instrs out;
  out.reserve(instrs.size());
  const std::size_t n = instrs.size();
  std::size_t i = 0;
  while (i < n) {
    const auto m = try_match(instrs, i, ctx.symbols);
    if (!m) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    emit_rotated(instrs, *m, out);
    i = m->break_idx + 1;
  }
  return framework::with_instructions(fn, std::move(out));
}

}  // namespace c6502::passes
