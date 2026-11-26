#include "passes/type_checker.h"

#include <algorithm>
#include <cctype>

#include "bit_precise.h"
#include "pyrepr.h"

namespace c6502::passes::type_checking {

namespace {

namespace ir = ::c6502::c99_ast;
using K = ir::Kind_data_type;
using E = ir::Kind_exp;

[[noreturn]] void fail(const std::string& message) { throw TypeCheckError(message); }

std::string class_name(const Type& t) {
  std::string r = asdl::py_repr(t);
  return r.substr(0, r.find('('));
}

std::string struct_keyword(const Type& t) {
  return asdl::isa<ir::Union>(t) ? "union" : "struct";
}

const std::string& tag_of(const Type& t) {
  if (const auto* s = std::get_if<ir::Structure>(&t)) {
    return s->tag;
  }
  return std::get<ir::Union>(t).tag;
}

std::string repr_tree(const ValueTree& v);

std::string tuple_str(const std::vector<ValueTree>& items) {
  std::string out = "(";
  for (std::size_t i = 0; i < items.size(); ++i) {
    if (i > 0) {
      out += ", ";
    }
    out += repr_tree(items[i]);
  }
  if (items.size() == 1) {
    out += ',';
  }
  out += ')';
  return out;
}

std::string repr_tree(const ValueTree& v) {
  using VK = ValueTree::Kind;
  switch (v.kind) {
    case VK::kInt:
      return asdl::int_to_string(v.integer);
    case VK::kTuple:
      return tuple_str(v.items);
    case VK::kAddress:
      return "AddressInit(name=" + pyrepr::string(v.address.name) +
             ", offset=" + asdl::int_to_string(v.address.offset) + ")";
    case VK::kUnion:
      return "UnionValue(member=" + asdl::int_to_string(v.member) +
             ", value=" + repr_tree(v.items.at(0)) + ")";
    default:
      throw InternalError("value_str: a value tree the type checker doesn't make");
  }
}

}  // namespace

std::string repr_or_none(const std::optional<Type>& t) {
  return t ? asdl::py_repr(*t) : std::string("None");
}

std::string c_type_or_none(const std::optional<Type>& t) {
  return t ? c_type(*t) : std::string("NoneType");
}

const std::optional<Type>& data_type_of(const Exp& exp) {
  static const std::optional<Type> kNone;
  switch (ir::kind(exp)) {
    case E::Constant:
      return std::get<ir::Constant>(exp).data_type;
    case E::Var:
      return std::get<ir::Var>(exp).data_type;
    case E::Cast:
      return std::get<ir::Cast>(exp).data_type;
    case E::Unary:
      return std::get<ir::Unary>(exp).data_type;
    case E::Binary:
      return std::get<ir::Binary>(exp).data_type;
    case E::Assignment:
      return std::get<ir::Assignment>(exp).data_type;
    case E::CompoundAssignment:
      return std::get<ir::CompoundAssignment>(exp).data_type;
    case E::Postfix:
      return std::get<ir::Postfix>(exp).data_type;
    case E::Prefix:
      return std::get<ir::Prefix>(exp).data_type;
    case E::Conditional:
      return std::get<ir::Conditional>(exp).data_type;
    case E::FunctionCall:
      return std::get<ir::FunctionCall>(exp).data_type;
    case E::IndirectCall:
      return std::get<ir::IndirectCall>(exp).data_type;
    case E::Dereference:
      return std::get<ir::Dereference>(exp).data_type;
    case E::AddressOf:
      return std::get<ir::AddressOf>(exp).data_type;
    case E::Subscript:
      return std::get<ir::Subscript>(exp).data_type;
    case E::InitList:
      return std::get<ir::InitList>(exp).data_type;
    case E::SizeOfExp:
      return std::get<ir::SizeOfExp>(exp).data_type;
    case E::SizeOfType:
      return std::get<ir::SizeOfType>(exp).data_type;
    case E::Dot:
      return std::get<ir::Dot>(exp).data_type;
    case E::Arrow:
      return std::get<ir::Arrow>(exp).data_type;
    case E::Comma:
      return std::get<ir::Comma>(exp).data_type;
    case E::CompoundLiteral:
      return std::get<ir::CompoundLiteral>(exp).data_type;
    case E::String:
    case E::Designation:
      return kNone;
  }
  return kNone;
}

std::optional<Type>* data_type_slot(Exp& exp) {
  if (asdl::isa<ir::String, ir::Designation>(exp)) {
    return nullptr;
  }
  return const_cast<std::optional<Type>*>(&data_type_of(exp));
}

bool is_stdbit_generic_type(const Type& t) {
  if (const auto* b = std::get_if<ir::BitInt>(&t)) {
    return !b->is_signed &&
           (b->width == 8 || b->width == 16 || b->width == 32 || b->width == 64);
  }
  return asdl::isa<ir::UChar, ir::UShort, ir::UInt, ir::ULong, ir::ULongLong>(t);
}

Type stdbit_parameter_type(std::string_view suffix) {
  if (suffix == "uc") {
    return ir::UChar{};
  }
  if (suffix == "us") {
    return ir::UShort{};
  }
  if (suffix == "ui") {
    return ir::UInt{};
  }
  if (suffix == "ul") {
    return ir::ULong{};
  }
  if (suffix == "ull") {
    return ir::ULongLong{};
  }
  throw InternalError("KeyError: " + pyrepr::string(suffix));
}

bool types_equal(const Type& a, const Type& b) { return a == b; }

std::string value_str(const ValueTree& value) { return repr_tree(value); }

InitialValue merge_initial_value(const std::string& name, const InitialValue& old,
                                 const InitialValue& next) {
  const auto* old_initial = std::get_if<Initial>(&old);
  const auto* new_initial = std::get_if<Initial>(&next);
  if (old_initial != nullptr && new_initial != nullptr) {
    fail("redefinition of object " + pyrepr::string(name) + ": prior initializer " +
         value_str(old_initial->value) + ", new initializer " + value_str(new_initial->value));
  }
  if (new_initial != nullptr) {
    return next;
  }
  if (old_initial != nullptr) {
    return old;
  }
  if (std::holds_alternative<Tentative>(old) || std::holds_alternative<Tentative>(next)) {
    return Tentative{};
  }
  return NoInitializer{};
}

bool is_object_type(const Type& t) {
  switch (ir::kind(strip_quals(t))) {
    case K::Int:
    case K::Long:
    case K::LongLong:
    case K::UInt:
    case K::ULong:
    case K::ULongLong:
    case K::Char:
    case K::SChar:
    case K::UChar:
    case K::Bool:
    case K::Short:
    case K::UShort:
    case K::BitInt:
    case K::Float:
    case K::Double:
    case K::Pointer:
    case K::NullptrT:
      return true;
    default:
      return false;
  }
}

bool is_complete_object_type(const Type& t) {
  return is_object_type(t) || asdl::isa<ir::Array, ir::Structure, ir::Union>(strip_quals(t));
}

bool is_arithmetic_type(const Type& t) { return is_integer_type(t) || is_floating_type(t); }

bool is_truth_op(const ir::Type_binary_operator& op) {
  using O = ir::Kind_binary_operator;
  switch (ir::kind(op)) {
    case O::Equal:
    case O::NotEqual:
    case O::LessThan:
    case O::GreaterThan:
    case O::LessOrEqual:
    case O::GreaterOrEqual:
    case O::LogicalAnd:
    case O::LogicalOr:
      return true;
    default:
      return false;
  }
}

std::string_view binary_op_spelling(const ir::Type_binary_operator& op) {
  using O = ir::Kind_binary_operator;
  switch (ir::kind(op)) {
    case O::Add:
      return "+";
    case O::Subtract:
      return "-";
    case O::Multiply:
      return "*";
    case O::Divide:
      return "/";
    case O::Modulo:
      return "%";
    case O::BitwiseAnd:
      return "&";
    case O::BitwiseOr:
      return "|";
    case O::BitwiseXor:
      return "^";
    case O::LeftShift:
      return "<<";
    case O::RightShift:
      return ">>";
    case O::Equal:
      return "==";
    case O::NotEqual:
      return "!=";
    case O::LessThan:
      return "<";
    case O::GreaterThan:
      return ">";
    case O::LessOrEqual:
      return "<=";
    case O::GreaterOrEqual:
      return ">=";
    case O::LogicalAnd:
      return "&&";
    case O::LogicalOr:
      return "||";
  }
  return "";
}

bool is_truth_valued(const Exp& exp) {
  if (const auto* b = std::get_if<ir::Binary>(&exp)) {
    return is_truth_op(b->op);
  }
  if (const auto* u = std::get_if<ir::Unary>(&exp)) {
    return asdl::isa<ir::LogicalNot>(u->op);
  }
  if (const auto* c = std::get_if<ir::Constant>(&exp)) {
    if (asdl::isa<ir::ConstBool>(c->const_)) {
      return true;
    }
    if (const auto* i = std::get_if<ir::ConstInt>(&c->const_)) {
      return i->value == 0 || i->value == 1;
    }
  }
  return false;
}

void check_unowned_object(const std::string& name, const InitialValue& initial,
                          const std::optional<Int>& address) {
  if (address && !std::holds_alternative<NoInitializer>(initial)) {
    fail(pyrepr::string(name) +
         " has `[[c6502::address(...)]]`, which names memory the program doesn't own, so it "
         "can't be defined; declare it `extern` without an initializer");
  }
}

void check_preserves(const std::string& name, const Type& ftype,
                     const std::optional<Int>& address, const NameSet& preserves,
                     const TypeTable* types) {
  if (preserves.empty()) {
    return;
  }
  if (!address) {
    std::string regs;
    for (const std::string& r : preserves) {
      if (!regs.empty()) {
        regs += " / ";
      }
      std::string lower = r;
      std::transform(lower.begin(), lower.end(), lower.begin(),
                     [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
      regs += "preserves_" + lower;
    }
    fail("function " + pyrepr::string(name) + " has `" + regs +
         "` but no `[[c6502::address(...)]]`; only a routine at a fixed address, whose code "
         "the compiler doesn't compile, can declare which registers it preserves");
  }
  const auto* fun = std::get_if<ir::FunType>(&ftype);
  if (fun == nullptr) {
    return;
  }
  const Type& ret = strip_quals(*fun->ret);
  const bool has_x = preserves.count("X") != 0;
  if (has_x && asdl::isa<ir::Int, ir::UInt>(ret)) {
    fail("function " + pyrepr::string(name) + " has `preserves_x` but returns an " +
         (asdl::isa<ir::Int>(ret) ? "int" : "unsigned int") + ", whose high byte comes back in X");
  }
  if (has_x && carry_struct_value_bytes(ret, types) == 2) {
    fail("function " + pyrepr::string(name) + " has `preserves_x` but returns `" + c_type(ret) +
         "`, whose value member is 2 bytes, and the high byte of that comes back in X");
  }
}

std::optional<int> carry_struct_value_bytes(const Type& ret_type, const TypeTable* types) {
  const Type& ret = strip_quals(ret_type);
  const auto* s = std::get_if<ir::Structure>(&ret);
  if (s == nullptr || types == nullptr) {
    return std::nullopt;
  }
  const StructLayout* layout = types->find(s->tag);
  if (layout == nullptr || !layout->complete || layout->members.size() != 2) {
    return std::nullopt;
  }
  const MemberInfo& value = layout->members[0];
  const MemberInfo& flag = layout->members[1];
  const Type& vt = strip_quals(value.type);
  const Type& ft = strip_quals(flag.type);
  int n = 0;
  if (asdl::isa<ir::Char, ir::SChar, ir::UChar, ir::Bool>(vt)) {
    n = 1;
  } else if (asdl::isa<ir::Int, ir::UInt, ir::Short, ir::UShort>(vt)) {
    n = 2;
  } else {
    return std::nullopt;
  }
  if (!asdl::isa<ir::Bool>(ft) || flag.byte_offset != Int(n)) {
    return std::nullopt;
  }
  return n;
}

std::optional<std::string> merge_section(const std::string& name,
                                         const std::optional<std::string>& prior,
                                         const std::optional<std::string>& next) {
  if (prior && next && *prior != *next) {
    fail("two declarations of " + pyrepr::string(display_name(name)) +
         " put it in different sections, " + pyrepr::string(*prior) + " and " +
         pyrepr::string(*next));
  }
  return next ? next : prior;
}

void check_section(const std::string& name, const std::optional<std::string>& section,
                   const std::optional<Int>& address,
                   const std::optional<ir::Type_placement>& placement) {
  if (!section) {
    return;
  }
  if (address) {
    fail(pyrepr::string(display_name(name)) +
         " has `address`, so it's already placed and can't also be in section " +
         pyrepr::string(*section));
  }
  if (placement && asdl::isa<ir::ZeroPage>(*placement)) {
    fail(pyrepr::string(display_name(name)) +
         " is in zero page, so it can't also be in section " + pyrepr::string(*section));
  }
}

void check_carry_return_type(const std::string& name, const Type& ret, const TypeTable* types) {
  const Type& s = strip_quals(ret);
  if (asdl::isa<ir::Bool>(s)) {
    return;
  }
  if (asdl::isa<ir::Structure, ir::Union>(s)) {
    if (carry_struct_value_bytes(ret, types)) {
      return;
    }
    fail("`[[c6502::return_bool_in_carry]]` on " + pyrepr::string(name) + " returns `" +
         c_type(ret) +
         "`, but a struct it returns has to have exactly two members: first an integer value "
         "of 1 or 2 bytes (`char`, `unsigned char`, `bool`, `int`, or `unsigned int`, not a "
         "pointer), which comes back in A (and X), then a `bool` flag, which comes back in the "
         "carry flag");
  }
  fail("`[[c6502::return_bool_in_carry]]` on " + pyrepr::string(name) +
       " needs a `bool` return type, or a struct of a value and a `bool`, not `" + c_type(ret) +
       "`");
}

Type strip_quals_recursive(const Type& t) {
  switch (ir::kind(t)) {
    case K::Const:
      return strip_quals_recursive(*std::get<ir::Const>(t).referenced_type);
    case K::Volatile:
      return strip_quals_recursive(*std::get<ir::Volatile>(t).referenced_type);
    case K::Pointer:
      return ir::Pointer{
          .referenced_type = strip_quals_recursive(*std::get<ir::Pointer>(t).referenced_type)};
    case K::Array: {
      const auto& a = std::get<ir::Array>(t);
      return ir::Array{.element_type = strip_quals_recursive(*a.element_type), .size = a.size};
    }
    case K::FunType: {
      const auto& f = std::get<ir::FunType>(t);
      ir::FunType out;
      for (const Type& p : f.params) {
        out.params.push_back(strip_quals_recursive(p));
      }
      out.ret = strip_quals_recursive(*f.ret);
      return out;
    }
    default:
      return t;
  }
}

bool is_const_qualified(const Type& t) {
  const Type* p = &t;
  while (const auto* v = std::get_if<ir::Volatile>(p)) {
    p = &*v->referenced_type;
  }
  return asdl::isa<ir::Const>(*p);
}

bool is_volatile_qualified(const Type& t) {
  const Type* p = &t;
  while (const auto* c = std::get_if<ir::Const>(p)) {
    p = &*c->referenced_type;
  }
  return asdl::isa<ir::Volatile>(*p);
}

Type propagate_quals(const Type& member_t, const Type& container_t) {
  if (const auto* a = std::get_if<ir::Array>(&member_t)) {
    return ir::Array{.element_type = propagate_quals(*a->element_type, container_t),
                     .size = a->size};
  }
  Type out = member_t;
  if (is_volatile_qualified(container_t) && !is_volatile_qualified(out)) {
    out = ir::Volatile{.referenced_type = std::move(out)};
  }
  if (is_const_qualified(container_t) && !is_const_qualified(out)) {
    out = ir::Const{.referenced_type = std::move(out)};
  }
  return out;
}

std::optional<Type> composite_pointer(const Type& tl, const Type& tr) {
  const Type& pl = *std::get<ir::Pointer>(strip_quals(tl)).referenced_type;
  const Type& pr = *std::get<ir::Pointer>(strip_quals(tr)).referenced_type;
  Type base;
  if (is_void(pl) || is_void(pr)) {
    base = ir::Void{};
  } else if (types_equal(strip_quals(pl), strip_quals(pr))) {
    base = strip_quals(pl);
  } else {
    return std::nullopt;
  }
  if (is_volatile_qualified(pl) || is_volatile_qualified(pr)) {
    base = ir::Volatile{.referenced_type = std::move(base)};
  }
  if (is_const_qualified(pl) || is_const_qualified(pr)) {
    base = ir::Const{.referenced_type = std::move(base)};
  }
  return ir::Pointer{.referenced_type = std::move(base)};
}

Int sizeof_type(const Type& type, const TypeTable* types) {
  const Type& t = strip_quals(type);
  switch (ir::kind(t)) {
    case K::Char:
    case K::SChar:
    case K::UChar:
    case K::Bool:
      return 1;
    // `nullptr_t` is as wide as a pointer to a character type (C23
    // §7.21.2p3).
    case K::Int:
    case K::UInt:
    case K::Short:
    case K::UShort:
    case K::Pointer:
    case K::NullptrT:
      return 2;
    case K::Long:
    case K::ULong:
    case K::Float:
      return 4;
    case K::LongLong:
    case K::ULongLong:
    case K::Double:
      return 8;
    case K::BitInt:
      return bit_precise::storage_bytes(std::get<ir::BitInt>(t).width);
    case K::Array: {
      const auto& a = std::get<ir::Array>(t);
      return sizeof_type(*a.element_type, types) * a.size;
    }
    case K::Structure:
    case K::Union: {
      if (types == nullptr) {
        fail("sizeof of struct/union type " + asdl::py_repr(t) +
             " requires a TypeTable (none provided)");
      }
      const StructLayout* layout = types->find(tag_of(t));
      if (layout == nullptr || !layout->complete) {
        fail("sizeof of incomplete struct/union type '" + struct_keyword(t) + " " + tag_of(t) +
             "' is illegal (C99 §6.5.3.4.1)");
      }
      return layout->size;
    }
    default:
      fail("cannot take sizeof an incomplete or function type: " + asdl::py_repr(t));
  }
}

void check_register_object(const Type& t, int max_bytes, std::string_view where) {
  const std::string w(where);
  if (is_pointer_type(t)) {
    fail(w + ": a pointer can't be placed in a register (`register` is not allowed on pointer "
             "types)");
  }
  if (asdl::isa<ir::Array, ir::Structure, ir::Union, ir::FunType, ir::NullptrT>(strip_quals(t))) {
    fail(w + ": only a scalar integer object can be placed in a register");
  }
  const Int n = sizeof_type(t);
  if (n > Int(max_bytes)) {
    fail(w + ": a " + asdl::int_to_string(n) +
         "-byte value doesn't fit the register budget (at most " + std::to_string(max_bytes) +
         " byte" + (max_bytes != 1 ? "s" : "") + ")");
  }
}

void check_named_register_param(const Type& t, std::string_view regs, std::string_view where) {
  std::string attr = "in_";
  for (const char c : regs) {
    attr += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }
  const std::string w(where);
  if (!(is_pointer_type(t) || is_integer_type(strip_quals(t)))) {
    fail(w + ": `" + attr + "` passes an integer or a pointer, one byte in each register it names");
  }
  const Int n = sizeof_type(t);
  if (n != Int(static_cast<long long>(regs.size()))) {
    fail(w + " is " + asdl::int_to_string(n) + " byte" + (n != Int(1) ? "s" : "") + ", but `" + attr +
         "` names " + std::to_string(regs.size()) + " register" + (regs.size() != 1 ? "s" : "") +
         "; name one register for each byte, low byte first");
  }
}

void check_well_formed_type(const Type& type, std::string_view where, const TypeTable* types,
                            bool require_complete, TypeChecker* checker) {
  const Type& t = strip_quals(type);
  const std::string w(where);
  if (const auto* a = std::get_if<ir::Array>(&t)) {
    if (require_complete && a->size == 0) {
      fail(w + ": incomplete array type (`[]`) is allowed only at the outermost type of an "
               "`extern` declaration");
    }
    if (asdl::isa<ir::Void>(*a->element_type)) {
      fail(w + ": array element type cannot be void (C99 §6.7.5.2.1 — array element must be a "
               "complete object type)");
    }
    if (asdl::isa<ir::FunType>(*a->element_type)) {
      fail(w + ": array element type cannot be a function type (C99 §6.7.5.2.1)");
    }
    check_well_formed_type(*a->element_type, where, types, true, checker);
    return;
  }
  if (const auto* p = std::get_if<ir::Pointer>(&t)) {
    check_well_formed_type(*p->referenced_type, where, types, false, checker);
    return;
  }
  if (!asdl::isa<ir::Structure, ir::Union>(t) || types == nullptr) {
    return;
  }
  const std::string& tag = tag_of(t);
  const std::string display_tag = display_name(tag);
  const std::string kw = struct_keyword(t);
  const StructLayout* layout = types->find(tag);
  if (checker != nullptr && !checker->tag_visible(tag)) {
    if (layout == nullptr) {
      checker->auto_introduce_tag(t);
      layout = types->find(tag);
    } else {
      fail(w + ": '" + kw + " " + display_tag + "' is not in scope");
    }
  }
  if (layout == nullptr) {
    fail(w + ": undeclared type '" + kw + " " + display_tag + "'");
  }
  if (layout->is_union != asdl::isa<ir::Union>(t)) {
    fail(w + ": tag " + pyrepr::string(display_tag) + " declared as '" +
         (layout->is_union ? "union" : "struct") + "' but used as '" + kw + "'");
  }
  if (require_complete && !layout->complete) {
    fail(w + ": incomplete type '" + kw + " " + display_tag +
         "' (C99 §6.7.2.1.8 — type must be complete)");
  }
}

// C99 §6.3.2.1p1: a function designator isn't an lvalue.
void require_not_function(const Type& t, std::string_view what) {
  if (asdl::isa<ir::FunType>(t)) {
    fail(std::string(what) + " target is a function, not a modifiable lvalue");
  }
}

Exp decay_if_array(Exp exp) {
  const std::optional<Type>& t = data_type_of(exp);
  if (!t) {
    return exp;
  }
  if (const auto* a = std::get_if<ir::Array>(&*t)) {
    Type pointer = ir::Pointer{.referenced_type = *a->element_type};
    return ir::AddressOf{.exp = std::move(exp), .data_type = std::move(pointer)};
  }
  if (asdl::isa<ir::FunType>(*t)) {
    Type pointer = ir::Pointer{.referenced_type = *t};
    return ir::AddressOf{.exp = std::move(exp), .data_type = std::move(pointer)};
  }
  return exp;
}

bool is_pointer_to_function(const Type& t) {
  const auto* p = std::get_if<ir::Pointer>(&strip_quals(t));
  return p != nullptr && asdl::isa<ir::FunType>(*p->referenced_type);
}

std::optional<std::string> callee_name(const Exp& exp, const SymbolTable& symbols) {
  std::vector<bool> derefs;
  const Exp* e = &exp;
  while (true) {
    if (const auto* d = std::get_if<ir::Dereference>(e)) {
      derefs.push_back(true);
      e = &*d->exp;
    } else if (const auto* a = std::get_if<ir::AddressOf>(e)) {
      derefs.push_back(false);
      e = &*a->exp;
    } else {
      break;
    }
  }
  const auto* var = std::get_if<ir::Var>(e);
  if (var == nullptr) {
    return std::nullopt;
  }
  const Symbol* sym = symbols.find(var->name);
  if (sym == nullptr) {
    return std::nullopt;
  }
  const Type& t = strip_quals(sym->type);
  bool is_function = false;
  if (asdl::isa<ir::FunType>(t)) {
    is_function = true;
  } else if (is_pointer_to_function(t)) {
    is_function = false;
  } else {
    return std::nullopt;
  }
  for (auto it = derefs.rbegin(); it != derefs.rend(); ++it) {
    if (*it) {
      is_function = true;
    } else if (is_function) {
      is_function = false;
    } else {
      return std::nullopt;
    }
  }
  return var->name;
}

bool is_null_pointer_constant(const Exp& expression) {
  const Exp* exp = &expression;
  while (const auto* c = std::get_if<ir::Cast>(exp)) {
    exp = &*c->exp;
  }
  if (const auto named = constant_expression::named_constant(*exp)) {
    return is_integer_type(named->type) && named->value.kind == ValueTree::Kind::kInt &&
           named->value.integer == 0;
  }
  const auto* constant = std::get_if<ir::Constant>(exp);
  if (constant == nullptr) {
    return false;
  }
  using C = ir::Kind_const;
  const auto& c = constant->const_;
  switch (ir::kind(c)) {
    case C::ConstNullptr:
      return true;
    case C::ConstInt:
      return std::get<ir::ConstInt>(c).value == 0;
    case C::ConstLong:
      return std::get<ir::ConstLong>(c).value == 0;
    case C::ConstLongLong:
      return std::get<ir::ConstLongLong>(c).value == 0;
    case C::ConstUInt:
      return std::get<ir::ConstUInt>(c).value == 0;
    case C::ConstULong:
      return std::get<ir::ConstULong>(c).value == 0;
    case C::ConstULongLong:
      return std::get<ir::ConstULongLong>(c).value == 0;
    case C::ConstChar:
      return std::get<ir::ConstChar>(c).value == 0;
    case C::ConstUChar:
      return std::get<ir::ConstUChar>(c).value == 0;
    case C::ConstBool:
      return std::get<ir::ConstBool>(c).value == 0;
    case C::ConstBitInt:
      return std::get<ir::ConstBitInt>(c).value == 0;
    default:
      return false;
  }
}

bool is_c23_null_pointer_constant(const Exp& expression) {
  if (const auto* c = std::get_if<ir::Constant>(&expression)) {
    if (asdl::isa<ir::ConstNullptr>(c->const_)) {
      return true;
    }
  }
  const Exp* exp = &expression;
  if (const auto* cast = std::get_if<ir::Cast>(exp)) {
    if (const auto* p = std::get_if<ir::Pointer>(&cast->target_type)) {
      if (asdl::isa<ir::Void>(*p->referenced_type)) {
        exp = &*cast->exp;
      }
    }
  }
  const std::optional<Type>& t = data_type_of(*exp);
  if (!t || !is_integer_type(*t)) {
    return false;
  }
  try {
    return constant_expression::evaluate_integer_constant_expression(*exp).first == 0;
  } catch (const constant_expression::ConstantExpressionError&) {
    return false;
  }
}

ir::Type_const const_for_value(Int value, const Type& type) {
  const Type& t = strip_quals(type);
  switch (ir::kind(t)) {
    case K::BitInt: {
      const auto& b = std::get<ir::BitInt>(t);
      const int width = static_cast<int>(b.width);
      return ir::ConstBitInt{.value = value & ((Int(1) << width) - 1),
                             .width = b.width,
                             .is_signed = b.is_signed};
    }
    case K::Int:
    case K::Short:
      return ir::ConstInt{.value = value & Int(0xFFFF)};
    case K::UInt:
    case K::UShort:
      return ir::ConstUInt{.value = value & Int(0xFFFF)};
    case K::SChar:
      return ir::ConstChar{.value = value & Int(0xFF)};
    case K::Char:
    case K::UChar:
    case K::Bool:
      return ir::ConstUChar{.value = value & Int(0xFF)};
    case K::Long:
      return ir::ConstLong{.value = value & Int(0xFFFFFFFFULL)};
    case K::ULong:
      return ir::ConstULong{.value = value & Int(0xFFFFFFFFULL)};
    case K::LongLong:
      return ir::ConstLongLong{.value = value & Int(0xFFFFFFFFFFFFFFFFULL)};
    case K::ULongLong:
      return ir::ConstULongLong{.value = value & Int(0xFFFFFFFFFFFFFFFFULL)};
    default:
      throw InternalError("_const_for_value: not an integer type: " + asdl::py_repr(t));
  }
}

Exp convert_to(Exp exp, const Type& target) {
  const std::optional<Type> src = data_type_of(exp);
  if (src && types_equal(strip_quals_recursive(*src), strip_quals_recursive(target))) {
    return exp;
  }
  if (src && is_void(*src)) {
    fail("void expression cannot be converted to " + asdl::py_repr(target) +
         "; void has no value");
  }
  if (is_void(target)) {
    fail("cannot implicitly convert " + repr_or_none(src) +
         " to void; use an explicit `(void)expr` cast to discard a value");
  }
  if (is_struct_or_union(target) || (src && is_struct_or_union(*src))) {
    fail("cannot convert " + repr_or_none(src) + " to " + asdl::py_repr(target) +
         "; a struct or union value converts only to its own type");
  }
  if (src && is_nullptr_type(*src)) {
    if (!asdl::isa<ir::Pointer, ir::Bool>(strip_quals(target))) {
      fail("cannot convert nullptr_t to " + c_type(target) +
           ": a nullptr_t value converts implicitly only to a pointer type or bool (C23 "
           "§6.3.2.4, §6.5.17.2)");
    }
    return ir::Cast{.target_type = target, .exp = std::move(exp), .data_type = target};
  }
  if (is_nullptr_type(target)) {
    if (!is_c23_null_pointer_constant(exp)) {
      fail("cannot convert " + c_type_or_none(src) +
           " to nullptr_t: only nullptr, another nullptr_t value, or a null pointer constant "
           "converts to nullptr_t (C23 §6.3.2.4, §6.5.17.2)");
    }
    return ir::Cast{.target_type = target, .exp = std::move(exp), .data_type = target};
  }
  if (asdl::isa<ir::Pointer>(target) && src) {
    if (is_integer_type(*src)) {
      if (!is_null_pointer_constant(exp)) {
        fail("cannot implicitly convert non-null integer to pointer type " +
             asdl::py_repr(target) +
             "; only the null pointer constant (an integer constant expression with value 0) "
             "is assignable to a pointer per C99 §6.3.2.3.3");
      }
    } else if (is_pointer_type(*src)) {
      if (!is_void_pointer(*src) && !is_void_pointer(target)) {
        fail("cannot implicitly convert " + asdl::py_repr(*src) + " to " +
             asdl::py_repr(target) +
             "; pointer-to-pointer assignment requires matching pointee types per C99 "
             "§6.5.16.1.1");
      }
    } else {
      fail("cannot implicitly convert " + asdl::py_repr(*src) + " to pointer type " +
           asdl::py_repr(target) +
           "; only an integer null pointer constant or a matching pointer type is assignable "
           "per C99 §6.5.16.1.1");
    }
  }
  if (is_arithmetic_type(target) && !asdl::isa<ir::Bool>(strip_quals(target)) && src &&
      is_pointer_type(*src)) {
    fail("cannot implicitly convert pointer type " + asdl::py_repr(*src) +
         " to arithmetic type " + asdl::py_repr(target) + "; use an explicit cast");
  }
  return ir::Cast{.target_type = target, .exp = std::move(exp), .data_type = target};
}

void TypeChecker::push_tag_scope() { tag_scopes.emplace_back(); }

void TypeChecker::pop_tag_scope() { tag_scopes.pop_back(); }

bool TypeChecker::tag_visible(std::string_view tag) const {
  for (const auto& scope : tag_scopes) {
    if (scope.find(tag) != scope.end()) {
      return true;
    }
  }
  return false;
}

void TypeChecker::record_tag_visible(const std::string& tag) { tag_scopes.back().insert(tag); }

void TypeChecker::auto_introduce_tag(const Type& t) {
  const std::string& tag = tag_of(t);
  types.insert_or_assign(tag, StructLayout{.tag = tag,
                                           .is_union = asdl::isa<ir::Union>(t),
                                           .members = {},
                                           .size = 0,
                                           .complete = false});
  record_tag_visible(tag);
}

void TypeChecker::require_scalar_controlling(const Exp& exp, std::string_view where) const {
  const std::optional<Type>& t = data_type_of(exp);
  if (t && asdl::isa<ir::Array>(*t)) {
    return;
  }
  if (t && is_object_type(*t)) {
    return;
  }
  const std::string kind = t ? class_name(*t) : std::string("NoneType");
  std::string tag;
  if (t && asdl::isa<ir::Structure, ir::Union>(*t)) {
    tag = " '" + struct_keyword(*t) + " " + tag_of(*t) + "'";
  }
  fail(std::string(where) + ": controlling expression must have scalar type, got " + kind + tag +
       " (C99 §6.8.4.1)");
}

void TypeChecker::require_complete_value(const Exp& exp, std::string_view where) const {
  const std::optional<Type>& t = data_type_of(exp);
  if (!t || !asdl::isa<ir::Structure, ir::Union>(*t)) {
    return;
  }
  const StructLayout* layout = types.find(tag_of(*t));
  if (layout == nullptr || !layout->complete) {
    fail(std::string(where) + ": incomplete type '" + struct_keyword(*t) + " " + tag_of(*t) +
         "' (C99 §6.7.2.1.8)");
  }
}

void TypeChecker::require_complete_pointee(const Type& ptr, std::string_view where) const {
  const Type& pointee = strip_quals(*std::get<ir::Pointer>(strip_quals(ptr)).referenced_type);
  const std::string w(where);
  if (asdl::isa<ir::Void>(pointee)) {
    fail(w + " is not defined on a pointer to void (" + asdl::py_repr(ptr) + ")");
  }
  if (asdl::isa<ir::FunType>(pointee)) {
    fail(w + " is not defined on a pointer to a function (" + asdl::py_repr(ptr) + ")");
  }
  if (asdl::isa<ir::Structure, ir::Union>(pointee)) {
    const StructLayout* layout = types.find(tag_of(pointee));
    if (layout == nullptr || !layout->complete) {
      fail(w + " is not defined on a pointer to the incomplete type '" +
           struct_keyword(pointee) + " " + tag_of(pointee) + "' (C99 §6.5.6)");
    }
  }
}

void TypeChecker::check_well_formed(const Type& t, std::string_view where, bool require_complete) {
  check_well_formed_type(t, where, &types, require_complete, this);
}

}  // namespace c6502::passes::type_checking
