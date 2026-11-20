// Asm-level backward copy propagation.
//
// Forward copy propagation (copy_propagation.cpp) replaces the uses of
// a copy's destination with its source, along the direction from a
// definition to its uses. This pass does the opposite: when the one use
// of a Pseudo P feeds its value into a final memory destination D, P's
// definition is made to write D, and the round trip goes.
//
// `tac_to_asm` sends every transfer between a Pseudo and memory through
// A, so a copy of P into D is the pair
//
//     Mov(Pseudo P, Reg(A))      # load P into A
//     Mov(Reg(A), D)             # store A into D
//
// which this pass takes as one `Copy(P, D)`. When that is P's one use
// and P's definition is `Mov(Reg(A), P)`, the pair goes and the
// definition writes D. One memory-to-memory `Mov(P, D)`, which emits as
// the same `LDA P; STA D`, counts too: `tac_to_asm` writes each byte of
// a zp_abi argument that way, so a pointer computed only to be passed,
// as in `print_ascii_string(prompts[which].text, 15)`, is stored
// straight into the callee's parameter bytes.
//
// The rewrite takes
//
//     Mov(Reg(A), Pseudo P)        # P's definition (`def_idx`)
//     ... region R ...
//     Mov(Pseudo P, Reg(A))        # P's last use (`i`)
//     Mov(Reg(A), D)               # right after it (`i + 1`)
//
// to
//
//     Mov(Reg(A), D)               # the definition, moved
//     ... region R ...
//
// when every one of these holds.
//   * P has exactly one use in the function, the pair's first
//     instruction.
//   * P isn't excluded (statics, names whose address is taken, the
//     targets of a read-modify-write), the set SSA construction
//     excludes.
//   * `def_idx` is the one `Mov(Reg(A), P)` before `i`.
//   * `def_idx`, `i`, and `i + 1` are in one straight-line block, with
//     no `Label`, `Jump`, `Branch`, `Ret`, or `Return` from `def_idx + 1`
//     to `i`.
//   * No instruction of R reads or writes D's storage, which every
//     operand of every instruction of R is checked against.
//   * R has no `Call`. A call writes HARGS, the runtime helpers'
//     exchange block, and what else it writes can't be bounded.
//   * A is dead right after the pair goes: walking forward from what
//     was `i + 2`, the next instruction that touches A writes it without
//     reading it, or the walk reaches a `Ret` / `Return` without
//     `save_a` or the function's end.
//   * The flags N and Z are dead right after the pair. The dropped
//     `Mov(P, Reg(A))` is an `LDA`, which sets them, and dropping it
//     would change a later `Branch` that reads them with no instruction
//     setting them between.
//
// D is a memory operand that isn't a Pseudo (`Data` or `Stack`).
// Merging two Pseudos is the work of the register allocation and move
// coalescing.
//
// A rewrite can make another possible, so the pass repeats until
// nothing changes. A static P is excluded, since a write of one is seen
// outside the function and its one use in the function isn't its only
// one. A static D is a fine target, since the rule against calls covers
// what is seen outside.
#include "passes/optimization_asm/backward_copy_propagation.h"

#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/optimization_asm/ssa_operands.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

ByteVar key_of(const ir::Pseudo& p) { return {p.name, static_cast<std::int64_t>(p.offset)}; }

// Returns the number of uses of each (name, offset). Definitions don't
// count, since the pass looks for a Pseudo with one use.
std::map<ByteVar, int> compute_use_counts(const ir::Function& fn) {
  std::map<ByteVar, int> out;
  for (const auto& instr : fn.instructions) {
    ssa_operands::for_each_use_operand(instr, [&](const ir::Type_operand& op) {
      if (const auto* p = as<ir::Pseudo>(op)) {
        ++out[key_of(*p)];
      }
    });
  }
  return out;
}

// Returns whether `op` is a destination the pass moves a definition to:
// a memory byte that isn't a Pseudo. `Data` and `Stack` are what
// `tac_to_asm` writes at this point of the pipeline, and `Frame` and
// `ZP` come from the later `replace_pseudoregisters`.
bool is_memory_dst(const ir::Type_operand& op) { return isa<ir::Data, ir::Stack>(op); }

// Returns whether `a` and `b` may name the same memory cell.
//
// Operands of the forms with stable names compare directly: `Data` or
// `Pseudo` with `Data` or `Pseudo` by (name, offset), `Stack` with
// `Stack` by offset, and `ZP` with `ZP` by address plus offset. An
// indirect access aliases the bytes of its pointer: `Indirect` uses
// DPTR, so it aliases any `Data("DPTR", _)`, and `IndirectZp(addr, _)`
// aliases the `ZP` bytes at `addr` and `addr + 1`. Those cases matter
// because moving the pass's store into `Data("DPTR", k)` ahead of an
// `Indirect` read between would change part of the pointer the read
// goes through, giving it a mixed address.
bool operands_alias(const ir::Type_operand& a, const ir::Type_operand& b) {
  const auto name_offset = [](const ir::Type_operand& op)
      -> std::optional<std::pair<const std::string*, asdl::Int>> {
    if (const auto* d = as<ir::Data>(op)) {
      return std::make_pair(&d->name, d->offset);
    }
    if (const auto* p = as<ir::Pseudo>(op)) {
      return std::make_pair(&p->name, p->offset);
    }
    return std::nullopt;
  };
  const auto na = name_offset(a);
  const auto nb = name_offset(b);
  if (na && nb) {
    // A static is still a `Pseudo` where `tac_to_asm` wrote one, and a
    // `Data` where it wrote that, as for a store to a constant byte of
    // it, so `Pseudo(Z, 2)` and `Data(Z, 2)` are the same byte. With
    // `Z = 0x01020304UL; ZB[2] = v;` and `v` in A, the pass once moved
    // the store of `v` in front of `Z`'s.
    return *na->first == *nb->first && na->second == nb->second;
  }
  if (const auto* sa = as<ir::Stack>(a)) {
    if (const auto* sb = as<ir::Stack>(b)) {
      return sa->offset == sb->offset;
    }
  }
  const auto* za = as<ir::ZP>(a);
  const auto* zb = as<ir::ZP>(b);
  if (za != nullptr && zb != nullptr) {
    return za->address + za->offset == zb->address + zb->offset;
  }
  const auto is_dptr = [](const ir::Type_operand& op) {
    const auto* d = as<ir::Data>(op);
    return d != nullptr && d->name == "DPTR";
  };
  if ((isa<ir::Indirect>(a) && is_dptr(b)) || (isa<ir::Indirect>(b) && is_dptr(a))) {
    return true;
  }
  const auto pointer_byte = [](const ir::IndirectZp& iz, const ir::ZP& z) {
    const asdl::Int addr = z.address + z.offset;
    return addr == iz.address || addr == iz.address + 1;
  };
  if (const auto* iza = as<ir::IndirectZp>(a); iza != nullptr && zb != nullptr) {
    return pointer_byte(*iza, *zb);
  }
  if (const auto* izb = as<ir::IndirectZp>(b); izb != nullptr && za != nullptr) {
    return pointer_byte(*izb, *za);
  }
  return false;
}

// Returns the index of the one `Mov(Reg(A), P)` before `upper_bound`,
// or nothing when there is none, when there are several (SSA has one),
// or when a `Mov` of another source defines P first.
std::optional<std::size_t> find_canonical_def(const std::vector<ir::Type_instruction>& instrs,
                                              const ir::Pseudo& p, std::size_t upper_bound) {
  std::optional<std::size_t> found;
  for (std::size_t idx = 0; idx < upper_bound; ++idx) {
    const auto* mov = as<ir::Mov>(instrs[idx]);
    if (mov == nullptr) {
      continue;
    }
    const auto* dst = as<ir::Pseudo>(mov->dst);
    if (dst == nullptr || dst->name != p.name || dst->offset != p.offset) {
      continue;
    }
    if (!is_reg_a(mov->src)) {
      return std::nullopt;
    }
    if (found) {
      return std::nullopt;
    }
    found = idx;
  }
  return found;
}

// Returns whether every instruction of `instrs[lo:hi]` lets D's write
// move to `lo - 1`. It doesn't for these.
//   * A control-flow boundary (`Label`, `Jump`, `Branch`, `Ret`,
//     `Return`). The definition and the use must be in one straight-line
//     block for the walk to cover what runs between them.
//   * A `Call`, which may write D (HARGS, statics), and whose writes
//     aren't known.
//   * An `AllocateStack` or `FunctionPrologue`, which move SSP, so that a
//     `Stack(k)` before one isn't the byte `Stack(k)` names after it. A
//     soft-stack call writes its arguments to `Stack` bytes right after
//     its `AllocateStack`.
//   * An operand of any instruction between that aliases D's storage.
bool safe_relocation_range(const std::vector<ir::Type_instruction>& instrs, std::size_t lo,
                           std::size_t hi, const ir::Type_operand& d) {
  for (std::size_t idx = lo; idx < hi; ++idx) {
    const auto& instr = instrs[idx];
    if (isa<ir::Label, ir::Jump, ir::Branch, ir::Ret, ir::Return, ir::Call, ir::AllocateStack,
            ir::FunctionPrologue>(instr)) {
      return false;
    }
    bool alias = false;
    ssa_operands::for_each_operand(instr, [&](const ir::Type_operand& op) {
      alias = alias || operands_alias(op, d);
    });
    if (alias) {
      return false;
    }
  }
  return true;
}

struct Match {
  std::size_t def_idx;
  ir::Type_operand dst;
  std::size_t length;
};

// Returns the rewrite of the copy at `i` when `instrs[i:i+2]` is the pair
// `P -> A -> D`, or `instrs[i]` the memory-to-memory `Mov(P, D)`, and
// the copy can go: the index of P's definition, D, and the number of
// instructions the copy takes.
std::optional<Match> try_match_at(const std::vector<ir::Type_instruction>& instrs,
                                  const LabelMap& labels, std::size_t i,
                                  const ExcludedNames& excluded, const StaticNames& statics,
                                  const std::map<ByteVar, int>& use_counts) {
  const auto* first = as<ir::Mov>(instrs[i]);
  // A volatile load of P is itself an effect that is seen, and moving
  // the definition would drop the `LDA P` from the output. A volatile
  // store into D has to stay too.
  if (first == nullptr || first->is_volatile) {
    return std::nullopt;
  }
  const auto* p = as<ir::Pseudo>(first->src);
  if (p == nullptr) {
    return std::nullopt;
  }
  std::size_t length = 0;
  const ir::Type_operand* d = nullptr;
  if (is_memory_dst(first->dst)) {
    length = 1;
    d = &first->dst;
  } else {
    if (!is_reg_a(first->dst) || i + 1 >= instrs.size()) {
      return std::nullopt;
    }
    const auto* second = as<ir::Mov>(instrs[i + 1]);
    if (second == nullptr || second->is_volatile || !is_reg_a(second->src) ||
        !is_memory_dst(second->dst)) {
      return std::nullopt;
    }
    length = 2;
    d = &second->dst;
  }
  if (is_excluded(p->name, excluded, statics)) {
    return std::nullopt;
  }
  const auto count = use_counts.find(key_of(*p));
  if (count == use_counts.end() || count->second != 1) {
    return std::nullopt;
  }
  const auto def_idx = find_canonical_def(instrs, *p, i);
  if (!def_idx) {
    return std::nullopt;
  }
  // The definition stores into P, and dropping a volatile store would
  // drop a write that is seen.
  if (const auto* def = as<ir::Mov>(instrs[*def_idx]); def != nullptr && def->is_volatile) {
    return std::nullopt;
  }
  if (!safe_relocation_range(instrs, *def_idx + 1, i, *d)) {
    return std::nullopt;
  }
  if (!a_dead_at(instrs, labels, i + length)) {
    return std::nullopt;
  }
  if (!flags_dead_at(instrs, labels, i + length)) {
    return std::nullopt;
  }
  return Match{*def_idx, *d, length};
}

ir::Function one_pass(ir::Function fn, const StaticNames& statics) {
  const ExcludedNames excluded = excluded_pseudo_names(fn);
  const auto use_counts = compute_use_counts(fn);
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);

  struct Rewrite {
    std::size_t def_idx;
    std::size_t pair_idx;
    ir::Type_operand dst;
    std::size_t length;
  };
  std::vector<Rewrite> rewrites;
  std::set<std::size_t> claimed;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    if (claimed.contains(i)) {
      continue;
    }
    auto match = try_match_at(instrs, labels, i, excluded, statics, use_counts);
    if (!match) {
      continue;
    }
    bool taken = claimed.contains(match->def_idx);
    for (std::size_t k = i; k < i + match->length && !taken; ++k) {
      taken = claimed.contains(k);
    }
    if (taken) {
      continue;
    }
    claimed.insert(match->def_idx);
    for (std::size_t k = i; k < i + match->length; ++k) {
      claimed.insert(k);
    }
    rewrites.push_back(Rewrite{match->def_idx, i, std::move(match->dst), match->length});
  }
  if (rewrites.empty()) {
    return fn;
  }

  std::vector<bool> deleted(instrs.size(), false);
  std::map<std::size_t, const ir::Type_operand*> rewrite_def;
  for (const auto& r : rewrites) {
    rewrite_def[r.def_idx] = &r.dst;
    for (std::size_t k = r.pair_idx; k < r.pair_idx + r.length; ++k) {
      deleted[k] = true;
    }
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  for (std::size_t idx = 0; idx < instrs.size(); ++idx) {
    if (deleted[idx]) {
      continue;
    }
    if (const auto it = rewrite_def.find(idx); it != rewrite_def.end()) {
      // The definition moved is always `Mov(Reg(A), P)`.
      const auto& mov = std::get<ir::Mov>(instrs[idx]);
      out.push_back(ir::Mov{.src = mov.src, .dst = *it->second, .is_volatile = mov.is_volatile});
    } else {
      out.push_back(instrs[idx]);
    }
  }
  fn.instructions = std::move(out);
  return fn;
}

}  // namespace

ir::Function backward_copy_propagate(ir::Function fn, const StaticNames& statics) {
  // Each rewrite drops the copy's one or two instructions, so a pass
  // changed the function exactly when it made it shorter.
  while (true) {
    const std::size_t before = fn.instructions.size();
    fn = one_pass(std::move(fn), statics);
    if (fn.instructions.size() == before) {
      return fn;
    }
  }
}

}  // namespace c6502::passes
