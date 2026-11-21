/* Inlines small `static` functions into their callers.

A call of a function costs the caller the stores of its arguments, a
`JSR`, and the copies of its result, and the function costs an `RTS`
and often the copies of its parameters out of their slots. For a small
function, a copy of its body in place of the call is shorter than all
of that, and the optimizer then works on the copy with the caller's own
values: an argument that is a constant folds into the body, a `bool`
result that the caller only branches on becomes the branch, and a
pointer result that the caller reads through is read where it was
computed. C programmers write such functions as function-like macros
for this reason. This pass makes that unnecessary under `-O`.

A function is a candidate when

- it's defined in this unit with internal linkage (`static`), so every
  call of it is in this unit,
- its address isn't taken (`abi_selection::address_taken`), so every
  call is a direct `FunctionCall`,
- it calls itself through no chain of calls, which a walk of the unit's
  call graph checks,
- its declaration has none of the c6502 attributes that set a calling
  convention or a placement: `[[c6502::zp_abi]]`, `section`, `address`,
  `return_bool_in_carry`, `[[noreturn]]`, a `register` or `in_*`
  parameter, a `zeropage(addr)` parameter, or a `register` local
  (`abi_selection::has_register_request`),
- its parameters and its result are scalars (an integer, a pointer,
  `float`, or `double`), so that one `Copy` moves each of them,
- its body has no `IndirectCall`, no call marked
  `[[c6502::save_hardware_stack]]`, no `GetAddress` of one of its own
  locals, and no block-scope `static` object, whose name starts with
  `@` (`identifier_resolution` renames only block-scope names).

A call of a candidate is inlined when its caller has no `register`
local, whose pin to Y the inlined body could break, when the call isn't
marked `[[c6502::save_hardware_stack]]`, and when each argument has the
width and kind (integer or floating) of its parameter, which the type
checker's conversions give every call.

Each function's calls are all inlined, or none are. They're inlined
when an estimate of the bytes that inlining adds is at most the
estimate of the bytes it removes (`Inliner::pays`). It removes, at each
of the n calls, the `JSR` and the moves of the arguments and the
result, `kCallBytes + kArgumentByteCost * a + kResultByteCost * r` for
`a` bytes of arguments and `r` of result, and once the function's
`RTS`. It adds n - 1 copies of the body beyond the one the function
held, `kBytesPerCost` bytes for each unit of `inline_cost` of the
function after the TAC optimizer, and `kEarlyReturnBytes` for each
`Ret` before the body's end at each call whose result the caller
doesn't return at once, since such a `Ret` becomes a store and a jump
where the function had an `RTS`. So a function with one call is inlined
unless it has several early returns, and one with several calls only
when its body is about as small as a call. The constants were fitted to
the example programs compiled with each candidate inlined alone, which
docs/optimizations/inline_functions.md describes. When every call of a
function has been inlined, the function is dropped from the program.

The functions are taken leaves first, so a candidate that calls another
candidate is costed and copied with that call already inlined.

In TAC, the call `FunctionCall(f, [a1, ..., an], d)` becomes

    Copy(a1, p1') ... Copy(an, pn')
    <f's body, with each local and label renamed>
    Label(end)

where `pi'` is the renamed i-th parameter. Each `Ret(v)` in the copy
becomes `Copy(v, d)` (when the call has a result and `Ret` a value) and
`Jump(end)`, which is left out after the last instruction. When the
caller's next instruction is a `Ret` of the call's result, or a `Ret`
with no value, the copy replaces that `Ret` too, and each `Ret` in the
copy stays a `Ret`, of its own value or of none. The arguments were
evaluated into the `ai` before the call in the order `c99_to_tac` gives
them, and the copies read each of them once, at the place the call
read them, so the order of side effects and of volatile accesses is
that of the call. Every instruction keeps its `is_volatile` flag, and
each renamed local keeps its symbol's type, `volatile` included. Copy
propagation removes the parameter copies where the argument isn't
written again before the parameter is read.

A renamed local is `@N.name` with a fresh N, or `%N` with a fresh N for
a temporary, so the zero-page slot names still carry the C variable's
name (passes/zp_slot_naming.py). The k-th copy's labels are `.ik_`
followed by the original label without its leading `.`, which keeps
the `_start`, `_continue`, and `_break` endings that loop rotation
reads and the `_keep` mark that dead-loop elimination reads, and its
end label is `.ik@end`.

The cost estimate runs the TAC optimizer on a copy of the callee with a
copy of the symbol table, so that the names the optimizer adds don't
reach the compile's table. Python's copy of the table is shallow and
shares its `Symbol` objects with the compile's, and no pass of the
optimizer changes a symbol in place, so a copy of the symbols as values
gives the same results.

The pass runs once, over the whole program, on the TAC from
`c99_to_tac`, in `translate_for_codegen` before every other `-O` pass,
so the TAC optimizer, ABI selection, the call graph, the carry-return
choice, zero-page promotion, and the link metadata read only the
functions that are left. It doesn't run in the unoptimized pipeline. */
#include "passes/optimization/inline_functions.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "c99_to_tac.h"
#include "errors.h"
#include "passes/abi_selection.h"
#include "passes/optimization/optimizer.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using Instrs = std::vector<ir::Type_instruction>;

// The estimates of bytes that choose whether a function's calls are
// inlined (the comment at the top). Each was fitted to the examples,
// whose units were compiled with each candidate inlined alone
// (docs/optimizations/inline_functions.md).
//
// Code bytes for each unit of `inline_cost` after the TAC optimizer.
constexpr int kBytesPerCost = 3;
// A call's `JSR`.
constexpr int kCallBytes = 3;
// Each byte of the arguments, which the call stores into the callee's
// parameter bytes and the callee reads from there.
constexpr int kArgumentByteCost = 2;
// Each byte of the result, which the callee stores and the caller reads
// back.
constexpr int kResultByteCost = 4;
// Each return before the end of the body, at a call whose result isn't
// returned at once: a store of the result and a `JMP` where the function
// had its own `RTS`.
constexpr int kEarlyReturnBytes = 4;

// The width in bytes of a scalar and whether it's floating.
struct Shape {
  int width = 0;
  bool floating = false;
  bool operator==(const Shape&) const = default;
};

const c99::Type_data_type& unqualified(const c99::Type_data_type& type) {
  const c99::Type_data_type* t = &type;
  while (true) {
    if (const auto* c = std::get_if<c99::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = std::get_if<c99::Volatile>(t)) {
      t = &*v->referenced_type;
    } else {
      return *t;
    }
  }
}

bool is_scalar(const c99::Type_data_type& type) {
  using K = c99::Kind_data_type;
  switch (c99::kind(unqualified(type))) {
    case K::Char:
    case K::SChar:
    case K::UChar:
    case K::Bool:
    case K::Int:
    case K::UInt:
    case K::Long:
    case K::ULong:
    case K::LongLong:
    case K::ULongLong:
    case K::BitInt:
    case K::Float:
    case K::Double:
    case K::Pointer:
      return true;
    default:
      return false;
  }
}

// Returns the width in bytes of the scalar type `t` and whether it's
// floating, or none for any other type.
std::optional<Shape> shape_of(const c99::Type_data_type& type) {
  const c99::Type_data_type& t = unqualified(type);
  if (!is_scalar(t)) {
    return std::nullopt;
  }
  return Shape{.width = c99_to_tac::byte_width_of(t),
               .floating = std::holds_alternative<c99::Float>(t) ||
                           std::holds_alternative<c99::Double>(t)};
}

// Returns the width of the TAC constant `c` and whether it's floating.
Shape const_shape(const ir::Type_const& c) {
  using K = ir::Kind_const;
  switch (ir::kind(c)) {
    case K::ConstChar:
    case K::ConstUChar:
      return {1, false};
    case K::ConstInt:
    case K::ConstUInt:
      return {2, false};
    case K::ConstInt24:
    case K::ConstUInt24:
      return {3, false};
    case K::ConstLong:
    case K::ConstULong:
      return {4, false};
    case K::ConstInt40:
    case K::ConstUInt40:
      return {5, false};
    case K::ConstInt48:
    case K::ConstUInt48:
      return {6, false};
    case K::ConstInt56:
    case K::ConstUInt56:
      return {7, false};
    case K::ConstLongLong:
    case K::ConstULongLong:
      return {8, false};
    case K::ConstFloat:
      return {4, true};
    case K::ConstDouble:
      return {8, true};
  }
  throw InternalError("inline_functions: a constant of no known width");
}

std::optional<Shape> val_shape(const ir::Type_val& v, const SymbolTable& symbols) {
  if (const auto* c = std::get_if<ir::Constant>(&v)) {
    return const_shape(c->const_);
  }
  const Symbol* sym = symbols.find(std::get<ir::Var>(v).name);
  return sym == nullptr ? std::nullopt : shape_of(sym->type);
}

std::vector<std::string> calls_of(const ir::Function& fn) {
  std::vector<std::string> out;
  for (const auto& ins : fn.instructions) {
    if (const auto* call = std::get_if<ir::FunctionCall>(&ins)) {
      out.push_back(call->name);
    }
  }
  return out;
}

bool is_digit(char c) { return c >= '0' && c <= '9'; }

// Splits a name of the form `@N.rest` into N and the rest, as the
// pattern `^@(\d+)\.(.*)$` does, or gives none.
std::optional<std::pair<long long, std::string_view>> at_name(std::string_view name) {
  if (name.size() < 3 || name[0] != '@') {
    return std::nullopt;
  }
  std::size_t k = 1;
  while (k < name.size() && is_digit(name[k])) {
    ++k;
  }
  if (k == 1 || k >= name.size() || name[k] != '.') {
    return std::nullopt;
  }
  return std::make_pair(std::stoll(std::string(name.substr(1, k - 1))), name.substr(k + 1));
}

// Returns N for a name of the form `%N` (the pattern `^%(\d+)$`), or
// none.
std::optional<long long> temp_name(std::string_view name) {
  if (name.size() < 2 || name[0] != '%') {
    return std::nullopt;
  }
  for (std::size_t k = 1; k < name.size(); ++k) {
    if (!is_digit(name[k])) {
      return std::nullopt;
    }
  }
  return std::stoll(std::string(name.substr(1)));
}

// Returns whether `nxt`, the instruction after `call`, is a `Ret` that
// returns the call's result, or returns nothing.
bool returns_result(const ir::FunctionCall& call, const ir::Type_instruction* nxt) {
  const auto* ret = nxt == nullptr ? nullptr : std::get_if<ir::Ret>(nxt);
  if (ret == nullptr) {
    return false;
  }
  if (!ret->val) {
    return true;
  }
  const auto* v = std::get_if<ir::Var>(&*ret->val);
  const auto* d = call.dst ? std::get_if<ir::Var>(&*call.dst) : nullptr;
  return d != nullptr && v != nullptr && d->name == v->name;
}

// Returns the object that `ins` names by a string field instead of a
// `Var`, or null.
const std::string* named_object(const ir::Type_instruction& ins) {
  if (const auto* l = std::get_if<ir::IndexedLoad>(&ins)) {
    return &l->name;
  }
  if (const auto* s = std::get_if<ir::IndexedSymbolStore>(&ins)) {
    return &s->name;
  }
  return nullptr;
}

using Renames = std::map<std::string, std::string, std::less<>>;

const std::string& renamed(const Renames& names, const std::string& name) {
  const auto it = names.find(name);
  return it == names.end() ? name : it->second;
}

// Returns a copy of `ins` with each local in `locals` and each label in
// `labels` renamed.
ir::Type_instruction rename(const ir::Type_instruction& ins, const Renames& locals,
                            const Renames& labels) {
  ir::Type_instruction out = ins;
  // `vals_in` gives pointers into `out`, which this function owns, so
  // writing through them is a write to the copy.
  for (const ir::Type_val* v : vals_in(out)) {
    if (const auto* var = std::get_if<ir::Var>(v)) {
      auto* mutable_var = const_cast<ir::Var*>(var);
      mutable_var->name = renamed(locals, mutable_var->name);
    }
  }
  using K = ir::Kind_instruction;
  switch (ir::kind(out)) {
    case K::Label: {
      auto& x = std::get<ir::Label>(out);
      x.name = renamed(labels, x.name);
      break;
    }
    case K::Jump: {
      auto& x = std::get<ir::Jump>(out);
      x.target = renamed(labels, x.target);
      break;
    }
    case K::JumpIfTrue: {
      auto& x = std::get<ir::JumpIfTrue>(out);
      x.target = renamed(labels, x.target);
      break;
    }
    case K::JumpIfFalse: {
      auto& x = std::get<ir::JumpIfFalse>(out);
      x.target = renamed(labels, x.target);
      break;
    }
    case K::JumpIfCmp: {
      auto& x = std::get<ir::JumpIfCmp>(out);
      x.target = renamed(labels, x.target);
      break;
    }
    case K::JumpIfMasked: {
      auto& x = std::get<ir::JumpIfMasked>(out);
      x.target = renamed(labels, x.target);
      break;
    }
    case K::IndexedLoad: {
      auto& x = std::get<ir::IndexedLoad>(out);
      x.name = renamed(locals, x.name);
      break;
    }
    case K::IndexedSymbolStore: {
      auto& x = std::get<ir::IndexedSymbolStore>(out);
      x.name = renamed(locals, x.name);
      break;
    }
    default:
      break;
  }
  return out;
}

ir::Type_val rename_val(const ir::Type_val& v, const Renames& locals) {
  if (const auto* var = std::get_if<ir::Var>(&v)) {
    return ir::Var{.name = renamed(locals, var->name)};
  }
  return v;
}

class Inliner {
 public:
  Inliner(const ir::Program& tac, SymbolTable& symbols, const c99::Program& prog)
      : tac_(tac), symbols_(symbols), decls_(abi_selection::function_decls_by_name(prog)) {
    for (const auto& tl : tac.top_level) {
      if (const auto* fn = std::get_if<ir::Function>(&tl)) {
        functions_.insert_or_assign(fn->name, *fn);
      }
    }
    for (const auto& [name, _] : symbols) {
      if (const auto m = at_name(name)) {
        next_at_ = std::max(next_at_, m->first + 1);
      }
      if (const auto m = temp_name(name)) {
        next_temp_ = std::max(next_temp_, *m + 1);
      }
    }
    for (const auto& [_, fn] : functions_) {
      for (const auto& ins : fn.instructions) {
        if (const auto* label = std::get_if<ir::Label>(&ins)) {
          labels_.insert(label->name);
        }
      }
    }
  }

  ir::Program run() {
    NameSet declared;
    for (const auto& [name, _] : decls_) {
      declared.insert(name);
    }
    const NameSet taken = abi_selection::address_taken(tac_, declared);
    const NameSet recursive = find_recursive();
    NameSet candidates;
    for (const auto& [name, fn] : functions_) {
      if (candidate(fn, taken, recursive)) {
        candidates.insert(name);
      }
    }
    if (candidates.empty()) {
      return tac_;
    }
    NameSet dropped;
    NameSet done;
    // Leaves first: a candidate is taken once every candidate it calls
    // has been taken, which the absence of recursion makes possible for
    // all of them.
    while (true) {
      std::vector<std::string> ready;
      for (const auto& c : candidates) {
        if (done.contains(c)) {
          continue;
        }
        bool calls_pending = false;
        for (const auto& callee : calls_of(functions_.at(c))) {
          if (candidates.contains(callee) && !done.contains(callee)) {
            calls_pending = true;
            break;
          }
        }
        if (!calls_pending) {
          ready.push_back(c);
        }
      }
      if (ready.empty()) {
        break;
      }
      for (const auto& name : ready) {
        done.insert(name);
        if (inline_all(name)) {
          dropped.insert(name);
        }
      }
    }
    if (dropped.empty() && !changed_) {
      return tac_;
    }
    ir::Program out;
    for (const auto& tl : tac_.top_level) {
      if (const auto* fn = std::get_if<ir::Function>(&tl)) {
        if (!dropped.contains(fn->name)) {
          out.top_level.push_back(functions_.at(fn->name));
        }
      } else {
        out.top_level.push_back(tl);
      }
    }
    return out;
  }

 private:
  // Returns the functions that can reach themselves through calls.
  NameSet find_recursive() const {
    std::map<std::string, NameSet, std::less<>> graph;
    for (const auto& [name, fn] : functions_) {
      NameSet callees;
      for (const auto& c : calls_of(fn)) {
        if (functions_.contains(c)) {
          callees.insert(c);
        }
      }
      graph[name] = std::move(callees);
    }
    NameSet out;
    for (const auto& [start, callees] : graph) {
      NameSet seen;
      std::vector<std::string> stack(callees.begin(), callees.end());
      while (!stack.empty()) {
        std::string n = std::move(stack.back());
        stack.pop_back();
        if (n == start) {
          out.insert(start);
          break;
        }
        if (seen.contains(n)) {
          continue;
        }
        seen.insert(n);
        const auto it = graph.find(n);
        if (it != graph.end()) {
          stack.insert(stack.end(), it->second.begin(), it->second.end());
        }
      }
    }
    return out;
  }

  const c99::Type_function_decl* decl_of(const std::string& name) const {
    const auto* d = decls_.find(name);
    return d == nullptr ? nullptr : *d;
  }

  bool candidate(const ir::Function& fn, const NameSet& taken, const NameSet& recursive) const {
    if (fn.is_global || taken.contains(fn.name) || recursive.contains(fn.name)) {
      return false;
    }
    const Symbol* sym = symbols_.find(fn.name);
    if (sym == nullptr) {
      return false;
    }
    const auto* a = std::get_if<FunAttr>(&sym->attrs);
    if (a == nullptr) {
      return false;
    }
    if (a->carry_return || a->address || a->section || a->noreturn ||
        std::any_of(a->param_zeropage.begin(), a->param_zeropage.end(),
                    [](const auto& z) { return z.has_value(); })) {
      return false;
    }
    const c99::Type_function_decl* decl = decl_of(fn.name);
    if (decl == nullptr || decl->abi_annotation) {
      return false;
    }
    if (decl->noreturn || abi_selection::has_register_request(*decl)) {
      return false;
    }
    const auto* ftype = std::get_if<c99::FunType>(&sym->type);
    if (ftype == nullptr) {
      return false;
    }
    if (!(std::holds_alternative<c99::Void>(unqualified(*ftype->ret)) || is_scalar(*ftype->ret))) {
      return false;
    }
    if (!std::all_of(ftype->params.begin(), ftype->params.end(),
                     [](const auto& p) { return is_scalar(p); })) {
      return false;
    }
    if (ftype->params.size() != fn.params.size()) {
      return false;
    }
    for (const auto& p : fn.params) {
      const Symbol* psym = symbols_.find(p);
      if (psym == nullptr) {
        return false;
      }
      const auto* local = std::get_if<LocalAttr>(&psym->attrs);
      if (local == nullptr || local->register_) {
        return false;
      }
    }
    for (const auto& ins : fn.instructions) {
      if (std::holds_alternative<ir::IndirectCall>(ins)) {
        return false;
      }
      if (const auto* call = std::get_if<ir::FunctionCall>(&ins); call != nullptr && call->hw_stack_save) {
        return false;
      }
      if (const auto* g = std::get_if<ir::GetAddress>(&ins)) {
        if (const auto* o = std::get_if<ir::Var>(&g->operand)) {
          const Symbol* osym = symbols_.find(o->name);
          if (osym == nullptr || std::holds_alternative<LocalAttr>(osym->attrs)) {
            return false;
          }
        }
      }
      for (const ir::Type_val* v : vals_in(ins)) {
        const auto* var = std::get_if<ir::Var>(v);
        if (var == nullptr) {
          continue;
        }
        const Symbol* vsym = symbols_.find(var->name);
        if (vsym == nullptr) {
          return false;
        }
        if (std::holds_alternative<StaticAttr>(vsym->attrs) && var->name.starts_with('@')) {
          return false;
        }
        if (const auto* local = std::get_if<LocalAttr>(&vsym->attrs); local != nullptr && local->register_) {
          return false;
        }
      }
      if (const std::string* name = named_object(ins)) {
        const Symbol* nsym = symbols_.find(*name);
        if (nsym == nullptr || std::holds_alternative<LocalAttr>(nsym->attrs)) {
          return false;
        }
        if (std::holds_alternative<StaticAttr>(nsym->attrs) && name->starts_with('@')) {
          return false;
        }
      }
    }
    return true;
  }

  bool site_ok(const ir::FunctionCall& call, const ir::Function& callee) const {
    if (call.hw_stack_save || call.args.size() != callee.params.size()) {
      return false;
    }
    for (std::size_t i = 0; i < call.args.size(); ++i) {
      const auto want = shape_of(symbols_.at(callee.params[i]).type);
      const auto got = val_shape(call.args[i], symbols_);
      if (!want || got != want) {
        return false;
      }
    }
    if (call.dst) {
      const auto& ret = *std::get<c99::FunType>(symbols_.at(callee.name).type).ret;
      const auto want = shape_of(ret);
      if (!want || val_shape(*call.dst, symbols_) != want) {
        return false;
      }
    }
    return true;
  }

  bool caller_ok(const ir::Function& caller) const {
    const c99::Type_function_decl* decl = decl_of(caller.name);
    return decl == nullptr || !abi_selection::has_register_local(*decl);
  }

  // Inlines every call of `name` when the choice rule (the comment at
  // the top) allows it, and returns whether no call of it is left, so
  // that it can be dropped.
  bool inline_all(const std::string& name) {
    const ir::Function callee = functions_.at(name);
    int sites = 0;
    int tails = 0;
    bool ok = true;
    for (const auto& [cname, caller] : functions_) {
      if (cname == name) {
        continue;
      }
      const Instrs& body = caller.instructions;
      for (std::size_t idx = 0; idx < body.size(); ++idx) {
        const auto* call = std::get_if<ir::FunctionCall>(&body[idx]);
        if (call == nullptr || call->name != name) {
          continue;
        }
        ++sites;
        const ir::Type_instruction* nxt = idx + 1 < body.size() ? &body[idx + 1] : nullptr;
        if (returns_result(*call, nxt)) {
          ++tails;
        }
        if (!(caller_ok(caller) && site_ok(*call, callee))) {
          ok = false;
        }
      }
    }
    if (sites == 0 || !ok) {
      return false;
    }
    if (!pays(callee, sites, tails)) {
      return false;
    }
    for (auto& [cname, caller] : functions_) {
      if (cname == name) {
        continue;
      }
      const bool calls = std::any_of(caller.instructions.begin(), caller.instructions.end(),
                                     [&](const auto& i) {
                                       const auto* c = std::get_if<ir::FunctionCall>(&i);
                                       return c != nullptr && c->name == name;
                                     });
      if (!calls) {
        continue;
      }
      Instrs out;
      const Instrs& body = caller.instructions;
      std::size_t idx = 0;
      while (idx < body.size()) {
        const auto& ins = body[idx];
        ++idx;
        const auto* call = std::get_if<ir::FunctionCall>(&ins);
        if (call == nullptr || call->name != name) {
          out.push_back(ins);
          continue;
        }
        const ir::Type_instruction* nxt = idx < body.size() ? &body[idx] : nullptr;
        Instrs copied;
        if (returns_result(*call, nxt)) {
          copied = copy(callee, *call, &std::get<ir::Ret>(*nxt));
          ++idx;
        } else {
          copied = copy(callee, *call, nullptr);
        }
        for (auto& c : copied) {
          out.push_back(std::move(c));
        }
      }
      caller.instructions = std::move(out);
      changed_ = true;
    }
    return true;
  }

  // Returns whether inlining all `sites` calls of `callee`, `tails` of
  // them in tail position, is estimated to take no more bytes than the
  // calls and the function (the comment at the top).
  bool pays(const ir::Function& callee, int sites, int tails) {
    const auto& ftype = std::get<c99::FunType>(symbols_.at(callee.name).type);
    long long arg_bytes = 0;
    for (const auto& t : ftype.params) {
      arg_bytes += shape_of(t)->width;
    }
    const auto ret = shape_of(*ftype.ret);
    const long long call =
        kCallBytes + kArgumentByteCost * arg_bytes + kResultByteCost * (ret ? ret->width : 0);
    long long early = 0;
    for (std::size_t idx = 0; idx < callee.instructions.size(); ++idx) {
      if (std::holds_alternative<ir::Ret>(callee.instructions[idx]) &&
          idx != callee.instructions.size() - 1) {
        ++early;
      }
    }
    long long added = static_cast<long long>(kEarlyReturnBytes) * early * (sites - tails);
    if (sites > 1) {
      added += static_cast<long long>(sites - 1) * kBytesPerCost * optimized_cost(callee);
    }
    return added <= sites * call + 1;
  }

  // Returns `inline_cost` of a copy of `callee` after the TAC optimizer,
  // which drops what the unoptimized TAC carries and an inlined copy
  // wouldn't keep either. The copy's new names go in a copy of the
  // symbol table.
  int optimized_cost(const ir::Function& callee) const {
    SymbolTable table = symbols_;
    const ir::Function opt = optimize_function(callee, &table);
    return inline_cost(opt, table);
  }

  std::string fresh_local(const std::string& name) {
    // The names are appended piece by piece, since GCC 12 reports a
    // false -Wrestrict for `"@" + std::to_string(n)`.
    std::string fresh;
    if (const auto m = at_name(name)) {
      fresh = '@';
      fresh += std::to_string(next_at_);
      fresh += '.';
      fresh += m->second;
      ++next_at_;
    } else {
      fresh = '%';
      fresh += std::to_string(next_temp_);
      ++next_temp_;
    }
    if (symbols_.contains(fresh)) {
      throw InternalError(fresh);
    }
    const Symbol old = symbols_.at(name);
    symbols_.insert_or_assign(fresh, old);
    return fresh;
  }

  // Returns the instructions that replace `call` (the comment at the
  // top). `tail` is the caller's `Ret` right after the call when it
  // returns the call's result or nothing (`returns_result`), and then
  // the copy replaces both, and each `Ret` in it stays a `Ret`.
  Instrs copy(const ir::Function& callee, const ir::FunctionCall& call, const ir::Ret* tail) {
    const int k = copies_++;
    const std::string prefix = ".i" + std::to_string(k) + "_";
    const std::string end = ".i" + std::to_string(k) + "@end";
    if (labels_.contains(end)) {
      throw InternalError(end);
    }
    Renames locals;
    const auto local = [&](const std::string& name) {
      if (!locals.contains(name)) {
        std::string fresh = fresh_local(name);
        locals.emplace(name, std::move(fresh));
      }
    };
    for (const auto& p : callee.params) {
      local(p);
    }
    for (const auto& ins : callee.instructions) {
      for (const ir::Type_val* v : vals_in(ins)) {
        const auto* var = std::get_if<ir::Var>(v);
        if (var == nullptr) {
          continue;
        }
        const Symbol* sym = symbols_.find(var->name);
        if (sym != nullptr && std::holds_alternative<LocalAttr>(sym->attrs)) {
          local(var->name);
        }
      }
    }
    Renames labels;
    for (const auto& ins : callee.instructions) {
      if (const auto* label = std::get_if<ir::Label>(&ins)) {
        std::string_view stripped = label->name;
        while (!stripped.empty() && stripped.front() == '.') {
          stripped.remove_prefix(1);
        }
        std::string fresh = prefix + std::string(stripped);
        if (labels_.contains(fresh)) {
          throw InternalError(fresh);
        }
        labels_.insert(fresh);
        labels.insert_or_assign(label->name, std::move(fresh));
      }
    }
    labels_.insert(end);

    Instrs out;
    for (std::size_t i = 0; i < call.args.size() && i < callee.params.size(); ++i) {
      out.push_back(ir::Copy{.src = call.args[i], .dst = ir::Var{.name = locals.at(callee.params[i])}});
    }
    bool jumped = false;
    const Instrs& body = callee.instructions;
    for (std::size_t idx = 0; idx < body.size(); ++idx) {
      const auto& ins = body[idx];
      if (const auto* ret = std::get_if<ir::Ret>(&ins)) {
        if (tail != nullptr) {
          ir::Ret r;
          if (tail->val && ret->val) {
            r.val = rename_val(*ret->val, locals);
          }
          out.push_back(std::move(r));
          continue;
        }
        if (call.dst && ret->val) {
          out.push_back(ir::Copy{.src = rename_val(*ret->val, locals), .dst = *call.dst});
        }
        if (idx != body.size() - 1) {
          out.push_back(ir::Jump{.target = end});
          jumped = true;
        }
        continue;
      }
      out.push_back(rename(ins, locals, labels));
    }
    if (jumped) {
      out.push_back(ir::Label{.name = end});
    }
    return out;
  }

  const ir::Program& tac_;
  SymbolTable& symbols_;
  OrderedMap<std::string, const c99::Type_function_decl*> decls_;
  OrderedMap<std::string, ir::Function> functions_;
  std::set<std::string, std::less<>> labels_;
  int copies_ = 0;
  // The next free N for `@N.name` and `%N`.
  long long next_at_ = 0;
  long long next_temp_ = 0;
  // Whether a caller was rewritten.
  bool changed_ = false;
};

}  // namespace

int inline_cost(const ir::Function& fn, const SymbolTable& symbols) {
  int total = 0;
  const Instrs& body = fn.instructions;
  for (std::size_t idx = 0; idx < body.size(); ++idx) {
    const auto& ins = body[idx];
    if (std::holds_alternative<ir::Label>(ins)) {
      continue;
    }
    if (std::holds_alternative<ir::Ret>(ins) && idx == body.size() - 1) {
      continue;
    }
    if (std::holds_alternative<ir::FunctionCall>(ins) || std::holds_alternative<ir::IndirectCall>(ins)) {
      total += 3;
      continue;
    }
    int widest = 0;
    for (const ir::Type_val* v : vals_in(ins)) {
      if (const auto s = val_shape(*v, symbols); s && s->width > widest) {
        widest = s->width;
      }
    }
    total += widest != 0 ? widest : 1;
  }
  return total;
}

ir::Program inline_functions(ir::Program tac, SymbolTable& symbols, const c99::Program& prog) {
  return Inliner(tac, symbols, prog).run();
}

}  // namespace c6502::passes
