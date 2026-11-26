/* A function this unit defines that

- is `static`,
- returns a pointer, and
- whose address isn't taken, so that every call is a direct call from
  this unit,

stores its result in a zero-page pointer of its own, `F@ret`, and
returns nothing, and each call copies `F@ret` into the call's result.
In TAC, `Ret(v)` in `F` becomes `Copy(v, F@ret); Ret()`, and
`FunctionCall(F, args, d)` becomes `FunctionCall(F, args); Copy(F@ret,
d)`.

`F@ret` is a `[[c6502::zeropage]]` object whose `StaticVariable` has no
initial bytes. */
#include "passes/return_in_zero_page.h"

#include <utility>
#include <vector>

#include "errors.h"
#include "passes/abi_selection.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;

using Chosen = OrderedMap<std::string, c99::Type_data_type>;

ir::Function rewrite(ir::Function fn, const Chosen& chosen) {
  std::vector<ir::Type_instruction> out;
  out.reserve(fn.instructions.size());
  const bool own = chosen.contains(fn.name);
  const std::string own_slot = own ? return_slot_name(fn.name) : std::string();
  for (auto& instr : fn.instructions) {
    if (own) {
      if (auto* ret = std::get_if<ir::Ret>(&instr); ret != nullptr && ret->val) {
        out.push_back(ir::Copy{.src = std::move(*ret->val), .dst = ir::Var{.name = own_slot}});
        out.push_back(ir::Ret{});
        continue;
      }
    }
    if (auto* call = std::get_if<ir::FunctionCall>(&instr);
        call != nullptr && call->dst && chosen.contains(call->name)) {
      ir::Type_val dst = std::move(*call->dst);
      std::string slot = return_slot_name(call->name);
      call->dst.reset();
      out.push_back(std::move(*call));
      out.push_back(ir::Copy{.src = ir::Var{.name = std::move(slot)}, .dst = std::move(dst)});
      continue;
    }
    out.push_back(std::move(instr));
  }
  fn.instructions = std::move(out);
  return fn;
}

}  // namespace

std::string return_slot_name(const std::string& fn) { return fn + "@ret"; }

ir::Program return_pointers_in_zero_page(ir::Program tac, SymbolTable& symbols) {
  const NameSet taken = abi_selection::address_taken(tac);
  Chosen chosen;
  for (const auto& tl : tac.top_level) {
    const auto* fn = std::get_if<ir::Function>(&tl);
    if (fn == nullptr || fn->is_global || taken.contains(fn->name)) {
      continue;
    }
    const Symbol* sym = symbols.find(fn->name);
    if (sym == nullptr || !std::holds_alternative<FunAttr>(sym->attrs)) {
      continue;
    }
    const auto* ftype = std::get_if<c99::FunType>(&sym->type);
    if (ftype == nullptr) {
      throw InternalError("return_pointers_in_zero_page: the function " + fn->name +
                          " has no function type");
    }
    if (std::holds_alternative<c99::Pointer>(*ftype->ret)) {
      chosen.insert_or_assign(fn->name, *ftype->ret);
    }
  }
  if (chosen.empty()) {
    return tac;
  }
  for (auto& tl : tac.top_level) {
    if (auto* fn = std::get_if<ir::Function>(&tl)) {
      *fn = rewrite(std::move(*fn), chosen);
    }
  }
  for (const auto& [fn, ret] : chosen) {
    std::string name = return_slot_name(fn);
    StaticAttr attrs;
    attrs.initial_value = Tentative{};
    attrs.is_global = false;
    attrs.zeropage = true;
    symbols.insert_or_assign(name, Symbol{.type = ret, .attrs = std::move(attrs)});
    tac.top_level.push_back(ir::StaticVariable{
        .name = std::move(name), .is_global = false, .data_type = ir::Pointer{}, .init = {}});
  }
  return tac;
}

}  // namespace c6502::passes
