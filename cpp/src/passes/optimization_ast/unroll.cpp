/*
Loop unrolling for `[[c6502::unroll]]`.

It runs under -O before identifier_resolution, and replaces every for
loop whose `loop_attribute` is "unroll" with a compound of N copies of
the body, one after another and each in a scope of its own, with the
induction variable replaced in each copy by the integer constant it
holds in that pass.

Only the canonical shape is taken, and anything else is an
`UnrollError`, since the program asked for the loop to be unrolled.

  init  `T i = <integer constant>;` with T one of int, unsigned int,
        long, unsigned long, long long, unsigned long long, char,
        signed char, unsigned char, short, and unsigned short
  cond  `i <op> <integer bound>` with op one of <, <=, >, >=
  post  `i++`, `i--`, `++i`, `--i`, `i += K`, or `i -= K`, with K an
        integer bound above 0
  body  no break, continue, goto, or labeled statement, no assignment,
        increment, decrement, or address of the induction variable, and
        no inner declaration that shadows it

An integer bound is an integer constant, or a subscript of a file-scope
`static const` array of integers, of one dimension or several, whose
every index is an integer bound itself. So a loop's bound may come from
a constant table indexed by the induction variable of an enclosing
unrolled loop, which that loop has replaced with a constant:

    static const uint8_t COUNTS[7] = {7, 1, 7, 1, 7, 1, 7};
    [[c6502::unroll]] for (uint8_t b = 0; b < 7; b++)
        [[c6502::unroll]] for (uint8_t i = 0; i < COUNTS[b]; i++)  // folds
            ...

The number of passes is capped at kMaxUnrollIterations.

The passes are counted as C runs the loop. The induction variable holds
a value of its own type, so the init and each step wrap at its width and
signedness, and the condition compares it with the bound after the
integer promotions and the usual arithmetic conversions. A loop whose
variable comes back to a value it held before never ends, as `for
(uint8_t i = 0; i <= 255; i++)` doesn't, and is an UnrollError.

The pass works on the c99 program because identifier_resolution and
loop_labeling run after it, so each copy's locals get names of their own
per pass with no more work here, and the constant that replaces the
induction variable goes through the type checker, the TAC, and the asm
passes as any constant the program writes does, with no extra folding.
*/
#include "passes/optimization_ast/unroll.h"

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "errors.h"
#include "passes/type_checking_types.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::c99_ast;
namespace tc = ::c6502::passes::type_checking;

using Type = ir::Type_data_type;
using Int = asdl::Int;

[[noreturn]] void fail(const std::string& message) {
  throw CompileError("UnrollError", message);
}

std::string int_text(Int v) {
  // The values in messages are the cap and small counts, which fit.
  return std::to_string(static_cast<long long>(v));
}

// Returns whether `t` is a type the induction variable may have.
bool is_iv_type(const Type& t) {
  switch (ir::kind(t)) {
    case ir::Kind_data_type::Int:
    case ir::Kind_data_type::UInt:
    case ir::Kind_data_type::Long:
    case ir::Kind_data_type::ULong:
    case ir::Kind_data_type::LongLong:
    case ir::Kind_data_type::ULongLong:
    case ir::Kind_data_type::Char:
    case ir::Kind_data_type::SChar:
    case ir::Kind_data_type::UChar:
    case ir::Kind_data_type::Short:
    case ir::Kind_data_type::UShort:
      return true;
    default:
      return false;
  }
}

// Returns the constant that replaces an induction variable of type `t`
// holding `value`. The char types follow the type checker: c6502's plain
// `char` is unsigned, so `char` and `unsigned char` both give a
// ConstUChar, and only `signed char` a ConstChar. `short` and `unsigned
// short` give the constants of `int` and `unsigned int`.
ir::Type_const iv_constant(const Type& t, Int value) {
  switch (ir::kind(t)) {
    case ir::Kind_data_type::Int:
    case ir::Kind_data_type::Short:
      return ir::ConstInt{.value = value};
    case ir::Kind_data_type::UInt:
    case ir::Kind_data_type::UShort:
      return ir::ConstUInt{.value = value};
    case ir::Kind_data_type::Long:
      return ir::ConstLong{.value = value};
    case ir::Kind_data_type::ULong:
      return ir::ConstULong{.value = value};
    case ir::Kind_data_type::LongLong:
      return ir::ConstLongLong{.value = value};
    case ir::Kind_data_type::ULongLong:
      return ir::ConstULongLong{.value = value};
    case ir::Kind_data_type::Char:
    case ir::Kind_data_type::UChar:
      return ir::ConstUChar{.value = value};
    case ir::Kind_data_type::SChar:
      return ir::ConstChar{.value = value};
    default:
      throw InternalError("unroll: not an induction variable type");
  }
}

// Returns how a message spells each induction variable's type.
const char* c_type_name(const Type& t) {
  switch (ir::kind(t)) {
    case ir::Kind_data_type::Int:
      return "int";
    case ir::Kind_data_type::UInt:
      return "unsigned int";
    case ir::Kind_data_type::Long:
      return "long";
    case ir::Kind_data_type::ULong:
      return "unsigned long";
    case ir::Kind_data_type::LongLong:
      return "long long";
    case ir::Kind_data_type::ULongLong:
      return "unsigned long long";
    case ir::Kind_data_type::Char:
      return "char";
    case ir::Kind_data_type::SChar:
      return "signed char";
    case ir::Kind_data_type::UChar:
      return "unsigned char";
    case ir::Kind_data_type::Short:
      return "short";
    case ir::Kind_data_type::UShort:
      return "unsigned short";
    default:
      throw InternalError("unroll: not an induction variable type");
  }
}

// Returns the value of an integer constant of one of the kinds an
// integer bound may be, with its type, or none for the other kinds.
// `true` and `false` are `bool`, but a bound or a step uses them
// promoted to `int` (C23 §6.3.1.1), and they hold 0 or 1.
std::optional<std::pair<Int, Type>> integer_constant(const ir::Type_const& c) {
  switch (ir::kind(c)) {
    case ir::Kind_const::ConstInt:
      return std::pair<Int, Type>{std::get<ir::ConstInt>(c).value, ir::Int{}};
    case ir::Kind_const::ConstUInt:
      return std::pair<Int, Type>{std::get<ir::ConstUInt>(c).value, ir::UInt{}};
    case ir::Kind_const::ConstLong:
      return std::pair<Int, Type>{std::get<ir::ConstLong>(c).value, ir::Long{}};
    case ir::Kind_const::ConstULong:
      return std::pair<Int, Type>{std::get<ir::ConstULong>(c).value, ir::ULong{}};
    case ir::Kind_const::ConstLongLong:
      return std::pair<Int, Type>{std::get<ir::ConstLongLong>(c).value, ir::LongLong{}};
    case ir::Kind_const::ConstULongLong:
      return std::pair<Int, Type>{std::get<ir::ConstULongLong>(c).value, ir::ULongLong{}};
    case ir::Kind_const::ConstChar:
      return std::pair<Int, Type>{std::get<ir::ConstChar>(c).value, ir::SChar{}};
    case ir::Kind_const::ConstUChar:
      return std::pair<Int, Type>{std::get<ir::ConstUChar>(c).value, ir::UChar{}};
    case ir::Kind_const::ConstBool:
      return std::pair<Int, Type>{std::get<ir::ConstBool>(c).value, ir::Int{}};
    default:
      return std::nullopt;
  }
}

// Returns `exp` without the casts around it.
const ir::Type_exp& strip_casts(const ir::Type_exp& exp) {
  const ir::Type_exp* e = &exp;
  while (const auto* c = std::get_if<ir::Cast>(e)) {
    e = &*c->exp;
  }
  return *e;
}

// Returns the value of an integer constant, under any casts, without
// the subscripts of `const_int_value`, for the initializers of the
// constant arrays, which are read before the arrays are known.
// Subscripts in those initializers, which would need another array's
// value, aren't folded.
std::optional<Int> scalar_const_int(const ir::Type_exp* exp) {
  if (exp == nullptr) {
    return std::nullopt;
  }
  const auto* c = std::get_if<ir::Constant>(&strip_casts(*exp));
  if (c == nullptr) {
    return std::nullopt;
  }
  auto v = integer_constant(c->const_);
  if (!v) {
    return std::nullopt;
  }
  return v->first;
}

bool is_var(const ir::Type_exp& exp, const std::string& name) {
  const auto* v = std::get_if<ir::Var>(&exp);
  return v != nullptr && v->name == name;
}

// A constant array's values: a leaf with an integer, or one level of
// the array with a tree per element. Each level holds as many elements
// as its declared size, or more when the initializer has more. Padding
// shares one zero tree.
struct ValueNode {
  bool leaf = false;
  Int value = 0;
  std::vector<std::shared_ptr<const ValueNode>> items;
};

using ValueTree = std::shared_ptr<const ValueNode>;

ValueTree zero_tree(const std::vector<Int>& sizes, std::size_t level) {
  auto node = std::make_shared<ValueNode>();
  if (level == sizes.size()) {
    node->leaf = true;
    return node;
  }
  ValueTree inner = zero_tree(sizes, level + 1);
  for (Int i = 0; i < sizes[level]; ++i) {
    node->items.push_back(inner);
  }
  return node;
}

// Returns the values of an initializer for an array of the dimensions
// `sizes` from `level` on, with each level padded with zeros to its
// declared size, or null when an item isn't an integer constant.
ValueTree init_to_value_tree(const ir::Type_exp* init, const std::vector<Int>& sizes,
                             std::size_t level) {
  if (level == sizes.size()) {
    auto v = scalar_const_int(init);
    if (!v) {
      return nullptr;
    }
    auto node = std::make_shared<ValueNode>();
    node->leaf = true;
    node->value = *v;
    return node;
  }
  if (init == nullptr) {
    return zero_tree(sizes, level);
  }
  const auto* list = std::get_if<ir::InitList>(init);
  if (list == nullptr) {
    return nullptr;
  }
  auto node = std::make_shared<ValueNode>();
  for (const auto& sub : list->items) {
    ValueTree v = init_to_value_tree(&sub, sizes, level + 1);
    if (!v) {
      return nullptr;
    }
    node->items.push_back(std::move(v));
  }
  ValueTree pad = zero_tree(sizes, level + 1);
  while (static_cast<Int>(node->items.size()) < sizes[level]) {
    node->items.push_back(pad);
  }
  return node;
}

// Returns the dimensions of `t`, outermost first, when it is an array,
// of one dimension or several, whose element is a `const` integer
// scalar, and none otherwise. A volatile element is refused, since the
// unroller reads the array at compile time, which would drop the
// accesses' side effects.
std::optional<std::vector<Int>> const_array_sizes(const Type& type) {
  std::vector<Int> sizes;
  const Type* t = &type;
  while (const auto* a = std::get_if<ir::Array>(t)) {
    sizes.push_back(a->size);
    t = &*a->element_type;
  }
  if (sizes.empty()) {
    return std::nullopt;
  }
  bool has_const = false;
  for (;;) {
    if (std::holds_alternative<ir::Volatile>(*t)) {
      return std::nullopt;
    }
    const auto* c = std::get_if<ir::Const>(t);
    if (c == nullptr) {
      break;
    }
    has_const = true;
    t = &*c->referenced_type;
  }
  if (!has_const) {
    return std::nullopt;
  }
  switch (ir::kind(*t)) {
    case ir::Kind_data_type::Int:
    case ir::Kind_data_type::UInt:
    case ir::Kind_data_type::Long:
    case ir::Kind_data_type::ULong:
    case ir::Kind_data_type::LongLong:
    case ir::Kind_data_type::ULongLong:
    case ir::Kind_data_type::Char:
    case ir::Kind_data_type::SChar:
    case ir::Kind_data_type::UChar:
      return sizes;
    default:
      return std::nullopt;
  }
}

// Returns the element type under every array level and qualifier of
// `t`.
const Type& array_leaf_type(const Type& type) {
  const Type* t = &type;
  for (;;) {
    if (const auto* a = std::get_if<ir::Array>(t)) {
      t = &*a->element_type;
    } else if (const auto* c = std::get_if<ir::Const>(t)) {
      t = &*c->referenced_type;
    } else if (const auto* v = std::get_if<ir::Volatile>(t)) {
      t = &*v->referenced_type;
    } else {
      return *t;
    }
  }
}

// A walk over a statement: each node before its fields, and the fields
// in the `.asdl`'s order. The walk calls the visitor's `stmt` on each
// statement, `decl` on each declaration, `for_init` on each for init,
// and `exp` on each expression, which returns whether to walk the
// expression's operands. Types hold no expression, so the walk doesn't
// enter them.
template <class V>
class Walker {
 public:
  explicit Walker(V& v) : v_(v) {}

  void stmt(ir::Type_statement& s) {
    v_.stmt(s);
    switch (ir::kind(s)) {
      case ir::Kind_statement::Return: {
        auto& r = std::get<ir::Return>(s);
        if (r.exp) {
          exp(*r.exp);
        }
        return;
      }
      case ir::Kind_statement::Expression:
        exp(std::get<ir::Expression>(s).exp);
        return;
      case ir::Kind_statement::IfStmt: {
        auto& i = std::get<ir::IfStmt>(s);
        exp(i.condition);
        stmt(*i.then_clause);
        if (i.else_clause) {
          stmt(**i.else_clause);
        }
        return;
      }
      case ir::Kind_statement::Compound:
        block(*std::get<ir::Compound>(s).block);
        return;
      case ir::Kind_statement::WhileStmt: {
        auto& w = std::get<ir::WhileStmt>(s);
        exp(w.condition);
        stmt(*w.body);
        return;
      }
      case ir::Kind_statement::DoWhileStmt: {
        auto& d = std::get<ir::DoWhileStmt>(s);
        stmt(*d.body);
        exp(d.condition);
        return;
      }
      case ir::Kind_statement::ForStmt: {
        auto& f = std::get<ir::ForStmt>(s);
        for_init(f.init);
        if (f.condition) {
          exp(*f.condition);
        }
        if (f.post_clause) {
          exp(*f.post_clause);
        }
        stmt(*f.body);
        return;
      }
      case ir::Kind_statement::SwitchStmt: {
        auto& sw = std::get<ir::SwitchStmt>(s);
        exp(sw.control);
        stmt(*sw.body);
        for (auto& c : sw.cases) {
          exp(c.value);
        }
        return;
      }
      case ir::Kind_statement::CaseStmt: {
        auto& c = std::get<ir::CaseStmt>(s);
        exp(c.value);
        stmt(*c.body);
        return;
      }
      case ir::Kind_statement::DefaultStmt:
        stmt(*std::get<ir::DefaultStmt>(s).body);
        return;
      case ir::Kind_statement::LabeledStmt:
        stmt(*std::get<ir::LabeledStmt>(s).statement);
        return;
      case ir::Kind_statement::BreakStmt:
      case ir::Kind_statement::ContinueStmt:
      case ir::Kind_statement::Goto:
      case ir::Kind_statement::Null:
        return;
    }
  }

  void block(ir::Type_block& b) {
    for (auto& item : std::get<ir::Block>(b).block_item) {
      switch (ir::kind(item)) {
        case ir::Kind_block_item::S:
          stmt(*std::get<ir::S>(item).statement);
          break;
        case ir::Kind_block_item::D:
          decl(*std::get<ir::D>(item).declaration);
          break;
      }
    }
  }

  void decl(ir::Type_declaration& d) {
    v_.decl(d);
    switch (ir::kind(d)) {
      case ir::Kind_declaration::FunctionDecl: {
        auto& fd = *std::get<ir::FunctionDecl>(d).function_decl;
        if (fd.body) {
          block(**fd.body);
        }
        return;
      }
      case ir::Kind_declaration::VarDecl:
        var_decl(std::get<ir::VarDecl>(d).var_decl);
        return;
      case ir::Kind_declaration::StructDecl:
        return;
    }
  }

  void var_decl(ir::Type_var_decl& vd) {
    if (vd.init) {
      exp(*vd.init);
    }
  }

  void for_init(ir::Type_for_init& init) {
    v_.for_init(init);
    switch (ir::kind(init)) {
      case ir::Kind_for_init::InitDecl:
        for (auto& vd : std::get<ir::InitDecl>(init).var_decls) {
          var_decl(vd);
        }
        return;
      case ir::Kind_for_init::InitExp: {
        auto& e = std::get<ir::InitExp>(init);
        if (e.exp) {
          exp(*e.exp);
        }
        return;
      }
    }
  }

  void exp(ir::Type_exp& e) {
    if (!v_.exp(e)) {
      return;
    }
    switch (ir::kind(e)) {
      case ir::Kind_exp::Constant:
      case ir::Kind_exp::String:
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
      case ir::Kind_exp::Dot:
        exp(*std::get<ir::Dot>(e).operand);
        return;
      case ir::Kind_exp::Arrow:
        exp(*std::get<ir::Arrow>(e).operand);
        return;
      case ir::Kind_exp::Comma: {
        auto& c = std::get<ir::Comma>(e);
        exp(*c.left);
        exp(*c.right);
        return;
      }
      case ir::Kind_exp::CompoundLiteral:
        exp(*std::get<ir::CompoundLiteral>(e).init);
        return;
    }
  }

 private:
  V& v_;
};

// Rejects the shapes of a body that the unroller can't copy faithfully,
// at the first node of the walk that has one.
struct BodyValidator {
  const std::string& iv;

  void stmt(const ir::Type_statement& s) const {
    switch (ir::kind(s)) {
      case ir::Kind_statement::BreakStmt:
        fail("unroll: `break` inside an unrolled loop body is not supported");
      case ir::Kind_statement::ContinueStmt:
        fail("unroll: `continue` inside an unrolled loop body is not supported");
      case ir::Kind_statement::Goto:
        fail("unroll: `goto` inside an unrolled loop body is not supported");
      case ir::Kind_statement::LabeledStmt:
        fail("unroll: labeled statements inside an unrolled loop body are not supported");
      default:
        return;
    }
  }

  void decl(const ir::Type_declaration& d) const {
    const auto* vd = std::get_if<ir::VarDecl>(&d);
    if (vd != nullptr && vd->var_decl.name == iv) {
      fail("unroll: induction variable `" + iv + "` is shadowed by an inner declaration");
    }
  }

  void for_init(const ir::Type_for_init& init) const {
    const auto* d = std::get_if<ir::InitDecl>(&init);
    if (d == nullptr) {
      return;
    }
    for (const auto& vd : d->var_decls) {
      if (vd.name == iv) {
        // An inner for loop's init declares the same name.
        fail("unroll: induction variable `" + iv + "` is shadowed by an inner for-loop init");
      }
    }
  }

  bool exp(const ir::Type_exp& e) const {
    switch (ir::kind(e)) {
      case ir::Kind_exp::AddressOf:
        if (is_var(*std::get<ir::AddressOf>(e).exp, iv)) {
          fail("unroll: address of induction variable `" + iv + "` is taken in body");
        }
        break;
      case ir::Kind_exp::Assignment:
        if (is_var(*std::get<ir::Assignment>(e).lval, iv)) {
          fail("unroll: induction variable `" + iv + "` is reassigned in body");
        }
        break;
      case ir::Kind_exp::CompoundAssignment:
        if (is_var(*std::get<ir::CompoundAssignment>(e).lval, iv)) {
          fail("unroll: induction variable `" + iv + "` is modified in body");
        }
        break;
      case ir::Kind_exp::Prefix:
        if (is_var(*std::get<ir::Prefix>(e).operand, iv)) {
          fail("unroll: induction variable `" + iv + "` is modified in body");
        }
        break;
      case ir::Kind_exp::Postfix:
        if (is_var(*std::get<ir::Postfix>(e).operand, iv)) {
          fail("unroll: induction variable `" + iv + "` is modified in body");
        }
        break;
      default:
        break;
    }
    return true;
  }
};

// Replaces every `Var(iv)` of a copied body with the constant of the
// pass, keeping the variable's `data_type`.
struct Substituter {
  const std::string& iv;
  const Type& iv_type;
  Int value;

  void stmt(const ir::Type_statement&) const {}
  void decl(const ir::Type_declaration&) const {}
  void for_init(const ir::Type_for_init&) const {}

  bool exp(ir::Type_exp& e) const {
    auto* v = std::get_if<ir::Var>(&e);
    if (v == nullptr || v->name != iv) {
      return true;
    }
    std::optional<Type> data_type = std::move(v->data_type);
    e = ir::Constant{.const_ = iv_constant(iv_type, value), .data_type = std::move(data_type)};
    return false;
  }
};

// The comparison of a loop's condition.
enum class Compare { kLess, kLessOrEqual, kGreater, kGreaterOrEqual };

bool holds(Compare op, Int i, Int b) {
  switch (op) {
    case Compare::kLess:
      return i < b;
    case Compare::kLessOrEqual:
      return i <= b;
    case Compare::kGreater:
      return i > b;
    case Compare::kGreaterOrEqual:
      return i >= b;
  }
  return false;
}

// The induction variable, as the init declares it.
struct InductionVariable {
  std::string name;
  Type type;
  Int init = 0;
};

// The condition's comparison and its bound, with the bound's type.
struct Condition {
  Compare op = Compare::kLess;
  Int bound = 0;
  Type bound_type;
};

class Unroller {
 public:
  ir::Program unroll_program(ir::Program prog) {
    build_const_array_map(prog);
    for (auto& d : prog.declaration) {
      auto* fd = std::get_if<ir::FunctionDecl>(&d);
      if (fd != nullptr && fd->function_decl->body) {
        for (auto& item : std::get<ir::Block>(**fd->function_decl->body).block_item) {
          unroll_block_item(item);
        }
      }
    }
    return prog;
  }

 private:
  // The values of each file-scope `static const` integer array, by its
  // source name, padded with zeros to the declared sizes, which
  // `const_int_value` reads to fold a subscript.
  std::unordered_map<std::string, ValueTree> arrays_;
  // The integer type of each array's elements, such as `unsigned char`
  // for `static const uint8_t T[4][2]`. A subscript bound has this
  // type, and its value wraps to it.
  std::unordered_map<std::string, Type> leaves_;

  // Records the values of each file-scope `static` array of `const`
  // integer scalars whose initializer folds. A declaration with an item
  // that doesn't fold is left out, and a subscript of it then doesn't
  // fold either.
  void build_const_array_map(const ir::Program& prog) {
    for (const auto& d : prog.declaration) {
      const auto* v = std::get_if<ir::VarDecl>(&d);
      if (v == nullptr) {
        continue;
      }
      const auto& vd = v->var_decl;
      if (!vd.storage_class || !std::holds_alternative<ir::Static>(*vd.storage_class)) {
        continue;
      }
      auto sizes = const_array_sizes(vd.data_type);
      if (!sizes || !vd.init) {
        continue;
      }
      ValueTree tree = init_to_value_tree(&*vd.init, *sizes, 0);
      if (tree) {
        arrays_[vd.name] = std::move(tree);
      }
    }
    // Every declaration of a recorded name gives its leaf type, the
    // last one winning.
    for (const auto& d : prog.declaration) {
      const auto* v = std::get_if<ir::VarDecl>(&d);
      if (v != nullptr && arrays_.count(v->var_decl.name) != 0) {
        leaves_.insert_or_assign(v->var_decl.name, array_leaf_type(v->var_decl.data_type));
      }
    }
  }

  void unroll_block_item(ir::Type_block_item& item) {
    if (auto* s = std::get_if<ir::S>(&item)) {
      unroll_statement(*s->statement);
    }
  }

  // Unrolls the marked for loops of a statement, and walks every other
  // statement so that a marked loop nested in it is unrolled too.
  void unroll_statement(ir::Type_statement& stmt) {
    switch (ir::kind(stmt)) {
      case ir::Kind_statement::ForStmt: {
        auto& f = std::get<ir::ForStmt>(stmt);
        if (f.loop_attribute && *f.loop_attribute == "unroll") {
          ir::Type_statement unrolled = unroll_for(f);
          stmt = std::move(unrolled);
        } else {
          unroll_statement(*f.body);
        }
        return;
      }
      case ir::Kind_statement::IfStmt: {
        auto& i = std::get<ir::IfStmt>(stmt);
        unroll_statement(*i.then_clause);
        if (i.else_clause) {
          unroll_statement(**i.else_clause);
        }
        return;
      }
      case ir::Kind_statement::Compound:
        for (auto& item : std::get<ir::Block>(*std::get<ir::Compound>(stmt).block).block_item) {
          unroll_block_item(item);
        }
        return;
      case ir::Kind_statement::WhileStmt:
        unroll_statement(*std::get<ir::WhileStmt>(stmt).body);
        return;
      case ir::Kind_statement::DoWhileStmt:
        unroll_statement(*std::get<ir::DoWhileStmt>(stmt).body);
        return;
      case ir::Kind_statement::SwitchStmt:
        unroll_statement(*std::get<ir::SwitchStmt>(stmt).body);
        return;
      case ir::Kind_statement::CaseStmt:
        unroll_statement(*std::get<ir::CaseStmt>(stmt).body);
        return;
      case ir::Kind_statement::DefaultStmt:
        unroll_statement(*std::get<ir::DefaultStmt>(stmt).body);
        return;
      case ir::Kind_statement::LabeledStmt:
        unroll_statement(*std::get<ir::LabeledStmt>(stmt).statement);
        return;
      default:
        return;
    }
  }

  // Returns the compound that replaces the marked loop `fs`, one copy of
  // the body per pass.
  ir::Type_statement unroll_for(ir::ForStmt& fs) {
    InductionVariable iv = validate_init(fs.init);
    Condition cond = validate_condition(fs.condition, iv.name);
    Int step = validate_post(fs.post_clause, iv.name);
    validate_body(*fs.body, iv.name);
    std::vector<Int> values = compute_iterations(iv, cond, step);

    ir::Block result;
    for (Int v : values) {
      ir::Type_statement clone = *fs.body;
      Substituter sub{iv.name, iv.type, v};
      Walker<Substituter>(sub).stmt(clone);
      // The copy may hold marked loops of its own, which unroll too.
      unroll_statement(clone);
      if (std::holds_alternative<ir::Compound>(clone)) {
        result.block_item.push_back(ir::S{.statement = std::move(clone)});
      } else {
        // A body of one statement (`for (...) x++;`) gets a compound,
        // so that each pass has a scope of its own, which a body that
        // declares locals needs.
        ir::Block one;
        one.block_item.push_back(ir::S{.statement = std::move(clone)});
        result.block_item.push_back(
            ir::S{.statement = ir::Compound{.block = std::move(one)}});
      }
    }
    return ir::Compound{.block = std::move(result)};
  }

  InductionVariable validate_init(const ir::Type_for_init& init) const {
    const auto* d = std::get_if<ir::InitDecl>(&init);
    if (d == nullptr) {
      fail(
          "unroll: for-init must declare the induction variable "
          "(`for (T i = <const>; ...; ...)`)");
    }
    if (d->var_decls.size() != 1) {
      fail(
          "unroll: for-init must declare exactly one induction "
          "variable (multi-declarator forms aren't supported)");
    }
    const auto& vd = d->var_decls[0];
    if (vd.storage_class && !std::holds_alternative<ir::Register>(*vd.storage_class)) {
      fail("unroll: induction variable cannot have a storage class");
    }
    if (!is_iv_type(vd.data_type)) {
      fail("unroll: induction variable type " + asdl::constructor_name(vd.data_type) +
           " not supported (use int / unsigned int / long / unsigned long / "
           "long long / unsigned long long)");
    }
    auto typed = typed_const_int(vd.init ? &*vd.init : nullptr);
    if (!typed) {
      fail("unroll: induction variable must be initialized to an integer constant");
    }
    // `T i = <const>` converts the constant to T.
    return InductionVariable{vd.name, vd.data_type,
                             tc::coerce_int_to_type(typed->first, vd.data_type)};
  }

  Condition validate_condition(const std::optional<ir::Type_exp>& cond,
                               const std::string& iv_name) const {
    if (!cond) {
      fail("unroll: for-loop condition is required");
    }
    const auto* b = std::get_if<ir::Binary>(&*cond);
    if (b == nullptr) {
      fail("unroll: condition must be `<iv> <op> <const>` with op in {<, <=, >, >=}");
    }
    Compare op;
    switch (ir::kind(b->op)) {
      case ir::Kind_binary_operator::LessThan:
        op = Compare::kLess;
        break;
      case ir::Kind_binary_operator::LessOrEqual:
        op = Compare::kLessOrEqual;
        break;
      case ir::Kind_binary_operator::GreaterThan:
        op = Compare::kGreater;
        break;
      case ir::Kind_binary_operator::GreaterOrEqual:
        op = Compare::kGreaterOrEqual;
        break;
      default:
        fail("unroll: comparison op " + asdl::constructor_name(b->op) +
             " not supported (use <, <=, >, or >=)");
    }
    if (!is_var(*b->left, iv_name)) {
      fail("unroll: condition's left operand must be the induction variable `" + iv_name + "`");
    }
    auto typed = typed_const_int(&*b->right);
    if (!typed) {
      fail("unroll: condition's right operand must be an integer constant");
    }
    return Condition{op, typed->first, std::move(typed->second)};
  }

  // Returns the step of the post clause.
  Int validate_post(const std::optional<ir::Type_exp>& post, const std::string& iv_name) const {
    if (!post) {
      fail("unroll: for-loop post-clause is required");
    }
    switch (ir::kind(*post)) {
      case ir::Kind_exp::Postfix: {
        const auto& p = std::get<ir::Postfix>(*post);
        if (is_var(*p.operand, iv_name)) {
          return std::holds_alternative<ir::Increment>(p.op) ? 1 : -1;
        }
        break;
      }
      case ir::Kind_exp::Prefix: {
        const auto& p = std::get<ir::Prefix>(*post);
        if (is_var(*p.operand, iv_name)) {
          return std::holds_alternative<ir::Increment>(p.op) ? 1 : -1;
        }
        break;
      }
      case ir::Kind_exp::CompoundAssignment: {
        const auto& a = std::get<ir::CompoundAssignment>(*post);
        if (!is_var(*a.lval, iv_name)) {
          break;
        }
        auto typed = typed_const_int(&*a.rval);
        if (!typed || typed->first <= 0) {
          fail("unroll: post-clause step must be a positive integer constant");
        }
        if (std::holds_alternative<ir::Add>(a.op)) {
          return typed->first;
        }
        if (std::holds_alternative<ir::Subtract>(a.op)) {
          return -typed->first;
        }
        fail("unroll: post-clause op " + asdl::constructor_name(a.op) +
             " not supported (use +=, -=, ++, or --)");
      }
      default:
        break;
    }
    fail(
        "unroll: post-clause must be `i++`, `i--`, `++i`, `--i`, "
        "`i += K`, or `i -= K`");
  }

  static void validate_body(ir::Type_statement& body, const std::string& iv_name) {
    BodyValidator v{iv_name};
    Walker<BodyValidator>(v).stmt(body);
  }

  // Simulates the loop and returns the induction variable's values, one
  // per pass. `iv.init` is already a value of the variable's type. Each
  // step wraps `i + step` to that type, as the conversion back to the
  // variable's type does in C. The condition compares `i` and the bound
  // in the common type of their promoted types (C99 §6.3.1.8), so `int
  // i < 40000u` compares as unsigned. The loop's whole state is `i`, so
  // a value seen a second time means the loop never ends: a `uint8_t i
  // <= 255` wraps from 255 to 0 and is rejected that way. A loop that
  // would end, but only after more than kMaxUnrollIterations passes, is
  // rejected by the cap.
  static std::vector<Int> compute_iterations(const InductionVariable& iv, const Condition& cond,
                                             Int step) {
    Type cmp_type =
        tc::common_type(tc::promote_integer(iv.type), tc::promote_integer(cond.bound_type));
    Int bound_cmp = tc::coerce_int_to_type(cond.bound, cmp_type);
    std::vector<Int> values;
    Int i = iv.init;
    while (holds(cond.op, tc::coerce_int_to_type(i, cmp_type), bound_cmp)) {
      for (Int seen : values) {
        if (seen == i) {
          fail("unroll: loop never terminates (`" + iv.name + "` wraps at the width of `" +
               c_type_name(iv.type) + "` and the condition stays true)");
        }
      }
      if (values.size() >= static_cast<std::size_t>(kMaxUnrollIterations)) {
        fail("unroll: iteration count exceeds cap of " + int_text(kMaxUnrollIterations));
      }
      values.push_back(i);
      i = tc::coerce_int_to_type(i + step, iv.type);
    }
    return values;
  }

  // Returns the value of an integer constant under any casts, or of a
  // subscript that folds, or none.
  std::optional<Int> const_int_value(const ir::Type_exp& exp) const {
    const ir::Type_exp& e = strip_casts(exp);
    if (const auto* c = std::get_if<ir::Constant>(&e)) {
      auto v = integer_constant(c->const_);
      if (v) {
        return v->first;
      }
      return std::nullopt;
    }
    if (const auto* s = std::get_if<ir::Subscript>(&e)) {
      return fold_subscript(*s);
    }
    return std::nullopt;
  }

  // Returns the value and the type of an integer bound, with the value
  // converted to the type: a constant has its kind's type, a cast to an
  // integer type converts its operand's value, and a subscript that
  // folds has its array's element type. Returns none for anything else,
  // and for a cast to a type that isn't an induction variable's.
  std::optional<std::pair<Int, Type>> typed_const_int(const ir::Type_exp* exp) const {
    if (exp == nullptr) {
      return std::nullopt;
    }
    if (const auto* c = std::get_if<ir::Cast>(exp)) {
      auto inner = typed_const_int(&*c->exp);
      if (!inner) {
        return std::nullopt;
      }
      const Type& t = tc::strip_quals(c->target_type);
      if (!is_iv_type(t)) {
        return std::nullopt;
      }
      return std::pair<Int, Type>{tc::coerce_int_to_type(inner->first, t), t};
    }
    if (const auto* c = std::get_if<ir::Constant>(exp)) {
      auto v = integer_constant(c->const_);
      if (!v) {
        return std::nullopt;
      }
      return std::pair<Int, Type>{tc::coerce_int_to_type(v->first, v->second), v->second};
    }
    if (const auto* s = std::get_if<ir::Subscript>(exp)) {
      auto v = fold_subscript(*s);
      if (!v) {
        return std::nullopt;
      }
      const ir::Type_exp* base = exp;
      while (const auto* inner = std::get_if<ir::Subscript>(base)) {
        base = &*inner->array;
      }
      const auto* var = std::get_if<ir::Var>(&strip_casts(*base));
      if (var == nullptr) {
        return std::nullopt;
      }
      auto it = leaves_.find(var->name);
      if (it == leaves_.end()) {
        return std::nullopt;
      }
      return std::pair<Int, Type>{tc::coerce_int_to_type(*v, it->second), it->second};
    }
    return std::nullopt;
  }

  // Folds a subscript, of one dimension or several, to its element's
  // value when the base is a known constant array and every index
  // folds. The outermost subscript holds the last index, so the walk
  // collects the indices last first.
  std::optional<Int> fold_subscript(const ir::Subscript& exp) const {
    std::vector<Int> indices;
    const ir::Type_exp* cur = nullptr;
    const ir::Subscript* s = &exp;
    for (;;) {
      auto idx = const_int_value(*s->index);
      if (!idx) {
        return std::nullopt;
      }
      indices.push_back(*idx);
      cur = &*s->array;
      s = std::get_if<ir::Subscript>(cur);
      if (s == nullptr) {
        break;
      }
    }
    const auto* var = std::get_if<ir::Var>(&strip_casts(*cur));
    if (var == nullptr) {
      return std::nullopt;
    }
    auto it = arrays_.find(var->name);
    if (it == arrays_.end()) {
      return std::nullopt;
    }
    const ValueNode* val = it->second.get();
    for (auto i = indices.rbegin(); i != indices.rend(); ++i) {
      if (val->leaf) {
        return std::nullopt;
      }
      if (*i < 0 || *i >= static_cast<Int>(val->items.size())) {
        return std::nullopt;
      }
      val = val->items[static_cast<std::size_t>(*i)].get();
    }
    if (!val->leaf) {
      return std::nullopt;
    }
    return val->value;
  }
};

}  // namespace

c99_ast::Program unroll_program(c99_ast::Program prog) {
  return Unroller().unroll_program(std::move(prog));
}

}  // namespace c6502::passes
