#include "passes/string_lifting.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "passes/designators.h"
#include "passes/type_checking_types.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::c99_ast;

using Exp = ir::Type_exp;
using Type = ir::Type_data_type;
using designators::Members;

using type_checking::display_name;

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

  void var_decl(ir::Type_var_decl& vd) {
    if (!vd.init) {
      return;
    }
    normalize(*vd.init, vd.data_type, display_name(vd.name));
    init(*vd.init, &vd.data_type);
  }

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

  void init(Exp& e, const Type* declared) {
    if (std::holds_alternative<ir::String>(e) &&
        (declared == nullptr || is_char_array(declared))) {
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
        exp(*std::get<ir::Designation>(e).init);
        return;
      case ir::Kind_exp::SizeOfExp:
        exp(*std::get<ir::SizeOfExp>(e).exp);
        return;
      case ir::Kind_exp::CompoundLiteral: {
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

  Exp mint(std::string s) {
    // The name has no leading `.`, since dasm treats a name that starts
    // with one as a local label of the function emitted before it.
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
  std::unordered_map<std::string, Members> struct_members_;
};

}  // namespace

c99_ast::Program lift_strings(c99_ast::Program prog) {
  return Lifter().lift_program(std::move(prog));
}

}  // namespace c6502::passes
