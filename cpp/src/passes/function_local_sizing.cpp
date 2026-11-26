#include "passes/function_local_sizing.h"

#include <set>
#include <string>
#include <vector>

#include "passes/optimization_asm/all_operands.h"
#include "passes/replace_pseudoregisters.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using Names = std::set<std::string, std::less<>>;

std::set<asdl::Int> zp_bytes_used(const ir::Function& fn) {
  std::set<asdl::Int> addrs;
  for (const auto& instr : fn.instructions) {
    for (const ir::Type_operand* op : optimizer::all_operands_in(instr)) {
      if (const auto* zp = as<ir::ZP>(*op)) {
        addrs.insert(zp->address + zp->offset);
      }
    }
  }
  return addrs;
}

Names statics_set(const SymbolTable& symbols) {
  Names out;
  for (const auto& [name, sym] : symbols) {
    if (std::holds_alternative<StaticAttr>(sym.attrs)) {
      out.insert(name);
    }
  }
  return out;
}

std::vector<std::string> address_taken_local_names(const ir::Function& fn, const Names& statics) {
  Names seen;
  std::vector<std::string> out;
  const Names params(fn.params.begin(), fn.params.end());
  for (const auto& instr : fn.instructions) {
    const auto* la = as<ir::LoadAddress>(instr);
    if (la == nullptr) {
      continue;
    }
    const auto* p = as<ir::Pseudo>(la->src);
    if (p == nullptr || params.contains(p->name) || statics.contains(p->name) ||
        seen.contains(p->name)) {
      continue;
    }
    seen.insert(p->name);
    out.push_back(p->name);
  }
  return out;
}

}  // namespace

OrderedMap<std::string, asdl::Int> compute_local_bytes(const asm_ast::Program& prog) {
  OrderedMap<std::string, asdl::Int> out;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = as<ir::Function>(tl)) {
      out.insert_or_assign(fn->name, static_cast<asdl::Int>(zp_bytes_used(*fn).size()));
    }
  }
  return out;
}

OrderedMap<std::string, asdl::Int> compute_address_taken_bytes(const asm_ast::Program& prog,
                                                               const SymbolTable* symbols,
                                                               const TypeTable* types) {
  OrderedMap<std::string, asdl::Int> out;
  if (symbols == nullptr) {
    for (const auto& tl : prog.top_level) {
      if (const auto* fn = as<ir::Function>(tl)) {
        out.insert_or_assign(fn->name, 0);
      }
    }
    return out;
  }
  const Names statics = statics_set(*symbols);
  for (const auto& tl : prog.top_level) {
    const auto* fn = as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    asdl::Int total = 0;
    for (const auto& name : address_taken_local_names(*fn, statics)) {
      total += size_of_name(name, symbols, types);
    }
    out.insert_or_assign(fn->name, total);
  }
  return out;
}

}  // namespace c6502::passes
