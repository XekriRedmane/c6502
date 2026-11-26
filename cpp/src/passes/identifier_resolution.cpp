#include "passes/identifier_resolution.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "builtin_functions.h"
#include "errors.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::c99_ast;

// The linkage kinds of C99 §6.2.2.
enum class Linkage { kNone, kInternal, kExternal };

const char* linkage_value(Linkage l) {
  switch (l) {
    case Linkage::kNone:
      return "none";
    case Linkage::kInternal:
      return "internal";
    case Linkage::kExternal:
      return "external";
  }
  return "";
}

struct Entry {
  std::string resolved;
  bool inner = false;
  Linkage linkage = Linkage::kNone;
};

using Scope = std::unordered_map<std::string, Entry>;

struct TagEntry {
  std::string resolved;
  bool inner = false;
};

using TagScope = std::unordered_map<std::string, TagEntry>;

[[noreturn]] void fail(const std::string& message) {
  throw CompileError("IdentifierResolutionError", message);
}

bool is_static(const std::optional<ir::Type_storage_class>& sc) {
  return sc && std::holds_alternative<ir::Static>(*sc);
}

bool is_extern(const std::optional<ir::Type_storage_class>& sc) {
  return sc && std::holds_alternative<ir::Extern>(*sc);
}

bool has_linkage(Linkage l) { return l == Linkage::kInternal || l == Linkage::kExternal; }

// The lvalue shapes of C99 §6.3.2.1p1, and §6.5.2.5p4 for a compound
// literal.
bool is_lvalue(const ir::Type_exp& exp) {
  switch (ir::kind(exp)) {
    case ir::Kind_exp::Var:
    case ir::Kind_exp::Dereference:
    case ir::Kind_exp::Subscript:
    case ir::Kind_exp::Arrow:
    case ir::Kind_exp::String:
    case ir::Kind_exp::CompoundLiteral:
      return true;
    case ir::Kind_exp::Dot:
      return is_lvalue(*std::get<ir::Dot>(exp).operand);
    default:
      return false;
  }
}

// A builtin's name is in the implementation's reserved namespace (C99
// §7.1.3).
void reject_builtin_name(const std::string& name) {
  if (builtin_functions::is_builtin(name)) {
    fail(name + " is a builtin and can't be declared");
  }
}

const std::string* decl_name(const ir::Type_declaration& decl) {
  switch (ir::kind(decl)) {
    case ir::Kind_declaration::VarDecl:
      return &std::get<ir::VarDecl>(decl).var_decl.name;
    case ir::Kind_declaration::FunctionDecl:
      return &std::get<ir::FunctionDecl>(decl).function_decl->name;
    case ir::Kind_declaration::StructDecl:
      return nullptr;
  }
  return nullptr;
}

class Resolver {
 public:
  ir::Program resolve_program(ir::Program prog) {
    for (const auto& d : prog.declaration) {
      register_file_scope(d);
    }
    for (auto& d : prog.declaration) {
      if (const std::string* name = decl_name(d)) {
        seen_at_file_scope_.insert(*name);
      }
      resolve_file_scope_decl(d);
    }
    return prog;
  }

 private:
  int counter_ = 0;
  std::unordered_map<std::string, Linkage> file_scope_;
  // The scope of a file-scope identifier begins after its declarator
  // (C99 §6.2.1p7).
  std::unordered_set<std::string> seen_at_file_scope_;
  std::vector<TagScope> tag_scopes_ = std::vector<TagScope>(1);

  std::string make_unique(const std::string& original) {
    std::string name = "@" + std::to_string(counter_) + "." + original;
    ++counter_;
    return name;
  }

  class TagScopeGuard {
   public:
    explicit TagScopeGuard(Resolver& r) : r_(r) {
      TagScope inner;
      for (const auto& [name, entry] : r_.tag_scopes_.back()) {
        inner.emplace(name, TagEntry{entry.resolved, false});
      }
      r_.tag_scopes_.push_back(std::move(inner));
    }
    ~TagScopeGuard() { r_.tag_scopes_.pop_back(); }
    TagScopeGuard(const TagScopeGuard&) = delete;
    TagScopeGuard& operator=(const TagScopeGuard&) = delete;

   private:
    Resolver& r_;
  };

  std::string introduce_tag(const std::string& source_tag) {
    std::string resolved;
    if (tag_scopes_.size() == 1) {
      resolved = source_tag;
    } else {
      resolved = "@" + std::to_string(counter_) + "." + source_tag;
      ++counter_;
    }
    tag_scopes_.back()[source_tag] = TagEntry{resolved, true};
    return resolved;
  }

  // A struct or union specifier with no visible declaration of its tag
  // declares the tag in the current scope (C99 §6.7.2.3p8).
  void resolve_type(ir::Type_data_type& t) {
    switch (ir::kind(t)) {
      case ir::Kind_data_type::Structure:
        resolve_tag(std::get<ir::Structure>(t).tag);
        return;
      case ir::Kind_data_type::Union:
        resolve_tag(std::get<ir::Union>(t).tag);
        return;
      case ir::Kind_data_type::Pointer:
        resolve_type(*std::get<ir::Pointer>(t).referenced_type);
        return;
      case ir::Kind_data_type::Array:
        resolve_type(*std::get<ir::Array>(t).element_type);
        return;
      case ir::Kind_data_type::FunType: {
        auto& f = std::get<ir::FunType>(t);
        for (auto& p : f.params) {
          resolve_type(p);
        }
        resolve_type(*f.ret);
        return;
      }
      case ir::Kind_data_type::Const:
        resolve_type(*std::get<ir::Const>(t).referenced_type);
        return;
      case ir::Kind_data_type::Volatile:
        resolve_type(*std::get<ir::Volatile>(t).referenced_type);
        return;
      default:
        return;
    }
  }

  void resolve_tag(std::string& tag) {
    const TagScope& scope = tag_scopes_.back();
    auto it = scope.find(tag);
    if (it == scope.end()) {
      tag = introduce_tag(tag);
    } else {
      tag = it->second.resolved;
    }
  }

  void register_file_scope(const ir::Type_declaration& decl) {
    switch (ir::kind(decl)) {
      case ir::Kind_declaration::VarDecl: {
        const auto& vd = std::get<ir::VarDecl>(decl).var_decl;
        record_file_scope(vd.name, file_scope_object_linkage(vd.name, vd.storage_class));
        return;
      }
      case ir::Kind_declaration::FunctionDecl: {
        const auto& fd = *std::get<ir::FunctionDecl>(decl).function_decl;
        record_file_scope(fd.name, file_scope_function_linkage(fd.name, fd.storage_class));
        return;
      }
      case ir::Kind_declaration::StructDecl:
        // A tag is in a namespace of its own (C99 §6.2.3).
        return;
    }
  }

  // Returns the linkage of a file-scope object (C99 §6.2.2p3-5):
  // internal for `static`, the prior visible declaration's for
  // `extern`, else external, and external without a specifier.
  Linkage file_scope_object_linkage(const std::string& name,
                                    const std::optional<ir::Type_storage_class>& sc) {
    if (is_static(sc)) {
      return Linkage::kInternal;
    }
    if (is_extern(sc)) {
      return extern_inherited_linkage(name);
    }
    return Linkage::kExternal;
  }

  // Returns the linkage of a file-scope function (C99 §6.2.2p3, p5):
  // internal for `static`, and otherwise, with `extern` or without a
  // specifier, the prior visible declaration's, else external.
  Linkage file_scope_function_linkage(const std::string& name,
                                      const std::optional<ir::Type_storage_class>& sc) {
    if (is_static(sc)) {
      return Linkage::kInternal;
    }
    return extern_inherited_linkage(name);
  }

  // Applies C99 §6.2.2p4: an `extern` declaration, or a function's
  // without a specifier, takes the linkage of the prior visible
  // declaration when that has linkage, and external otherwise.
  Linkage extern_inherited_linkage(const std::string& name) const {
    auto it = file_scope_.find(name);
    if (it != file_scope_.end() && has_linkage(it->second)) {
      return it->second;
    }
    return Linkage::kExternal;
  }

  // C99 §6.2.2p7 makes an identifier with both internal and external
  // linkage in one unit undefined behavior.
  void record_file_scope(const std::string& name, Linkage linkage) {
    reject_builtin_name(name);
    auto it = file_scope_.find(name);
    if (it != file_scope_.end() && it->second != linkage) {
      fail("file-scope identifier " + pyrepr::string(name) + " declared with " +
           linkage_value(linkage) + " linkage after a prior " + linkage_value(it->second) +
           " declaration");
    }
    file_scope_[name] = linkage;
  }

  void resolve_file_scope_decl(ir::Type_declaration& decl) {
    switch (ir::kind(decl)) {
      case ir::Kind_declaration::VarDecl: {
        auto& vd = std::get<ir::VarDecl>(decl).var_decl;
        Scope seed = file_scope_seed();
        resolve_type(vd.data_type);
        if (vd.init) {
          resolve_exp(*vd.init, seed);
        }
        return;
      }
      case ir::Kind_declaration::FunctionDecl:
        resolve_function_decl(*std::get<ir::FunctionDecl>(decl).function_decl, true);
        return;
      case ir::Kind_declaration::StructDecl:
        resolve_struct_decl(std::get<ir::StructDecl>(decl).struct_decl);
        return;
    }
  }

  void resolve_struct_decl(ir::Type_struct_decl& sd) {
    const TagScope& scope = tag_scopes_.back();
    auto it = scope.find(sd.tag);
    if (it != scope.end() && it->second.inner) {
      sd.tag = it->second.resolved;
    } else {
      sd.tag = introduce_tag(sd.tag);
    }
    for (auto& m : sd.members) {
      resolve_type(m.data_type);
    }
  }

  Scope file_scope_seed() const {
    Scope seed;
    for (const auto& [name, linkage] : file_scope_) {
      if (seen_at_file_scope_.count(name) != 0) {
        seed.emplace(name, Entry{name, false, linkage});
      }
    }
    return seed;
  }

  void resolve_function_decl(ir::Type_function_decl& fd, bool file_scope) {
    resolve_type(fd.data_type);
    Scope param_scope = resolve_params(fd.params, fd.body.has_value());
    if (!fd.body) {
      return;
    }
    // A parameter is in effect declared at the head of the body, so the
    // body's outermost block can't redeclare it (C99 §6.9.1p9).
    Scope seed = file_scope ? file_scope_seed() : Scope{};
    for (auto& [name, entry] : param_scope) {
      seed[name] = std::move(entry);
    }
    TagScopeGuard guard(*this);
    for (auto& item : std::get<ir::Block>(**fd.body).block_item) {
      resolve_block_item(item, seed);
    }
  }

  // A definition must name every parameter (C99 §6.9.1p5).
  Scope resolve_params(std::vector<std::string>& params, bool is_definition) {
    Scope scope;
    for (auto& original : params) {
      if (original.empty()) {
        if (is_definition) {
          fail("parameter name omitted in a function definition (C99 §6.9.1.5)");
        }
        continue;
      }
      if (scope.count(original) != 0) {
        fail("duplicate parameter name " + pyrepr::string(original));
      }
      reject_builtin_name(original);
      std::string unique = make_unique(original);
      scope[original] = Entry{unique, true, Linkage::kNone};
      original = std::move(unique);
    }
    return scope;
  }

  void resolve_block(ir::Type_block& block, const Scope& parent) {
    Scope local;
    for (const auto& [name, entry] : parent) {
      local.emplace(name, Entry{entry.resolved, false, entry.linkage});
    }
    TagScopeGuard guard(*this);
    for (auto& item : std::get<ir::Block>(block).block_item) {
      resolve_block_item(item, local);
    }
  }

  void resolve_block_item(ir::Type_block_item& item, Scope& scope) {
    switch (ir::kind(item)) {
      case ir::Kind_block_item::S:
        resolve_statement(*std::get<ir::S>(item).statement, scope);
        return;
      case ir::Kind_block_item::D:
        resolve_declaration(*std::get<ir::D>(item).declaration, scope);
        return;
    }
  }

  void resolve_declaration(ir::Type_declaration& decl, Scope& scope) {
    switch (ir::kind(decl)) {
      case ir::Kind_declaration::StructDecl:
        resolve_struct_decl(std::get<ir::StructDecl>(decl).struct_decl);
        return;
      case ir::Kind_declaration::VarDecl: {
        auto& vd = std::get<ir::VarDecl>(decl).var_decl;
        resolve_var_decl(vd, scope, block_scope_object_linkage(vd, scope));
        return;
      }
      case ir::Kind_declaration::FunctionDecl: {
        auto& fd = *std::get<ir::FunctionDecl>(decl).function_decl;
        Linkage linkage = block_scope_function_linkage(fd, scope);
        record_block_decl(fd.name, fd.name, linkage, scope);
        resolve_function_decl(fd, false);
        return;
      }
    }
  }

  // Returns the linkage of a block-scope object (C99 §6.2.2p4, p6): for
  // `extern` the prior visible declaration's, else external, and
  // otherwise none, `static` included, which changes only the storage
  // duration.
  static Linkage block_scope_object_linkage(const ir::Type_var_decl& vd, const Scope& scope) {
    if (is_extern(vd.storage_class)) {
      auto it = scope.find(vd.name);
      if (it != scope.end() && has_linkage(it->second.linkage)) {
        return it->second.linkage;
      }
      return Linkage::kExternal;
    }
    return Linkage::kNone;
  }

  // Returns the linkage of a block-scope function declaration (C99
  // §6.2.2p5): with `extern` or without a specifier, the prior visible
  // declaration's, else external.
  static Linkage block_scope_function_linkage(const ir::Type_function_decl& fd,
                                              const Scope& scope) {
    if (is_static(fd.storage_class)) {
      fail("static is not allowed on a block-scope function declaration: " +
           pyrepr::string(fd.name));
    }
    auto it = scope.find(fd.name);
    if (it != scope.end() && has_linkage(it->second.linkage)) {
      return it->second.linkage;
    }
    return Linkage::kExternal;
  }

  static void record_block_decl(const std::string& original, const std::string& resolved,
                                Linkage linkage, Scope& scope) {
    reject_builtin_name(original);
    auto it = scope.find(original);
    if (it != scope.end() && it->second.inner) {
      if (it->second.linkage != linkage || linkage == Linkage::kNone) {
        fail("duplicate declaration of " + pyrepr::string(original));
      }
      return;
    }
    scope[original] = Entry{resolved, true, linkage};
  }

  void resolve_var_decl(ir::Type_var_decl& vd, Scope& scope, Linkage linkage) {
    std::string resolved = linkage == Linkage::kNone ? make_unique(vd.name) : vd.name;
    record_block_decl(vd.name, resolved, linkage, scope);
    resolve_type(vd.data_type);
    if (vd.init) {
      resolve_exp(*vd.init, scope);
    }
    vd.name = std::move(resolved);
  }

  void resolve_statement(ir::Type_statement& stmt, Scope& scope) {
    switch (ir::kind(stmt)) {
      case ir::Kind_statement::Return: {
        auto& r = std::get<ir::Return>(stmt);
        if (r.exp) {
          resolve_exp(*r.exp, scope);
        }
        return;
      }
      case ir::Kind_statement::Expression:
        resolve_exp(std::get<ir::Expression>(stmt).exp, scope);
        return;
      case ir::Kind_statement::IfStmt: {
        auto& i = std::get<ir::IfStmt>(stmt);
        resolve_exp(i.condition, scope);
        resolve_statement(*i.then_clause, scope);
        if (i.else_clause) {
          resolve_statement(**i.else_clause, scope);
        }
        return;
      }
      case ir::Kind_statement::Compound:
        resolve_block(*std::get<ir::Compound>(stmt).block, scope);
        return;
      case ir::Kind_statement::LabeledStmt:
        resolve_statement(*std::get<ir::LabeledStmt>(stmt).statement, scope);
        return;
      case ir::Kind_statement::WhileStmt: {
        auto& w = std::get<ir::WhileStmt>(stmt);
        resolve_exp(w.condition, scope);
        resolve_statement(*w.body, scope);
        return;
      }
      case ir::Kind_statement::DoWhileStmt: {
        auto& d = std::get<ir::DoWhileStmt>(stmt);
        resolve_statement(*d.body, scope);
        resolve_exp(d.condition, scope);
        return;
      }
      case ir::Kind_statement::SwitchStmt: {
        auto& sw = std::get<ir::SwitchStmt>(stmt);
        resolve_exp(sw.control, scope);
        resolve_statement(*sw.body, scope);
        return;
      }
      case ir::Kind_statement::CaseStmt: {
        auto& c = std::get<ir::CaseStmt>(stmt);
        resolve_exp(c.value, scope);
        resolve_statement(*c.body, scope);
        return;
      }
      case ir::Kind_statement::DefaultStmt:
        resolve_statement(*std::get<ir::DefaultStmt>(stmt).body, scope);
        return;
      case ir::Kind_statement::ForStmt: {
        // The for header opens a block scope of its own (C99 §6.8.5p5).
        auto& f = std::get<ir::ForStmt>(stmt);
        Scope for_scope;
        for (const auto& [name, entry] : scope) {
          for_scope.emplace(name, Entry{entry.resolved, false, entry.linkage});
        }
        TagScopeGuard guard(*this);
        resolve_for_init(f.init, for_scope);
        if (f.condition) {
          resolve_exp(*f.condition, for_scope);
        }
        if (f.post_clause) {
          resolve_exp(*f.post_clause, for_scope);
        }
        resolve_statement(*f.body, for_scope);
        return;
      }
      case ir::Kind_statement::Goto:
      case ir::Kind_statement::BreakStmt:
      case ir::Kind_statement::ContinueStmt:
      case ir::Kind_statement::Null:
        return;
    }
  }

  void resolve_for_init(ir::Type_for_init& init, Scope& scope) {
    switch (ir::kind(init)) {
      case ir::Kind_for_init::InitDecl:
        // A for-init declaration may have only the storage class `auto`
        // or `register` (C99 §6.8.5p3).
        for (auto& vd : std::get<ir::InitDecl>(init).var_decls) {
          if (vd.storage_class && !std::holds_alternative<ir::Register>(*vd.storage_class)) {
            fail("storage-class specifier not allowed on a for-init declaration: " +
                 pyrepr::string(vd.name));
          }
          resolve_var_decl(vd, scope, Linkage::kNone);
        }
        return;
      case ir::Kind_for_init::InitExp: {
        auto& e = std::get<ir::InitExp>(init);
        if (e.exp) {
          resolve_exp(*e.exp, scope);
        }
        return;
      }
    }
  }

  static void require_lvalue(const ir::Type_exp& exp, const char* what) {
    if (!is_lvalue(exp)) {
      fail(std::string(what) + asdl::py_repr(exp));
    }
  }

  void resolve_exp(ir::Type_exp& exp, Scope& scope) {
    switch (ir::kind(exp)) {
      case ir::Kind_exp::Constant:
        std::get<ir::Constant>(exp).data_type.reset();
        return;
      case ir::Kind_exp::String:
        return;
      case ir::Kind_exp::Cast: {
        auto& c = std::get<ir::Cast>(exp);
        resolve_type(c.target_type);
        resolve_exp(*c.exp, scope);
        c.data_type.reset();
        return;
      }
      case ir::Kind_exp::Var: {
        auto& v = std::get<ir::Var>(exp);
        auto it = scope.find(v.name);
        if (it == scope.end()) {
          if (builtin_functions::is_builtin(v.name)) {
            fail(v.name + " is a builtin, which can only be called");
          }
          fail("undeclared identifier " + pyrepr::string(v.name));
        }
        v.name = it->second.resolved;
        v.data_type.reset();
        return;
      }
      case ir::Kind_exp::Unary: {
        auto& u = std::get<ir::Unary>(exp);
        resolve_exp(*u.exp, scope);
        u.data_type.reset();
        return;
      }
      case ir::Kind_exp::Binary: {
        auto& b = std::get<ir::Binary>(exp);
        resolve_exp(*b.left, scope);
        resolve_exp(*b.right, scope);
        b.data_type.reset();
        return;
      }
      case ir::Kind_exp::Assignment: {
        auto& a = std::get<ir::Assignment>(exp);
        require_lvalue(*a.lval, "invalid lvalue in assignment: ");
        resolve_exp(*a.lval, scope);
        resolve_exp(*a.rval, scope);
        a.data_type.reset();
        return;
      }
      case ir::Kind_exp::CompoundAssignment: {
        auto& a = std::get<ir::CompoundAssignment>(exp);
        require_lvalue(*a.lval, "invalid lvalue in compound assignment: ");
        resolve_exp(*a.lval, scope);
        resolve_exp(*a.rval, scope);
        a.intermediate_type.reset();
        a.data_type.reset();
        return;
      }
      case ir::Kind_exp::Conditional: {
        auto& c = std::get<ir::Conditional>(exp);
        resolve_exp(*c.condition, scope);
        resolve_exp(*c.true_clause, scope);
        resolve_exp(*c.false_clause, scope);
        c.data_type.reset();
        return;
      }
      case ir::Kind_exp::Comma: {
        auto& c = std::get<ir::Comma>(exp);
        resolve_exp(*c.left, scope);
        resolve_exp(*c.right, scope);
        c.data_type.reset();
        return;
      }
      case ir::Kind_exp::Postfix: {
        auto& p = std::get<ir::Postfix>(exp);
        require_lvalue(*p.operand, "invalid lvalue in postfix: ");
        resolve_exp(*p.operand, scope);
        p.data_type.reset();
        return;
      }
      case ir::Kind_exp::Prefix: {
        auto& p = std::get<ir::Prefix>(exp);
        require_lvalue(*p.operand, "invalid lvalue in prefix increment/decrement: ");
        resolve_exp(*p.operand, scope);
        p.data_type.reset();
        return;
      }
      case ir::Kind_exp::Dereference: {
        auto& d = std::get<ir::Dereference>(exp);
        resolve_exp(*d.exp, scope);
        d.data_type.reset();
        return;
      }
      case ir::Kind_exp::Subscript: {
        auto& s = std::get<ir::Subscript>(exp);
        resolve_exp(*s.array, scope);
        resolve_exp(*s.index, scope);
        s.data_type.reset();
        return;
      }
      case ir::Kind_exp::CompoundLiteral: {
        auto& c = std::get<ir::CompoundLiteral>(exp);
        c.name = make_unique("compound_literal");
        resolve_type(c.target_type);
        resolve_exp(*c.init, scope);
        c.data_type.reset();
        return;
      }
      case ir::Kind_exp::InitList: {
        auto& l = std::get<ir::InitList>(exp);
        for (auto& item : l.items) {
          resolve_exp(item, scope);
        }
        l.data_type.reset();
        return;
      }
      case ir::Kind_exp::Designation:
        resolve_exp(*std::get<ir::Designation>(exp).init, scope);
        return;
      case ir::Kind_exp::SizeOfExp: {
        auto& s = std::get<ir::SizeOfExp>(exp);
        resolve_exp(*s.exp, scope);
        s.data_type.reset();
        return;
      }
      case ir::Kind_exp::SizeOfType: {
        auto& s = std::get<ir::SizeOfType>(exp);
        resolve_type(s.target_type);
        s.data_type.reset();
        return;
      }
      case ir::Kind_exp::AddressOf: {
        auto& a = std::get<ir::AddressOf>(exp);
        require_lvalue(*a.exp, "invalid operand of unary '&': ");
        resolve_exp(*a.exp, scope);
        a.data_type.reset();
        return;
      }
      case ir::Kind_exp::Dot: {
        auto& d = std::get<ir::Dot>(exp);
        resolve_exp(*d.operand, scope);
        d.data_type.reset();
        return;
      }
      case ir::Kind_exp::Arrow: {
        auto& a = std::get<ir::Arrow>(exp);
        resolve_exp(*a.operand, scope);
        a.data_type.reset();
        return;
      }
      case ir::Kind_exp::FunctionCall: {
        auto& f = std::get<ir::FunctionCall>(exp);
        for (auto& arg : f.args) {
          resolve_exp(arg, scope);
        }
        f.data_type.reset();
        if (builtin_functions::is_builtin(f.name)) {
          return;
        }
        auto it = scope.find(f.name);
        if (it == scope.end()) {
          fail("undeclared identifier " + pyrepr::string(f.name));
        }
        f.name = it->second.resolved;
        return;
      }
      case ir::Kind_exp::IndirectCall: {
        auto& c = std::get<ir::IndirectCall>(exp);
        resolve_exp(*c.callee, scope);
        for (auto& arg : c.args) {
          resolve_exp(arg, scope);
        }
        c.data_type.reset();
        return;
      }
    }
  }
};

}  // namespace

c99_ast::Program resolve_identifiers(c99_ast::Program prog) {
  return Resolver().resolve_program(std::move(prog));
}

}  // namespace c6502::passes
