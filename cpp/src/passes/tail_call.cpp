// The tail-call peephole: `Call(name); Return(_)` becomes `Jump(name)`,
// and a jump to a return becomes the return.
//
// Pattern
//
// Two instructions in a row at the end of a function:
//
//     Call(name)               # JSR name
//     Return(save_a)           # RTS (a bare exit, with no frame teardown)
//
// become
//
//     Jump(target=name)        # JMP name
//
// Soundness
//
// The bare `Return(save_a)` atom becomes a single `RTS` when it is
// lowered (`asm_to_asm2._ret`'s branch for a function with no frame
// ignores `save_a` and emits a bare `RTS`). So the original pair runs
//
//     JSR name        ; pushes the return address R, jumps to name
//     [name's body]
//     RTS             ; pops R, jumps to R, the RTS just below
//     RTS             ; pops the outer return address, to the caller
//
// and after the rewrite
//
//     JMP name        ; jumps to name with no change to the stack
//     [name's body]
//     RTS             ; pops the outer return address, to the caller
//
// The caller sees the same behavior, with one push and pull fewer, a
// byte fewer (JSR and RTS take 4, JMP 3), and 7 cycles saved for each
// call (6 for the JSR and 6 for the RTS, less the JMP's 3, is 9, and
// the callee's RTS now serves both roles, for 7).
//
// `save_a` on `Return` plays no part in the rewrite. A's value after
// `JMP name` is whatever `name` last loaded into A, as it would be
// after `JSR name`. The bare `RTS` doesn't save A with `PHA` and `PLA`
// (see `asm_to_asm2._ret`'s branch for no frame), so `save_a` is only
// information in the bare case.
//
// The frame
//
// The peephole matches only `Return(_)`, never `Ret(arg, local, ...)`.
// A `Ret` carries a frame teardown (restoring SSP, FP, and the
// callee-saved bytes) that has to run before control leaves the
// function, and a tail call past it would skip the teardown and
// corrupt the soft stack. `prologue_synthesis.synthesize_prologue`
// emits `Return(save_a)` exactly when the function has no frame
// (arg_bytes, local_bytes, and callee_saved_bytes all 0) and `Ret(...)`
// otherwise. The optimized pipeline's bare exits are where tail calls
// fire most often, since every zp_abi leaf or non-recursive function
// ends in `Return(_)`.
//
// A jump to a return
//
// A `Jump(L)` whose label `L` is followed (after any further labels) by
// a bare `Return` becomes a copy of that `Return`. `RTS` is 1 byte and
// 3 cycles less than `JMP`, and does what the jump would have done
// next. A `switch` whose cases end in `break` leaves this shape, each
// case jumping to the `RTS` after the switch, and a case that ends in a
// call, `JSR f; JMP .break`, then becomes `JSR f; RTS`, which the rule
// above makes `JMP f`. Zork's `do_2op` (examples/zork/decode.c) has a
// dozen such cases. A `Ret`, which tears down a frame, isn't copied.
//
// Where it runs
//
// Inside the asm peephole fixed point. Turning a `Call` into a `Jump`
// takes the call's `name` out of the call graph at that site, but
// `asm_dead_store` and the other passes that look for `Call` atoms
// still see the other calls in the body, and the function called is
// still named (any other JSR or JMP keeps its symbol). Its position in
// the fixed point doesn't matter much, since the `Jump` it makes ends
// a block and enables no later peephole.
#include "passes/tail_call.h"

#include <cstddef>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// Returns the bare `Return` a jump to `target` reaches first, or null.
const ir::Return* return_at(const std::vector<ir::Type_instruction>& instrs,
                            const LabelMap& labels, const std::string& target) {
  const long at = labels.find(target);
  if (at < 0) {
    return nullptr;
  }
  std::size_t k = static_cast<std::size_t>(at);
  while (k < instrs.size() && isa<ir::Label>(instrs[k])) {
    k += 1;
  }
  if (k < instrs.size()) {
    return as<ir::Return>(instrs[k]);
  }
  return nullptr;
}

void rewrite_function(ir::Function& fn) {
  const std::vector<ir::Type_instruction>& original = fn.instructions;
  const LabelMap labels(original);
  std::vector<ir::Type_instruction> instrs;
  instrs.reserve(original.size());
  for (const auto& ins : original) {
    if (const auto* jump = as<ir::Jump>(ins)) {
      if (const ir::Return* ret = return_at(original, labels, jump->target)) {
        instrs.push_back(*ret);
        continue;
      }
    }
    instrs.push_back(ins);
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (i + 1 < instrs.size() && isa<ir::Return>(instrs[i + 1])) {
      if (auto* call = as<ir::Call>(instrs[i])) {
        out.push_back(ir::Jump{.target = std::move(call->name),
                               .reg_args = std::move(call->reg_args)});
        i += 2;
        continue;
      }
    }
    out.push_back(std::move(instrs[i]));
    i += 1;
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program apply_tail_call(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
