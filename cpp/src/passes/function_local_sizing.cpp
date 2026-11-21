// Sizes the zero-page locals of each function.
//
// `compute_local_bytes` counts how many distinct zero-page bytes the
// body locals of each function take after the asm optimizer's register
// allocation. Its input is the program `optimize_program` returns, in
// which `apply_coloring` has rewritten every pseudo the allocation
// colored as a `ZP(addr, 0)` operand, and the pseudos left (parameters,
// locals whose address is taken, spilled values, and the names pinned to
// X or Y) aren't resolved yet, which `replace_pseudoregisters_bare_exit`
// does later. So the bytes counted are those of the coloring of the body
// locals, which is the size of the private range the allocator of local
// slots gives each function.
//
// These operands aren't counted.
//
// - `Pseudo` operands are the parameters, the locals whose address is taken, and the
//   spilled locals, which take frame slots in RAM later and so don't
//   compete for zero page.
// - `Data` operands are references to static storage (user statics, the runtime's
//   `HARGS` and `DPTR`, and the `__zpabi_*` slots), which are at
//   addresses of their own or are sized by other passes.
// - `Reg` operands are the registers.
// - An `IndexedData`'s base is a static resolved at link time.
// - `Frame`, `Stack`, and `IndirectY` address RAM.
// - `Imm`, `ImmLabelLow`, and `ImmLabelHigh` are immediates.
//
// Each `ZP(addr, offset)` is counted. After the coloring, the only
// `ZP` operands of a body come from the coloring of its pseudo locals,
// so the set of their `addr + offset` is the set of bytes the function
// uses. `zp_slot_allocation` reads the count to size each function's
// private range.
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

// Returns the zero-page bytes the operands of `fn` name, `address +
// offset` of each `ZP`.
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

// Returns the names of the symbol table's objects with static storage.
Names statics_set(const SymbolTable& symbols) {
  Names out;
  for (const auto& [name, sym] : symbols) {
    if (std::holds_alternative<StaticAttr>(sym.attrs)) {
      out.insert(name);
    }
  }
  return out;
}

// Returns the names of the pseudos that are the source of a
// `LoadAddress` in `fn`, each once, in the order of their first
// occurrence, leaving out the parameters, which have frame storage of
// their own, and the statics. The order makes the slot symbols made from
// them the same in every compile.
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
