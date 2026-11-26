#include "passes/abi_selection.h"

#include <algorithm>
#include <string_view>
#include <utility>

#include "errors.h"
#include "passes/call_edges.h"
#include "passes/optimization/var_visit.h"
#include "passes/recursion_saves.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/zp_local_allocation.h"
#include "passes/zp_slot_naming.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace tac = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

using Markers = std::vector<std::string>;
using Annotations = OrderedMap<std::string, std::optional<std::string>>;
using Decls = OrderedMap<std::string, const c99::Type_function_decl*>;

[[noreturn]] void fail(const std::string& message) {
  throw CompileError("AbiSelectionError", message);
}

bool is_abi_error(const CompileError& e) { return e.python_type() == "AbiSelectionError"; }

Range zp_param_window(const Pool& pool) { return pool.caller_saved(); }

std::string hex2(asdl::Int v) { return pyrepr::hex(v, 2); }

const c99::FunType* fun_type_of(const c99::Type_function_decl& fn_decl) {
  return as<c99::FunType>(fn_decl.data_type);
}

asdl::Int param_byte_count(const c99::Type_function_decl& fn_decl, const TypeTable* types) {
  const auto* ft = fun_type_of(fn_decl);
  if (ft == nullptr) {
    return 0;
  }
  asdl::Int total = 0;
  for (const auto& p : ft->params) {
    total += sizeof_type(p, types);
  }
  return total;
}

std::vector<asdl::Int> per_param_byte_sizes(const c99::Type_function_decl& fn_decl,
                                            const TypeTable* types) {
  std::vector<asdl::Int> out;
  if (const auto* ft = fun_type_of(fn_decl)) {
    for (const auto& p : ft->params) {
      out.push_back(sizeof_type(p, types));
    }
  }
  return out;
}

constexpr char kArgRegisters[] = {'A', 'X'};

const c99::Type_data_type& strip_quals(const c99::Type_data_type& t) {
  const c99::Type_data_type* cur = &t;
  while (true) {
    if (const auto* c = as<c99::Const>(*cur)) {
      cur = &*c->referenced_type;
    } else if (const auto* v = as<c99::Volatile>(*cur)) {
      cur = &*v->referenced_type;
    } else {
      return *cur;
    }
  }
}

bool is_integer_type(const c99::Type_data_type& t) {
  return isa<c99::Char, c99::SChar, c99::UChar, c99::Short, c99::UShort, c99::Int, c99::UInt,
             c99::BitInt, c99::Bool>(t);
}

// Returns the name of parameter `i` of `fn`, or `#<i + 1>` for one the
// declaration leaves unnamed.
std::string param_name(const c99::Type_function_decl& fn, std::size_t i) {
  if (i >= fn.params.size() || fn.params[i].empty()) {
    return "#" + std::to_string(i + 1);
  }
  return fn.params[i];
}

std::string lower(std::string s) {
  for (char& c : s) {
    if (c >= 'A' && c <= 'Z') {
      c = static_cast<char>(c - 'A' + 'a');
    }
  }
  return s;
}

bool starts_with_at(const std::string& m) { return !m.empty() && m.front() == '@'; }

asdl::Int marker_address(const std::string& m) { return std::stoll(m.substr(1)); }

// Returns a list parallel to the function's parameter bytes (the
// `slot_symbols` order, low byte of the first parameter first), each
// entry "A", "X", "Y", or none.
std::vector<std::optional<std::string>> assign_param_registers(
    const c99::Type_function_decl& fn_decl, const std::vector<asdl::Int>& per_param_bytes,
    const std::string& where) {
  const auto* ft = fun_type_of(fn_decl);
  Markers markers = fn_decl.param_registers;
  while (markers.size() < per_param_bytes.size()) {
    markers.emplace_back();
  }
  for (auto& m : markers) {
    if (starts_with_at(m)) {
      m.clear();
    }
  }
  OrderedMap<char, std::string> taken;
  for (std::size_t i = 0; i < markers.size(); ++i) {
    const std::string& marker = markers[i];
    if (marker.empty() || marker == "auto") {
      continue;
    }
    const std::string pname = param_name(fn_decl, i);
    for (const char r : marker) {
      if (const std::string* other = taken.find(r)) {
        fail("function `" + where + "`: parameters `" + *other + "` and `" + pname +
             "` both arrive in " + std::string(1, r));
      }
      taken.insert_or_assign(r, pname);
    }
  }
  std::vector<char> positional;
  for (const char r : kArgRegisters) {
    if (!taken.contains(r)) {
      positional.push_back(r);
    }
  }
  std::vector<std::optional<std::string>> per_byte;
  std::size_t next_reg = 0;
  const std::size_t n_params = std::min(per_param_bytes.size(), markers.size());
  for (std::size_t i = 0; i < n_params; ++i) {
    const asdl::Int n_bytes = per_param_bytes[i];
    const std::string& marker = markers[i];
    if (marker.empty()) {
      per_byte.insert(per_byte.end(), static_cast<std::size_t>(n_bytes), std::nullopt);
      continue;
    }
    const c99::Type_data_type* param_t = ft != nullptr ? &strip_quals(ft->params[i]) : nullptr;
    const std::string pname = param_name(fn_decl, i);
    if (marker == "auto" && param_t != nullptr && isa<c99::Pointer>(*param_t)) {
      fail("function `" + where + "` parameter `" + pname +
           "` is a pointer and can't be passed in a register");
    }
    if (marker == "auto" && n_bytes > 2) {
      fail("function `" + where + "` parameter `" + pname + "` is " + pyrepr::integer(n_bytes) +
           " bytes \xE2\x80\x94 a `register` parameter must fit in at most 2 bytes (A, X)");
    }
    if (marker != "auto") {
      if (static_cast<asdl::Int>(marker.size()) != n_bytes) {
        const std::string attr = "in_" + lower(marker);
        fail("function `" + where + "` parameter `" + pname + "` is " +
             pyrepr::integer(n_bytes) + " byte" + (n_bytes > 1 ? "s" : "") + ", but `" + attr +
             "` names " + std::to_string(marker.size()) + " register" +
             (marker.size() > 1 ? "s" : "") +
             "; name one register for each byte, low byte first");
      }
      for (const char r : marker) {
        per_byte.emplace_back(std::string(1, r));
      }
      continue;
    }
    for (asdl::Int k = 0; k < n_bytes; ++k) {
      if (next_reg >= positional.size()) {
        std::string free;
        for (std::size_t j = 0; j < positional.size(); ++j) {
          if (j != 0) {
            free += ", ";
          }
          free += positional[j];
        }
        if (free.empty()) {
          free = "none";
        }
        fail("function `" + where +
             "`: too many `register` argument bytes \xE2\x80\x94 a `register` parameter "
             "without `in_a`, `in_x`, or `in_y` goes in A, then X, and the free ones here "
             "are: " +
             free);
      }
      per_byte.emplace_back(std::string(1, positional[next_reg]));
      ++next_reg;
    }
  }
  return per_byte;
}

bool any_marker(const Markers& markers) {
  return std::any_of(markers.begin(), markers.end(), [](const std::string& m) { return !m.empty(); });
}

std::vector<std::optional<std::string>> default_param_registers(
    const std::string& name, const c99::Type_function_decl& fn_decl,
    const std::vector<asdl::Int>& per_param_bytes,
    std::vector<std::optional<std::string>> per_byte) {
  const bool any_reg = std::any_of(per_byte.begin(), per_byte.end(),
                                   [](const auto& r) { return r.has_value() && !r->empty(); });
  if (name == "main" || any_reg || any_marker(fn_decl.param_registers)) {
    return per_byte;
  }
  const auto* ft = fun_type_of(fn_decl);
  if (ft == nullptr) {
    return per_byte;
  }
  std::size_t flat = 0;
  std::vector<char> free(std::begin(kArgRegisters), std::end(kArgRegisters));
  const std::size_t n = std::min(ft->params.size(), per_param_bytes.size());
  for (std::size_t i = 0; i < n; ++i) {
    const auto& t = strip_quals(ft->params[i]);
    const bool scalar = isa<c99::Pointer>(t) || is_integer_type(t);
    const auto bytes = static_cast<std::size_t>(per_param_bytes[i]);
    if (!scalar || bytes > free.size()) {
      break;
    }
    for (std::size_t k = 0; k < bytes; ++k) {
      per_byte.at(flat + k) = std::string(1, free.front());
      free.erase(free.begin());
    }
    flat += bytes;
  }
  return per_byte;
}

std::string describe_markers(const c99::Type_function_decl& fn, const Markers& markers) {
  std::string out;
  for (std::size_t i = 0; i < markers.size(); ++i) {
    std::string pname = param_name(fn, i);
    if (const auto dot = pname.find('.'); dot != std::string::npos) {
      pname = pname.substr(dot + 1);
    }
    const std::string& m = markers[i];
    if (i != 0) {
      out += ", ";
    }
    if (m.empty()) {
      out += "`" + pname + "`";
    } else if (starts_with_at(m)) {
      out += "`" + pname + " zeropage($" + hex2(marker_address(m)) + ")`";
    } else if (m == "auto") {
      out += "`register " + pname + "`";
    } else {
      out += "`" + pname + " in_" + lower(m) + "`";
    }
  }
  return out;
}

void check_forward_def_match(const c99::Program& c99_prog) {
  OrderedMap<std::string, const c99::Type_function_decl*> seen;
  for (const auto& d : c99_prog.declaration) {
    const auto* fd = std::get_if<c99::FunctionDecl>(&d);
    if (fd == nullptr) {
      continue;
    }
    const c99::Type_function_decl& fn = *fd->function_decl;
    const c99::Type_function_decl** prior = seen.find(fn.name);
    if (prior == nullptr) {
      seen.insert_or_assign(fn.name, &fn);
      continue;
    }
    Markers a = (*prior)->param_registers;
    Markers b = fn.param_registers;
    if (a.size() < b.size()) {
      a.resize(b.size());
    }
    if (b.size() < a.size()) {
      b.resize(a.size());
    }
    if (a != b) {
      fail("function `" + fn.name +
           "`: the declarations disagree on which parameters arrive in registers or at a "
           "zero-page address, or which ones (" +
           describe_markers(fn, a) + " against " + describe_markers(fn, b) +
           "); every declaration has to give the same calling convention");
    }
    if (fn.body) {
      *prior = &fn;
    }
  }
}

Annotations annotation_map(const c99::Program& c99_prog) {
  Annotations out;
  for (const auto& d : c99_prog.declaration) {
    const auto* fd = std::get_if<c99::FunctionDecl>(&d);
    if (fd == nullptr) {
      continue;
    }
    const c99::Type_function_decl& fn = *fd->function_decl;
    const std::optional<std::string>* existing = out.find(fn.name);
    const bool existing_none = existing == nullptr || !existing->has_value();
    if (existing_none && fn.abi_annotation) {
      out.insert_or_assign(fn.name, fn.abi_annotation);
    } else if (existing == nullptr) {
      out.insert_or_assign(fn.name, std::nullopt);
    }
  }
  return out;
}

Decls function_decls_by_name(const c99::Program& c99_prog) {
  Decls out;
  for (const auto& d : c99_prog.declaration) {
    const auto* fd = std::get_if<c99::FunctionDecl>(&d);
    if (fd == nullptr) {
      continue;
    }
    const c99::Type_function_decl& fn = *fd->function_decl;
    if (!out.contains(fn.name) || fn.body) {
      out.insert_or_assign(fn.name, &fn);
    }
  }
  return out;
}

bool has_indirect_call(const tac::Function& fn) {
  return std::any_of(fn.instructions.begin(), fn.instructions.end(),
                     [](const auto& i) { return isa<tac::IndirectCall>(i); });
}

bool on_cycle(const std::string& name, const CallGraph& callgraph) {
  const auto it = callgraph.find(name);
  if (it == callgraph.end() || it->second.empty()) {
    return false;
  }
  NameSet visited;
  std::vector<std::string> stack(it->second.begin(), it->second.end());
  while (!stack.empty()) {
    const std::string n = stack.back();
    stack.pop_back();
    if (n == name) {
      return true;
    }
    if (!visited.insert(n).second) {
      continue;
    }
    if (const auto next = callgraph.find(n); next != callgraph.end()) {
      stack.insert(stack.end(), next->second.begin(), next->second.end());
    }
  }
  return false;
}

std::vector<std::optional<asdl::Int>> pinned_addrs(const std::string& name,
                                                   const c99::Type_function_decl& fn_decl,
                                                   const std::vector<asdl::Int>& per_param_bytes) {
  auto pinned = pinned_param_addrs(fn_decl, per_param_bytes);
  for (const auto& a : pinned) {
    if (a && *a > 0xFF) {
      fail("function `" + name + "`: a `zeropage(addr)` parameter runs past $FF");
    }
  }
  return pinned;
}

bool returns_aggregate_without_carry(const c99::Type_function_decl& fn_decl) {
  const auto* ft = fun_type_of(fn_decl);
  return ft != nullptr && isa<c99::Structure, c99::Union>(*ft->ret) && !fn_decl.carry_return;
}

ZpLayout make_layout(const std::string& name, const c99::Type_function_decl& fn_decl,
                     const Pool& pool, const TypeTable* types) {
  const asdl::Int byte_count = param_byte_count(fn_decl, types);
  const Range window = zp_param_window(pool);
  const asdl::Int available = window.stop - window.start;
  if (byte_count > available) {
    fail("function `" + name + "` declared `[[c6502::zp_abi]]` has " +
         pyrepr::integer(byte_count) + " parameter bytes, exceeding the " +
         pyrepr::integer(available) + "-byte ZP window ($" + hex2(window.start) + "-$" +
         hex2(window.stop - 1) + ")");
  }
  ZpLayout layout;
  for (asdl::Int k = 0; k < byte_count; ++k) {
    layout.addrs.emplace_back(window.start + k);
  }
  const std::vector<asdl::Int> per_param_bytes = per_param_byte_sizes(fn_decl, types);
  std::vector<std::optional<std::string>> names;
  for (const auto& p : fn_decl.params) {
    names.push_back(p.empty() ? std::nullopt : std::optional<std::string>(p));
  }
  layout.slot_symbols = param_slot_symbols(name, names, per_param_bytes);
  layout.param_registers = default_param_registers(
      name, fn_decl, per_param_bytes, assign_param_registers(fn_decl, per_param_bytes, name));
  layout.pinned = pinned_addrs(name, fn_decl, per_param_bytes);
  return layout;
}

ZpLayout validate_zp_abi(const tac::Function& fn, const c99::Type_function_decl& fn_decl,
                         const NameSet& address_taken, const CallGraph& callgraph,
                         const Pool& pool, const TypeTable* types) {
  if (returns_aggregate_without_carry(fn_decl)) {
    fail("function `" + fn.name +
         "` declared `[[c6502::zp_abi]]` but returns a struct / union; the implicit "
         "sret-pointer parameter doesn't fit the ZP-layout slot scheme");
  }
  if (has_indirect_call(fn)) {
    fail("function `" + fn.name +
         "` declared `[[c6502::zp_abi]]` but its body contains an indirect call; the "
         "callee's ABI can't be known at the call site, so a ZP-passing function can't make "
         "indirect calls");
  }
  if (on_cycle(fn.name, callgraph)) {
    fail("function `" + fn.name +
         "` declared `[[c6502::zp_abi]]` but is reachable from itself through the static call "
         "graph (direct or mutual recursion); a recursive call would clobber the parameter ZP "
         "slots before the outer activation returned");
  }
  if (address_taken.count(fn.name) != 0) {
    fail("function `" + fn.name +
         "` declared `[[c6502::zp_abi]]` but its address is taken somewhere in the program; "
         "ZP-passing functions can't be reached through a function pointer");
  }
  return make_layout(fn.name, fn_decl, pool, types);
}

ZpLayout validate_zp_abi_extern(const std::string& name, const c99::Type_function_decl& fn_decl,
                                const NameSet& address_taken, const Pool& pool,
                                const TypeTable* types) {
  if (returns_aggregate_without_carry(fn_decl)) {
    fail("function `" + name +
         "` declared `[[c6502::zp_abi]]` but returns a struct / union; the implicit "
         "sret-pointer parameter doesn't fit the ZP-layout slot scheme");
  }
  if (address_taken.count(name) != 0) {
    fail("function `" + name +
         "` declared `[[c6502::zp_abi]]` but its address is taken somewhere in the program; "
         "ZP-passing functions can't be reached through a function pointer");
  }
  return make_layout(name, fn_decl, pool, types);
}

bool has_register_local_in_block(const c99::Type_block& block);
bool has_register_local_in_statement(const c99::Type_statement& s);

bool is_register_var(const c99::Type_var_decl& v) {
  return v.storage_class && isa<c99::Register>(*v.storage_class);
}

bool has_register_local_in_function_decl(const c99::Type_function_decl& fn) {
  return fn.body && has_register_local_in_block(**fn.body);
}

bool has_register_local_in_declaration(const c99::Type_declaration& d) {
  if (const auto* v = std::get_if<c99::VarDecl>(&d)) {
    return is_register_var(v->var_decl);
  }
  if (const auto* f = std::get_if<c99::FunctionDecl>(&d)) {
    return has_register_local_in_function_decl(*f->function_decl);
  }
  return false;
}

bool has_register_local_in_block(const c99::Type_block& block) {
  for (const auto& item : std::get<c99::Block>(block).block_item) {
    if (const auto* s = std::get_if<c99::S>(&item)) {
      if (has_register_local_in_statement(*s->statement)) {
        return true;
      }
    } else if (has_register_local_in_declaration(*std::get<c99::D>(item).declaration)) {
      return true;
    }
  }
  return false;
}

bool has_register_local_in_statement(const c99::Type_statement& s) {
  switch (c99::kind(s)) {
    case c99::Kind_statement::IfStmt: {
      const auto& x = std::get<c99::IfStmt>(s);
      return has_register_local_in_statement(*x.then_clause) ||
             (x.else_clause && has_register_local_in_statement(**x.else_clause));
    }
    case c99::Kind_statement::Compound:
      return has_register_local_in_block(*std::get<c99::Compound>(s).block);
    case c99::Kind_statement::WhileStmt:
      return has_register_local_in_statement(*std::get<c99::WhileStmt>(s).body);
    case c99::Kind_statement::DoWhileStmt:
      return has_register_local_in_statement(*std::get<c99::DoWhileStmt>(s).body);
    case c99::Kind_statement::ForStmt: {
      const auto& x = std::get<c99::ForStmt>(s);
      if (const auto* init = std::get_if<c99::InitDecl>(&x.init)) {
        for (const auto& v : init->var_decls) {
          if (is_register_var(v)) {
            return true;
          }
        }
      }
      return has_register_local_in_statement(*x.body);
    }
    case c99::Kind_statement::SwitchStmt:
      return has_register_local_in_statement(*std::get<c99::SwitchStmt>(s).body);
    case c99::Kind_statement::CaseStmt:
      return has_register_local_in_statement(*std::get<c99::CaseStmt>(s).body);
    case c99::Kind_statement::DefaultStmt:
      return has_register_local_in_statement(*std::get<c99::DefaultStmt>(s).body);
    case c99::Kind_statement::LabeledStmt:
      return has_register_local_in_statement(*std::get<c99::LabeledStmt>(s).statement);
    default:
      return false;
  }
}

bool has_register_request(const c99::Type_function_decl& fn_decl) {
  return any_marker(fn_decl.param_registers) || has_register_local_in_function_decl(fn_decl);
}

std::string request_words(const c99::Type_function_decl& fn_decl) {
  for (const auto& m : fn_decl.param_registers) {
    if (starts_with_at(m)) {
      return "a `zeropage(addr)` parameter";
    }
  }
  return "a `register` parameter or local";
}

NameSet addressed_functions(const c99::Program& c99_prog) {
  NameSet out;
  for (const auto& d : c99_prog.declaration) {
    if (const auto* fd = std::get_if<c99::FunctionDecl>(&d)) {
      if (fd->function_decl->address) {
        out.insert(fd->function_decl->name);
      }
    }
  }
  return out;
}

NameSet names_used_as_values(const tac::Program& prog, const NameSet& names) {
  NameSet used;
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto& instr : fn->instructions) {
      for (const auto* v : vals_in(instr)) {
        if (const auto* var = std::get_if<tac::Var>(v); var != nullptr && names.count(var->name)) {
          used.insert(var->name);
        }
      }
    }
  }
  return used;
}

bool fits_return_registers(const c99::Type_data_type& ret) {
  const auto& t = strip_quals(ret);
  if (isa<c99::Structure>(t)) {
    return true;
  }
  return isa<c99::Void, c99::Char, c99::SChar, c99::UChar, c99::Int, c99::UInt, c99::Pointer>(t);
}

OrderedMap<std::string, NameSet> direct_callees(const tac::Program& prog) {
  OrderedMap<std::string, NameSet> out;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac::Function>(&tl)) {
      NameSet callees;
      for (const auto* call : graph_calls(*fn)) {
        callees.insert(call->name);
      }
      out.insert_or_assign(fn->name, std::move(callees));
    }
  }
  return out;
}

void demote_soft_stack_callers(const tac::Program& prog, Abi& out, const Annotations& annotations,
                               const Decls& decls) {
  const NameSet pooled = pool_eligible(prog, out);
  const auto callees = direct_callees(prog);
  const auto is_zp = [&](const std::string& n) {
    const ParamLayout* l = out.find(n);
    return l != nullptr && std::holds_alternative<ZpLayout>(*l);
  };
  bool changed = true;
  while (changed) {
    changed = false;
    for (const auto& [name, called] : callees) {
      if (!is_zp(name)) {
        continue;
      }
      std::vector<std::string> soft;
      for (const auto& c : called) {
        if (!is_zp(c) && pooled.count(c) == 0) {
          soft.push_back(c);
        }
      }
      if (soft.empty()) {
        continue;
      }
      const auto* const* fn_decl = decls.find(name);
      const std::optional<std::string>* ann = annotations.find(name);
      const bool annotated = ann != nullptr && *ann == "zp_abi";
      const bool strict = annotated || (fn_decl != nullptr && has_register_request(**fn_decl));
      if (strict) {
        const std::string what = annotated ? "declared `[[c6502::zp_abi]]`"
                                           : "declares " + request_words(**fn_decl);
        fail("function `" + name + "` " + what + " but calls `" + soft.front() +
             "`, which uses the soft-stack ABI without a private pool; `" + soft.front() +
             "` may overwrite the $80-$BF ZP window that holds `" + name + "`'s parameters");
      }
      out.insert_or_assign(name, SoftStackLayout{});
      changed = true;
    }
  }
}

}  // namespace

namespace abi_selection {

NameSet address_taken(const tac::Program& prog, const NameSet& declared) {
  NameSet function_names = declared;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac::Function>(&tl)) {
      function_names.insert(fn->name);
    }
  }
  if (function_names.empty()) {
    return {};
  }
  NameSet taken;
  for (const auto& tl : prog.top_level) {
    if (const auto* sv = std::get_if<tac::StaticVariable>(&tl)) {
      for (const auto& item : sv->init) {
        const std::string* name = nullptr;
        if (const auto* a = std::get_if<tac::AddressInit>(&item)) {
          name = &a->name;
        } else if (const auto* b = std::get_if<tac::AddressByteInit>(&item)) {
          name = &b->name;
        }
        if (name != nullptr && function_names.count(*name) != 0) {
          taken.insert(*name);
        }
      }
      continue;
    }
    for (const auto& instr : std::get<tac::Function>(tl).instructions) {
      for (const auto* v : vals_in(instr)) {
        if (const auto* var = std::get_if<tac::Var>(v);
            var != nullptr && function_names.count(var->name) != 0) {
          taken.insert(var->name);
        }
      }
    }
  }
  return taken;
}

OrderedMap<std::string, const c99::Type_function_decl*> function_decls_by_name(
    const c99::Program& c99_prog) {
  return passes::function_decls_by_name(c99_prog);
}

bool has_register_local(const c99::Type_function_decl& fn_decl) {
  return has_register_local_in_function_decl(fn_decl);
}

bool has_register_request(const c99::Type_function_decl& fn_decl) {
  return passes::has_register_request(fn_decl);
}

}  // namespace abi_selection

std::vector<std::optional<asdl::Int>> pinned_param_addrs(
    const c99::Type_function_decl& fn_decl, const std::vector<asdl::Int>& per_param_bytes) {
  const Markers& markers = fn_decl.param_registers;
  if (std::none_of(markers.begin(), markers.end(), starts_with_at)) {
    return {};
  }
  std::vector<std::optional<asdl::Int>> out;
  for (std::size_t i = 0; i < per_param_bytes.size(); ++i) {
    const std::string m = i < markers.size() ? markers[i] : "";
    if (starts_with_at(m)) {
      const asdl::Int base = marker_address(m);
      for (asdl::Int k = 0; k < per_param_bytes[i]; ++k) {
        out.emplace_back(base + k);
      }
    } else {
      out.insert(out.end(), static_cast<std::size_t>(per_param_bytes[i]), std::nullopt);
    }
  }
  return out;
}

OrderedMap<std::string, asdl::Int> pinned_slot_symbols(const Abi& abi) {
  OrderedMap<std::string, asdl::Int> out;
  for (const auto& [name, layout] : abi) {
    const auto* zp = std::get_if<ZpLayout>(&layout);
    if (zp == nullptr) {
      continue;
    }
    const std::size_t n = std::min(zp->slot_symbols.size(), zp->pinned.size());
    for (std::size_t i = 0; i < n; ++i) {
      if (zp->pinned[i]) {
        out.insert_or_assign(zp->slot_symbols[i], *zp->pinned[i]);
      }
    }
  }
  return out;
}

OrderedMap<std::string, ZpLayout> fixed_address_layouts(const tac::Program& prog,
                                                        const c99::Program& c99_prog,
                                                        const TypeTable* types, const Pool& pool) {
  const NameSet addressed = addressed_functions(c99_prog);
  if (addressed.empty()) {
    return {};
  }
  const Annotations annotations = annotation_map(c99_prog);
  const Decls decls = function_decls_by_name(c99_prog);
  const NameSet taken = names_used_as_values(prog, addressed);
  OrderedMap<std::string, ZpLayout> out;
  for (const auto& name : addressed) {
    const c99::Type_function_decl& fn_decl = *decls.at(name);
    const std::optional<std::string>* ann = annotations.find(name);
    if (ann == nullptr || *ann != "zp_abi") {
      if (any_marker(fn_decl.param_registers)) {
        fail("function `" + name +
             "` has a fixed address and a `register` or `zeropage(addr)` parameter but no "
             "`[[c6502::zp_abi]]`; without zp_abi its arguments go on the soft stack, so "
             "declare it `[[c6502::zp_abi, c6502::address(...)]]`");
      }
      continue;
    }
    if (taken.count(name) != 0) {
      fail("the address of `" + name +
           "` is taken, but it's a zp_abi function with a fixed address; a call through a "
           "pointer passes its arguments on the soft stack, which the code at that address "
           "doesn't read");
    }
    const auto* ft = fun_type_of(fn_decl);
    if (ft == nullptr) {
      throw InternalError("the declaration of `" + name + "` has no FunType");
    }
    if (!fits_return_registers(*ft->ret)) {
      fail("zp_abi function `" + name +
           "` has a fixed address but returns a value that doesn't fit in A and X; anything "
           "wider than 2 bytes comes back in HARGS");
    }
    ZpLayout layout = validate_zp_abi_extern(name, fn_decl, NameSet{}, pool, types);
    std::vector<std::optional<asdl::Int>> pinned = layout.pinned;
    if (pinned.empty()) {
      pinned.resize(layout.param_registers.size());
    }
    const std::size_t n = std::min(pinned.size(), layout.param_registers.size());
    for (std::size_t i = 0; i < n; ++i) {
      if (!layout.param_registers[i] && !pinned[i]) {
        fail("zp_abi function `" + name +
             "` has a fixed address, so every parameter has to be passed in a register or at a "
             "zero-page address its code reads; declare each parameter `register` (A, then X), "
             "or name its register with `in_a`, `in_x`, or `in_y`, at most 3 bytes in all, or "
             "give it `[[c6502::zeropage(addr)]]`");
      }
    }
    out.insert_or_assign(name, std::move(layout));
  }
  return out;
}

Abi select_abi(const tac::Program& prog, const c99::Program& c99_prog, const TypeTable* types,
               const Pool& pool) {
  check_forward_def_match(c99_prog);
  const Annotations annotations = annotation_map(c99_prog);
  const Decls decls = function_decls_by_name(c99_prog);
  NameSet declared;
  for (const auto& [name, d] : decls) {
    declared.insert(name);
  }
  const NameSet address_taken = abi_selection::address_taken(prog, declared);
  const CallGraph callgraph = call_graph(prog);
  Abi out;
  NameSet defined;
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    defined.insert(fn->name);
    const std::optional<std::string>* ann_p = annotations.find(fn->name);
    const std::optional<std::string> ann = ann_p != nullptr ? *ann_p : std::nullopt;
    const auto* const* decl_p = decls.find(fn->name);
    const c99::Type_function_decl* fn_decl = decl_p != nullptr ? *decl_p : nullptr;
    if (ann == "zp_abi") {
      if (fn_decl == nullptr) {
        fail("function `" + fn->name +
             "` declared `[[c6502::zp_abi]]` but its c99 declaration is missing \xE2\x80\x94 "
             "internal error");
      }
      out.insert_or_assign(fn->name,
                           validate_zp_abi(*fn, *fn_decl, address_taken, callgraph, pool, types));
    } else if (!ann) {
      if (fn_decl == nullptr) {
        out.insert_or_assign(fn->name, SoftStackLayout{});
        continue;
      }
      try {
        out.insert_or_assign(fn->name,
                             validate_zp_abi(*fn, *fn_decl, address_taken, callgraph, pool, types));
      } catch (const CompileError& exc) {
        if (!is_abi_error(exc)) {
          throw;
        }
        if (has_register_request(*fn_decl)) {
          const std::string msg = exc.what();
          if (msg.find("register") != std::string::npos) {
            throw;
          }
          fail("function `" + fn->name + "` declares " + request_words(*fn_decl) +
               " but isn't eligible for the ZP-passing ABI: " + msg);
        }
        out.insert_or_assign(fn->name, SoftStackLayout{});
      }
    } else {
      fail("function `" + fn->name + "` has unrecognized abi annotation " + pyrepr::string(*ann));
    }
  }
  const auto fixed = fixed_address_layouts(prog, c99_prog, types, pool);
  const NameSet addressed = addressed_functions(c99_prog);
  for (const auto& [name, layout] : fixed) {
    out.insert_or_assign(name, layout);
  }
  for (const auto& [name, ann] : annotations) {
    if (defined.count(name) != 0 || addressed.count(name) != 0) {
      continue;
    }
    const auto* const* fn_decl = decls.find(name);
    if (fn_decl == nullptr) {
      continue;
    }
    if (ann == "zp_abi") {
      out.insert_or_assign(name, validate_zp_abi_extern(name, **fn_decl, address_taken, pool, types));
    }
  }
  for (const auto& [name, fn_decl] : decls) {
    if (defined.count(name) != 0 || out.contains(name) || addressed.count(name) != 0) {
      continue;
    }
    const std::optional<std::string>* ann = annotations.find(name);
    if (ann != nullptr && ann->has_value()) {
      continue;
    }
    try {
      out.insert_or_assign(name, validate_zp_abi_extern(name, *fn_decl, address_taken, pool, types));
    } catch (const CompileError& exc) {
      if (!is_abi_error(exc)) {
        throw;
      }
      if (has_register_request(*fn_decl)) {
        fail("extern function `" + name + "` declares " + request_words(*fn_decl) +
             " but isn't eligible for the ZP-passing ABI, which that requires");
      }
    }
  }
  demote_soft_stack_callers(prog, out, annotations, decls);
  return out;
}

}  // namespace c6502::passes
