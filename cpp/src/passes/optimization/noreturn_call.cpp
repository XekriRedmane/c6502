/* TAC pass: nothing runs after a call to a function declared
`[[noreturn]]`.

What it does. C23 §6.7.13.7 makes it undefined for a `[[noreturn]]`
function to return, so the instructions after a call to one, up to the
next label, never run. The pass puts a `Ret` right after the call and
drops those instructions. The `Ret` doesn't run either, and is there
because every path through a TAC function ends in one. On the 6502 the
call and the `Ret` become `JSR f; RTS`, which `tail_call` turns into
`JMP f`.

Zork's `do_2op` (examples/zork/decode.c) checks its operand count with

    if (left == 0)
        internal_error();

and reads `a` and `b` after. With `internal_error` declared
`[[noreturn]]`, no call comes between the copy of `OPERANDS[0]` into
`a` at the function's entry and the reads of `a`, so the asm's memory
value propagation reads `OPERANDS` there and the copy goes.

Rules.

- A `FunctionCall` whose callee's `FunAttr.noreturn` is set, by any of
  its declarations. A call through a pointer is left alone, since the
  attribute doesn't apply to the pointer's type.
- The call is followed by a `Ret` of nothing, in place of whatever
  followed it, in a function of any return type. Since the `Ret` never
  runs, no value has to be loaded for it, and `JSR f; RTS` is left for
  `tail_call`.

Where it runs. Before SSA construction, so that the dropped code's
definitions never take part in it. Under `-O` only. */
#include "passes/optimization/noreturn_call.h"

#include <utility>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;

// Returns whether `name` is a function with `FunAttr.noreturn` set.
bool is_noreturn(const std::string& name, const SymbolTable& symbols) {
  const Symbol* sym = symbols.find(name);
  if (sym == nullptr) {
    return false;
  }
  const auto* attrs = std::get_if<FunAttr>(&sym->attrs);
  return attrs != nullptr && attrs->noreturn;
}

}  // namespace

ir::Function noreturn_calls(ir::Function fn, const framework::PassContext& ctx) {
  if (ctx.symbols == nullptr) {
    return fn;
  }
  std::vector<ir::Type_instruction> out;
  out.reserve(fn.instructions.size());
  bool skipping = false;
  for (const auto& ins : fn.instructions) {
    if (skipping) {
      if (!asdl::isa<ir::Label>(ins)) {
        continue;
      }
      skipping = false;
    }
    out.push_back(ins);
    const auto* call = asdl::as<ir::FunctionCall>(ins);
    if (call != nullptr && is_noreturn(call->name, *ctx.symbols)) {
      out.push_back(ir::Ret{});
      skipping = true;
    }
  }
  if (out == fn.instructions) {
    return fn;
  }
  return framework::with_instructions(fn, std::move(out));
}

}  // namespace c6502::passes
