// The port of passes/optimization_asm/hwreg_eligibility.py: which SSA
// pseudos may live in X or Y, and which the allocator should try there
// first.
#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "asdl/runtime.h"
#include "asm_ast.h"
#include "ordered_map.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// A function's eligibility for X and Y, its hints, and the use count
// of each name.
//
// The eligibility is per register, since some operand shapes work with
// only one of the two. The usual case is `Mov(IndexedData, P)`: P may go
// in Y when the `IndexedData` is indexed by X (`LDY abs,X` exists), and
// not in X, which would take `LDX abs,X`, which the 6502 doesn't have,
// and the reverse for an `IndexedData` indexed by Y. Most other peers
// (Imm, ZP, Data, Reg) work with both, and there `eligible_x` and
// `eligible_y` agree.
//
// `use_count[name]` counts the pseudo operands of `name` anywhere in
// the function, in any role, in the order the names are first seen. The
// coloring gives a name with more references the register first: a name
// used 50 times (a loop's induction variable used in many indexed
// accesses) wins over one used twice (a short-lived intermediate). The
// count is approximate, since it weighs every position alike and doesn't
// model the nesting of loops, but it follows the intuition that more
// references mean more savings.
//
// `required_x` and `required_y` hold the names that must be pinned to
// that register, those of a `register` local, which is always Y. The
// allocator raises when it can't honor one, since a fallback to zero
// page would break the user's contract. `required_x` stays empty under
// the `register` keyword and is kept for symmetry.
//
// `hints_carry` holds the upper bytes of adds and subtracts of a byte
// (see "Carry hint" in hwreg_eligibility.cpp), which the allocator tries
// for X and then Y after `hints_x` and `hints_y`, and `carry_pairs` each
// (P, Q), which share a register or get none.
struct HwRegEligibility {
  NameSet eligible_x;
  NameSet eligible_y;
  NameSet hints_x;
  NameSet hints_y;
  OrderedMap<std::string, asdl::Int> use_count;
  NameSet required_x;
  NameSet required_y;
  NameSet hints_carry;
  std::vector<std::pair<std::string, std::string>> carry_pairs;

  // Returns the names that may be pinned to at least one register, the
  // union of `eligible_x` and `eligible_y`.
  NameSet eligible() const;
  bool operator==(const HwRegEligibility&) const = default;
};

// Computes the eligibility of each pseudo of `fn` for X and Y, and the
// X and Y hints that the chains of index setup give, in one forward pass
// over its instructions.
//
// The eligibility is per register and conservative. A pseudo is in
// `eligible_x` when every definition and use of it can be encoded with
// X, and likewise for `eligible_y`. A name can be in both (the usual
// case, with Imm, ZP, Data, or Reg peers), in one (with an asymmetric
// `IndexedData` peer), or in neither (any role that disqualifies it).
//
// `register_pins` maps a base name of the c99 IR (such as `@2.y`) to
// the register ("X" or "Y") it must take, and each name derived from it
// joins `required_x` or `required_y`. That pinning is hard, and the
// coloring raises when the IR's uses conflict with the register. It is
// for `register` locals (always Y), which have no storage to fall back
// on.
//
// `register_hints` is the soft form. It adds the matching SSA names to
// `hints_x` or `hints_y`, so the allocator tries the register and falls
// back when it doesn't fit. It is for `register` parameters, which the
// calling convention leaves in the named register on entry, where the
// entry stub of `tac_to_asm` writes the parameter's slot, so the body
// has a slot to fall back on.
//
// Both maps match names through `ssa_base`, so one base spelling matches
// every SSA version of it.
HwRegEligibility scan_function(const asm_ast::Function& fn,
                               const OrderedMap<std::string, std::string>* register_pins = nullptr,
                               const OrderedMap<std::string, std::string>* register_hints = nullptr);

namespace hwreg_eligibility {

// Returns the base spelling of the c99 IR that an SSA-renamed name
// comes from. Two layers of suffix ride on the original name. TAC SSA
// (`passes.optimization.ssa_construction.fresh`) appends `.<counter>`
// for each definition, so `@1.acc` becomes `@1.acc.1`, `@1.acc.2`, and
// so on, and asm SSA (`passes.optimization_asm.ssa_construction.fresh`)
// appends `.b<offset>.v<counter>` for each byte and version. A 1-byte
// local `@1.acc` may then appear as `@1.acc.2.b0.v3`. The asm suffix is
// stripped first (`.b<digits>.v<digits>` at the end), then one TAC
// suffix (`.<digits>` at the end). The rename of identifier resolution,
// `@<counter>.<orig>`, has a segment `<orig>` that isn't all digits, so
// it survives both.
std::string ssa_base(std::string_view name);

}  // namespace hwreg_eligibility

}  // namespace c6502::passes
