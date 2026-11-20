// Promotes a uchar loop counter to the X register, with full
// cross-call save/restore and a Y-pivot of transient indexed accesses.
//
// Motivating case
//
// The outer loop of `refresh_hit_entities` was, before this pass:
//
//     LDA p0
//     STA b4
// .loop_start:
//     LDX b4                      ; reload counter at top
//     LDA entity_hit_y,X
//     ...
//     LDX p2                      ; sprite_xref, which clobbers X
//     LDA hit_spr_*,X
//     ...
//     LDX b4                      ; mid-iter reload after clobber
//     LDA entity_hit_row,X
//     ...
//     STA p5
//     JSR draw_sprite_opaque       ; callee clobbers X
//     DEC b4
// .loop_continue:
//     BPL .loop_start
//
// The hand-written equivalent (Drol's REFRESH_HIT_ENTITIES at $631D) is:
//
//     LDX ZP_HIT_MAX              ; init X
// HIT_DRAW_BODY:
//     LDA ENTITY_HIT_Y,X
//     ...
//     LDY ZP_SPRITE_XREF          ; sprite_xref in Y
//     LDA HIT_SPR_*,Y
//     ...
//     LDA ENTITY_HIT_ROW,X        ; X still has counter
//     ...
//     STX ZP_DRAW_LOOP_IDX        ; save before JSR
//     JSR DRAW_SPRITE_OPAQUE
//     LDX ZP_DRAW_LOOP_IDX        ; restore after JSR
//     JMP HIT_DRAW_TAIL
// HIT_DRAW_TAIL:
//     DEX
//     BPL HIT_DRAW_BODY
//
// Three transformations combine to bridge the gap.
//
//   1. The Y-pivot turns any `LDX <other>; LDA arr,X; ...` block into
//      `LDY <other>; LDA arr,Y; ...`, so X isn't clobbered by the
//      transient indexed access.
//   2. The cross-call save and restore inserts `STX M` before each
//      `Call` in the body and `LDX M` after it, which preserves the
//      counter across the callee's clobber.
//   3. The counter promotion drops the loop-top `LDX M` (X carries from
//      the prior iteration's DEX or from the init), drops the mid-iter
//      `LDX M` reloads (X is preserved by the Y-pivot and the save and
//      restore), appends `TAX` to the init, and replaces `DEC M` with
//      `DEX`.
//
// Eligibility
//
// The slot M is eligible if and only if all of these hold.
//
//   * Exactly one Mov has M as its dst (the init).
//   * Exactly one `Dec(M)` (the decrement).
//   * One or more `Mov(M, Reg(X))` (LDX M reloads).
//   * No other uses of M anywhere.
//   * Every other X-write in the function is one of these.
//     - An `LDX <other>` where the counter is dead, that is, where no
//       path reads M before the init writes it again. It stays an LDX.
//     - An `LDX <other>` that starts a pivot range (see
//       `try_collect_pivot_range`). The range touches no Y, its only X
//       reads are `abs,X` operands with an `abs,Y` form, and it ends
//       before any use of M. Neither X's loaded value nor Y's earlier
//       value is read after the range.
//     - A `Call` instruction (save and restore handled).
//   * Other X-writers (INX, DEX, TAX) disqualify.
//   * M is one of the compiler's own slots: a body-local (`__local_*`),
//     a parameter slot (`__zpabi_*`), or a `ZP` byte the regalloc
//     chose. A user global or static keeps its value after the function
//     returns and has one when it starts, but the promoted counter
//     lives in X and `DEX` / `INX` leave M stale. Before this rule,
//     `--z` in one loop and `z++` after it, on a zero-page
//     `unsigned long z`, became a counter in X that nothing loaded
//     (fuzz.stmts seed 1549).
//   * The init comes before every other access to M on every path from
//     the function's entry, so no read sees M's value at entry.
//
// Soundness sketch
//
// After the transformation, X holds M's current value at every program
// point that originally read M with `LDX M` (or any other X consumer).
//
//   * Init: `LDA <src>; STA M; TAX` makes X = M = initial. An init
//     `STX M` gets no TAX, and an init `STY M` gets `LDX M`.
//   * Loop-top: X carries from the previous iteration's DEX (back-edge)
//     or from the init's TAX. The dropped LDX M is unobservable.
//   * Mid-body: Y-pivoted ranges leave X untouched, and the canonical
//     home M is read implicitly through the same X. A mid-iter LDX M
//     was only there to restore after an X-clobber, and with no
//     clobber it goes.
//   * Around a Call: STX M before the Call writes the canonical home,
//     so the LDX M after it restores X correctly.
//   * Loop tail: DEX decrements X (= M). No STX is needed, because
//     either the next iteration starts at the loop top with X carrying,
//     or an STX before the next Call syncs M.
//
// Count-up counters and preserving calls
//
// The step can also be an `Inc(M)`, for a loop that counts up and tests
// the counter at the top, the way Zork's buffer dump does:
//
//     LDA  #0             ; init               LDX  #0
//     STA  M                                  .top:
// .top:                                         CPX  BUFF_END
//     LDA  M              ; test        =>      BEQ  .done
//     CMP  BUFF_END                             LDA  BUFF_AREA,X
//     BEQ  .done                                JSR  COUT1
//     LDX  M              ; reload              INX
//     LDA  BUFF_AREA,X                          JMP  .top
//     JSR  COUT1
//     INC  M              ; step
//     JMP  .top
//
// The `Inc(M)` becomes `INX`. An `LDA M` directly followed by `CMP op`,
// where `op` is something CPX can address (an immediate or a direct
// memory operand) and A isn't read afterwards, becomes `CPX op`, which
// sets the same flags. Every other `LDA M` becomes `TXA` as before.
//
// A call to a routine declared `preserves_x` doesn't write X
// (`asm_liveness.call_preserves`), so it isn't wrapped. Any other call
// is wrapped only when the counter is read after it before the init
// runs again (`counter_live_after`). A call before the loop, whose X is
// overwritten by the init, needs no `STX M` / `LDX M`.
//
// Where to run
//
// After `replace_pseudoregisters` (operands are concrete) and after the
// asm-peephole fixed-point loop (so `dec_inc_branch_fold` has already
// simplified `DEC M; LDA M; B<cond>` to `DEC M; B<cond>`). Before
// `expand_long_branches`.
#include "passes/loop_counter_to_x.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_x_op(const ir::Type_operand& op) { return is_reg_x(op); }
bool is_reg_y_op(const ir::Type_operand& op) { return is_reg_y(op); }

const ir::Type_operand kRegA = ir::Reg{.reg = ir::A{}};
const ir::Type_operand kRegX = ir::Reg{.reg = ir::X{}};
const ir::Type_operand kRegY = ir::Reg{.reg = ir::Y{}};

// The memory byte an operand names, `("data", name, offset)` or
// `("zp", address, offset)` in Python's `_operand_key`.
struct Key {
  bool is_zp = false;
  std::string name;
  asdl::Int address = 0;
  asdl::Int offset = 0;

  bool operator==(const Key&) const = default;
  bool operator<(const Key& o) const {
    if (is_zp != o.is_zp) {
      return is_zp < o.is_zp;
    }
    if (name != o.name) {
      return name < o.name;
    }
    if (address != o.address) {
      return address < o.address;
    }
    return offset < o.offset;
  }
};

std::optional<Key> operand_key(const ir::Type_operand& op) {
  if (const auto* d = as<ir::Data>(op)) {
    return Key{.is_zp = false, .name = d->name, .address = 0, .offset = d->offset};
  }
  if (const auto* z = as<ir::ZP>(op)) {
    return Key{.is_zp = true, .name = {}, .address = z->address, .offset = z->offset};
  }
  return std::nullopt;
}

bool key_is(const ir::Type_operand& op, const Key& k) {
  std::optional<Key> got = operand_key(op);
  return got && *got == k;
}

ir::Type_operand mem_op_from_key(const Key& k) {
  if (k.is_zp) {
    return ir::ZP{.address = k.address, .offset = k.offset};
  }
  return ir::Data{.name = k.name, .offset = k.offset};
}

bool stable_mem_eq(const ir::Type_operand& a, const ir::Type_operand& b) {
  if (isa<ir::Data>(a) && isa<ir::Data>(b)) {
    return a == b;
  }
  if (isa<ir::ZP>(a) && isa<ir::ZP>(b)) {
    return a == b;
  }
  return false;
}

// The role a memory operand plays for the candidate scan.
enum class Role { kInit, kLdx, kLda, kDec, kInc, kOther };

using Roles = std::vector<std::pair<Role, ir::Type_operand>>;

// Classifies each memory operand's role for the candidate scan.
Roles operand_roles(const ir::Type_instruction& instr) {
  Roles out;
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      if (stable_mem_eq(m.src, m.dst)) {
        return out;  // a move of a byte to itself
      }
      const bool src_is_reg = isa<ir::Reg>(m.src);
      if (is_reg_x(m.dst) && !src_is_reg) {
        out.emplace_back(Role::kLdx, m.src);
        return out;
      }
      if (is_reg_a(m.dst) && !src_is_reg) {
        // `Mov(M, Reg(A))`, LDA M. It doesn't disqualify the counter
        // slot, and the apply step rewrites it to TXA (since X = M is
        // the promotion invariant).
        out.emplace_back(Role::kLda, m.src);
        return out;
      }
      if (!isa<ir::Reg>(m.dst)) {
        out.emplace_back(Role::kInit, m.dst);
        return out;
      }
      if (!src_is_reg) {
        out.emplace_back(Role::kOther, m.src);
      }
      return out;
    }
    case ir::Kind_instruction::Dec: {
      const auto& d = std::get<ir::Dec>(instr);
      if (!isa<ir::Reg>(d.dst)) {
        out.emplace_back(Role::kDec, d.dst);
      }
      return out;
    }
    case ir::Kind_instruction::Inc: {
      const auto& d = std::get<ir::Inc>(instr);
      if (!isa<ir::Reg>(d.dst)) {
        out.emplace_back(Role::kInc, d.dst);
      }
      return out;
    }
    case ir::Kind_instruction::Add:
    case ir::Kind_instruction::Sub:
    case ir::Kind_instruction::And:
    case ir::Kind_instruction::Or:
      for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
        if (!isa<ir::Reg>(op)) {
          out.emplace_back(Role::kOther, op);
        }
      });
      return out;
    case ir::Kind_instruction::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      if (!isa<ir::Reg>(c.left)) {
        out.emplace_back(Role::kOther, c.left);
      }
      if (!isa<ir::Reg>(c.right)) {
        out.emplace_back(Role::kOther, c.right);
      }
      return out;
    }
    default:
      break;
  }
  // Any other instruction that names a memory operand (a shift or
  // rotate of it, `EOR`, `BIT`, `PHA`, ...) is a use the promotion
  // doesn't rewrite, so it disqualifies the slot. Missing one would
  // leave X stale: `STA M; ROL M; INC M; LDX M` once promoted M to X
  // with the `ROL` still updating memory. A `DecimalArith` keeps its
  // operands in lists, which are visited. A `Phi`'s arguments are
  // `AsmPhiArg`s, which Python's walk over the fields doesn't take for
  // operands.
  auto visit = [&](const ir::Type_operand& op) {
    if (isa<ir::Data, ir::ZP>(op)) {
      out.emplace_back(Role::kOther, op);
    }
  };
  if (isa<ir::Phi>(instr)) {
    for_each_operand<false>(instr, visit);
  } else {
    for_each_operand<true>(instr, visit);
  }
  return out;
}

bool uses_key(const ir::Type_instruction& instr, const Key& k, bool skip_init) {
  for (const auto& [role, op] : operand_roles(instr)) {
    if (skip_init && role == Role::kInit) {
      continue;
    }
    if (key_is(op, k)) {
      return true;
    }
  }
  return false;
}

// Python's `op_uses_x` inside `_reads_x`.
bool op_uses_x(const ir::Type_operand& op, bool is_dst = false, bool rmw = false) {
  if (is_reg_x(op)) {
    return !is_dst || rmw;
  }
  // Indexed addressing through X reads X whatever the operand's role.
  const auto* ix = as<ir::IndexedData>(op);
  return ix != nullptr && isa<ir::X>(ix->index);
}

// Returns whether `instr` reads Reg(X) in any operand role: a source, a
// compare operand, an RMW dst, or an `abs,X` index. A pure write to X,
// such as `LDX`, `TAX`, or `Pop(X)`, doesn't count.
bool reads_x_any(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov: {
      const auto& i = std::get<ir::Mov>(instr);
      return op_uses_x(i.src) || op_uses_x(i.dst, true);
    }
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      return op_uses_x(i.left) || op_uses_x(i.right);
    }
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      return op_uses_x(i.src) || op_uses_x(i.dst, true, true);
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      return op_uses_x(i.src) || op_uses_x(i.dst, true, true);
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      return op_uses_x(i.src) || op_uses_x(i.dst, true, true);
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      return op_uses_x(i.src) || op_uses_x(i.dst, true, true);
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      return op_uses_x(i.src1) || op_uses_x(i.src2) || op_uses_x(i.dst, true, true);
    }
    case ir::Kind_instruction::Inc:
      return op_uses_x(std::get<ir::Inc>(instr).dst, true, true);
    case ir::Kind_instruction::Dec:
      return op_uses_x(std::get<ir::Dec>(instr).dst, true, true);
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return op_uses_x(std::get<ir::ArithmeticShiftLeft>(instr).dst, true, true);
    case ir::Kind_instruction::LogicalShiftRight:
      return op_uses_x(std::get<ir::LogicalShiftRight>(instr).dst, true, true);
    case ir::Kind_instruction::RotateLeft:
      return op_uses_x(std::get<ir::RotateLeft>(instr).dst, true, true);
    case ir::Kind_instruction::RotateRight:
      return op_uses_x(std::get<ir::RotateRight>(instr).dst, true, true);
    case ir::Kind_instruction::Push:
      return op_uses_x(std::get<ir::Push>(instr).src);
    case ir::Kind_instruction::BitTest:
      return op_uses_x(std::get<ir::BitTest>(instr).src);
    case ir::Kind_instruction::LoadAddress: {
      const auto& i = std::get<ir::LoadAddress>(instr);
      return op_uses_x(i.src) || op_uses_x(i.dst, true);
    }
    default:
      return false;
  }
}

// Returns the address of the `dst` field of a Mov, Pop, or LoadAddress,
// whose Reg(Y) is a pure write, or null.
const ir::Type_operand* pure_write_dst(const ir::Type_instruction& instr) {
  if (const auto* m = as<ir::Mov>(instr)) {
    return &m->dst;
  }
  if (const auto* p = as<ir::Pop>(instr)) {
    return &p->dst;
  }
  if (const auto* l = as<ir::LoadAddress>(instr)) {
    return &l->dst;
  }
  return nullptr;
}

// Returns whether `instr` reads Reg(Y) as a value, as an `abs,Y` index,
// or as the offset of an `(ind),Y` operand that has no `LDY #off` setup
// of its own (`IndirectY`, `IndirectZpY`).
bool reads_y_any(const ir::Type_instruction& instr) {
  const ir::Type_operand* write_dst = pure_write_dst(instr);
  bool found = false;
  for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
    if (found) {
      return;
    }
    if (is_reg_y(op)) {
      // A pure write to Y (`LDY`, `TAY`, `PLY`) isn't a read.
      if (&op != write_dst) {
        found = true;
      }
      return;
    }
    if (const auto* ix = as<ir::IndexedData>(op); ix != nullptr && isa<ir::Y>(ix->index)) {
      found = true;
      return;
    }
    if (isa<ir::IndirectY, ir::IndirectZpY>(op)) {
      found = true;
    }
  });
  return found;
}

// Returns whether `instr` reads or writes Reg(Y) in any way, including
// the `LDY #off` that `asm_emit` puts in front of an `Indirect`,
// `IndirectZp`, `Frame`, or `Stack` access.
bool touches_y(const ir::Type_instruction& instr) {
  if (reads_y_any(instr) || sets_up_y(instr)) {
    return true;
  }
  bool found = false;
  for_each_operand<false>(instr, [&](const ir::Type_operand& op) {
    found = found || is_reg_y(op);
  });
  return found;
}

enum class IndexReg { kX, kY };

bool has_reg_arg(const std::vector<std::string>& reg_args, std::string_view reg) {
  return std::find(reg_args.begin(), reg_args.end(), reg) != reg_args.end();
}

// Returns whether `instr` reads `reg`, counting a call or tail-call jump
// that takes the register as an argument and a return whose epilogue
// carries X out (`save_x`).
bool reg_reads(const ir::Type_instruction& instr, IndexReg reg) {
  const std::string_view name = reg == IndexReg::kX ? "X" : "Y";
  if (const auto* c = as<ir::Call>(instr)) {
    return has_reg_arg(c->reg_args, name);
  }
  if (const auto* j = as<ir::Jump>(instr)) {
    return has_reg_arg(j->reg_args, name);
  }
  if (const auto* r = as<ir::Ret>(instr)) {
    return reg == IndexReg::kX && r->save_x;
  }
  if (const auto* r = as<ir::Return>(instr)) {
    return reg == IndexReg::kX && r->save_x;
  }
  return reg == IndexReg::kX ? reads_x_any(instr) : reads_y_any(instr);
}

// Returns whether `instr` overwrites `reg` without reading it. It is
// asked after `reg_reads`, so an instruction that does both counts as a
// read.
bool reg_kills(const ir::Type_instruction& instr, IndexReg reg) {
  if (isa<ir::Call>(instr)) {
    return !call_preserves(instr, reg == IndexReg::kX ? "X" : "Y");
  }
  const ir::Type_operand* dst = nullptr;
  if (const auto* m = as<ir::Mov>(instr)) {
    dst = &m->dst;
  } else if (const auto* p = as<ir::Pop>(instr)) {
    dst = &p->dst;
  }
  if (dst != nullptr && (reg == IndexReg::kX ? is_reg_x(*dst) : is_reg_y(*dst))) {
    return true;
  }
  return reg == IndexReg::kY && sets_up_y(instr);
}

const std::string* jump_or_branch_target(const ir::Type_instruction& instr) {
  if (const auto* j = as<ir::Jump>(instr)) {
    return &j->target;
  }
  if (const auto* b = as<ir::Branch>(instr)) {
    return &b->target;
  }
  return nullptr;
}

// Returns whether `reg` is read on some path from `instrs[start]` before
// it is overwritten, that is, whether the value the register holds at
// `start` is still used. The walk follows both arms of a branch and the
// targets of jumps. A return ends a path, reading X when it has
// `save_x`.
bool reg_live_from(Instrs instrs, const LabelMap& labels, std::size_t start,
                   IndexReg reg) {
  std::vector<bool> seen(instrs.size(), false);
  std::vector<std::size_t> stack{start};
  while (!stack.empty()) {
    std::size_t j = stack.back();
    stack.pop_back();
    while (j < instrs.size() && !seen[j]) {
      seen[j] = true;
      const auto& instr = instrs[j];
      if (reg_reads(instr, reg)) {
        return true;
      }
      if (reg_kills(instr, reg)) {
        break;
      }
      if (isa<ir::Ret, ir::Return>(instr)) {
        break;
      }
      if (const std::string* t = jump_or_branch_target(instr)) {
        const long k = labels.find(*t);
        if (k >= 0) {
          stack.push_back(static_cast<std::size_t>(k));
        }
        if (isa<ir::Jump>(instr)) {
          break;
        }
      }
      ++j;
    }
  }
  return false;
}

// Returns whether `instr` writes Reg(X) and the write isn't an LDX from
// the counter's canonical home M.
bool is_x_write_other_than(const ir::Type_instruction& instr, const Key& m_key) {
  if (const auto* m = as<ir::Mov>(instr)) {
    if (is_reg_x(m->dst)) {
      // LDX from M is the canonical reload.
      return !key_is(m->src, m_key);
    }
    return false;
  }
  if (const auto* i = as<ir::Inc>(instr)) {
    return is_reg_x(i->dst);
  }
  if (const auto* d = as<ir::Dec>(instr)) {
    return is_reg_x(d->dst);
  }
  if (isa<ir::Call>(instr)) {
    return !call_preserves(instr, "X");
  }
  if (const auto* p = as<ir::Pop>(instr)) {
    return is_reg_x(p->dst);
  }
  return false;
}

// Returns whether the counter M is read on some path after the
// instruction at `call_idx` before the init at `init_idx` writes it
// again. Only then does a call need `STX M` / `LDX M` around it, so
// that X still holds M afterwards. The walk follows both arms of a
// branch and jump targets.
bool counter_live_after(Instrs instrs, const LabelMap& labels, std::size_t call_idx,
                        const Key& m_key, std::size_t init_idx) {
  std::vector<bool> seen(instrs.size(), false);
  std::vector<std::size_t> stack{call_idx + 1};
  while (!stack.empty()) {
    std::size_t j = stack.back();
    stack.pop_back();
    while (j < instrs.size() && !seen[j]) {
      seen[j] = true;
      if (j == init_idx) {
        break;
      }
      const auto& ins = instrs[j];
      if (uses_key(ins, m_key, /*skip_init=*/true)) {
        return true;
      }
      if (isa<ir::Return, ir::Ret>(ins)) {
        break;
      }
      if (const auto* jump = as<ir::Jump>(ins)) {
        const long k = labels.find(jump->target);
        if (k >= 0) {
          stack.push_back(static_cast<std::size_t>(k));
        }
        break;
      }
      if (const auto* br = as<ir::Branch>(ins)) {
        const long k = labels.find(br->target);
        if (k >= 0) {
          stack.push_back(static_cast<std::size_t>(k));
        }
      }
      ++j;
    }
  }
  return false;
}

bool is_ldx_to_x(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && is_reg_x(m->dst) && !isa<ir::Reg>(m->src);
}

ir::Type_operand rewrite_op_for_pivot(const ir::Type_operand& op) {
  if (const auto* ix = as<ir::IndexedData>(op); ix != nullptr && isa<ir::X>(ix->index)) {
    return ir::IndexedData{.name = ix->name, .offset = ix->offset, .index = ir::Y{}};
  }
  return op;
}

// Rewrites a pivot-range instruction: the leading LDX becomes LDY, and
// any IndexedData(index=X) consumer is indexed by Y.
ir::Type_instruction pivot_rewrite(const ir::Type_instruction& instr) {
  if (const auto* m = as<ir::Mov>(instr)) {
    ir::Type_operand new_src = rewrite_op_for_pivot(m->src);
    ir::Type_operand new_dst = rewrite_op_for_pivot(m->dst);
    // The leading LDX (dst=Reg(X), a source that isn't a register)
    // becomes LDY.
    if (is_reg_x(new_dst) && !isa<ir::Reg>(new_src)) {
      new_dst = kRegY;
    }
    // The move keeps its `is_volatile` bit.
    return ir::Mov{.src = new_src, .dst = new_dst, .is_volatile = m->is_volatile};
  }
  if (const auto* i = as<ir::Add>(instr)) {
    return ir::Add{.src = rewrite_op_for_pivot(i->src), .dst = rewrite_op_for_pivot(i->dst)};
  }
  if (const auto* i = as<ir::Sub>(instr)) {
    return ir::Sub{.src = rewrite_op_for_pivot(i->src), .dst = rewrite_op_for_pivot(i->dst)};
  }
  if (const auto* i = as<ir::And>(instr)) {
    return ir::And{.src = rewrite_op_for_pivot(i->src), .dst = rewrite_op_for_pivot(i->dst)};
  }
  if (const auto* i = as<ir::Or>(instr)) {
    return ir::Or{.src = rewrite_op_for_pivot(i->src), .dst = rewrite_op_for_pivot(i->dst)};
  }
  if (const auto* c = as<ir::Compare>(instr); c != nullptr && is_reg_a(c->left)) {
    // `CMP abs,X` has the `CMP abs,Y` form.
    return ir::Compare{.left = c->left, .right = rewrite_op_for_pivot(c->right)};
  }
  return instr;
}

// Verifies a Y-pivot range starting at the `LDX` at `instrs[start]`.
// Returns the exclusive end index, or nothing if no valid range exists
// from here.
//
// The range is valid when all of these hold.
//   * The LDX's source is one LDY can load (Imm, Data, ZP,
//     ImmLabelLow, ImmLabelHigh), each of which has an `LDY` form that
//     reads neither X nor Y.
//   * No instruction in the range reads or writes Y, including the
//     `LDY #off` setup of a `Frame`, `Stack`, `Indirect`, or
//     `IndirectZp` operand, since Y holds the loaded value.
//   * Every X read in the range is an `abs,X` operand that
//     `pivot_rewrite` turns into `abs,Y`.
//   * The range ends before another X write, a label, a branch, a
//     jump, a call, a return, or any use of the counter M. The apply
//     step rewrites a use of M (its step to `DEX` or `INX`, an `LDA M`
//     to `TXA`), and it gives the pivot rewrite precedence, so a use of
//     M inside the range would keep reading or stepping the memory
//     copy.
std::optional<std::size_t> try_collect_pivot_range(Instrs instrs, std::size_t start,
                                                   const Key& m_key) {
  const auto& first = std::get<ir::Mov>(instrs[start]);
  if (!isa<ir::Imm, ir::Data, ir::ZP, ir::ImmLabelLow, ir::ImmLabelHigh>(first.src)) {
    return std::nullopt;
  }
  std::size_t j = start + 1;
  while (j < instrs.size()) {
    const auto& instr = instrs[j];
    // Control-flow and call boundaries end the range before this
    // instruction.
    if (isa<ir::Label, ir::Jump, ir::Branch, ir::Call, ir::Ret, ir::Return>(instr)) {
      return j;
    }
    if (uses_key(instr, m_key, /*skip_init=*/false)) {
      return j;
    }
    // Another X-write ends the range before this instruction.
    if (const auto* m = as<ir::Mov>(instr); m != nullptr && is_reg_x(m->dst)) {
      return j;
    }
    if (const auto* p = as<ir::Pop>(instr); p != nullptr && is_reg_x(p->dst)) {
      return j;
    }
    if (const auto* i = as<ir::Inc>(instr); i != nullptr && is_reg_x(i->dst)) {
      return j;
    }
    if (const auto* d = as<ir::Dec>(instr); d != nullptr && is_reg_x(d->dst)) {
      return j;
    }
    if (touches_y(instr)) {
      return std::nullopt;
    }
    if (reads_x_any(pivot_rewrite(instr))) {
      // An X read with no `abs,Y` form here: `TXA`, `STX`, `CPX`,
      // `INC abs,X`, a shift of `abs,X`, ...
      return std::nullopt;
    }
    ++j;
  }
  return j;
}

// Returns whether a byte is one the compiler owns, which nothing
// outside the function reads: a regalloc `ZP` byte, a body-local, or a
// parameter slot.
bool is_compiler_slot(const Key& k) {
  if (k.is_zp) {
    return true;
  }
  return k.name.starts_with("__local_") || k.name.starts_with("__zpabi_");
}

// Returns whether every path from the function's entry passes
// `instrs[init_idx]` before reaching any index in `uses`.
bool init_comes_first(Instrs instrs, const LabelMap& labels, std::size_t init_idx,
                      const std::vector<std::size_t>& uses) {
  const std::set<std::size_t> targets(uses.begin(), uses.end());
  std::vector<bool> seen(instrs.size(), false);
  std::vector<std::size_t> stack{0};
  while (!stack.empty()) {
    std::size_t j = stack.back();
    stack.pop_back();
    while (j < instrs.size() && !seen[j]) {
      seen[j] = true;
      if (j == init_idx) {
        break;
      }
      if (targets.count(j) != 0) {
        return false;
      }
      const auto& instr = instrs[j];
      if (isa<ir::Ret, ir::Return>(instr)) {
        break;
      }
      if (const std::string* t = jump_or_branch_target(instr)) {
        const long k = labels.find(*t);
        if (k >= 0) {
          stack.push_back(static_cast<std::size_t>(k));
        }
        if (isa<ir::Jump>(instr)) {
          break;
        }
      }
      ++j;
    }
  }
  return true;
}

// Verifies that the DEC is followed by a Branch on N or Z whose target
// chain lands at one of `ldx_indices`, and returns that index.
std::optional<std::size_t> find_loop_top_lda(Instrs instrs, const LabelMap& labels,
                                             std::size_t dec_idx,
                                             const std::vector<std::size_t>& ldx_indices) {
  std::set<std::string, std::less<>> branch_targets;
  for (const auto& inst : instrs) {
    if (const std::string* t = jump_or_branch_target(inst)) {
      branch_targets.insert(*t);
    }
  }
  std::size_t j = dec_idx + 1;
  while (j < instrs.size()) {
    const auto* label = as<ir::Label>(instrs[j]);
    if (label == nullptr) {
      break;
    }
    if (branch_targets.count(label->name) != 0) {
      return std::nullopt;
    }
    ++j;
  }
  if (j >= instrs.size()) {
    return std::nullopt;
  }
  const auto* br = as<ir::Branch>(instrs[j]);
  if (br == nullptr || !isa<ir::EQ, ir::NE, ir::MI, ir::PL>(br->cond)) {
    return std::nullopt;
  }
  std::string target = br->target;
  std::set<std::string> seen;
  while (true) {
    const long label_at = labels.find(target);
    if (label_at < 0 || seen.count(target) != 0) {
      return std::nullopt;
    }
    seen.insert(target);
    std::size_t k = static_cast<std::size_t>(label_at) + 1;
    while (k < instrs.size()) {
      const auto& inst = instrs[k];
      if (isa<ir::Label>(inst)) {
        ++k;
        continue;
      }
      if (const auto* m = as<ir::Mov>(inst); m != nullptr && stable_mem_eq(m->src, m->dst)) {
        ++k;
        continue;
      }
      break;
    }
    if (k >= instrs.size()) {
      return std::nullopt;
    }
    if (const auto* jump = as<ir::Jump>(instrs[k])) {
      target = jump->target;
      continue;
    }
    if (std::find(ldx_indices.begin(), ldx_indices.end(), k) != ldx_indices.end()) {
      return k;
    }
    return std::nullopt;
  }
}

struct Candidate {
  Key m_key;
  std::size_t init_idx = 0;
  // The index of the step, a `Dec(M)` or an `Inc(M)`.
  std::size_t dec_idx = 0;
  bool step_is_inc = false;
  std::optional<std::size_t> loop_top_lda_idx;
  std::vector<std::size_t> lda_m_indices;
};

// What the candidate scan found for one byte.
struct Uses {
  int init_count = 0;
  std::size_t init_idx = 0;
  int dec_count = 0;
  std::size_t dec_idx = 0;
  int inc_count = 0;
  std::size_t inc_idx = 0;
  std::vector<std::size_t> ldx;
  std::vector<std::size_t> lda;
  bool disqualified = false;
};

// Identifies a memory slot M with the loop-counter use pattern. A
// count-down counter needs its `DEC` to branch back to a loop-top
// `LDX M`. A count-up counter has no loop-top reload to find, since
// its loop tests the counter with `LDA M; CMP` instead.
//
// A byte of a `(zp),Y` pointer is never a candidate. A read through the
// pointer reads the byte too, and names it by the pointer's address,
// which the scan below doesn't count as a use, so the reads would see
// the byte in memory after the step had moved to X. The single `INC`
// that `carry_in_add` leaves on the high byte of `msg = msg + msg[0] +
// 1` was taken for a counter that way.
std::optional<Candidate> find_counter_candidate(Instrs instrs, const LabelMap& labels,
                                                const SymbolAddrs* zp_symbol_addrs) {
  const std::set<asdl::Int> pointer_addrs = pointer_byte_addresses(instrs);
  std::map<Key, Uses> uses;
  // The keys in the order Python's `init_count`, `dec_count`, and
  // `ldx_idx` dicts gain them.
  std::vector<Key> init_order;
  std::vector<Key> dec_order;
  std::vector<Key> ldx_order;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    for (const auto& [role, op] : operand_roles(instrs[i])) {
      std::optional<Key> k = operand_key(op);
      if (!k) {
        continue;
      }
      Uses& u = uses[*k];
      if (is_pointer_byte(op, pointer_addrs, zp_symbol_addrs)) {
        u.disqualified = true;
        continue;
      }
      switch (role) {
        case Role::kInit:
          if (u.init_count++ == 0) {
            init_order.push_back(*k);
          }
          u.init_idx = i;
          break;
        case Role::kLdx:
          if (u.ldx.empty()) {
            ldx_order.push_back(*k);
          }
          u.ldx.push_back(i);
          break;
        case Role::kLda:
          u.lda.push_back(i);
          break;
        case Role::kDec:
          if (u.dec_count++ == 0) {
            dec_order.push_back(*k);
          }
          u.dec_idx = i;
          break;
        case Role::kInc:
          ++u.inc_count;
          u.inc_idx = i;
          break;
        case Role::kOther:
          u.disqualified = true;
          break;
      }
    }
  }
  std::vector<Key> order = init_order;
  order.insert(order.end(), dec_order.begin(), dec_order.end());
  order.insert(order.end(), ldx_order.begin(), ldx_order.end());
  for (const Key& key : order) {
    const Uses& u = uses.at(key);
    if (u.disqualified || !is_compiler_slot(key)) {
      continue;
    }
    if (u.init_count != 1) {
      continue;
    }
    std::vector<std::size_t> all_uses = u.ldx;
    all_uses.insert(all_uses.end(), u.lda.begin(), u.lda.end());
    if (u.dec_count > 0) {
      all_uses.push_back(u.dec_idx);
    }
    if (u.inc_count > 0) {
      all_uses.push_back(u.inc_idx);
    }
    if (!init_comes_first(instrs, labels, u.init_idx, all_uses)) {
      continue;
    }
    if (u.ldx.empty()) {
      continue;
    }
    Candidate c{.m_key = key, .init_idx = u.init_idx};
    if (u.dec_count == 1 && u.inc_count == 0) {
      c.loop_top_lda_idx = find_loop_top_lda(instrs, labels, u.dec_idx, u.ldx);
      if (!c.loop_top_lda_idx) {
        continue;
      }
      c.dec_idx = u.dec_idx;
      c.step_is_inc = false;
    } else if (u.inc_count == 1 && u.dec_count == 0) {
      c.dec_idx = u.inc_idx;
      c.step_is_inc = true;
    } else {
      continue;
    }
    c.lda_m_indices = u.lda;
    return c;
  }
  return std::nullopt;
}

struct Plan {
  Candidate cand;
  std::vector<std::size_t> mid_lda_indices;
  std::vector<std::pair<std::size_t, std::size_t>> pivot_ranges;
  std::vector<std::size_t> call_indices;
};

// Identifies a candidate counter slot M and verifies that the
// transformation can be applied.
std::optional<Plan> plan_promotion(Instrs instrs, const SymbolAddrs* zp_symbol_addrs) {
  const LabelMap labels(instrs);
  // Step 1: find counter-slot candidates by use-pattern.
  std::optional<Candidate> cand = find_counter_candidate(instrs, labels, zp_symbol_addrs);
  if (!cand) {
    return std::nullopt;
  }
  const Key& m_key = cand->m_key;
  // The counter is in X from its init on, so X can't hold anything
  // else there, such as a parameter that arrives in X and is read
  // after the init.
  if (reg_live_from(instrs, labels, cand->init_idx + 1, IndexReg::kX)) {
    return std::nullopt;
  }

  // Step 2: scan all X-writes and classify each.
  Plan plan{.cand = *cand};
  std::size_t i = 0;
  while (i < instrs.size()) {
    const auto& instr = instrs[i];
    if (is_x_write_other_than(instr, m_key)) {
      // Some X-write other than the counter's. It must be one the
      // Y-pivot can move, or a Call.
      if (isa<ir::Call>(instr)) {
        // The promotion wraps each Call with `STX M` before and `LDX M`
        // after it to preserve the counter across the JSR. That model
        // takes the callee to deliver nothing in X. A function
        // returning a non-pointer value of 2 bytes returns its high
        // byte in X, and the caller captures it (`STX slot_hi` / `TXA`)
        // right after the JSR. Inserting `LDX M` there would clobber
        // the live return high byte before it's captured. If X is read
        // after this Call before being redefined, the callee returned a
        // live value in X, and the promotion is refused. In the normal
        // loop-counter case the counter is reloaded with `LDX M` (a
        // pure write) before any X use, often at the loop top reached
        // across the back-edge, so X is redefined first on every path.
        if (reg_live_from(instrs, labels, i + 1, IndexReg::kX)) {
          return std::nullopt;
        }
        if (counter_live_after(instrs, labels, i, m_key, cand->init_idx)) {
          plan.call_indices.push_back(i);
        }
        ++i;
        continue;
      }
      if (is_ldx_to_x(instr)) {
        // Where the counter is dead (no read of M before the init
        // writes it again, as for the `LDX hi` of a 2-byte return after
        // the loop), the load can stay an LDX. X isn't expected to hold
        // the counter there.
        if (!counter_live_after(instrs, labels, i, m_key, cand->init_idx)) {
          ++i;
          continue;
        }
        std::optional<std::size_t> end = try_collect_pivot_range(instrs, i, m_key);
        if (!end) {
          return std::nullopt;
        }
        // The pivot leaves the counter in X and the loaded value in Y
        // from the LDY onward. An X read after the range that the LDX
        // reaches would get the counter where it read the loaded value,
        // and a Y read after it would get the loaded value where it
        // read whatever Y held before.
        if (reg_live_from(instrs, labels, *end, IndexReg::kX)) {
          return std::nullopt;
        }
        if (reg_live_from(instrs, labels, *end, IndexReg::kY)) {
          return std::nullopt;
        }
        plan.pivot_ranges.emplace_back(i, *end);
        i = *end;
        continue;
      }
      // Some other X-write (INX, DEX, TAX, PLX) disqualifies the slot.
      return std::nullopt;
    }
    // An LDX M reload other than the loop-top one is a mid-iter one.
    if (const auto* m = as<ir::Mov>(instr);
        m != nullptr && is_reg_x(m->dst) && key_is(m->src, m_key) &&
        (!cand->loop_top_lda_idx || i != *cand->loop_top_lda_idx)) {
      plan.mid_lda_indices.push_back(i);
    }
    ++i;
  }
  return plan;
}

// Walks the function's instructions and applies every rewrite of the
// plan.
std::vector<ir::Type_instruction> apply_plan(Instrs instrs, const Plan& plan) {
  const Candidate& c = plan.cand;
  const ir::Type_operand m_op = mem_op_from_key(c.m_key);
  const std::set<std::size_t> mid_lda_set(plan.mid_lda_indices.begin(),
                                          plan.mid_lda_indices.end());
  const std::set<std::size_t> call_set(plan.call_indices.begin(), plan.call_indices.end());
  const std::set<std::size_t> lda_m_set(c.lda_m_indices.begin(), c.lda_m_indices.end());
  std::vector<bool> in_pivot(instrs.size(), false);
  for (const auto& [s, e] : plan.pivot_ranges) {
    for (std::size_t j = s; j < e; ++j) {
      in_pivot[j] = true;
    }
  }
  const LabelMap labels(instrs);

  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size() + 2 * plan.call_indices.size() + 1);
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (c.loop_top_lda_idx && i == *c.loop_top_lda_idx) {
      ++i;
      continue;
    }
    if (mid_lda_set.count(i) != 0) {
      ++i;
      continue;
    }
    if (call_set.count(i) != 0) {
      // Wrap: STX M; <Call>; LDX M.
      out.push_back(ir::Mov{.src = kRegX, .dst = m_op});
      out.push_back(instrs[i]);
      out.push_back(ir::Mov{.src = m_op, .dst = kRegX});
      ++i;
      continue;
    }
    if (in_pivot[i]) {
      // Y-pivot: rewrite the leading LDX to LDY and each
      // IndexedData(X) consumer in the range.
      out.push_back(pivot_rewrite(instrs[i]));
      ++i;
      continue;
    }
    if (lda_m_set.count(i) != 0) {
      const ir::Compare* nxt =
          i + 1 < instrs.size() ? as<ir::Compare>(instrs[i + 1]) : nullptr;
      if (nxt != nullptr && is_reg_a(nxt->left) &&
          isa<ir::Imm, ir::Data, ir::ZP>(nxt->right) && a_dead_at(instrs, labels, i + 2)) {
        // `LDA M; CMP op` becomes `CPX op`, which sets the same flags
        // and leaves A alone, and A isn't read afterwards.
        out.push_back(ir::Compare{.left = kRegX, .right = nxt->right});
        i += 2;
        continue;
      }
      // `LDA M` becomes `TXA`. X = M is the promotion invariant, so the
      // two give the same value.
      out.push_back(ir::Mov{.src = kRegX, .dst = kRegA});
      ++i;
      continue;
    }
    if (i == c.dec_idx) {
      // The step becomes DEX or INX.
      if (c.step_is_inc) {
        out.push_back(ir::Inc{.dst = kRegX});
      } else {
        out.push_back(ir::Dec{.dst = kRegX});
      }
      ++i;
      continue;
    }
    if (i == c.init_idx) {
      // X has to hold M's initial value after the init. A store from A,
      // and a memory or immediate source (which `asm_emit` copies
      // through A), leave the value in A, so a TAX does it. An `STX M`
      // leaves it in X already, and after an `STY M` the value is in M
      // and Y but not in A, so X is loaded from M.
      out.push_back(instrs[i]);
      const ir::Type_operand& src = std::get<ir::Mov>(instrs[i]).src;
      if (is_reg_y_op(src)) {
        out.push_back(ir::Mov{.src = m_op, .dst = kRegX});
      } else if (!is_reg_x_op(src)) {
        out.push_back(ir::Mov{.src = kRegA, .dst = kRegX});
      }
      ++i;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  return out;
}

}  // namespace

asm_ast::Program apply_loop_counter_to_x(asm_ast::Program prog,
                                         const SymbolAddrs* zp_symbol_addrs) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      if (std::optional<Plan> plan = plan_promotion(fn->instructions, zp_symbol_addrs)) {
        fn->instructions = apply_plan(fn->instructions, *plan);
      }
    }
  }
  return prog;
}

}  // namespace c6502::passes
