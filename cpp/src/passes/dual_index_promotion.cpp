// Promotes a ZP-resident `Data` operand that several `LDX`s load into
// Reg(Y), loaded once at the function's entry.
//
// Motivating shape
//
// Once the TAC sinker, `adc_commute`, and the A tracking across blocks
// have run, `apply_bobble`'s body looks like this.
//
//     LDX __zpabi_apply_bobble_p1   ; X = bobble_idx
//     LDA rescue_bobble,X
//     BPL .if_else@1
// .add:
//     AND #$7F
//     LDX __zpabi_apply_bobble_p0    ; the slot, reloaded
//     CLC
//     ADC entity_floor_pos,X
//     STA entity_floor_pos,X
//     JMP .if_end@0
// .if_else@1:
//     ...
//     LDX __zpabi_apply_bobble_p0    ; the slot, reloaded
//     LDA entity_floor_pos,X
//     ...
//
// `__zpabi_apply_bobble_p0` (the slot) is loaded into X in both
// branches because `bobble_idx` already holds X at the function's
// entry. Pinning the slot to Y at the entry and switching the
// `entity_floor_pos,X` accesses to `,Y` collapses the two LDX reloads
// into one LDY at the entry. That saves one `LDX abs` (3 bytes) of code
// and no cycles here, since only one branch runs per call, but the
// saving grows in functions with more uses per branch.
//
// Eligibility
//
// A Data symbol `D` qualifies for Y-promotion in a function if and only
// if all of these hold.
//
//   1. `D` is the source of two or more `Mov(Data(D, 0), Reg(X))`
//      instructions in the function. A single LDX has nothing to fold,
//      and promoting it would only change the register it loads.
//   2. Reg(Y) is unused elsewhere in the function. Any of the following
//      disqualifies the function:
//        - any operand that is `Reg(Y)`, such as a `Mov` to or from Y,
//          an `INY` / `DEY`, a `CPY`, or a pull into Y,
//        - any `IndexedData(_, _, Y())` operand,
//        - any `IndirectY` / `IndirectZpY` operand, which takes Y as
//          its offset,
//        - any `Indirect` / `IndirectZp` / `Frame` / `Stack` operand,
//          which `asm_emit` lowers with an `LDY #off` in front,
//        - a call or tail-call jump that passes an argument in Y.
//   3. At least one other source loads X, so that some value other
//      than D already holds X. When nothing competes for X, keeping D
//      in X costs the same.
//   4. Every read of X that an `LDX D` reaches reads D on every path to
//      it (`plan_x_reads`). A forward dataflow over the control-flow
//      graph follows whether X may hold D, so a read after a label or a
//      branch is handled the same as one in the LDX's own block. An
//      `abs,X` read where X holds D on some paths and another value on
//      others disqualifies, and so does a read of X as a value (`TXA`,
//      `STX`, `CPX`, `INX`, a return with `save_x`) where X may hold D.
//      Each `abs,X` read of D must also be in an instruction with an
//      `abs,Y` form. The 6502 has no `INC abs,Y` / `DEC abs,Y` /
//      `ASL abs,Y` / `LSR abs,Y` / `ROL abs,Y` / `ROR abs,Y`.
//   5. Nothing in the function writes `D`, and the function makes no
//      call. Y is loaded from `D` once at the entry, so `D` has to hold
//      its entry value at every LDX the rewrite drops, and Y has to
//      keep its value until the last use. A callee may change Y, and
//      may also write `D` if `D` is a global or a byte of the shared
//      caller-saved zero-page range. A loop temporary that is stored
//      and then loaded into X again fails this gate.
//   6. N and Z are dead after every dropped `LDX D` and at the point
//      where the `LDY D` goes, since both loads set them.
//
// Rewrite
//
//   1. Insert `Mov(Data(D, 0), Reg(Y))` at the start of the function
//      body, after any leading `Label` and `FunctionPrologue` /
//      `AllocateStack`, and before the first instruction of the body.
//   2. Drop every `Mov(Data(D, 0), Reg(X))`.
//   3. In each instruction where gate 4 found that X holds D, rewrite
//      each `IndexedData(name, offset, X())` operand to
//      `IndexedData(name, offset, Y())`. An `abs,X` read where X holds
//      another value stays as it is.
//
// Soundness
//
// Y is unused by the function before the rewrite (gate 2), so loading D
// into Y at the entry disturbs nothing, since no instruction looked at
// Y before. After the rewrite, every IndexedData,Y in the function body
// reads the byte at `name + offset + (D's stored value)`, which equals
// what `name + offset + (X's stored value)` was at that point in the
// original, because only reads where X holds D on every path are
// rewritten. The reads left as `abs,X` see the same X as before,
// because every path to them that passes a dropped `LDX D` also passes
// a later write of X.
//
// For the soft-stack ABI, Y is not part of the calling convention
// between functions. Callers don't rely on any value of Y on return, so
// pinning Y inside the function body can't be seen from the call site.
//
// Cycle and byte effect
//
// Each dropped `LDX abs` saves 3 cycles and 3 bytes (the zp form saves
// 2 of each). The `LDY abs` added at the entry costs 3 cycles and 3
// bytes per call. The trade costs no cycles when only one LDX would run
// per call (apply_bobble's branches, which save 3 bytes overall because
// two LDXs in the code became one LDY), and saves cycles when several
// LDXs run on the same path (a loop using the same index across many
// accesses).
//
// Where it runs
//
// After `_peephole_fixedpoint` has converged and before
// `expand_long_branches`. The pass only shrinks code (one fewer LDX per
// duplicate, and the rewritten operands are the same size), so no
// long-branch displacement check is invalidated.
#include "passes/dual_index_promotion.h"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrList = std::vector<ir::Type_instruction>;

bool is_reg(const ir::Type_operand& op, bool (*test)(const ir::Type_reg&)) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && test(r->reg);
}

bool is_x(const ir::Type_reg& r) { return isa<ir::X>(r); }
bool is_y(const ir::Type_reg& r) { return isa<ir::Y>(r); }

bool is_reg_x(const ir::Type_operand& op) { return is_reg(op, is_x); }
bool is_reg_y(const ir::Type_operand& op) { return is_reg(op, is_y); }

bool has_reg_arg(const std::vector<std::string>& reg_args, const char* reg) {
  return std::find(reg_args.begin(), reg_args.end(), reg) != reg_args.end();
}

// Returns whether `instr` reads or writes Reg(Y). That includes an
// `abs,Y` operand, the `(ind),Y` operands, and the `LDY #off` that
// `asm_emit` puts in front of an `Indirect`, `IndirectZp`, `Frame`, or
// `Stack` access. A call or tail-call jump that passes an argument in Y
// counts too.
bool references_y(const ir::Type_instruction& instr) {
  if (const auto* da = as<ir::DecimalArith>(instr)) {
    return decimal_arith::writes_y(*da);
  }
  if (const auto* call = as<ir::Call>(instr); call != nullptr && has_reg_arg(call->reg_args, "Y")) {
    return true;
  }
  if (const auto* jump = as<ir::Jump>(instr); jump != nullptr && has_reg_arg(jump->reg_args, "Y")) {
    return true;
  }
  bool found = false;
  for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
    if (is_reg_y(op)) {
      found = true;
    } else if (const auto* ix = as<ir::IndexedData>(op); ix != nullptr && isa<ir::Y>(ix->index)) {
      found = true;
    } else if (isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY, ir::Frame,
                   ir::Stack>(op)) {
      // The indirect-Y addressing modes take Y as their offset, and the
      // ones with an offset load it first.
      found = true;
    }
  });
  return found;
}

// Returns `name` if `instr` is `Mov(Data(name, 0), Reg(X))`, else
// nothing. The offset of 0 makes the pinned value a single-byte symbol,
// since a value of several bytes can't ride in Y as a whole.
const std::string* ldx_data_source_name(const ir::Type_instruction& instr) {
  const auto* mov = as<ir::Mov>(instr);
  if (mov == nullptr || !is_reg_x(mov->dst)) {
    return nullptr;
  }
  const auto* src = as<ir::Data>(mov->src);
  if (src == nullptr || src->offset != 0) {
    return nullptr;
  }
  return &src->name;
}

bool is_ldx_of(const ir::Type_instruction& instr, const std::string& name) {
  const std::string* n = ldx_data_source_name(instr);
  return n != nullptr && *n == name;
}

// Returns, for each Data symbol that is the source of
// `Mov(Data(name, 0), Reg(X))` two or more times, the name and its
// count, in the order the names first appear.
std::vector<std::pair<std::string, int>> ldx_data_candidates(const InstrList& instrs) {
  std::vector<std::pair<std::string, int>> counts;
  for (const auto& instr : instrs) {
    const std::string* name = ldx_data_source_name(instr);
    if (name == nullptr) {
      continue;
    }
    auto it = std::find_if(counts.begin(), counts.end(),
                           [&](const auto& nc) { return nc.first == *name; });
    if (it == counts.end()) {
      counts.emplace_back(*name, 1);
    } else {
      ++it->second;
    }
  }
  std::vector<std::pair<std::string, int>> out;
  for (auto& nc : counts) {
    if (nc.second >= 2) {
      out.push_back(std::move(nc));
    }
  }
  return out;
}

// Returns whether some `Mov(_, Reg(X))` in the function loads X from a
// source other than `Data(promoted_name, 0)`. Without one, X only ever
// holds `promoted_name`'s value, and Y-promotion changes nothing.
bool x_has_other_uses(const InstrList& instrs, const std::string& promoted_name) {
  for (const auto& instr : instrs) {
    const auto* mov = as<ir::Mov>(instr);
    if (mov == nullptr || !is_reg_x(mov->dst)) {
      continue;
    }
    if (const auto* src = as<ir::Data>(mov->src);
        src != nullptr && src->name == promoted_name && src->offset == 0) {
      continue;
    }
    return true;
  }
  return false;
}

// Returns whether `instr` modifies Reg(X): LDX (`Mov(_, X)`), INX
// (`Inc(X)`), DEX (`Dec(X)`), and PLX (`Pop(X)`, which the NMOS 6502
// doesn't have, covered to be safe).
bool writes_x(const ir::Type_instruction& instr) {
  if (const auto* mov = as<ir::Mov>(instr)) {
    return is_reg_x(mov->dst);
  }
  if (const auto* inc = as<ir::Inc>(instr)) {
    return is_reg_x(inc->dst);
  }
  if (const auto* dec = as<ir::Dec>(instr)) {
    return is_reg_x(dec->dst);
  }
  if (const auto* pop = as<ir::Pop>(instr)) {
    return is_reg_x(pop->dst);
  }
  return false;
}

// Returns whether `instr` reads Reg(X) other than as the index of an
// `abs,X` operand. A pure write to X (`LDX`, `TAX`, `PLX`) isn't a
// read.
bool reads_x_value(const ir::Type_instruction& instr) {
  if (const auto* call = as<ir::Call>(instr)) {
    return has_reg_arg(call->reg_args, "X");
  }
  if (const auto* jump = as<ir::Jump>(instr)) {
    return has_reg_arg(jump->reg_args, "X");
  }
  if (const auto* ret = as<ir::Ret>(instr)) {
    return ret->save_x;
  }
  if (const auto* ret = as<ir::Return>(instr)) {
    return ret->save_x;
  }
  // The `dst` of a Mov, a Pop, or a LoadAddress is a write.
  if (const auto* mov = as<ir::Mov>(instr)) {
    return is_reg_x(mov->src);
  }
  if (isa<ir::Pop>(instr)) {
    return false;
  }
  if (const auto* la = as<ir::LoadAddress>(instr)) {
    return is_reg_x(la->src);
  }
  bool found = false;
  for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
    found = found || is_reg_x(op);
  });
  return found;
}

bool is_indexed_data_x(const ir::Type_operand& op) {
  const auto* ix = as<ir::IndexedData>(op);
  return ix != nullptr && isa<ir::X>(ix->index);
}

// Returns `op` with an `IndexedData(_, _, X())` operand's index swapped
// to `Y()`, and any other operand unchanged.
void fix_op(ir::Type_operand& op) {
  if (auto* ix = as<ir::IndexedData>(op); ix != nullptr && isa<ir::X>(ix->index)) {
    ix->index = ir::Y{};
  }
}

// Returns whether `rewrite_x_to_y` handles `instr`'s kind: the
// instruction kinds whose `IndexedData,X` operands all have an `abs,Y`
// form.
bool rewritable_kind(const ir::Type_instruction& instr) {
  return isa<ir::Mov, ir::Add, ir::Sub, ir::And, ir::Or, ir::Xor, ir::Compare>(instr);
}

// Rewrites every `IndexedData(_, _, X())` operand of `instr` to
// `IndexedData(_, _, Y())`. Instruction kinds that `rewritable_kind`
// rejects are left unchanged. `Inc` / `Dec` / the shifts with an
// IndexedData,X destination are rejected before this is reached.
void rewrite_x_to_y(ir::Type_instruction& instr) {
  if (!rewritable_kind(instr)) {
    return;
  }
  std::visit(
      [](auto& ins) {
        // Each field that holds one operand, in field order. A list of
        // operands (a DecimalArith's) is never rewritten.
        auto fix = [](auto& field) {
          if constexpr (std::is_same_v<std::decay_t<decltype(field)>, ir::Type_operand>) {
            fix_op(field);
          }
        };
        if constexpr (requires { ins.src; }) {
          fix(ins.src);
        }
        if constexpr (requires { ins.src1; }) {
          fix(ins.src1);
        }
        if constexpr (requires { ins.src2; }) {
          fix(ins.src2);
        }
        if constexpr (requires { ins.left; }) {
          fix(ins.left);
        }
        if constexpr (requires { ins.right; }) {
          fix(ins.right);
        }
        if constexpr (requires { ins.dst; }) {
          fix(ins.dst);
        }
      },
      instr);
}

// Returns whether `instr` is one of the instruction kinds whose `dst`
// has no `IndexedData,Y` form on the NMOS 6502, which has `INC abs,X`,
// `DEC abs,X`, and the shifts on abs,X, but not their abs,Y forms.
bool dst_x_only_kind(const ir::Type_instruction& instr) {
  return isa<ir::Inc, ir::Dec, ir::ArithmeticShiftLeft, ir::LogicalShiftRight,
             ir::RotateLeft, ir::RotateRight>(instr);
}

// Returns the indices control can reach right after `instrs[k]`. A jump
// or branch to a name that isn't a label of the function (a tail call)
// has no successor there.
std::vector<std::size_t> successors(
    const InstrList& instrs, std::size_t k,
    const std::unordered_map<std::string, std::size_t>& label_idx) {
  const auto& instr = instrs[k];
  if (isa<ir::Ret, ir::Return>(instr)) {
    return {};
  }
  if (const auto* jump = as<ir::Jump>(instr)) {
    const auto it = label_idx.find(jump->target);
    if (it == label_idx.end()) {
      return {};
    }
    return {it->second};
  }
  std::vector<std::size_t> out;
  if (k + 1 < instrs.size()) {
    out.push_back(k + 1);
  }
  if (const auto* br = as<ir::Branch>(instr)) {
    if (const auto it = label_idx.find(br->target); it != label_idx.end()) {
      out.push_back(it->second);
    }
  }
  return out;
}

// What X holds, as `plan_x_reads` follows it: the value of D, loaded by
// an LDX the rewrite drops, or anything else. A state is a set of
// these, as bits.
constexpr unsigned kXIsD = 1;
constexpr unsigned kXIsOther = 2;

// Returns the indices of the instructions whose `abs,X` operands read
// D's value, which the rewrite turns into `abs,Y`, or nothing if the
// rewrite can't be done.
//
// A forward dataflow over the control-flow graph follows, at each
// instruction, whether X may hold D (from an `LDX D`) or something
// else. X holds something else at the function's entry. An `abs,X`
// read where X holds D on every path is rewritten. One where X holds D
// on some paths and something else on others can't be, since the
// rewritten code has to pick one register, so this returns nothing. So
// does a read of X as a value (`TXA`, `STX`, `CPX`, `INX`, a return
// with `save_x`, ...) where X may hold D, and an `abs,X` read of D in a
// position with no `abs,Y` form (`INC`, `DEC`, the shifts).
std::optional<std::vector<bool>> plan_x_reads(const InstrList& instrs,
                                              const std::string& promoted_name) {
  std::unordered_map<std::string, std::size_t> label_idx;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto* l = as<ir::Label>(instrs[k])) {
      label_idx[l->name] = k;
    }
  }
  const std::size_t n = instrs.size();
  std::vector<bool> rewrite(n, false);
  if (n == 0) {
    return rewrite;
  }
  // 0 is no state yet (unreachable so far).
  std::vector<unsigned> state_in(n, 0);
  state_in[0] = kXIsOther;
  std::vector<std::size_t> work = {0};
  while (!work.empty()) {
    const std::size_t k = work.back();
    work.pop_back();
    const auto& instr = instrs[k];
    unsigned out;
    if (is_ldx_of(instr, promoted_name)) {
      out = kXIsD;
    } else if (writes_x(instr)) {
      out = kXIsOther;
    } else {
      out = state_in[k];
    }
    for (const std::size_t succ : successors(instrs, k, label_idx)) {
      const unsigned old = state_in[succ];
      const unsigned now = old | out;
      if (now != old) {
        state_in[succ] = now;
        work.push_back(succ);
      }
    }
  }
  for (std::size_t k = 0; k < n; ++k) {
    const auto& instr = instrs[k];
    const unsigned state = state_in[k];
    if ((state & kXIsD) == 0) {
      continue;  // Unreachable, or X can't hold D here.
    }
    if (reads_x_value(instr)) {
      return std::nullopt;
    }
    bool indexed = false;
    for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
      indexed = indexed || is_indexed_data_x(op);
    });
    if (!indexed) {
      continue;
    }
    if (state != kXIsD) {
      return std::nullopt;
    }
    if (dst_x_only_kind(instr)) {
      return std::nullopt;
    }
    if (!rewritable_kind(instr)) {
      return std::nullopt;  // An instruction kind the rewrite doesn't know.
    }
    rewrite[k] = true;
  }
  return rewrite;
}

// Returns the first index past the leading entry instructions (`Label`
// / `FunctionPrologue` / `AllocateStack`), where the LDY goes.
std::size_t entry_insertion_point(const InstrList& instrs) {
  std::size_t i = 0;
  while (i < instrs.size() &&
         isa<ir::Label, ir::FunctionPrologue, ir::AllocateStack>(instrs[i])) {
    ++i;
  }
  return i;
}

// Returns whether `instr` writes a byte of the object `name`, directly
// or through an index.
bool writes_name(const ir::Type_instruction& instr, const std::string& name) {
  if (const auto* da = as<ir::DecimalArith>(instr)) {
    for (const auto& w : decimal_arith::writes(*da)) {
      if (const auto* d = as<ir::Data>(w); d != nullptr && d->name == name) {
        return true;
      }
    }
    return false;
  }
  if (isa<ir::Push, ir::Compare, ir::BitTest>(instr)) {
    return false;
  }
  bool found = false;
  std::visit(
      [&](const auto& ins) {
        if constexpr (requires { ins.dst; }) {
          using D = std::decay_t<decltype(ins.dst)>;
          if constexpr (std::is_same_v<D, ir::Type_operand>) {
            if (const auto* d = as<ir::Data>(ins.dst)) {
              found = d->name == name;
            } else if (const auto* ix = as<ir::IndexedData>(ins.dst)) {
              found = ix->name == name;
            }
          }
        }
      },
      instr);
  return found;
}

// Inserts `LDY <promoted_name>` at the entry insertion point, drops
// every `LDX <promoted_name>`, and rewrites IndexedData,X to ,Y in the
// instructions `plan_x_reads` marked.
void do_promote(ir::Function& fn, const std::string& promoted_name,
                const std::vector<bool>& rewrite) {
  InstrList& instrs = fn.instructions;
  const std::size_t insert_at = entry_insertion_point(instrs);
  InstrList out;
  out.reserve(instrs.size() + 1);
  for (std::size_t k = 0; k < insert_at; ++k) {
    out.push_back(std::move(instrs[k]));
  }
  out.push_back(ir::Mov{.src = ir::Data{.name = promoted_name, .offset = 0},
                        .dst = ir::Reg{.reg = ir::Y{}},
                        .is_volatile = false});
  for (std::size_t k = insert_at; k < instrs.size(); ++k) {
    if (is_ldx_of(instrs[k], promoted_name)) {
      continue;
    }
    if (rewrite[k]) {
      rewrite_x_to_y(instrs[k]);
    }
    out.push_back(std::move(instrs[k]));
  }
  instrs = std::move(out);
}

void rewrite_function(ir::Function& fn) {
  const InstrList& instrs = fn.instructions;
  // Gate 2: Y must be unused.
  for (const auto& instr : instrs) {
    if (references_y(instr)) {
      return;
    }
  }
  // Gate 1: the candidates with two or more LDX uses.
  auto candidates = ldx_data_candidates(instrs);
  if (candidates.empty()) {
    return;
  }
  // The candidate picked is the one with the highest count, then the
  // first name in order, so the choice is deterministic.
  std::stable_sort(candidates.begin(), candidates.end(), [](const auto& a, const auto& b) {
    if (a.second != b.second) {
      return a.second > b.second;
    }
    return a.first < b.first;
  });
  const std::string promoted_name = candidates[0].first;
  // Gate 3: X must have some other live source. Without one, no LDX
  // reloads happen, the LDX(D) chain can stay in X with no conflict,
  // and Y-promotion only moves the work.
  if (!x_has_other_uses(instrs, promoted_name)) {
    return;
  }
  // Gate 5: D keeps its entry value and Y keeps its value up to every
  // use.
  for (const auto& instr : instrs) {
    if (isa<ir::Call>(instr) || writes_name(instr, promoted_name)) {
      return;
    }
  }
  // Gate 4: every X read that an LDX of D reaches reads D on every path,
  // and has an abs,Y form.
  const auto rewrite = plan_x_reads(instrs, promoted_name);
  if (!rewrite) {
    return;
  }
  // The dropped LDXs and the added LDY set N and Z.
  const LabelMap labels(instrs);
  if (!flags_dead_at(instrs, labels, entry_insertion_point(instrs))) {
    return;
  }
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (is_ldx_of(instrs[k], promoted_name) && !flags_dead_at(instrs, labels, k + 1)) {
      return;
    }
  }
  do_promote(fn, promoted_name, *rewrite);
}

}  // namespace

asm_ast::Program apply_dual_index_promotion(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
