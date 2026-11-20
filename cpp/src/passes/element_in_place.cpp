// Steps an array element in place when a slot holds a copy of it.
//
// Pattern
//
// `uint8_t timer = --BRICK_FILL_TIMERS[i];` in Lode Runner's
// `handle_timers` reads the element into one of the function's own
// slots, decrements the slot, and stores it back, and the code after it
// reads the slot:
//
//         LDA   BRICK_FILL_TIMERS,X
//         STA   t
//         BNE   .skip
//         JMP   .next
//     .skip:
//         DEC   t                     ; the site
//         LDA   t
//         STA   BRICK_FILL_TIMERS,X
//         BEQ   .expired
//         ...
//         LDA   t                     ; a read of the new value
//         CMP   #$14
//
// When `t` holds the same value as the element at the `DEC`, the site
// becomes `DEC BRICK_FILL_TIMERS,X; LDA BRICK_FILL_TIMERS,X`, and each
// read of `t` that the site's new value reaches reads the element
// instead:
//
//     .skip:
//         DEC   BRICK_FILL_TIMERS,X
//         LDA   BRICK_FILL_TIMERS,X
//         BEQ   .expired
//         ...
//         LDA   BRICK_FILL_TIMERS,X
//         CMP   #$14
//
// `redundant_load_after_rmw` then drops the `LDA` after the `DEC` when A
// is dead, and `asm_dead_store` drops the `STA t` that nothing reads any
// more, which leaves the original's `DEC BRICK_FILL_TIMERS,X; BEQ`. The
// site is `INC`, `DEC`, `ASL`, `LSR`, `ROL`, or `ROR` of `t`, then `LDA
// t` and a non-volatile `STA e` of an element `e` indexed by X, the
// three adjacent.
//
// When it applies
//
// `t` is a `Data` slot of the function's own pool (`__local_<fn>__*`)
// whose address the function never forms, so it's dead when the
// function returns or jumps to another function, no callee reads it,
// and only the function's own instructions can write it. Its address
// must be in the slot map, so that a `(zp),Y` access through a pointer
// in its bytes counts as a read of it.
//
// A forward dataflow over the function's instructions follows which of
// A, `t`, and `e` hold the same value (`transfer`). A non-volatile
// `LDA e` or `LDA t` makes A equal to that byte, and a store of A makes
// the byte stored equal to whatever A equals. A write of X changes the
// byte `e` names, so it ends what's known of `e`. So does a write that
// may reach `e` (`may_reach`), a call, and any instruction the pass
// doesn't follow. At a label, what's known is what holds on every way
// in. A site qualifies when `t` equals `e` before it.
//
// From each qualifying site the pass walks forward along every path
// until `t` is written without being read, or the function ends. Every
// read of `t` it meets must be another qualifying site, or a load,
// compare, or ALU operation with `t` as its source, before which `t`
// equals `e`, so that it can read `e` instead (`rewritten`). There's
// no `LDX abs,X` or `CPX abs,X`, so a read into or against X doesn't
// qualify. Otherwise the pass leaves `t` alone.
//
// Why it's sound
//
// Outside the sites, the rewritten program runs the same instructions
// on the same values, except that the reads it changes read `e` where
// the original read `t`, which held the same value there. Inside a
// site, `DEC e` writes the value the original's `STA e` wrote, and
// `LDA e` loads the value the original's `LDA t` loaded. X doesn't
// change within the three instructions, so `e` names the same byte at
// each. The flags come out the same: the step sets N and Z (and for a
// shift, C) from the same value in either place, the `LDA` sets N and
// Z again, and the store sets none. Only `t` differs afterwards. It
// keeps its old value where the original stepped it. Every read of `t`
// that value can reach has become a read of `e`, and `t` is dead where
// the walk stopped.
//
// `e` is never volatile, since only non-volatile accesses make it equal
// to anything.
//
// Cost
//
// A site in zero page is 7 bytes and becomes 6, or 3 when A is dead
// after it and the `LDA` goes. Each read it rewrites becomes a byte
// longer, an absolute address in place of a zero-page one. The pass
// rewrites a slot only when the sum is a gain, before counting the
// store into `t` that dies with it. In `handle_timers` it saves 5
// bytes: 4 at the site, minus 1 for the compare's load, plus 2 for the
// store.
//
// Where it runs
//
// In the asm peephole fixed point, after `dec_peephole` and
// `inc_peephole` have made the step an `INC` / `DEC` and before
// `redundant_load_after_rmw` and `asm_dead_store`. It needs the slot
// map, so it doesn't run in the unoptimized pipeline, whose locals are
// on the soft stack.
#include "passes/element_in_place.h"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "passes/inc_peephole.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

const ir::Type_operand kA = ir::Reg{.reg = ir::A{}};

// The cells whose equality the dataflow follows.
enum Cell { kCellA, kCellT, kCellE };

// What is known before an instruction is a set of pairs of cells that
// hold the same value, as bits: A and t, A and e, t and e. Two pairs
// give the third, so a set holds none, one, or all three.
using Facts = std::uint8_t;
constexpr Facts kAT = 1;
constexpr Facts kAE = 2;
constexpr Facts kTE = 4;
constexpr Facts kAll = kAT | kAE | kTE;

Facts pair_of(Cell a, Cell b) {
  if ((a == kCellA && b == kCellT) || (a == kCellT && b == kCellA)) {
    return kAT;
  }
  if ((a == kCellA && b == kCellE) || (a == kCellE && b == kCellA)) {
    return kAE;
  }
  return kTE;
}

Facts kill(Facts s, Cell cell) {
  switch (cell) {
    case kCellA:
      return s & static_cast<Facts>(~(kAT | kAE));
    case kCellT:
      return s & static_cast<Facts>(~(kAT | kTE));
    case kCellE:
      return s & static_cast<Facts>(~(kAE | kTE));
  }
  return s;
}

// Returns `s` after `cell` takes the value of `value` (a cell, or
// nothing).
Facts assign(Facts s, Cell cell, std::optional<Cell> value) {
  s = kill(s, cell);
  if (!value || *value == cell) {
    return s;
  }
  s |= pair_of(cell, *value);
  return std::popcount(static_cast<unsigned>(s)) >= 2 ? kAll : s;
}

bool starts_with(const std::string& s, const std::string& prefix) {
  return s.compare(0, prefix.size(), prefix) == 0;
}

bool is_rmw(const ir::Type_instruction& ins) {
  return isa<ir::Inc, ir::Dec, ir::ArithmeticShiftLeft, ir::LogicalShiftRight,
             ir::RotateLeft, ir::RotateRight>(ins);
}

bool is_alu(const ir::Type_instruction& ins) {
  return isa<ir::Add, ir::Sub, ir::And, ir::Or>(ins);
}

// Returns the `dst` of a step, an ALU instruction, or a `Pop`, and
// null for any other instruction.
const ir::Type_operand* dst_of(const ir::Type_instruction& ins) {
  switch (ir::kind(ins)) {
    case ir::Kind_instruction::Inc:
      return &std::get<ir::Inc>(ins).dst;
    case ir::Kind_instruction::Dec:
      return &std::get<ir::Dec>(ins).dst;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return &std::get<ir::ArithmeticShiftLeft>(ins).dst;
    case ir::Kind_instruction::LogicalShiftRight:
      return &std::get<ir::LogicalShiftRight>(ins).dst;
    case ir::Kind_instruction::RotateLeft:
      return &std::get<ir::RotateLeft>(ins).dst;
    case ir::Kind_instruction::RotateRight:
      return &std::get<ir::RotateRight>(ins).dst;
    case ir::Kind_instruction::Add:
      return &std::get<ir::Add>(ins).dst;
    case ir::Kind_instruction::Sub:
      return &std::get<ir::Sub>(ins).dst;
    case ir::Kind_instruction::And:
      return &std::get<ir::And>(ins).dst;
    case ir::Kind_instruction::Or:
      return &std::get<ir::Or>(ins).dst;
    case ir::Kind_instruction::Xor:
      return &std::get<ir::Xor>(ins).dst;
    case ir::Kind_instruction::Pop:
      return &std::get<ir::Pop>(ins).dst;
    default:
      return nullptr;
  }
}

// Returns the `dst` of a step instruction, which `is_rmw` accepts.
const ir::Type_operand& rmw_dst(const ir::Type_instruction& ins) { return *dst_of(ins); }

// Returns `ins`, a step instruction, with its operand replaced by `dst`.
ir::Type_instruction with_rmw_dst(const ir::Type_instruction& ins,
                                  const ir::Type_operand& dst) {
  switch (ir::kind(ins)) {
    case ir::Kind_instruction::Inc:
      return ir::Inc{.dst = dst};
    case ir::Kind_instruction::Dec:
      return ir::Dec{.dst = dst};
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return ir::ArithmeticShiftLeft{.dst = dst};
    case ir::Kind_instruction::LogicalShiftRight:
      return ir::LogicalShiftRight{.dst = dst};
    case ir::Kind_instruction::RotateLeft:
      return ir::RotateLeft{.dst = dst};
    default:
      return ir::RotateRight{.dst = dst};
  }
}

// Returns whether a move into X or Y from `op` is an `LDX` / `LDY` that
// reads it without going through A. A move into X or Y from anything
// else is taken to use A.
bool is_direct(const ir::Type_operand& op) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::ZP,
             ir::IndexedData, ir::Reg>(op);
}

// Returns the names whose address an operand of `instrs` forms.
NameSet named_labels(const std::vector<ir::Type_instruction>& instrs) {
  NameSet out;
  for (const auto& ins : instrs) {
    for_each_operand<true>(ins, [&](const ir::Type_operand& op) {
      if (const auto* lo = as<ir::ImmLabelLow>(op)) {
        out.insert(lo->name);
      } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
        out.insert(hi->name);
      }
    });
  }
  return out;
}

// The state the dataflow and the rewrite share for one pair of a slot
// `t` and an element `e`.
struct Pair {
  const ir::Type_operand& t;
  const ir::Type_operand& e;
  const AsmContext& ctx;
  const SymbolAddrs& slots;

  std::optional<Cell> key(const ir::Type_operand& op) const {
    if (op == t) {
      return kCellT;
    }
    if (op == e) {
      return kCellE;
    }
    return std::nullopt;
  }

  bool in_zero_page(const std::string& name) const {
    if (ctx.is_user_zp_global(name) || is_zp_symbol(name)) {
      return true;
    }
    return ctx.layout.addr(name) != nullptr;
  }

  // Returns whether a write of `op` may change the element `e`. A byte
  // of a zero-page object can't be an element of an object outside zero
  // page indexed from its start or past it, which lies at `$100` or
  // above, so the store of `GAME_COLNUM` between `handle_timers`'s site
  // and its compare leaves the timer alone. `may_alias` treats two names
  // as the same memory unless one is a compiler slot.
  bool may_reach(const ir::Type_operand& op) const {
    if (!may_alias(op, e, ctx, {.zp_slot_symbols = &slots})) {
      return false;
    }
    const auto& el = std::get<ir::IndexedData>(e);
    const auto* d = as<ir::Data>(op);
    return !(d != nullptr && d->name != el.name && !el.name.empty() && el.offset >= 0 &&
             in_zero_page(d->name) && !in_zero_page(el.name));
  }

  // Returns `s` after a write of `dst`, which ends up equal to `value`
  // (one of the cells, or nothing).
  Facts write(Facts s, const ir::Type_operand& dst, std::optional<Cell> value,
              bool is_volatile) const {
    if (dst == t) {
      return assign(s, kCellT, value);
    }
    if (dst == e && !is_volatile) {
      return assign(s, kCellE, value);
    }
    if (may_alias(dst, t, ctx, {.zp_slot_symbols = &slots})) {
      s = kill(s, kCellT);
    }
    if (may_reach(dst)) {
      s = kill(s, kCellE);
    }
    return s;
  }

  Facts transfer_mov(const ir::Mov& ins, Facts s) const {
    const auto& src = ins.src;
    const auto& dst = ins.dst;
    const bool vol = ins.is_volatile;
    if (const auto* reg = as<ir::Reg>(dst)) {
      if (isa<ir::A>(reg->reg)) {
        return assign(s, kCellA, vol ? std::nullopt : key(src));
      }
      if (kills_a(ins) || !is_direct(src)) {
        s = kill(s, kCellA);
      }
      if (isa<ir::X>(reg->reg)) {
        s = kill(s, kCellE);
      }
      return s;
    }
    std::optional<Cell> value;
    if (src == kA) {
      value = kCellA;
    } else if (isa<ir::Reg>(src)) {
      if (kills_a(ins)) {
        s = kill(s, kCellA);
      }
    } else {
      // `LDA src; STA dst`.
      s = assign(s, kCellA, vol ? std::nullopt : key(src));
      value = kCellA;
    }
    return write(s, dst, value, vol);
  }

  Facts transfer(const ir::Type_instruction& ins, Facts s) const {
    // Instructions that change no register but A and no byte, and that
    // the walk passes over.
    if (isa<ir::Label, ir::Branch, ir::Compare, ir::BitTest, ir::SetCarry,
            ir::ClearCarry, ir::Push>(ins)) {
      return s;
    }
    if (const auto* jump = as<ir::Jump>(ins)) {
      return starts_with(jump->target, ".") ? s : Facts{0};
    }
    if (const auto* mov = as<ir::Mov>(ins)) {
      return transfer_mov(*mov, s);
    }
    if (is_rmw(ins) || isa<ir::Pop>(ins)) {
      const ir::Type_operand& d = *dst_of(ins);
      if (const auto* reg = as<ir::Reg>(d)) {
        if (isa<ir::A>(reg->reg)) {
          return kill(s, kCellA);
        }
        if (isa<ir::X>(reg->reg)) {
          return kill(s, kCellE);
        }
        return s;
      }
      return write(s, d, std::nullopt, true);
    }
    if (is_alu(ins) || isa<ir::Xor>(ins)) {
      const ir::Type_operand& d = *dst_of(ins);
      if (d == kA) {
        return kill(s, kCellA);
      }
      return 0;
    }
    return 0;
  }

  // Returns, for each instruction, the pairs of A, `t`, and `e` known to
  // hold the same value before it, or nothing where the walk doesn't
  // reach.
  std::vector<std::optional<Facts>> facts(const std::vector<ir::Type_instruction>& instrs,
                                          const LabelMap& labels,
                                          const NameSet& named) const {
    const std::size_t n = instrs.size();
    std::vector<std::optional<Facts>> before(n);
    if (n == 0) {
      return before;
    }
    before[0] = Facts{0};
    std::vector<std::size_t> work = {0};
    while (!work.empty()) {
      const std::size_t j = work.back();
      work.pop_back();
      const auto& ins = instrs[j];
      const Facts after = transfer(ins, *before[j]);
      std::vector<std::size_t> succ;
      if (const auto* table = as<ir::JumpTable>(ins)) {
        for (const auto& x : table->targets) {
          if (const long k = labels.find(x); k >= 0) {
            succ.push_back(static_cast<std::size_t>(k));
          }
        }
      } else {
        succ = asm_dead_store::successors(instrs, j, labels);
      }
      for (const std::size_t s : succ) {
        // A label whose address is formed may be reached from anywhere.
        Facts state = after;
        if (const auto* label = as<ir::Label>(instrs[s]);
            label != nullptr && named.contains(label->name)) {
          state = 0;
        }
        const std::optional<Facts> old = before[s];
        const Facts updated = old ? static_cast<Facts>(*old & state) : state;
        if (!old || updated != *old) {
          before[s] = updated;
          work.push_back(s);
        }
      }
    }
    return before;
  }

  // Returns `ins` reading `e` where it reads `t`, when `t` is its only
  // use of `t` and the 6502 has the abs,X form, else nothing.
  std::optional<ir::Type_instruction> rewritten(const ir::Type_instruction& ins) const {
    if (const auto* mov = as<ir::Mov>(ins); mov != nullptr && mov->src == t && mov->dst != t) {
      if (const auto* reg = as<ir::Reg>(mov->dst); reg != nullptr && isa<ir::X>(reg->reg)) {
        return std::nullopt;
      }
      return ir::Mov{.src = e, .dst = mov->dst, .is_volatile = mov->is_volatile};
    }
    if (const auto* cmp = as<ir::Compare>(ins);
        cmp != nullptr && cmp->left == kA && cmp->right == t) {
      return ir::Compare{.left = kA, .right = e};
    }
    if (const auto* i = as<ir::Add>(ins); i != nullptr && i->dst == kA && i->src == t) {
      return ir::Add{.src = e, .dst = kA};
    }
    if (const auto* i = as<ir::Sub>(ins); i != nullptr && i->dst == kA && i->src == t) {
      return ir::Sub{.src = e, .dst = kA};
    }
    if (const auto* i = as<ir::And>(ins); i != nullptr && i->dst == kA && i->src == t) {
      return ir::And{.src = e, .dst = kA};
    }
    if (const auto* i = as<ir::Or>(ins); i != nullptr && i->dst == kA && i->src == t) {
      return ir::Or{.src = e, .dst = kA};
    }
    if (const auto* x = as<ir::Xor>(ins);
        x != nullptr && x->dst == kA && ((x->src1 == t) != (x->src2 == t))) {
      return ir::Xor{.src1 = x->src1 == t ? e : x->src1,
                     .src2 = x->src2 == t ? e : x->src2,
                     .dst = kA};
    }
    return std::nullopt;
  }
};

// Returns whether `ins` writes `t` without reading it.
bool writes_whole(const ir::Type_instruction& ins, const ir::Type_operand& t) {
  if (const auto* mov = as<ir::Mov>(ins)) {
    return mov->dst == t;
  }
  if (const auto* pop = as<ir::Pop>(ins)) {
    return pop->dst == t;
  }
  return false;
}

// Returns the indexes of `<step> t; LDA t; STA e` with `t` an eligible
// slot and `e` a non-volatile element indexed by X.
std::vector<std::size_t> sites(const ir::Function& fn, const SymbolAddrs& slots) {
  const auto& instrs = fn.instructions;
  const std::string prefix = "__local_" + fn.name + "__";
  const NameSet taken = named_labels(instrs);
  std::vector<std::size_t> out;
  for (std::size_t i = 0; i + 2 < instrs.size(); ++i) {
    const auto& step = instrs[i];
    const auto& load = instrs[i + 1];
    const auto& store = instrs[i + 2];
    if (!is_rmw(step)) {
      continue;
    }
    const auto* t = as<ir::Data>(rmw_dst(step));
    if (!(t != nullptr && starts_with(t->name, prefix) && !taken.contains(t->name) &&
          slots.contains(t->name))) {
      continue;
    }
    const auto* ld = as<ir::Mov>(load);
    if (!(ld != nullptr && ld->src == rmw_dst(step) && ld->dst == kA && !ld->is_volatile)) {
      continue;
    }
    const auto* st = as<ir::Mov>(store);
    if (st != nullptr && st->src == kA && is_x_indexed(st->dst) && !st->is_volatile) {
      out.push_back(i);
    }
  }
  return out;
}

// Returns the instructions with the sites at `indices` (one slot `t`,
// one element `e`) and the reads they reach rewritten, or nothing.
std::optional<std::vector<ir::Type_instruction>> rewrite_pair(
    const std::vector<ir::Type_instruction>& instrs, const std::vector<std::size_t>& indices,
    const AsmContext& ctx, const SymbolAddrs& slots) {
  const ir::Type_operand t = rmw_dst(instrs[indices[0]]);
  const ir::Type_operand e = std::get<ir::Mov>(instrs[indices[0] + 2]).dst;
  const Pair pair{.t = t, .e = e, .ctx = ctx, .slots = slots};
  const LabelMap labels(instrs);
  const NameSet named = named_labels(instrs);
  const auto facts = pair.facts(instrs, labels, named);
  auto holds_te = [&](std::size_t j) { return facts[j] && (*facts[j] & kTE) != 0; };
  std::vector<std::size_t> good;
  for (const std::size_t i : indices) {
    if (holds_te(i)) {
      good.push_back(i);
    }
  }
  if (good.empty()) {
    return std::nullopt;
  }
  const std::set<std::size_t> starts(good.begin(), good.end());
  std::set<std::size_t> reads;
  std::set<std::size_t> seen;
  std::vector<std::size_t> stack;
  for (const std::size_t i : good) {
    stack.push_back(i + 3);
  }
  while (!stack.empty()) {
    const std::size_t j = stack.back();
    stack.pop_back();
    if (j >= instrs.size() || seen.contains(j)) {
      continue;
    }
    seen.insert(j);
    if (starts.contains(j)) {
      stack.push_back(j + 3);
      continue;
    }
    const auto& ins = instrs[j];
    if (isa<ir::Phi, ir::FunctionPrologue, ir::AllocateStack, ir::DecimalArith>(ins)) {
      return std::nullopt;
    }
    if (asm_dead_store::reads(ins, t, ctx, &slots, true)) {
      if (!holds_te(j) || !pair.rewritten(ins)) {
        return std::nullopt;
      }
      reads.insert(j);
    } else if (writes_whole(ins, t)) {
      continue;
    }
    if (const auto* table = as<ir::JumpTable>(ins)) {
      for (const auto& n : table->targets) {
        if (const long k = labels.find(n); k >= 0) {
          stack.push_back(static_cast<std::size_t>(k));
        }
      }
      continue;
    }
    for (const std::size_t s : asm_dead_store::successors(instrs, j, labels)) {
      stack.push_back(s);
    }
  }
  long gain = 0;
  for (const std::size_t i : good) {
    gain += 1 + (a_dead_at(instrs, labels, i + 3) ? 3 : 0);
  }
  if (gain - static_cast<long>(reads.size()) <= 0) {
    return std::nullopt;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto& ins = instrs[k];
    if (starts.contains(k)) {
      out.push_back(with_rmw_dst(ins, e));
      out.push_back(ir::Mov{.src = e, .dst = kA, .is_volatile = false});
      continue;
    }
    bool inside = false;
    for (const std::size_t s : starts) {
      if (s < k && k <= s + 2) {
        inside = true;
        break;
      }
    }
    if (inside) {
      continue;
    }
    if (reads.contains(k)) {
      out.push_back(*pair.rewritten(ins));
    } else {
      out.push_back(ins);
    }
  }
  return out;
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx, const SymbolAddrs& slots) {
  const auto& instrs = fn.instructions;
  const auto found = sites(fn, slots);
  if (found.empty()) {
    return;
  }
  // The sites grouped by their slot and element, in the order of each
  // group's first site.
  std::vector<std::pair<std::pair<ir::Type_operand, ir::Type_operand>,
                        std::vector<std::size_t>>>
      by_pair;
  for (const std::size_t i : found) {
    std::pair<ir::Type_operand, ir::Type_operand> k{rmw_dst(instrs[i]),
                                                    std::get<ir::Mov>(instrs[i + 2]).dst};
    bool placed = false;
    for (auto& [key, indices] : by_pair) {
      if (key == k) {
        indices.push_back(i);
        placed = true;
        break;
      }
    }
    if (!placed) {
      by_pair.push_back({std::move(k), {i}});
    }
  }
  for (const auto& [key, indices] : by_pair) {
    if (auto out = rewrite_pair(instrs, indices, ctx, slots)) {
      fn.instructions = std::move(*out);
      return;
    }
  }
}

}  // namespace

asm_ast::Program apply_element_in_place(asm_ast::Program prog, const AsmContext& ctx,
                                        const SymbolAddrs* zp_slot_symbols) {
  if (zp_slot_symbols == nullptr || zp_slot_symbols->empty()) {
    return prog;
  }
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx, *zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
