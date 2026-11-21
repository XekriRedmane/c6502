// Allocates zero-page bytes for the address-taken locals of functions
// with private pools, ported from passes/address_taken_zp.py.
//
// The case that motivated it is `entity_proximity` in
// examples/companion_update.c, which declares a local `uint8_t
// entity_row` and passes its address to `find_active_entity`. A local
// whose address is taken needs storage with a fixed address, which used
// to put it in a frame slot from `replace_pseudoregisters`, with the
// soft-stack prologue and epilogue that come with a frame and an
// `LDY; (FP),Y` read at every access.
//
// A zp_abi function on the path of call-graph-disjoint pools doesn't
// need that. No code of a function that can be live at the same time
// touches its private pool, so the local can go in a byte of the pool.
// `&entity_row` in the caller is then a 2-byte immediate load (the low
// byte the address, the high byte $00) where it was a 6-byte `FP + off`
// addition at run time, `find_active_entity`'s `STA (DPTR),Y` writes the
// byte directly, and the function needs no frame, prologue, or
// epilogue.
//
// For each function the allocation takes these steps.
//
//   1. Scan the asm for `LoadAddress` instructions whose `src` is a
//      `Pseudo`. Each such name is a candidate, except a parameter or a
//      static, which has storage of its own.
//   2. Size each candidate from the symbol table.
//   3. Take the function's pool, `local_pools[fn]`, whose size the
//      caller grew by the address-taken bytes before the allocation.
//   4. Leave out the bytes the register allocation colored
//      (`coloring.assignments`, across each value's width).
//   5. Walk the free bytes in order, split into runs of consecutive
//      addresses, and give each candidate the first run with room for
//      it. The first byte of that run is the candidate's address.
//
// A candidate with no run long enough gets no address, and the caller
// gives it a frame slot as before.
//
// The register allocation doesn't place these locals itself, for two
// reasons. It works on the byte pseudos SSA renaming makes, and
// address-taken locals are left out of the renaming so that they keep
// one identity. Adding them would change the rules of what SSA leaves
// out and every later pass that relies on an address-taken name being
// an unrenamed pseudo. And its interference graph is built per byte,
// while the bytes of an address-taken local have to stay together as
// one addressable block, which would take constraints the graph doesn't
// have. An allocation after the coloring gives these locals the pool
// bytes the coloring didn't use, with no interference to model.

#include "passes/address_taken_zp.h"

#include <set>
#include <vector>

#include "errors.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/zp_slot_naming.h"

namespace c6502::passes {

namespace ir = ::c6502::asm_ast;

namespace {

NameSet statics_set(const SymbolTable& symbols) {
  NameSet out;
  for (const auto& [name, sym] : symbols) {
    if (std::holds_alternative<StaticAttr>(sym.attrs)) {
      out.insert(name);
    }
  }
  return out;
}

// Returns every zero-page byte the coloring takes, each colored pseudo's
// base and the bytes of its width (`size_of_name`) after it. Leaving
// out the bytes after the base would let an address-taken local overlap
// the upper bytes of a colored value wider than one byte.
std::set<asdl::Int> coloring_used_bytes(const Coloring* coloring, const SymbolTable* symbols,
                                        const TypeTable* types) {
  std::set<asdl::Int> out;
  if (coloring == nullptr || coloring->assignments.empty()) {
    return out;
  }
  for (const auto& [name, addr] : coloring->assignments) {
    const asdl::Int size = size_of_name(name, symbols, types);
    for (asdl::Int k = 0; k < size; ++k) {
      out.insert(addr + k);
    }
  }
  return out;
}

// Returns the names of the pseudos that are the `src` of a
// `LoadAddress` in `fn`, without its parameters and the statics, in the
// order of their first appearance, so that the allocation is the same
// on every run.
std::vector<std::string> address_taken_names(const ir::Function& fn, const NameSet& statics) {
  std::set<std::string, std::less<>> seen;
  std::vector<std::string> out;
  const std::set<std::string, std::less<>> param_set(fn.params.begin(), fn.params.end());
  for (const auto& instr : fn.instructions) {
    const auto* la = asdl::as<ir::LoadAddress>(instr);
    if (la == nullptr) {
      continue;
    }
    const auto* pseudo = asdl::as<ir::Pseudo>(la->src);
    if (pseudo == nullptr) {
      continue;
    }
    const std::string& name = pseudo->name;
    if (param_set.contains(name) || statics.contains(name) || seen.contains(name)) {
      continue;
    }
    seen.insert(name);
    out.push_back(name);
  }
  return out;
}

OrderedMap<std::string, asdl::Int> allocate_in_pool(const ir::Function& fn,
                                                    const std::vector<asdl::Int>& pool,
                                                    const std::set<asdl::Int>& used,
                                                    const NameSet& statics,
                                                    const SymbolTable* symbols,
                                                    const TypeTable* types) {
  OrderedMap<std::string, asdl::Int> out;
  const auto candidates = address_taken_names(fn, statics);
  if (candidates.empty()) {
    return out;
  }
  // The free bytes, ascending, split into runs of consecutive
  // addresses. A run is consumed from its front.
  const std::set<asdl::Int> pool_set(pool.begin(), pool.end());
  std::vector<std::vector<asdl::Int>> runs;
  for (const asdl::Int& a : pool_set) {
    if (used.contains(a)) {
      continue;
    }
    if (!runs.empty() && runs.back().back() + 1 == a) {
      runs.back().push_back(a);
    } else {
      runs.push_back({a});
    }
  }
  for (const auto& name : candidates) {
    const asdl::Int size = size_of_name(name, symbols, types);
    // The first run with room for the local, first fit.
    for (auto& run : runs) {
      if (static_cast<asdl::Int>(run.size()) >= size) {
        if (run.empty()) {
          // A local of no bytes (an array of length 0) meets an empty
          // run, where Python's `run[0]` raises IndexError.
          throw InternalError("address_taken_zp: list index out of range");
        }
        out.insert_or_assign(name, run.front());
        run.erase(run.begin(), run.begin() + static_cast<std::ptrdiff_t>(size));
        break;
      }
    }
  }
  return out;
}

// Throws when an address-taken local of `fn` got no pool bytes while the
// coloring keeps a value in Y. The allocation counted the local as a
// zero-page byte (`frame_names` in optimizer.cpp), so it may have kept a
// value in Y across an access to it, and an access to the local's frame
// slot loads Y with the slot's offset.
void check_no_frame_under_y(const ir::Function& fn, const OrderedMap<std::string, asdl::Int>& placed,
                            const Coloring* coloring, const NameSet& statics) {
  if (coloring == nullptr) {
    return;
  }
  bool y_used = false;
  for (const auto& [name, letter] : coloring->hwreg_assignments) {
    y_used = y_used || letter == "Y";
  }
  if (!y_used) {
    return;
  }
  // Python's repr of the list of names.
  std::string missing;
  for (const auto& name : address_taken_names(fn, statics)) {
    if (placed.find(name) == nullptr) {
      missing += (missing.empty() ? "'" : ", '") + name + "'";
    }
  }
  if (!missing.empty()) {
    throw InternalError("function '" + fn.name + "': the address-taken locals [" + missing +
                        "] didn't fit in the function's pool, and the coloring keeps a value "
                        "in Y, which an access to their frame slots would overwrite");
  }
}

}  // namespace

AddressTakenAssignments compute_address_taken_assignments(const ir::Program& prog,
                                                          const LocalPools& local_pools,
                                                          const ColoringsByFunction& colorings,
                                                          const SymbolTable* symbols,
                                                          const TypeTable* types) {
  AddressTakenAssignments out;
  const NameSet statics = symbols != nullptr ? statics_set(*symbols) : NameSet{};
  for (const auto& tl : prog.top_level) {
    const auto* fn = asdl::as<ir::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    const auto* pool = local_pools.find(fn->name);
    if (pool == nullptr) {
      out.insert_or_assign(fn->name, {});
      continue;
    }
    const Coloring* coloring = colorings.find(fn->name);
    const auto used = coloring_used_bytes(coloring, symbols, types);
    auto placed = allocate_in_pool(*fn, *pool, used, statics, symbols, types);
    check_no_frame_under_y(*fn, placed, coloring, statics);
    out.insert_or_assign(fn->name, std::move(placed));
  }
  return out;
}

namespace address_taken_zp {

OrderedMap<std::string, std::string> slot_symbols(std::string_view fn_name,
                                                  const OrderedMap<std::string, asdl::Int>& assignments,
                                                  const SymbolTable*, const TypeTable*) {
  OrderedMap<std::string, std::string> out;
  OrderedMap<std::string, asdl::Int> used;
  const std::string prefix = "__local_" + std::string(fn_name) + "__";
  for (const auto& [pname, addr] : assignments) {
    const auto [source, byte_index] = parse_pseudo_name(pname);
    if (!source) {
      // A compiler temporary takes a name made from the pseudo's.
      out.insert_or_assign(pname, prefix + "addr_" + pname);
      continue;
    }
    std::string name = prefix + *source;
    asdl::Int& count = used[name];
    count += 1;
    if (count > 1) {
      name += "@" + asdl::int_to_string(count);
    }
    out.insert_or_assign(pname, std::move(name));
  }
  return out;
}

}  // namespace address_taken_zp

}  // namespace c6502::passes
