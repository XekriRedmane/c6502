// The driver of the asm optimizer.
//
// It runs the asm SSA round trip on each `Function` of the program. It
// has the shape of the TAC optimizer's `optimize_program`, on an asm
// program with `Pseudo` operands, so it must run before
// `replace_pseudoregisters`. Each function goes through
//
//     fn -> to_ssa
//        -> [copy_propagate -> backward_copy_propagate -> byte_dce]*
//        -> liveness + interference + regalloc -> from_ssa -> fn'
//
// where the bracket runs to a fixed point, since each pass makes work
// for the others.
//
// - `copy_propagate` replaces the uses of a copy's `dst` with its `src`,
//   which leaves the `Mov` dead.
// - `backward_copy_propagate` removes a `Pseudo P` whose only role is
//   to bridge the round trip `Mov(Reg(A), P); ...; Mov(P, Reg(A));
//   Mov(Reg(A), D)`, by making the definition of P write D.
// - `byte_dce` drops the definitions nothing reads any more.
//
// That is the TAC optimizer's bracket of constant folding, unreachable
// code elimination, copy propagation, and dead store elimination, with
// the asm passes in their place.
//
// The program comes back with each function's `Coloring` (an empty one
// for a function where nothing was colored). compile.py and
// sim/harness.py give the colorings to
// `replace_pseudoregisters_bare_exit`, which lowers each `Pseudo(name, 0)`
// whose name is colored to `ZP(addr, 0)`. `StaticVariable`s pass through.
#include "passes/optimization_asm/optimizer.h"

#include <map>
#include <string>
#include <string_view>
#include <utility>

#include "errors.h"
#include "passes/asm_liveness.h"
#include "passes/optimization_asm/all_operands.h"
#include "passes/optimization_asm/apply_coloring.h"
#include "passes/optimization_asm/backward_copy_propagation.h"
#include "passes/optimization_asm/byte_dce.h"
#include "passes/optimization_asm/coalescing.h"
#include "passes/optimization_asm/const_static_fold.h"
#include "passes/optimization_asm/copy_propagation.h"
#include "passes/optimization_asm/dead_static.h"
#include "passes/optimization_asm/hwreg_eligibility.h"
#include "passes/optimization_asm/interference.h"
#include "passes/optimization_asm/liveness.h"
#include "passes/optimization_asm/or_zero_absorb.h"
#include "passes/optimization_asm/regalloc.h"
#include "passes/optimization_asm/ssa_destruction.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/sub1_test_zero_peephole.h"
#include "passes/zp_slot_naming.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using Hints = OrderedMap<std::string, std::string>;
using EntryLimits = OrderedMap<std::string, NameSet>;

const ParamLayout* layout_of(const Abi* param_layouts, const std::string& name) {
  return param_layouts == nullptr ? nullptr : param_layouts->find(name);
}

// Returns the zero-page addresses that the coloring of `fn`'s body must
// avoid.
//
// - The slots of its own incoming parameters, when it has a `ZpLayout`,
//   since the body's locals mustn't overwrite the bytes the caller wrote
//   there.
// - The parameter slots of every zp_abi callee, since a body local
//   mustn't live at an address that the writes of the arguments just
//   before each `Call` land on, which would overwrite it at the call.
//
// The addresses are blocked for the whole function, which is longer than
// the window of the argument writes or the parameter's lifetime.
// The few zero-page bytes wasted are usually small next to what removing
// the frame saves.
std::set<asdl::Int> blocked_addrs_for(const ir::Function& fn, const Abi* param_layouts) {
  std::set<asdl::Int> out;
  if (param_layouts == nullptr) {
    return out;
  }
  auto add = [&](const ParamLayout* layout) {
    const auto* zp = layout == nullptr ? nullptr : std::get_if<ZpLayout>(layout);
    if (zp == nullptr) {
      return;
    }
    for (const auto& a : zp->addrs) {
      if (a) {
        out.insert(*a);
      }
    }
  };
  add(param_layouts->find(fn.name));
  for (const auto& instr : fn.instructions) {
    if (const auto* call = as<ir::Call>(instr)) {
      add(param_layouts->find(call->name));
    }
  }
  return out;
}

// Returns `range(min, max + 1)` of `fn`'s private pool, which the
// allocator keeps contiguous, so a range is what `color_graph`'s
// `find_fit` reads. Returns nothing when the function has no pool, and
// the allocation then falls back to the split into caller-saved and
// callee-saved bytes.
std::optional<Range> allowed_range_for(
    const ir::Function& fn, const OrderedMap<std::string, std::vector<asdl::Int>>* local_pools) {
  if (local_pools == nullptr) {
    return std::nullopt;
  }
  const auto* pool = local_pools->find(fn.name);
  if (pool == nullptr || pool->empty()) {
    return std::nullopt;
  }
  return Range{.start = pool->front(), .stop = pool->back() + 1};
}

// Returns the names of the c99 IR (the resolved `@<N>.<orig>`
// spellings) with the `register` storage class, the `register` locals
// and parameters, in the symbol table's order. Empty without a table.
std::vector<std::string> register_var_names(const SymbolTable* symbols) {
  std::vector<std::string> out;
  if (symbols == nullptr) {
    return out;
  }
  for (const auto& [name, sym] : *symbols) {
    const auto* local = std::get_if<LocalAttr>(&sym.attrs);
    if (local != nullptr && local->register_) {
      out.push_back(name);
    }
  }
  return out;
}

// Returns the soft hints of the registers for `fn`'s 1-byte `register`
// parameters. Each maps to the register (A or X) the calling convention
// passes it in, so the allocation can keep the value there and the
// store to the slot at entry becomes dead. Only an X hint has an effect,
// since A isn't a register the allocation colors to, and
// `hwreg_eligibility` ignores the A hints. A `register` parameter of 2
// bytes can't be pinned to one register, so it gets none.
Hints param_register_hints(const ir::Function& fn, const ParamLayout* layout,
                           const SymbolTable* symbols) {
  Hints hints;
  const auto* zp = layout == nullptr ? nullptr : std::get_if<ZpLayout>(layout);
  if (zp == nullptr || symbols == nullptr) {
    return hints;
  }
  asdl::Int off = 0;
  for (const auto& pname : fn.params) {
    const Symbol* sym = symbols->find(pname);
    const asdl::Int n = sym != nullptr ? sizeof_type(sym->type, nullptr) : asdl::Int(1);
    const auto* local = sym == nullptr ? nullptr : std::get_if<LocalAttr>(&sym->attrs);
    const bool is_reg = local != nullptr && local->register_;
    if (is_reg && n == 1 && off < static_cast<asdl::Int>(zp->param_registers.size())) {
      const auto& reg = zp->param_registers[static_cast<std::size_t>(off)];
      // No hint for Y. A `register` local is the one value the
      // allocation keeps in Y, and the soft-stack prologue writes Y, so
      // a Y parameter is read from its slot.
      if (reg && *reg != "Y") {
        // The parameter's versions, and the local its value at entry is
        // copied into (`tac_to_asm`).
        hints.insert_or_assign(pname, *reg);
        hints.insert_or_assign(register_param_local(pname), *reg);
      }
    }
    off += n;
  }
  return hints;
}

// Returns the registers that `entry_register_limits` keeps the asm SSA
// name `name` (`<base>.b<k>.v<n>`) out of, or null for a name it doesn't
// limit. Python matches `^(.*?)\.b(\d+)\.v\d+$`, whose groups are the
// name before its last `.b<digits>.v<digits>` and the digits of the
// `.b`, since neither run of digits holds a dot.
const NameSet* entry_limit(const EntryLimits& entry_limits, std::string_view name) {
  auto is_digits = [](std::string_view s) {
    if (s.empty()) {
      return false;
    }
    for (const char c : s) {
      if (c < '0' || c > '9') {
        return false;
      }
    }
    return true;
  };
  const std::size_t v = name.rfind(".v");
  if (v == std::string_view::npos || !is_digits(name.substr(v + 2))) {
    return nullptr;
  }
  const std::string_view head = name.substr(0, v);
  const std::size_t b = head.rfind(".b");
  if (b == std::string_view::npos || !is_digits(head.substr(b + 2))) {
    return nullptr;
  }
  const std::string key = hwreg_eligibility::ssa_base(head.substr(0, b)) + ".b" +
                          std::string(head.substr(b + 2));
  return entry_limits.find(key);
}

// Throws when a colorable name is live across a call to one of
// `callees`, which reuses `fn`'s pool bytes.
void check_nothing_live_across(const ir::Function& fn, const Liveness& liveness,
                               const InterferenceGraph& graph, const NameSet& callees) {
  for (const auto& blk : liveness.cfg.blocks) {
    NameSet live;
    for (const auto& n : liveness.live_out[static_cast<std::size_t>(blk.id)]) {
      if (graph.nodes.contains(n)) {
        live.insert(n);
      }
    }
    const auto instrs = liveness.cfg.instructions(blk.id);
    for (auto it = instrs.rbegin(); it != instrs.rend(); ++it) {
      const ir::Type_instruction& instr = *it;
      if (isa<ir::Phi>(instr)) {
        continue;
      }
      for (const ir::Pseudo* d : liveness::defs_in(instr)) {
        live.erase(d->name);
      }
      const auto* call = as<ir::Call>(instr);
      if (call != nullptr && callees.contains(call->name) && !live.empty()) {
        std::string names = "[";
        bool first = true;
        for (const auto& n : live) {
          if (!first) {
            names += ", ";
          }
          first = false;
          names += pyrepr::string(n);
        }
        names += "]";
        throw InternalError("function '" + fn.name + "': " + names + " live across the call to '" +
                            call->name + "', which can recurse");
      }
      for (const ir::Pseudo* u : liveness::uses_in(instr)) {
        if (graph.nodes.contains(u->name)) {
          live.insert(u->name);
        }
      }
    }
  }
}

// Returns every pseudo name in `fn`'s instructions, for the projection
// of the eligibility.
NameSet all_pseudo_names(const ir::Function& fn) {
  NameSet out;
  for (const auto& instr : fn.instructions) {
    for (const ir::Type_operand* op : optimizer::all_operands_in(instr)) {
      if (const auto* p = as<ir::Pseudo>(*op)) {
        out.insert(p->name);
      }
    }
  }
  return out;
}

// Returns the eligibility of each name projected onto the
// representatives of the coalescing.
//
// A representative is in `eligible_x` when every member mapped to it is
// in `raw.eligible_x`, which is conservative, since one member that
// can't be pinned to X rules out the whole node, and likewise and on its
// own for `eligible_y`. Hints come from any member, so the
// representative is in `hints_x` when any member is, and the same for
// `hints_y` and `hints_carry`, and the hints are then restricted to the
// names eligible for some register. A name of `all_names` that the
// coalescing didn't merge is its own representative.
//
// Python keys `use_count` in the order of a walk over a set of names,
// which no reader follows, since the allocator only looks names up.
HwRegEligibility project_eligibility(const HwRegEligibility& raw, const CoalesceResult& coalesced,
                                     const NameSet& all_names) {
  std::map<std::string, NameSet, std::less<>> members_by_rep;
  for (const auto& name : all_names) {
    members_by_rep[coalesced.resolve(name)].insert(name);
  }
  HwRegEligibility out;
  for (const auto& [rep, members] : members_by_rep) {
    bool all_x = true;
    bool all_y = true;
    bool any_hx = false;
    bool any_hy = false;
    bool any_hc = false;
    bool any_rx = false;
    bool any_ry = false;
    asdl::Int uses = 0;
    for (const auto& m : members) {
      all_x = all_x && raw.eligible_x.contains(m);
      all_y = all_y && raw.eligible_y.contains(m);
      any_hx = any_hx || raw.hints_x.contains(m);
      any_hy = any_hy || raw.hints_y.contains(m);
      any_hc = any_hc || raw.hints_carry.contains(m);
      // A pin passes to the representative when any member has one.
      // Coalescing is symmetric, so a node that swallows a pinned member
      // is itself pinned. The allocator checks that the node is in the
      // matching eligible set, and when coalescing merged a pinned name
      // with one that can't take the register, it isn't, and the
      // allocator raises with a message that says so.
      any_rx = any_rx || raw.required_x.contains(m);
      any_ry = any_ry || raw.required_y.contains(m);
      // The use count is the sum over the members.
      if (const asdl::Int* c = raw.use_count.find(m)) {
        uses += *c;
      }
    }
    if (all_x) {
      out.eligible_x.insert(rep);
    }
    if (all_y) {
      out.eligible_y.insert(rep);
    }
    if (any_hx) {
      out.hints_x.insert(rep);
    }
    if (any_hy) {
      out.hints_y.insert(rep);
    }
    if (any_hc) {
      out.hints_carry.insert(rep);
    }
    if (any_rx) {
      out.required_x.insert(rep);
    }
    if (any_ry) {
      out.required_y.insert(rep);
    }
    out.use_count.insert_or_assign(rep, uses);
  }
  const NameSet eligible_any = out.eligible();
  auto restrict = [&](NameSet& s) {
    std::erase_if(s, [&](const std::string& n) { return !eligible_any.contains(n); });
  };
  restrict(out.hints_x);
  restrict(out.hints_y);
  restrict(out.hints_carry);
  for (const auto& [p, q] : raw.carry_pairs) {
    out.carry_pairs.emplace_back(coalesced.resolve(p), coalesced.resolve(q));
  }
  return out;
}

}  // namespace

namespace optimizer {

// The registers that each byte of the local a `register` parameter is
// copied into at entry (`tac_to_asm`'s `_register_param_locals`) mustn't
// be colored to, which are X or Y when another byte, of this parameter or
// another, arrives in it, since that byte's copy would overwrite the
// register before the other byte's copy reads it. A byte may take the
// register it arrives in, where its copy is a move of the register to
// itself, which emits nothing, and no other copy writes that register,
// since every other byte is kept out of it. The keys are
// `<local>.b<k>`, as `entry_limit` looks them up. A soft-stack prologue
// writes Y too, but it saves Y around itself when Y is live
// (`prologue_synthesis`).
EntryLimits entry_register_limits(const ir::Function& fn, const ParamLayout* layout,
                                  const SymbolTable* symbols) {
  EntryLimits out;
  const auto* zp = layout == nullptr ? nullptr : std::get_if<ZpLayout>(layout);
  if (zp == nullptr || symbols == nullptr) {
    return out;
  }
  NameSet incoming;
  for (const auto& r : zp->param_registers) {
    if (r) {
      incoming.insert(*r);
    }
  }
  const std::size_t count = zp->param_registers.size();
  asdl::Int off = 0;
  for (const auto& pname : fn.params) {
    const Symbol* sym = symbols->find(pname);
    const asdl::Int n = sym != nullptr ? sizeof_type(sym->type, nullptr) : asdl::Int(1);
    // Python's slice `param_registers[off:off + n]`, which stops at the
    // end of the list.
    std::vector<std::optional<std::string>> regs;
    for (asdl::Int k = off; k < off + n && k < static_cast<asdl::Int>(count); ++k) {
      regs.push_back(zp->param_registers[static_cast<std::size_t>(k)]);
    }
    off += n;
    bool any = false;
    for (const auto& r : regs) {
      any = any || (r && !r->empty());
    }
    if (!any) {
      continue;
    }
    for (std::size_t k = 0; k < regs.size(); ++k) {
      NameSet others;
      for (const auto& r : incoming) {
        if ((r == "X" || r == "Y") && !(regs[k] && *regs[k] == r)) {
          others.insert(r);
        }
      }
      if (!others.empty()) {
        out.insert_or_assign(register_param_local(pname) + ".b" + std::to_string(k),
                             std::move(others));
      }
    }
  }
  return out;
}

// Puts a `CPX #0` (`CPY #0`) before each branch on N or Z that reads
// the flags of a move into X (or Y) right before it. The optimizer
// treats a move as a copy that leaves the flags alone. Copy propagation
// folds `TXA; TAX` into one move, the coloring makes that a move of X to
// itself, which emits nothing, and the branch then read the flags of
// whatever came before. A shift by a count arriving in X, `LDA n; TAX;
// BEQ done`, branched on the caller's flags. The compare keeps the test,
// and `redundant_compare` drops it after an `LDX` that sets the same
// flags. When the code the branch reaches reads C, which the compare
// sets, `INX; DEX` takes its place. A move into A is left alone, since
// the allocator never colors a value to A, so a load into A stays a
// load.
ir::Function explicit_index_tests(ir::Function fn) {
  const auto& instrs = fn.instructions;
  std::vector<ir::Type_instruction> out;
  bool changed = false;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const ir::Type_instruction& ins = instrs[i];
    const auto* br = as<ir::Branch>(ins);
    const ir::Mov* prev = i == 0 ? nullptr : as<ir::Mov>(instrs[i - 1]);
    const ir::Reg* reg = prev == nullptr ? nullptr : as<ir::Reg>(prev->dst);
    if (br != nullptr && isa<ir::EQ, ir::NE, ir::MI, ir::PL>(br->cond) && reg != nullptr &&
        isa<ir::X, ir::Y>(reg->reg)) {
      const ir::Type_operand r = *reg;
      if (c_dead_at(instrs, i)) {
        out.emplace_back(ir::Compare{.left = r, .right = ir::Imm{.value = 0}});
      } else {
        out.emplace_back(ir::Inc{.dst = r});
        out.emplace_back(ir::Dec{.dst = r});
      }
      changed = true;
    }
    out.push_back(ins);
  }
  if (!changed) {
    return fn;
  }
  fn.instructions = std::move(out);
  return fn;
}

}  // namespace optimizer

OptimizedAsmFunction optimize_function(ir::Function fn, const StaticNames& statics,
                                       const std::set<asdl::Int>& blocked_addrs,
                                       const std::optional<Range>& allowed_range,
                                       const std::vector<asdl::Int>* local_pool,
                                       const AsmFunctionOptions& options) {
  // Fuse `LDA P; SEC; SBC #1; STA dst; LDA #0; CMP P; B<cc>` into `LDA
  // P; SEC; SBC #1; STA dst; B<flipped>` before SSA construction, so the
  // eligibility scan sees the IR without the compare against zero.
  // Without the compare's `cmp_right` position, the induction variable
  // becomes eligible for X or Y.
  {
    ir::Program prog;
    prog.top_level.emplace_back(std::move(fn));
    prog = apply_sub1_test_zero_peephole(std::move(prog));
    fn = std::move(std::get<ir::Function>(prog.top_level[0]));
  }
  fn = optimizer::explicit_index_tests(std::move(fn));
  fn = to_ssa(fn, statics);
  // Forward and backward copy propagation and byte-granular dead code
  // elimination on the SSA form, to a fixed point. The forward pass
  // replaces uses by the copy's source, the backward one collapses the
  // round trips `Pseudo -> Reg(A) -> memory` into one direct write, and
  // byte DCE drops the moves whose destination nothing reads, and each
  // can expose work for the others. The statics go to all three, so
  // writes that are visible outside stay.
  while (true) {
    const std::vector<ir::Type_instruction> prev = fn.instructions;
    fn = copy_propagate(std::move(fn), statics);
    fn = backward_copy_propagate(std::move(fn), statics);
    fn = byte_dce(std::move(fn), statics);
    // Collapse `Mov(Imm(0), A); Or(X, A)` to `Mov(X, A)` here, before
    // coalescing, so the cleaner chain of copies feeds the move
    // coalescing. or_zero_absorb.cpp says why this is apart from the
    // `const_arith_fold` pass after the coloring.
    fn = absorb_zero_load(std::move(fn));
    if (fn.instructions == prev) {
      break;
    }
  }
  // The byte-granular allocation on the function, still in SSA form.
  // The interference graph of an SSA program is chordal, which makes the
  // greedy coloring in a perfect elimination order optimal at unit
  // width. `blocked_addrs` holds the zero-page bytes of the zp_abi
  // parameters, if any, so the body's locals avoid them and don't
  // overwrite an incoming parameter in mid-computation.
  const Liveness liveness = compute_liveness(fn);
  InterferenceGraph graph = build_interference(fn, liveness, statics);
  if (options.recursive_callees != nullptr && !options.recursive_callees->empty()) {
    check_nothing_live_across(fn, liveness, graph, *options.recursive_callees);
  }
  // Move coalescing merges the pairs of pseudos that a `Mov` or a `Phi`
  // argument connects and that don't interfere into one node, so they
  // get one color. After `Phi` destruction the matching moves are moves
  // to themselves, which `asm_emit` drops, which removes the routing
  // through temporaries that the `Phi`s gave.
  const CoalesceResult coalesced = coalesce_moves(fn, graph);
  // Eligibility for X and Y. The scan finds the pseudos whose uses meet
  // the shape constraints of each instruction, with hints for the names
  // that feed a `Mov(P, A); Mov(A, X|Y)` chain of index setup, the
  // candidates that save the most. It scans the IR's names from before
  // the coalescing and projects them onto the representatives.
  //
  // A `register` local is a hard pin, always to Y, since the user's
  // variable has no other storage, so a failure has to be an error. A
  // `register` parameter is a soft hint to the register it arrives in.
  // The entry stub already writes its zero-page slot, so the body has
  // somewhere to fall back to when the IR's shape doesn't fit the
  // register (`n` used in `LDA #0; CMP n`, where `CMP` takes no X or Y as
  // its right operand). When the hint succeeds, the store at entry
  // becomes dead and is dropped.
  HwRegEligibility raw = scan_function(fn, options.local_pins, options.param_hints);
  if (options.entry_limits != nullptr && !options.entry_limits->empty()) {
    const EntryLimits& limits = *options.entry_limits;
    auto limited_to = [&](const std::string& n, const char* reg) {
      const NameSet* l = entry_limit(limits, n);
      return l != nullptr && l->contains(reg);
    };
    for (NameSet* s : {&raw.eligible_x, &raw.hints_x}) {
      std::erase_if(*s, [&](const std::string& n) { return limited_to(n, "X"); });
    }
    for (NameSet* s : {&raw.eligible_y, &raw.hints_y}) {
      std::erase_if(*s, [&](const std::string& n) { return limited_to(n, "Y"); });
    }
    std::erase_if(raw.hints_carry, [&](const std::string& n) {
      const NameSet* l = entry_limit(limits, n);
      return l != nullptr && !l->empty();
    });
  }
  const HwRegEligibility rep_eligibility =
      project_eligibility(raw, coalesced, all_pseudo_names(fn));
  Coloring coloring = color_graph(fn, graph, std::nullopt, &blocked_addrs, &rep_eligibility,
                                  allowed_range, &liveness, &coalesced.representative);
  // Every name merged into a representative takes the representative's
  // color, since `apply_coloring` looks names up, so the merged names go
  // back into the assignments.
  for (const auto& [name, rep_unused] : coalesced.representative) {
    const std::string rep = coalesced.resolve(name);
    if (const std::string* letter = coloring.hwreg_assignments.find(rep)) {
      const std::string value = *letter;
      coloring.hwreg_assignments.insert_or_assign(name, value);
    } else if (const asdl::Int* base = coloring.assignments.find(rep)) {
      const asdl::Int value = *base + coalesced.offset_of(name);
      coloring.assignments.insert_or_assign(name, value);
    } else if (coloring.spilled.contains(rep)) {
      coloring.spilled.insert(name);
    }
  }
  // Apply the coloring to the SSA function before its destruction, so
  // the ordering of the parallel copies in `from_ssa` can find cycles
  // between moves at the level of the physical slots. Two moves from
  // `Phi`s whose distinct SSA names happen to color to one slot would
  // otherwise miss each other in the cycle check, which compared SSA
  // names. After the rewrite they are `Mov(ZP($A), ZP($B))`, which the
  // check on storage handles.
  fn = apply_coloring(std::move(fn), coloring, local_pool);
  fn = from_ssa(fn);
  return {std::move(fn), std::move(coloring)};
}

OptimizedAsmProgram optimize_program(ir::Program prog, const AsmOptimizerOptions& options) {
  // A prepass over the program replaces the references to the const
  // scalar statics of internal linkage with immediates of their bytes
  // and drops the statics nothing names any more. It runs before the
  // round trip of each function, whose forward copy propagation picks up
  // the `Mov(Imm, Pseudo)` shapes it leaves.
  prog = fold_const_statics(std::move(prog), options.symbols);
  StaticNames statics = options.extra_statics;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      statics.insert(fn->name);
    } else {
      statics.insert(std::get<ir::StaticVariable>(tl).name);
    }
  }
  // The c99 names with the `register` storage class, locals and
  // parameters. A `register` local is a hard pin to Y, and a `register`
  // parameter a soft hint to the register (A or X) its convention
  // passes it in, which comes from each function's layout below.
  const std::vector<std::string> register_names = register_var_names(options.symbols);
  OptimizedAsmProgram out;
  for (auto& tl : prog.top_level) {
    auto* fn = as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    std::set<asdl::Int> blocked = blocked_addrs_for(*fn, options.param_layouts);
    blocked.insert(options.reserved_zp.begin(), options.reserved_zp.end());
    const std::optional<Range> allowed = allowed_range_for(*fn, options.local_pools);
    const std::vector<asdl::Int>* pool =
        options.local_pools == nullptr ? nullptr : options.local_pools->find(fn->name);
    // The `register` locals, which aren't among the parameters, are hard
    // pins to Y.
    Hints local_pins;
    const NameSet params(fn->params.begin(), fn->params.end());
    for (const auto& n : register_names) {
      if (!params.contains(n)) {
        local_pins.insert_or_assign(n, "Y");
      }
    }
    const ParamLayout* layout = layout_of(options.param_layouts, fn->name);
    const Hints param_hints = param_register_hints(*fn, layout, options.symbols);
    const EntryLimits limits = optimizer::entry_register_limits(*fn, layout, options.symbols);
    AsmFunctionOptions fn_options{
        .local_pins = local_pins.empty() ? nullptr : &local_pins,
        .param_hints = param_hints.empty() ? nullptr : &param_hints,
        .entry_limits = &limits,
        .recursive_callees = options.recursion_saves == nullptr
                                 ? nullptr
                                 : options.recursion_saves->find(fn->name)};
    OptimizedAsmFunction r =
        optimize_function(std::move(*fn), statics, blocked, allowed, pool, fn_options);
    out.colorings.insert_or_assign(r.fn.name, std::move(r.coloring));
    tl = std::move(r.fn);
  }
  // Drop the statics of internal linkage that nothing names. This runs
  // after the functions, so their dead code elimination and copy
  // propagation settle before the references are counted, and a static
  // whose last use was a dead load doesn't stay because the load was
  // still in the IR upstream.
  out.prog = apply_dead_static_elimination(std::move(prog));
  return out;
}

}  // namespace c6502::passes
