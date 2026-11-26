#include "passes/optimization/noreturn_call.h"

#include <utility>
#include <vector>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;

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
