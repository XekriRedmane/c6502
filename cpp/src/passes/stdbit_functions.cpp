// The port of passes/stdbit_functions.py, the definitions of the
// <stdbit.h> functions a unit refers to by name.
//
// <stdbit.h> declares each function with a suffix, such as `unsigned int
// stdc_leading_zeros_uc(unsigned char value);`, and defines its name as
// a macro for the builtin `__c6502_stdc_leading_zeros_uc`
// (docs/stdbit.md). A call written the ordinary way expands to the
// builtin, which is lowered in place. C23 §7.1.4 also lets a program
// reach the function itself, by taking its address, by calling
// `(stdc_leading_zeros_uc)(x)`, whose parentheses keep the macro from
// expanding, or by calling it after `#undef`. c6502 has no library to
// hold the functions, so `add_definitions` writes a definition of each
// one a unit refers to that way, whose body returns the builtin of its
// parameter, and the type checker gives it internal linkage.
//
// A definition is added only for a name that a file-scope declaration
// without `static` declares as a function, so a program that doesn't
// include <stdbit.h> can still use the names for its own static
// functions and objects. A name the unit defines itself gets no second
// definition.
//
// The declarations of the functions the unit doesn't refer to are
// removed, all 70 of them in a unit that calls none, since the later
// passes would give each one zero-page parameter slots, `EQU` lines, and
// a line of link metadata.
//
// Each unit that refers to a function has its own copy, at its own
// address. C23 has one function with external linkage, so a pointer to
// it from one unit compares unequal to a pointer from another, which is
// the one way this falls short of the standard.
#include "passes/stdbit_functions.h"

#include <set>
#include <utility>

#include "builtin_functions.h"

namespace c6502::passes::stdbit_functions {

namespace {

namespace ir = ::c6502::c99_ast;
using Names = std::set<std::string, std::less<>>;

// Returns the builtin of the <stdbit.h> function `name`, one of a
// single parameter type such as `stdc_leading_zeros_uc`, or an empty
// string when `name` isn't one (Python's `STDBIT_FUNCTIONS`).
std::string function_builtin(std::string_view name) {
  std::string builtin = "__c6502_" + std::string(name);
  const auto* b = builtin_functions::find_stdbit_builtin(builtin);
  if (b == nullptr || !b->suffix) {
    return {};
  }
  return builtin;
}

bool is_stdbit_function(std::string_view name) { return !function_builtin(name).empty(); }

void names_in(const ir::Type_exp& e, Names& names);
void names_in(const ir::Type_statement& s, Names& names);
void names_in(const ir::Type_declaration& d, Names& names);

void names_in(const ir::Type_var_decl& vd, Names& names) {
  if (vd.init) {
    names_in(*vd.init, names);
  }
}

void names_in(const ir::Type_block& b, Names& names) {
  for (const auto& item : std::get<ir::Block>(b).block_item) {
    if (const auto* s = std::get_if<ir::S>(&item)) {
      names_in(*s->statement, names);
    } else {
      names_in(*std::get<ir::D>(item).declaration, names);
    }
  }
}

void names_in(const ir::Type_function_decl& fd, Names& names) {
  if (fd.body) {
    names_in(**fd.body, names);
  }
}

void names_in(const ir::Type_declaration& d, Names& names) {
  switch (ir::kind(d)) {
    case ir::Kind_declaration::FunctionDecl:
      names_in(*std::get<ir::FunctionDecl>(d).function_decl, names);
      break;
    case ir::Kind_declaration::VarDecl:
      names_in(std::get<ir::VarDecl>(d).var_decl, names);
      break;
    case ir::Kind_declaration::StructDecl:
      break;
  }
}

// Adds to `names` every name a `Var` or `FunctionCall` in `e` refers to.
void names_in(const ir::Type_exp& e, Names& names) {
  using E = ir::Kind_exp;
  switch (ir::kind(e)) {
    case E::Constant:
    case E::String:
    case E::SizeOfType:
      return;
    case E::Var:
      names.insert(std::get<ir::Var>(e).name);
      return;
    case E::Cast:
      names_in(*std::get<ir::Cast>(e).exp, names);
      return;
    case E::Unary:
      names_in(*std::get<ir::Unary>(e).exp, names);
      return;
    case E::Binary: {
      const auto& b = std::get<ir::Binary>(e);
      names_in(*b.left, names);
      names_in(*b.right, names);
      return;
    }
    case E::Assignment: {
      const auto& a = std::get<ir::Assignment>(e);
      names_in(*a.lval, names);
      names_in(*a.rval, names);
      return;
    }
    case E::CompoundAssignment: {
      const auto& a = std::get<ir::CompoundAssignment>(e);
      names_in(*a.lval, names);
      names_in(*a.rval, names);
      return;
    }
    case E::Postfix:
      names_in(*std::get<ir::Postfix>(e).operand, names);
      return;
    case E::Prefix:
      names_in(*std::get<ir::Prefix>(e).operand, names);
      return;
    case E::Conditional: {
      const auto& c = std::get<ir::Conditional>(e);
      names_in(*c.condition, names);
      names_in(*c.true_clause, names);
      names_in(*c.false_clause, names);
      return;
    }
    case E::FunctionCall: {
      const auto& c = std::get<ir::FunctionCall>(e);
      names.insert(c.name);
      for (const auto& a : c.args) {
        names_in(a, names);
      }
      return;
    }
    case E::IndirectCall: {
      const auto& c = std::get<ir::IndirectCall>(e);
      names_in(*c.callee, names);
      for (const auto& a : c.args) {
        names_in(a, names);
      }
      return;
    }
    case E::Dereference:
      names_in(*std::get<ir::Dereference>(e).exp, names);
      return;
    case E::AddressOf:
      names_in(*std::get<ir::AddressOf>(e).exp, names);
      return;
    case E::Subscript: {
      const auto& s = std::get<ir::Subscript>(e);
      names_in(*s.array, names);
      names_in(*s.index, names);
      return;
    }
    case E::InitList:
      for (const auto& item : std::get<ir::InitList>(e).items) {
        names_in(item, names);
      }
      return;
    case E::Designation:
      names_in(*std::get<ir::Designation>(e).init, names);
      return;
    case E::SizeOfExp:
      names_in(*std::get<ir::SizeOfExp>(e).exp, names);
      return;
    case E::Dot:
      names_in(*std::get<ir::Dot>(e).operand, names);
      return;
    case E::Arrow:
      names_in(*std::get<ir::Arrow>(e).operand, names);
      return;
    case E::Comma: {
      const auto& c = std::get<ir::Comma>(e);
      names_in(*c.left, names);
      names_in(*c.right, names);
      return;
    }
    case E::CompoundLiteral:
      names_in(*std::get<ir::CompoundLiteral>(e).init, names);
      return;
  }
}

void names_in(const ir::Type_for_init& init, Names& names) {
  if (const auto* d = std::get_if<ir::InitDecl>(&init)) {
    for (const auto& vd : d->var_decls) {
      names_in(vd, names);
    }
  } else if (const auto& e = std::get<ir::InitExp>(init).exp) {
    names_in(*e, names);
  }
}

void names_in(const ir::Type_statement& s, Names& names) {
  using S = ir::Kind_statement;
  switch (ir::kind(s)) {
    case S::Return:
      if (const auto& e = std::get<ir::Return>(s).exp) {
        names_in(*e, names);
      }
      return;
    case S::Expression:
      names_in(std::get<ir::Expression>(s).exp, names);
      return;
    case S::IfStmt: {
      const auto& i = std::get<ir::IfStmt>(s);
      names_in(i.condition, names);
      names_in(*i.then_clause, names);
      if (i.else_clause) {
        names_in(**i.else_clause, names);
      }
      return;
    }
    case S::Compound:
      names_in(*std::get<ir::Compound>(s).block, names);
      return;
    case S::WhileStmt: {
      const auto& w = std::get<ir::WhileStmt>(s);
      names_in(w.condition, names);
      names_in(*w.body, names);
      return;
    }
    case S::DoWhileStmt: {
      const auto& w = std::get<ir::DoWhileStmt>(s);
      names_in(*w.body, names);
      names_in(w.condition, names);
      return;
    }
    case S::ForStmt: {
      const auto& f = std::get<ir::ForStmt>(s);
      names_in(f.init, names);
      if (f.condition) {
        names_in(*f.condition, names);
      }
      if (f.post_clause) {
        names_in(*f.post_clause, names);
      }
      names_in(*f.body, names);
      return;
    }
    case S::SwitchStmt: {
      const auto& w = std::get<ir::SwitchStmt>(s);
      names_in(w.control, names);
      names_in(*w.body, names);
      for (const auto& c : w.cases) {
        names_in(c.value, names);
      }
      return;
    }
    case S::CaseStmt: {
      const auto& c = std::get<ir::CaseStmt>(s);
      names_in(c.value, names);
      names_in(*c.body, names);
      return;
    }
    case S::DefaultStmt:
      names_in(*std::get<ir::DefaultStmt>(s).body, names);
      return;
    case S::LabeledStmt:
      names_in(*std::get<ir::LabeledStmt>(s).statement, names);
      return;
    case S::BreakStmt:
    case S::ContinueStmt:
    case S::Goto:
    case S::Null:
      return;
  }
}

}  // namespace

std::string parameter(std::string_view name) { return "@" + std::string(name) + ".value"; }

std::vector<std::string> add_definitions(std::vector<ir::Type_declaration>& decls) {
  // The first declaration of each function, in the order of the
  // program (Python's dict keeps the order of insertion).
  std::vector<std::pair<std::string, ir::Type_data_type>> declared;
  Names declared_names;
  Names defined;
  for (const auto& d : decls) {
    const auto* f = std::get_if<ir::FunctionDecl>(&d);
    if (f == nullptr) {
      continue;
    }
    const ir::Type_function_decl& fd = *f->function_decl;
    if (!is_stdbit_function(fd.name)) {
      continue;
    }
    if (fd.body) {
      defined.insert(fd.name);
    } else if (!(fd.storage_class && asdl::isa<ir::Static>(*fd.storage_class))) {
      if (declared_names.insert(fd.name).second) {
        declared.emplace_back(fd.name, fd.data_type);
      }
    }
  }
  if (declared.empty()) {
    return {};
  }
  // Only the bodies of function definitions and the other declarations
  // are searched, so a declaration of a function doesn't count as a
  // reference to it.
  Names referenced;
  for (const auto& d : decls) {
    if (const auto* f = std::get_if<ir::FunctionDecl>(&d)) {
      if (f->function_decl->body) {
        names_in(**f->function_decl->body, referenced);
      }
    } else {
      names_in(d, referenced);
    }
  }
  std::vector<ir::Type_declaration> kept;
  kept.reserve(decls.size());
  for (auto& d : decls) {
    if (const auto* f = std::get_if<ir::FunctionDecl>(&d)) {
      const ir::Type_function_decl& fd = *f->function_decl;
      if (!fd.body && is_stdbit_function(fd.name) && referenced.count(fd.name) == 0) {
        continue;
      }
    }
    kept.push_back(std::move(d));
  }
  decls = std::move(kept);
  std::vector<std::string> added;
  for (const auto& [name, data_type] : declared) {
    if (defined.count(name) != 0 || referenced.count(name) == 0) {
      continue;
    }
    const std::string param = parameter(name);
    ir::FunctionCall call{.name = function_builtin(name), .args = {ir::Var{.name = param}}};
    ir::Block body;
    body.block_item.push_back(ir::S{.statement = ir::Return{.exp = ir::Type_exp(std::move(call))}});
    ir::Type_function_decl fd{.name = name,
                              .params = {param},
                              .body = asdl::Box<ir::Type_block>(ir::Type_block(std::move(body))),
                              .data_type = data_type,
                              .storage_class = std::nullopt,
                              .abi_annotation = std::nullopt,
                              .param_registers = {""},
                              .carry_return = false,
                              .address = std::nullopt,
                              .preserves = {},
                              .section = std::nullopt,
                              .noreturn = false};
    decls.push_back(ir::FunctionDecl{.function_decl = std::move(fd)});
    added.push_back(name);
  }
  return added;
}

}  // namespace c6502::passes::stdbit_functions
