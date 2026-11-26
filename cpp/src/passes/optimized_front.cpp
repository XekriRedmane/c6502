// compile.py's `_optimized_front` after the TAC optimizer, the `-O`
// pipeline from `select_abi` through `synthesize_prologue`.
//
// The ABI comes first, then the carry-flag returns, the functions that
// get private pools (with the cycles whose recursive calls save on the
// hardware stack), the values kept in a callee's parameter bytes across
// calls, and the addresses of the zp_abi slots. `tac_to_asm` lowers the
// TAC with all of those, and the asm optimizer runs twice: once with
// the default caller/callee partition to size each function's demand
// for body-local bytes, and once more for each allocation of the pools.
// A pool as large as the first run's count of bytes can still be too
// small, because a pointer read through `(zp),Y` needs two adjacent
// bytes, which the bytes left free in a pool of that size may not have.
// A function that then spills a value the first run kept in zero page
// gets a byte more for each such value, and the pools are allocated
// again, at most `kPoolRetries` times. The address-taken locals of the
// functions with pools go in the pool bytes the coloring left free, the
// pseudos are replaced, the slots are named, each `LoadAddress` of a
// `Data` source is split, and the prologues are synthesized.
//
// Each call takes the arguments compile.py gives it, in compile.py's
// order, since several of them change the symbol table that later ones
// read.
#include "passes/optimized_front.h"

#include <utility>

#include "pass_order.h"
#include "passes/address_taken_zp.h"
#include "passes/carry_return_choice.h"
#include "passes/function_local_sizing.h"
#include "passes/ir_check.h"
#include "passes/lower_data_load_address.h"
#include "passes/optimization/pool.h"
#include "passes/optimization_asm/optimizer.h"
#include "passes/prologue_synthesis.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/zp_slot_allocation.h"
#include "tac_to_asm.h"

namespace c6502::passes {

namespace {

// How many times the pools of the functions whose final register
// allocation spilled more than the preliminary one are grown
// (`compile._POOL_RETRIES`).
constexpr int kPoolRetries = 3;

// The runtime's symbols that `sim.assembler.DEFAULT_ZP_SYMBOLS` names,
// which count as zero-page bytes wherever the block goes.
constexpr const char* kRuntimeSymbols[] = {"SSP", "FP", "HARGS", "DPTR"};

// Returns each function with a private pool mapped to the number of
// byte values its final coloring spilled that its preliminary coloring
// didn't, leaving out the functions with none
// (`compile._pool_shortfalls`).
OrderedMap<std::string, asdl::Int> pool_shortfalls(const OrderedMap<std::string, Coloring>& prelim,
                                                   const OrderedMap<std::string, Coloring>& final_,
                                                   const LocalPools& local_pools) {
  OrderedMap<std::string, asdl::Int> out;
  for (const auto& [fn, coloring] : final_) {
    const Coloring* before = prelim.find(fn);
    if (!local_pools.contains(fn) || before == nullptr) {
      continue;
    }
    asdl::Int extra = 0;
    for (const auto& name : coloring.spilled) {
      if (!before->spilled.contains(name)) {
        ++extra;
      }
    }
    if (extra != 0) {
      out.insert_or_assign(fn, extra);
    }
  }
  return out;
}

// Returns the functions `ret_slot_coalescing` may change: those with
// private pools, off every cycle, that make no `save_hardware_stack`
// call (`compile._ret_coalescing_eligible`).
NameSet ret_coalescing_eligible(const tac_ast::Program& tac, const NameSet& pooled,
                                const Cycles& recursion) {
  NameSet saving;
  for (const auto& tl : tac.top_level) {
    const auto* fn = asdl::as<tac_ast::Function>(tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto& ins : fn->instructions) {
      const auto* call = asdl::as<tac_ast::FunctionCall>(ins);
      if (call != nullptr && call->hw_stack_save) {
        saving.insert(fn->name);
        break;
      }
    }
  }
  NameSet out;
  for (const auto& name : pooled) {
    if (!recursion.contains(name) && !saving.contains(name)) {
      out.insert(name);
    }
  }
  return out;
}

// Returns `{**first, **second}`.
OrderedMap<std::string, asdl::Int> merged(OrderedMap<std::string, asdl::Int> first,
                                          const OrderedMap<std::string, asdl::Int>& second) {
  for (const auto& [name, addr] : second) {
    first.insert_or_assign(name, addr);
  }
  return first;
}

}  // namespace

OptimizedFront optimized_front(const tac_ast::Program& tac, const c99_ast::Program& c99,
                               SymbolTable& symbols, const TypeTable& types, const ZpGlobals& zp,
                               const NameSet& statics, const AsmContext& ctx) {
  OptimizedFront out;
  for (const auto& [name, sym] : symbols) {
    const auto* fun = std::get_if<FunAttr>(&sym.attrs);
    if (fun != nullptr && fun->noreturn) {
      out.noreturn.insert(name);
    }
  }
  const pass_order::Run pass_run("optimized_front");
  pass_order::ran("select_abi");
  Abi abi = select_abi(tac, c99, &types);
  // Every zp_abi `bool` function with external linkage returns it in the
  // carry flag, which every unit decides from the declaration alone, and
  // so does a static one whose calls all branch on its result or return
  // it from a function that does too (passes/carry_return_choice.py).
  choose_global_carry_returns(tac, symbols, abi);
  choose_carry_returns(tac, symbols);
  // Functions on a cycle whose recursive calls are all under
  // `[[c6502::save_hardware_stack]]` get private pools too, and save
  // their live values around those calls (passes/recursion_saves.py).
  const Cycles recursion = recursive_pool_functions(tac, abi);
  const NameSet pooled = pool_eligible(tac, abi);
  NameSet recursion_names;
  for (const auto& [name, members] : recursion) {
    recursion_names.insert(name);
  }
  NameSet fixed;
  for (const auto& [name, addr] : zp.fixed) {
    fixed.insert(name);
  }
  // Values kept in a callee's parameter bytes across calls
  // (passes/slot_sharing.py).
  auto [shares, bindings] = find_shares(tac, abi, symbols, &types, pooled, fixed, recursion_names);
  // When every function here has a private pool, none keeps a value in
  // the pool's callee-saved half across a call, so the slots and pools
  // can come from the whole pool, as they do when linking. And, as when
  // linking, the free bytes below the pool, around the runtime's block
  // and the zero-page objects, take what doesn't fit there before
  // anything spills out of zero page.
  bool all_pooled = true;
  for (const auto& tl : tac.top_level) {
    const auto* fn = asdl::as<tac_ast::Function>(tl);
    if (fn != nullptr && !pooled.contains(fn->name)) {
      all_pooled = false;
      break;
    }
  }
  ZpAllocationOptions options;
  options.reserved = zp.reserved;
  if (all_pooled) {
    options.window = Range{.start = Pool{}.start, .stop = 0x100};
    options.fallback = Range{.start = 0, .stop = Pool{}.start};
    for (const asdl::Int b : ctx.layout.bytes()) {
      options.reserved.insert(b);
    }
  }
  options.shares = shares;
  auto [allocated, zp_slot_symbols] = allocate_zp_slots(tac, abi, options);
  abi = std::move(allocated);
  // A `zeropage(addr)` parameter that something the function calls
  // might overwrite is copied into a local at entry.
  const tac_to_asm::PinnedCopies pinned_copies =
      pinned_param_copies(tac, abi, symbols, &types, fixed);
  const tac_to_asm::SlotAliases aliases = slot_aliases(shares);
  const tac_to_asm::Options lowering{.bare_exit = true,
                                     .abi = &abi,
                                     .recursion_saves = &recursion,
                                     .bindings = &bindings,
                                     .pinned_copies = &pinned_copies,
                                     .slot_aliases = &aliases};
  asm_ast::Program asm0 = tac_to_asm::translate_program(tac, &symbols, &types, ctx, lowering);
  ir_check::check_asm(asm0, "tac_to_asm", /*pseudos=*/true);
  // The preliminary run of the optimizer, with the default caller/callee
  // partition, only sizes each function's demand for local bytes, and
  // its program is dropped.
  AsmOptimizerOptions prelim_options;
  prelim_options.extra_statics = statics;
  prelim_options.param_layouts = &abi;
  prelim_options.symbols = &symbols;
  prelim_options.reserved_zp = zp.reserved;
  prelim_options.recursion_saves = &recursion;
  const OptimizedAsmProgram prelim = optimize_program(asm0, prelim_options);
  const OrderedMap<std::string, asdl::Int> local_bytes = compute_local_bytes(prelim.prog);
  // The address-taken locals of a function with a private pool go in
  // the pool's bytes, where `&local` is a 2-byte constant, so the pool
  // is sized for them too.
  const OrderedMap<std::string, asdl::Int> addr_taken_bytes =
      compute_address_taken_bytes(prelim.prog, &symbols, &types);
  // A recursive function's address-taken locals stay in its frame, one
  // copy per call. Python builds this dict from a set of names, and the
  // allocator only looks the names up, so the order here is the sizing's.
  OrderedMap<std::string, asdl::Int> combined;
  for (const auto& [fn, n] : local_bytes) {
    combined.insert_or_assign(fn, n);
  }
  for (const auto& [fn, n] : addr_taken_bytes) {
    if (!combined.contains(fn)) {
      combined.insert_or_assign(fn, 0);
    }
  }
  for (auto& [fn, n] : combined) {
    if (!recursion.contains(fn)) {
      if (const asdl::Int* extra = addr_taken_bytes.find(fn)) {
        n += *extra;
      }
    }
  }
  // The final run, with the private pools given to the register
  // allocation, repeated with larger pools while a function spills more
  // than the preliminary run did.
  LocalPools local_pools;
  OptimizedAsmProgram final_;
  for (int attempt = 0; attempt <= kPoolRetries; ++attempt) {
    pass_order::ran("allocate_function_locals");
    local_pools = allocate_function_locals(tac, abi, combined, options);
    AsmOptimizerOptions final_options = prelim_options;
    final_options.local_pools = &local_pools;
    final_ = optimize_program(asm0, final_options);
    const auto grown = pool_shortfalls(prelim.colorings, final_.colorings, local_pools);
    if (grown.empty()) {
      break;
    }
    for (const auto& [fn, extra] : grown) {
      combined[fn] += extra;
    }
  }
  // Each address-taken local of a function with a private pool, off
  // every cycle, gets the pool bytes the coloring left free. A local
  // that doesn't fit stays out and gets a frame slot from
  // `replace_pseudoregisters`.
  LocalPools acyclic_pools;
  for (const auto& [fn, pool] : local_pools) {
    if (!recursion.contains(fn)) {
      acyclic_pools.insert_or_assign(fn, pool);
    }
  }
  const AddressTakenAssignments assignments = compute_address_taken_assignments(
      final_.prog, acyclic_pools, final_.colorings, &symbols, &types);
  AddressTakenSymbols assignment_symbols;
  for (const auto& [fn, by_name] : assignments) {
    assignment_symbols.insert_or_assign(
        fn, address_taken_zp::slot_symbols(fn, by_name, &symbols, &types));
  }
  const BareExitTables tables{.param_layouts = &abi,
                              .local_pools = &local_pools,
                              .address_taken_assignments = &assignments,
                              .address_taken_symbols = &assignment_symbols};
  ir_check::check_asm(final_.prog, "the asm optimizer", /*pseudos=*/true);
  auto [asm1, dims_by_fn] = replace_program_bare_exit(std::move(final_.prog), statics, &symbols,
                                                      &types, &final_.colorings, tables);
  // The names of the pools' bytes, which the EQU table and the link
  // metadata share, and the EQU table of the zp_abi slots and the pools.
  LocalSlots slots = local_slots(local_pools, final_.colorings, &assignments, &assignment_symbols);
  out.slot_symbols = merged(zp_slot_symbols, slots.slot_symbols);
  // Each `LoadAddress(src=Data, dst=mem)` becomes two moves of the
  // label's halves, so that memory value propagation can fold the round
  // trip through `dst`. The high byte is written under the name the pool
  // gives that byte, which is the name its reads use.
  ByteNamesByFunction byte_names;
  for (const auto& [fn, names] : slots.slot_names_by_fn) {
    OrderedMap<asdl::Int, std::string> by_byte;
    const std::vector<asdl::Int>& pool = local_pools.at(fn);
    for (std::size_t k = 0; k < pool.size() && k < names.size(); ++k) {
      by_byte.insert_or_assign(pool[k], names[k]);
    }
    byte_names.insert_or_assign(fn, std::move(by_byte));
  }
  pass_order::ran("lower_data_load_address");
  asm1 = lower_data_load_address(std::move(asm1), &out.slot_symbols, &byte_names);
  pass_order::ran("synthesize_prologue");
  out.prog = synthesize_program(std::move(asm1), dims_by_fn);
  // The names that are zero-page bytes, for the sizes of the code that
  // `apply_shared_tails` and `apply_outline_runs` move.
  for (const auto& [name, addr] : merged(out.slot_symbols, zp.addrs)) {
    if (addr <= 0xFF) {
      out.zp_names.insert(name);
    }
  }
  for (const char* name : kRuntimeSymbols) {
    out.zp_names.insert(name);
  }
  // Neither moves a `(zp),Y` pointer in a slot's bytes into another
  // function, where `asm_emit` would name it by that function's slot.
  for (const auto& [name, addr] : out.slot_symbols) {
    if (addr <= 0xFF) {
      out.slot_addrs.insert(addr);
    }
  }
  out.ret_eligible = ret_coalescing_eligible(tac, pooled, recursion);
  out.abi = std::move(abi);
  out.local_pools = std::move(local_pools);
  out.slot_names_by_fn = std::move(slots.slot_names_by_fn);
  out.shares = std::move(shares);
  out.bindings = std::move(bindings);
  return out;
}

}  // namespace c6502::passes
