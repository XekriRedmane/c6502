// Peepholes for the carry-flag return of `return_bool_in_carry`.
//
// A function declared `[[c6502::return_bool_in_carry]]` returns its
// truth value in the carry flag. `tac_to_asm` lowers both sides of that
// convention plainly,
//
//     callee:  LDA value ; CMP #1 ; RTS     C = (value >= 1)
//     caller:  JSR f ; LDA #0 ; ROL A       A = C, so 0 or 1
//
// and these rewrites tidy up what the rest of the pipeline leaves.
//
// `apply_carry_return_tails` copies a `CMP #c; RTS` return tail into
// each `LDA #k; JMP tail` that reaches it, and in front of the tail's
// label when the code falling into it ends in `LDA #k`. A comparison or
// a `return true;` inside a loop reaches the shared tail that way, with
// a constant in A. The copy puts the constant next to the compare, where
// `apply_carry_const_fold` can fold it.
//
// `apply_carry_const_fold` turns `LDA #k; CMP #c` into `SEC` (when k >=
// c, unsigned) or `CLC`, when A and N/Z are dead after it. Only C reaches
// the next reader, and C is known at compile time. It does the same for
// a compare of A, X, or Y with a constant when the register holds a
// constant from earlier in the block, such as the `CPY #1` that `return
// false;` becomes after an `LDY #0`, when `redundant_load` has loaded the
// 0 with `TYA` and `cpx_cpy_peephole` has folded `TYA; CMP #1`. The
// register keeps its value, so only N/Z must be dead.
//
// `apply_carry_branch_fold` reads the caller's captured carry directly:
//
//     LDA #0 ; ROL A ; BEQ L    becomes    BCC L
//     LDA #0 ; ROL A ; BNE L    becomes    BCS L
//
// when A, N, Z, and C are dead on both sides of the branch. After `ROL
// A`, A is the old C and Z is set if and only if A is 0, so BEQ is taken
// if and only if C was clear. The rewrite leaves C as the callee set it
// and N/Z as they were, where the original leaves C = 0, N = 0, and Z =
// !C, which is why those have to be dead. It also drops
//
//     LDA #0 ; ROL A ; CMP #1
//
// when A and N/Z are dead after it, since `CMP #1` on the 0 or 1 in A
// sets C back to the value `ROL` read. That's `return g();` in one
// carry-flag function calling another.
//
// A caller that uses the value of a returned struct before testing its
// flag stores the capture and loads it back for the test:
//
//     JSR f ; STA v ; LDA #0 ; ROL A ; STA flag
//     LDA v ; STA out                    nothing here touches C
//     LDA flag ; BNE L
//
// `ROL A` clears C, so the load can't simply become `BCS`. Instead the
// capture and its store go, which leaves C holding the callee's flag up
// to the test, and `LDA flag ; BNE L` becomes `BCS L` (`BEQ` becomes
// `BCC`). That needs the instructions in between not to touch C or
// `flag`, A and N/Z dead after the store (they held the capture), A,
// N/Z, and C dead on both sides of the branch, and `flag` dead after the
// load. Zork's `if (found.not_found)` right after `PAGE_TABLE_INDEX2 =
// found.slot;` has this shape.
#include "passes/carry_return.h"

#include <concepts>
#include <cstddef>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrList = std::vector<ir::Type_instruction>;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};

bool is_imm_to_a(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && isa<ir::Imm>(m->src) && m->dst == kA;
}

bool is_cmp_a_imm(const ir::Type_instruction& instr) {
  const auto* c = as<ir::Compare>(instr);
  return c != nullptr && c->left == kA && isa<ir::Imm>(c->right);
}

// Returns the byte of the immediate source of an `is_imm_to_a` move.
asdl::Int imm_src_byte(const ir::Type_instruction& instr) {
  return std::get<ir::Imm>(std::get<ir::Mov>(instr).src).value & 0xFF;
}

// Returns the byte of the immediate of an `is_cmp_a_imm` compare.
asdl::Int cmp_right_byte(const ir::Type_instruction& instr) {
  return std::get<ir::Imm>(std::get<ir::Compare>(instr).right).value & 0xFF;
}

// Replaces the instructions of each function for which `fn` returns new
// ones, and leaves the others as they are.
template <class Fn>
asm_ast::Program map_functions(asm_ast::Program prog, Fn&& fn) {
  for (auto& tl : prog.top_level) {
    if (auto* f = as<ir::Function>(tl)) {
      if (auto instrs = fn(f->instructions, *f)) {
        f->instructions = std::move(*instrs);
      }
    }
  }
  return prog;
}

// --- Tail copies ---

// Returns the `[Compare, Return]` carry-flag tail at `instrs[i]`,
// skipping labels. Only a bare `Return` that carries nothing but C
// qualifies, since copying a `Ret` would copy its frame teardown, and
// the fold needs A to be dead at the return.
std::optional<std::pair<std::size_t, std::size_t>> tail_after(const InstrList& instrs,
                                                              std::size_t i) {
  while (i < instrs.size() && isa<ir::Label>(instrs[i])) {
    ++i;
  }
  if (i + 1 >= instrs.size()) {
    return std::nullopt;
  }
  if (!is_cmp_a_imm(instrs[i])) {
    return std::nullopt;
  }
  const auto* ret = as<ir::Return>(instrs[i + 1]);
  if (ret == nullptr || !ret->save_c || ret->save_a || ret->save_x) {
    return std::nullopt;
  }
  return std::pair{i, i + 1};
}

std::optional<InstrList> copy_tails(const InstrList& instrs) {
  const LabelMap labels(instrs);
  InstrList out;
  bool changed = false;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto& instr = instrs[i];
    const bool prev_imm = !out.empty() && is_imm_to_a(out.back());
    if (prev_imm) {
      if (const auto* j = as<ir::Jump>(instr)) {
        const long at = labels.find(j->target);
        if (at >= 0) {
          if (const auto tail = tail_after(instrs, static_cast<std::size_t>(at))) {
            out.push_back(instrs[tail->first]);
            out.push_back(instrs[tail->second]);
            changed = true;
            continue;
          }
        }
      }
    }
    if (prev_imm && isa<ir::Label>(instr)) {
      if (const auto tail = tail_after(instrs, i)) {
        // The fall-through path gets its own copy. The label and the
        // tail after it stay for the other paths.
        out.push_back(instrs[tail->first]);
        out.push_back(instrs[tail->second]);
        changed = true;
      }
    }
    out.push_back(instr);
  }
  if (!changed) {
    return std::nullopt;
  }
  return out;
}

// --- Constant compare ---

// Returns the constant `reg` holds at `instrs[i]`, from an immediate
// load earlier in the block (through register transfers).
std::optional<asdl::Int> known_constant(const InstrList& instrs, std::size_t i,
                                        ir::Type_operand reg) {
  for (std::size_t k = i; k-- > 0;) {
    const auto& ins = instrs[k];
    if (isa<ir::Label, ir::Call, ir::Return, ir::Ret, ir::Jump, ir::FunctionPrologue,
            ir::AllocateStack, ir::Pop>(ins)) {
      return std::nullopt;
    }
    const auto& r = std::get<ir::Reg>(reg).reg;
    if (isa<ir::Y>(r) && sets_up_y(ins)) {
      return std::nullopt;
    }
    const ir::Type_operand* dst = std::visit(
        [](const auto& x) -> const ir::Type_operand* {
          using T = std::remove_cvref_t<decltype(x)>;
          if constexpr (requires(const T& t) {
                          { t.dst } -> std::same_as<const ir::Type_operand&>;
                        }) {
            return &x.dst;
          } else {
            return nullptr;
          }
        },
        ins);
    if (dst == nullptr || *dst != reg) {
      if ((isa<ir::X>(r) && kills_x(ins)) || (isa<ir::Y>(r) && kills_y(ins))) {
        return std::nullopt;
      }
      continue;
    }
    if (const auto* m = as<ir::Mov>(ins); m != nullptr && !m->is_volatile) {
      if (const auto* imm = as<ir::Imm>(m->src)) {
        return imm->value & 0xFF;
      }
      if (isa<ir::Reg>(m->src)) {
        reg = m->src;
        continue;
      }
    }
    return std::nullopt;
  }
  return std::nullopt;
}

ir::Type_instruction carry_setter(bool set) {
  if (set) {
    return ir::SetCarry{};
  }
  return ir::ClearCarry{};
}

std::optional<InstrList> fold_const_compares(const InstrList& instrs) {
  InstrList out = instrs;
  bool changed = false;
  std::size_t i = 0;
  while (i + 1 < out.size()) {
    if (is_imm_to_a(out[i]) && is_cmp_a_imm(out[i + 1]) && a_dead_at(out, i + 2) &&
        flags_dead_at(out, i + 2)) {
      const asdl::Int k = imm_src_byte(out[i]);
      const asdl::Int c = cmp_right_byte(out[i + 1]);
      out[i] = carry_setter(k >= c);
      out.erase(out.begin() + static_cast<std::ptrdiff_t>(i) + 1);
      changed = true;
      continue;
    }
    ++i;
  }
  for (std::size_t j = 0; j < out.size(); ++j) {
    const auto* cmp = as<ir::Compare>(out[j]);
    if (!(cmp != nullptr && isa<ir::Imm>(cmp->right) && isa<ir::Reg>(cmp->left) &&
          flags_dead_at(out, j + 1))) {
      continue;
    }
    const auto k = known_constant(out, j, cmp->left);
    if (!k) {
      continue;
    }
    const asdl::Int c = std::get<ir::Imm>(cmp->right).value & 0xFF;
    out[j] = carry_setter(*k >= c);
    changed = true;
  }
  if (!changed) {
    return std::nullopt;
  }
  return out;
}

// --- Caller-side capture ---

// Returns whether `instrs[i]` starts `LDA #0; ROL A`.
bool is_capture(const InstrList& instrs, std::size_t i) {
  if (i + 1 >= instrs.size()) {
    return false;
  }
  if (!is_imm_to_a(instrs[i]) || imm_src_byte(instrs[i]) != 0) {
    return false;
  }
  const auto* rol = as<ir::RotateLeft>(instrs[i + 1]);
  return rol != nullptr && rol->dst == kA;
}

// Returns whether `instr` can sit between a stored capture and its
// test: none of these reads or writes C. `Mov` covers loads, stores,
// and transfers, and a move from memory to memory goes through A, which
// is dead there.
bool leaves_c(const ir::Type_instruction& instr) {
  return isa<ir::Mov, ir::Inc, ir::Dec, ir::And, ir::Or, ir::Xor, ir::BitTest>(instr);
}

// Returns whether A, N/Z, and C are all dead at `instrs[idx]`.
bool dead_after_branch(const InstrList& instrs, std::size_t idx) {
  return a_dead_at(instrs, idx) && flags_dead_at(instrs, idx) && c_dead_at(instrs, idx);
}

// Returns the branch condition that tests C as `cond` tested the
// captured flag: BCC for BEQ, and BCS for BNE.
ir::Type_condition carry_condition(const ir::Type_condition& cond) {
  if (isa<ir::EQ>(cond)) {
    return ir::CC{};
  }
  return ir::CS{};
}

bool is_eq_or_ne_branch(const ir::Type_instruction& instr) {
  const auto* b = as<ir::Branch>(instr);
  return b != nullptr && isa<ir::EQ, ir::NE>(b->cond);
}

std::optional<InstrList> fold_captures(const InstrList& instrs) {
  InstrList out = instrs;
  bool changed = false;
  std::size_t i = 0;
  while (i + 2 < out.size()) {
    if (!is_capture(out, i)) {
      ++i;
      continue;
    }
    if (is_eq_or_ne_branch(out[i + 2])) {
      const auto br = std::get<ir::Branch>(out[i + 2]);
      const LabelMap labels(out);
      const long tgt = labels.find(br.target);
      if (tgt >= 0 && dead_after_branch(out, i + 3) &&
          dead_after_branch(out, static_cast<std::size_t>(tgt))) {
        out[i] = ir::Branch{.cond = carry_condition(br.cond), .target = br.target};
        const auto first = out.begin() + static_cast<std::ptrdiff_t>(i) + 1;
        out.erase(first, first + 2);
        changed = true;
        continue;
      }
    }
    if (is_cmp_a_imm(out[i + 2]) && cmp_right_byte(out[i + 2]) == 1 &&
        a_dead_at(out, i + 3) && flags_dead_at(out, i + 3)) {
      const auto first = out.begin() + static_cast<std::ptrdiff_t>(i);
      out.erase(first, first + 3);
      changed = true;
      continue;
    }
    ++i;
  }
  if (!changed) {
    return std::nullopt;
  }
  return out;
}

// Returns the operands of `instr` among its `src`, `dst`, `src1`, and
// `src2` fields.
std::vector<ir::Type_operand> operands(const ir::Type_instruction& instr) {
  std::vector<ir::Type_operand> out;
  std::visit(
      [&](const auto& x) {
        using T = std::remove_cvref_t<decltype(x)>;
        if constexpr (requires(const T& t) {
                        { t.src } -> std::same_as<const ir::Type_operand&>;
                      }) {
          out.push_back(x.src);
        }
        if constexpr (requires(const T& t) {
                        { t.dst } -> std::same_as<const ir::Type_operand&>;
                      }) {
          out.push_back(x.dst);
        }
        if constexpr (requires(const T& t) {
                        { t.src1 } -> std::same_as<const ir::Type_operand&>;
                      }) {
          out.push_back(x.src1);
        }
        if constexpr (requires(const T& t) {
                        { t.src2 } -> std::same_as<const ir::Type_operand&>;
                      }) {
          out.push_back(x.src2);
        }
      },
      instr);
  return out;
}

bool is_flag_reload(const ir::Type_instruction& instr, const ir::Type_operand& flag) {
  const auto* m = as<ir::Mov>(instr);
  return m != nullptr && m->src == flag && m->dst == kA && !m->is_volatile;
}

// Rewrites `LDA #0; ROL A; STA flag; ...; LDA flag; Bxx L`, with nothing
// in between touching C or `flag`, into `...; BCx L`.
std::optional<InstrList> fold_stored_captures(const InstrList& instrs, const std::string& fn_name,
                                           const AsmContext& ctx,
                                           const SymbolAddrs* zp_slot_symbols) {
  InstrList out = instrs;
  bool changed = false;
  std::optional<LabelMap> labels;
  std::size_t i = 0;
  while (i + 2 < out.size()) {
    if (!is_capture(out, i)) {
      ++i;
      continue;
    }
    const auto* store = as<ir::Mov>(out[i + 2]);
    if (!(store != nullptr && store->src == kA && isa<ir::Data, ir::ZP>(store->dst) &&
          !store->is_volatile)) {
      ++i;
      continue;
    }
    const ir::Type_operand flag = store->dst;
    std::size_t k = i + 3;
    while (k < out.size() && leaves_c(out[k]) && !is_flag_reload(out[k], flag)) {
      bool touches = false;
      for (const auto& op : operands(out[k])) {
        if (isa<ir::Data, ir::ZP, ir::IndexedData, ir::Indirect, ir::IndirectY,
                ir::IndirectZp, ir::IndirectZpY>(op) &&
            may_alias(op, flag, ctx)) {
          touches = true;
          break;
        }
      }
      if (touches) {
        break;
      }
      ++k;
    }
    if (!(k + 1 < out.size() && is_flag_reload(out[k], flag))) {
      ++i;
      continue;
    }
    // `LDA flag; CMP #1` turns the flag straight back into C, as in
    // `bool failed = f(); ...; return failed;`. C still holds it, so the
    // reload and the compare go too.
    if (is_cmp_a_imm(out[k + 1]) && cmp_right_byte(out[k + 1]) == 1 &&
        a_dead_at(out, i + 3) && flags_dead_at(out, i + 3) && a_dead_at(out, k + 2) &&
        flags_dead_at(out, k + 2) &&
        asm_dead_store::mem_dead_after(out, k, flag, ctx, fn_name, zp_slot_symbols)) {
      const auto at_k = out.begin() + static_cast<std::ptrdiff_t>(k);
      out.erase(at_k, at_k + 2);
      const auto at_i = out.begin() + static_cast<std::ptrdiff_t>(i);
      out.erase(at_i, at_i + 3);
      changed = true;
      labels.reset();
      continue;
    }
    if (!is_eq_or_ne_branch(out[k + 1])) {
      ++i;
      continue;
    }
    const auto br = std::get<ir::Branch>(out[k + 1]);
    if (!labels) {
      labels.emplace(out);
    }
    const long tgt = labels->find(br.target);
    if (!(tgt >= 0 && a_dead_at(out, i + 3) && flags_dead_at(out, i + 3) &&
          dead_after_branch(out, k + 2) &&
          dead_after_branch(out, static_cast<std::size_t>(tgt)) &&
          asm_dead_store::mem_dead_after(out, k, flag, ctx, fn_name, zp_slot_symbols))) {
      ++i;
      continue;
    }
    out[k] = ir::Branch{.cond = carry_condition(br.cond), .target = br.target};
    out.erase(out.begin() + static_cast<std::ptrdiff_t>(k) + 1);
    const auto at_i = out.begin() + static_cast<std::ptrdiff_t>(i);
    out.erase(at_i, at_i + 3);
    changed = true;
    labels.reset();
  }
  if (!changed) {
    return std::nullopt;
  }
  return out;
}

}  // namespace

asm_ast::Program apply_carry_return_tails(asm_ast::Program prog) {
  return map_functions(std::move(prog), [](const InstrList& instrs, const ir::Function&) {
    return copy_tails(instrs);
  });
}

asm_ast::Program apply_carry_const_fold(asm_ast::Program prog) {
  return map_functions(std::move(prog), [](const InstrList& instrs, const ir::Function&) {
    return fold_const_compares(instrs);
  });
}

asm_ast::Program apply_carry_branch_fold(asm_ast::Program prog, const AsmContext& ctx,
                                         const SymbolAddrs* zp_slot_symbols) {
  prog = map_functions(std::move(prog), [](const InstrList& instrs, const ir::Function&) {
    return fold_captures(instrs);
  });
  return map_functions(std::move(prog), [&](const InstrList& instrs, const ir::Function& fn) {
    return fold_stored_captures(instrs, fn.name, ctx, zp_slot_symbols);
  });
}

}  // namespace c6502::passes
