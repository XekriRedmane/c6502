/* TAC pass of the fixed point that sinks the increment of a Var past
the Var's last use.

# What it does

It looks for the pattern

    Binary(Add | Subtract, X, c, Y)   # defines Y, whose uses come later
    ... no use of Y ...
    <last use of X>                    # X read here
    ... rest ...

and moves the Binary to just after X's last use:

    ... no use of Y ...                # Y not yet defined
    <last use of X>                    # X read here, for the last time
    Binary(Add | Subtract, X, c, Y)   # moved here, after X's range ends
    ... rest ...

After the move, X's live range ends at the old last use and Y's starts
at the new position. The two no longer overlap, so they don't interfere
in the register allocator, which lets a chain of `Y_i = Y_{i-1} + 1`
increments all share one register (typically Y, for counters that index
arrays).

# Why it matters

In the idiom of a postfix increment and then a use,

    pixels = arr[y++];

the lowering from c99 gives, after SSA and copy propagation,

    Binary(Add, y_phi, 1, y.1)
    IndexedConstLoad(arr_addr, y_phi, pixels.0)

where the Add comes first (defining y.1) and the IndexedConstLoad then
reads y_phi, the value before the increment. Both y_phi and y.1 are live
at the IndexedConstLoad, y_phi as its index and y.1 just defined. The
interference graph has an edge between them, which forces different
colors.

For a chain of 7 stages (`pixels = arr[y++]` repeated 7 times), every
`y_after_bN` interferes with `y_after_b(N-1)` and with
`y_after_b(N+1)`. Only one of the eight values can have the one
register, and the rest spill to zero page.

After this pass, each Add is sunk past its IndexedConstLoad, and the
chain becomes

    IndexedConstLoad(arr_addr, y_phi, pixels.0)
    Binary(Add, y_phi, 1, y.1)
    IndexedConstLoad(arr_addr, y.1, pixels.1)
    Binary(Add, y.1, 1, y.2)
    ...

where each y.N is dead before y.(N+1) is defined. The interference
edges are gone, so all of them can share one register, and the asm INC
peephole then turns each `Add(Reg(Y), 1, Reg(Y))` into `INY`. Eight
chains of `LDA Y; CLC; ADC #1; STA <slot>` become 7 `INY`s.

# Algorithm

The pass walks each linear segment of a block, the instructions between
two boundaries of control flow (a Label, a jump, a call, and so on). For
each `Binary(Add | Subtract, X, c, Y)` where X is a Var and c a
Constant, it does this.

  1. Find the last use of X within this segment after the Binary. With
     no such use, nothing moves.
  2. Check that no instruction between the Binary and that last use
     reads or writes Y. A read would see Y before its definition after
     the move, and a write isn't possible in SSA form, so that check is
     only a guard. Skip on a conflict.
  3. Move the Binary to just after the last use.

# Soundness

  * In SSA form every Var has one definition, so X isn't redefined
    between the Binary and any later instruction, and reading X at the
    new position gives the value it had before. The pass requires X to
    be in `ctx.ssa_dsts`, since a static or an address-taken local can
    be written by name or through a pointer in between. The scan also
    stops at any write to X by name, and, when no SSA information is
    present, at every store.
  * Y's definition moves later. No instruction between saw Y before,
    since the check of uses of Y rejects a read, so Y's definition still
    comes before its first use.
  * A call or a store doesn't move itself. The Binary moves, and it is
    pure arithmetic on SSA Vars, so moving it across other operations is
    sound when no dependence on a name is broken.
  * Nothing moves across a block's boundary, since the walk of a
    segment stops there.

# Where to run

In the TAC fixed-point loop, after copy propagation, so that the chains
of Copies have settled and the Binary's src1 is stable. Each pass of the
loop sinks each link of a chain once, and the fixed point handles long
chains.

# Only a constant src2

Only `Add(X, Constant, Y)` and `Subtract(X, Constant, Y)` move. Moving
`Add(X, Z, Y)` for a Var Z later delays the read of Z, which an
instruction between the Binary and X's last use might change through a
static or an address-taken name, and the other operations aren't in
scope, since the motivating case is the increment of an induction
variable.

# The test of the step

After the moves, a jump on X right after `Y = X - c` (or `X + c`), X's
last use, tests Y instead (`test_the_step`). */
#include "passes/optimization/sink_increment.h"

#include <set>
#include <string>
#include <string_view>

#include "bit_precise.h"
#include "passes/optimization/narrow_widened_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using asdl::Int;
using framework::Instrs;
using framework::PassContext;
using IK = ir::Kind_instruction;

// Returns whether `instr` ends a segment of a block for this pass.
// Labels and control flow change what the program can reach, and calls
// have side effects the pass doesn't reason about.
bool is_block_terminator(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case IK::Label:
    case IK::Jump:
    case IK::JumpIfTrue:
    case IK::JumpIfFalse:
    case IK::JumpIfCmp:
    case IK::JumpIfMasked:
    case IK::Ret:
    case IK::FunctionCall:
    case IK::IndirectCall:
      return true;
    default:
      return false;
  }
}

// Returns whether `instr` writes memory without naming the cell in its
// definitions. Without SSA information, X could be a static or an
// address-taken local that one of these writes.
bool is_store(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case IK::Store:
    case IK::IndexedStore:
    case IK::IndexedSymbolStore:
    case IK::IndirectIndexedStore:
      return true;
    default:
      return false;
  }
}

// The Binary of the pattern: an `Add` or `Subtract` of a Var and a
// Constant into a Var.
struct Step {
  const ir::Binary* binary;
  const ir::Var* x;
  const ir::Var* y;
};

std::optional<Step> as_step(const ir::Type_instruction& instr) {
  const auto* b = std::get_if<ir::Binary>(&instr);
  if (b == nullptr ||
      !(std::holds_alternative<ir::Add>(b->op) || std::holds_alternative<ir::Subtract>(b->op))) {
    return std::nullopt;
  }
  const auto* x = std::get_if<ir::Var>(&b->src1);
  const auto* y = std::get_if<ir::Var>(&b->dst);
  if (x == nullptr || y == nullptr || !std::holds_alternative<ir::Constant>(b->src2)) {
    return std::nullopt;
  }
  return Step{b, x, y};
}

// Returns whether `instr` is a sinkable `Binary(Add | Subtract, X, c,
// Y)` with X and Y different (Python's `_sink_candidate`).
bool is_sink_candidate(const ir::Type_instruction& instr) {
  const auto s = as_step(instr);
  return s && s->x->name != s->y->name;
}

// Returns the number of instructions past `instrs[start]` that the
// Binary there can sink past, or none when it can't move. The
// instruction at `start + offset` reads X and is the last one in the
// segment that does, and no instruction from `start + 1` to `start +
// offset` reads or writes Y, ends the segment, writes X, or, with
// `stop_at_stores`, writes memory through a store.
std::optional<std::size_t> find_sink_offset(const Instrs& instrs, std::size_t start,
                                            std::string_view x_name, std::string_view y_name,
                                            bool stop_at_stores) {
  std::optional<std::size_t> last_x_use;
  for (std::size_t j = start + 1; j < instrs.size(); ++j) {
    const auto& instr = instrs[j];
    if (is_block_terminator(instr)) {
      break;
    }
    // A use or a definition of Y caps the scan, since Y's definition has
    // to come before any read of Y, and a second definition of Y can't
    // happen in SSA form. A use of X found before it is still a valid
    // target.
    bool uses_x = false;
    bool touches_y = false;
    for (const auto u : uses_in(instr)) {
      uses_x = uses_x || u == x_name;
      touches_y = touches_y || u == y_name;
    }
    bool defines_x = false;
    for (const auto d : defs_in(instr)) {
      touches_y = touches_y || d == y_name;
      defines_x = defines_x || d == x_name;
    }
    if (touches_y) {
      break;
    }
    // The moved Binary reads X at its new position, so X must hold the
    // same value there. An SSA-renamed X has one definition and never
    // reaches this test, but a write to X by name ends the scan anyway.
    if (defines_x) {
      break;
    }
    if (stop_at_stores && is_store(instr)) {
      break;
    }
    // A sinkable Binary that also reads X stops the scan. Otherwise two
    // sinkable Binaries that share their src1 would each sink past the
    // other forever, each taking the other as a later reader of X worth
    // sinking past. With such peers as boundaries the pass is
    // idempotent: each Binary settles right before the next sinkable
    // peer, if any, or right after its last reader.
    if (uses_x && is_sink_candidate(instr)) {
      break;
    }
    if (uses_x) {
      last_x_use = j - start;
    }
  }
  return last_x_use;
}

// Returns a jump on X right after `Y = X - c` (or `X + c`), X's last
// use, as a test of Y: `X != k` is `Y != k - c` modulo the width, and so
// is `X == k`, and a jump on X being nonzero is a test of Y against
// `-c`. X's live range then ends at the step, so X and Y can share a
// register. `while (x-- != 0)` in Zork's `search_nonalpha_table` had
// kept the old and the new `x`, computed the step through A into a
// slot, and copied it back on the loop's edge, and it's now `DEX; CPX
// #$FF; BNE`. The jump has to come right after the step, with only
// instructions that read neither between, and nothing after it may read
// X, nor any Phi, so the rewrite only shortens X's range. A jump
// rewritten here is what later steps of the walk see.
ir::Function test_the_step(ir::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr || ctx.symbols == nullptr) {
    return fn;
  }
  Instrs instrs = fn.instructions;
  std::set<std::string, std::less<>> phi_uses;
  for (const auto& ins : instrs) {
    if (std::holds_alternative<ir::Phi>(ins)) {
      for (const auto u : uses_in(ins)) {
        phi_uses.insert(std::string(u));
      }
    }
  }
  const auto reads = [](const ir::Type_instruction& ins, std::string_view name) {
    for (const auto u : uses_in(ins)) {
      if (u == name) {
        return true;
      }
    }
    return false;
  };
  const auto is_x = [](const ir::Type_val& v, const ir::Var& x) {
    const auto* var = std::get_if<ir::Var>(&v);
    return var != nullptr && var->name == x.name;
  };
  bool changed = false;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const auto step = as_step(instrs[i]);
    if (!step || !ctx.ssa_dsts->contains(step->x->name) ||
        !ctx.ssa_dsts->contains(step->y->name) || step->x->name == step->y->name) {
      continue;
    }
    const auto c = bit_precise::raw_value(std::get<ir::Constant>(step->binary->src2).const_);
    if (!c) {
      continue;
    }
    const ir::Var x = *step->x;
    const ir::Var y = *step->y;
    const bool subtract = std::holds_alternative<ir::Subtract>(step->binary->op);
    const auto width = narrow_widened_arith::byte_width(y, ctx.symbols);
    if (!width || width != narrow_widened_arith::byte_width(x, ctx.symbols)) {
      continue;
    }
    const auto is_unsigned = narrow_widened_arith::is_unsigned(y, ctx.symbols);
    if (!is_unsigned || phi_uses.contains(x.name)) {
      continue;
    }
    std::size_t j = i + 1;
    while (j < instrs.size()) {
      const IK k = ir::kind(instrs[j]);
      if (k == IK::Label || k == IK::Jump || k == IK::JumpIfTrue || k == IK::JumpIfFalse ||
          k == IK::JumpIfCmp || k == IK::JumpIfMasked || k == IK::Ret) {
        break;
      }
      if (reads(instrs[j], x.name) || reads(instrs[j], y.name)) {
        break;
      }
      ++j;
    }
    if (j >= instrs.size()) {
      continue;
    }
    const auto& jump = instrs[j];
    Int k = 0;
    ir::Type_binary_operator op;
    std::string target;
    if (const auto* jt = std::get_if<ir::JumpIfTrue>(&jump)) {
      if (!is_x(jt->condition, x)) {
        continue;
      }
      op = ir::NotEqual{};
      target = jt->target;
    } else if (const auto* jf = std::get_if<ir::JumpIfFalse>(&jump)) {
      if (!is_x(jf->condition, x)) {
        continue;
      }
      op = ir::Equal{};
      target = jf->target;
    } else if (const auto* jc = std::get_if<ir::JumpIfCmp>(&jump);
               jc != nullptr && (std::holds_alternative<ir::Equal>(jc->op) ||
                                 std::holds_alternative<ir::NotEqual>(jc->op))) {
      std::optional<Int> kv;
      if (is_x(jc->src1, x) && std::holds_alternative<ir::Constant>(jc->src2)) {
        kv = bit_precise::raw_value(std::get<ir::Constant>(jc->src2).const_);
      } else if (is_x(jc->src2, x) && std::holds_alternative<ir::Constant>(jc->src1)) {
        kv = bit_precise::raw_value(std::get<ir::Constant>(jc->src1).const_);
      } else {
        continue;
      }
      if (!kv) {
        continue;
      }
      k = *kv;
      op = jc->op;
      target = jc->target;
    } else {
      continue;
    }
    bool read_later = false;
    for (std::size_t r = j + 1; r < instrs.size() && !read_later; ++r) {
      read_later = reads(instrs[r], x.name);
    }
    if (read_later) {
      continue;
    }
    const Int moved = subtract ? k - *c : k + *c;
    const auto narrowed = narrow_widened_arith::narrow_constant(
        ir::Constant{.const_ = ir::ConstInt{.value = moved}}, *width, *is_unsigned);
    if (!narrowed) {
      continue;
    }
    instrs[j] = ir::JumpIfCmp{.op = std::move(op), .src1 = y, .src2 = *narrowed, .target = target};
    changed = true;
  }
  if (!changed) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(instrs));
}

}  // namespace

ir::Function sink_increments(ir::Function fn, const PassContext& ctx) {
  ir::Function sunk = framework::run_sink(
      std::move(fn), [&](const Instrs& instrs, std::size_t i) -> std::optional<std::size_t> {
        const auto step = as_step(instrs[i]);
        if (!step || step->x->name == step->y->name) {
          return std::nullopt;
        }
        // A static or an address-taken local can be written by name or
        // through a pointer between the Binary and X's last use, and the
        // moved Binary would then read the new value. Only an
        // SSA-renamed X keeps one value. Without SSA information (the
        // standalone entry point of the Python unit tests), the scan
        // stops at every store.
        if (ctx.ssa_dsts != nullptr && !ctx.ssa_dsts->contains(step->x->name)) {
          return std::nullopt;
        }
        const auto offset = find_sink_offset(instrs, i, step->x->name, step->y->name,
                                             /*stop_at_stores=*/ctx.ssa_dsts == nullptr);
        if (!offset) {
          return std::nullopt;
        }
        // The Binary ends up after `instrs[i + offset]`, X's last use.
        return i + *offset;
      });
  return test_the_step(std::move(sunk), ctx);
}

}  // namespace c6502::passes
