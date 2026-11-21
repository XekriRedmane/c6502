/* Folds the 0-or-1 tail of a short-circuit `&&` or `||`, and the
JumpIf that tests it right after, into direct conditional jumps.

# The idiom

C99 `if (a && b) ...` lowers through `c99_to_tac.translate_short_circuit`
(and survives SSA destruction) as

    JumpIfFalse(a, .and_false@N)
    JumpIfFalse(b, .and_false@N)
    Copy(1, %t)
    Jump(.and_end@N)
    Label(.and_false@N)
    Copy(0, %t)
    Label(.and_end@N)
    JumpIfFalse(%t, .if_end@M)

The five instructions of the tail (the two Copies, the Jump between
them, and the two Labels) put a 0 or a 1 in `%t`, and the consumer
right after them tests that value again. No C code reads the value,
since every short-circuit jump of the chain already decides where
control goes, as the consumer's branch would. The asm lowering would
otherwise branch twice for one decision:

    BCC .and_false@N        ; chain jump 1
    BCS .and_false@N        ; chain jump 2
    LDA #$01                ; Copy(1, %t)
    STA __local_t
    JMP .and_end@N
    .and_false@N:
    LDA #$00                ; Copy(0, %t)
    STA __local_t
    .and_end@N:
    LDA __local_t           ; consumer LDA
    BEQ .if_end@M           ; consumer branch

`||` with `JumpIfFalse`, `&&` with `JumpIfTrue`, and `||` with
`JumpIfTrue` have the same shape with other constants and another
consumer, and the pass folds all four into one round of branches to the
consumer's destinations.

# The rewrite

When the instructions at `i` are the tail

    Copy(C_ft, %t)
    Jump(end_label)
    Label(branch_label)
    Copy(C_sc, %t)
    Label(end_label)

with `(C_ft, C_sc)` either `(ConstInt 1, ConstInt 0)` or `(ConstInt 0,
ConstInt 1)`, followed by a consumer `JumpIf{True,False}(%t, T)`, the
pass finds where control goes when the chain fired (`%t == C_sc`
reaches the consumer) and where it goes when no chain jump fired (`%t
== C_ft`). Each is `T` or the instruction after the consumer.

  - In the natural case the chain goes to `T` and the fall-through to
    the next instruction. Every jump to `branch_label` goes to `T`, and
    the tail and the consumer go.
  - In the flipped case the chain goes to the next instruction and the
    fall-through to `T`. A new label `.<fn>@scfold@<N>` takes the
    chain, and the tail and the consumer become `Jump(T);
    Label(.<fn>@scfold@<N>)`, where the Jump sends the fall-through to
    T.

The chain's jumps test the original operands at their own widths, and
the lowerings of `JumpIf{True,False}` and `JumpIfCmp` read every byte
and branch once, so the rewrite depends on nothing about `%t`'s width.

# Conditions

  - The two Copies write the same `%t`.
  - The tail's `Jump` goes to the second `Label`.
  - `(C_ft, C_sc)` are `ConstInt 0` and `ConstInt 1` in either order,
    the shape of the short-circuit lowering.
  - `%t` is read once in the function, by the consumer. Another reader
    would see the value the rewrite drops.
  - Only the tail's Jump goes to `end_label`. Any other jump to it would
    lose its target.
  - The consumer is `JumpIfTrue(%t, T)` or `JumpIfFalse(%t, T)` on `%t`
    itself, after `end_label` and any `Copy`s that neither read nor
    write `%t` (see "Copies before the consumer").

# Copies before the consumer

SSA destruction puts a Phi's copies at the end of its predecessor
block, which is where the consumer is when the consumer's branch skips
an assignment. `if (c >= 0x60 && c < 0x80) c -= 0x20;` leaves

    Label(.and_end@3)
    Copy(@0.c, @0.c.2)          ; the phi's copy for the skipped arm
    JumpIfFalse(%4.3, .if_end@4)

The short-circuit arm may have copies of its own too, between
`branch_label` and `Copy(C_sc, %t)`, as SSA destruction leaves for a
Phi of a name the chain changes, such as the `--left` in Zork's `je`
(examples/zork/decode.c), `!equal && --left != 0`:

    Label(.and_false@23)
    Copy(@77.left.3, @77.left.4)  ; only on the chain's path
    Copy(0, %417.3)
    Label(.and_end@24)

Those run only on the chain's path, so they go first in its stub, or
right after the flipped case's new label.

The copies before the consumer run on every path through it, so the
rewrite keeps them on both: in place for the path that falls through,
and for the chain's path in a stub `Label(L); <copies>; Jump(D_sc)`
after the function's last instruction, which has to be a `Ret` or a
`Jump` so that nothing falls into the stub. In the flipped case the
copies go before the new `Jump(T)` and again after the new label. Once
the asm optimizer coalesces a Phi's names such a copy is nothing, the
stub is a bare `JMP`, and branch threading sends the chain straight to
`D_sc`.

Any number of jumps may go to `branch_label`, since those are the chain
being folded. Each of them runs `Copy(C_sc, %t)`, falls through, and
goes where the consumer sends `C_sc`, which is where the rewrite sends
it directly, without the dead write of `%t`.

# Nested shapes

`(a && b) || c` and the like give two foldable tails whose temporaries
don't overlap, and one sweep folds every tail that doesn't overlap
another. The inner fold retargets its `branch_label` to the outer's
`branch_label`, which the outer fold retargets too, so the map of
targets is followed to a label it doesn't map (the transitive
closure). A tail that becomes foldable only after another is folded
waits for the next sweep of the driver's fixed point.

The pass runs after SSA destruction, where copy folding has collapsed
each arm's copies into the tail above. Before destruction the tail is
two SSA definitions of `%t` joined by a Phi.

# Merging equality jumps

After each sweep, `merge_equality_jumps` makes two adjacent jumps to one
target, on `x == a` and `x == b` with `a` and `b` one bit `p` apart, one
jump on `(x & ~p) == min(a, b)`. See its comment below. */
#include "passes/optimization/short_circuit_jump_fold.h"

#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "bit_precise.h"
#include "passes/optimization/value_range.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using asdl::Int;
using framework::Instrs;
using IK = ir::Kind_instruction;
using CK = ir::Kind_const;

std::string* jump_target(ir::Type_instruction& instr) {
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

const std::string* jump_target(const ir::Type_instruction& instr) {
  return jump_target(const_cast<ir::Type_instruction&>(instr));
}

// Returns the value of `val` when it is a `Constant` holding a
// `ConstInt`, or none. Only `ConstInt` counts, since the short-circuit
// lowering makes its 0 and 1 with `_tac_const_val(c99_ast.Int(), 0|1)`.
std::optional<Int> const_int_value(const ir::Type_val& val) {
  const auto* c = std::get_if<ir::Constant>(&val);
  if (c == nullptr) {
    return std::nullopt;
  }
  const auto* i = std::get_if<ir::ConstInt>(&c->const_);
  if (i == nullptr) {
    return std::nullopt;
  }
  return i->value;
}

bool is_var_named(const ir::Type_val& v, const std::string& name) {
  const auto* var = std::get_if<ir::Var>(&v);
  return var != nullptr && var->name == name;
}

// A tail that `try_fold` matched: whether it is the natural case, the
// chain's label, the consumer's target, the copies before the consumer,
// and the copies of the short-circuit arm.
struct Fold {
  bool natural = false;
  std::string branch_label;
  std::string target;
  Instrs copies;
  Instrs sc_copies;
};

// Returns the fold of the tail at `instrs[i]` when the instructions
// from there are `Copy / Jump / Label / Copy / Label / JumpIf{True,False}`
// in the short-circuit shape, with `Copy`s that don't touch `%t` allowed
// before the consumer and before the short-circuit arm's `Copy`, or
// none.
std::optional<Fold> try_fold(const Instrs& instrs, std::size_t i,
                             const OrderedMap<std::string, std::size_t>& use_count,
                             const std::map<std::string, std::size_t, std::less<>>& jump_targets) {
  const std::size_t n = instrs.size();
  if (i + 5 >= n) {
    return std::nullopt;
  }
  // The short-circuit arm's own copies, then its `Copy(C_sc, %t)`.
  std::size_t k = i + 3;
  while (k < n && std::holds_alternative<ir::Copy>(instrs[k])) {
    ++k;
  }
  if (k == i + 3 || k >= n) {
    return std::nullopt;
  }
  std::size_t j = k + 1;
  while (j < n && std::holds_alternative<ir::Copy>(instrs[j])) {
    ++j;
  }
  if (j >= n) {
    return std::nullopt;
  }
  const auto* cp_ft = std::get_if<ir::Copy>(&instrs[i]);
  const auto* jump_in_tail = std::get_if<ir::Jump>(&instrs[i + 1]);
  const auto* label_branch = std::get_if<ir::Label>(&instrs[i + 2]);
  const auto* cp_sc = std::get_if<ir::Copy>(&instrs[k - 1]);
  const auto* label_end = std::get_if<ir::Label>(&instrs[k]);
  const ir::Type_instruction& consumer = instrs[j];
  const IK consumer_kind = ir::kind(consumer);
  if (cp_ft == nullptr || jump_in_tail == nullptr || label_branch == nullptr ||
      cp_sc == nullptr || label_end == nullptr ||
      (consumer_kind != IK::JumpIfTrue && consumer_kind != IK::JumpIfFalse)) {
    return std::nullopt;
  }
  const auto* ft_dst = std::get_if<ir::Var>(&cp_ft->dst);
  const auto* sc_dst = std::get_if<ir::Var>(&cp_sc->dst);
  if (ft_dst == nullptr || sc_dst == nullptr || ft_dst->name != sc_dst->name) {
    return std::nullopt;
  }
  const std::string& t_name = ft_dst->name;
  const auto c_ft = const_int_value(cp_ft->src);
  const auto c_sc = const_int_value(cp_sc->src);
  if (!c_ft || !c_sc) {
    return std::nullopt;
  }
  if (!((*c_ft == 0 && *c_sc == 1) || (*c_ft == 1 && *c_sc == 0))) {
    return std::nullopt;
  }
  if (jump_in_tail->target != label_end->name) {
    return std::nullopt;
  }
  const ir::Type_val& condition = consumer_kind == IK::JumpIfTrue
                                      ? std::get<ir::JumpIfTrue>(consumer).condition
                                      : std::get<ir::JumpIfFalse>(consumer).condition;
  if (!is_var_named(condition, t_name)) {
    return std::nullopt;
  }
  Fold fold;
  fold.copies.assign(instrs.begin() + static_cast<std::ptrdiff_t>(k + 1),
                     instrs.begin() + static_cast<std::ptrdiff_t>(j));
  fold.sc_copies.assign(instrs.begin() + static_cast<std::ptrdiff_t>(i + 3),
                        instrs.begin() + static_cast<std::ptrdiff_t>(k - 1));
  for (const Instrs* group : {&fold.copies, &fold.sc_copies}) {
    for (const auto& cp : *group) {
      const auto& c = std::get<ir::Copy>(cp);
      if (is_var_named(c.src, t_name) || is_var_named(c.dst, t_name)) {
        return std::nullopt;
      }
    }
  }
  const std::size_t* uses = use_count.find(t_name);
  if (uses == nullptr || *uses != 1) {
    return std::nullopt;
  }
  const auto jt = jump_targets.find(label_end->name);
  if (jt == jump_targets.end() || jt->second != 1) {
    return std::nullopt;
  }
  const Int consumer_branch_value = consumer_kind == IK::JumpIfTrue ? 1 : 0;
  fold.natural = *c_sc == consumer_branch_value;
  fold.branch_label = label_branch->name;
  fold.target = *jump_target(consumer);
  return fold;
}

// Counts how many jumps of every kind go to each label.
std::map<std::string, std::size_t, std::less<>> count_jump_targets(const Instrs& instrs) {
  std::map<std::string, std::size_t, std::less<>> out;
  for (const auto& instr : instrs) {
    if (const std::string* t = jump_target(instr)) {
      ++out[*t];
    }
  }
  return out;
}

// --- merge_equality_jumps ---

// Returns the bit width of an integer constant class, or none for the
// floating classes and the odd widths of the bit-precise types.
std::optional<int> const_bits(CK k) {
  switch (k) {
    case CK::ConstChar:
    case CK::ConstUChar:
      return 8;
    case CK::ConstInt:
    case CK::ConstUInt:
      return 16;
    case CK::ConstLong:
    case CK::ConstULong:
      return 32;
    case CK::ConstLongLong:
    case CK::ConstULongLong:
      return 64;
    default:
      return std::nullopt;
  }
}

bool signed_class(CK k) {
  return k == CK::ConstChar || k == CK::ConstInt || k == CK::ConstLong || k == CK::ConstLongLong;
}

// A jump to `target` taken when the Var `name` equals an integer
// constant, which is none for the 0 of `JumpIfFalse(x, T)`.
struct EqualJump {
  std::string name;
  std::optional<ir::Type_const> constant;
  std::string target;
};

// Returns the jump `instr` makes when the Var `name` equals an integer
// constant: `JumpIfFalse(x, T)`, whose constant is none for 0 of x's
// width, or `JumpIfCmp(Equal, x, k, T)` with the constant on either
// side.
std::optional<EqualJump> jump_if_equal(const ir::Type_instruction& instr) {
  if (const auto* jf = std::get_if<ir::JumpIfFalse>(&instr)) {
    if (const auto* v = std::get_if<ir::Var>(&jf->condition)) {
      return EqualJump{.name = v->name, .constant = std::nullopt, .target = jf->target};
    }
    return std::nullopt;
  }
  const auto* jc = std::get_if<ir::JumpIfCmp>(&instr);
  if (jc == nullptr || !std::holds_alternative<ir::Equal>(jc->op)) {
    return std::nullopt;
  }
  const std::pair<const ir::Type_val*, const ir::Type_val*> orders[] = {{&jc->src1, &jc->src2},
                                                                         {&jc->src2, &jc->src1}};
  for (const auto& [v, k] : orders) {
    const auto* var = std::get_if<ir::Var>(v);
    const auto* c = std::get_if<ir::Constant>(k);
    if (var != nullptr && c != nullptr && const_bits(ir::kind(c->const_))) {
      return EqualJump{.name = var->name, .constant = c->const_, .target = jc->target};
    }
  }
  return std::nullopt;
}

// The merged jump: its Var, the mask, the constant it compares with,
// and the target.
struct Merged {
  std::string name;
  ir::Type_const mask;
  ir::Type_const low;
  std::string target;
};

// Returns the merged test of two equality jumps on one Var whose
// constants differ in one bit, or none. See `merge_equality_jumps`.
std::optional<Merged> merged_test(const EqualJump& first, const EqualJump& second,
                                  const SymbolTable& symbols) {
  const Symbol* sym = symbols.find(first.name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  const c99::Type_data_type* t = &sym->type;
  while (const auto* c = std::get_if<c99::Const>(t)) {
    t = &*c->referenced_type;
  }
  if (std::holds_alternative<c99::Volatile>(*t)) {
    return std::nullopt;
  }
  const auto width = value_range::int_width(*t);
  if (!width) {
    return std::nullopt;
  }
  const ir::Type_const* k1 = first.constant ? &*first.constant : nullptr;
  const ir::Type_const* k2 = second.constant ? &*second.constant : nullptr;
  const ir::Type_const* proto = k1 != nullptr ? k1 : k2;
  if (proto == nullptr) {
    return std::nullopt;
  }
  const CK cls = ir::kind(*proto);
  if (*const_bits(cls) != 8 * *width) {
    return std::nullopt;
  }
  for (const ir::Type_const* k : {k1, k2}) {
    if (k != nullptr && ir::kind(*k) != cls) {
      return std::nullopt;
    }
  }
  const int bits = 8 * *width;
  const Int full = (Int(1) << bits) - 1;
  const Int a = k1 == nullptr ? Int(0) : (*bit_precise::raw_value(*k1) & full);
  const Int b = k2 == nullptr ? Int(0) : (*bit_precise::raw_value(*k2) & full);
  const Int p = a ^ b;
  if (p == 0 || (p & (p - 1)) != 0) {
    return std::nullopt;
  }
  const Int low = a < b ? a : b;
  const auto as_const = [&](Int v) {
    if (signed_class(cls) && v > (full >> 1)) {
      v -= full + 1;
    }
    return bit_precise::with_value(*proto, v);
  };
  return Merged{.name = first.name,
                .mask = as_const(full & ~p),
                .low = as_const(low),
                .target = first.target};
}

}  // namespace

ir::Function fold_short_circuit_jump(ir::Function fn) {
  const Instrs& instrs = fn.instructions;
  const auto use_count = count_uses(instrs);
  const auto jump_targets = count_jump_targets(instrs);
  std::set<std::string, std::less<>> existing_labels;
  for (const auto& ins : instrs) {
    if (const auto* l = std::get_if<ir::Label>(&ins)) {
      existing_labels.insert(l->name);
    }
  }
  int label_counter = 0;
  const auto fresh_label = [&]() {
    while (true) {
      std::string name = "." + fn.name + "@scfold@" + std::to_string(label_counter);
      ++label_counter;
      if (!existing_labels.contains(name)) {
        existing_labels.insert(name);
        return name;
      }
    }
  };
  // A stub can go after the last instruction only if nothing falls off
  // the end into it.
  const bool ends_in_terminator =
      !instrs.empty() &&
      (std::holds_alternative<ir::Ret>(instrs.back()) ||
       std::holds_alternative<ir::Jump>(instrs.back()));
  // The map of retargets in the order the folds make them, as Python's
  // dict holds it.
  OrderedMap<std::string, std::string> retarget_map;
  Instrs new_instrs;
  Instrs stubs;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::optional<Fold> fold = try_fold(instrs, i, use_count, jump_targets);
    if (fold && (!fold->copies.empty() || !fold->sc_copies.empty()) && fold->natural &&
        !ends_in_terminator) {
      fold.reset();
    }
    if (!fold) {
      new_instrs.push_back(instrs[i]);
      ++i;
      continue;
    }
    if (fold->natural) {
      if (!fold->copies.empty() || !fold->sc_copies.empty()) {
        std::string stub = fresh_label();
        stubs.push_back(ir::Label{.name = stub});
        stubs.insert(stubs.end(), fold->sc_copies.begin(), fold->sc_copies.end());
        stubs.insert(stubs.end(), fold->copies.begin(), fold->copies.end());
        stubs.push_back(ir::Jump{.target = fold->target});
        retarget_map.insert_or_assign(fold->branch_label, stub);
        new_instrs.insert(new_instrs.end(), fold->copies.begin(), fold->copies.end());
      } else {
        retarget_map.insert_or_assign(fold->branch_label, fold->target);
      }
    } else {
      std::string new_label = fresh_label();
      retarget_map.insert_or_assign(fold->branch_label, new_label);
      new_instrs.insert(new_instrs.end(), fold->copies.begin(), fold->copies.end());
      new_instrs.push_back(ir::Jump{.target = fold->target});
      new_instrs.push_back(ir::Label{.name = new_label});
      new_instrs.insert(new_instrs.end(), fold->sc_copies.begin(), fold->sc_copies.end());
      new_instrs.insert(new_instrs.end(), fold->copies.begin(), fold->copies.end());
    }
    // The tail (5), both arms' copies, and the consumer (1).
    i += 6 + fold->copies.size() + fold->sc_copies.size();
  }
  new_instrs.insert(new_instrs.end(), stubs.begin(), stubs.end());
  if (retarget_map.empty()) {
    return fn;
  }
  // The transitive closure. Nested short-circuits chain through a shared
  // label: the inner fold retargets its branch_label to the outer's
  // branch_label, which the outer fold also retargets onward. One
  // substitution would leave the inner chain at a label that is about to
  // go, so each target is followed until it lands on a label the map
  // doesn't hold.
  std::map<std::string, std::string, std::less<>> resolved;
  for (const auto& [key, first] : retarget_map) {
    std::string target = first;
    std::set<std::string, std::less<>> seen{key};
    while (true) {
      const std::string* next = retarget_map.find(target);
      if (next == nullptr || seen.contains(target)) {
        break;
      }
      seen.insert(target);
      target = *next;
    }
    resolved.insert_or_assign(key, target);
  }
  for (auto& instr : new_instrs) {
    if (std::string* t = jump_target(instr)) {
      if (const auto it = resolved.find(*t); it != resolved.end()) {
        *t = it->second;
      }
    }
  }
  return framework::with_instructions(fn, std::move(new_instrs));
}

/* `e == 0 || e == 1`, once the short-circuit fold has sent both tests
to the `if`'s target, is `JumpIfFalse(e, T); JumpIfCmp(==, e, 1, T)`,
which becomes `JumpIfFalse(e & ~1, T)`: on a `uint16_t`, `AND #$FE; ORA
hi; BEQ` where the two tests read each byte and compared the low one
with 1.

Clearing bit `p` maps `a` and `b` to `a` (the one without the bit) and
every other value to something else, so the one jump is taken for the
same values of x. The second jump directly follows the first, with no
label before it, so it is reached only when the first isn't taken, and
nothing writes x between them. `x` must be a non-volatile integer Var,
since the merged test reads it once, and the `&` goes into a new
temporary of x's type. */
ir::Function merge_equality_jumps(ir::Function fn, SymbolTable* symbols) {
  if (symbols == nullptr) {
    return fn;
  }
  const Instrs& instrs = fn.instructions;
  Instrs out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const auto first = jump_if_equal(instrs[i]);
    const auto second =
        i + 1 < instrs.size() ? jump_if_equal(instrs[i + 1]) : std::optional<EqualJump>{};
    std::optional<Merged> merged;
    if (first && second && first->name == second->name && first->target == second->target) {
      merged = merged_test(*first, *second, *symbols);
    }
    if (!merged) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    const Symbol* sym = symbols->find(merged->name);
    int k = 0;
    while (symbols->contains(merged->name + ".eqm" + std::to_string(k))) {
      ++k;
    }
    const ir::Var tmp{.name = merged->name + ".eqm" + std::to_string(k)};
    c99::Type_data_type type = sym->type;
    symbols->insert_or_assign(tmp.name, Symbol{.type = std::move(type), .attrs = LocalAttr{}});
    out.push_back(ir::Binary{.op = ir::BitwiseAnd{},
                             .src1 = ir::Var{.name = merged->name},
                             .src2 = ir::Constant{.const_ = merged->mask},
                             .dst = tmp});
    if (*bit_precise::raw_value(merged->low) == 0) {
      out.push_back(ir::JumpIfFalse{.condition = tmp, .target = merged->target});
    } else {
      out.push_back(ir::JumpIfCmp{.op = ir::Equal{},
                                  .src1 = tmp,
                                  .src2 = ir::Constant{.const_ = merged->low},
                                  .target = merged->target});
    }
    changed = true;
    i += 2;
  }
  if (!changed) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(out));
}

ir::Function fold_short_circuit_jumps(ir::Function fn, const framework::PassContext& ctx) {
  return merge_equality_jumps(fold_short_circuit_jump(std::move(fn)), ctx.symbols);
}

}  // namespace c6502::passes
