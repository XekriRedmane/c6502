// Memory-value propagation over the CFG.
//
// The pass follows, at every point of a function, the recomputable
// source expression each zero-page byte holds. The analysis is a forward
// dataflow of facts that must hold, over the function's CFG, and a join
// keeps only the facts every predecessor agrees on.
//
// The rewriter walks each instruction with the state before it and
// rewrites the operands that read a followed byte to read its source
// directly. An access through DPTR (`Indirect(off)` / `IndirectY()`)
// becomes `IndirectZp(N, off)` / `IndirectZpY(N)` wherever the dataflow
// shows that DPTR holds the bytes of a stable zero-page pair at address
// `N`. When the pair at `N` holds the two halves of a label's address,
// `#<name` and `#>name`, the access goes to the label directly: `(N),Y`
// becomes `name,Y` (`IndexedData`) and the constant-Y `IndirectZp(N, k)`
// becomes `Data(name, k)`. That's a local array of a function with a
// private pool, which has a fixed address, as Zork's `print_number`
// keeps its digits in (examples/zork/numbers.c). The pointer's stores
// then go when nothing else reads them. A read of a stage slot whose
// source is a recomputable `Imm`, `Data`, or `IndexedData` reads the
// source, as `apply_remat` does within a block.
//
// The lattice
//
// The state at a point holds
//
//   - `a_value`, what `Reg(A)` holds, when it is an `Expr` the pass
//     follows, and nothing when it isn't known,
//   - `cells`, what each followed zero-page byte holds, by address,
//   - `x_token` and `y_token`, the identity of the value in X and in Y
//     (see `IndexedData` below).
//
// A `ZPRef(X)` fact for byte K says that K's value equals X's, as long
// as neither K nor X has been written since the fact was made. The other
// expressions are an 8-bit immediate (never ended), the low or high byte
// of a label's link-time address (never ended), the byte of a static
// outside zero page (`Data`, ended by a write of that byte), and an
// element `name + offset + index_reg` of such a static, with the index
// register's token when the fact was made, which has to be the token at
// the use.
//
// A join intersects the facts: one survives the join when every
// predecessor's out-state agrees on it. An unvisited state stands for
// every possible fact, and joining it with a state gives the state.
//
// What ends facts
//
// A write of zero-page byte W ends `cells[W]`, every `cells[K]` whose
// expression is W's old value, and `a_value` when it is. An indirect or
// unknown write (`Frame`, `Indirect*`, `Stack`, or an `IndexedData`
// whose range can't be bounded) ends every fact in `cells` and
// `a_value`.
//
// A `Call` to a function whose effects aren't known ends everything.
// A call to a function of the program with a summary of what it and its
// callees may write (`call_summaries`) ends only the facts about those
// bytes, and A, X, and Y. `FunctionPrologue`, `AllocateStack`, and
// `LoadAddress` are compound atoms that lower to several moves, and end
// everything.
//
// What makes facts
//
// `Mov(src, Reg(A))` with `src` a stable zero-page byte at X sets
// `a_value` to `ZPRef(X)`. `Mov(Reg(A), dst)` with `dst` a zero-page
// byte at Y and `a_value` a known expression E sets `cells[Y]` to E. A
// memory-to-memory `Mov(src, dst)` with both in zero page, at X and Y,
// is emitted as `LDA src; STA dst`, so A becomes `ZPRef(X)` and
// `cells[Y]` becomes `ZPRef(X)` too. A `Mov(Imm(_), ...)` or any other
// write of A sets `a_value` to what it writes, or to nothing.
//
// The zero-page address of an operand
//
//   - `ZP(addr, off)` is at `addr + off`.
//   - `Data(name, off)` is at `zp_symbol_addrs[name] + off` when the
//     name resolves to a byte at $FF or below.
//   - Any other operand isn't a followed zero-page byte.
//
// Reads of a slot that copies a fixed byte
//
// A read of one of the function's `__local_*` slots whose fact is a
// `ZPRef` to a byte named by a symbol the linker doesn't move reads that
// byte instead (`fixed_source`), when every read reached by the slot's
// stores can (`allowed_slot_reads`), so that the stores go. A first walk
// of the rewriter only records the candidates.
//
// Reads that cost more
//
// A fact may give a slot's value as an absolute `Data` byte or an
// `abs,X` / `abs,Y` element, whose load takes 3 bytes and a cycle more
// than the zero-page slot's 2. Such a read replaces the slot's only when
// the store into the slot goes with it: the slot must be one of the
// function's `__local_*` slots, every read its stores reach must be
// rewritten, and at most two of them may take the longer form
// (`dearer_read`, `allowed_slot_reads`). A zero-page byte or an
// immediate costs what the slot costs, and replaces it at every read.
//
// The identity of X and Y
//
// The token of a write of X or Y is its 1-based position in the
// function's instructions, and the entry state's tokens are 0. A
// rewritten instruction has no token.
//
// Where it runs
//
// After `replace_program_bare_exit`, so the operands are concrete, and
// inside the peephole fixed point. It overlaps with `apply_remat`'s
// rewrites of `Imm`, `Data`, and `ImmLabel` sources.
#include "passes/memory_value_propagation.h"

#include <algorithm>
#include <deque>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "errors.h"
#include "passes/asm_aliasing.h"
#include "passes/asm_dead_store.h"
#include "passes/asm_liveness.h"
#include "passes/decimal_arith.h"
#include "passes/optimization_asm/cfg.h"
#include "passes/redundant_load.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using K = ir::Kind_instruction;

constexpr int kMaxDearerReads = 2;

bool is_slot_name(std::string_view name) {
  return name.starts_with("__local_") || name.starts_with("__zpabi_");
}

bool is_reg_a_op(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

struct Expr {
  enum Kind { kZPRef, kImm, kLabelLow, kLabelHigh, kData, kIndexed };
  Kind kind = kZPRef;
  long long addr = 0;
  bool has_name = false;
  std::string name;
  asdl::Int offset = 0;
  asdl::Int value = 0;
  bool idx_is_x = false;
  long long idx_token = 0;

  friend bool operator==(const Expr& a, const Expr& b) {
    if (a.kind != b.kind) {
      return false;
    }
    switch (a.kind) {
      case kZPRef:
        return a.addr == b.addr;
      case kImm:
        return a.value == b.value;
      case kLabelLow:
      case kLabelHigh:
      case kData:
        return a.name == b.name && a.offset == b.offset;
      case kIndexed:
        return a.name == b.name && a.offset == b.offset &&
               a.idx_is_x == b.idx_is_x && a.idx_token == b.idx_token;
    }
    return false;
  }
};

Expr zp_ref(long long addr) { return Expr{.kind = Expr::kZPRef, .addr = addr}; }

struct State {
  std::optional<Expr> a_value;
  std::map<long long, Expr> cells;
  std::optional<long long> x_token;
  std::optional<long long> y_token;

  bool operator==(const State&) const = default;
};

struct Summary {
  std::set<long long> zp;
  std::set<std::pair<std::string, std::optional<asdl::Int>>> data;
};

using Summaries = std::map<std::string, Summary, std::less<>>;

std::optional<long long> zp_addr(const ir::Type_operand& op, const SymbolAddrs& zp_addrs) {
  if (const auto* z = as<ir::ZP>(op)) {
    return static_cast<long long>(z->address + z->offset);
  }
  if (const auto* d = as<ir::Data>(op)) {
    const auto it = zp_addrs.find(d->name);
    if (it == zp_addrs.end()) {
      return std::nullopt;
    }
    const asdl::Int addr = it->second + d->offset;
    if (addr > 0xFF) {
      return std::nullopt;
    }
    return static_cast<long long>(addr);
  }
  return std::nullopt;
}

const ir::Type_operand* instr_write_dst(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Mov:
      return &std::get<ir::Mov>(instr).dst;
    case K::Add:
      return &std::get<ir::Add>(instr).dst;
    case K::Sub:
      return &std::get<ir::Sub>(instr).dst;
    case K::And:
      return &std::get<ir::And>(instr).dst;
    case K::Or:
      return &std::get<ir::Or>(instr).dst;
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
    case K::Pop:
      return &std::get<ir::Pop>(instr).dst;
    default:
      return nullptr;
  }
}

std::optional<Summary> own_writes(const ir::Function& fn, const SymbolAddrs& zp_addrs) {
  Summary out;
  for (const auto& ins : fn.instructions) {
    if (isa<ir::FunctionPrologue, ir::AllocateStack, ir::LoadAddress, ir::Ret, ir::Phi,
            ir::DecimalArith>(ins)) {
      return std::nullopt;
    }
    const ir::Type_operand* dst = instr_write_dst(ins);
    if (dst == nullptr || isa<ir::Reg>(*dst)) {
      continue;
    }
    if (isa<ir::ZP, ir::Data>(*dst)) {
      if (const auto addr = zp_addr(*dst, zp_addrs)) {
        out.zp.insert(*addr);
      } else if (const auto* d = as<ir::Data>(*dst)) {
        if (zp_addrs.find(d->name) != zp_addrs.end()) {
          return std::nullopt;
        }
        out.data.emplace(d->name, d->offset);
      }
      continue;
    }
    if (const auto* idx = as<ir::IndexedData>(*dst);
        idx != nullptr && !idx->name.empty() && zp_addrs.find(idx->name) == zp_addrs.end()) {
      out.data.emplace(idx->name, std::nullopt);
      continue;
    }
    return std::nullopt;
  }
  return out;
}

Summaries call_summaries(const ir::Program& prog, const SymbolAddrs& zp_addrs,
                         const NameSet& noreturn) {
  std::map<std::string, const ir::Function*, std::less<>> fns;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      fns[fn->name] = fn;
    }
  }
  std::map<std::string, std::optional<Summary>, std::less<>> own;
  std::map<std::string, std::set<std::string>, std::less<>> calls;
  for (const auto& [name, fn] : fns) {
    own[name] = own_writes(*fn, zp_addrs);
    auto& callees = calls[name];
    std::set<std::string> labels;
    for (const auto& ins : fn->instructions) {
      if (const auto* l = as<ir::Label>(ins)) {
        labels.insert(l->name);
      }
    }
    for (const auto& ins : fn->instructions) {
      std::string callee;
      if (const auto* call = as<ir::Call>(ins)) {
        callee = call->name;
      } else if (const auto* jump = as<ir::Jump>(ins);
                 jump != nullptr && labels.count(jump->target) == 0) {
        callee = jump->target;
      } else {
        continue;
      }
      if (noreturn.find(callee) != noreturn.end()) {
        continue;
      }
      if (fns.find(callee) == fns.end()) {
        own[name].reset();
      }
      callees.insert(callee);
    }
  }
  std::set<std::string> known;
  for (const auto& [name, s] : own) {
    if (s) {
      known.insert(name);
    }
  }
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto it = known.begin(); it != known.end();) {
      const auto& callees = calls[*it];
      if (std::any_of(callees.begin(), callees.end(),
                      [&](const std::string& c) { return known.count(c) == 0; })) {
        it = known.erase(it);
        changed = true;
      } else {
        ++it;
      }
    }
  }
  Summaries out;
  for (const auto& n : known) {
    Summary sum;
    std::set<std::string> seen;
    std::vector<std::string> stack = {n};
    while (!stack.empty()) {
      const std::string f = stack.back();
      stack.pop_back();
      if (!seen.insert(f).second) {
        continue;
      }
      const Summary& s = *own[f];
      sum.zp.insert(s.zp.begin(), s.zp.end());
      sum.data.insert(s.data.begin(), s.data.end());
      for (const auto& c : calls[f]) {
        stack.push_back(c);
      }
    }
    out[n] = std::move(sum);
  }
  return out;
}

bool writes_index_reg(const ir::Type_instruction& instr, bool y) {
  if (y && sets_up_y(instr)) {
    return true;
  }
  const ir::Type_operand* dst = nullptr;
  if (const auto* mov = as<ir::Mov>(instr)) {
    dst = &mov->dst;
  } else if (const auto* inc = as<ir::Inc>(instr)) {
    dst = &inc->dst;
  } else if (const auto* dec = as<ir::Dec>(instr)) {
    dst = &dec->dst;
  } else if (const auto* pop = as<ir::Pop>(instr)) {
    dst = &pop->dst;
  }
  if (dst == nullptr) {
    return false;
  }
  const auto* r = as<ir::Reg>(*dst);
  return r != nullptr && (y ? isa<ir::Y>(r->reg) : isa<ir::X>(r->reg));
}

using ReadKey = std::pair<std::size_t, long long>;

struct Ctx {
  const AsmContext& actx;
  const SymbolAddrs& zp_addrs;
  NameSet writable;
  std::vector<long long> x_tokens;
  std::vector<long long> y_tokens;
  const Summaries& summaries;
  bool probing = false;
  std::set<ReadKey> probe;
  std::set<ReadKey> allowed;
  std::set<ReadKey> dearer;
  std::size_t index = 0;
};

void kill_cell(long long addr, State& state) {
  state.cells.erase(addr);
  for (auto it = state.cells.begin(); it != state.cells.end();) {
    if (it->second.kind == Expr::kZPRef && it->second.addr == addr) {
      it = state.cells.erase(it);
    } else {
      ++it;
    }
  }
  if (state.a_value && state.a_value->kind == Expr::kZPRef && state.a_value->addr == addr) {
    state.a_value.reset();
  }
}

void kill_data_name(const std::string& name, const std::optional<asdl::Int>& offset,
                    State& state) {
  const auto matches = [&](const Expr& e) {
    return e.kind == Expr::kData && e.name == name && (!offset || e.offset == *offset);
  };
  for (auto it = state.cells.begin(); it != state.cells.end();) {
    if (matches(it->second)) {
      it = state.cells.erase(it);
    } else {
      ++it;
    }
  }
  if (state.a_value && matches(*state.a_value)) {
    state.a_value.reset();
  }
}

void kill_writes(const ir::Type_operand& dst, State& state, const SymbolAddrs& zp_addrs) {
  if (isa<ir::ZP, ir::Data>(dst)) {
    const auto addr = zp_addr(dst, zp_addrs);
    if (!addr) {
      if (const auto* d = as<ir::Data>(dst)) {
        kill_data_name(d->name, d->offset, state);
      }
      return;
    }
    kill_cell(*addr, state);
    return;
  }
  state.cells.clear();
  state.a_value.reset();
}

void kill_stale_index_facts(State& state, bool is_x) {
  const std::optional<long long> current = is_x ? state.x_token : state.y_token;
  const auto stale = [&](const Expr& e) {
    return e.kind == Expr::kIndexed && e.idx_is_x == is_x &&
           (!current || e.idx_token != *current);
  };
  for (auto it = state.cells.begin(); it != state.cells.end();) {
    if (stale(it->second)) {
      it = state.cells.erase(it);
    } else {
      ++it;
    }
  }
  if (state.a_value && stale(*state.a_value)) {
    state.a_value.reset();
  }
}

void bump_index_tokens(std::optional<std::size_t> pos, State& state, const Ctx& ctx) {
  if (!pos || *pos >= ctx.x_tokens.size()) {
    return;
  }
  if (const long long t = ctx.x_tokens[*pos]; t != 0) {
    state.x_token = t;
    kill_stale_index_facts(state, true);
  }
  if (const long long t = ctx.y_tokens[*pos]; t != 0) {
    state.y_token = t;
    kill_stale_index_facts(state, false);
  }
}

std::optional<Expr> src_expr(const ir::Type_operand& src, const State& state,
                             const Ctx& ctx) {
  switch (ir::kind(src)) {
    case ir::Kind_operand::Imm:
      return Expr{.kind = Expr::kImm, .value = std::get<ir::Imm>(src).value};
    case ir::Kind_operand::ImmLabelLow: {
      const auto& l = std::get<ir::ImmLabelLow>(src);
      return Expr{.kind = Expr::kLabelLow, .name = l.name, .offset = l.offset};
    }
    case ir::Kind_operand::ImmLabelHigh: {
      const auto& h = std::get<ir::ImmLabelHigh>(src);
      return Expr{.kind = Expr::kLabelHigh, .name = h.name, .offset = h.offset};
    }
    case ir::Kind_operand::ZP: {
      const auto& z = std::get<ir::ZP>(src);
      return zp_ref(static_cast<long long>(z.address + z.offset));
    }
    case ir::Kind_operand::Data: {
      const auto& d = std::get<ir::Data>(src);
      if (const auto zp = zp_addr(src, ctx.zp_addrs)) {
        return Expr{.kind = Expr::kZPRef,
                    .addr = *zp,
                    .has_name = true,
                    .name = d.name,
                    .offset = d.offset};
      }
      if (ctx.writable.find(d.name) != ctx.writable.end()) {
        return std::nullopt;
      }
      return Expr{.kind = Expr::kData, .name = d.name, .offset = d.offset};
    }
    case ir::Kind_operand::IndexedData: {
      const auto& x = std::get<ir::IndexedData>(src);
      if (ctx.writable.find(x.name) != ctx.writable.end()) {
        return std::nullopt;
      }
      const bool is_x = isa<ir::X>(x.index);
      const auto token = is_x ? state.x_token : state.y_token;
      if (!token) {
        return std::nullopt;
      }
      return Expr{.kind = Expr::kIndexed,
                  .name = x.name,
                  .offset = x.offset,
                  .idx_is_x = is_x,
                  .idx_token = *token};
    }
    default:
      return std::nullopt;
  }
}

void transfer_mov(const ir::Mov& instr, State& state, const Ctx& ctx) {
  const ir::Type_operand& src = instr.src;
  const ir::Type_operand& dst = instr.dst;
  std::optional<Expr> sexpr = src_expr(src, state, ctx);
  const std::optional<long long> dst_opt = zp_addr(dst, ctx.zp_addrs);
  bool dst_followed = dst_opt.has_value();
  const long long dst_zp = dst_opt.value_or(0);
  if (instr.is_volatile) {
    sexpr.reset();
    dst_followed = false;
  }
  const bool src_is_reg = isa<ir::Reg>(src);
  bool a_clobbered = false;
  std::optional<Expr> new_a;
  if (const auto* r = as<ir::Reg>(dst)) {
    if (isa<ir::A>(r->reg)) {
      if (!src_is_reg) {
        new_a = sexpr;
      }
      a_clobbered = true;
    }
  } else if (!src_is_reg) {
    new_a = sexpr;
    a_clobbered = true;
  }
  if (!isa<ir::Reg>(dst)) {
    kill_writes(dst, state, ctx.zp_addrs);
  }
  if (a_clobbered) {
    state.a_value = new_a;
  }
  if (!dst_followed) {
    return;
  }
  std::optional<Expr> fact;
  if (src_is_reg) {
    if (is_reg_a_op(src)) {
      fact = state.a_value;
    }
  } else {
    fact = sexpr;
  }
  if (fact) {
    if (fact->kind == Expr::kZPRef && fact->addr == dst_zp) {
      return;
    }
    state.cells[dst_zp] = *fact;
  }
}

void transfer(const ir::Type_instruction& instr, std::optional<std::size_t> pos,
              State& state, const Ctx& ctx) {
  if (const auto* call = as<ir::Call>(instr)) {
    const auto it = ctx.summaries.find(call->name);
    if (it != ctx.summaries.end()) {
      state.a_value.reset();
      state.x_token.reset();
      state.y_token.reset();
      for (const long long addr : it->second.zp) {
        kill_cell(addr, state);
      }
      for (const auto& [name, offset] : it->second.data) {
        kill_data_name(name, offset, state);
      }
      return;
    }
  }
  switch (ir::kind(instr)) {
    case K::Call:
    case K::FunctionPrologue:
    case K::AllocateStack:
    case K::LoadAddress:
    case K::Phi:
    case K::DecimalArith:
      state.a_value.reset();
      state.cells.clear();
      state.x_token.reset();
      state.y_token.reset();
      return;
    case K::Mov:
      transfer_mov(std::get<ir::Mov>(instr), state, ctx);
      bump_index_tokens(pos, state, ctx);
      return;
    case K::Add:
    case K::Sub:
    case K::And:
    case K::Or:
    case K::Xor: {
      const ir::Type_operand& dst = *instr_write_dst(instr);
      if (is_reg_a_op(dst)) {
        state.a_value.reset();
      } else {
        kill_writes(dst, state, ctx.zp_addrs);
      }
      bump_index_tokens(pos, state, ctx);
      return;
    }
    case K::Inc:
    case K::Dec:
    case K::ArithmeticShiftLeft:
    case K::LogicalShiftRight:
    case K::RotateLeft:
    case K::RotateRight:
    case K::Pop: {
      const ir::Type_operand& dst = *instr_write_dst(instr);
      if (isa<ir::Reg>(dst)) {
        if (is_reg_a_op(dst)) {
          state.a_value.reset();
        } else {
          bump_index_tokens(pos, state, ctx);
        }
        return;
      }
      kill_writes(dst, state, ctx.zp_addrs);
      bump_index_tokens(pos, state, ctx);
      return;
    }
    case K::Compare:
    case K::Push:
      bump_index_tokens(pos, state, ctx);
      return;
    default:
      return;
  }
}

State transfer_block(const Cfg& cfg, int id, const State& in, const Ctx& ctx) {
  State state = in;
  const BasicBlock& block = cfg.blocks[static_cast<std::size_t>(id)];
  for (std::size_t p = block.begin; p < block.end; ++p) {
    transfer(cfg.instruction(p), p, state, ctx);
  }
  return state;
}

std::optional<State> meet(const std::optional<State>& a, const std::optional<State>& b) {
  if (!a) {
    return b;
  }
  if (!b) {
    return a;
  }
  State out;
  if (a->a_value == b->a_value) {
    out.a_value = a->a_value;
  }
  if (a->x_token == b->x_token) {
    out.x_token = a->x_token;
  }
  if (a->y_token == b->y_token) {
    out.y_token = a->y_token;
  }
  for (const auto& [k, v] : a->cells) {
    const auto it = b->cells.find(k);
    if (it != b->cells.end() && it->second == v) {
      out.cells.emplace(k, v);
    }
  }
  return out;
}

std::vector<std::optional<State>> solve(const Cfg& cfg, const Ctx& ctx) {
  const std::size_t n = cfg.blocks.size();
  std::vector<std::optional<State>> in_state(n);
  std::vector<std::optional<State>> out_state(n);
  in_state[kEntryId] = State{.x_token = 0, .y_token = 0};
  out_state[kEntryId] = State{.x_token = 0, .y_token = 0};
  std::deque<int> worklist(cfg.block_order.begin(), cfg.block_order.end());
  worklist.push_back(kExitId);
  std::vector<bool> in_worklist(n, false);
  for (const int b : worklist) {
    in_worklist[static_cast<std::size_t>(b)] = true;
  }
  std::vector<bool> seen(n, false);
  std::size_t iterations = 0;
  const std::size_t cap = std::max<std::size_t>(1024, n * 16);
  while (!worklist.empty()) {
    ++iterations;
    if (iterations > cap) {
      throw InternalError(
          "RuntimeError: memory_value_propagation: worklist did not converge after " +
          std::to_string(cap) + " iterations");
    }
    const int bid = worklist.front();
    worklist.pop_front();
    const auto b = static_cast<std::size_t>(bid);
    in_worklist[b] = false;
    if (bid == kEntryId) {
      continue;
    }
    std::optional<State> new_in;
    for (const int p : cfg.blocks[b].predecessors) {
      new_in = meet(new_in, out_state[static_cast<std::size_t>(p)]);
    }
    if (!new_in) {
      continue;
    }
    const bool first_visit = !seen[b];
    if (!first_visit && new_in == in_state[b]) {
      continue;
    }
    seen[b] = true;
    in_state[b] = new_in;
    State new_out = transfer_block(cfg, bid, *new_in, ctx);
    if (!out_state[b] || !(new_out == *out_state[b])) {
      out_state[b] = std::move(new_out);
      for (const int s : cfg.blocks[b].successors) {
        if (!in_worklist[static_cast<std::size_t>(s)]) {
          worklist.push_back(s);
          in_worklist[static_cast<std::size_t>(s)] = true;
        }
      }
    }
  }
  return in_state;
}

bool new_read_in_zero_page(const std::string& name, const asdl::Int& offset, const Ctx& ctx) {
  const auto it = ctx.zp_addrs.find(name);
  if (it != ctx.zp_addrs.end()) {
    return it->second + offset <= 0xFF;
  }
  return ctx.actx.is_user_zp_global(name);
}

bool is_local_slot_data(const ir::Type_operand& op) {
  const auto* d = as<ir::Data>(op);
  return d != nullptr && d->name.starts_with("__local_");
}

std::optional<ir::Type_operand> dearer_read(const ir::Type_operand& op,
                                            ir::Type_operand new_op,
                                            const std::string& name,
                                            const asdl::Int& offset, long long addr,
                                            Ctx& ctx) {
  if (new_read_in_zero_page(name, offset, ctx)) {
    return new_op;
  }
  if (!is_local_slot_data(op)) {
    return std::nullopt;
  }
  const ReadKey key{ctx.index, addr};
  if (ctx.probing) {
    ctx.probe.insert(key);
    ctx.dearer.insert(key);
    return std::nullopt;
  }
  if (ctx.allowed.count(key) == 0) {
    return std::nullopt;
  }
  return new_op;
}

std::optional<ir::Type_operand> fixed_source(const ir::Type_operand& op, const Expr& fact,
                                             long long addr, Ctx& ctx) {
  if (!is_local_slot_data(op)) {
    return std::nullopt;
  }
  if (!fact.has_name || is_slot_name(fact.name)) {
    return std::nullopt;
  }
  const ReadKey key{ctx.index, addr};
  if (ctx.probing) {
    ctx.probe.insert(key);
    return std::nullopt;
  }
  if (ctx.allowed.count(key) == 0) {
    return std::nullopt;
  }
  return ir::Data{.name = fact.name, .offset = fact.offset};
}

std::optional<ir::Type_operand> rewrite_src(const ir::Type_operand& op, const State& state,
                                            Ctx& ctx, bool allow_indexed) {
  const auto addr = zp_addr(op, ctx.zp_addrs);
  if (!addr) {
    return std::nullopt;
  }
  const auto it = state.cells.find(*addr);
  if (it == state.cells.end()) {
    return std::nullopt;
  }
  const Expr& fact = it->second;
  switch (fact.kind) {
    case Expr::kZPRef:
      return fixed_source(op, fact, *addr, ctx);
    case Expr::kImm:
      return ir::Imm{.value = fact.value};
    case Expr::kLabelLow:
      return ir::ImmLabelLow{.name = fact.name, .offset = fact.offset};
    case Expr::kLabelHigh:
      return ir::ImmLabelHigh{.name = fact.name, .offset = fact.offset};
    case Expr::kData:
      return dearer_read(op, ir::Data{.name = fact.name, .offset = fact.offset}, fact.name,
                         fact.offset, *addr, ctx);
    case Expr::kIndexed: {
      if (!allow_indexed) {
        return std::nullopt;
      }
      const auto current = fact.idx_is_x ? state.x_token : state.y_token;
      if (!current || *current != fact.idx_token) {
        return std::nullopt;
      }
      ir::Type_reg idx_reg = fact.idx_is_x ? ir::Type_reg{ir::X{}} : ir::Type_reg{ir::Y{}};
      return dearer_read(
          op, ir::IndexedData{.name = fact.name, .offset = fact.offset, .index = idx_reg},
          fact.name, fact.offset, *addr, ctx);
    }
  }
  return std::nullopt;
}

template <class Fn>
bool rewrite_reads(ir::Type_instruction& instr, Fn&& fn, bool push) {
  switch (ir::kind(instr)) {
    case K::Add: {
      auto& i = std::get<ir::Add>(instr);
      if (auto n = fn(i.src)) {
        i.src = std::move(*n);
        return true;
      }
      return false;
    }
    case K::Sub: {
      auto& i = std::get<ir::Sub>(instr);
      if (auto n = fn(i.src)) {
        i.src = std::move(*n);
        return true;
      }
      return false;
    }
    case K::And: {
      auto& i = std::get<ir::And>(instr);
      if (auto n = fn(i.src)) {
        i.src = std::move(*n);
        return true;
      }
      return false;
    }
    case K::Or: {
      auto& i = std::get<ir::Or>(instr);
      if (auto n = fn(i.src)) {
        i.src = std::move(*n);
        return true;
      }
      return false;
    }
    case K::Xor: {
      auto& i = std::get<ir::Xor>(instr);
      auto n1 = fn(i.src1);
      auto n2 = fn(i.src2);
      if (n1) {
        i.src1 = std::move(*n1);
      }
      if (n2) {
        i.src2 = std::move(*n2);
      }
      return n1 || n2;
    }
    case K::Compare: {
      auto& i = std::get<ir::Compare>(instr);
      if (auto n = fn(i.right)) {
        i.right = std::move(*n);
        return true;
      }
      return false;
    }
    case K::Push: {
      if (!push) {
        return false;
      }
      auto& i = std::get<ir::Push>(instr);
      if (auto n = fn(i.src)) {
        i.src = std::move(*n);
        return true;
      }
      return false;
    }
    default:
      return false;
  }
}

template <class Fn>
void rewrite_operands(ir::Type_instruction& instr, Fn&& fn) {
  if (auto* mov = as<ir::Mov>(instr)) {
    std::optional<ir::Type_operand> new_src;
    std::optional<ir::Type_operand> new_dst;
    const auto* dst_reg = as<ir::Reg>(mov->dst);
    const auto* src_reg = as<ir::Reg>(mov->src);
    if (!(dst_reg != nullptr && isa<ir::Y>(dst_reg->reg))) {
      new_src = fn(mov->src);
    }
    if (!(src_reg != nullptr && isa<ir::X, ir::Y>(src_reg->reg))) {
      new_dst = fn(mov->dst);
    }
    const ir::Type_operand& s = new_src ? *new_src : mov->src;
    const ir::Type_operand& d = new_dst ? *new_dst : mov->dst;
    if (isa<ir::IndexedData>(s) && isa<ir::IndexedData>(d)) {
      return;
    }
    if (new_src) {
      mov->src = std::move(*new_src);
    }
    if (new_dst) {
      mov->dst = std::move(*new_dst);
    }
    return;
  }
  rewrite_reads(instr, fn, false);
}

std::optional<ir::Type_operand> label_pointer_operand(const ir::Type_operand& op,
                                                      const State& state,
                                                      const AsmContext& actx) {
  long long base = 0;
  bool y_form = false;
  asdl::Int k = 0;
  if (const auto* ind = as<ir::Indirect>(op)) {
    base = static_cast<long long>(actx.layout.dptr);
    k = ind->offset;
  } else if (isa<ir::IndirectY>(op)) {
    base = static_cast<long long>(actx.layout.dptr);
    y_form = true;
  } else if (const auto* zp = as<ir::IndirectZp>(op)) {
    base = static_cast<long long>(zp->address);
    k = zp->offset;
  } else if (const auto* zpy = as<ir::IndirectZpY>(op)) {
    base = static_cast<long long>(zpy->address);
    y_form = true;
  } else {
    return std::nullopt;
  }
  const auto lo = state.cells.find(base);
  const auto hi = state.cells.find(base + 1);
  if (lo == state.cells.end() || hi == state.cells.end()) {
    return std::nullopt;
  }
  if (!(lo->second.kind == Expr::kLabelLow && hi->second.kind == Expr::kLabelHigh &&
        lo->second.name == hi->second.name && lo->second.offset == hi->second.offset)) {
    return std::nullopt;
  }
  if (y_form) {
    return ir::IndexedData{.name = lo->second.name, .offset = lo->second.offset,
                           .index = ir::Y{}};
  }
  return ir::Data{.name = lo->second.name, .offset = lo->second.offset + k};
}

std::optional<ir::Type_operand> copied_pointer_operand(const ir::Type_operand& op,
                                                       const State& state) {
  long long base = 0;
  if (const auto* zp = as<ir::IndirectZp>(op)) {
    base = static_cast<long long>(zp->address);
  } else if (const auto* zpy = as<ir::IndirectZpY>(op)) {
    base = static_cast<long long>(zpy->address);
  } else {
    return std::nullopt;
  }
  const auto lo = state.cells.find(base);
  const auto hi = state.cells.find(base + 1);
  if (lo == state.cells.end() || hi == state.cells.end()) {
    return std::nullopt;
  }
  const Expr& l = lo->second;
  const Expr& h = hi->second;
  if (l.kind != Expr::kZPRef || h.kind != Expr::kZPRef) {
    return std::nullopt;
  }
  if (h.addr != l.addr + 1 || l.addr == base) {
    return std::nullopt;
  }
  if (!l.has_name || is_slot_name(l.name) || !h.has_name || is_slot_name(h.name)) {
    return std::nullopt;
  }
  if (const auto* zp = as<ir::IndirectZp>(op)) {
    return ir::IndirectZp{.address = l.addr, .offset = zp->offset};
  }
  return ir::IndirectZpY{.address = l.addr};
}

std::optional<long long> dptr_base(const State& state, const AsmContext& actx) {
  const long long dptr = static_cast<long long>(actx.layout.dptr);
  const auto lo = state.cells.find(dptr);
  const auto hi = state.cells.find(dptr + 1);
  if (lo == state.cells.end() || hi == state.cells.end()) {
    return std::nullopt;
  }
  if (lo->second.kind != Expr::kZPRef || hi->second.kind != Expr::kZPRef) {
    return std::nullopt;
  }
  if (hi->second.addr != lo->second.addr + 1) {
    return std::nullopt;
  }
  return lo->second.addr;
}

std::optional<ir::Type_operand> op_for_dptr(const ir::Type_operand& op, long long base) {
  if (const auto* ind = as<ir::Indirect>(op)) {
    return ir::IndirectZp{.address = base, .offset = ind->offset};
  }
  if (isa<ir::IndirectY>(op)) {
    return ir::IndirectZpY{.address = base};
  }
  return std::nullopt;
}

void rewrite_instr(ir::Type_instruction& instr, const State& state, Ctx& ctx) {
  if (const auto base = dptr_base(state, ctx.actx)) {
    if (auto* mov = as<ir::Mov>(instr)) {
      if (auto s = op_for_dptr(mov->src, *base)) {
        mov->src = std::move(*s);
      }
      if (auto d = op_for_dptr(mov->dst, *base)) {
        mov->dst = std::move(*d);
      }
    } else {
      rewrite_reads(instr, [&](const ir::Type_operand& op) { return op_for_dptr(op, *base); },
                    false);
    }
  }
  rewrite_operands(instr, [&](const ir::Type_operand& op) {
    return label_pointer_operand(op, state, ctx.actx);
  });
  rewrite_operands(instr,
                   [&](const ir::Type_operand& op) { return copied_pointer_operand(op, state); });
  if (auto* mov = as<ir::Mov>(instr)) {
    auto new_src = rewrite_src(mov->src, state, ctx, true);
    if (!new_src) {
      return;
    }
    // The 6502 has no `LDX abs,X` or `LDY abs,Y`.
    if (const auto* x = as<ir::IndexedData>(*new_src)) {
      const auto* r = as<ir::Reg>(mov->dst);
      if (r != nullptr && r->reg.index() == x->index.index()) {
        return;
      }
    }
    mov->src = std::move(*new_src);
    return;
  }
  rewrite_reads(
      instr, [&](const ir::Type_operand& op) { return rewrite_src(op, state, ctx, false); },
      true);
}

bool is_through_pointer(const ir::Type_operand& op) {
  return isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY>(op);
}

bool store_made_direct(const ir::Type_instruction& old_instr,
                       const ir::Type_instruction& new_instr) {
  const auto* o = as<ir::Mov>(old_instr);
  const auto* n = as<ir::Mov>(new_instr);
  return o != nullptr && n != nullptr && is_through_pointer(o->dst) &&
         !is_through_pointer(n->dst);
}

std::vector<ir::Type_instruction> rewrite(const Cfg& cfg,
                                          const std::vector<std::optional<State>>& in_states,
                                          Ctx& ctx) {
  std::vector<ir::Type_instruction> out;
  for (const int bid : cfg.block_order) {
    const BasicBlock& block = cfg.blocks[static_cast<std::size_t>(bid)];
    const auto& bin = in_states[static_cast<std::size_t>(bid)];
    State state = bin ? *bin : State{};
    for (std::size_t p = block.begin; p < block.end; ++p) {
      const ir::Type_instruction& instr = cfg.instruction(p);
      ctx.index = out.size();
      ir::Type_instruction new_instr = instr;
      rewrite_instr(new_instr, state, ctx);
      if (store_made_direct(instr, new_instr)) {
        transfer(new_instr, std::nullopt, state, ctx);
      } else {
        transfer(instr, p, state, ctx);
      }
      out.push_back(std::move(new_instr));
    }
  }
  return out;
}

bool slot_accesses(const ir::Type_instruction& instr, const SymbolAddrs& zp_addrs,
                   std::set<long long>& read, std::set<long long>& written) {
  std::vector<ir::Type_operand> ops = asm_dead_store::read_operands(instr);
  const std::size_t n = ops.size();
  for (std::size_t i = 0; i < n; ++i) {
    for (auto& p : asm_dead_store::ptr_source_reads(ops[i])) {
      ops.push_back(std::move(p));
    }
  }
  for (const auto& op : ops) {
    if (const auto* x = as<ir::IndexedData>(op); x != nullptr && x->name.starts_with("__local_")) {
      return false;
    }
    if (const auto a = zp_addr(op, zp_addrs)) {
      read.insert(*a);
    }
  }
  if (const auto* d = as<ir::DecimalArith>(instr)) {
    for (const auto& w : decimal_arith::writes(*d)) {
      if (const auto a = zp_addr(w, zp_addrs)) {
        written.insert(*a);
      }
    }
    return true;
  }
  const auto* la = as<ir::LoadAddress>(instr);
  const ir::Type_operand* dst = la != nullptr ? &la->dst : instr_write_dst(instr);
  if (dst != nullptr) {
    if (const auto a = zp_addr(*dst, zp_addrs)) {
      written.insert(*a);
      if (la != nullptr) {
        written.insert(*a + 1);
      }
    }
  }
  return true;
}

class Webs {
 public:
  ReadKey find(ReadKey x) {
    parent_.try_emplace(x, x);
    ReadKey root = x;
    while (parent_.at(root) != root) {
      root = parent_.at(root);
    }
    while (x != root) {
      ReadKey& p = parent_.at(x);
      x = p;
      p = root;
    }
    return root;
  }
  void join(ReadKey a, ReadKey root) { parent_[find(a)] = root; }

 private:
  std::map<ReadKey, ReadKey> parent_;
};

std::set<ReadKey> allowed_slot_reads(const Cfg& cfg, const Ctx& ctx) {
  if (ctx.probe.empty()) {
    return {};
  }
  std::vector<const ir::Type_instruction*> instrs;
  std::vector<std::pair<std::size_t, std::size_t>> block_span(cfg.blocks.size());
  for (const int bid : cfg.block_order) {
    const BasicBlock& block = cfg.blocks[static_cast<std::size_t>(bid)];
    const std::size_t start = instrs.size();
    for (std::size_t p = block.begin; p < block.end; ++p) {
      instrs.push_back(&cfg.instruction(p));
    }
    block_span[static_cast<std::size_t>(bid)] = {start, instrs.size()};
  }
  for (const auto* i : instrs) {
    if (isa<ir::FunctionPrologue, ir::Ret>(*i)) {
      return {};
    }
  }
  std::set<long long> slots;
  for (const auto& [idx, addr] : ctx.probe) {
    slots.insert(addr);
  }
  std::vector<std::vector<long long>> reads(instrs.size());
  std::vector<std::vector<long long>> writes(instrs.size());
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    std::set<long long> r;
    std::set<long long> w;
    if (!slot_accesses(*instrs[i], ctx.zp_addrs, r, w)) {
      return {};
    }
    for (const long long a : r) {
      if (slots.count(a) != 0) {
        reads[i].push_back(a);
      }
    }
    for (const long long a : w) {
      if (slots.count(a) != 0) {
        writes[i].push_back(a);
      }
    }
  }

  using Defs = std::map<long long, std::set<std::size_t>>;
  const auto transfer_defs = [&](int bid, Defs state) {
    const auto [lo, hi] = block_span[static_cast<std::size_t>(bid)];
    for (std::size_t i = lo; i < hi; ++i) {
      for (const long long a : writes[i]) {
        state[a] = {i};
      }
    }
    return state;
  };
  std::vector<Defs> in_defs(cfg.blocks.size());
  std::vector<std::optional<Defs>> out_defs(cfg.blocks.size());
  bool changed = true;
  while (changed) {
    changed = false;
    for (const int bid : cfg.block_order) {
      Defs merged;
      for (const int p : cfg.blocks[static_cast<std::size_t>(bid)].predecessors) {
        if (const auto& od = out_defs[static_cast<std::size_t>(p)]) {
          for (const auto& [a, ds] : *od) {
            merged[a].insert(ds.begin(), ds.end());
          }
        }
      }
      in_defs[static_cast<std::size_t>(bid)] = merged;
      Defs next = transfer_defs(bid, std::move(merged));
      auto& od = out_defs[static_cast<std::size_t>(bid)];
      if (!od || *od != next) {
        od = std::move(next);
        changed = true;
      }
    }
  }

  Webs webs;
  std::vector<std::pair<ReadKey, std::optional<ReadKey>>> read_webs;
  for (const int bid : cfg.block_order) {
    const auto [lo, hi] = block_span[static_cast<std::size_t>(bid)];
    Defs state = in_defs[static_cast<std::size_t>(bid)];
    for (std::size_t i = lo; i < hi; ++i) {
      for (const long long a : reads[i]) {
        const auto it = state.find(a);
        if (it == state.end() || it->second.empty()) {
          read_webs.emplace_back(ReadKey{i, a}, std::nullopt);
          continue;
        }
        const std::set<std::size_t>& ds = it->second;
        const ReadKey first{*ds.begin(), a};
        const ReadKey root = webs.find(first);
        for (auto d = std::next(ds.begin()); d != ds.end(); ++d) {
          webs.join(ReadKey{*d, a}, root);
        }
        read_webs.emplace_back(ReadKey{i, a}, first);
      }
      for (const long long a : writes[i]) {
        state[a] = {i};
      }
    }
  }
  std::set<ReadKey> bad;
  std::map<ReadKey, int> dearer;
  for (const auto& [key, web] : read_webs) {
    if (web && ctx.probe.count(key) == 0) {
      bad.insert(webs.find(*web));
    }
    if (web && ctx.dearer.count(key) != 0) {
      ++dearer[webs.find(*web)];
    }
  }
  for (const auto& [root, n] : dearer) {
    if (n > kMaxDearerReads) {
      bad.insert(root);
    }
  }
  std::set<ReadKey> out;
  for (const auto& [key, web] : read_webs) {
    if (ctx.probe.count(key) != 0 && web && bad.count(webs.find(*web)) == 0) {
      out.insert(key);
    }
  }
  return out;
}

NameSet writable_data_names(const ir::Function& fn) {
  NameSet out;
  for (const auto& instr : fn.instructions) {
    if (const auto* d = as<ir::DecimalArith>(instr)) {
      for (const auto& w : decimal_arith::writes(*d)) {
        if (const auto* data = as<ir::Data>(w)) {
          out.insert(data->name);
        }
      }
      continue;
    }
    const ir::Type_operand* dst = instr_write_dst(instr);
    if (dst == nullptr) {
      continue;
    }
    if (const auto* data = as<ir::Data>(*dst)) {
      out.insert(data->name);
    } else if (const auto* idx = as<ir::IndexedData>(*dst)) {
      out.insert(idx->name);
    }
  }
  return out;
}

void rewrite_function(ir::Function& fn, const AsmContext& actx, const SymbolAddrs& zp_addrs,
                      const Summaries& summaries) {
  const Cfg cfg = build_cfg_tolerant(fn);
  Ctx ctx{.actx = actx, .zp_addrs = zp_addrs, .summaries = summaries};
  ctx.writable = writable_data_names(fn);
  const std::size_t n = fn.instructions.size();
  ctx.x_tokens.assign(n, 0);
  ctx.y_tokens.assign(n, 0);
  for (std::size_t i = 0; i < n; ++i) {
    if (writes_index_reg(fn.instructions[i], false)) {
      ctx.x_tokens[i] = static_cast<long long>(i) + 1;
    }
    if (writes_index_reg(fn.instructions[i], true)) {
      ctx.y_tokens[i] = static_cast<long long>(i) + 1;
    }
  }
  const auto in_states = solve(cfg, ctx);
  ctx.probing = true;
  rewrite(cfg, in_states, ctx);
  ctx.allowed = allowed_slot_reads(cfg, ctx);
  ctx.probing = false;
  std::vector<ir::Type_instruction> out = rewrite(cfg, in_states, ctx);
  out.resize(n);
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_memory_value_propagation(asm_ast::Program prog,
                                                const AsmContext& ctx,
                                                const SymbolAddrs* zp_symbol_addrs,
                                                const NameSet& noreturn) {
  SymbolAddrs addrs = runtime_zp_addrs(ctx);
  if (zp_symbol_addrs != nullptr) {
    for (const auto& [name, addr] : *zp_symbol_addrs) {
      addrs[name] = addr;
    }
  }
  const Summaries summaries = call_summaries(prog, addrs, noreturn);
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx, addrs, summaries);
    }
  }
  return prog;
}

}  // namespace c6502::passes
