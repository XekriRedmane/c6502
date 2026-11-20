// Rewrites `LDA M` to `TXA` when M is an X-save slot.
//
// Motivating shape
//
// When the asm-SSA regalloc colors a loop counter (or any Pseudo with
// uses that X can serve) to `Reg(X)`, it also uses a memory slot
// `M = __local_<fn>__<name>` as the home that X is saved to across
// calls, in the standard `STX M; JSR callee; LDX M` wrap.
//
// If the same Pseudo also appears as a value, for instance passed as a
// zp_abi argument by `LDA M; STA __zpabi_<callee>__<slot>`, the code
// after coloring reads M alongside the in-place steps of X (DEX / INX),
// which leave M stale until the next STX M. The pattern looks like
// this.
//
//     LDA M                       ; reads stale M after DEX
//     STA __zpabi_callee_slot
//     STX M                       ; syncs M, after the LDA M has
//                                 ; already read the stale value
//     JSR callee
//     LDX M
//
// The regalloc intends M and X to represent the same logical value,
// with X holding the live copy and M its spill home. Under that
// invariant `LDA M` gives the same value as `TXA`, and TXA is the
// better of the two, because it reads X (the live copy) where M can be
// stale between a DEX / INX and the next STX M.
//
// Rewrite
//
// For each function, the memory operands that are the destination of a
// `Mov(Reg(X), M)` (STX M) anywhere in the function are the "X-save
// slots". Each `Mov(M, Reg(A))` (LDA M) whose source is one of those
// slots becomes `Mov(Reg(X), Reg(A))` (TXA).
//
// The slots are taken from the whole function because the invariant is
// global to the function. If M is ever stored from X, the regalloc has
// committed to treating M as X's spill home for the entire function, so
// any read of M should read X's current value.
//
// Soundness
//
// The rewrite is sound when M's value at the LDA M is supposed to match
// X's value at the same point. That requires these conditions.
//
// - M is initialized to match X, either by a direct STX M or by an init
//   pattern like `STA M; TAX` that writes the same value to both.
// - Every write to M is STX M, which keeps M == X.
// - Every write to X is DEX / INX, which keeps the slot's meaning,
//   since the new X is the slot's next value, "slot-1" or "slot+1".
//
// The invariant starts only where X is first given M's value. Before
// that, X holds whatever the caller left in it, and M is the only copy,
// as in `LDA p; STA out; LDX p; DEX; STX p` with `p` a global. So the
// rewrite applies only to a read of M that every path from the
// function's entry reaches through an `STX M`, an `LDX M`, or the init
// `TAX` (`x_holds_slot`). Once X holds M's value it keeps holding it,
// because every other X write is one of the accepted kinds below.
//
// A write to M of another kind could break the M == X invariant, so the
// pass skips any function with such a write to an X-save slot. It also
// skips any function where X is written by anything other than INX /
// DEX, `LDX M`, the init `TAX`, or a call that is immediately followed
// by `LDX M`. A call that returns a value in X is the case that
// matters, since after `STX M; JSR f`, X holds f's result and M keeps
// the saved value.
//
// Where it runs
//
// After the asm peephole fixed point and `apply_loop_counter_to_x`,
// before `expand_long_branches`. The rewrite takes one fewer byte per
// occurrence (TXA is 1 byte, LDA abs 3 and LDA zp 2), so no long-branch
// displacement grows.
#include "passes/x_save_slot_load.h"

#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_reg_x(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X>(r->reg);
}

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_reg_x_or_y(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X, ir::Y>(r->reg);
}

bool is_reg_y(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::Y>(r->reg);
}

// The byte a `Data` or `ZP` operand names (Python's `_op_key`).
struct OpKey {
  bool zp = false;
  std::string name;
  asdl::Int address = 0;
  asdl::Int offset = 0;

  bool operator==(const OpKey&) const = default;
};

std::optional<OpKey> op_key(const ir::Type_operand& op) {
  if (const auto* d = as<ir::Data>(op)) {
    return OpKey{.zp = false, .name = d->name, .offset = d->offset};
  }
  if (const auto* z = as<ir::ZP>(op)) {
    return OpKey{.zp = true, .address = z->address, .offset = z->offset};
  }
  return std::nullopt;
}

bool has_key(const ir::Type_operand& op, const OpKey& key) {
  const auto k = op_key(op);
  return k.has_value() && *k == key;
}

using Instrs = std::vector<ir::Type_instruction>;

// Returns whether `instrs[i]` is followed, skipping labels and moves
// that write neither A nor X, by a `Mov(Reg(A), Reg(X))` (TAX) before
// any instruction writes A or X. It accepts the init shape `LDA c; STA
// M; TAX` as a write to M that syncs M with X again.
bool followed_by_tax(const Instrs& instrs, std::size_t i) {
  for (std::size_t j = i + 1; j < instrs.size(); ++j) {
    const auto& instr = instrs[j];
    if (isa<ir::Label>(instr)) {
      continue;
    }
    if (const auto* mov = as<ir::Mov>(instr)) {
      if (is_reg_a(mov->src) && is_reg_x(mov->dst)) {
        return true;
      }
      if (is_reg_a(mov->dst) || is_reg_x(mov->dst)) {
        return false;
      }
      continue;
    }
    // Anything else gives up, which is conservative.
    return false;
  }
  return false;
}

// Returns whether `instr` can change X. Calls count, since the callee
// may clobber X or return a value in it. A `FunctionPrologue` runs
// before any STX to the slot, and a `Ret` / `Return` ends the function,
// so neither matters here.
bool writes_x(const ir::Type_instruction& instr) {
  if (const auto* mov = as<ir::Mov>(instr)) {
    return is_reg_x(mov->dst);
  }
  if (const auto* pop = as<ir::Pop>(instr)) {
    return is_reg_x(pop->dst);
  }
  if (const auto* inc = as<ir::Inc>(instr)) {
    return is_reg_x(inc->dst);
  }
  if (const auto* dec = as<ir::Dec>(instr)) {
    return is_reg_x(dec->dst);
  }
  return isa<ir::Call, ir::AllocateStack>(instr);
}

// Returns whether the X write at `instrs[i]` leaves X equal to the slot
// `key`, or to its in-place successor for INX / DEX. These writes are
// accepted.
//
// - `Inc(Reg(X))` / `Dec(Reg(X))`, the counter stepping in X.
// - `Mov(M, Reg(X))` (LDX M), which reloads X from the slot.
// - `Mov(Reg(A), Reg(X))` (TAX) right after `Mov(_, M)`, the init shape
//   `LDA c; STA M; TAX`.
// - A `Call` immediately followed by `LDX M`, the restore half of the
//   `STX M; JSR f; LDX M` wrap.
bool x_write_keeps_slot(const Instrs& instrs, std::size_t i, const OpKey& key) {
  const auto& instr = instrs[i];
  if (isa<ir::Inc, ir::Dec>(instr)) {
    return true;
  }
  if (const auto* mov = as<ir::Mov>(instr)) {
    if (has_key(mov->src, key)) {
      return true;
    }
    if (is_reg_a(mov->src) && i > 0) {
      // The store before the `TAX` has to leave A equal to M: a store
      // of A, or a move through A. `STX M; TAX` puts A's other value in
      // X.
      const auto* prev = as<ir::Mov>(instrs[i - 1]);
      return prev != nullptr && has_key(prev->dst, key) &&
             !is_reg_x_or_y(prev->src);
    }
    return false;
  }
  if (isa<ir::Call>(instr) && i + 1 < instrs.size()) {
    const auto* nxt = as<ir::Mov>(instrs[i + 1]);
    return nxt != nullptr && is_reg_x(nxt->dst) && has_key(nxt->src, key);
  }
  return false;
}

// Returns, for each instruction, whether X holds the slot `key`'s value
// on every path from the function's entry to it. An `STX M`, an `LDX
// M`, or the init `TAX` (`x_write_keeps_slot`) makes X hold it. The
// caller has already checked that every other X write keeps it, so
// nothing else changes the fact. X doesn't hold it at entry. An
// unreachable instruction gets true, which is harmless since it never
// runs.
std::vector<bool> x_holds_slot(const Instrs& instrs, const OpKey& key) {
  std::unordered_map<std::string, std::size_t> label_idx;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (const auto* l = as<ir::Label>(instrs[k])) {
      label_idx[l->name] = k;
    }
  }
  const std::size_t n = instrs.size();
  if (n == 0) {
    return {};
  }
  // -1 for no state yet, else 0 or 1.
  std::vector<int> state(n, -1);
  state[0] = 0;
  std::vector<std::size_t> work = {0};
  auto target_of = [&](const std::string& name) -> std::optional<std::size_t> {
    const auto it = label_idx.find(name);
    if (it == label_idx.end()) {
      return std::nullopt;
    }
    return it->second;
  };
  while (!work.empty()) {
    const std::size_t k = work.back();
    work.pop_back();
    const auto& instr = instrs[k];
    int out = state[k];
    if (const auto* mov = as<ir::Mov>(instr)) {
      if ((is_reg_x(mov->src) && has_key(mov->dst, key)) ||
          (is_reg_x(mov->dst) && has_key(mov->src, key)) ||
          (is_reg_a(mov->src) && is_reg_x(mov->dst) &&
           x_write_keeps_slot(instrs, k, key))) {
        out = 1;
      }
    }
    std::vector<std::size_t> succs;
    if (isa<ir::Ret, ir::Return>(instr)) {
    } else if (const auto* jump = as<ir::Jump>(instr)) {
      if (const auto t = target_of(jump->target)) {
        succs.push_back(*t);
      }
    } else {
      if (k + 1 < n) {
        succs.push_back(k + 1);
      }
      if (const auto* br = as<ir::Branch>(instr)) {
        if (const auto t = target_of(br->target)) {
          succs.push_back(*t);
        }
      }
    }
    for (const std::size_t succ : succs) {
      const int old = state[succ];
      const int now = old < 0 ? out : (old != 0 && out != 0 ? 1 : 0);
      if (now != old) {
        state[succ] = now;
        work.push_back(succ);
      }
    }
  }
  std::vector<bool> out(n);
  for (std::size_t k = 0; k < n; ++k) {
    out[k] = state[k] != 0;
  }
  return out;
}

void rewrite_function(ir::Function& fn) {
  const Instrs& instrs = fn.instructions;
  // Pass 1 collects the candidates, every memory operand that is the
  // destination of `Mov(Reg(X), M)` (STX M).
  std::vector<OpKey> candidates;
  for (const auto& instr : instrs) {
    if (const auto* mov = as<ir::Mov>(instr); mov != nullptr && is_reg_x(mov->src)) {
      if (auto k = op_key(mov->dst)) {
        bool seen = false;
        for (const auto& c : candidates) {
          seen = seen || c == *k;
        }
        if (!seen) {
          candidates.push_back(std::move(*k));
        }
      }
    }
  }
  // The whole-function "X mirrors M" invariant the rewrite relies on
  // holds only when X carries a single logical value spilled to one
  // home slot (the X-promoted loop counter). If X is stored to two or
  // more distinct slots, it carries several logical values over its
  // lifetime, for instance the high bytes of two different
  // register-returning calls (`STX $C0` for the first result, `STX $81`
  // for the second), and rewriting a read of one slot to `TXA` would
  // read whichever value X happens to hold at the time. The function is
  // skipped in that case, and the `LDA M` left as it is reads the
  // correct memory value.
  if (candidates.size() != 1) {
    return;
  }
  const OpKey key = candidates[0];

  // Pass 2 rejects the candidate when some write to it could leave
  // M != X. A bare `Mov(<something>, M)` that isn't `STX M` is
  // acceptable when a `TAX` (`Mov(Reg(A), Reg(X))`) after it syncs X
  // with A again, which is the init shape `LDA c; STA M; TAX` that makes
  // M = X = c. Any read-modify-write (Inc / Dec / shift / arithmetic)
  // of M, or a Pop into M, breaks the invariant.
  bool disqualified = false;
  for (std::size_t i = 0; i < instrs.size() && !disqualified; ++i) {
    const auto& instr = instrs[i];
    if (const auto* da = as<ir::DecimalArith>(instr)) {
      for (const auto& w : decimal_arith::writes(*da)) {
        disqualified = disqualified || has_key(w, key);
      }
      continue;
    }
    if (const auto* mov = as<ir::Mov>(instr)) {
      if (!has_key(mov->dst, key) || is_reg_x(mov->src)) {
        continue;  // Not M, or STX M, which is fine.
      }
      if (is_reg_y(mov->src)) {
        // STY M leaves A unchanged, so a following TAX wouldn't pick
        // up M's value.
        disqualified = true;
        continue;
      }
      if (followed_by_tax(instrs, i)) {
        // `asm_emit` writes `Mov(src, M)` as `LDA src; STA M` when src
        // isn't a register, or as `STA M` when src is Reg(A). Either way
        // A ends up holding M's value, and the TAX after it makes
        // X = A = M. This is the canonical init pattern `Mov(<v>, M);
        // Mov(Reg(A), Reg(X))`, whose load into A the memory-to-memory
        // move hides at the IR level.
        continue;
      }
      disqualified = true;
      continue;
    }
    std::visit(
        [&](const auto& ins) {
          using T = std::decay_t<decltype(ins)>;
          if constexpr (std::is_same_v<T, ir::Inc> || std::is_same_v<T, ir::Dec> ||
                        std::is_same_v<T, ir::ArithmeticShiftLeft> ||
                        std::is_same_v<T, ir::LogicalShiftRight> ||
                        std::is_same_v<T, ir::RotateLeft> ||
                        std::is_same_v<T, ir::RotateRight> ||
                        std::is_same_v<T, ir::Add> || std::is_same_v<T, ir::Sub> ||
                        std::is_same_v<T, ir::And> || std::is_same_v<T, ir::Or> ||
                        std::is_same_v<T, ir::Xor> || std::is_same_v<T, ir::Pop>) {
            if (has_key(ins.dst, key)) {
              disqualified = true;
            }
          }
        },
        instr);
  }
  if (disqualified) {
    return;
  }
  // The rewrite also needs every write to X to keep X == M. A call
  // returning a value in X, or any LDX / TAX / PLX of another value,
  // leaves X holding something else, and `LDA M` after it must keep
  // reading memory. (`STX p; JSR f; LDA p` with `f` returning an int in
  // A/X is the shape that read the callee's high byte in place of the
  // register parameter `p`.)
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (writes_x(instrs[i]) && !x_write_keeps_slot(instrs, i, key)) {
      return;
    }
  }

  // Pass 3 rewrites reads of M to reads of X.
  //
  //   `Mov(M, Reg(A))` becomes `Mov(Reg(X), Reg(A))` (LDA M to TXA).
  //   `Mov(M, D)` with D a `Data` or `ZP` byte, the addressing modes
  //   STX has, becomes `Mov(Reg(X), D)`, so the memory-to-memory
  //   `LDA M; STA D` becomes a single `STX D`.
  //
  // A memory-to-memory `Mov(M, D)` into a byte STX can't address
  // (Frame / Stack / Indirect / IndexedData) is left alone, since STX
  // has no indirect-Y or abs,X form. `Mov(M, Reg(X))` is already an LDX
  // that copy propagation and direct_index_load handle. A read that
  // some path from the entry reaches before X is given M's value keeps
  // reading memory.
  const std::vector<bool> holds = x_holds_slot(instrs, key);
  bool any = false;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto* mov = as<ir::Mov>(instrs[k]);
    if (mov != nullptr && holds[k] && has_key(mov->src, key) &&
        (is_reg_a(mov->dst) || isa<ir::Data, ir::ZP>(mov->dst))) {
      any = true;
      break;
    }
  }
  if (!any) {
    return;
  }
  const ir::Type_operand reg_x = ir::Reg{.reg = ir::X{}};
  for (std::size_t k = 0; k < fn.instructions.size(); ++k) {
    auto* mov = as<ir::Mov>(fn.instructions[k]);
    if (mov != nullptr && holds[k] && has_key(mov->src, key) &&
        (is_reg_a(mov->dst) || isa<ir::Data, ir::ZP>(mov->dst))) {
      mov->src = reg_x;
    }
  }
}

}  // namespace

asm_ast::Program apply_x_save_slot_load(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
