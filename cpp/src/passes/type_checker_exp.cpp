#include <string>
#include <utility>

#include "builtin_functions.h"
#include "passes/type_checker.h"
#include "pyrepr.h"

namespace c6502::passes::type_checking {

namespace {

namespace ir = ::c6502::c99_ast;
using E = ir::Kind_exp;
using O = ir::Kind_binary_operator;

[[noreturn]] void fail(const std::string& message) { throw TypeCheckError(message); }

std::string class_name(const std::optional<Type>& t) {
  if (!t) {
    return "NoneType";
  }
  std::string r = asdl::py_repr(*t);
  return r.substr(0, r.find('('));
}

const std::string& tag_of(const Type& t) {
  if (const auto* s = std::get_if<ir::Structure>(&t)) {
    return s->tag;
  }
  return std::get<ir::Union>(t).tag;
}

std::optional<Type> checked_type(const Exp& exp) {
  if (const auto* s = std::get_if<ir::String>(&exp)) {
    return ir::Array{.element_type = ir::Char{},
                     .size = static_cast<long long>(pyrepr::code_points(s->str).size()) + 1};
  }
  return data_type_of(exp);
}

Exp decay(Exp exp) {
  if (asdl::isa<ir::String>(exp)) {
    return ir::AddressOf{.exp = std::move(exp),
                         .data_type = Type(ir::Pointer{.referenced_type = ir::Char{}})};
  }
  return decay_if_array(std::move(exp));
}

void decay_in_place(asdl::Box<Exp>& slot) { *slot = decay(std::move(*slot)); }

void convert_in_place(asdl::Box<Exp>& slot, const Type& target) {
  *slot = convert_to(std::move(*slot), target);
}

bool opt_object(const std::optional<Type>& t) { return t && is_object_type(*t); }
bool opt_pointer(const std::optional<Type>& t) { return t && is_pointer_type(*t); }
bool opt_integer(const std::optional<Type>& t) { return t && is_integer_type(*t); }
bool opt_floating(const std::optional<Type>& t) { return t && is_floating_type(*t); }
bool opt_arithmetic(const std::optional<Type>& t) { return t && is_arithmetic_type(*t); }
bool opt_void(const std::optional<Type>& t) { return t && is_void(*t); }
bool opt_struct_or_union(const std::optional<Type>& t) { return t && is_struct_or_union(*t); }

const Type& need(const std::optional<Type>& t, const char* where) {
  if (!t) {
    throw InternalError(std::string("no type on the ") + where);
  }
  return *t;
}

bool is_equality(O op) { return op == O::Equal || op == O::NotEqual; }

bool is_relational(O op) {
  return op == O::LessThan || op == O::GreaterThan || op == O::LessOrEqual ||
         op == O::GreaterOrEqual;
}

Type check_cast(TypeChecker& tc, ir::Cast& e) {
  const Type target = e.target_type;
  if (!(is_object_type(target) || is_void(target))) {
    fail("cast target type must be an object type (Int / Long / LongLong / UInt / ULong / "
         "ULongLong / Float / Double / Pointer) or void, got " +
         asdl::py_repr(target));
  }
  tc.check_well_formed(target, "cast target");
  tc.check_exp(*e.exp);
  decay_in_place(e.exp);
  const Exp& inner = *e.exp;
  const std::optional<Type> inner_type = checked_type(inner);
  tc.require_complete_value(inner, "cast operand");
  if (is_void(target)) {
    e.data_type = target;
    return target;
  }
  if (!opt_object(inner_type)) {
    fail("cannot cast non-object type " + repr_or_none(inner_type) + " to " +
         asdl::py_repr(target));
  }
  const Type& src = *inner_type;
  if (is_nullptr_type(src) &&
      !asdl::isa<ir::Pointer, ir::Bool, ir::NullptrT>(strip_quals(target))) {
    fail("cannot cast nullptr_t to " + c_type(target) +
         ": nullptr_t converts only to void, bool, or a pointer type (C23 §6.5.5p4)");
  }
  if (is_nullptr_type(target) && !is_nullptr_type(src) && !is_c23_null_pointer_constant(inner)) {
    fail("cannot cast " + c_type(src) +
         " to nullptr_t: the operand must be a null pointer constant or have type nullptr_t "
         "(C23 §6.5.5p4)");
  }
  // C99 §6.3.2.3 defines conversions only between pointers and integers
  // and between two pointers.
  const bool src_fp = is_floating_type(src);
  const bool tgt_fp = is_floating_type(target);
  const bool src_ptr = is_pointer_type(src);
  const bool tgt_ptr = is_pointer_type(target);
  if ((src_fp && tgt_ptr) || (src_ptr && tgt_fp)) {
    fail("cannot cast between pointer and floating-point type: " + asdl::py_repr(src) + " → " +
         asdl::py_repr(target));
  }
  if (src_ptr && is_integer_type(target)) {
    if (const auto offset = null_based_offset(inner, &tc.types)) {
      *e.exp = ir::Constant{.const_ = ir::ConstUInt{.value = *offset & Int(0xFFFF)},
                            .data_type = Type(ir::UInt{})};
    }
  }
  e.data_type = target;
  return target;
}

Type check_var(TypeChecker& tc, ir::Var& e) {
  const Symbol* sym = tc.symbols.find(e.name);
  if (sym == nullptr) {
    fail("undeclared identifier " + pyrepr::string(e.name));
  }
  if (const auto* f = std::get_if<FunAttr>(&sym->attrs); f != nullptr && f->carry_return) {
    fail("can't take the address of " + pyrepr::string(e.name) +
         ", which returns its result in the carry flag");
  }
  e.data_type = sym->type;
  return sym->type;
}

std::string unary_spelling(const ir::Type_unary_operator& op) {
  switch (ir::kind(op)) {
    case ir::Kind_unary_operator::Negate:
      return "-";
    case ir::Kind_unary_operator::Complement:
      return "~";
    case ir::Kind_unary_operator::Plus:
      return "+";
    default:
      throw InternalError("check_unary: an operator of no kind the type checker knows: " +
                          asdl::py_repr(op));
  }
}

Type check_unary(TypeChecker& tc, ir::Unary& e) {
  tc.check_exp(*e.exp);
  decay_in_place(e.exp);
  std::optional<Type> t = checked_type(*e.exp);
  if (!opt_object(t)) {
    fail("unary operator on non-object type " + repr_or_none(t));
  }
  const bool logical_not = asdl::isa<ir::LogicalNot>(e.op);
  if (is_nullptr_type(*t) && !logical_not) {
    fail("unary '" + unary_spelling(e.op) + "' is not defined on nullptr_t (C23 §6.5.4.3)");
  }
  if (!logical_not && is_pointer_type(*t)) {
    fail("unary '" + unary_spelling(e.op) + "' is not defined on pointer type " +
         asdl::py_repr(*t));
  }
  if (asdl::isa<ir::Complement>(e.op) && is_floating_type(*t)) {
    fail("unary '~' requires integer operand, got " + class_name(t));
  }
  if (!logical_not) {
    const Type promoted = promote_integer(*t);
    if (!types_equal(promoted, *t)) {
      convert_in_place(e.exp, promoted);
      t = promoted;
    }
  }
  const Type result = logical_not ? Type(ir::Int{}) : *t;
  e.data_type = result;
  return result;
}

Type check_pointer_additive(TypeChecker& tc, ir::Binary& e, const Type& tl, const Type& tr) {
  const bool add = asdl::isa<ir::Add>(e.op);
  const std::string op_name = add ? "+" : "-";
  const std::string operands = "(" + asdl::py_repr(tl) + " " + op_name + " " + asdl::py_repr(tr) + ")";
  const Type tl_p = strip_quals(tl);
  const Type tr_p = strip_quals(tr);
  for (const Type* t : {&tl_p, &tr_p}) {
    const auto* p = std::get_if<ir::Pointer>(t);
    if (p == nullptr) {
      continue;
    }
    if (asdl::isa<ir::FunType>(strip_quals(*p->referenced_type))) {
      fail("binary '" + op_name + "' is not defined on pointer-to-function operands " + operands);
    }
    if (is_void(*p->referenced_type)) {
      fail("binary '" + op_name + "' is not defined on void pointer operands " + operands);
    }
    tc.require_complete_pointee(*t, "binary '" + op_name + "'");
  }
  if (is_floating_type(tl) || is_floating_type(tr)) {
    fail("binary '" + op_name + "' is not defined on a pointer and a floating-point operand " +
         operands);
  }
  if (is_pointer_type(tl) && is_pointer_type(tr)) {
    if (add) {
      fail("binary '+' is not defined on two pointer operands (" + asdl::py_repr(tl) + " + " +
           asdl::py_repr(tr) + ")");
    }
    if (!types_equal(strip_quals(*std::get<ir::Pointer>(tl_p).referenced_type),
                     strip_quals(*std::get<ir::Pointer>(tr_p).referenced_type))) {
      fail("subtraction of distinct pointer types: " + asdl::py_repr(tl) + " - " +
           asdl::py_repr(tr));
    }
    e.data_type = Type(ir::Int{});
    return ir::Int{};
  }
  const bool int_is_left = is_integer_type(tl);
  if (int_is_left && !add) {
    fail("binary '-' between integer and pointer is not defined (" + asdl::py_repr(tl) + " - " +
         asdl::py_repr(tr) + ")");
  }
  const Type* ptr_type = nullptr;
  if (int_is_left) {
    convert_in_place(e.left, ir::Int{});
    ptr_type = &tr_p;
  } else {
    convert_in_place(e.right, ir::Int{});
    ptr_type = &tl_p;
  }
  Type result = ir::Pointer{.referenced_type = *std::get<ir::Pointer>(*ptr_type).referenced_type};
  e.data_type = result;
  return result;
}

Type check_binary(TypeChecker& tc, ir::Binary& e) {
  tc.check_exp(*e.left);
  tc.check_exp(*e.right);
  decay_in_place(e.left);
  decay_in_place(e.right);
  const std::optional<Type> otl = checked_type(*e.left);
  const std::optional<Type> otr = checked_type(*e.right);
  if (!opt_object(otl) || !opt_object(otr)) {
    fail("binary operator on non-object types: " + repr_or_none(otl) + ", " + repr_or_none(otr));
  }
  Type tl = *otl;
  Type tr = *otr;
  const O op = ir::kind(e.op);
  if (op == O::LogicalAnd || op == O::LogicalOr) {
    e.data_type = Type(ir::Int{});
    return ir::Int{};
  }
  if (is_nullptr_type(tl) || is_nullptr_type(tr)) {
    if (auto result = tc.check_nullptr_binary(e, tl, tr)) {
      return *result;
    }
  }
  if (is_equality(op) && (is_pointer_type(tl) || is_pointer_type(tr))) {
    const Type common = tc.pointer_equality_common_type(*e.left, *e.right, tl, tr);
    convert_in_place(e.left, common);
    convert_in_place(e.right, common);
    e.data_type = Type(ir::Int{});
    return ir::Int{};
  }
  if (is_relational(op) && (is_pointer_type(tl) || is_pointer_type(tr))) {
    const std::string op_name(binary_op_spelling(e.op));
    if (!(is_pointer_type(tl) && is_pointer_type(tr))) {
      fail("binary '" + op_name + "' between pointer and non-pointer operand: " +
           asdl::py_repr(tl) + " " + op_name + " " + asdl::py_repr(tr));
    }
    const auto common = composite_pointer(tl, tr);
    if (!common || is_void_pointer(tl) != is_void_pointer(tr)) {
      fail("comparison of distinct pointer types: " + asdl::py_repr(tl) + " " + op_name + " " +
           asdl::py_repr(tr));
    }
    e.data_type = Type(ir::Int{});
    return ir::Int{};
  }
  if ((op == O::Multiply || op == O::Divide || op == O::Modulo) &&
      (is_pointer_type(tl) || is_pointer_type(tr))) {
    const std::string op_name(binary_op_spelling(e.op));
    fail("binary '" + op_name + "' is not defined on pointer operands (" + asdl::py_repr(tl) +
         " " + op_name + " " + asdl::py_repr(tr) + ")");
  }
  if ((op == O::Add || op == O::Subtract) && (is_pointer_type(tl) || is_pointer_type(tr))) {
    return check_pointer_additive(tc, e, tl, tr);
  }
  if ((op == O::Modulo || op == O::BitwiseAnd || op == O::BitwiseOr || op == O::BitwiseXor ||
       op == O::LeftShift || op == O::RightShift) &&
      (is_floating_type(tl) || is_floating_type(tr) || is_pointer_type(tl) ||
       is_pointer_type(tr))) {
    const std::string op_name(binary_op_spelling(e.op));
    fail("binary '" + op_name + "' requires integer operands (" + asdl::py_repr(tl) + " " +
         op_name + " " + asdl::py_repr(tr) + ")");
  }
  convert_in_place(e.left, promote_integer(tl));
  convert_in_place(e.right, promote_integer(tr));
  tl = need(checked_type(*e.left), "binary left operand");
  tr = need(checked_type(*e.right), "binary right operand");
  if (op == O::LeftShift || op == O::RightShift) {
    e.data_type = tl;
    return tl;
  }
  const Type common = common_type(tl, tr);
  convert_in_place(e.left, common);
  convert_in_place(e.right, common);
  const Type result = is_truth_op(e.op) ? Type(ir::Int{}) : common;
  e.data_type = result;
  return result;
}

Type check_compound_assignment(TypeChecker& tc, ir::CompoundAssignment& e) {
  const Type tl = tc.check_exp(*e.lval);
  const Type tr0 = tc.check_exp(*e.rval);
  require_not_function(tl, "compound assignment");
  if (is_const_qualified(tl)) {
    fail("cannot modify const-qualified lvalue with compound assignment: " +
         asdl::py_repr(*e.lval) + " has type " + asdl::py_repr(tl));
  }
  tc.require_complete_value(*e.lval, "compound assignment lval");
  tc.require_complete_value(*e.rval, "compound assignment rval");
  if (is_array_type(tl)) {
    fail("cannot use compound assignment on an array: " + asdl::py_repr(*e.lval));
  }
  if (is_struct_or_union(tl)) {
    fail("compound assignment is not defined for struct/union types (" + asdl::py_repr(tl) + ")");
  }
  if (is_nullptr_type(tl) || is_nullptr_type(tr0)) {
    fail("compound assignment is not defined on nullptr_t (" + c_type(tl) + ", " + c_type(tr0) +
         "; C23 §6.5.17.3)");
  }
  decay_in_place(e.rval);
  const std::optional<Type> tr = checked_type(*e.rval);
  if (!asdl::isa<ir::Pointer>(strip_quals(tl)) && !opt_arithmetic(tr)) {
    fail("compound assignment needs an arithmetic right operand, got " + repr_or_none(tr));
  }
  const O op = ir::kind(e.op);
  const std::string operands = "(" + asdl::py_repr(tl) + ", " + repr_or_none(tr) + ")";
  if (op == O::Modulo || op == O::BitwiseAnd || op == O::BitwiseOr || op == O::BitwiseXor ||
      op == O::LeftShift || op == O::RightShift) {
    if (is_floating_type(tl) || opt_floating(tr)) {
      fail("compound `" + asdl::py_repr(e.op) + "` requires integer operands " + operands);
    }
    if (op == O::LeftShift || op == O::RightShift) {
      if (is_pointer_type(tl) || opt_pointer(tr)) {
        fail("compound shift requires integer operands " + operands);
      }
    }
  }
  const bool additive = op == O::Add || op == O::Subtract;
  if (is_pointer_type(tl) && !additive) {
    fail("compound `" + asdl::py_repr(e.op) + "` is not defined on a pointer left operand " +
         operands);
  }
  if (additive && is_pointer_type(tl)) {
    if (!opt_integer(tr)) {
      fail("pointer compound +/- requires integer rhs " + operands);
    }
    tc.require_complete_pointee(tl, "compound +/-");
    convert_in_place(e.rval, ir::Int{});
    e.intermediate_type = tl;
    e.data_type = tl;
    return tl;
  }
  const Type& trv = need(tr, "compound assignment right operand");
  const Type tl_p = promote_integer(tl);
  const Type tr_p = promote_integer(trv);
  if (op == O::LeftShift || op == O::RightShift) {
    if (!(trv == tr_p)) {
      convert_in_place(e.rval, tr_p);
    }
    e.intermediate_type = tl_p;
    e.data_type = tl;
    return tl;
  }
  const Type common = common_type(tl_p, tr_p);
  convert_in_place(e.rval, common);
  e.intermediate_type = common;
  e.data_type = tl;
  return tl;
}

Type check_assignment(TypeChecker& tc, ir::Assignment& e) {
  const Type tl = tc.check_exp(*e.lval);
  const Type tr = tc.check_exp(*e.rval);
  require_not_function(tl, "assignment");
  if (is_void(tl)) {
    fail("cannot assign to an expression of type void: " + asdl::py_repr(*e.lval));
  }
  if (is_const_qualified(tl)) {
    fail("cannot assign to const-qualified lvalue: " + asdl::py_repr(*e.lval) + " has type " +
         asdl::py_repr(tl));
  }
  tc.require_complete_value(*e.lval, "assignment lval");
  tc.require_complete_value(*e.rval, "assignment rval");
  if (is_array_type(tl)) {
    fail("cannot assign to an array (use subscript): " + asdl::py_repr(*e.lval));
  }
  if (is_struct_or_union(tl)) {
    if (!types_equal(strip_quals(tl), strip_quals(tr))) {
      fail("struct/union assignment between distinct types: " + asdl::py_repr(tl) + " = " +
           asdl::py_repr(tr));
    }
    e.data_type = tl;
    return tl;
  }
  decay_in_place(e.rval);
  convert_in_place(e.rval, tl);
  e.data_type = tl;
  return tl;
}

Type check_incdec(TypeChecker& tc, asdl::Box<Exp>& operand, std::optional<Type>& data_type) {
  const Type t = tc.check_exp(*operand);
  require_not_function(t, "increment / decrement");
  if (is_const_qualified(t)) {
    fail("cannot use ++/-- on const-qualified lvalue: " + asdl::py_repr(*operand) + " has type " +
         asdl::py_repr(t));
  }
  if (!is_object_type(t)) {
    fail("increment/decrement operator on non-object type " + asdl::py_repr(t));
  }
  if (is_nullptr_type(t)) {
    fail("the operand of ++ or -- must have a real or pointer type, and nullptr_t is neither "
         "(C23 §6.5.3.5, §6.5.4.1)");
  }
  if (is_void_pointer(t)) {
    fail("increment/decrement on void pointer is not defined (sizeof(void) is undefined)");
  }
  data_type = t;
  return t;
}

Type check_conditional(TypeChecker& tc, ir::Conditional& e) {
  tc.check_exp(*e.condition);
  tc.require_scalar_controlling(*e.condition, "`?:` condition");
  tc.check_exp(*e.true_clause);
  tc.check_exp(*e.false_clause);
  decay_in_place(e.true_clause);
  decay_in_place(e.false_clause);
  std::optional<Type> tt = checked_type(*e.true_clause);
  std::optional<Type> tf = checked_type(*e.false_clause);
  // C99 §6.5.15p5: "If both the second and third operands have void
  // type, the result has void type."
  if (opt_void(tt) && opt_void(tf)) {
    e.data_type = Type(ir::Void{});
    return ir::Void{};
  }
  if (opt_void(tt) || opt_void(tf)) {
    fail("conditional branches must both be void or both be value-producing; got " +
         repr_or_none(tt) + " vs " + repr_or_none(tf));
  }
  if (opt_struct_or_union(tt) || opt_struct_or_union(tf)) {
    if (tt) {
      tt = strip_quals(*tt);
    }
    if (tf) {
      tf = strip_quals(*tf);
    }
    if (!(opt_struct_or_union(tt) && opt_struct_or_union(tf) && types_equal(*tt, *tf))) {
      fail("conditional branches must have matching struct/union type; got " + repr_or_none(tt) +
           " vs " + repr_or_none(tf));
    }
    const StructLayout* layout = tc.types.find(tag_of(*tt));
    if (layout == nullptr || !layout->complete) {
      fail(std::string("conditional branches have incomplete type '") +
           (asdl::isa<ir::Union>(*tt) ? "union" : "struct") + " " + tag_of(*tt) + "'");
    }
    e.data_type = *tt;
    return *tt;
  }
  if (!opt_object(tt) || !opt_object(tf)) {
    fail("conditional branches must be object types, got " + repr_or_none(tt) + ", " +
         repr_or_none(tf));
  }
  const bool tt_ptr = is_pointer_type(*tt);
  const bool tf_ptr = is_pointer_type(*tf);
  // C23 §6.5.16p3, footnote 109: "If a second or third operand of type
  // nullptr_t is used and the other operand is not a pointer and does
  // not have type nullptr_t itself, a constraint is violated even if
  // that other operand is a null pointer constant such as 0."
  const bool tt_null = is_nullptr_type(*tt);
  const bool tf_null = is_nullptr_type(*tf);
  if (tt_null && tf_null) {
    e.data_type = Type(ir::NullptrT{});
    return ir::NullptrT{};
  }
  if ((tt_null && !tf_ptr) || (tf_null && !tt_ptr)) {
    fail("conditional branches of type " + c_type(*tt) + " and " + c_type(*tf) +
         ": the other branch of a nullptr_t branch must be a pointer or another nullptr_t (C23 "
         "§6.5.16p3)");
  }
  Type common;
  if (tt_ptr || tf_ptr) {
    if (tt_ptr && tf_ptr) {
      auto c = composite_pointer(*tt, *tf);
      if (!c) {
        fail("conditional branches have distinct pointer types: " + asdl::py_repr(*tt) + " vs " +
             asdl::py_repr(*tf));
      }
      common = std::move(*c);
    } else if (tt_ptr && (is_null_pointer_constant(*e.false_clause) || tf_null)) {
      common = ir::Pointer{.referenced_type = *std::get<ir::Pointer>(strip_quals(*tt)).referenced_type};
    } else if (tf_ptr && (is_null_pointer_constant(*e.true_clause) || tt_null)) {
      common = ir::Pointer{.referenced_type = *std::get<ir::Pointer>(strip_quals(*tf)).referenced_type};
    } else {
      fail("conditional branches must both be arithmetic or both pointer (or pointer + null "
           "pointer constant); got " +
           asdl::py_repr(*tt) + " vs " + asdl::py_repr(*tf));
    }
  } else {
    convert_in_place(e.true_clause, promote_integer(*tt));
    convert_in_place(e.false_clause, promote_integer(*tf));
    tt = checked_type(*e.true_clause);
    tf = checked_type(*e.false_clause);
    common = common_type(need(tt, "true clause"), need(tf, "false clause"));
  }
  convert_in_place(e.true_clause, common);
  convert_in_place(e.false_clause, common);
  e.data_type = common;
  return common;
}

Type check_comma(TypeChecker& tc, ir::Comma& e) {
  tc.check_exp(*e.left);
  tc.require_complete_value(*e.left, "left operand of comma operator");
  tc.check_exp(*e.right);
  decay_in_place(e.right);
  const std::optional<Type> t = checked_type(*e.right);
  e.data_type = t;
  return need(t, "comma right operand");
}

Type check_function_call(TypeChecker& tc, ir::FunctionCall& e) {
  if (builtin_functions::is_builtin(e.name)) {
    return tc.check_builtin_call(e);
  }
  const Symbol* sym = tc.symbols.find(e.name);
  if (sym == nullptr) {
    fail("undeclared identifier " + pyrepr::string(e.name));
  }
  ir::FunType fn_type;
  if (const auto* f = std::get_if<ir::FunType>(&sym->type)) {
    fn_type = *f;
  } else if (is_pointer_to_function(sym->type)) {
    fn_type = std::get<ir::FunType>(*std::get<ir::Pointer>(strip_quals(sym->type)).referenced_type);
  } else {
    fail("variable " + pyrepr::string(e.name) + " called as a function");
  }
  return tc.check_call_args(e.data_type, pyrepr::string(e.name), fn_type, e.args);
}

Type check_indirect_call(TypeChecker& tc, ir::IndirectCall& e) {
  const ir::FunType fn_type = tc.check_callee(e);
  std::string label = "through a function pointer";
  if (const auto* v = std::get_if<ir::Var>(&*e.callee)) {
    label = pyrepr::string(v->name);
  }
  return tc.check_call_args(e.data_type, label, fn_type, e.args);
}

Type check_dereference(TypeChecker& tc, ir::Dereference& e) {
  tc.check_exp(*e.exp);
  decay_in_place(e.exp);
  const std::optional<Type> t_inner = checked_type(*e.exp);
  if (!opt_pointer(t_inner)) {
    fail("unary '*' requires a pointer operand, got " + class_name(t_inner));
  }
  const Type pointee = *std::get<ir::Pointer>(strip_quals(*t_inner)).referenced_type;
  const Type& pointee_uq = strip_quals(pointee);
  if (asdl::isa<ir::Structure, ir::Union>(pointee_uq)) {
    const StructLayout* layout = tc.types.find(tag_of(pointee_uq));
    if (layout == nullptr || !layout->complete) {
      fail(std::string("dereference of pointer to incomplete type '") +
           (asdl::isa<ir::Union>(pointee_uq) ? "union" : "struct") + " " +
           display_name(tag_of(pointee_uq)) + "' is not permitted");
    }
  }
  e.data_type = pointee;
  return pointee;
}

Type check_sizeof_exp(TypeChecker& tc, ir::SizeOfExp& e) {
  tc.check_exp(*e.exp);
  const std::optional<Type> inner_t = checked_type(*e.exp);
  if (!inner_t) {
    fail("sizeof: inner expression has no type");
  }
  if (is_void(*inner_t)) {
    fail("sizeof of void expression is illegal (C99 §6.5.3.4.1)");
  }
  if (asdl::isa<ir::FunType>(*inner_t)) {
    fail("sizeof of a function type is illegal (C99 §6.5.3.4.1)");
  }
  if (const auto* a = std::get_if<ir::Array>(&strip_quals(*inner_t)); a != nullptr && a->size == 0) {
    fail("sizeof of an incomplete array type is illegal (C99 §6.5.3.4.1)");
  }
  e.data_type = Type(ir::UInt{});
  return ir::UInt{};
}

Type check_sizeof_type(TypeChecker& tc, ir::SizeOfType& e) {
  const Type& t = e.target_type;
  if (is_void(t)) {
    fail("sizeof(void) is illegal — void is an incomplete type (C99 §6.5.3.4.1)");
  }
  if (asdl::isa<ir::FunType>(t)) {
    fail("sizeof of a function type is illegal (C99 §6.5.3.4.1)");
  }
  tc.check_well_formed(t, "sizeof type",
                       asdl::isa<ir::Structure, ir::Union, ir::Array>(strip_quals(t)));
  e.data_type = Type(ir::UInt{});
  return ir::UInt{};
}

Type check_subscript(TypeChecker& tc, ir::Subscript& e) {
  tc.check_exp(*e.array);
  tc.check_exp(*e.index);
  decay_in_place(e.array);
  decay_in_place(e.index);
  const std::optional<Type> ta = checked_type(*e.array);
  const std::optional<Type> ti = checked_type(*e.index);
  if (opt_pointer(ta) && opt_integer(ti)) {
  } else if (opt_pointer(ti) && opt_integer(ta)) {
    std::swap(*e.array, *e.index);
  } else {
    fail("subscript needs a pointer/array operand and an integer operand; got " +
         repr_or_none(ta) + " and " + repr_or_none(ti));
  }
  const Type ptr_type_uq = strip_quals(need(checked_type(*e.array), "subscript pointer"));
  const Type& referenced = *std::get<ir::Pointer>(ptr_type_uq).referenced_type;
  if (asdl::isa<ir::FunType>(referenced)) {
    fail("subscript of pointer-to-function is not supported");
  }
  if (is_void(referenced)) {
    tc.require_complete_pointee(ptr_type_uq, "subscript");
  }
  convert_in_place(e.index, ir::Int{});
  e.data_type = referenced;
  return referenced;
}

Type check_dot(TypeChecker& tc, ir::Dot& e) {
  tc.check_exp(*e.operand);
  const Type t_op = need(checked_type(*e.operand), "member access operand");
  const MemberInfo m = tc.lookup_member(t_op, e.member, "member access '." + e.member + "'");
  const Type result = propagate_quals(m.type, t_op);
  e.data_type = result;
  return result;
}

Type check_arrow(TypeChecker& tc, ir::Arrow& e) {
  tc.check_exp(*e.operand);
  decay_in_place(e.operand);
  const std::optional<Type> t_op = checked_type(*e.operand);
  const Type* t_op_uq = t_op ? &strip_quals(*t_op) : nullptr;
  if (t_op_uq == nullptr || !asdl::isa<ir::Pointer>(*t_op_uq)) {
    fail("member access '->" + e.member + "': operand has non-pointer type " +
         repr_or_none(t_op));
  }
  const Type pointee = *std::get<ir::Pointer>(*t_op_uq).referenced_type;
  const MemberInfo m = tc.lookup_member(pointee, e.member, "member access '->" + e.member + "'");
  const Type result = propagate_quals(m.type, pointee);
  e.data_type = result;
  return result;
}

Type check_address_of(TypeChecker& tc, ir::AddressOf& e) {
  Exp& inner = *e.exp;
  if (const auto* cl = std::get_if<ir::CompoundLiteral>(&inner)) {
    if (cl->storage_class && asdl::isa<ir::Register>(*cl->storage_class)) {
      fail("can't take the address of a `register` compound literal (C99 §6.5.3.2.1)");
    }
  }
  if (auto* v = std::get_if<ir::Var>(&inner)) {
    if (const Symbol* sym = tc.symbols.find(v->name)) {
      if (const auto* local = std::get_if<LocalAttr>(&sym->attrs);
          local != nullptr && local->register_) {
        fail("can't take the address of `" + v->name +
             "`: declared `register` so it lives in a 6502 register, not memory (C99 "
             "§6.5.3.2.1)");
      }
      if (asdl::isa<ir::FunType>(sym->type)) {
        const Type fun = sym->type;
        v->data_type = fun;
        Type result = ir::Pointer{.referenced_type = fun};
        e.data_type = result;
        return result;
      }
    }
  }
  if (auto* d = std::get_if<ir::Dereference>(&inner)) {
    tc.check_exp(*d->exp);
    decay_in_place(d->exp);
    const std::optional<Type> ptr_type = checked_type(*d->exp);
    if (!opt_pointer(ptr_type)) {
      fail("unary '*' requires a pointer operand, got " + class_name(ptr_type));
    }
    const Type stripped = strip_quals(*ptr_type);
    d->data_type = *std::get<ir::Pointer>(stripped).referenced_type;
    e.data_type = stripped;
    return stripped;
  }
  const Type t_inner = tc.check_exp(inner);
  Type result = ir::Pointer{.referenced_type = t_inner};
  e.data_type = result;
  return result;
}

}  // namespace

Type TypeChecker::pointer_equality_common_type(Exp& lhs, Exp& rhs, const Type& tl,
                                               const Type& tr) {
  const bool l_ptr = is_pointer_type(tl);
  const bool r_ptr = is_pointer_type(tr);
  const Type& tl_p = strip_quals(tl);
  const Type& tr_p = strip_quals(tr);
  if (l_ptr && r_ptr) {
    auto common = composite_pointer(tl, tr);
    if (!common) {
      fail("comparison of distinct pointer types: " + asdl::py_repr(tl) + " vs " +
           asdl::py_repr(tr));
    }
    return std::move(*common);
  }
  if (l_ptr && (is_null_pointer_constant(rhs) || is_nullptr_type(tr))) {
    return ir::Pointer{.referenced_type = *std::get<ir::Pointer>(tl_p).referenced_type};
  }
  if (r_ptr && (is_null_pointer_constant(lhs) || is_nullptr_type(tl))) {
    return ir::Pointer{.referenced_type = *std::get<ir::Pointer>(tr_p).referenced_type};
  }
  fail("comparison between pointer and non-null-constant non-pointer: " + asdl::py_repr(tl) +
       " vs " + asdl::py_repr(tr));
}

std::optional<Type> TypeChecker::check_nullptr_binary(c99_ast::Binary& exp, const Type& tl,
                                                      const Type& tr) {
  const std::string op_name(binary_op_spelling(exp.op));
  const O op = ir::kind(exp.op);
  if (is_equality(op)) {
    if (is_pointer_type(tl) || is_pointer_type(tr)) {
      return std::nullopt;
    }
    if ((is_nullptr_type(tl) && is_nullptr_type(tr)) ||
        (is_nullptr_type(tl) && is_c23_null_pointer_constant(*exp.right)) ||
        (is_nullptr_type(tr) && is_c23_null_pointer_constant(*exp.left))) {
      const Type void_p = ir::Pointer{.referenced_type = ir::Void{}};
      convert_in_place(exp.left, void_p);
      convert_in_place(exp.right, void_p);
      exp.data_type = Type(ir::Int{});
      return Type(ir::Int{});
    }
    fail("binary '" + op_name + "' between " + c_type(tl) + " and " + c_type(tr) +
         ": a nullptr_t compares only with another nullptr_t, a null pointer constant, or a "
         "pointer (C23 §6.5.10p2)");
  }
  if (is_relational(op)) {
    fail("binary '" + op_name +
         "' is not defined on nullptr_t: the relational operators compare real types or "
         "pointers to object types (C23 §6.5.9p2)");
  }
  fail("binary '" + op_name + "' is not defined on nullptr_t (" + c_type(tl) + " " + op_name +
       " " + c_type(tr) + ")");
}

Type TypeChecker::check_exp(Exp& exp) {
  switch (ir::kind(exp)) {
    case E::Constant: {
      auto& c = std::get<ir::Constant>(exp);
      const Type t = constant_type(c.const_);
      c.data_type = t;
      return t;
    }
    case E::String:
      return need(checked_type(exp), "string");
    case E::Cast:
      return check_cast(*this, std::get<ir::Cast>(exp));
    case E::Var:
      return check_var(*this, std::get<ir::Var>(exp));
    case E::CompoundLiteral:
      return check_compound_literal(std::get<ir::CompoundLiteral>(exp));
    case E::Unary:
      return check_unary(*this, std::get<ir::Unary>(exp));
    case E::Binary:
      return check_binary(*this, std::get<ir::Binary>(exp));
    case E::CompoundAssignment:
      return check_compound_assignment(*this, std::get<ir::CompoundAssignment>(exp));
    case E::Assignment:
      return check_assignment(*this, std::get<ir::Assignment>(exp));
    case E::Postfix: {
      auto& p = std::get<ir::Postfix>(exp);
      return check_incdec(*this, p.operand, p.data_type);
    }
    case E::Prefix: {
      auto& p = std::get<ir::Prefix>(exp);
      return check_incdec(*this, p.operand, p.data_type);
    }
    case E::Conditional:
      return check_conditional(*this, std::get<ir::Conditional>(exp));
    case E::Comma:
      return check_comma(*this, std::get<ir::Comma>(exp));
    case E::FunctionCall:
      return check_function_call(*this, std::get<ir::FunctionCall>(exp));
    case E::IndirectCall:
      return check_indirect_call(*this, std::get<ir::IndirectCall>(exp));
    case E::Dereference:
      return check_dereference(*this, std::get<ir::Dereference>(exp));
    case E::InitList:
      fail("brace-enclosed initializer (`{...}`) is only valid as a variable initializer for an "
           "array");
    case E::Designation:
      fail("a designator such as `[2] =` or `.x =` goes only in front of an item of a braced "
           "initializer");
    case E::SizeOfExp:
      return check_sizeof_exp(*this, std::get<ir::SizeOfExp>(exp));
    case E::SizeOfType:
      return check_sizeof_type(*this, std::get<ir::SizeOfType>(exp));
    case E::Subscript:
      return check_subscript(*this, std::get<ir::Subscript>(exp));
    case E::Dot:
      return check_dot(*this, std::get<ir::Dot>(exp));
    case E::Arrow:
      return check_arrow(*this, std::get<ir::Arrow>(exp));
    case E::AddressOf:
      return check_address_of(*this, std::get<ir::AddressOf>(exp));
  }
  throw InternalError("unexpected exp: " + asdl::py_repr(exp));
}

}  // namespace c6502::passes::type_checking
