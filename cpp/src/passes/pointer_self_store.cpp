// Drops a store through a zero-page pointer of the byte read there.
//
// The pattern
//
// `P->buffer += 0x100` on a pointer member reads both bytes of the
// member through `P`, adds 1 to the high byte, and writes both bytes
// back. The low byte goes back unchanged:
//
//     LDY #8
//     LDA (P),Y        ; the low byte
//     STA t1
//     INY
//     LDA (P),Y        ; the high byte
//     STA t0
//     INC t0
//     LDA t1
//     DEY
//     STA (P),Y        ; the low byte, as it was read
//     LDA t0
//     INY
//     STA (P),Y
//
// Within a basic block, the pass follows which value A and each
// zero-page or absolute byte hold when that value is a byte read
// through a pointer, `IndirectZp(N, k)`, the byte at `(N),Y` with
// `Y = k`. A store of A to `IndirectZp(N, k)` that isn't volatile, when
// A holds the byte read from `IndirectZp(N, k)` and nothing since could
// have changed that byte or the pointer, writes the value the byte
// already holds, and goes. `asm_dead_store` and `dead_a_arith` then
// remove the copy into `t1` and the read, and `shift_in_a` makes the
// rest `LDY #9; LDA (P),Y; CLC; ADC #1; STA (P),Y`.
//
// What ends a fact
//
// A fact that A or a byte holds the byte at `(N)+k` ends at
//
// - a write of `N` or `N + 1`, which moves the pointer,
// - a write that may reach `(N)+k` (`may_alias`, with the unit's bytes
//   that no pointer reaches installed as the pointer proof). A store
//   through another pointer, or to a byte a pointer can reach (a user
//   zero-page object, a static outside zero page), may. A store to a
//   compiler slot no pointer reaches, or through `N` at another offset
//   `k'`, doesn't, since `(N)+k'` differs from `(N)+k` while `N` is
//   unchanged,
// - a call, a compound atom, a label, or a branch, since the pass looks
//   at one basic block at a time.
//
// A write of a byte ends what that byte held, and a write of A ends
// what A held. A volatile access never makes a fact, and a volatile
// store is never dropped. Dropping the store changes no register or
// flag, since `STA` writes neither, and the value in memory is the
// value it held.
//
// Where it runs
//
// In the peephole fixed point after `memory_value_propagation`, which
// makes the `IndirectZp` operands, and before `asm_dead_store`.
#include "passes/pointer_self_store.h"

#include <map>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using Int = ::c6502::asdl::Int;

// The value of a byte read through a pointer: (N, k) for the byte at
// `(N),Y` with `Y = k`.
using Value = std::pair<Int, Int>;

// A key for a byte with a fixed address: (0, "", address) for a `ZP`
// byte, with its offset added in, and (1, name, offset) for a `Data`
// byte. Python's `("zp", address)` and `("data", name, offset)`.
using Cell = std::tuple<int, std::string, Int>;

std::optional<Cell> cell(const ir::Type_operand& op) {
  if (const auto* z = as<ir::ZP>(op)) {
    return Cell{0, std::string(), z->address + z->offset};
  }
  if (const auto* d = as<ir::Data>(op)) {
    return Cell{1, d->name, d->offset};
  }
  return std::nullopt;
}

// Returns the operand of a `cell` key.
ir::Type_operand cell_operand(const Cell& key) {
  if (std::get<0>(key) == 0) {
    return ir::ZP{.address = std::get<2>(key), .offset = 0};
  }
  return ir::Data{.name = std::get<1>(key), .offset = std::get<2>(key)};
}

bool is_barrier(const ir::Type_instruction& ins) {
  return isa<ir::Label, ir::Jump, ir::Branch, ir::JumpTable, ir::Call, ir::Ret,
             ir::Return, ir::FunctionPrologue, ir::AllocateStack,
             ir::LoadAddress, ir::Phi>(ins);
}

bool keeps_a(const ir::Type_instruction& ins) {
  return isa<ir::Compare, ir::BitTest, ir::SetCarry, ir::ClearCarry, ir::Push>(ins);
}

// Returns the destination of a read-modify-write, an operation of the
// ALU, or a `Pop`, or null for any other instruction.
const ir::Type_operand* rmw_alu_pop_dst(const ir::Type_instruction& ins) {
  return std::visit(
      [](const auto& i) -> const ir::Type_operand* {
        using T = std::decay_t<decltype(i)>;
        if constexpr (std::is_same_v<T, ir::Inc> || std::is_same_v<T, ir::Dec> ||
                      std::is_same_v<T, ir::ArithmeticShiftLeft> ||
                      std::is_same_v<T, ir::LogicalShiftRight> ||
                      std::is_same_v<T, ir::RotateLeft> ||
                      std::is_same_v<T, ir::RotateRight> ||
                      std::is_same_v<T, ir::Add> || std::is_same_v<T, ir::Sub> ||
                      std::is_same_v<T, ir::And> || std::is_same_v<T, ir::Or> ||
                      std::is_same_v<T, ir::Xor> || std::is_same_v<T, ir::Pop>) {
          return &i.dst;
        } else {
          return nullptr;
        }
      },
      ins);
}

class State {
 public:
  State(const SymbolAddrs& zp_addrs, const AsmContext& ctx)
      : zp_addrs_(zp_addrs), ctx_(ctx) {}

  const SymbolAddrs& zp_addrs() const { return zp_addrs_; }

  std::optional<Value> a;
  std::map<Cell, Value> cells;
  // The index of the load through a pointer that set A, when A holds
  // what it read.
  std::optional<std::size_t> a_from;
  // For a byte whose fact a store right after such a load made, the
  // store's index, until something reads the byte.
  std::map<Cell, std::size_t> origin;

  void reset() {
    a.reset();
    a_from.reset();
    cells.clear();
    origin.clear();
  }

  // Ends the `origin` of each byte `ins` reads.
  void read(const ir::Type_instruction& ins) {
    for (const auto& op : asm_dead_store::read_operands(ins)) {
      if (const auto key = cell(op)) {
        origin.erase(*key);
      }
    }
  }

  // Ends the facts a write of `dst` may change.
  void write(const ir::Type_operand& dst) {
    const MayAliasOptions options{.zp_slot_symbols = &zp_addrs_};
    if (const auto key = cell(dst)) {
      cells.erase(*key);
      origin.erase(*key);
    } else {
      // A store or read-modify-write of an element, `STA t,X` or
      // `DEC t,X`, may write any byte of `t`.
      for (auto it = cells.begin(); it != cells.end();) {
        if (may_alias(cell_operand(it->first), dst, ctx_, options)) {
          origin.erase(it->first);
          it = cells.erase(it);
        } else {
          ++it;
        }
      }
    }
    const std::optional<Int> addr = address(dst);
    bool unresolved = false;
    if (const auto* d = as<ir::Data>(dst); d != nullptr && !addr) {
      unresolved = d->name.starts_with("__local_") ||
                   d->name.starts_with("__zpabi_") || zp_addrs_.contains(d->name);
    }
    auto survives = [&](const Value& v) {
      const auto& [n, k] = v;
      if (unresolved || (addr && (*addr == n || *addr == n + 1))) {
        return false;
      }
      return !may_alias(ir::IndirectZp{.address = n, .offset = k}, dst, ctx_,
                        options);
    };
    if (a && !survives(*a)) {
      a.reset();
      a_from.reset();
    }
    for (auto it = cells.begin(); it != cells.end();) {
      if (!survives(it->second)) {
        origin.erase(it->first);
        it = cells.erase(it);
      } else {
        ++it;
      }
    }
  }

 private:
  std::optional<Int> address(const ir::Type_operand& op) const {
    if (const auto* z = as<ir::ZP>(op)) {
      return z->address + z->offset;
    }
    if (const auto* d = as<ir::Data>(op)) {
      const auto it = zp_addrs_.find(d->name);
      if (it == zp_addrs_.end()) {
        return std::nullopt;
      }
      return it->second + d->offset;
    }
    return std::nullopt;
  }

  const SymbolAddrs& zp_addrs_;
  const AsmContext& ctx_;
};

// Returns whether `ins` stores A through a pointer where A holds the
// byte read from there.
bool drops(const ir::Mov& ins, const State& state) {
  if (ins.is_volatile || !is_reg_a(ins.src)) {
    return false;
  }
  const auto* dst = as<ir::IndirectZp>(ins.dst);
  return dst != nullptr && state.a && *state.a == Value{dst->address, dst->offset};
}

// Returns `LDA (N),Y` with `Y = k` in place of `instrs[i]`, `LDA t`,
// when `t` holds the byte at `(N)+k`, read just before the store into
// `t` with A dead after that store, nothing has read `t` since, and
// `t` is dead after `instrs[i]`, and so is Y. The load and the store
// into `t` are then dead too, and later sweeps remove them, so that the
// byte is read once, where it's used. Returns nothing otherwise.
std::optional<ir::Mov> reload(const ir::Function& fn, std::size_t i,
                              const State& state, const AsmContext& ctx) {
  const auto& instrs = fn.instructions;
  const auto* ins = as<ir::Mov>(instrs[i]);
  if (ins == nullptr || !is_reg_a(ins->dst) || ins->is_volatile) {
    return std::nullopt;
  }
  const auto key = cell(ins->src);
  if (!key || !state.origin.contains(*key) || !state.cells.contains(*key)) {
    return std::nullopt;
  }
  const auto* data = as<ir::Data>(ins->src);
  if (data == nullptr || !data->name.starts_with("__local_" + fn.name + "__")) {
    return std::nullopt;
  }
  const std::size_t s = state.origin.at(*key);
  if (!(a_dead_at(instrs, s + 1) && y_dead_at(instrs, i + 1) &&
        asm_dead_store::mem_dead_after(instrs, i, ins->src, ctx, fn.name,
                                       &state.zp_addrs()))) {
    return std::nullopt;
  }
  const auto& [n, k] = state.cells.at(*key);
  return ir::Mov{.src = ir::IndirectZp{.address = n, .offset = k}, .dst = ins->dst};
}

void transfer_mov(const ir::Mov& ins, State& state, std::size_t i) {
  const ir::Type_operand& src = ins.src;
  const ir::Type_operand& dst = ins.dst;
  if (isa<ir::Reg>(dst)) {
    if (!is_reg_a(dst)) {
      return;
    }
    state.a_from.reset();
    if (ins.is_volatile) {
      state.a.reset();
    } else if (const auto* p = as<ir::IndirectZp>(src)) {
      state.a = Value{p->address, p->offset};
      state.a_from = i;
    } else {
      state.a.reset();
      if (const auto key = cell(src)) {
        if (const auto it = state.cells.find(*key); it != state.cells.end()) {
          state.a = it->second;
        }
      }
    }
    return;
  }
  // A write of memory. A store of A, X, or Y leaves A, and any other
  // move into memory goes through A (`LDA src; STA dst`), as does a
  // move of X or Y through a pointer.
  std::optional<Value> value;
  const bool src_is_a = is_reg_a(src);
  if (isa<ir::Reg>(src)) {
    if (src_is_a) {
      value = state.a;
    } else if (!cell(dst)) {
      state.a.reset();
    }
  } else {
    if (!ins.is_volatile) {
      if (const auto* p = as<ir::IndirectZp>(src)) {
        value = Value{p->address, p->offset};
      } else if (const auto key = cell(src)) {
        if (const auto it = state.cells.find(*key); it != state.cells.end()) {
          value = it->second;
        }
      }
    }
    state.a = value;
  }
  state.write(dst);
  if (src_is_a) {
    // The write may have ended A's own fact (a store through another
    // pointer), and then the stored byte holds A's value only as it
    // was.
    value = state.a;
  }
  const auto key = cell(dst);
  if (key && value && !ins.is_volatile) {
    state.cells[*key] = *value;
    if (src_is_a && i > 0 && state.a_from == i - 1) {
      state.origin[*key] = i;
    }
  }
}

void rewrite_function(ir::Function& fn, const SymbolAddrs& zp_addrs,
                      const AsmContext& ctx) {
  State state(zp_addrs, ctx);
  const auto& instrs = fn.instructions;
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  bool changed = false;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const ir::Type_instruction& ins = instrs[i];
    if (is_barrier(ins)) {
      state.reset();
      out.push_back(ins);
      continue;
    }
    if (const auto* mov = as<ir::Mov>(ins)) {
      if (drops(*mov, state)) {
        changed = true;
        continue;
      }
      if (auto fresh = reload(fn, i, state, ctx)) {
        changed = true;
        transfer_mov(*fresh, state, i);
        out.push_back(std::move(*fresh));
      } else {
        state.read(ins);
        transfer_mov(*mov, state, i);
        out.push_back(ins);
      }
      continue;
    }
    state.read(ins);
    if (const auto* dec = as<ir::DecimalArith>(ins)) {
      for (const auto& w : decimal_arith::writes(*dec)) {
        state.write(w);
      }
      state.a.reset();
    } else if (keeps_a(ins)) {
      // These leave A and memory alone.
    } else if (const ir::Type_operand* dst = rmw_alu_pop_dst(ins)) {
      if (is_reg_a(*dst)) {
        state.a.reset();
      } else if (!isa<ir::Reg>(*dst)) {
        state.write(*dst);
      }
    } else {
      state.reset();
    }
    state.a_from.reset();
    out.push_back(ins);
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_pointer_self_store_drop(asm_ast::Program prog,
                                               const AsmContext& ctx,
                                               const SymbolAddrs* zp_symbol_addrs) {
  SymbolAddrs addrs = runtime_zp_addrs(ctx);
  if (zp_symbol_addrs != nullptr) {
    for (const auto& [name, addr] : *zp_symbol_addrs) {
      addrs[name] = addr;
    }
  }
  const AsmContext inner = ctx.with_pointer_proof(unreachable_by_pointer(prog));
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, addrs, inner);
    }
  }
  return prog;
}

}  // namespace c6502::passes
