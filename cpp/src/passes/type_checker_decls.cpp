// Part T-C of the port of passes/type_checking.py: `check_program`, the
// file-scope and struct declarations, the layouts, functions, calls and
// the builtins' checks, and statements (type_checking.py:2538-3636 and
// 4439-4775).
//
// The type checker walks a program once, after identifier_resolution,
// label_resolution, and loop_labeling. It checks that every identifier is
// used as its declaration allows, computes the initial value of each
// object with static storage and whether each function is defined, and
// fills a symbol table keyed by each identifier's resolved name. That
// table is what every later pass reads to learn what a name means.
// c99_to_tac reads it to set `is_global` on each TAC function and to
// list every object with static storage, and the back end reads it to
// size operands and to tell internal names from external ones.
//
// The types are the c99 IR's own (`c99_ast::Int` and the rest), which
// the parser puts on every declaration. Types compare by value.
//
// Mixed arithmetic follows C99's usual arithmetic conversions
// (§6.3.1.8): the operand whose type isn't the common type is wrapped in
// an implicit `Cast`, so every operand that reaches TAC has its own
// type, and every change of size or signedness is an explicit cast. A
// cast expression is accepted in any direction between object types,
// and the conversion is code generation's work. The cast's target type
// is the type of the expression.
//
// A symbol carries a type and one of three kinds of attributes.
// `LocalAttr` is an object with automatic storage, a block-scope object
// or a parameter, with a slot of its own in each activation.
// `StaticAttr` is an object with static storage, every file-scope object
// and each block-scope `static`, with its initial value (`Initial`,
// `Tentative`, or `NoInitializer`) and whether it has external linkage.
// `FunAttr` is a function, which becomes defined at its first definition,
// and a second definition is an error.
//
// The initial values follow C99 §6.7.8 and §6.9.2, with the expected
// type from the declaration. A file-scope `T x;` without `extern` is
// `Tentative`, `T x = c;` is `Initial(c)`, `extern T x;` is
// `NoInitializer`, and `extern T x = c;` is `Initial(c)`. A block-scope
// `static T x;` is `Initial(0)` (§6.7.8p10), `static T x = c;` is
// `Initial(c)`, and `extern T x;` is `NoInitializer`. A block-scope
// automatic object has a `LocalAttr`, and c99_to_tac lowers its
// initializer to a copy at run time. The initializer of an object with
// static storage has to be a constant expression whose type converts to
// the declared type.
//
// The checker raises `TypeCheckError` for a function designator where an
// object is required, a variable called as a function, a call with the
// wrong number of arguments or an argument of the wrong type, operands
// that no conversion makes compatible, an initializer, cast, or return
// value of a type that can't be assigned, a static initializer that
// isn't constant, a cast to a type that isn't an object type, an
// incompatible redeclaration, and a second definition.
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

// Returns Python's `type(attrs).__name__` for a symbol's attributes.
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

// Returns `v` as Python's `f"{v:#x}"` gives it for a nonnegative value.
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

// Finds every `CaseStmt` in a switch's body, by its label, for
// `check_switch`. A nested switch's cases have labels of their own, so
// one map serves both.
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

// ---------------------------------------------------------------------
// The program and its file-scope declarations.
// ---------------------------------------------------------------------

void TypeChecker::check_program(ir::Program& prog) {
  // The <stdbit.h> functions the program reaches other than through
  // their macros get definitions here, with internal linkage, since no
  // library has them (passes/stdbit_functions.py).
  const std::vector<std::string> library = stdbit_functions::add_definitions(prog.declaration);
  {
    const ce::NamedConstants resolver([this](const Exp& e) { return named_constant(e); });
    for (auto& d : prog.declaration) {
      check_file_scope_declaration(d);
    }
  }
  for (const std::string& name : library) {
    Symbol* sym = symbols.find(name);
    if (sym == nullptr) {
      throw InternalError("AttributeError: 'NoneType' object has no attribute 'attrs'");
    }
    // Python's `dataclasses.replace(sym.attrs, is_global=False)`. Each
    // name is a function's, so the attributes are a `FunAttr`.
    if (auto* f = std::get_if<FunAttr>(&sym->attrs)) {
      f->is_global = false;
    } else if (auto* s = std::get_if<StaticAttr>(&sym->attrs)) {
      s->is_global = false;
    } else {
      throw InternalError("TypeError: LocalAttr.__init__() got an unexpected keyword argument "
                          "'is_global'");
    }
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

// Checks a struct or union declaration and, when it has a body, computes
// its layout. The tag becomes visible in the current scope. A tag may be
// declared again in one scope, so a forward declaration and then a body
// completes the layout, and two bodies for one tag in one scope are a
// redefinition.
void TypeChecker::check_struct_decl(ir::Type_struct_decl& sd) {
  const std::string tag = sd.tag;
  const bool is_union = sd.is_union;
  const StructLayout* prior_layout = types.find(tag);
  const bool has_prior = prior_layout != nullptr;
  const bool prior_is_union = has_prior && prior_layout->is_union;
  const bool prior_complete = has_prior && prior_layout->complete;
  // The tag enters the current tag scope whether or not an outer scope
  // has it. An inner tag of the same spelling shadows the outer one for
  // visibility, and the flat type table shares the layout, which is
  // enough for the programs c6502 compiles.
  record_tag_visible(tag);
  if (sd.members.empty()) {
    // A forward declaration registers an incomplete layout when nothing
    // is registered yet, and leaves an existing entry alone.
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
  // A definition with a body.
  if (has_prior) {
    if (prior_is_union != is_union) {
      fail("redeclaration of '" + kw_of(prior_is_union) + " " + tag + "' as a " +
           kw_of(is_union));
    }
    if (prior_complete) {
      fail("redefinition of '" + kw_of(is_union) + " " + tag + "'");
    }
  }
  // An incomplete layout goes in before the final one is computed, so
  // that a member pointing to the struct itself (`struct list { struct
  // list *next; };`) resolves its pointee through the declaration in
  // progress. A pointer's pointee doesn't have to be complete, so the
  // incomplete entry is enough.
  if (!has_prior) {
    types.insert_or_assign(tag, StructLayout{.tag = tag,
                                             .is_union = is_union,
                                             .members = {},
                                             .size = 0,
                                             .complete = false});
  }
  // Each member's type must be a complete object type, so a struct that
  // contains itself is rejected unless it does so through a pointer.
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
    // A member can't have void type or a function type.
    if (asdl::isa<ir::Void>(mtype)) {
      fail(where + " cannot have void type");
    }
    if (asdl::isa<ir::FunType>(mtype)) {
      fail(where + " cannot have function type");
    }
    // A member that contains the struct itself other than through a
    // pointer (`struct foo { struct foo inner; };`) is rejected.
    if (contains_self_struct(mtype, sd.tag)) {
      fail(where + " recursively contains the same struct/union type (use a pointer instead)");
    }
    check_well_formed(mtype, where, true);
    const Int msize = sizeof_type(mtype, &types);
    members.push_back(MemberInfo{.name = m.name, .type = mtype, .byte_offset = offset});
    // Every member of a union is at offset 0, and its size is the
    // largest member's.
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

// Returns whether `t` holds a struct or union of tag `tag` other than
// through a pointer, looking through the element types of arrays.
bool TypeChecker::contains_self_struct(const Type& t, const std::string& tag) const {
  if (asdl::isa<ir::Structure, ir::Union>(t) && tag_of(t) == tag) {
    return true;
  }
  if (const auto* a = std::get_if<ir::Array>(&t)) {
    return contains_self_struct(*a->element_type, tag);
  }
  return false;
}

// Returns the member `member` of the struct or union type `t`, or throws
// for an incomplete type or a missing member. A top-level qualifier is
// looked through, and propagating it to the member's type is the
// caller's work.
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

// The linkage of a file-scope object follows identifier_resolution:
// `static` gives internal linkage, `extern` the linkage of the visible
// prior declaration (external when there is none), and no specifier
// external linkage. The initial value depends on the storage class
// itself: `extern` without an initializer is `NoInitializer` (a
// reference), `extern` with one is `Initial` (a definition by its
// initializer), and otherwise no initializer is `Tentative` and an
// initializer `Initial`.
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
  // An `extern` reference to a struct may have an incomplete type, since
  // the definition can be in another unit. Otherwise the type must be
  // complete.
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
    // A file-scope `int a[3] = {1, 2, 3};` takes the path of a
    // block-scope `static` array: the list is checked and its constant
    // values lifted into a tuple padded with zeros of the element type.
    // A string literal for an array of characters (`char a[N] = "abc";`)
    // is C99 §6.7.8p14's special case.
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
    // A file-scope `struct s x = {1, 2, 3};` has its list checked against
    // the layout and lifted to a value tree. A structure constant takes
    // the place of the list.
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
    // The initializer is checked as any expression and converted to the
    // declared type, as an assignment's value is, so it is a constant of
    // the declared type or a `Cast` to that type, which
    // `const_init_value` looks through. A braced scalar (`int x = {5};`)
    // or the empty initializer (`int x = {};`) is replaced by its
    // expression first. An array decays before the conversion to a
    // pointer, for `static char *p = arr;` and for a string literal after
    // string_lifting.
    vd.init = unbrace_scalar_init(std::move(*vd.init), vd.data_type,
                                  "scalar variable " + pyrepr::string(display_name(vd.name)));
    check_exp(*vd.init);
    vd.init = decay_if_array(std::move(*vd.init));
    vd.init = convert_to(std::move(*vd.init), vd.data_type);
    initial = Initial{const_init_value(*vd.init, vd.data_type, vd.name, &symbols, &types)};
  }
  // The linkage comes back from the storage class.
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

// Checks a function's declaration or definition, which `body` tells
// apart. `file_scope` changes nothing yet, and Python keeps it for the
// call sites' clarity.
void TypeChecker::check_function_decl(ir::Type_function_decl& fd, bool /*file_scope*/) {
  if (!asdl::isa<ir::FunType>(fd.data_type)) {
    throw InternalError("function decl " + pyrepr::string(fd.name) +
                        " has non-FunType data_type " + asdl::py_repr(fd.data_type));
  }
  const ir::FunType ftype = std::get<ir::FunType>(fd.data_type);
  const bool defined = fd.body.has_value();
  // A file-scope function follows the rules of `static`, `extern`, and
  // no specifier. A block-scope one always has external linkage, since
  // the resolver accepts only no specifier and `extern` for functions.
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
    // A parameter can't have void type (C99 §6.7.5.3p4). The parser
    // handles `f(void)` itself, so a `void` here is a parameter such as
    // `int f(void x)`.
    if (is_void(p_type)) {
      fail(where_param + " cannot have void type (C99 §6.7.5.3.4)");
    }
    // The adjustment of a parameter array (§6.7.5.3p7) rewrites only the
    // outermost array to a pointer, so an `Array` here is an inner one,
    // as in `int (*a)[3]`, whose element type must be complete. A struct
    // or union parameter's tag must be declared, and a definition needs
    // the type complete, since its body reads the bytes. A declaration
    // may name an incomplete struct that is completed before a call
    // (C99 §6.7.2.1p8).
    const bool require_complete = asdl::isa<ir::Array>(p_type) ||
                                  (defined && asdl::isa<ir::Structure, ir::Union>(p_type));
    check_well_formed(p_type, where_param, require_complete);
  }
  // The return type must be well formed too. A definition's struct or
  // union return type must be complete, and a declaration's may stay
  // incomplete until a caller uses the result.
  const Type& ret_t = *ftype.ret;
  const bool require_ret_complete = defined && asdl::isa<ir::Structure, ir::Union>(ret_t);
  check_well_formed(ret_t, "return type of function " + pyrepr::string(fd.name),
                    require_ret_complete);
  if (!defined) {
    return;
  }
  // The parameters share the body's outermost scope (§6.9.1p7), so they
  // go in the symbol table with `LocalAttr` before the body is checked.
  // The `register` markers run parallel to `params`.
  std::vector<std::string> param_regs = fd.param_registers;
  while (param_regs.size() < fd.params.size()) {
    param_regs.push_back("");
  }
  const std::size_t n = std::min({fd.params.size(), ftype.params.size(), param_regs.size()});
  for (std::size_t i = 0; i < n; ++i) {
    const std::string& p_name = fd.params[i];
    const Type& p_type = ftype.params[i];
    // "@addr" is a `zeropage(addr)` parameter, which isn't `register`.
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
  // The function body gets a tag scope of its own. C99 §6.2.1p4 gives a
  // prototype a scope of its own too, but c6502 allows no struct or
  // union definition in a parameter declaration, so the only tags here
  // are the surrounding scope's.
  push_tag_scope();
  check_block(**fd.body);
  pop_tag_scope();
  return_type = std::move(saved);
  carry_return = saved_carry;
}

// ---------------------------------------------------------------------
// Merging declarations into the symbol table.
// ---------------------------------------------------------------------

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
    // `extern int t[];` and `int t[3];` declare one object, of the
    // composite type `int[3]`.
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
  // `zeropage` accumulates across declarations, as GCC's attributes do,
  // and two different fixed addresses conflict.
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
  // The attribute changes the calling convention, so a declaration
  // without it would compile callers that read A. Unlike `zeropage`, it
  // doesn't accumulate across declarations.
  if (prior->carry_return != carry_return_in) {
    fail("every declaration of " + pyrepr::string(name) +
         " must agree on `[[c6502::return_bool_in_carry]]`");
  }
  // `address` accumulates across declarations, like `zeropage`.
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
  // `preserves_*` accumulates across declarations, like `address`.
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

// ---------------------------------------------------------------------
// Calls and the builtins.
// ---------------------------------------------------------------------

// Each argument is converted as if by assignment to its parameter's
// type (C99 §6.5.2.2p7), in place, so the back end sees the converted
// list, as for an assignment, a binary operator, and `?:`. A struct or
// union argument bypasses `convert_to`, which doesn't model structs, and
// must have exactly the parameter's type, complete at the call (C99
// §6.7.2.1p8), since the caller can't materialize the value otherwise.
// The function's return type must be `void` or complete at the call
// (C99 §6.5.2.2p1). A declaration may return an incomplete struct, but a
// call needs the type completed first.
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

// Types a call of a builtin (builtin_functions.py). Each builtin has its
// own rules, which docs/builtins.md gives.
Type TypeChecker::check_builtin_call(ir::FunctionCall& exp) {
  namespace bf = ::c6502::builtin_functions;
  const std::optional<int> want = bf::builtin_arity(exp.name);
  if (!want) {
    throw InternalError("KeyError: " + pyrepr::string(exp.name));
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

// Types a <stdbit.h> builtin (C23 §7.18, docs/stdbit.md). A form with a
// suffix, such as `__c6502_stdc_leading_zeros_uc`, is checked as a call
// of a function with a parameter of its type, so the argument is
// converted as if by assignment. The type-generic form takes the type of
// its argument, which has to be a standard unsigned integer type other
// than `bool` (§7.18.3p2), with no integer promotion. `bit_floor` and
// `bit_ceil` return a value of the argument's type, `has_single_bit` a
// `bool`, and the rest an `unsigned int`, which C23 gives the forms with
// a suffix and leaves to the implementation for the type-generic form
// ("a suitable large unsigned integer type"). A constant argument
// becomes a `Constant` of the value type, which c99_to_tac folds.
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

// Types `__c6502_bin_to_bcd(x)`. The argument has any integer type and
// is converted to `unsigned int` as by assignment, so a negative value
// or one wider than 16 bits is taken modulo 65536. The result is an
// `unsigned long` holding the value's five decimal digits in packed BCD
// (docs/builtins.md). A constant argument becomes a `Constant` of
// `unsigned int`, which c99_to_tac folds.
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

// Types `__c6502_bcd_add(a, b)` and `__c6502_bcd_sub(a, b)`. Each operand
// has an unsigned integer type other than `bool`, or is a nonnegative
// integer constant expression of any integer type (a plain `0x75`, which
// is an `int`). The result has the operand type of higher rank, as the
// usual arithmetic conversions give it without the integer promotions,
// so two `unsigned char`s give an `unsigned char`. A constant of signed
// type takes the other operand's type and must fit in it, and two of
// them give the first of `unsigned int`, `unsigned long`, and `unsigned
// long long` that holds both. Every operand is converted to the result
// type, so a narrower one is zero-extended, and a constant becomes a
// `Constant` of that type, which c99_to_tac folds.
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
        throw InternalError("ValueError: " + asdl::py_repr(t) + " is not in tuple");
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
    // `max` keeps the first of equal keys.
    if (!result || order(kd.type) > order(*result)) {
      result = kd.type;
    }
  }
  if (!result) {
    // A signed constant is at most 2**63 - 1, so one of the three holds
    // it.
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
      throw InternalError("StopIteration");
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

// Type-checks the callee of `exp`, replacing it with the expression that
// gives the function's address, and returns the function's type. A `*`
// of a function or function pointer and a `&` of a function name the
// same function (C99 §6.3.2.1p4, §6.5.3.2p4), so `(*fp)(x)`, `(&f)(x)`,
// and `(*t[0])(x)` call `fp`, `f`, and `t[0]`. When that leaves a name,
// the callee is that `Var`, and c99_to_tac calls it as it would `fp(x)`.
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
    throw InternalError("AttributeError: the callee's pointer type is qualified");
  }
  return get_fun(*p->referenced_type);
}

// Returns whether `exp` names a `bool` object, whose value is already 0
// or 1, so a carry-flag function returns it without testing it against
// 0 again.
bool TypeChecker::is_bool_var(const Exp& exp) const {
  const auto* v = std::get_if<ir::Var>(&exp);
  if (v == nullptr) {
    return false;
  }
  const Symbol* sym = symbols.find(v->name);
  return sym != nullptr && asdl::isa<ir::Bool>(strip_quals(sym->type));
}

// Returns whether `exp` calls a `return_bool_in_carry` function that
// returns a `bool`, whose result is already 0 or 1, so returning it from
// another such function passes the carry on as it is.
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

// ---------------------------------------------------------------------
// Statements.
// ---------------------------------------------------------------------

void TypeChecker::check_statement(ir::Type_statement& stmt) {
  using S = ir::Kind_statement;
  switch (ir::kind(stmt)) {
    case S::Return: {
      auto& ret = std::get<ir::Return>(stmt);
      // Without an enclosing function there is no return type, which
      // the parser doesn't allow.
      if (!return_type) {
        fail("return statement outside of any function");
      }
      const Type expected = *return_type;
      // A bare `return;` is legal only in a function returning void (C99
      // §6.8.6.4p1).
      if (!ret.exp) {
        if (!is_void(expected)) {
          fail("`return;` without a value in a function returning `" + c_type(expected) +
               "`; bare `return;` is only legal in a void-returning function (C99 "
               "§6.8.6.4.1)");
        }
        return;
      }
      // "A return statement with an expression shall not appear in a
      // function whose return type is void" (§6.8.6.4p1).
      if (is_void(expected)) {
        fail("`return <expr>;` in a void-returning function; void functions must use bare "
             "`return;` (C99 §6.8.6.4.1)");
      }
      Exp exp = std::move(*ret.exp);
      if (carry_return && !asdl::isa<ir::Structure, ir::Union>(strip_quals(expected)) &&
          !is_truth_valued(exp) && !is_carry_call(exp) && !is_bool_var(exp)) {
        // The carry flag holds one bit, so the function returns `e !=
        // 0`, as a `bool` would convert `e`. Wrapping before the check
        // lets `p != 0` on a pointer and the decay of an array work.
        exp = ir::Binary{.op = ir::NotEqual{},
                         .left = std::move(exp),
                         .right = ir::Constant{.const_ = ir::ConstInt{.value = 0}}};
      }
      check_exp(exp);
      // The value is converted to the return type (C99 §6.8.6.4p3) after
      // the decay of an array, as `int *f() { return arr; }` needs. A
      // struct or union value bypasses `convert_to` and must have exactly
      // the declared type.
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
      // `e;` evaluates `e` and discards it, and the value of an
      // incomplete struct or union can't be materialized.
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
      // A compound statement is a scope of its own for tags.
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
      // The header opens a block scope of its own (C99 §6.8.5p3), so a
      // struct declared in it isn't visible after the loop, and a
      // compound body opens another inside it.
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
      // The owning switch's `check_switch` types the case's value, so
      // only the statement inside is checked here.
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

// The controlling expression of a switch shall have integer type (C99
// §6.8.4.2p1), and it goes through the integer promotions (§6.3.1.1),
// in an implicit cast, so the dispatch in c99_to_tac reads the promoted
// value. That matters for a switch on a char, whose case constants would
// otherwise be reduced modulo 256, so a case of 33554632 would become
// -56 and match a `char c = -56`. The promoted type is stamped on the
// switch for c99_to_tac. Each case value must be an integer constant
// expression (§6.6p6, §6.8.4.2p3), and no two may be equal after the
// conversion to the promoted type (§6.8.4.2p3), so `case 65536L:` in a
// switch on an `int` becomes 0 and conflicts with `case 0:`. Each value
// is then replaced by one constant of the promoted type, so c99_to_tac
// sees a uniform shape.
//
// loop_labeling puts one expression object in both the switch's `cases`
// and the body's `CaseStmt`, so Python's check of the value stamps the
// one in the body too, and the replacement afterward changes only the
// switch's entry. The dump the C++ checker reads holds two copies, so
// each stamped value is copied into the body's `CaseStmt` of the same
// label.
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
    // The value is checked first, so that a `sizeof e` inside it has
    // `e`'s type stamped, which the evaluator reads.
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
    // The value is converted to the promoted type by the reduction
    // modulo its width that a cast at run time makes.
    const Int converted = coerce_int_to_type(value, ctrl_type);
    if (!seen_values.insert(converted).second) {
      fail("duplicate case value " + asdl::int_to_string(converted) +
           " in switch statement (C99 §6.8.4.2.3)");
    }
    c.value = ir::Constant{.const_ = const_for_value(converted, ctrl_type), .data_type = ctrl_type};
  }
  // The body is checked last, and its case and default statements only
  // descend into what they label.
  check_statement(*stmt.body);
}

void TypeChecker::check_for_init(ir::Type_for_init& init) {
  if (auto* e = std::get_if<ir::InitExp>(&init)) {
    if (e->exp) {
      check_exp(*e->exp);
      // As for an expression statement, the value is discarded, so an
      // incomplete struct or union can't be used here.
      require_complete_value(*e->exp, "for-init expression");
    }
    return;
  }
  // The resolver allows no storage class on a declaration in a `for`
  // header other than `register`, so each is `T name = e;` and gets a
  // `LocalAttr`.
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
      // A block-scope declaration takes the same two forms: a braced
      // list, or a copy of a struct of the type.
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
    // The initializer is checked and then converted, as a block-scope
    // declaration's is.
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
