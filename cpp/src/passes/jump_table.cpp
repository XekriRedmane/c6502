// Jump tables: a chain of compares of A with constants, each followed
// by a branch to its case, becomes a dispatch through a table of the
// cases' addresses.
//
// Pattern
//
// A `switch` on a byte becomes a chain like this one, from Zork's
// `do_2op` (examples/zork/decode.c), with 24 cases:
//
//     CMP   #$01
//     BEQ   .case@35
//     CMP   #$02
//     BEQ   .case@36
//     ...
//     CMP   #$18
//     BEQ   .case@58
//     JMP   .default@59
//
// Most of the cases are too far for a branch, so each `BEQ` grows into
// `BNE; JMP`, 7 bytes a case. The chain becomes
//
//     CMP   #$19
//     BCS   .default@59
//     ASL
//     TAY
//     LDA   .jtab@0+1,Y
//     PHA
//     LDA   .jtab@0,Y
//     PHA
//     RTS
// .jtab@0:
//     DC.W  .default@59-1      ; 0
//     DC.W  .case@35-1         ; 1
//     ...
//
// `RTS` pulls the address the two `PHA`s pushed and adds one, so it
// goes to the case. That's 15 bytes and 2 a case, and the same 32
// cycles or so for every case, where the chain took 5 for each compare
// it passed. Zork's own code dispatches through tables the same way.
//
// Rules
//
// - At least four compares of A with constants from 0 to 127, each
//   right after the one before and followed by a `BEQ` to its case,
//   with no label in the chain, and no constant twice. The case for 0
//   may be a `BEQ` right after the load of A, or after an `ORA #$00`
//   that tests A, with no compare. The `ORA` goes with the chain.
// - Then a `JMP` to the default, or code that is the default, which
//   gets a label (`.jtdef@N`).
// - At every case and the default, A, N, Z, and C are dead, since the
//   dispatch leaves A and the flags as the table's load set them, and
//   so is Y, the index, or else X, which takes its place.
// - The table covers the values from the smallest constant to the
//   largest, a value no compare names going to the default. When the
//   smallest is 2 or less, it starts at 0, which costs at most two
//   entries and saves the `CMP; BCC` of a lower bound.
// - The table is shorter than the chain, counting each compare and
//   branch as 2 bytes, as they are when the branch reaches its case.
//
// Where it runs
//
// Once, at the end of the `-O` asm pipeline, after `carry_known` and
// before `expand_long_branches`. Every pass that follows control flow
// has run by then, so none of them has to know the table.
// `expand_long_branches` sizes it through `instruction_size`.
#include "passes/jump_table.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/label_numbers.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

// The fewest compares in a chain that are worth a table.
constexpr std::size_t kMinCases = 4;
// The dispatch, `ASL; TAY; LDA t+1,Y; PHA; LDA t,Y; PHA; RTS`, in bytes.
constexpr long kDispatch = 11;

bool is_a(const ir::Type_operand& op) {
  const auto* reg = as<ir::Reg>(op);
  return reg != nullptr && isa<ir::A>(reg->reg);
}

// Returns whether `ins` loads A from memory or a constant, setting Z.
bool loads_a(const ir::Type_instruction& ins) {
  const auto* mov = as<ir::Mov>(ins);
  return mov != nullptr && is_a(mov->dst) && !isa<ir::Reg>(mov->src);
}

// Returns whether `ins` is `ORA #$00`, which sets Z from A and leaves A
// as it was, as before a switch on a register parameter.
bool tests_a(const ir::Type_instruction& ins) {
  const auto* ora = as<ir::Or>(ins);
  return ora != nullptr && is_a(ora->dst) &&
         ora->src == ir::Type_operand{ir::Imm{.value = 0}};
}

// One case of a chain: the constant and the case's label.
struct Case {
  asdl::Int key;
  std::string target;
};

// Returns the cases of the chain starting at `instrs[i]`, and stores in
// `length` how many instructions the chain takes. A case for 0 may come
// first as a `BEQ` right after a load into A, which sets Z from the
// value, with no `CMP #$00`.
std::vector<Case> chain_at(const Instructions& instrs, std::size_t i,
                           std::size_t& length) {
  const std::size_t start = i;
  std::vector<Case> pairs;
  if (0 < i && i < instrs.size()) {
    const auto* br = as<ir::Branch>(instrs[i]);
    if (br != nullptr && isa<ir::EQ>(br->cond) &&
        (loads_a(instrs[i - 1]) || tests_a(instrs[i - 1]))) {
      pairs.push_back(Case{.key = 0, .target = br->target});
      i += 1;
    }
  }
  while (i + 1 < instrs.size()) {
    const auto* cmp = as<ir::Compare>(instrs[i]);
    const auto* br = as<ir::Branch>(instrs[i + 1]);
    const ir::Imm* imm = cmp != nullptr ? as<ir::Imm>(cmp->right) : nullptr;
    if (!(cmp != nullptr && is_a(cmp->left) && imm != nullptr &&
          0 <= imm->value && imm->value <= 127 && br != nullptr &&
          isa<ir::EQ>(br->cond))) {
      break;
    }
    pairs.push_back(Case{.key = imm->value, .target = br->target});
    i += 2;
  }
  length = i - start;
  return pairs;
}

// Returns `instrs` with the chain of `pairs` from `instrs[i]` to before
// `instrs[end]` made a jump table, or nothing when the rules don't
// allow it.
std::optional<Instructions> table_for(const Instructions& instrs, std::size_t i,
                                      std::size_t end,
                                      const std::vector<Case>& pairs,
                                      asdl::Int& counter) {
  std::set<asdl::Int> distinct;
  for (const auto& c : pairs) {
    distinct.insert(c.key);
  }
  if (distinct.size() != pairs.size()) {
    return std::nullopt;
  }
  const LabelMap labels(instrs);
  const ir::Jump* after =
      end < instrs.size() ? as<ir::Jump>(instrs[end]) : nullptr;
  const bool jump_default =
      after != nullptr && labels.contains(after->target) && after->reg_args.empty();
  std::optional<std::string> default_label;
  std::size_t default_at = 0;
  std::size_t rest = 0;
  if (jump_default) {
    default_label = after->target;
    default_at = static_cast<std::size_t>(labels.find(after->target));
    rest = end + 1;
  } else {
    default_at = end;
    rest = end;
  }
  const asdl::Int lo = *distinct.begin();
  const asdl::Int hi = *distinct.rbegin();
  const asdl::Int base = lo <= 2 ? asdl::Int(0) : lo;
  // Each `CMP #k` and `BEQ` is 2 bytes.
  const asdl::Int chain_bytes =
      2 * static_cast<long>(end - i) + (jump_default ? 3 : 0);
  const asdl::Int table_bytes =
      asdl::Int(base != 0 ? 4 : 0) + 4 + kDispatch + 2 * (hi - base + 1);
  if (table_bytes >= chain_bytes) {
    return std::nullopt;
  }
  // Every place the dispatch can go: A, N/Z, C, and the index register
  // have to be dead there.
  std::vector<std::size_t> places;
  for (const auto& c : pairs) {
    const long at = labels.find(c.target);
    if (at < 0) {
      return std::nullopt;
    }
    places.push_back(static_cast<std::size_t>(at));
  }
  places.push_back(default_at);
  for (const std::size_t p : places) {
    if (!(a_dead_at(instrs, labels, p) && flags_dead_at(instrs, labels, p) &&
          c_dead_at(instrs, labels, p))) {
      return std::nullopt;
    }
  }
  std::string index;
  if (std::all_of(places.begin(), places.end(), [&](std::size_t p) {
        return y_dead_at(instrs, labels, p);
      })) {
    index = "Y";
  } else if (std::all_of(places.begin(), places.end(), [&](std::size_t p) {
               return x_dead_at(instrs, labels, p);
             })) {
    index = "X";
  } else {
    return std::nullopt;
  }
  std::optional<ir::Label> new_label;
  if (!default_label) {
    default_label = ".jtdef@" + asdl::int_to_string(counter);
    counter += 1;
    new_label = ir::Label{.name = *default_label};
  }
  const std::string table = ".jtab@" + asdl::int_to_string(counter);
  counter += 1;
  std::vector<std::string> targets;
  for (asdl::Int v = base; v <= hi; v += 1) {
    std::string target = *default_label;
    for (const auto& c : pairs) {
      if (c.key == v) {
        target = c.target;
        break;
      }
    }
    targets.push_back(std::move(target));
  }
  Instructions code;
  const ir::Type_operand reg_a = ir::Reg{.reg = ir::A{}};
  if (base != 0) {
    code.push_back(ir::Compare{.left = reg_a, .right = ir::Imm{.value = base}});
    code.push_back(ir::Branch{.cond = ir::CC{}, .target = *default_label});
  }
  code.push_back(ir::Compare{.left = reg_a, .right = ir::Imm{.value = hi + 1}});
  code.push_back(ir::Branch{.cond = ir::CS{}, .target = *default_label});
  code.push_back(ir::JumpTable{
      .table = table, .index = index, .base = base, .targets = targets});
  if (new_label) {
    code.push_back(*new_label);
  }
  // The `ORA #$00` that set Z for the case for 0 has no use left, since
  // the bounds check's `CMP` sets the flags again.
  bool has_zero = false;
  for (const auto& c : pairs) {
    has_zero = has_zero || c.key == 0;
  }
  if (i > 0 && has_zero && tests_a(instrs[i - 1])) {
    i -= 1;
  }
  Instructions out(instrs.begin(), instrs.begin() + static_cast<long>(i));
  out.insert(out.end(), code.begin(), code.end());
  out.insert(out.end(), instrs.begin() + static_cast<long>(rest), instrs.end());
  return out;
}

void rewrite_function(ir::Function& fn, asdl::Int& counter) {
  Instructions instrs = fn.instructions;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    std::size_t length = 0;
    const auto pairs = chain_at(instrs, i, length);
    if (pairs.size() < kMinCases) {
      i += 1;
      continue;
    }
    auto replaced = table_for(instrs, i, i + length, pairs, counter);
    if (!replaced) {
      i += length;
      continue;
    }
    instrs = std::move(*replaced);
    changed = true;
    i += 1;
  }
  if (changed) {
    fn.instructions = std::move(instrs);
  }
}

}  // namespace

asm_ast::Program apply_jump_tables(asm_ast::Program prog) {
  asdl::Int counter = next_label_number(prog, {".jtab@", ".jtdef@"});
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, counter);
    }
  }
  return prog;
}

}  // namespace c6502::passes
