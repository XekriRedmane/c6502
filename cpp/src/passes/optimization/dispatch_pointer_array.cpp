/* Inline-switch dispatch for `static const T * const arr[N]` subscript
chains.

When `arr` is a small file-scope `static const T * const[N]` whose
elements are all `AddressInit(some_named_static)`, and the program
reads `arr[i][j]` for runtime `i` and `j`, the generic c99_to_tac
lowering builds an indirect read:

    %scaled    = Binary(LeftShift, %i, 1, _)        ; i * 2
    %ptr       = IndexedLoad(arr, %scaled, _)       ; 2-byte pointer load
    %val       = IndirectIndexedLoad(%ptr, %j, _)   ; deref *(ptr + j)

That lowers to `LDA arr,X; STA dptr; LDA arr+1,X; STA dptr+1; LDA
(dptr),Y` (5 instructions) plus the `i*2` scaling, and it ties up X
(the abs,X index), Y (the (zp),Y offset), and DPTR (the indirect base)
at once.

This pass recognizes the chain and rewrites it to a CMP/BEQ dispatch on
`i`:

    JumpIfCmp(Equal, %i, ConstChar(0), case_0)
    JumpIfCmp(Equal, %i, ConstChar(1), case_1)
    ... (N-1 checks) ...
    ; fallthrough is case_{N-1}
    IndexedLoad(target_{N-1}, %j, %val)
    Jump(end)
    Label(case_0):
    IndexedLoad(target_0, %j, %val)
    Jump(end)
    ... (one block per case) ...
    Label(end):

Each case is a single direct array load, with no DPTR staging and no
pointer indirection. The dispatch frees X and Y from the dual-index
conflict, so a loop counter can stay in X across it.

Eligibility. A chain is recognized when

  * `IndirectIndexedLoad(ptr=Var(%P), index=Var(%J), dst=Var(%V))`
    appears in the function,
  * the latest in-order definition of `%P` is `IndexedLoad(name=arr,
    index=Var(%I_scaled), dst=Var(%P))`, and `%P` has exactly that one
    definition and one use,
  * the latest in-order definition of `%I_scaled` is `Binary(LeftShift,
    Var(%I), Constant(1), Var(%I_scaled))` or `Binary(Multiply,
    Var(%I), Constant(2), Var(%I_scaled))`, and `%I_scaled` has one
    definition and one use,
  * `arr` is a `StaticVariable` whose `init` is a list of
    `AddressInit(target_k, offset=0)` entries,
  * `arr`'s elements are const-qualified in the symbol table, as in
    `static T * const arr[N]` or `static const T * const arr[N]`. The
    dispatch hard-codes the initial targets, so a table whose elements
    another function can store into is left alone. The pointees may be
    writable, since each case reads the target array at run time.
    Without a symbol table no table qualifies.
  * the list length N is at most `kDispatchThreshold` (8).

Why after SSA destruction. This pass runs once, after `from_ssa`, so
each case can write to the same dst `%V` without inserting Phis. The
code after it reads `%V` unchanged.

The single-use requirement. The chain's instructions are deleted from
the IR, which would be unsound if `%I_scaled` or `%P` had other uses,
so the eligibility check requires one use of each. */
#include "passes/optimization/dispatch_pointer_array.h"

#include <cstddef>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bit_precise.h"
#include "passes/optimization/framework.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;

// The most cases dispatched inline. 8 covers every small table of the
// corpus, and the dispatch chain of a larger table (16 or more) would
// cost more than the access it replaces.
constexpr std::size_t kDispatchThreshold = 8;

using PointerArrays = OrderedMap<std::string, std::vector<std::string>>;

// Returns whether `name` is an array whose element type is
// const-qualified at its top level, so that no store can change which
// address an element holds. A volatile element doesn't qualify.
bool elements_are_const(const SymbolTable* symbols, const std::string& name) {
  if (symbols == nullptr) {
    return false;
  }
  const Symbol* sym = symbols->find(name);
  if (sym == nullptr) {
    return false;
  }
  const c99::Type_data_type* t = &sym->type;
  while (const auto* c = std::get_if<c99::Const>(t)) {
    t = &*c->referenced_type;
  }
  const auto* arr = std::get_if<c99::Array>(t);
  if (arr == nullptr) {
    return false;
  }
  const c99::Type_data_type* elem = &*arr->element_type;
  bool saw_const = false;
  while (true) {
    if (std::holds_alternative<c99::Volatile>(*elem)) {
      return false;
    }
    const auto* c = std::get_if<c99::Const>(elem);
    if (c == nullptr) {
      break;
    }
    saw_const = true;
    elem = &*c->referenced_type;
  }
  return saw_const;
}

// Maps each eligible `static const T * const[N]` to the ordered list of
// its `AddressInit` targets. A table is eligible when it has internal
// linkage, its elements are const (`elements_are_const`), and its
// `init` is a non-empty list of at most `kDispatchThreshold`
// `AddressInit(name, offset=0)` entries, pointers to other named
// statics with no offset.
PointerArrays collect_pointer_arrays(const ir::Program& prog, const SymbolTable* symbols) {
  PointerArrays out;
  for (const auto& tl : prog.top_level) {
    const auto* sv = std::get_if<ir::StaticVariable>(&tl);
    if (sv == nullptr || sv->is_global || !elements_are_const(symbols, sv->name)) {
      continue;
    }
    if (sv->init.empty() || sv->init.size() > kDispatchThreshold) {
      continue;
    }
    std::vector<std::string> targets;
    bool ok = true;
    for (const auto& entry : sv->init) {
      const auto* a = std::get_if<ir::AddressInit>(&entry);
      if (a == nullptr || a->offset != 0) {
        ok = false;
        break;
      }
      targets.push_back(a->name);
    }
    if (ok) {
      out.insert_or_assign(sv->name, std::move(targets));
    }
  }
  return out;
}

// Returns whether `val` is an integer constant equal to `target`.
bool is_int_constant(const ir::Type_val& val, asdl::Int target) {
  const auto* c = std::get_if<ir::Constant>(&val);
  if (c == nullptr) {
    return false;
  }
  const auto v = bit_precise::raw_value(c->const_);
  return v && *v == target;
}

// Returns the Var operand when `instr` scales a Var by 2, as
// `LeftShift(v, 1)` or `Multiply(v, 2)` in either operand order, and
// none otherwise.
std::optional<ir::Type_val> match_scale_by_two(const ir::Type_instruction& instr) {
  const auto* b = std::get_if<ir::Binary>(&instr);
  if (b == nullptr) {
    return std::nullopt;
  }
  if (std::holds_alternative<ir::LeftShift>(b->op)) {
    if (std::holds_alternative<ir::Var>(b->src1) && is_int_constant(b->src2, 1)) {
      return b->src1;
    }
    return std::nullopt;
  }
  if (std::holds_alternative<ir::Multiply>(b->op)) {
    if (std::holds_alternative<ir::Var>(b->src1) && is_int_constant(b->src2, 2)) {
      return b->src1;
    }
    if (std::holds_alternative<ir::Var>(b->src2) && is_int_constant(b->src1, 2)) {
      return b->src2;
    }
  }
  return std::nullopt;
}

// One chain the pass rewrites: the positions of its three
// instructions, the index operands `i` and `j`, the destination, and
// the table.
struct Chain {
  std::size_t scaled_idx = 0;
  std::size_t indexed_load_idx = 0;
  std::size_t ind_load_idx = 0;
  ir::Type_val i_val;
  ir::Type_val j_val;
  std::string v_name;
  std::string arr_name;
};

// Returns the first eligible chain of `instrs`, or none.
std::optional<Chain> find_one_chain(const framework::Instrs& instrs,
                                    const PointerArrays& pointer_arrays) {
  const auto use_count = count_uses(instrs);
  const auto uses = [&](const std::string& name) -> std::size_t {
    const std::size_t* n = use_count.find(name);
    return n == nullptr ? 0 : *n;
  };
  const auto def_of = framework::build_def_idx(instrs);
  const auto def_index = [&](const std::string& name) -> std::optional<std::size_t> {
    const auto it = def_of.find(name);
    return it == def_of.end() ? std::nullopt : std::optional<std::size_t>(it->second);
  };
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    const auto* inst = std::get_if<ir::IndirectIndexedLoad>(&instrs[k]);
    if (inst == nullptr) {
      continue;
    }
    const auto* ptr = std::get_if<ir::Var>(&inst->ptr);
    if (ptr == nullptr) {
      continue;
    }
    // A volatile pointee or pointer table is left alone. The dispatch
    // would replace one observable access with N (or none, for the
    // cases not taken), which changes the program's trace of volatile
    // accesses, a change of meaning by C99 §6.7.3.6.
    if (inst->is_volatile) {
      continue;
    }
    const std::string& p_name = ptr->name;
    if (uses(p_name) != 1) {
      continue;
    }
    const auto ptr_def_idx = def_index(p_name);
    if (!ptr_def_idx || *ptr_def_idx >= k) {
      continue;
    }
    const auto* ptr_def = std::get_if<ir::IndexedLoad>(&instrs[*ptr_def_idx]);
    if (ptr_def == nullptr || ptr_def->is_volatile || ptr_def->offset != 0) {
      continue;
    }
    if (!pointer_arrays.contains(ptr_def->name)) {
      continue;
    }
    const auto* scaled = std::get_if<ir::Var>(&ptr_def->index);
    if (scaled == nullptr || uses(scaled->name) != 1) {
      continue;
    }
    const auto scaled_def_idx = def_index(scaled->name);
    if (!scaled_def_idx || *scaled_def_idx >= *ptr_def_idx) {
      continue;
    }
    auto i_val = match_scale_by_two(instrs[*scaled_def_idx]);
    if (!i_val) {
      continue;
    }
    const auto* dst = std::get_if<ir::Var>(&inst->dst);
    if (dst == nullptr) {
      continue;
    }
    return Chain{.scaled_idx = *scaled_def_idx,
                 .indexed_load_idx = *ptr_def_idx,
                 .ind_load_idx = k,
                 .i_val = std::move(*i_val),
                 .j_val = inst->index,
                 .v_name = dst->name,
                 .arr_name = ptr_def->name};
  }
  return std::nullopt;
}

// Builds the dispatch block for `chain` and splices it into `instrs`,
// removing the chain's three instructions. The dispatch goes at the
// position of the `IndirectIndexedLoad`, the last of the three, which
// may have unrelated instructions between them.
framework::Instrs apply_dispatch(const framework::Instrs& instrs, const Chain& chain,
                                 const PointerArrays& pointer_arrays, int& label_counter) {
  const std::vector<std::string>& targets = pointer_arrays.at(chain.arr_name);
  const int n = label_counter++;
  const std::string base = ".dispatch@" + std::to_string(n);
  std::vector<std::string> case_labels;
  for (std::size_t k = 0; k < targets.size(); ++k) {
    case_labels.push_back(base + "@case@" + std::to_string(k));
  }
  const std::string end_label = base + "@end";
  const auto load = [&](const std::string& target) {
    return ir::IndexedLoad{
        .name = target, .index = chain.j_val, .dst = ir::Var{.name = chain.v_name}};
  };

  framework::Instrs dispatch;
  // The compare-and-branch chain on `i`. The last case falls through,
  // so only cases 0 to N-2 are tested.
  for (std::size_t k = 0; k + 1 < targets.size(); ++k) {
    dispatch.push_back(ir::JumpIfCmp{
        .op = ir::Equal{},
        .src1 = chain.i_val,
        .src2 = ir::Constant{.const_ = ir::ConstChar{.value = static_cast<asdl::Int>(k)}},
        .target = case_labels[k]});
  }
  dispatch.push_back(load(targets.back()));
  dispatch.push_back(ir::Jump{.target = end_label});
  for (std::size_t k = 0; k + 1 < targets.size(); ++k) {
    dispatch.push_back(ir::Label{.name = case_labels[k]});
    dispatch.push_back(load(targets[k]));
    dispatch.push_back(ir::Jump{.target = end_label});
  }
  dispatch.push_back(ir::Label{.name = end_label});

  framework::Instrs out;
  out.reserve(instrs.size() + dispatch.size());
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (k == chain.scaled_idx || k == chain.indexed_load_idx || k == chain.ind_load_idx) {
      if (k == chain.ind_load_idx) {
        for (auto& d : dispatch) {
          out.push_back(std::move(d));
        }
      }
      continue;
    }
    out.push_back(instrs[k]);
  }
  return out;
}

// Finds and rewrites eligible chains until none is left. Each rewrite
// changes the use counts, so each round counts them again.
ir::Function rewrite_function(ir::Function fn, const PointerArrays& pointer_arrays,
                              int& label_counter) {
  while (true) {
    const auto chain = find_one_chain(fn.instructions, pointer_arrays);
    if (!chain) {
      break;
    }
    fn.instructions = apply_dispatch(fn.instructions, *chain, pointer_arrays, label_counter);
  }
  return fn;
}

}  // namespace

ir::Program dispatch_const_pointer_arrays(ir::Program prog, const SymbolTable* symbols) {
  const PointerArrays pointer_arrays = collect_pointer_arrays(prog, symbols);
  if (pointer_arrays.empty()) {
    return prog;
  }
  int label_counter = 0;
  for (auto& tl : prog.top_level) {
    if (auto* fn = std::get_if<ir::Function>(&tl)) {
      *fn = rewrite_function(std::move(*fn), pointer_arrays, label_counter);
    }
  }
  return prog;
}

}  // namespace c6502::passes
