// Shifts a byte in A instead of in the memory it's stored to.
//
// The pattern
//
//     Mov(Reg(A), M)            // STA M
//     <op>(M)                   // ASL / LSR / ROL / ROR M, one or more
//     ...
//     [Mov(M, Reg(A))]          // LDA M, optional
//
// becomes
//
//     <op>(Reg(A))              // ASL A ... (each shift on A)
//     ...
//     Mov(Reg(A), M)            // STA M
//
// A held M's value at the `STA`, so the shifts give the same result in
// A, and the final `STA` leaves M as it was. The flags come out the
// same. Each `ASL A` sets C, N, and Z as `ASL M` did, and `STA` changes
// none of them. A `ROL` / `ROR` reads the same C either way. The
// trailing `LDA M` set N and Z from M's final value, which the last
// shift already set them to.
//
// When it applies
//
// - `M` is a `ZP` or `Data` byte, and neither the `STA` nor the `LDA`
//   is volatile.
// - The shifts are of `M`, one right after another, right after the
//   `STA`.
// - After them comes `LDA M`, which the rewrite deletes, since A now
//   holds M. Without it, A has to be dead after the shifts, since the
//   rewrite leaves the shifted value in A where the unshifted one was.
//
// The run may include shifts of other bytes, which leave A alone and
// stay where they are. Each still reads the carry that the shift of M
// before it set, now from A. A constant store right after the `STA`,
// which would lose A, moves up in front of the code that loaded A (a
// load into A and then operations on A alone that don't touch the
// stored byte).
//
// An increment or decrement
//
// `STA M; INC M; LDA M` becomes `CLC; ADC #1; STA M`, and `DEC M`
// becomes `SEC; SBC #1` (`rewrite_steps`). Both set N and Z from the
// new value, but `ADC` and `SBC` also set C and V, which `INC` and
// `DEC` leave alone, so both have to be dead after the load.
//
// The converse: a shift loaded from and stored back to the same byte
//
//     Mov(M, Reg(A))            // LDA M
//     <op>(Reg(A))              // one or two shifts of A
//     Mov(Reg(A), M)            // STA M
//
// becomes the shifts of M, `ASL M` (`shift_in_place`), when A is dead
// after the store. M may also be an array element indexed by X, since
// X can't change between the adjacent load and store. A volatile byte
// is left alone, since a read-modify-write instruction on the NMOS 6502
// writes the old value back before the new one.
//
// A loop of one-bit shifts
//
// `STA m`, a count put in X, a loop of `ASL m; DEX; BNE`, and `LDA m`
// after it become the loop on A and `STA m` after it
// (`shift_loop_in_a`), when the flags are dead after the load, since N
// and Z then come from `DEX`.
#include "passes/shift_in_a.h"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/inc_peephole.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrList = std::vector<ir::Type_instruction>;

const ir::Type_operand kRegA = ir::Reg{.reg = ir::A{}};

template <class T>
inline constexpr bool is_shift_v =
    std::is_same_v<T, ir::ArithmeticShiftLeft> ||
    std::is_same_v<T, ir::LogicalShiftRight> || std::is_same_v<T, ir::RotateLeft> ||
    std::is_same_v<T, ir::RotateRight>;

const ir::Type_operand* shift_dst(const ir::Type_instruction& instr) {
  return std::visit(
      [](const auto& ins) -> const ir::Type_operand* {
        if constexpr (is_shift_v<std::decay_t<decltype(ins)>>) {
          return &ins.dst;
        } else {
          return nullptr;
        }
      },
      instr);
}

ir::Type_instruction with_dst(const ir::Type_instruction& instr,
                              const ir::Type_operand& dst) {
  ir::Type_instruction out = instr;
  std::visit(
      [&](auto& ins) {
        if constexpr (is_shift_v<std::decay_t<decltype(ins)>>) {
          ins.dst = dst;
        }
      },
      out);
  return out;
}

bool is_reg_a_op(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

bool is_reg_x_op(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::X>(r->reg);
}

bool is_mem(const ir::Type_operand& op) { return isa<ir::ZP, ir::Data>(op); }

bool is_imm(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(op);
}

bool distinct(const ir::Type_operand& a, const ir::Type_operand& b) {
  return a.index() == b.index() && a != b;
}

InstrList constant_stores(const InstrList& instrs, std::size_t i,
                          const ir::Type_operand& mem) {
  InstrList out;
  std::size_t j = i + 1;
  while (j < instrs.size()) {
    const auto* ins = as<ir::Mov>(instrs[j]);
    if (ins != nullptr && is_imm(ins->src) && !ins->is_volatile &&
        is_mem(ins->dst) && distinct(ins->dst, mem)) {
      out.push_back(instrs[j]);
      ++j;
      continue;
    }
    if (j + 1 >= instrs.size()) {
      break;
    }
    const auto* store = as<ir::Mov>(instrs[j + 1]);
    if (!(ins != nullptr && is_reg_a_op(ins->dst) && is_imm(ins->src) &&
          store != nullptr && is_reg_a_op(store->src) && !store->is_volatile &&
          is_mem(store->dst) && distinct(store->dst, mem))) {
      break;
    }
    out.push_back(instrs[j]);
    out.push_back(instrs[j + 1]);
    j += 2;
  }
  return out;
}

bool safe_read(const ir::Type_operand& src, const std::vector<ir::Type_operand>& avoid) {
  if (isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Reg>(src)) {
    return true;
  }
  if (is_mem(src)) {
    for (const auto& h : avoid) {
      if (!distinct(src, h)) {
        return false;
      }
    }
    return true;
  }
  return false;
}

const ir::Type_operand* a_op_src(const ir::Type_instruction& instr, bool& is_a_op) {
  is_a_op = false;
  if (const ir::Type_operand* dst = shift_dst(instr)) {
    is_a_op = is_reg_a_op(*dst);
    return nullptr;
  }
  return std::visit(
      [&](const auto& ins) -> const ir::Type_operand* {
        using T = std::decay_t<decltype(ins)>;
        if constexpr (std::is_same_v<T, ir::Add> || std::is_same_v<T, ir::Sub> ||
                      std::is_same_v<T, ir::And> || std::is_same_v<T, ir::Or>) {
          is_a_op = is_reg_a_op(ins.dst);
          return &ins.src;
        } else {
          return nullptr;
        }
      },
      instr);
}

std::optional<std::size_t> a_chain_start(const InstrList& instrs, std::size_t i,
                                         const std::vector<ir::Type_operand>& avoid) {
  for (std::size_t k = i; k-- > 0;) {
    const ir::Type_instruction& ins = instrs[k];
    if (const auto* m = as<ir::Mov>(ins); m != nullptr && is_reg_a_op(m->dst)) {
      if (m->is_volatile || !safe_read(m->src, avoid)) {
        return std::nullopt;
      }
      return k;
    }
    bool is_a_op = false;
    const ir::Type_operand* src = a_op_src(ins, is_a_op);
    if (is_a_op) {
      if (src != nullptr && !safe_read(*src, avoid)) {
        return std::nullopt;
      }
      continue;
    }
    return std::nullopt;
  }
  return std::nullopt;
}

bool rewrite_function(InstrList& instrs) {
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const auto* store_mov = as<ir::Mov>(instrs[i]);
    if (!(store_mov != nullptr && !store_mov->is_volatile &&
          is_reg_a_op(store_mov->src) && is_mem(store_mov->dst))) {
      ++i;
      continue;
    }
    const ir::Type_instruction store = instrs[i];
    const ir::Type_operand mem = store_mov->dst;
    const InstrList pairs = constant_stores(instrs, i, mem);
    const std::size_t first = i + 1 + pairs.size();
    std::size_t j = first;
    int own = 0;
    while (j < instrs.size()) {
      const ir::Type_operand* dst = shift_dst(instrs[j]);
      if (dst == nullptr) {
        break;
      }
      if (*dst == mem) {
        ++own;
      } else if (!(is_mem(*dst) && distinct(*dst, mem))) {
        break;
      }
      ++j;
    }
    while (j > first && *shift_dst(instrs[j - 1]) != mem) {
      --j;
    }
    if (own == 0) {
      ++i;
      continue;
    }
    std::optional<std::size_t> start;
    if (!pairs.empty()) {
      std::vector<ir::Type_operand> avoid;
      for (const auto& p : pairs) {
        const auto& dst = std::get<ir::Mov>(p).dst;
        if (is_mem(dst)) {
          avoid.push_back(dst);
        }
      }
      start = a_chain_start(instrs, i, avoid);
      if (!start) {
        ++i;
        continue;
      }
    }
    bool reloads = false;
    if (j < instrs.size()) {
      const auto* load = as<ir::Mov>(instrs[j]);
      reloads = load != nullptr && !load->is_volatile && load->src == mem &&
                is_reg_a_op(load->dst);
    }
    if (!reloads && !a_dead_at(instrs, j)) {
      ++i;
      continue;
    }
    InstrList replacement;
    for (std::size_t k = first; k < j; ++k) {
      if (*shift_dst(instrs[k]) == mem) {
        replacement.push_back(with_dst(instrs[k], kRegA));
      } else {
        replacement.push_back(instrs[k]);
      }
    }
    const std::size_t n_shifts = replacement.size();
    replacement.push_back(store);
    const std::size_t end = reloads ? j + 1 : j;
    instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(i),
                 instrs.begin() + static_cast<std::ptrdiff_t>(end));
    instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(i), replacement.begin(),
                  replacement.end());
    if (!pairs.empty()) {
      instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(*start), pairs.begin(),
                    pairs.end());
      i += pairs.size();
    }
    changed = true;
    i += n_shifts + 1;
  }
  return changed;
}

bool shift_in_place(InstrList& instrs) {
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    const auto* load = as<ir::Mov>(instrs[i]);
    if (!(load != nullptr && is_reg_a_op(load->dst) && !load->is_volatile &&
          (is_mem(load->src) || is_x_indexed(load->src)))) {
      ++i;
      continue;
    }
    std::size_t k = i + 1;
    while (k < instrs.size() && k - i <= 2) {
      const ir::Type_operand* dst = shift_dst(instrs[k]);
      if (dst == nullptr || !is_reg_a_op(*dst)) {
        break;
      }
      ++k;
    }
    const ir::Mov* store = k < instrs.size() ? as<ir::Mov>(instrs[k]) : nullptr;
    if (!(k > i + 1 && store != nullptr && is_reg_a_op(store->src) &&
          !store->is_volatile && store->dst == load->src &&
          a_dead_at(instrs, k + 1))) {
      ++i;
      continue;
    }
    const ir::Type_operand target = load->src;
    InstrList shifts;
    for (std::size_t s = i + 1; s < k; ++s) {
      shifts.push_back(with_dst(instrs[s], target));
    }
    instrs.erase(instrs.begin() + static_cast<std::ptrdiff_t>(i),
                 instrs.begin() + static_cast<std::ptrdiff_t>(k + 1));
    instrs.insert(instrs.begin() + static_cast<std::ptrdiff_t>(i), shifts.begin(),
                  shifts.end());
    changed = true;
    ++i;
  }
  return changed;
}

bool rewrite_steps(InstrList& instrs) {
  bool changed = false;
  std::size_t i = 0;
  while (i + 2 < instrs.size()) {
    const auto* store = as<ir::Mov>(instrs[i]);
    const auto* load = as<ir::Mov>(instrs[i + 2]);
    const ir::Type_instruction& step = instrs[i + 1];
    const ir::Type_operand* step_dst = nullptr;
    if (const auto* inc = as<ir::Inc>(step)) {
      step_dst = &inc->dst;
    } else if (const auto* dec = as<ir::Dec>(step)) {
      step_dst = &dec->dst;
    }
    if (store != nullptr && is_reg_a_op(store->src) && !store->is_volatile &&
        is_mem(store->dst) && step_dst != nullptr && *step_dst == store->dst &&
        load != nullptr && is_reg_a_op(load->dst) && !load->is_volatile &&
        load->src == store->dst && c_dead_at(instrs, i + 3) &&
        v_dead_at(instrs, i + 3)) {
      const ir::Type_instruction kept = instrs[i];
      const ir::Type_operand one = ir::Imm{.value = 1};
      if (isa<ir::Inc>(step)) {
        instrs[i] = ir::ClearCarry{};
        instrs[i + 1] = ir::Add{.src = one, .dst = kRegA};
      } else {
        instrs[i] = ir::SetCarry{};
        instrs[i + 1] = ir::Sub{.src = one, .dst = kRegA};
      }
      instrs[i + 2] = kept;
      changed = true;
      i += 3;
      continue;
    }
    ++i;
  }
  return changed;
}

std::map<std::string, int> labels_named(const InstrList& instrs) {
  std::map<std::string, int> counts;
  for (const auto& ins : instrs) {
    if (const auto* b = as<ir::Branch>(ins); b != nullptr && !b->target.empty()) {
      ++counts[b->target];
    } else if (const auto* j = as<ir::Jump>(ins); j != nullptr && !j->target.empty()) {
      ++counts[j->target];
    }
  }
  return counts;
}

int named_count(const std::map<std::string, int>& named, const std::string& name) {
  const auto it = named.find(name);
  return it == named.end() ? 0 : it->second;
}

bool shift_loop_in_a(InstrList& instrs) {
  const std::map<std::string, int> named = labels_named(instrs);
  bool changed = false;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto* store = as<ir::Mov>(instrs[i]);
    if (!(store != nullptr && is_reg_a_op(store->src) && is_mem(store->dst) &&
          !store->is_volatile)) {
      continue;
    }
    const ir::Type_operand m = store->dst;
    std::size_t j = i + 1;
    std::optional<std::string> done;
    while (j < instrs.size()) {
      const ir::Type_instruction& ins = instrs[j];
      if (const auto* mv = as<ir::Mov>(ins);
          mv != nullptr && is_reg_x_op(mv->dst) &&
          (is_imm(mv->src) || (is_mem(mv->src) && mv->src != m))) {
        ++j;
      } else if (const auto* c = as<ir::Compare>(ins);
                 c != nullptr && is_reg_x_op(c->left) && is_imm(c->right)) {
        ++j;
      } else if (const auto* b = as<ir::Branch>(ins);
                 b != nullptr && isa<ir::EQ>(b->cond) && !done) {
        done = b->target;
        ++j;
      } else {
        break;
      }
    }
    if (j + 3 >= instrs.size() || j == i + 1) {
      continue;
    }
    const auto* label = as<ir::Label>(instrs[j]);
    const ir::Type_operand* shift = shift_dst(instrs[j + 1]);
    const auto* dec = as<ir::Dec>(instrs[j + 2]);
    const auto* back = as<ir::Branch>(instrs[j + 3]);
    if (!(label != nullptr && shift != nullptr && *shift == m && dec != nullptr &&
          is_reg_x_op(dec->dst) && back != nullptr && isa<ir::NE>(back->cond) &&
          back->target == label->name && named_count(named, label->name) == 1)) {
      continue;
    }
    std::size_t k = j + 4;
    if (done) {
      const ir::Label* done_label =
          k < instrs.size() ? as<ir::Label>(instrs[k]) : nullptr;
      if (!(done_label != nullptr && done_label->name == *done &&
            named_count(named, *done) == 1)) {
        continue;
      }
      ++k;
    }
    if (k >= instrs.size()) {
      continue;
    }
    const auto* load = as<ir::Mov>(instrs[k]);
    if (!(load != nullptr && is_reg_a_op(load->dst) && load->src == m &&
          !load->is_volatile && flags_dead_at(instrs, k + 1))) {
      continue;
    }
    instrs[j + 1] = with_dst(instrs[j + 1], kRegA);
    instrs[k] = ir::Mov{.src = kRegA, .dst = m, .is_volatile = false};
    changed = true;
  }
  return changed;
}

}  // namespace

asm_ast::Program apply_shift_in_a(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(fn->instructions);
      rewrite_steps(fn->instructions);
      shift_in_place(fn->instructions);
      shift_loop_in_a(fn->instructions);
    }
  }
  return prog;
}

}  // namespace c6502::passes
