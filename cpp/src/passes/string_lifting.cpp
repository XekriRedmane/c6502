/*
The string lifting pass, c99_ast to c99_ast (passes/string_lifting.py).

A string literal is an object of static storage duration (C99 §6.4.5p6),
and most expressions use it by reference, through its decay to `char *`.
Only a direct initializer of a character array, `char a[N] = "abc";`,
takes its bytes as the array's own. This pass hoists every other string
into a file-scope declaration `static char str@N[len+1] = "...";` and
puts a `Var` naming it where the string was, so the passes after it
treat `char *p = "abc"`, `&"abc"`, and `"abc"[1]` as uses of an
ordinary file-scope array. The `@` keeps the name apart from every C
identifier, and with no leading `.` it isn't a dasm local label, which
would be visible only inside the function emitted just before it.

Each literal gets a name of its own, numbered from 0 by one counter in
the order of Python's walk, which visits the fields of each node in the
order its constructor names them (`for`'s init, condition, step, and
body; a do-while's body before its condition). The new declarations go
in front of the program's, so that a static initializer such as
`static char *p = "abc";`, which becomes `&str@0`, finds its target
declared before it.

A string stays in place when it initializes a character array: the
initializer of a `char`, `signed char`, or `unsigned char` array (with
any qualifiers on the element), at any depth of a braced list whose
element or member there is such an array, and `{"abc"}` for such an
array, which is `"abc"` (§6.7.8p14). Each item of a braced list goes to
the subobject it initializes. An array's items go to its element type,
and a struct's to its members in order, from the bodies of the struct
and union declarations seen so far in the walk, so a string for a
`char *` member is lifted and one for a `char[N]` member stays. A
union's list goes to the member `InitList.union_member` names, or to
its first member. Where the type isn't known (a member past the end of
a struct, or the members of a struct with no body), a string item stays
and the type checker reports what is wrong with it. A list for any other
type, such as a `const struct`, is walked as an expression, which lifts
every string in it.

A list with designators is first rewritten into positional form by
`designators::normalize_initializer`, with the member lists seen so far,
so no pass after this one sees a `Designation`. The pass runs after
identifier_resolution, so the names it makes aren't renamed, and before
the label passes and the type checker, which see the rewritten program.

Python builds a new program and leaves its input alone. This one
rewrites the program it is given, which it takes by value.
*/
#include "passes/string_lifting.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "passes/designators.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::c99_ast;

using Exp = ir::Type_exp;
using Type = ir::Type_data_type;
using designators::Members;

// Returns `name` as the program spelled it. identifier_resolution
// renames a block-scope object `x` to `@N.x`.
std::string display_name(const std::string& name) {
  if (!name.empty() && name[0] == '@') {
    const std::size_t dot = name.find('.');
    if (dot != std::string::npos) {
      return name.substr(dot + 1);
    }
  }
  return name;
}

// Returns whether `t` is an array of `char`, `signed char`, or `unsigned
// char`, with any qualifiers on the element. A null `t` is Python's None.
bool is_char_array(const Type* t) {
  if (t == nullptr) {
    return false;
  }
  const auto* a = std::get_if<ir::Array>(t);
  if (a == nullptr) {
    return false;
  }
  const Type* elem = &*a->element_type;
  for (;;) {
    if (const auto* c = std::get_if<ir::Const>(elem)) {
      elem = &*c->referenced_type;
    } else if (const auto* v = std::get_if<ir::Volatile>(elem)) {
      elem = &*v->referenced_type;
    } else {
      break;
    }
  }
  return asdl::isa<ir::Char, ir::SChar, ir::UChar>(*elem);
}

class Lifter {
 public:
  ir::Program lift_program(ir::Program prog) {
    for (auto& decl : prog.declaration) {
      declaration(decl);
    }
    std::vector<ir::Type_declaration> out = std::move(lifted_);
    out.reserve(out.size() + prog.declaration.size());
    for (auto& decl : prog.declaration) {
      out.push_back(std::move(decl));
    }
    prog.declaration = std::move(out);
    return prog;
  }

 private:
  void declaration(ir::Type_declaration& decl) {
    switch (ir::kind(decl)) {
      case ir::Kind_declaration::VarDecl:
        var_decl(std::get<ir::VarDecl>(decl).var_decl);
        return;
      case ir::Kind_declaration::FunctionDecl:
        function_decl(*std::get<ir::FunctionDecl>(decl).function_decl);
        return;
      case ir::Kind_declaration::StructDecl: {
        // A struct or union declaration holds no expression. Its body
        // is recorded, so that a later braced list dispatches its items
        // by member type. A declaration without a body leaves the last
        // body of its tag in place.
        const auto& sd = std::get<ir::StructDecl>(decl).struct_decl;
        if (!sd.members.empty()) {
          Members members;
          for (const auto& m : sd.members) {
            members.emplace_back(m.name, m.data_type);
          }
          struct_members_[sd.tag] = std::move(members);
        }
        return;
      }
    }
  }

  // Lifts the strings in a declaration's initializer, after rewriting a
  // list with designators into positional form, so that each item lines
  // up with the subobject it initializes.
  void var_decl(ir::Type_var_decl& vd) {
    if (!vd.init) {
      return;
    }
    normalize(*vd.init, vd.data_type, display_name(vd.name));
    init(*vd.init, &vd.data_type);
  }

  // Rewrites `e`, the initializer of an object of type `t`, into
  // positional form.
  void normalize(Exp& e, const Type& t, const std::optional<std::string>& what) {
    if (!std::holds_alternative<ir::InitList>(e) || !designators::has_designation(e)) {
      return;
    }
    const auto layout_of = [this](std::string_view tag) -> std::optional<Members> {
      auto it = struct_members_.find(std::string(tag));
      if (it == struct_members_.end()) {
        return std::nullopt;
      }
      return it->second;
    };
    e = designators::normalize_initializer(std::move(e), t, layout_of, what);
  }

  // Lifts the strings of `e`, the initializer of a subobject of type
  // `declared`, or of unknown type when `declared` is null.
  void init(Exp& e, const Type* declared) {
    if (std::holds_alternative<ir::String>(e) &&
        (declared == nullptr || is_char_array(declared))) {
      // A direct initializer of a character array. Inside a braced
      // list of unknown type the item is taken to initialize a
      // character array member, which the type checker checks.
      return;
    }
    auto* list = std::get_if<ir::InitList>(&e);
    if (list == nullptr) {
      exp(e);
      return;
    }
    if (is_char_array(declared) && list->items.size() == 1 &&
        std::holds_alternative<ir::String>(list->items[0])) {
      // `char s[] = {"abc"};` is `char s[] = "abc";` (C99 §6.7.8p14).
      Exp only = std::move(list->items[0]);
      e = std::move(only);
      return;
    }
    if (declared != nullptr) {
      if (const auto* a = std::get_if<ir::Array>(declared)) {
        // Each item initializes an element. A nested list goes to the
        // element type, so a string at the deepest character array
        // stays in place.
        const Type& elem = *a->element_type;
        for (auto& item : list->items) {
          init(item, &elem);
        }
        return;
      }
      if (asdl::isa<ir::Structure, ir::Union>(*declared)) {
        struct_list(*list, *declared);
        return;
      }
      exp(e);
      return;
    }
    // A list inside a struct's list whose member type isn't known.
    // Its strings stay, and everything else is lifted.
    for (auto& item : list->items) {
      if (std::holds_alternative<ir::String>(item)) {
        continue;
      }
      if (std::holds_alternative<ir::InitList>(item)) {
        init(item, nullptr);
      } else {
        exp(item);
      }
    }
  }

  // Lifts the strings of the list of a struct or union of type
  // `declared`, each item by the type of the member it initializes. A
  // union's list initializes the member a designator chose, or its
  // first member (C99 §6.7.8p16). A tag with no body leaves every
  // member's type unknown, and the type checker rejects the list.
  void struct_list(ir::InitList& list, const Type& declared) {
    const std::string& tag = std::holds_alternative<ir::Structure>(declared)
                                 ? std::get<ir::Structure>(declared).tag
                                 : std::get<ir::Union>(declared).tag;
    auto it = struct_members_.find(tag);
    const Members* members = it == struct_members_.end() ? nullptr : &it->second;
    Members chosen;
    if (members != nullptr && std::holds_alternative<ir::Union>(declared)) {
      for (const auto& m : *members) {
        if (list.union_member && m.first == *list.union_member) {
          chosen.push_back(m);
        }
      }
      if (chosen.empty()) {
        chosen.push_back(members->front());
      }
      members = &chosen;
    }
    for (std::size_t i = 0; i < list.items.size(); ++i) {
      const Type* m_type =
          members != nullptr && i < members->size() ? &(*members)[i].second : nullptr;
      init(list.items[i], m_type);
    }
  }

  void function_decl(ir::Type_function_decl& fd) {
    if (fd.body) {
      block(**fd.body);
    }
  }

  void block(ir::Type_block& b) {
    for (auto& item : std::get<ir::Block>(b).block_item) {
      if (auto* s = std::get_if<ir::S>(&item)) {
        statement(*s->statement);
      } else {
        declaration(*std::get<ir::D>(item).declaration);
      }
    }
  }

  void statement(ir::Type_statement& stmt) {
    switch (ir::kind(stmt)) {
      case ir::Kind_statement::Return: {
        auto& r = std::get<ir::Return>(stmt);
        if (r.exp) {
          exp(*r.exp);
        }
        return;
      }
      case ir::Kind_statement::Expression:
        exp(std::get<ir::Expression>(stmt).exp);
        return;
      case ir::Kind_statement::IfStmt: {
        auto& i = std::get<ir::IfStmt>(stmt);
        exp(i.condition);
        statement(*i.then_clause);
        if (i.else_clause) {
          statement(**i.else_clause);
        }
        return;
      }
      case ir::Kind_statement::Compound:
        block(*std::get<ir::Compound>(stmt).block);
        return;
      case ir::Kind_statement::WhileStmt: {
        auto& w = std::get<ir::WhileStmt>(stmt);
        exp(w.condition);
        statement(*w.body);
        return;
      }
      case ir::Kind_statement::DoWhileStmt: {
        // Python builds the node with its body first.
        auto& d = std::get<ir::DoWhileStmt>(stmt);
        statement(*d.body);
        exp(d.condition);
        return;
      }
      case ir::Kind_statement::ForStmt: {
        auto& f = std::get<ir::ForStmt>(stmt);
        if (auto* decls = std::get_if<ir::InitDecl>(&f.init)) {
          for (auto& vd : decls->var_decls) {
            var_decl(vd);
          }
        } else {
          auto& ie = std::get<ir::InitExp>(f.init);
          if (ie.exp) {
            exp(*ie.exp);
          }
        }
        if (f.condition) {
          exp(*f.condition);
        }
        if (f.post_clause) {
          exp(*f.post_clause);
        }
        statement(*f.body);
        return;
      }
      case ir::Kind_statement::SwitchStmt: {
        auto& s = std::get<ir::SwitchStmt>(stmt);
        exp(s.control);
        statement(*s.body);
        return;
      }
      case ir::Kind_statement::CaseStmt: {
        // A case value is a constant expression, where a string isn't
        // allowed (§6.6). It is walked anyway, and the type checker
        // rejects the `Var` a string leaves.
        auto& c = std::get<ir::CaseStmt>(stmt);
        exp(c.value);
        statement(*c.body);
        return;
      }
      case ir::Kind_statement::DefaultStmt:
        statement(*std::get<ir::DefaultStmt>(stmt).body);
        return;
      case ir::Kind_statement::LabeledStmt:
        statement(*std::get<ir::LabeledStmt>(stmt).statement);
        return;
      case ir::Kind_statement::Goto:
      case ir::Kind_statement::BreakStmt:
      case ir::Kind_statement::ContinueStmt:
      case ir::Kind_statement::Null:
        return;
    }
  }

  // Replaces each `String` in the expression `e` with a `Var` naming a
  // new file-scope static. The operands of each node are walked in the
  // order of the Python constructor's keyword arguments.
  void exp(Exp& e) {
    switch (ir::kind(e)) {
      case ir::Kind_exp::String: {
        std::string s = std::move(std::get<ir::String>(e).str);
        e = mint(std::move(s));
        return;
      }
      case ir::Kind_exp::Constant:
      case ir::Kind_exp::Var:
      case ir::Kind_exp::SizeOfType:
        return;
      case ir::Kind_exp::Cast:
        exp(*std::get<ir::Cast>(e).exp);
        return;
      case ir::Kind_exp::Unary:
        exp(*std::get<ir::Unary>(e).exp);
        return;
      case ir::Kind_exp::Binary: {
        auto& b = std::get<ir::Binary>(e);
        exp(*b.left);
        exp(*b.right);
        return;
      }
      case ir::Kind_exp::Assignment: {
        auto& a = std::get<ir::Assignment>(e);
        exp(*a.lval);
        exp(*a.rval);
        return;
      }
      case ir::Kind_exp::CompoundAssignment: {
        auto& a = std::get<ir::CompoundAssignment>(e);
        exp(*a.lval);
        exp(*a.rval);
        return;
      }
      case ir::Kind_exp::Postfix:
        exp(*std::get<ir::Postfix>(e).operand);
        return;
      case ir::Kind_exp::Prefix:
        exp(*std::get<ir::Prefix>(e).operand);
        return;
      case ir::Kind_exp::Conditional: {
        auto& c = std::get<ir::Conditional>(e);
        exp(*c.condition);
        exp(*c.true_clause);
        exp(*c.false_clause);
        return;
      }
      case ir::Kind_exp::Comma: {
        auto& c = std::get<ir::Comma>(e);
        exp(*c.left);
        exp(*c.right);
        return;
      }
      case ir::Kind_exp::FunctionCall:
        for (auto& a : std::get<ir::FunctionCall>(e).args) {
          exp(a);
        }
        return;
      case ir::Kind_exp::IndirectCall: {
        auto& c = std::get<ir::IndirectCall>(e);
        exp(*c.callee);
        for (auto& a : c.args) {
          exp(a);
        }
        return;
      }
      case ir::Kind_exp::Dereference:
        exp(*std::get<ir::Dereference>(e).exp);
        return;
      case ir::Kind_exp::AddressOf:
        exp(*std::get<ir::AddressOf>(e).exp);
        return;
      case ir::Kind_exp::Subscript: {
        auto& s = std::get<ir::Subscript>(e);
        exp(*s.array);
        exp(*s.index);
        return;
      }
      case ir::Kind_exp::InitList:
        for (auto& item : std::get<ir::InitList>(e).items) {
          exp(item);
        }
        return;
      case ir::Kind_exp::Designation:
        // Only a list of a type the pass couldn't work out keeps a
        // designation, and the type checker rejects that list.
        exp(*std::get<ir::Designation>(e).init);
        return;
      case ir::Kind_exp::SizeOfExp:
        // The operand isn't evaluated, but a string in it is lifted
        // like any other. The `Var` has the `char[N+1]` type the string
        // had, so the size comes out the same.
        exp(*std::get<ir::SizeOfExp>(e).exp);
        return;
      case ir::Kind_exp::CompoundLiteral: {
        // The initializer lifts as a declaration's of the literal's
        // type would, so `(char[]){"abc"}` keeps its string.
        auto& c = std::get<ir::CompoundLiteral>(e);
        normalize(*c.init, c.target_type, std::nullopt);
        init(*c.init, &c.target_type);
        return;
      }
      case ir::Kind_exp::Dot:
        exp(*std::get<ir::Dot>(e).operand);
        return;
      case ir::Kind_exp::Arrow:
        exp(*std::get<ir::Arrow>(e).operand);
        return;
    }
  }

  // Returns a `Var` naming a new declaration `static char str@N[len+1]
  // = s;`, which goes in front of the program's declarations. The
  // element type is plain `char`, since the only reads are byte loads
  // through the `char *` the array decays to. The length counts code
  // points, which the C++ string holds as UTF-8.
  Exp mint(std::string s) {
    std::string name = "str@" + std::to_string(counter_++);
    const auto size = static_cast<long long>(pyrepr::code_points(s).size()) + 1;
    lifted_.push_back(ir::VarDecl{.var_decl = ir::Type_var_decl{
                                      .name = name,
                                      .init = Exp(ir::String{.str = std::move(s)}),
                                      .data_type = ir::Array{.element_type = ir::Char{},
                                                             .size = size},
                                      .storage_class = ir::Static{},
                                  }});
    return ir::Var{.name = std::move(name)};
  }

  int counter_ = 0;
  std::vector<ir::Type_declaration> lifted_;
  // Each struct or union tag's members as `(name, type)` pairs, from
  // the last body of the tag the walk has passed. A list can only
  // initialize a complete type, so its body comes earlier in the walk.
  std::unordered_map<std::string, Members> struct_members_;
};

}  // namespace

c99_ast::Program lift_strings(c99_ast::Program prog) {
  return Lifter().lift_program(std::move(prog));
}

}  // namespace c6502::passes
