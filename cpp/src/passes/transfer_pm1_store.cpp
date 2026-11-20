// Folds `TXA; (SEC|CLC); (SBC|ADC) #1; STA M` to `DEX/INX; STX M`, and
// the Y mirror.
//
// The pattern
//
// Four consecutive instructions:
//
//     Mov(Reg(X), Reg(A))         // TXA, so A := X
//     SetCarry() / ClearCarry()
//     Sub(Imm(1), Reg(A))         // SBC #1, so A := A - 1
//       or
//     Add(Imm(1), Reg(A))         // ADC #1, so A := A + 1
//     Mov(Reg(A), <Data|ZP>)      // STA M
//
// become two:
//
//     Dec(Reg(X)) / Inc(Reg(X))   // DEX or INX
//     Mov(Reg(X), <Data|ZP>)      // STX M
//
// The shape for Y is the same (`TYA; ... STA M` becomes `DEY/INY; STY
// M`).
//
// Where this hits
//
// The lowering of `(c99 expr - 1)` or `(c99 expr + 1)` stored back to
// memory always goes through A, the 6502's only ALU register. When the
// source value is already in X or Y for some other reason, such as an
// `LDX m` for an indexed load just before, the chain of four atoms
// above appears. The TAC lowering can't see that X already holds m,
// and only a peephole on the asm after coloring can.
//
// The fold itself saves 3 bytes and 4 cycles over the sequence through
// A. The bigger gain comes after it. If A was spilled around the
// arithmetic, because the register allocator kept a value across the
// write of A, the spill is dead once A is no longer touched, and
// `redundant_load_elimination`, `asm_dead_store`, and `dead_a_arith`
// collect it in a later sweep of the fixed point.
//
// Soundness
//
// The original leaves `A := X-1` and `M := X-1`, with X unchanged and
// every flag set from `X-1` (TXA sets N/Z, and then SBC sets N/Z/C/V).
// The rewrite leaves `X := X-1` and `M := X-1`, with A unchanged and
// N/Z set from `X-1` by DEX, and C and V untouched.
//
// The rewrite is sound if and only if, after the STA M,
//   1. A is dead, since `X-1` is no longer left in A,
//   2. X is dead, since X now holds `X-1` where it held X, and
//   3. C and V are dead. The SBC of the original set them, and DEX
//      leaves them as they were. N and Z come out the same, since DEX
//      sets them from `X-1`, the value the SBC left in A. So a branch on
//      N or Z may follow, as in `get_next_code_byte2`'s
//      `if (++Z_PC2_L == 0)` when examples/zork/pages.c kept Z_PC2 in
//      three bytes, which became `INY; STY Z_PC2_L; BNE`.
//
// For the form that adds 1 (`CLC; ADC #1`) the change to C and V by the
// ADC is lost too, and the gate is the same.
//
// The destination
//
// STX and STY take only the `zp`, `zp,Y` (STX), `zp,X` (STY), and `abs`
// addressing modes. A plain `Data(name)` or `ZP(addr)` resolves to
// `zp` or `abs` at link time, and both work. The `IndexedData`,
// `Indirect`, `IndirectY`, `IndirectZp`, `IndirectZpY`, `Frame`, and
// `Stack` operands need addressing modes STX and STY don't have, so the
// pass leaves them alone. The `Mov` must not be volatile, as with the
// other peepholes, which leave volatile traffic between memory and A
// alone.
//
// An index register that already holds the byte
//
// The converse shape steps a byte into an index register that already
// holds it:
//
//     LDY M; ...; LDA M; CLC; ADC #1; TAY     ->     LDY M; ...; INY
//
// `rewrite_index_steps` turns `LDA M; CLC; ADC #k; TAY` into `INY` once
// or twice for `k` of 1 or 2, `SEC; SBC #k` into `DEY`, and the same
// with `TAX`, `INX`, and `DEX`, when the register still holds M: its
// last write in the block is `LDY M` (or `LDX M`), and nothing since
// writes M or might. A store through a pointer might, except to a byte
// no pointer can point to, a `static` or one of the function's own
// zero-page slots whose address the unit never takes
// (`unreachable_by_pointer`), such as Zork's `token_idx` in
// examples/zork/input.c. A, C, and V must be dead after. N and Z come
// out the same, from M plus or minus `k`.
//
// A step on either side of a transfer
//
// `rewrite_steps_across_transfer` moves a step of 1 or 2 from A to the
// index register on the other side of a transfer:
//
//     CLC; ADC #1; TAY      ->     TAY; INY        (A, C, V dead after)
//     TYA; CLC; ADC #1      ->     INY; TYA        (Y, C, V dead after)
//
// and the same for `SEC; SBC #k` with `DEY`, and for X. When the second
// is followed by a `TAY` back, all four are `INY` (A, C, V dead after).
// In the first, Y ends up with A's sum and A keeps its old value. In
// the second, A ends up with the sum and Y with it too. N and Z come out
// the same, from the sum. Zork's `match_dictionary_word`
// (examples/zork/input.c) steps its index past the count of separators
// with `y++` and adds the last step to the dictionary's address, the
// first and second forms. Each saves 2 bytes and 2 cycles for a step of
// 1, and 1 byte for a step of 2.
//
// A step stored and loaded into an index register
//
// `rewrite_step_then_reload` steps a byte in an index register when the
// stepped byte is stored back and then loaded into that register:
//
//     LDA M; SEC; SBC #2; STA M; LDX M     ->     LDX M; DEX; DEX; STX M
//
// for a step `k` of 1 or 2, `CLC; ADC #k` with `INX`, and the Y forms,
// when A, C, and V are dead after. X and M end up with the same value
// as before, and N and Z come out the same, from the last step. Zork's
// `return_value` (examples/zork/decode.c) steps the index of the local
// it restores this way, `x -= 2`. That saves 3 bytes for a step of 2 in
// zero page, and 4 for a step of 1.
//
// Where it runs
//
// Inside the peephole fixed point, after `replace_pseudoregisters`, so
// the Pseudos are resolved to their addressing modes, and before
// `expand_long_branches`. Every rewrite makes the program shorter, so
// no branch displacement grows, and the fixed point stays bounded.
#include "passes/transfer_pm1_store.h"

#include <cstddef>
#include <iterator>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using Instructions = std::vector<ir::Type_instruction>;

bool is_reg(const ir::Type_operand& op, bool (*test)(const ir::Type_reg&)) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && test(r->reg);
}
bool reg_is_a(const ir::Type_reg& r) { return isa<ir::A>(r); }
bool reg_is_index(const ir::Type_reg& r) { return isa<ir::X, ir::Y>(r); }

// Returns `Inc(reg)` for an increment and `Dec(reg)` otherwise.
ir::Type_instruction step_of(bool increment, const ir::Type_operand& reg) {
  if (increment) {
    return ir::Inc{.dst = reg};
  }
  return ir::Dec{.dst = reg};
}

// Returns the single operand `dst` of `ins`, or null for an instruction
// without one. Python's `getattr(ins, "dst", None)`, for the
// instructions that `holds` gets to.
template <class T>
concept HasOperandDst =
    requires(const T& t) { t.dst; } &&
    std::is_same_v<std::remove_cvref_t<decltype(std::declval<const T&>().dst)>,
                   ir::Type_operand>;

const ir::Type_operand* dst_of(const ir::Type_instruction& ins) {
  return std::visit(
      [](const auto& i) -> const ir::Type_operand* {
        if constexpr (HasOperandDst<std::decay_t<decltype(i)>>) {
          return &i.dst;
        } else {
          return nullptr;
        }
      },
      ins);
}

// A step found by `small_step`: whether it adds, and by how much.
struct Step {
  bool increment = false;
  int k = 0;
};

// Returns the step when `carry; arith` is `CLC; ADC #k` (an increment)
// or `SEC; SBC #k` (a decrement) on A with k of 1 or 2.
std::optional<Step> small_step(const ir::Type_instruction& carry,
                               const ir::Type_instruction& arith) {
  const ir::Type_operand* src = nullptr;
  const ir::Type_operand* dst = nullptr;
  bool increment = false;
  if (const auto* add = as<ir::Add>(arith); add && isa<ir::ClearCarry>(carry)) {
    src = &add->src;
    dst = &add->dst;
    increment = true;
  } else if (const auto* sub = as<ir::Sub>(arith); sub && isa<ir::SetCarry>(carry)) {
    src = &sub->src;
    dst = &sub->dst;
  } else {
    return std::nullopt;
  }
  const auto* imm = as<ir::Imm>(*src);
  if (imm == nullptr || !(imm->value == 1 || imm->value == 2) ||
      !is_reg(*dst, reg_is_a)) {
    return std::nullopt;
  }
  return Step{increment, static_cast<int>(imm->value)};
}

// Returns whether `ins` is a transfer, not volatile, from a register
// that `src_test` accepts to one that `dst_test` accepts.
bool is_transfer(const ir::Type_instruction* ins, bool (*src_test)(const ir::Type_reg&),
                 bool (*dst_test)(const ir::Type_reg&)) {
  const auto* m = ins != nullptr ? as<ir::Mov>(*ins) : nullptr;
  return m != nullptr && !m->is_volatile && is_reg(m->src, src_test) &&
         is_reg(m->dst, dst_test);
}

bool is_indirect(const ir::Type_operand& op) {
  return isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY, ir::Frame,
             ir::Stack, ir::IndexedData>(op);
}

// Returns whether `reg` (X or Y) holds the byte `mem` at `instrs[i]`:
// walking back through the block, the last write to `reg` is a load of
// `mem`, and nothing after it writes `mem` or might.
bool holds(const Instructions& instrs, std::size_t i, const ir::Type_operand& reg,
           const ir::Type_operand& mem, const NameSet& unaliased,
           const AsmContext& ctx) {
  const bool is_y = is_reg(reg, [](const ir::Type_reg& r) { return isa<ir::Y>(r); });
  const auto* mem_data = as<ir::Data>(mem);
  const bool is_private = mem_data != nullptr && unaliased.contains(mem_data->name);
  for (std::size_t k = i; k-- > 0;) {
    const ir::Type_instruction& ins = instrs[k];
    // A LoadAddress writes two bytes, which the check of `dst` below
    // would compare only the first of.
    if (isa<ir::Label, ir::Jump, ir::Branch, ir::Return, ir::Ret, ir::Call,
            ir::FunctionPrologue, ir::AllocateStack, ir::LoadAddress,
            ir::DecimalArith>(ins)) {
      return false;
    }
    if (const auto* m = as<ir::Mov>(ins);
        m != nullptr && m->dst == reg && m->src == mem && !m->is_volatile) {
      return true;
    }
    const ir::Type_operand* dst = dst_of(ins);
    // Any write to the register, including `INX` / `DEX`, which
    // `kills_x` doesn't count since they read X too.
    if ((is_y ? kills_y(ins) : kills_x(ins)) || (is_y && sets_up_y(ins)) ||
        (dst != nullptr && *dst == reg)) {
      return false;
    }
    if (dst == nullptr || isa<ir::Reg>(*dst)) {
      continue;
    }
    if (is_indirect(*dst)) {
      // Only an indexed store into `mem`'s own object reaches a byte
      // no pointer can.
      if (!is_private) {
        return false;
      }
      const auto* indexed = as<ir::IndexedData>(*dst);
      if (indexed != nullptr && indexed->name == mem_data->name) {
        return false;
      }
      continue;
    }
    if (*dst == mem || may_alias(*dst, mem, ctx)) {
      return false;
    }
  }
  return false;
}

// Rewrites `LDA M; CLC; ADC #k; TAY` as `INY` k times (and the `SBC`
// and X forms) where Y still holds M. See "An index register that
// already holds the byte" above.
bool rewrite_index_steps(Instructions& instrs, const NameSet& unaliased,
                         const AsmContext& ctx) {
  bool changed = false;
  std::size_t i = 0;
  while (i + 3 < instrs.size()) {
    const auto* load = as<ir::Mov>(instrs[i]);
    const auto* move = as<ir::Mov>(instrs[i + 3]);
    if (!(load != nullptr && !load->is_volatile && is_reg(load->dst, reg_is_a) &&
          isa<ir::Data, ir::ZP>(load->src) && move != nullptr &&
          is_reg(move->src, reg_is_a) && is_reg(move->dst, reg_is_index))) {
      ++i;
      continue;
    }
    const auto step = small_step(instrs[i + 1], instrs[i + 2]);
    if (!step) {
      ++i;
      continue;
    }
    const ir::Type_operand reg = move->dst;
    const ir::Type_operand mem = load->src;
    const std::size_t after = i + 4;
    if (!(holds(instrs, i, reg, mem, unaliased, ctx) && a_dead_at(instrs, after) &&
          c_dead_at(instrs, after) && v_dead_at(instrs, after))) {
      ++i;
      continue;
    }
    Instructions steps(static_cast<std::size_t>(step->k), step_of(step->increment, reg));
    instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(i),
                 instrs.begin() + static_cast<std::ptrdiff_t>(i + 4));
    instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(i), steps.begin(), steps.end());
    changed = true;
    ++i;
  }
  return changed;
}

void splice(Instructions& instrs, std::size_t begin, std::size_t end,
            Instructions replacement) {
  instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(begin),
               instrs.begin() + static_cast<std::ptrdiff_t>(end));
  instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(begin),
                std::make_move_iterator(replacement.begin()),
                std::make_move_iterator(replacement.end()));
}

// Rewrites `CLC; ADC #k; TAY` as `TAY; INY`, and `TYA; CLC; ADC #k` as
// `INY; TYA` (k of 1 or 2, `SBC` as `DEY`, and the X forms). See "A
// step on either side of a transfer" above.
bool rewrite_steps_across_transfer(Instructions& instrs) {
  bool changed = false;
  std::size_t i = 0;
  while (i + 2 < instrs.size()) {
    const ir::Type_instruction& first = instrs[i];
    const ir::Type_instruction& second = instrs[i + 1];
    const ir::Type_instruction& third = instrs[i + 2];
    std::size_t after = i + 3;
    std::optional<Instructions> rewritten;
    if (const auto found = small_step(first, second);
        found && is_transfer(&third, reg_is_a, reg_is_index) &&
        a_dead_at(instrs, after) && c_dead_at(instrs, after) &&
        v_dead_at(instrs, after)) {
      const ir::Type_operand& reg = std::get<ir::Mov>(third).dst;
      Instructions out{third};
      for (int k = 0; k < found->k; ++k) {
        out.push_back(step_of(found->increment, reg));
      }
      rewritten = std::move(out);
    }
    if (const auto found = small_step(second, third);
        !rewritten && found && is_transfer(&first, reg_is_index, reg_is_a)) {
      const ir::Type_operand reg = std::get<ir::Mov>(first).src;
      const bool is_x = isa<ir::X>(std::get<ir::Reg>(reg).reg);
      const ir::Type_instruction* back = after < instrs.size() ? &instrs[after] : nullptr;
      auto same_reg = is_x ? +[](const ir::Type_reg& r) { return isa<ir::X>(r); }
                           : +[](const ir::Type_reg& r) { return isa<ir::Y>(r); };
      Instructions steps;
      for (int k = 0; k < found->k; ++k) {
        steps.push_back(step_of(found->increment, reg));
      }
      if (is_transfer(back, reg_is_a, same_reg) && a_dead_at(instrs, after + 1) &&
          c_dead_at(instrs, after + 1) && v_dead_at(instrs, after + 1)) {
        // `TXA; CLC; ADC #1; TAX` is `INX`.
        rewritten = std::move(steps);
        after += 1;
      } else if ((is_x ? x_dead_at(instrs, after) : y_dead_at(instrs, after)) &&
                 c_dead_at(instrs, after) && v_dead_at(instrs, after)) {
        steps.push_back(first);
        rewritten = std::move(steps);
      }
    }
    if (!rewritten) {
      ++i;
      continue;
    }
    const std::size_t length = rewritten->size();
    splice(instrs, i, after, std::move(*rewritten));
    changed = true;
    i += length;
  }
  return changed;
}

// Rewrites `LDA M; SEC; SBC #k; STA M; LDX M` as `LDX M; DEX; [DEX;]
// STX M` (k of 1 or 2, `ADC` as `INX`, and the Y forms). See "A step
// stored and loaded into an index register" above.
bool rewrite_step_then_reload(Instructions& instrs) {
  bool changed = false;
  std::size_t i = 0;
  while (i + 4 < instrs.size()) {
    const auto found = small_step(instrs[i + 1], instrs[i + 2]);
    const auto* load = as<ir::Mov>(instrs[i]);
    const auto* reload = as<ir::Mov>(instrs[i + 4]);
    if (!(found && load != nullptr && !load->is_volatile &&
          isa<ir::Data, ir::ZP>(load->src) && is_reg(load->dst, reg_is_a) &&
          instrs[i + 3] == ir::Type_instruction{ir::Mov{.src = load->dst,
                                                        .dst = load->src}} &&
          reload != nullptr && !reload->is_volatile && reload->src == load->src &&
          is_reg(reload->dst, reg_is_index))) {
      ++i;
      continue;
    }
    const std::size_t after = i + 5;
    if (!(a_dead_at(instrs, after) && c_dead_at(instrs, after) &&
          v_dead_at(instrs, after))) {
      ++i;
      continue;
    }
    const ir::Type_operand reg = reload->dst;
    const ir::Type_operand mem = load->src;
    Instructions out{instrs[i + 4]};
    for (int k = 0; k < found->k; ++k) {
      out.push_back(step_of(found->increment, reg));
    }
    out.push_back(ir::Mov{.src = reg, .dst = mem});
    splice(instrs, i, after, std::move(out));
    changed = true;
    i += static_cast<std::size_t>(found->k) + 2;
  }
  return changed;
}

// The match of the pattern of four atoms: the index register, whether
// the step is a decrement, and the destination of the STA.
struct Match {
  ir::Type_operand reg;
  bool is_decrement = false;
  ir::Type_operand dst;
};

// Returns the match when the pattern of four atoms matches at `i` and
// every gate passes.
std::optional<Match> match_pattern(const Instructions& instrs, std::size_t i) {
  if (i + 3 >= instrs.size()) {
    return std::nullopt;
  }
  // a: Mov(Reg(X)|Reg(Y), Reg(A)), not volatile.
  const auto* a = as<ir::Mov>(instrs[i]);
  if (a == nullptr || a->is_volatile || !is_reg(a->src, reg_is_index) ||
      !is_reg(a->dst, reg_is_a)) {
    return std::nullopt;
  }
  const ir::Type_instruction& b = instrs[i + 1];
  const ir::Type_instruction& c = instrs[i + 2];
  // c: Sub(Imm(1), Reg(A)), which is SBC #1, or Add(Imm(1), Reg(A)),
  // which is ADC #1.
  const ir::Type_operand* c_src = nullptr;
  const ir::Type_operand* c_dst = nullptr;
  bool is_decrement = false;
  if (const auto* sub = as<ir::Sub>(c)) {
    if (!isa<ir::SetCarry>(b)) {
      return std::nullopt;
    }
    c_src = &sub->src;
    c_dst = &sub->dst;
    is_decrement = true;
  } else if (const auto* add = as<ir::Add>(c)) {
    if (!isa<ir::ClearCarry>(b)) {
      return std::nullopt;
    }
    c_src = &add->src;
    c_dst = &add->dst;
  } else {
    return std::nullopt;
  }
  const auto* imm = as<ir::Imm>(*c_src);
  if (imm == nullptr || imm->value != 1 || !is_reg(*c_dst, reg_is_a)) {
    return std::nullopt;
  }
  // d: Mov(Reg(A), Data|ZP), not volatile.
  const auto* d = as<ir::Mov>(instrs[i + 3]);
  if (d == nullptr || d->is_volatile || !is_reg(d->src, reg_is_a) ||
      !isa<ir::Data, ir::ZP>(d->dst)) {
    return std::nullopt;
  }
  // The gates at the boundary after the STA.
  const std::size_t after = i + 4;
  if (!a_dead_at(instrs, after)) {
    return std::nullopt;
  }
  const bool is_x = isa<ir::X>(std::get<ir::Reg>(a->src).reg);
  if (!(is_x ? x_dead_at(instrs, after) : y_dead_at(instrs, after))) {
    return std::nullopt;
  }
  if (!(c_dead_at(instrs, after) && v_dead_at(instrs, after))) {
    return std::nullopt;
  }
  return Match{a->src, is_decrement, d->dst};
}

// Rewrites each `TXA; SEC; SBC #1; STA M` (and its forms) as `DEX; STX
// M`.
bool rewrite_pm1_stores(Instructions& instrs) {
  Instructions out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (const auto match = match_pattern(instrs, i)) {
      const ir::Type_operand reg =
          ir::Reg{.reg = std::get<ir::Reg>(match->reg).reg};
      out.push_back(step_of(!match->is_decrement, reg));
      out.push_back(ir::Mov{.src = reg, .dst = match->dst, .is_volatile = false});
      i += 4;
      changed = true;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    instrs = std::move(out);
  }
  return changed;
}

}  // namespace

asm_ast::Program apply_transfer_pm1_store(asm_ast::Program prog,
                                          const AsmContext& ctx) {
  const NameSet unaliased = unreachable_by_pointer(prog);
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_pm1_stores(fn->instructions);
      rewrite_index_steps(fn->instructions, unaliased, ctx);
      rewrite_steps_across_transfer(fn->instructions);
      rewrite_step_then_reload(fn->instructions);
    }
  }
  return prog;
}

}  // namespace c6502::passes
