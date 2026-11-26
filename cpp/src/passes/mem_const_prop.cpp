// Forward propagation of the immediates that memory bytes hold, within
// a basic block.
//
// The pass follows the value of each named byte (`Data(name, offset)`
// and `ZP(addr, offset)`) within a basic block while the byte is known
// to hold an immediate, and puts the immediate in place of a later read
// of the byte in an operand that takes an `Imm`.
//
// The case that motivated it
//
// `(uchar & 0x80) != 0` after integer promotion lowers, once the byte
// shuffle of the shift, the fold of `Mov(Imm, A); And(Imm, A)`, and the
// truncation of the high byte have run, to
//
//     LDA #$00
//     STA M_hi        ; M_hi = 0
//     LDA M_lo
//     ORA M_hi        ; reads the 0 just stored
//
// `ORA M_hi` takes an `Imm` as its source. Since `M_hi` is known to be
// 0, it becomes `ORA #$00`, which `const_arith_fold` then drops as an
// identity, and the store to `M_hi` is left dead for `asm_dead_store`.
//
// The state, kept for one basic block
//
//   * `a_value` is the immediate A holds when that is known. A
//     `Mov(Imm, A)` sets it, and so does a `Mov(Imm, <mem>)`, which
//     `asm_emit` writes as `LDA #imm; STA mem`. Any other write of A
//     clears it, including the `LDA src` of a move from memory to
//     memory, the `TXA` of a move from X or Y that `STX` / `STY` can't
//     make, and a `PLA`.
//   * `mem_value` maps each tracked byte to the immediate it holds. A
//     `Mov(Reg(A), <mem>)` records A's value when it is known, a
//     `Mov(Imm(c), <mem>)` records `c`, and a write of a byte that may
//     alias a tracked one removes that one.
//
// The substitution
//
// Each instruction's source operands are looked up before the state is
// updated for it, and a `Data(name, k)` or `ZP(addr, k)` whose value is
// known becomes that `Imm`. Only the operands that take an `Imm` are
// candidates: the source of a `Mov` into A, of an `And`, `Or`, `Add`,
// or `Sub`, and the right side of a `Compare`.
//
// The boundaries of a block
//
// All state is cleared at
//
//   * a `Label`, since control may arrive there from anywhere,
//   * a `Jump`, `Branch`, `Return`, or `Ret`, which leave the block,
//   * a `Call`, since the routine called may write any memory,
//   * a `LoadAddress`, `FunctionPrologue`, `AllocateStack`, or
//     `DecimalArith`, compound instructions that `asm_to_asm2` expands
//     later and whose inner states the pass doesn't follow.
//
// Aliasing
//
// The model is conservative. A write whose destination isn't a definite
// `Data` or `ZP` byte removes every tracked byte. A write through
// `IndexedData(name, off, X)` removes every `Data(name, *)`, since the
// index can reach any byte of the range. A write through `Frame`,
// `Stack`, or `Indirect`, the soft-stack and indirect-Y modes, removes
// everything, since the address it writes isn't known.
//
// Where it runs
//
// Inside the peephole fixed point, before `const_arith_fold`, so that the
// identity drop sees the substituted operand. It runs after
// `replace_pseudoregisters`, so no `Pseudo` is left, and before
// `expand_long_branches`. It doesn't touch branches.
//
// The substitutions and the updates are functions of the input alone,
// so a second run gives the same program, which the fixed point needs.
#include "passes/mem_const_prop.h"

#include <map>
#include <optional>
#include <string>
#include <tuple>
#include <utility>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using K = ir::Kind_instruction;

using MemKey = std::tuple<int, std::string, long long, long long>;

struct BlockState {
  std::optional<int> a_value;
  std::map<MemKey, int> mem_value;

  void reset() {
    a_value.reset();
    mem_value.clear();
  }
};

std::optional<MemKey> mem_key(const ir::Type_operand& op) {
  if (const auto* d = as<ir::Data>(op)) {
    return MemKey{0, d->name, 0, static_cast<long long>(d->offset)};
  }
  if (const auto* z = as<ir::ZP>(op)) {
    return MemKey{1, std::string(), static_cast<long long>(z->address),
                  static_cast<long long>(z->offset)};
  }
  return std::nullopt;
}

std::optional<ir::Type_operand> substitute_source(const ir::Type_operand& op,
                                                  const BlockState& state) {
  const auto key = mem_key(op);
  if (!key) {
    return std::nullopt;
  }
  const auto it = state.mem_value.find(*key);
  if (it == state.mem_value.end()) {
    return std::nullopt;
  }
  return ir::Imm{.value = it->second};
}

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_block_boundary(const ir::Type_instruction& instr) {
  return isa<ir::Label, ir::Jump, ir::Branch, ir::Ret, ir::Return, ir::Call,
             ir::LoadAddress, ir::FunctionPrologue, ir::AllocateStack,
             ir::DecimalArith>(instr);
}

int low_byte(asdl::Int v) { return static_cast<int>(asdl::floor_mod(v, 256)); }

void invalidate_alias(BlockState& state, const ir::Type_operand& dst) {
  if (isa<ir::Data, ir::ZP>(dst)) {
    state.mem_value.erase(*mem_key(dst));
    return;
  }
  if (const auto* idx = as<ir::IndexedData>(dst)) {
    for (auto it = state.mem_value.begin(); it != state.mem_value.end();) {
      if (std::get<0>(it->first) == 0 && std::get<1>(it->first) == idx->name) {
        it = state.mem_value.erase(it);
      } else {
        ++it;
      }
    }
    return;
  }
  state.mem_value.clear();
}

std::optional<ir::Type_instruction> substitute(const ir::Type_instruction& instr,
                                               const BlockState& state) {
  if (const auto* mov = as<ir::Mov>(instr)) {
    if (!mov->is_volatile && is_reg_a(mov->dst)) {
      if (auto src = substitute_source(mov->src, state)) {
        return ir::Mov{.src = std::move(*src), .dst = mov->dst};
      }
    }
    return std::nullopt;
  }
  ir::Type_instruction out = instr;
  ir::Type_operand* src = nullptr;
  switch (ir::kind(out)) {
    case K::And:
      src = &std::get<ir::And>(out).src;
      break;
    case K::Or:
      src = &std::get<ir::Or>(out).src;
      break;
    case K::Add:
      src = &std::get<ir::Add>(out).src;
      break;
    case K::Sub:
      src = &std::get<ir::Sub>(out).src;
      break;
    case K::Compare:
      src = &std::get<ir::Compare>(out).right;
      break;
    default:
      return std::nullopt;
  }
  auto imm = substitute_source(*src, state);
  if (!imm) {
    return std::nullopt;
  }
  *src = std::move(*imm);
  return out;
}

const ir::Type_operand* written_dst(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Add:
      return &std::get<ir::Add>(instr).dst;
    case K::Sub:
      return &std::get<ir::Sub>(instr).dst;
    case K::Xor:
      return &std::get<ir::Xor>(instr).dst;
    case K::Inc:
      return &std::get<ir::Inc>(instr).dst;
    case K::Dec:
      return &std::get<ir::Dec>(instr).dst;
    case K::ArithmeticShiftLeft:
      return &std::get<ir::ArithmeticShiftLeft>(instr).dst;
    case K::LogicalShiftRight:
      return &std::get<ir::LogicalShiftRight>(instr).dst;
    case K::RotateLeft:
      return &std::get<ir::RotateLeft>(instr).dst;
    case K::RotateRight:
      return &std::get<ir::RotateRight>(instr).dst;
    default:
      return nullptr;
  }
}

bool writes_a_through_move(const ir::Mov& mov) {
  if (!isa<ir::Reg>(mov.src)) {
    return !isa<ir::Reg>(mov.dst);
  }
  if (is_reg_a(mov.src) || mov.src == mov.dst) {
    return false;
  }
  return !isa<ir::Data, ir::ZP>(mov.dst);
}

void update_state(const ir::Type_instruction& instr, BlockState& state) {
  if (const auto* pop = as<ir::Pop>(instr)) {
    if (is_reg_a(pop->dst)) {
      state.a_value.reset();
    } else if (!isa<ir::Reg>(pop->dst)) {
      invalidate_alias(state, pop->dst);
    }
    return;
  }
  if (const auto* mov = as<ir::Mov>(instr)) {
    if (mov->is_volatile) {
      state.a_value.reset();
      if (!is_reg_a(mov->dst)) {
        invalidate_alias(state, mov->dst);
      }
      return;
    }
    if (is_reg_a(mov->dst)) {
      if (const auto* imm = as<ir::Imm>(mov->src)) {
        state.a_value = low_byte(imm->value);
      } else {
        state.a_value.reset();
      }
      return;
    }
    if (const auto* imm = as<ir::Imm>(mov->src); imm != nullptr && !isa<ir::Reg>(mov->dst)) {
      state.a_value = low_byte(imm->value);
    } else if (writes_a_through_move(*mov)) {
      state.a_value.reset();
    }
    const auto key = mem_key(mov->dst);
    if (!key) {
      if (isa<ir::Reg>(mov->dst)) {
        return;
      }
      invalidate_alias(state, mov->dst);
      return;
    }
    if (const auto* imm = as<ir::Imm>(mov->src)) {
      state.mem_value[*key] = low_byte(imm->value);
      return;
    }
    if (is_reg_a(mov->src) && state.a_value) {
      state.mem_value[*key] = *state.a_value;
      return;
    }
    state.mem_value.erase(*key);
    return;
  }
  switch (ir::kind(instr)) {
    case K::And:
    case K::Or: {
      const bool is_and = isa<ir::And>(instr);
      const ir::Type_operand& src =
          is_and ? std::get<ir::And>(instr).src : std::get<ir::Or>(instr).src;
      const ir::Type_operand& dst =
          is_and ? std::get<ir::And>(instr).dst : std::get<ir::Or>(instr).dst;
      if (is_reg_a(dst)) {
        const auto* imm = as<ir::Imm>(src);
        if (state.a_value && imm != nullptr) {
          const int c = low_byte(imm->value);
          state.a_value = is_and ? (*state.a_value & c) : (*state.a_value | c);
        } else {
          state.a_value.reset();
        }
        return;
      }
      invalidate_alias(state, dst);
      return;
    }
    case K::Add:
    case K::Sub:
    case K::Xor:
    case K::Inc:
    case K::Dec:
    case K::ArithmeticShiftLeft:
    case K::LogicalShiftRight:
    case K::RotateLeft:
    case K::RotateRight: {
      const ir::Type_operand& dst = *written_dst(instr);
      if (is_reg_a(dst)) {
        state.a_value.reset();
        return;
      }
      invalidate_alias(state, dst);
      return;
    }
    default:
      return;
  }
}

void rewrite_function(ir::Function& fn) {
  BlockState state;
  for (auto& instr : fn.instructions) {
    if (is_block_boundary(instr)) {
      state.reset();
      continue;
    }
    if (auto rewritten = substitute(instr, state)) {
      instr = std::move(*rewritten);
    }
    update_state(instr, state);
  }
}

}  // namespace

asm_ast::Program apply_mem_const_prop(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
