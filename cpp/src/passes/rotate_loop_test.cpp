// Moves a loop's test from its top to its bottom.
//
// Motivating shape
//
// A `for` or `while` loop tests at the top and ends with a `JMP` back
// to the test:
//
//     L:      <test>          ; ends in Bcc EXIT
//     B:      <body>
//             JMP L
//     EXIT:
//
// Each pass runs the `JMP` (3 cycles) and the test's branch not taken
// (2 cycles). With the test at the bottom, the loop is entered with a
// `JMP` to it, and the test's branch, inverted, goes back to the body:
//
//             JMP L           ; only when L is reached by falling in
//     B:      <body>
//     L:      <test>          ; ends in B!cc B
//     EXIT:
//
// The code is the same size, and each pass runs the branch taken (3
// cycles) and no `JMP`, 2 cycles fewer. Entering costs the `JMP`, 3
// cycles, and leaving saves 1, since the test's branch falls through to
// EXIT where it used to be taken. A loop that runs `n` times saves
// `2n - 2` cycles.
//
// When the test is known to pass the first time, the entry `JMP` goes
// too, 3 bytes and 3 cycles, and the loop is entered at the body. That
// is the case of `for (uint8_t i = 0; i != 6; i++)`, whose test is
// `CPX #$06; BEQ` right after `LDX #$00`. `entry_passes` evaluates the
// test on the constants that the straight-line code in front of the
// loop leaves in the registers and in memory, and `test_effects_dead`
// checks that the body doesn't read a register or flag the test sets,
// and that the test writes no memory and reads no volatile byte, since
// the first pass then doesn't run the test.
//
// Eligibility
//
// - `JMP L` is followed, after any labels, by the label EXIT, and `L`
//   is a label earlier in the function.
// - The instructions from `L` up to the first branch, the test, contain
//   no label, jump, or return, and that branch is a conditional one to
//   EXIT.
// - The backward branch from the end of the test to the body fits in
//   the 8-bit range, with a margin, since `expand_long_branches` would
//   otherwise turn it into a branch around a `JMP`, 3 bytes more.
//
// Other jumps and branches to `L`, such as a `continue`, still go to
// `L`, which moves with the test.
//
// Where to run
//
// After the peephole fixed points and `loop_invariant_a`, which expects
// the loop's test at its top, and before `expand_long_branches`. Only
// under `-O`.
#include "passes/rotate_loop_test.h"

#include <cstddef>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"
#include "passes/long_branches.h"
#include "sim/assembler.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using InstrVec = std::vector<ir::Type_instruction>;
using Span = std::span<const ir::Type_instruction>;

// The longest body and test, in bytes, that the backward branch is
// expected to reach. The 6502's branch reaches 128 bytes back, and the
// margin covers long branches that `expand_long_branches` may add
// inside the body.
constexpr int kMaxReach = 112;

// Returns the names of the labels in the run of labels at `instrs[k:]`.
std::set<std::string, std::less<>> labels_from(const InstrVec& instrs, std::size_t k) {
  std::set<std::string, std::less<>> out;
  while (k < instrs.size()) {
    const auto* label = as<ir::Label>(instrs[k]);
    if (label == nullptr) {
      break;
    }
    out.insert(label->name);
    ++k;
  }
  return out;
}

// Returns the index of the conditional branch that ends the test
// starting after the label at `l`, when the test has no label, jump, or
// return before it and ends before `j`.
std::optional<std::size_t> test_end(const InstrVec& instrs, std::size_t l,
                                    std::size_t j) {
  for (std::size_t k = l + 1; k < j; ++k) {
    const auto& ins = instrs[k];
    if (isa<ir::Branch>(ins)) {
      return k;
    }
    if (isa<ir::Label, ir::Jump, ir::Return, ir::Ret, ir::FunctionPrologue,
            ir::AllocateStack>(ins)) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

int size_of(const ir::Type_instruction& ins) {
  return isa<ir::Label>(ins) ? 0 : sim::instruction_size(ins);
}

// Whether the instruction struct `T` has a `dst` field that holds one
// operand.
template <class T>
constexpr bool kHasOperandDst = requires(const T& t) {
  requires std::is_same_v<std::remove_cvref_t<decltype(t.dst)>, ir::Type_operand>;
};

const ir::Type_operand* dst_operand(const ir::Type_instruction& ins) {
  return std::visit(
      [](const auto& i) -> const ir::Type_operand* {
        if constexpr (kHasOperandDst<std::remove_cvref_t<decltype(i)>>) {
          return &i.dst;
        } else {
          return nullptr;
        }
      },
      ins);
}

// Returns the register `ins` names as its single `dst` operand, as
// Python's `getattr(ins, "dst", None)` finds it, or null.
const ir::Type_reg* dst_reg(const ir::Type_instruction& ins) {
  const ir::Type_operand* dst = dst_operand(ins);
  if (dst == nullptr) {
    return nullptr;
  }
  const auto* r = as<ir::Reg>(*dst);
  return r != nullptr ? &r->reg : nullptr;
}

// Returns whether `ins` does more than set registers and flags. That's
// a write to memory (a store, or an `INC`, `DEC`, shift, or rotate of a
// byte), a volatile access, or anything other than a move, compare,
// `BIT`, arithmetic, shift, step, carry setting, or branch.
bool has_side_effect(const ir::Type_instruction& ins) {
  if (const auto* mov = as<ir::Mov>(ins)) {
    return mov->is_volatile || !isa<ir::Reg>(mov->dst);
  }
  if (isa<ir::Inc, ir::Dec, ir::ArithmeticShiftLeft, ir::LogicalShiftRight,
          ir::RotateLeft, ir::RotateRight, ir::Add, ir::Sub, ir::And, ir::Or,
          ir::Xor>(ins)) {
    return !isa<ir::Reg>(*dst_operand(ins));
  }
  return !isa<ir::Compare, ir::BitTest, ir::SetCarry, ir::ClearCarry, ir::Branch>(ins);
}

// Returns whether nothing from `body` on reads a register or a flag
// that `test` writes before writing it again, so the first pass may
// skip the test. The body otherwise runs after the test, and a pass
// before this one may have relied on what the test left: after `LDA i;
// CMP #1; BCS exit`, C is clear in the body, and `carry_known` dropped
// the `CLC` of the body's `gs5.m0++`. Entered at the body, the add then
// added whatever C held.
//
// A test that writes memory or reads a volatile byte gives false, since
// the first pass would then skip that write or read too. The test of
// `for (uint8_t i = 0x1F; i-- != 0;)` is `DEC i; LDA i; CMP #$FF; BEQ
// exit`, and entered at the body, the loop indexed with 0x1F on its
// first pass and ran 32 times.
bool test_effects_dead(const InstrVec& instrs, const LabelMap& labels, Span test,
                       std::size_t body) {
  for (const auto& ins : test) {
    if (has_side_effect(ins)) {
      return false;
    }
  }
  bool a = false;
  bool x = false;
  bool y = false;
  for (const auto& ins : test) {
    if (const auto* r = dst_reg(ins)) {
      a = a || isa<ir::A>(*r);
      x = x || isa<ir::X>(*r);
      y = y || isa<ir::Y>(*r);
    }
    if (sets_up_y(ins)) {
      y = true;
    }
  }
  if ((a && !a_dead_at(instrs, labels, body)) ||
      (x && !x_dead_at(instrs, labels, body)) ||
      (y && !y_dead_at(instrs, labels, body))) {
    return false;
  }
  // Each flag on its own: `all_flags_dead_at` takes any branch to read
  // them all, and the `BNE` at the bottom of `clear_page1`'s loop reads
  // only Z, which the body's `DEX` writes first.
  return flags_dead_at(instrs, labels, body) && c_dead_at(instrs, labels, body) &&
         v_dead_at(instrs, labels, body);
}

// The index of a register in `State::regs`, or -1 for an operand that
// isn't a register.
int reg_index(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  if (r == nullptr) {
    return -1;
  }
  return static_cast<int>(ir::kind(r->reg));
}

bool is_memory(const ir::Type_operand& op) { return isa<ir::Data, ir::ZP>(op); }

// The known byte values of A, X, Y, and memory cells, and the N, Z, and
// C flags, through straight-line code. Nothing is unknown.
class State {
 public:
  explicit State(const AsmContext& ctx) : ctx_(ctx) {}

  void step(const ir::Type_instruction& ins) {
    if (sets_up_y(ins)) {
      regs_[static_cast<int>(ir::Kind_reg::Y)].reset();
    }
    switch (ir::kind(ins)) {
      case ir::Kind_instruction::Mov: {
        const auto& mov = std::get<ir::Mov>(ins);
        if (mov.is_volatile) {
          forget();
          return;
        }
        const auto v = value(mov.src);
        const int src_reg = reg_index(mov.src);
        const int dst_reg = reg_index(mov.dst);
        if (src_reg < 0 && dst_reg < 0) {
          // Memory to memory is LDA src; STA dst.
          regs_[static_cast<int>(ir::Kind_reg::A)] = v;
        }
        write(mov.dst, v);
        // A load or a transfer sets N and Z. A store doesn't.
        if (!(src_reg >= 0 && dst_reg < 0)) {
          set_nz(v);
        }
        return;
      }
      case ir::Kind_instruction::Compare: {
        const auto& cmp = std::get<ir::Compare>(ins);
        const auto a = value(cmp.left);
        const auto b = value(cmp.right);
        if (!a || !b) {
          n_.reset();
          z_.reset();
          c_.reset();
        } else {
          c_ = *a >= *b;
          z_ = *a == *b;
          n_ = ((*a - *b) & 0x80) != 0;
        }
        return;
      }
      case ir::Kind_instruction::Inc:
      case ir::Kind_instruction::Dec: {
        const bool inc = isa<ir::Inc>(ins);
        const auto& dst = inc ? std::get<ir::Inc>(ins).dst : std::get<ir::Dec>(ins).dst;
        auto v = value(dst);
        if (v) {
          v = (*v + (inc ? 1 : -1)) & 0xFF;
        }
        write(dst, v);
        set_nz(v);
        return;
      }
      case ir::Kind_instruction::SetCarry:
        c_ = true;
        return;
      case ir::Kind_instruction::ClearCarry:
        c_ = false;
        return;
      case ir::Kind_instruction::Label:
        return;
      default:
        // Anything else: forget what it may have changed.
        forget();
        return;
    }
  }

  std::optional<bool> branch_taken(const ir::Type_condition& cond) const {
    std::optional<bool> flag;
    bool want = true;
    switch (ir::kind(cond)) {
      case ir::Kind_condition::EQ:
        flag = z_;
        break;
      case ir::Kind_condition::NE:
        flag = z_;
        want = false;
        break;
      case ir::Kind_condition::CS:
        flag = c_;
        break;
      case ir::Kind_condition::CC:
        flag = c_;
        want = false;
        break;
      case ir::Kind_condition::MI:
        flag = n_;
        break;
      case ir::Kind_condition::PL:
        flag = n_;
        want = false;
        break;
      default:
        return std::nullopt;
    }
    if (!flag) {
      return std::nullopt;
    }
    return *flag == want;
  }

 private:
  std::optional<int> value(const ir::Type_operand& op) const {
    if (const auto* imm = as<ir::Imm>(op)) {
      return static_cast<int>(asdl::floor_mod(imm->value, 256));
    }
    const int r = reg_index(op);
    if (r >= 0) {
      return regs_[r];
    }
    if (is_memory(op)) {
      for (const auto& [cell, v] : mem_) {
        if (cell == op) {
          return v;
        }
      }
    }
    return std::nullopt;
  }

  void set_nz(std::optional<int> v) {
    if (!v) {
      n_.reset();
      z_.reset();
      return;
    }
    n_ = (*v & 0x80) != 0;
    z_ = *v == 0;
  }

  void write(const ir::Type_operand& op, std::optional<int> v) {
    const int r = reg_index(op);
    if (r >= 0) {
      regs_[r] = v;
      return;
    }
    if (is_memory(op)) {
      std::vector<std::pair<ir::Type_operand, int>> kept;
      for (auto& entry : mem_) {
        if (!may_alias(entry.first, op, ctx_)) {
          kept.push_back(std::move(entry));
        }
      }
      mem_ = std::move(kept);
      if (v) {
        mem_.emplace_back(op, *v);
      }
      return;
    }
    // A write through a pointer or an index may change any cell.
    mem_.clear();
  }

  void forget() {
    for (auto& r : regs_) {
      r.reset();
    }
    mem_.clear();
    n_.reset();
    z_.reset();
    c_.reset();
  }

  const AsmContext& ctx_;
  // Indexed by `Kind_reg`: A, X, Y.
  std::optional<int> regs_[3];
  std::vector<std::pair<ir::Type_operand, int>> mem_;
  std::optional<bool> n_;
  std::optional<bool> z_;
  std::optional<bool> c_;
};

// Returns whether the straight-line code that falls into the label at
// `l` leaves values for which `test`'s branch isn't taken, so the first
// pass of the loop needs no test. Unknown values, and any instruction
// the evaluation doesn't model, give false.
bool entry_passes(const InstrVec& instrs, std::size_t l, Span test,
                  const AsmContext& ctx) {
  std::size_t start = l;
  while (start > 0 && !isa<ir::Label, ir::Jump, ir::Branch, ir::Return, ir::Ret>(
                          instrs[start - 1])) {
    --start;
  }
  State state(ctx);
  for (std::size_t k = start; k < l; ++k) {
    state.step(instrs[k]);
  }
  for (std::size_t k = 0; k + 1 < test.size(); ++k) {
    state.step(test[k]);
  }
  const auto taken = state.branch_taken(std::get<ir::Branch>(test.back()).cond);
  return taken.has_value() && !*taken;
}

// Returns `instrs` with the first eligible loop rotated, or nothing
// when no loop is eligible.
std::optional<InstrVec> rotate_one(const InstrVec& instrs, const std::string& fn_name,
                                   int& counter, const AsmContext& ctx) {
  const LabelMap labels(instrs);
  for (std::size_t j = 0; j < instrs.size(); ++j) {
    const auto* jump = as<ir::Jump>(instrs[j]);
    if (jump == nullptr || !jump->reg_args.empty()) {
      continue;
    }
    const long found = labels.find(jump->target);
    if (found < 0 || static_cast<std::size_t>(found) >= j) {
      continue;
    }
    const auto l = static_cast<std::size_t>(found);
    const auto after = labels_from(instrs, j + 1);
    const auto end = test_end(instrs, l, j);
    if (!end) {
      continue;
    }
    const auto& branch = std::get<ir::Branch>(instrs[*end]);
    if (!after.contains(branch.target)) {
      continue;
    }
    const Span test(instrs.data() + l + 1, *end - l);
    const Span body(instrs.data() + *end + 1, j - *end - 1);
    int size = 0;
    for (const auto& ins : test) {
      size += size_of(ins);
    }
    for (const auto& ins : body) {
      size += size_of(ins);
    }
    if (size > kMaxReach) {
      continue;
    }
    // The body's first label, or a new one.
    std::string body_label;
    InstrVec head;
    if (!body.empty() && isa<ir::Label>(body.front())) {
      body_label = std::get<ir::Label>(body.front()).name;
    } else {
      body_label = "." + fn_name + "@rot@" + std::to_string(counter);
      ++counter;
      head.push_back(ir::Label{.name = body_label});
    }
    // A label at the function's start is where it's entered.
    const bool falls_in =
        l == 0 || !isa<ir::Jump, ir::Return, ir::Ret>(instrs[l - 1]);
    InstrVec entry;
    if (falls_in && !(entry_passes(instrs, l, test, ctx) &&
                      test_effects_dead(instrs, labels, test, *end + 1))) {
      entry.push_back(ir::Jump{.target = jump->target});
    }
    InstrVec out;
    out.reserve(instrs.size() + 2);
    out.insert(out.end(), instrs.begin(), instrs.begin() + static_cast<std::ptrdiff_t>(l));
    out.insert(out.end(), entry.begin(), entry.end());
    out.insert(out.end(), head.begin(), head.end());
    out.insert(out.end(), body.begin(), body.end());
    out.push_back(instrs[l]);
    out.insert(out.end(), test.begin(), test.end() - 1);
    out.push_back(ir::Branch{.cond = long_branches::invert(branch.cond), .target = body_label});
    out.insert(out.end(), instrs.begin() + static_cast<std::ptrdiff_t>(j + 1),
               instrs.end());
    // The loop's label stays where the test went, for the other jumps
    // to it, and goes when nothing jumps to it.
    const std::string target = jump->target;
    bool referenced = false;
    for (const auto& i : out) {
      if (const auto* jj = as<ir::Jump>(i); jj != nullptr && jj->target == target) {
        referenced = true;
        break;
      }
      if (const auto* bb = as<ir::Branch>(i); bb != nullptr && bb->target == target) {
        referenced = true;
        break;
      }
    }
    if (!referenced) {
      InstrVec kept;
      kept.reserve(out.size());
      for (auto& i : out) {
        const auto* label = as<ir::Label>(i);
        if (label != nullptr && label->name == target) {
          continue;
        }
        kept.push_back(std::move(i));
      }
      out = std::move(kept);
    }
    return out;
  }
  return std::nullopt;
}

}  // namespace

asm_ast::Program apply_rotate_loop_test(asm_ast::Program prog, const AsmContext& ctx) {
  int counter = 0;
  for (auto& tl : prog.top_level) {
    auto* fn = as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    while (auto rotated = rotate_one(fn->instructions, fn->name, counter, ctx)) {
      fn->instructions = std::move(*rotated);
    }
  }
  return prog;
}

}  // namespace c6502::passes
