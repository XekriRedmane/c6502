// Synthesizes each function's prologue and epilogue from the bare-exit
// shape of the asm.
//
// The `-O` pipeline runs `tac_to_asm` with `bare_exit=True`, which gives
// an asm program in which each function has no `FunctionPrologue` at
// its start and ends with a bare `Return(save_a)` atom, where the
// unoptimized pipeline has the compound node `Ret(arg_bytes,
// local_bytes, save_a, callee_saved_addrs)` that `replace_pseudoregisters`
// fills in.
//
// `replace_pseudoregisters.replace_program_bare_exit` keeps the body in
// that shape and also returns each function's `FrameDims`, the frame it
// computed: M, the local bytes, N, the argument bytes, and S, the
// callee-saved bytes. This pass starts from there. It puts
// `FunctionPrologue(N, M, callee_saved_addrs)` at the start of each
// function and rewrites every `Return(save_a)` as `Ret(N, M, save_a,
// callee_saved_addrs)`. The result is an asm program of the regular
// shape, which the rest of the pipeline (`asm_to_asm2`, `asm_emit`,
// `sim/assembler`) reads unchanged.
//
// The passes that work on the asm before this one (the asm SSA round
// trip and the byte-granular register allocation) see only atomic asm
// instructions and the bare exits, which is the body of the function
// without the frame's code, the view an asm optimizer works best on.
//
// A function whose state fits in zero page, with no arguments in its
// frame, no frame locals, and no callee-saved bytes, keeps its bare
// `Return`s, which become a bare RTS.
#include "passes/prologue_synthesis.h"

#include <utility>
#include <vector>

#include "errors.h"
#include "passes/asm_liveness.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

namespace {

bool is_reg(const ir::Type_operand& op, ir::Kind_reg r) {
  const auto* reg = asdl::as<ir::Reg>(op);
  return reg != nullptr && ir::kind(reg->reg) == r;
}

}  // namespace

// Splits the register parameters' entry stubs off the start of
// `instrs`, skipping labels: the leading moves from A, X, or Y into
// memory (a `Data`, the local each parameter is copied into), into X,
// or into A. At the entry those registers hold only parameters, and
// the prologue writes A and Y, so a parameter that arrives in A or Y
// has to be moved before it. The prologue leaves X alone, and it saves
// A around itself when the body reads A. Returns the stubs and the
// rest, with the labels left in the rest.
std::pair<std::vector<ir::Type_instruction>, std::vector<ir::Type_instruction>>
split_entry_stubs(std::vector<ir::Type_instruction> instrs) {
  std::vector<ir::Type_instruction> stubs;
  std::vector<ir::Type_instruction> rest;
  std::size_t i = 0;
  for (; i < instrs.size(); ++i) {
    auto& ins = instrs[i];
    if (asdl::isa<ir::Label>(ins)) {
      rest.push_back(std::move(ins));
      continue;
    }
    const auto* mov = asdl::as<ir::Mov>(ins);
    if (mov != nullptr && !mov->is_volatile &&
        (is_reg(mov->src, ir::Kind_reg::A) || is_reg(mov->src, ir::Kind_reg::X) ||
         is_reg(mov->src, ir::Kind_reg::Y)) &&
        (asdl::isa<ir::Data>(mov->dst) || is_reg(mov->dst, ir::Kind_reg::A) ||
         is_reg(mov->dst, ir::Kind_reg::X))) {
      stubs.push_back(std::move(ins));
      continue;
    }
    break;
  }
  for (; i < instrs.size(); ++i) {
    rest.push_back(std::move(instrs[i]));
  }
  return {std::move(stubs), std::move(rest)};
}

namespace {

ir::Type_operand reg(ir::Type_reg r) { return ir::Reg{.reg = std::move(r)}; }

void synthesize_function(ir::Function& fn, const FrameDims& dims) {
  // The input must have the bare-exit shape, which
  // `replace_program_bare_exit` leaves: atomic instructions and bare
  // `Return`s, with no `Ret` and no `FunctionPrologue`.
  for (const auto& instr : fn.instructions) {
    if (asdl::isa<ir::Ret>(instr)) {
      throw InternalError("prologue_synthesis: function '" + fn.name +
                          "' already has a Ret(...) node — bare-exit invariant broken");
    }
    if (asdl::isa<ir::FunctionPrologue>(instr)) {
      throw InternalError("prologue_synthesis: function '" + fn.name +
                          "' already has a FunctionPrologue node — bare-exit invariant broken");
    }
  }
  const bool needs_frame =
      dims.arg_bytes > 0 || dims.local_bytes > 0 || !dims.callee_saved_addrs.empty();
  if (!needs_frame) {
    // With no arguments, no frame locals, and no callee-saved bytes,
    // there is nothing for the SSP and FP arithmetic to do. The bare
    // `Return`s stay, and `asm_to_asm2` lowers each to RTS. `save_a`
    // doesn't matter without a 16-bit SSP add to bracket, since RTS
    // keeps A. This gives the bytes of the older `Ret(0, 0, save_a,
    // [])`, which `asm_to_asm2._ret` lowers to `[Return()]` too, and
    // makes the decision where later passes can see from the asm
    // alone that the function has no frame.
    return;
  }
  // The prologue's SSP and FP arithmetic runs through A and Y, so it
  // would overwrite a `register` parameter that arrives in A or Y. The
  // entry copies of the register parameters into their locals move in
  // front of it. If the body still reads A or Y at its entry after
  // that (a parameter kept in its register), the prologue goes between
  // pushes and pulls of them: PHA / PLA for A, and TYA; PHA / PLA; TAY
  // for Y, inside A's. The prologue never touches X.
  auto [stubs, body] = split_entry_stubs(std::move(fn.instructions));
  std::vector<ir::Type_instruction> out = std::move(stubs);
  const bool a_live = !a_dead_at(body, 0);
  const bool y_live = !y_dead_at(body, 0);
  if (a_live) {
    out.push_back(ir::Push{.src = reg(ir::A{})});
  }
  if (y_live) {
    out.push_back(ir::Mov{.src = reg(ir::Y{}), .dst = reg(ir::A{})});
    out.push_back(ir::Push{.src = reg(ir::A{})});
  }
  out.push_back(ir::FunctionPrologue{.arg_bytes = dims.arg_bytes,
                                     .local_bytes = dims.local_bytes,
                                     .callee_saved_addrs = dims.callee_saved_addrs});
  if (y_live) {
    out.push_back(ir::Pop{.dst = reg(ir::A{})});
    out.push_back(ir::Mov{.src = reg(ir::A{}), .dst = reg(ir::Y{})});
  }
  if (a_live) {
    out.push_back(ir::Pop{.dst = reg(ir::A{})});
  }
  for (auto& instr : body) {
    if (const auto* ret = asdl::as<ir::Return>(instr)) {
      out.push_back(ir::Ret{.arg_bytes = dims.arg_bytes,
                            .local_bytes = dims.local_bytes,
                            .save_a = ret->save_a,
                            .save_x = ret->save_x,
                            .callee_saved_addrs = dims.callee_saved_addrs,
                            .save_c = ret->save_c});
      continue;
    }
    out.push_back(std::move(instr));
  }
  fn.instructions = std::move(out);
}

}  // namespace

asm_ast::Program synthesize_program(asm_ast::Program prog, const FrameDimsByFunction& dims_by_fn) {
  for (auto& tl : prog.top_level) {
    auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    const FrameDims* dims = dims_by_fn.find(fn->name);
    if (dims == nullptr) {
      throw InternalError("prologue_synthesis: no FrameDims for function '" + fn->name + "'");
    }
    synthesize_function(*fn, *dims);
  }
  return prog;
}

}  // namespace c6502::passes
