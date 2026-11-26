#include "passes/type_checker.h"

#include <algorithm>
#include <map>
#include <set>
#include <utility>

#include "builtin_functions.h"
#include "passes/stdbit_functions.h"
#include "pyrepr.h"

namespace c6502::passes::type_checking {

namespace {

namespace ir = ::c6502::c99_ast;
namespace ce = ::c6502::passes::constant_expression;
using K = ir::Kind_data_type;

[[noreturn]] void fail(const std::string& message) { throw TypeCheckError(message); }

std::string kw_of(bool is_union) { return is_union ? "union" : "struct"; }

const std::string& tag_of(const Type& t) {
  if (const auto* s = std::get_if<ir::Structure>(&t)) {
    return s->tag;
  }
  return std::get<ir::Union>(t).tag;
}

std::string attr_class_name(const IdAttr& attrs) {
  switch (attrs.index()) {
    case 0:
      return "LocalAttr";
    case 1:
      return "StaticAttr";
    default:
      return "FunAttr";
  }
}

std::string hex_prefixed(Int v) {
  if (v == 0) {
    return "0x0";
  }
  std::string digits;
  while (v > 0) {
    const int d = static_cast<int>(v % Int(16));
    digits += static_cast<char>(d < 10 ? '0' + d : 'a' + d - 10);
    v /= Int(16);
  }
  std::reverse(digits.begin(), digits.end());
  return "0x" + digits;
}

void collect_cases(ir::Type_statement& s, std::map<std::string, ir::CaseStmt*>& out) {
  using S = ir::Kind_statement;
  switch (ir::kind(s)) {
    case S::IfStmt: {
      auto& i = std::get<ir::IfStmt>(s);
      collect_cases(*i.then_clause, out);
      if (i.else_clause) {
        collect_cases(**i.else_clause, out);
      }
      return;
    }
    case S::Compound:
      for (auto& item : std::get<ir::Block>(*std::get<ir::Compound>(s).block).block_item) {
        if (auto* st = std::get_if<ir::S>(&item)) {
          collect_cases(*st->statement, out);
        }
      }
      return;
    case S::WhileStmt:
      collect_cases(*std::get<ir::WhileStmt>(s).body, out);
      return;
    case S::DoWhileStmt:
      collect_cases(*std::get<ir::DoWhileStmt>(s).body, out);
      return;
    case S::ForStmt:
      collect_cases(*std::get<ir::ForStmt>(s).body, out);
      return;
    case S::SwitchStmt:
      collect_cases(*std::get<ir::SwitchStmt>(s).body, out);
      return;
    case S::CaseStmt: {
      auto& c = std::get<ir::CaseStmt>(s);
      out.emplace(c.label, &c);
      collect_cases(*c.body, out);
      return;
    }
    case S::DefaultStmt:
      collect_cases(*std::get<ir::DefaultStmt>(s).body, out);
      return;
    case S::LabeledStmt:
      collect_cases(*std::get<ir::LabeledStmt>(s).statement, out);
      return;
    default:
      return;
  }
}

std::optional<Int> evaluate_or_none(const Exp& e) {
  try {
    return ce::evaluate_integer_constant_expression(e).first;
  } catch (const ce::ConstantExpressionError&) {
    return std::nullopt;
  }
}

}  // namespace

void TypeChecker::check_program(ir::Program& prog) {
  const std::vector<std::string> library = stdbit_functions::add_definitions(prog.declaration);
  {
    const ce::NamedConstants resolver([this](const Exp& e) { return named_constant(e); });
    for (auto& d : prog.declaration) {
      check_file_scope_declaration(d);
    }
  }
  for (const std::string& name : library) {
    Symbol* sym = symbols.find(name);
    auto* f = sym != nullptr ? std::get_if<FunAttr>(&sym->attrs) : nullptr;
    if (f == nullptr) {
      throw InternalError("check_program: the <stdbit.h> definition of " + name +
                          " has no function in the symbol table");
    }
    f->is_global = false;
  }
}

void TypeChecker::check_file_scope_declaration(ir::Type_declaration& decl) {
  switch (ir::kind(decl)) {
    case ir::Kind_declaration::VarDecl:
      check_file_scope_var(std::get<ir::VarDecl>(decl).var_decl);
      return;
    case ir::Kind_declaration::FunctionDecl:
      check_function_decl(*std::get<ir::FunctionDecl>(decl).function_decl, true);
      return;
    case ir::Kind_declaration::StructDecl:
      check_struct_decl(std::get<ir::StructDecl>(decl).struct_decl);
      return;
  }
}

void TypeChecker::check_struct_decl(ir::Type_struct_decl& sd) {
  const std::string tag = sd.tag;
  const bool is_union = sd.is_union;
  const StructLayout* prior_layout = types.find(tag);
  const bool has_prior = prior_layout != nullptr;
  const bool prior_is_union = has_prior && prior_layout->is_union;
  const bool prior_complete = has_prior && prior_layout->complete;
  record_tag_visible(tag);
  if (sd.members.empty()) {
    if (!has_prior) {
      types.insert_or_assign(tag, StructLayout{.tag = tag,
                                               .is_union = is_union,
                                               .members = {},
                                               .size = 0,
                                               .complete = false});
    } else if (prior_is_union != is_union) {
      fail("redeclaration of '" + kw_of(is_union) + " " + tag + "' as a " +
           kw_of(prior_is_union));
    }
    return;
  }
  if (has_prior) {
    if (prior_is_union != is_union) {
      fail("redeclaration of '" + kw_of(prior_is_union) + " " + tag + "' as a " +
           kw_of(is_union));
    }
    if (prior_complete) {
      fail("redefinition of '" + kw_of(is_union) + " " + tag + "'");
    }
  }
  if (!has_prior) {
    types.insert_or_assign(tag, StructLayout{.tag = tag,
                                             .is_union = is_union,
                                             .members = {},
                                             .size = 0,
                                             .complete = false});
  }
  StructLayout layout = compute_layout(sd);
  types.insert_or_assign(tag, std::move(layout));
}

StructLayout TypeChecker::compute_layout(const ir::Type_struct_decl& sd) {
  std::vector<MemberInfo> members;
  std::set<std::string, std::less<>> seen;
  Int offset = 0;
  for (const auto& m : sd.members) {
    if (seen.count(m.name) != 0) {
      fail("duplicate member " + pyrepr::string(m.name) + " in struct/union " +
           pyrepr::string(sd.tag));
    }
    seen.insert(m.name);
    const Type& mtype = m.data_type;
    const std::string where =
        "member " + pyrepr::string(m.name) + " of struct/union " + pyrepr::string(sd.tag);
    if (asdl::isa<ir::Void>(mtype)) {
      fail(where + " cannot have void type");
    }
    if (asdl::isa<ir::FunType>(mtype)) {
      fail(where + " cannot have function type");
    }
    if (contains_self_struct(mtype, sd.tag)) {
      fail(where + " recursively contains the same struct/union type (use a pointer instead)");
    }
    check_well_formed(mtype, where, true);
    const Int msize = sizeof_type(mtype, &types);
    members.push_back(MemberInfo{.name = m.name, .type = mtype, .byte_offset = offset});
    if (!sd.is_union) {
      offset += msize;
    }
  }
  Int total = 0;
  if (sd.is_union) {
    for (const auto& m : sd.members) {
      total = std::max(total, sizeof_type(m.data_type, &types));
    }
    for (auto& m : members) {
      m.byte_offset = 0;
    }
  } else {
    total = offset;
  }
  return StructLayout{.tag = sd.tag,
                      .is_union = sd.is_union,
                      .members = std::move(members),
                      .size = total,
                      .complete = true};
}

bool TypeChecker::contains_self_struct(const Type& t, const std::string& tag) const {
  if (asdl::isa<ir::Structure, ir::Union>(t) && tag_of(t) == tag) {
    return true;
  }
  if (const auto* a = std::get_if<ir::Array>(&t)) {
    return contains_self_struct(*a->element_type, tag);
  }
  return false;
}

MemberInfo TypeChecker::lookup_member(const Type& type, const std::string& member,
                                      std::string_view where) const {
  const Type& t = strip_quals(type);
  const std::string w(where);
  if (!asdl::isa<ir::Structure, ir::Union>(t)) {
    fail(w + ": operand has non-struct/union type " + asdl::py_repr(t));
  }
  const std::string kw = kw_of(asdl::isa<ir::Union>(t));
  if (!tag_visible(tag_of(t))) {
    fail(w + ": '" + kw + " " + tag_of(t) + "' is not in scope");
  }
  const StructLayout* layout = types.find(tag_of(t));
  if (layout == nullptr || !layout->complete) {
    fail(w + ": incomplete type '" + kw + " " + tag_of(t) + "'");
  }
  for (const auto& m : layout->members) {
    if (m.name == member) {
      return m;
    }
  }
  fail(w + ": '" + kw + " " + tag_of(t) + "' has no member named " + pyrepr::string(member));
}

void TypeChecker::check_file_scope_var_well_formed(ir::Type_var_decl& vd) {
  const bool require_complete = asdl::isa<ir::Structure, ir::Union, ir::Array>(vd.data_type);
  check_well_formed(vd.data_type, "file-scope object " + pyrepr::string(vd.name),
                    require_complete);
}

void TypeChecker::check_file_scope_var(ir::Type_var_decl& vd) {
  if (constexpr_values.count(vd.name) != 0) {
    fail(pyrepr::string(vd.name) +
         " is declared again after its `constexpr` declaration, which c6502 takes only as the "
         "one declaration of a name (C23 §6.7.1p12 leaves any other to the implementation)");
  }
  if (vd.is_constexpr) {
    if (symbols.find(vd.name) != nullptr) {
      fail(pyrepr::string(vd.name) +
           " is declared `constexpr` after another declaration of it, and c6502 takes a "
           "`constexpr` declaration only as the one declaration of a name (C23 §6.7.1p12 leaves "
           "any other to the implementation)");
    }
    // A file-scope constexpr object has internal linkage (C23 §6.2.2p3).
    vd.storage_class = ir::Static{};
  }
  if (vd.init) {
    vd.init = normalize_init(std::move(*vd.init), vd.data_type, vd.name);
  }
  if (vd.is_constexpr) {
    begin_constexpr(vd);
  }
  check_file_scope_var_body(vd);
  if (vd.is_constexpr) {
    end_constexpr(vd);
  }
}

void TypeChecker::check_file_scope_var_body(ir::Type_var_decl& vd) {
  if (vd.storage_class && asdl::isa<ir::Register>(*vd.storage_class)) {
    fail("file-scope object " + pyrepr::string(vd.name) +
         " can't be declared `register` (C99 §6.7.1 — only block-scope objects and function "
         "parameters)");
  }
  vd.data_type = size_from_initializer(vd.data_type, vd.init ? &*vd.init : nullptr,
                                       "array " + pyrepr::string(display_name(vd.name)));
  if (!is_complete_object_type(vd.data_type)) {
    fail("file-scope object " + pyrepr::string(vd.name) + " declared with non-object type " +
         asdl::py_repr(vd.data_type));
  }
  const bool is_extern = vd.storage_class && asdl::isa<ir::Extern>(*vd.storage_class);
  const bool require_complete =
      asdl::isa<ir::Structure, ir::Union, ir::Array>(strip_quals(vd.data_type)) && !is_extern;
  check_well_formed(vd.data_type, "file-scope object " + pyrepr::string(vd.name),
                    require_complete);
  InitialValue initial;
  if (!vd.init) {
    if (is_extern) {
      initial = NoInitializer{};
    } else {
      initial = Tentative{};
    }
  } else if (asdl::isa<ir::Array>(vd.data_type)) {
    const auto& arr = std::get<ir::Array>(vd.data_type);
    if (asdl::isa<ir::String>(*vd.init) && is_char_element(*arr.element_type)) {
      auto& s = std::get<ir::String>(*vd.init);
      check_string_array_init(s, arr, vd.name);
      initial = Initial{string_to_value_tuple(s.str, std::get<ir::Array>(vd.data_type).size)};
    } else {
      if (!asdl::isa<ir::InitList>(*vd.init)) {
        fail("file-scope array " + pyrepr::string(vd.name) +
             " requires a brace-enclosed initializer (`{...}`)");
      }
      check_array_init_list(std::get<ir::InitList>(*vd.init), std::get<ir::Array>(vd.data_type),
                            vd.name);
      initial = Initial{const_init_aggregate(*vd.init, vd.data_type, vd.name, &symbols, &types)};
    }
  } else if (asdl::isa<ir::Structure, ir::Union>(strip_quals(vd.data_type))) {
    if (!asdl::isa<ir::InitList>(*vd.init)) {
      std::optional<ValueTree> value = structure_constant_init(vd);
      if (!value) {
        fail("file-scope struct/union " + pyrepr::string(vd.name) +
             " requires a brace-enclosed initializer (`{...}`)");
      }
      initial = Initial{std::move(*value)};
    } else {
      check_struct_init_list(std::get<ir::InitList>(*vd.init), vd.data_type, vd.name);
      initial = Initial{const_init_aggregate(*vd.init, vd.data_type, vd.name, &symbols, &types)};
    }
  } else {
    vd.init = unbrace_scalar_init(std::move(*vd.init), vd.data_type,
                                  "scalar variable " + pyrepr::string(display_name(vd.name)));
    check_exp(*vd.init);
    vd.init = decay_if_array(std::move(*vd.init));
    vd.init = convert_to(std::move(*vd.init), vd.data_type);
    initial = Initial{const_init_value(*vd.init, vd.data_type, vd.name, &symbols, &types)};
  }
  bool is_global = true;
  if (vd.storage_class && asdl::isa<ir::Static>(*vd.storage_class)) {
    is_global = false;
  } else if (is_extern) {
    const Symbol* prior = symbols.find(vd.name);
    if (prior != nullptr) {
      if (const auto* attr = std::get_if<StaticAttr>(&prior->attrs)) {
        is_global = attr->is_global;
      }
    }
  }
  add_or_merge_static_object(vd.name, vd.data_type, initial, is_global, vd.placement, vd.section);
}

void TypeChecker::check_function_decl(ir::Type_function_decl& fd, bool /*file_scope*/) {
  if (!asdl::isa<ir::FunType>(fd.data_type)) {
    throw InternalError("function decl " + pyrepr::string(fd.name) +
                        " has non-FunType data_type " + asdl::py_repr(fd.data_type));
  }
  const ir::FunType ftype = std::get<ir::FunType>(fd.data_type);
  const bool defined = fd.body.has_value();
  bool is_global = true;
  if (fd.storage_class && asdl::isa<ir::Static>(*fd.storage_class)) {
    is_global = false;
  } else if (const Symbol* prior = symbols.find(fd.name)) {
    if (const auto* attr = std::get_if<FunAttr>(&prior->attrs)) {
      is_global = attr->is_global;
    }
  }
  if (fd.carry_return) {
    check_carry_return_type(fd.name, *ftype.ret, &types);
  }
  add_or_merge_function(fd.name, ftype, defined, is_global, fd.carry_return, fd.address,
                        NameSet(fd.preserves.begin(), fd.preserves.end()), fd.section,
                        param_zeropage(fd.param_registers), fd.noreturn);
  const std::string where_param = "parameter of function " + pyrepr::string(fd.name);
  for (const Type& p_type : ftype.params) {
    if (is_void(p_type)) {
      fail(where_param + " cannot have void type (C99 §6.7.5.3.4)");
    }
    const bool require_complete = asdl::isa<ir::Array>(p_type) ||
                                  (defined && asdl::isa<ir::Structure, ir::Union>(p_type));
    check_well_formed(p_type, where_param, require_complete);
  }
  const Type& ret_t = *ftype.ret;
  const bool require_ret_complete = defined && asdl::isa<ir::Structure, ir::Union>(ret_t);
  check_well_formed(ret_t, "return type of function " + pyrepr::string(fd.name),
                    require_ret_complete);
  if (!defined) {
    return;
  }
  std::vector<std::string> param_regs = fd.param_registers;
  while (param_regs.size() < fd.params.size()) {
    param_regs.push_back("");
  }
  const std::size_t n = std::min({fd.params.size(), ftype.params.size(), param_regs.size()});
  for (std::size_t i = 0; i < n; ++i) {
    const std::string& p_name = fd.params[i];
    const Type& p_type = ftype.params[i];
    std::string p_reg = param_regs[i];
    if (!p_reg.empty() && p_reg[0] == '@') {
      p_reg.clear();
    }
    if (p_reg == "auto") {
      check_register_object(p_type, 2,
                            "register parameter " + pyrepr::string(display_name(p_name)));
    } else if (!p_reg.empty()) {
      check_named_register_param(p_type, p_reg,
                                 "parameter " + pyrepr::string(display_name(p_name)));
    }
    symbols.insert_or_assign(p_name,
                             Symbol{.type = p_type, .attrs = LocalAttr{.register_ = !p_reg.empty()}});
  }
  std::optional<Type> saved = return_type;
  const bool saved_carry = carry_return;
  return_type = *ftype.ret;
  carry_return = fd.carry_return;
  push_tag_scope();
  check_block(**fd.body);
  pop_tag_scope();
  return_type = std::move(saved);
  carry_return = saved_carry;
}

void TypeChecker::add_or_merge_static_object(const std::string& name, const Type& type,
                                             const InitialValue& initial, bool is_global,
                                             const std::optional<ir::Type_placement>& placement,
                                             const std::optional<std::string>& section_in) {
  const Symbol* existing = symbols.find(name);
  bool zeropage = placement && asdl::isa<ir::ZeroPage>(*placement);
  std::optional<Int> zp_address;
  if (zeropage) {
    zp_address = std::get<ir::ZeroPage>(*placement).address;
  }
  std::optional<Int> address;
  if (placement && asdl::isa<ir::Absolute>(*placement)) {
    address = std::get<ir::Absolute>(*placement).address;
    if (*address < Int(0x100)) {
      zeropage = true;
      zp_address = address;
    }
  }
  if (existing == nullptr) {
    check_unowned_object(name, initial, address);
    check_section(name, section_in, address, placement);
    symbols.insert_or_assign(name, Symbol{.type = type,
                                          .attrs = StaticAttr{.initial_value = initial,
                                                              .is_global = is_global,
                                                              .zeropage = zeropage,
                                                              .zp_address = zp_address,
                                                              .address = address,
                                                              .section = section_in}});
    return;
  }
  const auto* prior = std::get_if<StaticAttr>(&existing->attrs);
  if (prior == nullptr) {
    fail(pyrepr::string(name) + " previously declared as a " + attr_class_name(existing->attrs) +
         ", now redeclared as a static-storage object");
  }
  Type merged_type = type;
  if (!types_equal(existing->type, type)) {
    std::optional<Type> composite = composite_array(existing->type, type);
    if (!composite) {
      fail("incompatible redeclaration of " + pyrepr::string(name) + ": previous " +
           asdl::py_repr(existing->type) + ", new " + asdl::py_repr(type));
    }
    merged_type = std::move(*composite);
  }
  if (prior->is_global != is_global) {
    fail("linkage of " + pyrepr::string(name) + " disagrees with prior declaration");
  }
  const InitialValue merged = merge_initial_value(name, prior->initial_value, initial);
  const std::optional<Int> prior_addr = prior->zp_address;
  if (zp_address && prior_addr && *zp_address != *prior_addr) {
    fail(pyrepr::string(name) + " is placed at zero-page $" + pyrepr::hex(*prior_addr, 2) +
         " and at $" + pyrepr::hex(*zp_address, 2));
  }
  const std::optional<Int> prior_fixed = prior->address;
  if (address && prior_fixed && *address != *prior_fixed) {
    fail("two declarations of " + pyrepr::string(name) + " give it different addresses, $" +
         pyrepr::hex(*prior_fixed, 4) + " and $" + pyrepr::hex(*address, 4));
  }
  if (!address) {
    address = prior_fixed;
  }
  check_unowned_object(name, merged, address);
  const std::optional<std::string> section = merge_section(name, prior->section, section_in);
  zeropage = prior->zeropage || zeropage;
  check_section(name, section, address,
                zeropage ? std::optional<ir::Type_placement>(ir::ZeroPage{}) : std::nullopt);
  symbols.insert_or_assign(
      name, Symbol{.type = std::move(merged_type),
                   .attrs = StaticAttr{.initial_value = merged,
                                       .is_global = is_global,
                                       .zeropage = zeropage,
                                       .zp_address = zp_address ? zp_address : prior_addr,
                                       .address = address,
                                       .section = section}});
}

void TypeChecker::add_or_merge_function(const std::string& name, const ir::FunType& ftype,
                                        bool defined, bool is_global, bool carry_return_in,
                                        const std::optional<Int>& address_in,
                                        const NameSet& preserves_in,
                                        const std::optional<std::string>& section_in,
                                        const std::vector<std::optional<Int>>& param_zeropage_in,
                                        bool noreturn) {
  const Type fun_type = ftype;
  const Symbol* existing = symbols.find(name);
  if (existing == nullptr) {
    check_preserves(name, fun_type, address_in, preserves_in, &types);
    check_section(name, section_in, address_in, std::nullopt);
    symbols.insert_or_assign(name, Symbol{.type = fun_type,
                                          .attrs = FunAttr{.defined = defined,
                                                           .is_global = is_global,
                                                           .carry_return = carry_return_in,
                                                           .address = address_in,
                                                           .preserves = preserves_in,
                                                           .section = section_in,
                                                           .param_zeropage = param_zeropage_in,
                                                           .noreturn = noreturn}});
    return;
  }
  const auto* prior = std::get_if<FunAttr>(&existing->attrs);
  if (prior == nullptr) {
    fail(pyrepr::string(name) + " previously declared as a " + attr_class_name(existing->attrs) +
         ", now redeclared as a function");
  }
  if (!types_equal(existing->type, fun_type)) {
    fail("incompatible redeclaration of " + pyrepr::string(name) + ": previous " +
         asdl::py_repr(existing->type) + ", new " + asdl::py_repr(fun_type));
  }
  if (prior->is_global != is_global) {
    fail("linkage of " + pyrepr::string(name) + " disagrees with prior declaration");
  }
  if (defined && prior->defined) {
    fail("redefinition of function " + pyrepr::string(name));
  }
  if (prior->carry_return != carry_return_in) {
    fail("every declaration of " + pyrepr::string(name) +
         " must agree on `[[c6502::return_bool_in_carry]]`");
  }
  std::optional<Int> address = address_in;
  const std::optional<Int> prior_addr = prior->address;
  if (address && prior_addr && *address != *prior_addr) {
    fail("two declarations of function " + pyrepr::string(name) +
         " give it different addresses, $" + pyrepr::hex(*prior_addr, 4) + " and $" +
         pyrepr::hex(*address, 4));
  }
  if (!address) {
    address = prior_addr;
  }
  const bool new_defined = prior->defined || defined;
  if (address && new_defined) {
    fail("function " + pyrepr::string(name) +
         " has `[[c6502::address(...)]]`, which names code that's already at $" +
         pyrepr::hex(*address, 4) + ", so it can't also be defined");
  }
  NameSet preserves = prior->preserves;
  preserves.insert(preserves_in.begin(), preserves_in.end());
  check_preserves(name, fun_type, address, preserves, &types);
  const std::optional<std::string> section = merge_section(name, prior->section, section_in);
  check_section(name, section, address, std::nullopt);
  std::vector<std::optional<Int>> param_zeropage =
      param_zeropage_in.empty() ? prior->param_zeropage : param_zeropage_in;
  const bool merged_noreturn = noreturn || prior->noreturn;
  symbols.insert_or_assign(name, Symbol{.type = fun_type,
                                        .attrs = FunAttr{.defined = new_defined,
                                                         .is_global = is_global,
                                                         .carry_return = carry_return_in,
                                                         .address = address,
                                                         .preserves = std::move(preserves),
                                                         .section = section,
                                                         .param_zeropage = std::move(param_zeropage),
                                                         .noreturn = merged_noreturn}});
}

Type TypeChecker::check_call_args(std::optional<Type>& call_type, std::string_view label,
                                  const ir::FunType& fn_type, std::vector<Exp>& args) {
  const std::string l(label);
  if (args.size() != fn_type.params.size()) {
    fail("function " + l + " called with " + std::to_string(args.size()) + " argument" +
         (args.size() != 1 ? "s" : "") + ", expected " + std::to_string(fn_type.params.size()));
  }
  if (asdl::isa<ir::Structure, ir::Union>(*fn_type.ret)) {
    const std::string& tag = tag_of(*fn_type.ret);
    const StructLayout* layout = types.find(tag);
    if (layout == nullptr || !layout->complete) {
      fail("call to " + l + ": incomplete return type '" +
           kw_of(asdl::isa<ir::Union>(*fn_type.ret)) + " " + tag + "' (C99 §6.5.2.2.1)");
    }
  }
  for (std::size_t i = 0; i < args.size(); ++i) {
    check_exp(args[i]);
    Exp arg = decay_if_array(std::move(args[i]));
    const Type& declared = fn_type.params[i];
    if (asdl::isa<ir::Structure, ir::Union>(strip_quals(declared))) {
      const Type& expected = strip_quals(declared);
      const std::string& tag = tag_of(expected);
      const StructLayout* layout = types.find(tag);
      if (layout == nullptr || !layout->complete) {
        fail("argument " + std::to_string(i) + " of " + l + ": cannot pass incomplete type '" +
             kw_of(asdl::isa<ir::Union>(expected)) + " " + tag + "'");
      }
      const std::optional<Type>& at = data_type_of(arg);
      if (!at || !types_equal(strip_quals(*at), expected)) {
        fail("argument " + std::to_string(i) + " of " + l + ": type " + repr_or_none(at) +
             " doesn't match parameter type " + asdl::py_repr(expected));
      }
      args[i] = std::move(arg);
    } else {
      args[i] = convert_to(std::move(arg), declared);
    }
  }
  call_type = *fn_type.ret;
  return *fn_type.ret;
}

Type TypeChecker::check_builtin_call(ir::FunctionCall& exp) {
  namespace bf = ::c6502::builtin_functions;
  const std::optional<int> want = bf::builtin_arity(exp.name);
  if (!want) {
    throw InternalError("check_builtin_call: " + pyrepr::string(exp.name) + " isn't a builtin");
  }
  if (static_cast<int>(exp.args.size()) != *want) {
    fail(exp.name + " takes " + std::to_string(*want) + " arguments, got " +
         std::to_string(exp.args.size()));
  }
  if (exp.name == bf::kBcdAdd || exp.name == bf::kBcdSub) {
    return check_bcd_call(exp);
  }
  if (exp.name == bf::kBinToBcd) {
    return check_bin_to_bcd_call(exp);
  }
  if (bf::find_stdbit_builtin(exp.name) != nullptr) {
    return check_stdbit_call(exp);
  }
  throw InternalError("no type rule for builtin " + exp.name);
}

Type TypeChecker::check_stdbit_call(ir::FunctionCall& exp) {
  const auto* builtin = builtin_functions::find_stdbit_builtin(exp.name);
  const std::string spelled = exp.name.substr(std::string_view("__c6502_").size());
  auto& args = exp.args;
  Type value_t;
  std::optional<Exp> arg;
  if (builtin->suffix) {
    value_t = stdbit_parameter_type(*builtin->suffix);
  } else {
    check_exp(args[0]);
    arg = decay_if_array(args[0]);
    const std::optional<Type>& at = data_type_of(*arg);
    if (!at || !is_stdbit_generic_type(strip_quals(*at))) {
      const std::string spelled_t = at ? c_type(strip_quals(*at)) : std::string("NoneType");
      fail("the argument of " + spelled + " has type " + spelled_t +
           ", and C23 §7.18 defines the type-generic functions on unsigned char, unsigned "
           "short, unsigned int, unsigned long, unsigned long long, and the unsigned _BitInt "
           "of 8, 16, 32, and 64 bits, with no integer promotion (cast it to one, or call a "
           "function such as " +
           spelled + "_ui)");
    }
    value_t = strip_quals(*at);
  }
  Type result_t;
  if (builtin->operation == "bit_floor" || builtin->operation == "bit_ceil") {
    result_t = value_t;
  } else if (builtin->operation == "has_single_bit") {
    result_t = ir::Bool{};
  } else {
    result_t = ir::UInt{};
  }
  if (builtin->suffix) {
    check_call_args(exp.data_type, spelled,
                    ir::FunType{.params = {value_t}, .ret = result_t}, args);
  } else {
    args[0] = convert_to(std::move(*arg), value_t);
    exp.data_type = result_t;
  }
  const std::optional<Int> value = evaluate_or_none(args[0]);
  if (!value) {
    return result_t;
  }
  const Int masked = *value & ((Int(1) << (8 * int_width(value_t))) - 1);
  args[0] = ir::Constant{.const_ = const_for_value(masked, value_t), .data_type = value_t};
  return result_t;
}

Type TypeChecker::check_bin_to_bcd_call(ir::FunctionCall& exp) {
  auto& args = exp.args;
  check_exp(args[0]);
  Exp arg = decay_if_array(args[0]);
  const std::optional<Type>& at = data_type_of(arg);
  if (!at || !is_integer_type(strip_quals(*at))) {
    const std::string spelled_t = at ? c_type(strip_quals(*at)) : std::string("NoneType");
    fail("the argument of " + exp.name + " has type " + spelled_t +
         "; it converts an integer to packed BCD, and takes an argument of integer type");
  }
  args[0] = convert_to(std::move(arg), ir::UInt{});
  exp.data_type = ir::ULong{};
  const std::optional<Int> value = evaluate_or_none(args[0]);
  if (!value) {
    return *exp.data_type;
  }
  args[0] = ir::Constant{.const_ = const_for_value(*value & Int(0xFFFF), ir::UInt{}),
                         .data_type = ir::UInt{}};
  return *exp.data_type;
}

Type TypeChecker::check_bcd_call(ir::FunctionCall& exp) {
  auto& args = exp.args;
  const auto order = [](const Type& t) {
    switch (ir::kind(t)) {
      case K::UChar:
        return 0;
      case K::UShort:
        return 1;
      case K::UInt:
        return 2;
      case K::ULong:
        return 3;
      case K::ULongLong:
        return 4;
      default:
        throw InternalError("not an unsigned integer type a packed BCD operand takes: " +
                            asdl::py_repr(t));
    }
  };
  struct Kind {
    Type type;
    std::optional<Int> adapts;
  };
  std::vector<Kind> kinds;
  for (std::size_t k = 0; k < args.size(); ++k) {
    check_exp(args[k]);
    args[k] = decay_if_array(std::move(args[k]));
    const std::optional<Type>& at = data_type_of(args[k]);
    const std::string where = "argument " + std::to_string(k + 1) + " of " + exp.name;
    if (!at) {
      fail(where + " has type NoneType; packed BCD operands must have an unsigned integer type");
    }
    const Type t = strip_quals(*at);
    if (asdl::isa<ir::Float, ir::Double>(t)) {
      fail(where + " has floating type " + c_type(t) +
           "; packed BCD operands must have an unsigned integer type");
    }
    if (is_pointer_type(t)) {
      fail(where + " is a pointer (" + c_type(t) +
           "); packed BCD operands must have an unsigned integer type");
    }
    if (!is_integer_type(t)) {
      fail(where + " has type " + c_type(t) +
           "; packed BCD operands must have an unsigned integer type");
    }
    const std::optional<Int> value = evaluate_or_none(args[k]);
    if (value && *value < 0) {
      fail(where + " is the negative constant " + asdl::int_to_string(*value) +
           "; packed BCD operands are unsigned");
    }
    if (asdl::isa<ir::BitInt>(t)) {
      fail(where + " has the bit-precise type " + c_type(t) +
           "; packed BCD operands must have a standard unsigned integer type (cast it to one)");
    }
    if (asdl::isa<ir::Bool, ir::Char>(t)) {
      fail(where + " has type " + c_type(t) +
           "; packed BCD operands must have an unsigned integer type other than bool and plain "
           "char (use unsigned char)");
    }
    if (is_signed(t) && !value) {
      fail(where + " has the signed type " + c_type(t) +
           "; packed BCD operands must have an unsigned integer type (cast it, or give a "
           "constant a `u` suffix)");
    }
    kinds.push_back(Kind{t, is_signed(t) ? value : std::nullopt});
  }
  std::optional<Type> result;
  for (const auto& kd : kinds) {
    if (kd.adapts) {
      continue;
    }
    if (!result || order(kd.type) > order(*result)) {
      result = kd.type;
    }
  }
  if (!result) {
    Int biggest = *kinds.front().adapts;
    for (const auto& kd : kinds) {
      biggest = std::max(biggest, *kd.adapts);
    }
    for (const Type& t : {Type(ir::UInt{}), Type(ir::ULong{}), Type(ir::ULongLong{})}) {
      if (biggest < (Int(1) << (8 * int_width(t)))) {
        result = t;
        break;
      }
    }
    if (!result) {
      throw InternalError("no unsigned integer type holds the packed BCD result");
    }
  }
  const Type result_t = *result;
  for (std::size_t k = 0; k < args.size(); ++k) {
    const std::optional<Int> value = evaluate_or_none(args[k]);
    if (!value) {
      args[k] = convert_to(std::move(args[k]), result_t);
      continue;
    }
    if (*value >= (Int(1) << (8 * int_width(result_t)))) {
      fail("argument " + std::to_string(k + 1) + " of " + exp.name + ", " + hex_prefixed(*value) +
           ", doesn't fit in " + c_type(result_t) +
           ", the type of the result (cast the other operand to widen it)");
    }
    args[k] = ir::Constant{.const_ = const_for_value(*value, result_t), .data_type = result_t};
  }
  exp.data_type = result_t;
  return result_t;
}

ir::FunType TypeChecker::check_callee(ir::IndirectCall& exp) {
  const auto get_fun = [](const Type& t) -> ir::FunType {
    if (const auto* f = std::get_if<ir::FunType>(&t)) {
      return *f;
    }
    throw InternalError("check_callee: the callee's type isn't a function type: " +
                        asdl::py_repr(t));
  };
  if (const std::optional<std::string> name = callee_name(*exp.callee, symbols)) {
    const Type t = strip_quals(symbols.find(*name)->type);
    exp.callee = ir::Var{.name = *name, .data_type = t};
    if (asdl::isa<ir::FunType>(t)) {
      return std::get<ir::FunType>(t);
    }
    return get_fun(*std::get<ir::Pointer>(t).referenced_type);
  }
  check_exp(*exp.callee);
  Exp callee = decay_if_array(std::move(*exp.callee));
  while (true) {
    auto* d = std::get_if<ir::Dereference>(&callee);
    if (d == nullptr || !d->data_type || !asdl::isa<ir::FunType>(*d->data_type)) {
      break;
    }
    const std::optional<Type>& inner = data_type_of(*d->exp);
    if (!inner || !is_pointer_to_function(*inner)) {
      break;
    }
    Exp next = std::move(*d->exp);
    callee = std::move(next);
  }
  std::optional<Type> t;
  if (const std::optional<Type>& ct = data_type_of(callee)) {
    t = strip_quals(*ct);
  }
  if (t && asdl::isa<ir::FunType>(*t)) {
    callee = decay_if_array(std::move(callee));
    t = data_type_of(callee);
  }
  if (!t || !is_pointer_to_function(*t)) {
    fail("called object is not a function or function pointer (type " + repr_or_none(t) + ")");
  }
  exp.callee = std::move(callee);
  const auto* p = std::get_if<ir::Pointer>(&*t);
  if (p == nullptr) {
    throw InternalError("check_callee: the callee's pointer type is still qualified");
  }
  return get_fun(*p->referenced_type);
}

bool TypeChecker::is_bool_var(const Exp& exp) const {
  const auto* v = std::get_if<ir::Var>(&exp);
  if (v == nullptr) {
    return false;
  }
  const Symbol* sym = symbols.find(v->name);
  return sym != nullptr && asdl::isa<ir::Bool>(strip_quals(sym->type));
}

bool TypeChecker::is_carry_call(const Exp& exp) const {
  std::optional<std::string> name;
  if (const auto* f = std::get_if<ir::FunctionCall>(&exp)) {
    name = f->name;
  } else if (const auto* c = std::get_if<ir::IndirectCall>(&exp)) {
    name = callee_name(*c->callee, symbols);
  } else {
    return false;
  }
  const Symbol* sym = name ? symbols.find(*name) : nullptr;
  if (sym == nullptr) {
    return false;
  }
  const auto* attr = std::get_if<FunAttr>(&sym->attrs);
  if (attr == nullptr || !attr->carry_return) {
    return false;
  }
  const auto* f = std::get_if<ir::FunType>(&sym->type);
  return f != nullptr && asdl::isa<ir::Bool>(strip_quals(*f->ret));
}

void TypeChecker::check_statement(ir::Type_statement& stmt) {
  using S = ir::Kind_statement;
  switch (ir::kind(stmt)) {
    case S::Return: {
      auto& ret = std::get<ir::Return>(stmt);
      if (!return_type) {
        fail("return statement outside of any function");
      }
      const Type expected = *return_type;
      if (!ret.exp) {
        if (!is_void(expected)) {
          fail("`return;` without a value in a function returning `" + c_type(expected) +
               "`; bare `return;` is only legal in a void-returning function (C99 "
               "§6.8.6.4.1)");
        }
        return;
      }
      if (is_void(expected)) {
        fail("`return <expr>;` in a void-returning function; void functions must use bare "
             "`return;` (C99 §6.8.6.4.1)");
      }
      Exp exp = std::move(*ret.exp);
      if (carry_return && !asdl::isa<ir::Structure, ir::Union>(strip_quals(expected)) &&
          !is_truth_valued(exp) && !is_carry_call(exp) && !is_bool_var(exp)) {
        // The carry flag holds one bit, so the function returns `e != 0`.
        exp = ir::Binary{.op = ir::NotEqual{},
                         .left = std::move(exp),
                         .right = ir::Constant{.const_ = ir::ConstInt{.value = 0}}};
      }
      check_exp(exp);
      exp = decay_if_array(std::move(exp));
      if (asdl::isa<ir::Structure, ir::Union>(strip_quals(expected))) {
        const std::optional<Type>& et = data_type_of(exp);
        if (!et || !types_equal(strip_quals(*et), strip_quals(expected))) {
          fail("return value type " + repr_or_none(et) + " doesn't match declared return type " +
               asdl::py_repr(expected));
        }
        ret.exp = std::move(exp);
      } else {
        ret.exp = convert_to(std::move(exp), expected);
      }
      return;
    }
    case S::Expression: {
      auto& e = std::get<ir::Expression>(stmt).exp;
      check_exp(e);
      require_complete_value(e, "expression statement");
      return;
    }
    case S::IfStmt: {
      auto& i = std::get<ir::IfStmt>(stmt);
      check_exp(i.condition);
      require_scalar_controlling(i.condition, "`if` condition");
      check_statement(*i.then_clause);
      if (i.else_clause) {
        check_statement(**i.else_clause);
      }
      return;
    }
    case S::Compound:
      push_tag_scope();
      check_block(*std::get<ir::Compound>(stmt).block);
      pop_tag_scope();
      return;
    case S::WhileStmt: {
      auto& w = std::get<ir::WhileStmt>(stmt);
      check_exp(w.condition);
      require_scalar_controlling(w.condition, "`while` condition");
      check_statement(*w.body);
      return;
    }
    case S::DoWhileStmt: {
      auto& w = std::get<ir::DoWhileStmt>(stmt);
      check_statement(*w.body);
      check_exp(w.condition);
      require_scalar_controlling(w.condition, "`do`-`while` condition");
      return;
    }
    case S::ForStmt: {
      auto& f = std::get<ir::ForStmt>(stmt);
      push_tag_scope();
      check_for_init(f.init);
      if (f.condition) {
        check_exp(*f.condition);
        require_scalar_controlling(*f.condition, "`for` condition");
      }
      if (f.post_clause) {
        check_exp(*f.post_clause);
      }
      check_statement(*f.body);
      pop_tag_scope();
      return;
    }
    case S::LabeledStmt:
      check_statement(*std::get<ir::LabeledStmt>(stmt).statement);
      return;
    case S::SwitchStmt:
      check_switch(std::get<ir::SwitchStmt>(stmt));
      return;
    case S::CaseStmt:
      check_statement(*std::get<ir::CaseStmt>(stmt).body);
      return;
    case S::DefaultStmt:
      check_statement(*std::get<ir::DefaultStmt>(stmt).body);
      return;
    case S::Goto:
    case S::BreakStmt:
    case S::ContinueStmt:
    case S::Null:
      return;
  }
}

void TypeChecker::check_switch(ir::SwitchStmt& stmt) {
  check_exp(stmt.control);
  const std::optional<Type> control_type = data_type_of(stmt.control);
  if (!control_type || !is_integer_type(*control_type)) {
    fail("switch controlling expression must have integer type (got " +
         repr_or_none(control_type) + "); C99 §6.8.4.2.1");
  }
  const Type promoted = promote_integer(*control_type);
  if (promoted != *control_type) {
    stmt.control = convert_to(std::move(stmt.control), promoted);
  }
  stmt.promoted_type = promoted;
  const Type& ctrl_type = promoted;
  std::map<std::string, ir::CaseStmt*> body_cases;
  collect_cases(*stmt.body, body_cases);
  std::set<Int> seen_values;
  for (auto& c : stmt.cases) {
    check_exp(c.value);
    if (const auto it = body_cases.find(c.label); it != body_cases.end()) {
      it->second->value = c.value;
    }
    Int value = 0;
    try {
      value = ce::evaluate_integer_constant_expression(c.value).first;
    } catch (const ce::ConstantExpressionError& e) {
      fail(std::string("case label is not an integer constant expression: ") + e.what());
    }
    const Int converted = coerce_int_to_type(value, ctrl_type);
    if (!seen_values.insert(converted).second) {
      fail("duplicate case value " + asdl::int_to_string(converted) +
           " in switch statement (C99 §6.8.4.2.3)");
    }
    c.value = ir::Constant{.const_ = const_for_value(converted, ctrl_type), .data_type = ctrl_type};
  }
  check_statement(*stmt.body);
}

void TypeChecker::check_for_init(ir::Type_for_init& init) {
  if (auto* e = std::get_if<ir::InitExp>(&init)) {
    if (e->exp) {
      check_exp(*e->exp);
      require_complete_value(*e->exp, "for-init expression");
    }
    return;
  }
  for (auto& vd : std::get<ir::InitDecl>(init).var_decls) {
    if (vd.init) {
      vd.init = normalize_init(std::move(*vd.init), vd.data_type, vd.name);
    }
    vd.data_type = size_from_initializer(vd.data_type, vd.init ? &*vd.init : nullptr,
                                         "array " + pyrepr::string(display_name(vd.name)));
    if (vd.placement || vd.section) {
      fail("`[[c6502::zeropage]]` and `section` need an object with static storage; for-init " +
           pyrepr::string(vd.name) + " is automatic");
    }
    if (!is_complete_object_type(vd.data_type)) {
      fail("for-init " + pyrepr::string(vd.name) + " declared with non-object type " +
           asdl::py_repr(vd.data_type));
    }
    check_well_formed(vd.data_type, "for-init " + pyrepr::string(vd.name),
                      asdl::isa<ir::Structure, ir::Union, ir::Array>(vd.data_type));
    const bool is_register = vd.storage_class && asdl::isa<ir::Register>(*vd.storage_class);
    if (is_register) {
      check_register_object(vd.data_type, 1, "register local " + pyrepr::string(vd.name));
    }
    symbols.insert_or_assign(
        vd.name, Symbol{.type = vd.data_type, .attrs = LocalAttr{.register_ = is_register}});
    if (!vd.init) {
      continue;
    }
    if (asdl::isa<ir::Array>(vd.data_type)) {
      if (!asdl::isa<ir::InitList>(*vd.init)) {
        fail("array " + pyrepr::string(vd.name) +
             " requires a brace-enclosed initializer (`{...}`)");
      }
      check_array_init_list(std::get<ir::InitList>(*vd.init), std::get<ir::Array>(vd.data_type),
                            vd.name);
      continue;
    }
    if (asdl::isa<ir::Structure, ir::Union>(strip_quals(vd.data_type))) {
      if (asdl::isa<ir::InitList>(*vd.init)) {
        check_struct_init_list(std::get<ir::InitList>(*vd.init), vd.data_type, vd.name);
        continue;
      }
      check_exp(*vd.init);
      const std::optional<Type>& init_t = data_type_of(*vd.init);
      if (!init_t || !types_equal(strip_quals(*init_t), strip_quals(vd.data_type))) {
        fail("struct/union " + pyrepr::string(vd.name) + ": initializer has type " +
             repr_or_none(init_t) + ", expected " + asdl::py_repr(vd.data_type));
      }
      continue;
    }
    vd.init = unbrace_scalar_init(std::move(*vd.init), vd.data_type,
                                  "scalar variable " + pyrepr::string(display_name(vd.name)));
    check_exp(*vd.init);
    vd.init = decay_if_array(std::move(*vd.init));
    vd.init = convert_to(std::move(*vd.init), vd.data_type);
  }
}

CheckedProgram check_program(ir::Program& prog) {
  TypeChecker checker;
  checker.check_program(prog);
  return CheckedProgram{std::move(checker.symbols), std::move(checker.types)};
}

}  // namespace c6502::passes::type_checking
