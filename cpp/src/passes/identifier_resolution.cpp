/*
The identifier resolution pass, c99_ast to c99_ast
(passes/identifier_resolution.py).

It resolves every identifier the program writes, objects and functions
alike, to a form that later passes can treat as unambiguous, rejects a
reference that matches no declaration, and gives every declaration its
linkage (C99 §6.2.2).

Linkage decides the renaming. An identifier with no linkage gets a name
unique in the program, `@<N>.<original>`, so that later passes can
flatten the scopes without implementing them again. An identifier with
internal or external linkage keeps its spelling, because the linker (or,
for internal linkage, the later passes of the unit) finds it by name.
`@` can't appear in a C identifier, so a renamed name never collides
with anything the program could write or with an external symbol.

The linkage rules (C99 §6.2.2) are these. At file scope:

  static int x;  static int f(...);   internal
  extern int x;  extern int f(...);   the linkage of the prior visible
                                      declaration of the name (internal
                                      or external), and external
                                      without one
  int x;                              external (§6.2.2p5)
  int f(...);  int f(...) { ... }     as if `extern` (§6.2.2p5)

At block scope:

  int x;  static int x;   none. `static` at block scope changes the
                          storage duration, and §6.2.2 still gives no
                          linkage to "a block scope identifier for an
                          object declared without the storage-class
                          specifier extern".
  extern int x;           the linkage of the prior visible declaration
                          when it has one, and external otherwise. The
                          lookup goes through the scope chain, which
                          holds the file scope's names, so a block-scope
                          `extern int x;` after a file-scope `static
                          int x;` takes internal linkage.
  int f(...);             as if `extern`, by the same rule. `static` is
                          an error on a block-scope function declaration
                          (§6.2.2: "A function declaration can contain
                          the storage-class specifier static only if it
                          is at file scope").
  a parameter             none.

The counter of the unique names advances only at a rename, so two
declarations with external linkage of one name (`int foo(void); int
foo(void) { ... }`) leave the numbering alone. Block-scope struct and
union tags and compound literals take their names from the same counter,
so the order of the walk decides every number, and a program that fails
later shows those numbers in its messages.

The errors (`IdentifierResolutionError`) are a name with no linkage
declared twice in one block, a name declared twice in one scope with
different linkages (a constraint violation of §6.7 and §6.2.2, such as
`static int x; int x;` at file scope), a reference to an undeclared
name, a call of an undeclared function, `static` on a block-scope
function declaration, a builtin declared or used as a value, a storage
class on a for-init declaration, an unnamed parameter of a definition,
and an assignment, increment, decrement, or `&` whose operand isn't an
lvalue.

The walk goes over the program twice. A function body may call or read
a name declared later at file scope (`int main(void) { return foo(); }
int foo(void) { return 1; }` is well formed), so the first walk records
the linkage of every file-scope declaration before the second descends
into any body. Both walks go in source order, so each declaration's
linkage is computed against the declarations before it.

A scope maps each visible name to its resolved name, whether the current
block declared it (`inner`), and its linkage. Entering a block copies
the enclosing scope with every `inner` flag cleared. The file-scope
table seeds a function body's scope the same way, as entries of an
enclosing scope, so a declaration in the body shadows it, and the
prior-visible rule is a lookup in the current scope. A file-scope name
is visible from the end of its declarator (§6.2.1p4), so the seed holds
only the names whose declarations the second walk has reached.

Tags have scopes of the same shape without the linkage. A file-scope tag
keeps its spelling, which the type checker's table of layouts keys on,
and a block-scope tag becomes `@<N>.<tag>`, so that two blocks' tags of
one spelling are two entries in that flat table. A tag that a type names
before any declaration of it is declared in the current scope (§6.7.2.3p5).

The Python pass builds a new program, which drops the `data_type` of
every expression it rebuilds. This one resolves the program it is given,
which it takes by value, and clears those fields to match.
*/
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

// The linkage kinds of C99 §6.2.2, fixed at each declaration. An
// identifier with no linkage names a new entity at each declaration: a
// block-scope object, `static` or not, a parameter, or anything that
// isn't an object or a function. Internal linkage names one entity in
// the unit, from `static` at file scope or an `extern` redeclaration of
// such a name. External linkage names one entity across the program.
enum class Linkage { kNone, kInternal, kExternal };

// Returns the linkage as the Python enum's value spells it in messages.
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

// A visible name: its resolved spelling, whether the current block
// declared it, and its linkage.
struct Entry {
  std::string resolved;
  bool inner = false;
  Linkage linkage = Linkage::kNone;
};

using Scope = std::unordered_map<std::string, Entry>;

// A visible tag: its resolved spelling and whether the current block
// declared it.
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

// Returns whether `exp` has the shape of an lvalue (C99 §6.3.2.1p1): a
// name, `*p`, `a[i]` (which is `*(a+i)`), `p->m` (always, since the
// pointer is dereferenced), `s.m` when `s` is one, a string literal
// (the static object string lifting makes), or a compound literal (an
// unnamed object, §6.5.2.5p4). A call, a conditional, and an operator's
// result are rvalues, so `f().m` isn't one: the standard calls it a
// non-lvalue structure with temporary lifetime, which can be read and
// not assigned.
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

// Throws when a declaration names a builtin. The name is in the
// implementation's reserved namespace (C99 §7.1.3), and a call of it
// would still reach the builtin.
void reject_builtin_name(const std::string& name) {
  if (builtin_functions::is_builtin(name)) {
    fail(name + " is a builtin and can't be declared");
  }
}

// Returns the source name of a top-level declaration, or null for a
// struct or union declaration, whose tag is in a namespace of its own.
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

// Holds the counter of the unique names and the file-scope tables. One
// resolver handles one program, and each name with no linkage, each
// block-scope tag, and each compound literal advances the counter.
class Resolver {
 public:
  ir::Program resolve_program(ir::Program prog) {
    // The first walk records the linkage of every file-scope
    // declaration in source order, against the declarations before it.
    // It enters no body or initializer, so a body anywhere can name a
    // global or a function declared further down.
    for (const auto& d : prog.declaration) {
      register_file_scope(d);
    }
    // The second walk resolves the initializers, the bodies, and the
    // parameters. Each name is marked seen before its body or
    // initializer, so a function can call itself and `int x = x;`
    // names the object being declared, while a later declaration stays
    // invisible.
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
  // The file-scope ordinary identifiers, objects and functions, which
  // share one namespace in C, with their linkages. None of them is
  // renamed.
  std::unordered_map<std::string, Linkage> file_scope_;
  // The names whose file-scope declarator the second walk has reached
  // (C99 §6.2.1p4, "The scope of a file-scope identifier ... begins at
  // the point after the declarator").
  std::unordered_set<std::string> seen_at_file_scope_;
  // The tag scopes, the file scope's first. A block, a function body,
  // and a for header push a copy of the innermost with every entry
  // outer, so that a declaration of the same tag there shadows it, and
  // pop it at their end. Python compares the innermost scope with the
  // file scope's object, which is the test of a stack of one.
  std::vector<TagScope> tag_scopes_ = std::vector<TagScope>(1);

  std::string make_unique(const std::string& original) {
    std::string name = "@" + std::to_string(counter_) + "." + original;
    ++counter_;
    return name;
  }

  // Pushes a tag scope on construction and pops it on destruction.
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

  // Gives a newly declared tag its resolved name in the innermost
  // scope, as an inner entry, and returns it. A file-scope tag keeps
  // its spelling, and a block-scope tag becomes `@<N>.<tag>`.
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

  // Replaces each struct and union tag of the type `t` with its
  // resolved name in the innermost tag scope. A tag that no
  // declaration has introduced is introduced here, in the innermost
  // scope (C99 §6.7.2.3p5: a struct or union specifier in a declarator
  // introduces the tag with an incomplete type into the current
  // scope).
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
        // `const struct t x;` names the same tag as `struct t x;`.
        resolve_type(*std::get<ir::Const>(t).referenced_type);
        return;
      case ir::Kind_data_type::Volatile:
        resolve_type(*std::get<ir::Volatile>(t).referenced_type);
        return;
      default:
        // The other types carry no tag.
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

  // --- The first walk: the linkage of each file-scope declaration ---

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
        // A tag is in a namespace of its own (C99 §6.2.3), which the
        // second walk handles.
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

  // Records a file-scope name, which keeps its spelling for the linker.
  // A redeclaration with the same linkage changes nothing, which is
  // how both functions and tentative definitions of objects are
  // declared several times, and one that changes the linkage is an
  // error. C99 §6.2.2p7 makes that undefined behavior, and the table
  // has what a clear message needs.
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

  // --- The second walk: each top-level declaration's contents ---

  void resolve_file_scope_decl(ir::Type_declaration& decl) {
    switch (ir::kind(decl)) {
      case ir::Kind_declaration::VarDecl: {
        // A file-scope object keeps its name, since its linkage is
        // never none. Its initializer resolves against the file-scope
        // names seen so far, and its type against the file's tags.
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

  // Resolves a struct or union declaration's tag and its members' types
  // in the innermost tag scope. The tag is recorded before the members
  // are walked, so a member that points to the struct itself (`struct n
  // { struct n *next; };`) names the same tag instead of introducing a
  // new one.
  void resolve_struct_decl(ir::Type_struct_decl& sd) {
    const TagScope& scope = tag_scopes_.back();
    auto it = scope.find(sd.tag);
    if (it != scope.end() && it->second.inner) {
      // A redeclaration in the same block keeps the resolved name. The
      // type checker merges a forward declaration with the body.
      sd.tag = it->second.resolved;
    } else {
      // A first declaration in this scope, which may shadow an outer
      // tag of the same spelling.
      sd.tag = introduce_tag(sd.tag);
    }
    for (auto& m : sd.members) {
      resolve_type(m.data_type);
    }
  }

  // Returns a scope of the file-scope names whose declarators the walk
  // has reached (C99 §6.2.1p4), as outer entries, so that a block's
  // declaration may shadow them. It is the outermost scope of a
  // function body and the scope of a file-scope initializer.
  Scope file_scope_seed() const {
    Scope seed;
    for (const auto& [name, linkage] : file_scope_) {
      if (seen_at_file_scope_.count(name) != 0) {
        seed.emplace(name, Entry{name, false, linkage});
      }
    }
    return seed;
  }

  // Resolves a function declaration or definition, at file or block
  // scope. Its type is resolved in the enclosing tag scope, and its body
  // gets a tag scope of its own, so a struct declared in the body isn't
  // visible to the signature. `file_scope` decides whether the body's
  // outermost scope starts with the file-scope names. Only file scope
  // has definitions today.
  void resolve_function_decl(ir::Type_function_decl& fd, bool file_scope) {
    resolve_type(fd.data_type);
    Scope param_scope = resolve_params(fd.params, fd.body.has_value());
    if (!fd.body) {
      return;
    }
    // The parameters share the body's outermost scope (C99 §6.9.1p7),
    // so a declaration in the body's outermost block that reuses a
    // parameter's name is a duplicate. The parameters, with no linkage,
    // shadow any file-scope entry of the same name.
    Scope seed = file_scope ? file_scope_seed() : Scope{};
    for (auto& [name, entry] : param_scope) {
      seed[name] = std::move(entry);
    }
    TagScopeGuard guard(*this);
    for (auto& item : std::get<ir::Block>(**fd.body).block_item) {
      resolve_block_item(item, seed);
    }
  }

  // Checks that the parameters' names are distinct and renames each to
  // a new `@<N>.<name>`, in place, returning the scope of the
  // parameters. A declaration may leave a parameter unnamed (`int f(int,
  // int);`), which the C++ IR holds as the empty string for Python's
  // None, and which stays unnamed and takes no part in the check. A
  // definition must name every parameter (C99 §6.9.1p5).
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

  // --- Blocks and block-scope declarations ---

  // Resolves a block in a copy of the enclosing scope with every entry
  // outer, and in a new tag scope made the same way.
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
        // A block-scope struct or union's tag goes in the block's tag
        // scope as `@<N>.<tag>`, which the type checker reads from the
        // program.
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
        // The declaration's name goes in the current scope with its
        // spelling and linkage, where a reference in the scope chain
        // finds it and an inner block may shadow it. Its parameters get
        // names as variables do, in a scope of their own, which a
        // declaration without a body discards.
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
  // declaration's, else external. `static` is an error there.
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

  // Adds a block-scope name to `scope`, with the resolved name the
  // caller chose (a new one for no linkage, the spelling otherwise). A
  // second declaration in the same block is allowed only when both have
  // the same linkage and it isn't none, which is two declarations of
  // one external object or function, and it leaves the entry as it is.
  // Any other is a constraint violation.
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
    // The name is bound before the initializer is resolved, so `int a
    // = a;` names the object being declared, as C specifies, even
    // though reading it is undefined at run time. For the same reason
    // the initializer of a shadowing declaration, `int a = 5; { int a
    // = a; }`, reads the inner `a`.
    record_block_decl(vd.name, resolved, linkage, scope);
    resolve_type(vd.data_type);
    // `extern int x = ...;` is a matter for the type checker, so the
    // initializer resolves as any other does.
    if (vd.init) {
      resolve_exp(*vd.init, scope);
    }
    vd.name = std::move(resolved);
  }

  // --- Statements and expressions ---

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
        // A switch opens no scope of its own, as while and do-while
        // don't, though a compound body opens one as usual. The cases,
        // the default label, and the promoted type are filled in by
        // later passes, and the cases are empty here, since the case
        // statements are in the body.
        auto& sw = std::get<ir::SwitchStmt>(stmt);
        resolve_exp(sw.control, scope);
        resolve_statement(*sw.body, scope);
        return;
      }
      case ir::Kind_statement::CaseStmt: {
        // The labeling and type-checking passes require the value to
        // be a constant. Resolving it as any expression gives `case a:`
        // the error of an undeclared `a` when there's no `a`.
        auto& c = std::get<ir::CaseStmt>(stmt);
        resolve_exp(c.value, scope);
        resolve_statement(*c.body, scope);
        return;
      }
      case ir::Kind_statement::DefaultStmt:
        resolve_statement(*std::get<ir::DefaultStmt>(stmt).body, scope);
        return;
      case ir::Kind_statement::ForStmt: {
        // The for header opens a block scope of its own (C99
        // §6.8.5p5), made as a compound's is, and a tag scope, so a
        // struct declared in the init isn't visible after the loop.
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
        // Labels are in namespaces of their own, which label_resolution
        // and loop_labeling resolve.
        return;
    }
  }

  void resolve_for_init(ir::Type_for_init& init, Scope& scope) {
    switch (ir::kind(init)) {
      case ir::Kind_for_init::InitDecl:
        // A for-init declaration may have only the storage class `auto`
        // or `register` (C99 §6.8.5p3), and `register` pins the
        // variable to Y as it does any local.
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

  // Throws for an operand that must be an lvalue and isn't, showing the
  // operand as the program has it.
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
        // A string literal's bytes hold no names. String lifting, the
        // next pass, replaces each one that doesn't initialize an array
        // with the name of a file-scope static.
        return;
      case ir::Kind_exp::Cast: {
        auto& c = std::get<ir::Cast>(exp);
        resolve_type(c.target_type);
        resolve_exp(*c.exp, scope);
        c.data_type.reset();
        return;
      }
      case ir::Kind_exp::Var: {
        // Objects and functions share one namespace in C, so one lookup
        // in the block's scope finds either, since the file-scope names
        // arrive there through the seed and block-scope function
        // declarations are in the same map. The type checker decides
        // whether the name may be used this way.
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
        // The result of a unary `*` is an lvalue, and so is that of
        // `[]`, since `a[i]` is `*(a + i)` (C99 §6.3.2.1p1).
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
        // The lvalue check of `&(*e)` is the structural one of
        // AddressOf, so the operand needs none here.
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
        // `(T){...}` is an unnamed object, which gets a unique name as
        // a block-scope declaration does, with its type's tags and its
        // initializer resolved where it appears.
        auto& c = std::get<ir::CompoundLiteral>(exp);
        c.name = make_unique("compound_literal");
        resolve_type(c.target_type);
        resolve_exp(*c.init, scope);
        c.data_type.reset();
        return;
      }
      case ir::Kind_exp::InitList: {
        // The type checker allows a braced list only as an initializer.
        // Its items may be lists themselves, for a subaggregate.
        auto& l = std::get<ir::InitList>(exp);
        for (auto& item : l.items) {
          resolve_exp(item, scope);
        }
        l.data_type.reset();
        return;
      }
      case ir::Kind_exp::Designation:
        // The parser has folded each index, and designators.py looks a
        // member's name up in the struct or union, so only the
        // initializer holds names.
        resolve_exp(*std::get<ir::Designation>(exp).init, scope);
        return;
      case ir::Kind_exp::SizeOfExp: {
        // `sizeof e` doesn't evaluate `e`, but its names resolve all
        // the same, so the type checker can stamp its types and an
        // undeclared name is reported as anywhere else.
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
        // `&e` takes an lvalue: a name, `*p` (which `&` cancels, C99
        // §6.5.3.2p3), `a[i]` (which is `a + i`), a string literal
        // (§6.4.5p6, whose static object string lifting names), a
        // member, or a compound literal. The type checker rejects the
        // rest, such as a function or a `register` object.
        auto& a = std::get<ir::AddressOf>(exp);
        require_lvalue(*a.exp, "invalid operand of unary '&': ");
        resolve_exp(*a.exp, scope);
        a.data_type.reset();
        return;
      }
      case ir::Kind_exp::Dot: {
        // The member is in its struct's namespace, which the type
        // checker reads.
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
        // The arguments resolve before the name, so an undeclared name
        // among them is reported first. A builtin keeps its name, which
        // no declaration can take. The type checker checks that the
        // name denotes a function.
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
        // The callee is an expression, which the type checker checks
        // gives a function or a pointer to one.
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
