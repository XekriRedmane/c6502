// Part T-D of the port of passes/type_checking.py: the checker's
// expressions, type_checking.py:4776-6197, which are `check_exp` with
// its cases, the common type of a pointer equality, and the binary
// operators with a `nullptr_t` operand.
//
// `check_exp` type-checks an expression in place. It stamps the
// `data_type` of every node it visits and returns the type it computed,
// which callers such as `check_block_var` compare with a declared type.
// Where operand types disagree, the narrower operand is wrapped in an
// implicit `Cast`, so TAC sees a tree in which every operand has its
// own type and every conversion that changes a size is an explicit cast
// node. The wrapping replaces the parent's child fields (`left`,
// `right`, `true_clause`, `false_clause`, and so on), which the C++
// replaces through the child's box.
//
// Python gives a `String` node a `data_type` attribute when it checks
// one, which the dataclass has no field for and the dump format doesn't
// show. The C++ `String` has no such field, so `checked_type` computes
// the type a checked `String` holds, `char[N+1]`, and `decay` gives one
// the `AddressOf` that Python's `_decay_if_array` would. string_lifting
// lifts every literal that isn't a character array's initializer, so a
// `String` reaches these sites only in a program that skipped it.
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

// Returns Python's `type(t).__name__` for a type, or "NoneType".
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

// Returns the type a checked expression holds: its `data_type`, or for
// a `String`, the `char[N+1]` Python stamps on it, with N the number of
// code points.
std::optional<Type> checked_type(const Exp& exp) {
  if (const auto* s = std::get_if<ir::String>(&exp)) {
    return ir::Array{.element_type = ir::Char{},
                     .size = static_cast<long long>(pyrepr::code_points(s->str).size()) + 1};
  }
  return data_type_of(exp);
}

// Returns `decay_if_array(exp)`, with a `String` decayed as Python's
// `_decay_if_array` decays the `char[N+1]` it stamps on one.
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

// The predicates of type_checking_types.h on a type that may be none, as
// Python's give False for None.
bool opt_object(const std::optional<Type>& t) { return t && is_object_type(*t); }
bool opt_pointer(const std::optional<Type>& t) { return t && is_pointer_type(*t); }
bool opt_integer(const std::optional<Type>& t) { return t && is_integer_type(*t); }
bool opt_floating(const std::optional<Type>& t) { return t && is_floating_type(*t); }
bool opt_arithmetic(const std::optional<Type>& t) { return t && is_arithmetic_type(*t); }
bool opt_void(const std::optional<Type>& t) { return t && is_void(*t); }
bool opt_struct_or_union(const std::optional<Type>& t) { return t && is_struct_or_union(*t); }

const Type& need(const std::optional<Type>& t, const char* where) {
  if (!t) {
    throw InternalError(std::string("AttributeError: no type in ") + where);
  }
  return *t;
}

bool is_equality(O op) { return op == O::Equal || op == O::NotEqual; }

bool is_relational(O op) {
  return op == O::LessThan || op == O::GreaterThan || op == O::LessOrEqual ||
         op == O::GreaterOrEqual;
}

// Returns the type a constant has. Each constant's variant gives its
// type, since the parser chose the variant from the suffix, the base,
// and the value (C99 §6.4.4.1, §6.4.4.2).
Type constant_type(const ir::Type_const& c) {
  using C = ir::Kind_const;
  switch (ir::kind(c)) {
    case C::ConstInt:
      return ir::Int{};
    case C::ConstLong:
      return ir::Long{};
    case C::ConstLongLong:
      return ir::LongLong{};
    case C::ConstUInt:
      return ir::UInt{};
    case C::ConstULong:
      return ir::ULong{};
    case C::ConstULongLong:
      return ir::ULongLong{};
    case C::ConstChar:
      return ir::SChar{};
    case C::ConstUChar:
      return ir::UChar{};
    // `true` and `false` (C23 §6.4.4.6).
    case C::ConstBool:
      return ir::Bool{};
    // `nullptr` (C23 §6.4.4.6p4, §7.21.2).
    case C::ConstNullptr:
      return ir::NullptrT{};
    case C::ConstFloat:
      return ir::Float{};
    case C::ConstDouble:
      return ir::Double{};
    // `wb` and `uwb` (C23 §6.4.4.2p6).
    case C::ConstBitInt: {
      const auto& b = std::get<ir::ConstBitInt>(c);
      return ir::BitInt{.width = b.width, .is_signed = b.is_signed};
    }
  }
  throw InternalError("unexpected const: " + asdl::py_repr(c));
}

// Checks a cast. Its target is a scalar object type or `void`, the
// discard form `(void)e` (C99 §6.5.4p2). The parser rejects array and
// function targets.
Type check_cast(TypeChecker& tc, ir::Cast& e) {
  const Type target = e.target_type;
  if (!(is_object_type(target) || is_void(target))) {
    fail("cast target type must be an object type (Int / Long / LongLong / UInt / ULong / "
         "ULongLong / Float / Double / Pointer) or void, got " +
         asdl::py_repr(target));
  }
  // A malformed target such as `(void(*)[3])e` is rejected, since an
  // array's element type has to be complete even inside a pointer.
  tc.check_well_formed(target, "cast target");
  tc.check_exp(*e.exp);
  // An array operand decays first, so `(int *)arr` casts the pointer.
  decay_in_place(e.exp);
  const Exp& inner = *e.exp;
  const std::optional<Type> inner_type = checked_type(inner);
  // The operand of a cast is evaluated even for `(void)e` (C99
  // §6.3.2.2p1), so its value must exist, which rules out an incomplete
  // struct or union.
  tc.require_complete_value(inner, "cast operand");
  // `(void)e` takes an operand of any type, a void call among them, and
  // discards it, so the checks of the operand's shape don't apply.
  if (is_void(target)) {
    e.data_type = target;
    return target;
  }
  if (!opt_object(inner_type)) {
    fail("cannot cast non-object type " + repr_or_none(inner_type) + " to " +
         asdl::py_repr(target));
  }
  const Type& src = *inner_type;
  // C23 §6.5.5p4: "The type nullptr_t shall not be converted to any type
  // other than void, bool or a pointer type. If the target type is
  // nullptr_t, the cast expression shall be a null pointer constant or
  // have type nullptr_t."
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
  // C99 §6.3.2.3 defines only the conversions between pointers and
  // integers and between pointers. An address isn't an arithmetic value,
  // so a conversion between a pointer and a floating type has no meaning.
  const bool src_fp = is_floating_type(src);
  const bool tgt_fp = is_floating_type(target);
  const bool src_ptr = is_pointer_type(src);
  const bool tgt_ptr = is_pointer_type(target);
  if ((src_fp && tgt_ptr) || (src_ptr && tgt_fp)) {
    fail("cannot cast between pointer and floating-point type: " + asdl::py_repr(src) + " → " +
         asdl::py_repr(target));
  }
  // `offsetof(type, member)` from <stddef.h> is the address of a member
  // of an object at address 0, converted to an integer. The address is a
  // constant, so the cast is an integer constant expression.
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
    // The only use of a function name that isn't a direct call takes
    // its address, and an indirect call can't know that the result is
    // in C.
    fail("can't take the address of " + pyrepr::string(e.name) +
         ", which returns its result in the carry flag");
  }
  // A function name is a function designator. Its FunType stays on the
  // node, and `decay_if_array` makes it a pointer at every operand site
  // (C99 §6.3.2.1p4). A site that doesn't decay its operand rejects the
  // FunType as a type that isn't an object type.
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
      throw InternalError("KeyError: " + asdl::py_repr(op));
  }
}

Type check_unary(TypeChecker& tc, ir::Unary& e) {
  tc.check_exp(*e.exp);
  // The array decays (C99 §6.3.2.1p3), so `!arr` and `!"abc"` test a
  // pointer, which meets `!`'s requirement of a scalar operand. `&arr`
  // and `*arr` are nodes of their own and don't come here.
  decay_in_place(e.exp);
  std::optional<Type> t = checked_type(*e.exp);
  if (!opt_object(t)) {
    fail("unary operator on non-object type " + repr_or_none(t));
  }
  const bool logical_not = asdl::isa<ir::LogicalNot>(e.op);
  // `nullptr_t` is scalar, so `!nullptr` is 1, but `-`, `+`, and `~`
  // need an arithmetic or integer operand (C23 §6.5.4.3p1).
  if (is_nullptr_type(*t) && !logical_not) {
    fail("unary '" + unary_spelling(e.op) + "' is not defined on nullptr_t (C23 §6.5.4.3)");
  }
  // `-` and `+` need an arithmetic operand and `~` an integer one, so
  // each rejects a pointer (C99 §6.5.3.3p1), and `~` rejects a floating
  // operand too (§6.5.3.3p4, "The operand of the unary ~ operator shall
  // have integer type."). `!` takes any scalar and gives an `int`
  // (§6.5.3.3p5), so `!p` is `p == 0`, the test of a null pointer.
  if (!logical_not && is_pointer_type(*t)) {
    fail("unary '" + unary_spelling(e.op) + "' is not defined on pointer type " +
         asdl::py_repr(*t));
  }
  if (asdl::isa<ir::Complement>(e.op) && is_floating_type(*t)) {
    fail("unary '~' requires integer operand, got " + class_name(t));
  }
  // The operand of `-`, `~`, and `+` is promoted (C99 §6.3.1.1p2,
  // §6.5.3.3p4). `!` compares its operand with zero, and its result is
  // always `int`, so it doesn't promote.
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

// Checks the additive operators with a pointer operand (C99 §6.5.6),
// which take their own path, since `common_type` has no pointer to make.
// The legal forms are `ptr + int` and `int + ptr`, which give the
// pointer offset by a number of elements, `ptr - int`, and `ptr - ptr`
// of one type, which gives the count of elements as an `int`, c6502's
// stand-in for `ptrdiff_t`, 2 signed bytes as wide as an address. Any
// other form (`ptr + ptr`, `int - ptr`, a pointer and a floating value,
// two pointers of different types, or arithmetic on a pointer to a
// function) is a constraint violation.
Type check_pointer_additive(TypeChecker& tc, ir::Binary& e, const Type& tl, const Type& tr) {
  const bool add = asdl::isa<ir::Add>(e.op);
  const std::string op_name = add ? "+" : "-";
  const std::string operands = "(" + asdl::py_repr(tl) + " " + op_name + " " + asdl::py_repr(tr) + ")";
  // The qualifiers of a pointer itself don't change whether arithmetic
  // on it is legal.
  const Type tl_p = strip_quals(tl);
  const Type tr_p = strip_quals(tr);
  // §6.5.6p2 requires a pointer to a complete object type for the
  // additive operators, and neither `void` nor a function has a size.
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
    // `ptr + ptr` is illegal, and `ptr - ptr` is legal when the pointee
    // types match, without their qualifiers, so `int *` and `int *
    // const` subtract.
    if (add) {
      fail("binary '+' is not defined on two pointer operands (" + asdl::py_repr(tl) + " + " +
           asdl::py_repr(tr) + ")");
    }
    if (!types_equal(strip_quals(*std::get<ir::Pointer>(tl_p).referenced_type),
                     strip_quals(*std::get<ir::Pointer>(tr_p).referenced_type))) {
      fail("subtraction of distinct pointer types: " + asdl::py_repr(tl) + " - " +
           asdl::py_repr(tr));
    }
    // The operands keep their types, since both are 2 bytes and the
    // subtraction is an ordinary 2-byte one. c99_to_tac divides the
    // difference in bytes by the pointee's size to give the count of
    // elements.
    e.data_type = Type(ir::Int{});
    return ir::Int{};
  }
  // One operand is a pointer, and the other is an integer. `int - ptr`
  // isn't defined.
  const bool int_is_left = is_integer_type(tl);
  if (int_is_left && !add) {
    fail("binary '-' between integer and pointer is not defined (" + asdl::py_repr(tl) + " - " +
         asdl::py_repr(tr) + ")");
  }
  // The integer widens to `int`, 2 bytes, so that the add lines up with
  // the pointer's width. c99_to_tac scales it by the pointee's size
  // before the add. Before C99's widths, this widened to `long`, which
  // is now 4 bytes, wider than a pointer.
  const Type* ptr_type = nullptr;
  if (int_is_left) {
    convert_in_place(e.left, ir::Int{});
    ptr_type = &tr_p;
  } else {
    convert_in_place(e.right, ir::Int{});
    ptr_type = &tl_p;
  }
  // The result is the pointer type without its own qualifiers, since an
  // rvalue isn't qualified (§6.3.2.1p2). The pointee's qualifiers stay.
  Type result = ir::Pointer{.referenced_type = *std::get<ir::Pointer>(*ptr_type).referenced_type};
  e.data_type = result;
  return result;
}

Type check_binary(TypeChecker& tc, ir::Binary& e) {
  tc.check_exp(*e.left);
  tc.check_exp(*e.right);
  // Both operands decay (C99 §6.3.2.1p3) before anything dispatches on
  // their types, so an array's name acts as a pointer to its first
  // element.
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
  // `&&` and `||` test each operand against zero by itself (C99 §6.5.13,
  // §6.5.14), with no usual arithmetic conversions, so a pointer is
  // legal and true when it isn't null. Every scalar operand passed the
  // test above, and the function type, the only type that isn't scalar
  // and isn't rejected there, is an object type nowhere.
  if (op == O::LogicalAnd || op == O::LogicalOr) {
    e.data_type = Type(ir::Int{});
    return ir::Int{};
  }
  if (is_nullptr_type(tl) || is_nullptr_type(tr)) {
    if (auto result = tc.check_nullptr_binary(e, tl, tr)) {
      return *result;
    }
  }
  // Pointer equality (C99 §6.5.9p2) has rules of its own: a matching
  // pointer type is fine, and so is a pointer and a null pointer
  // constant, and `common_type` has no pointer to make.
  if (is_equality(op) && (is_pointer_type(tl) || is_pointer_type(tr))) {
    const Type common = tc.pointer_equality_common_type(*e.left, *e.right, tl, tr);
    convert_in_place(e.left, common);
    convert_in_place(e.right, common);
    e.data_type = Type(ir::Int{});
    return ir::Int{};
  }
  // Pointer ordering (C99 §6.5.8p2) is stricter than equality. Both
  // operands must be pointers to compatible object types, a null pointer
  // constant isn't allowed, and a pointer with an integer or floating
  // value is a constraint violation. The result is an `int`
  // (§6.5.8p6).
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
  // The multiplicative operators take arithmetic operands, and `%`
  // integer ones (C99 §6.5.5), so a pointer operand is rejected here
  // instead of failing in `common_type`.
  if ((op == O::Multiply || op == O::Divide || op == O::Modulo) &&
      (is_pointer_type(tl) || is_pointer_type(tr))) {
    const std::string op_name(binary_op_spelling(e.op));
    fail("binary '" + op_name + "' is not defined on pointer operands (" + asdl::py_repr(tl) +
         " " + op_name + " " + asdl::py_repr(tr) + ")");
  }
  if ((op == O::Add || op == O::Subtract) && (is_pointer_type(tl) || is_pointer_type(tr))) {
    return check_pointer_additive(tc, e, tl, tr);
  }
  // `%`, the bitwise operators, and the shifts take integer operands
  // (C99 §6.5.5p2, §6.5.7p2, §6.5.10p2, §6.5.11p2, §6.5.12p2), so a
  // floating or pointer operand is rejected. A pointer operand of `&`,
  // `|`, or `^` once reached `common_type` and crashed there, or reached
  // c99_to_tac, which has no width for the result (`10 & ptr`, `"s" &
  // 100`).
  if ((op == O::Modulo || op == O::BitwiseAnd || op == O::BitwiseOr || op == O::BitwiseXor ||
       op == O::LeftShift || op == O::RightShift) &&
      (is_floating_type(tl) || is_floating_type(tr) || is_pointer_type(tl) ||
       is_pointer_type(tr))) {
    const std::string op_name(binary_op_spelling(e.op));
    fail("binary '" + op_name + "' requires integer operands (" + asdl::py_repr(tl) + " " +
         op_name + " " + asdl::py_repr(tr) + ")");
  }
  // The integer promotions (C99 §6.3.1.1p2) come before the usual
  // arithmetic conversions. In c6502 a promotion of a character type is a
  // cast of one width, which c99_to_tac's lowering of casts drops.
  convert_in_place(e.left, promote_integer(tl));
  convert_in_place(e.right, promote_integer(tr));
  tl = need(checked_type(*e.left), "binary left operand");
  tr = need(checked_type(*e.right), "binary right operand");
  // A shift's operands are promoted each by itself, and the result has
  // the promoted left operand's type (C99 §6.5.7p3), with no usual
  // arithmetic conversions. tac_to_asm passes only the low byte of the
  // right operand to the shift helpers, since a count as large as the
  // width is undefined, so a right operand wider than the left one, as
  // in `ulong << longlong`, doesn't matter.
  if (op == O::LeftShift || op == O::RightShift) {
    e.data_type = tl;
    return tl;
  }
  // The usual arithmetic conversions (C99 §6.3.1.8) wrap the narrower
  // operand in a cast to the common type, so the operation happens at
  // one width.
  const Type common = common_type(tl, tr);
  convert_in_place(e.left, common);
  convert_in_place(e.right, common);
  // An arithmetic or bitwise operator gives the common type, and a
  // comparison or a logical operator gives `int` (§6.5.8p6, §6.5.9p3,
  // §6.5.13p3, §6.5.14p3).
  const Type result = is_truth_op(e.op) ? Type(ir::Int{}) : common;
  e.data_type = result;
  return result;
}

// Checks `lval OP= rval`. Both sides are checked, and then the rule of
// the binary operator (the integer promotions, and the usual arithmetic
// conversions except for a shift, §6.5.7p3) gives the intermediate type
// at which the operation happens. The right operand is converted to that
// type, so c99_to_tac reads it directly, and the left keeps its own
// type. c99_to_tac converts the loaded value to the intermediate type
// before the operation and the result back to the left operand's type
// before the store. The expression has the left operand's type, since
// its value is the left operand's new value.
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
  // C23 §6.5.17.3p1-2: the operands are arithmetic, or a pointer and an
  // integer.
  if (is_nullptr_type(tl) || is_nullptr_type(tr0)) {
    fail("compound assignment is not defined on nullptr_t (" + c_type(tl) + ", " + c_type(tr0) +
         "; C23 §6.5.17.3)");
  }
  // The right operand may be an array, which decays. The left one can't
  // be, as checked above.
  decay_in_place(e.rval);
  const std::optional<Type> tr = checked_type(*e.rval);
  // C99 §6.5.16.2p1: unless the left operand is a pointer (`p += n` and
  // `p -= n`, checked below), both operands have arithmetic type. This
  // rejects a function name on the right, which decays to a function
  // pointer, and any other pointer operand.
  if (!asdl::isa<ir::Pointer>(strip_quals(tl)) && !opt_arithmetic(tr)) {
    fail("compound assignment needs an arithmetic right operand, got " + repr_or_none(tr));
  }
  const O op = ir::kind(e.op);
  const std::string operands = "(" + asdl::py_repr(tl) + ", " + repr_or_none(tr) + ")";
  // The constraints by operator mirror those of the binary operators.
  // `%`, the bitwise operators, and the shifts take integer operands, and
  // a shift rejects a pointer too, since it skips `common_type`, which
  // the others reach.
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
  // C99 §6.5.16.2p1: a pointer left operand takes only `+=` and `-=`.
  // `x /= y` and `ptr &= 0` on pointers once reached `common_type` or
  // c99_to_tac and crashed there.
  const bool additive = op == O::Add || op == O::Subtract;
  if (is_pointer_type(tl) && !additive) {
    fail("compound `" + asdl::py_repr(e.op) + "` is not defined on a pointer left operand " +
         operands);
  }
  // `ptr += int` and `ptr -= int` are `ptr + int` and `ptr - int`
  // (C99 §6.5.16.2). The integer widens to `int`, as wide as a pointer.
  // c99_to_tac takes the path of pointer arithmetic before it reads the
  // intermediate type.
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
  // A shift's intermediate type is the promoted left operand. The right
  // keeps its own promoted type, and c99_to_tac passes only its low byte
  // to the shift helpers.
  if (op == O::LeftShift || op == O::RightShift) {
    if (!(trv == tr_p)) {
      convert_in_place(e.rval, tr_p);
    }
    e.intermediate_type = tl_p;
    e.data_type = tl;
    return tl;
  }
  // The other operators convert to the common type, and the right operand
  // is converted to it so c99_to_tac reads it at that width. The result
  // goes back to the left operand's type for the store.
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
  // `*p = e` with a `void *p` assigns to an expression of type void,
  // which isn't an lvalue (C99 §6.3.2.1p1). With a void `e` too, as in
  // `*x = foo();`, `convert_to` saw two equal types and let it through
  // to c99_to_tac, which stored no value.
  if (is_void(tl)) {
    fail("cannot assign to an expression of type void: " + asdl::py_repr(*e.lval));
  }
  // An assignment needs a modifiable lvalue on its left (C99
  // §6.5.16.1), and a const-qualified one isn't modifiable
  // (§6.3.2.1p1). The qualifiers of pointees aren't compared here, as
  // gcc's `-Wno-discarded-qualifiers` doesn't, so `int *q = (const int
  // *)p;` passes, and an explicit cast says the discard is meant.
  if (is_const_qualified(tl)) {
    fail("cannot assign to const-qualified lvalue: " + asdl::py_repr(*e.lval) + " has type " +
         asdl::py_repr(tl));
  }
  // Both sides read or write a struct's bytes, so neither may have an
  // incomplete type.
  tc.require_complete_value(*e.lval, "assignment lval");
  tc.require_complete_value(*e.rval, "assignment rval");
  // An array isn't a modifiable lvalue (C99 §6.5.16.1). A subscript or
  // a dereference has its element type here, so this catches only
  // `arr_name = ...`.
  if (is_array_type(tl)) {
    fail("cannot assign to an array (use subscript): " + asdl::py_repr(*e.lval));
  }
  // A struct or union assigns when both sides have the same tag and
  // kind, without `convert_to` and the decay of the right side.
  if (is_struct_or_union(tl)) {
    if (!types_equal(strip_quals(tl), strip_quals(tr))) {
      fail("struct/union assignment between distinct types: " + asdl::py_repr(tl) + " = " +
           asdl::py_repr(tr));
    }
    e.data_type = tl;
    return tl;
  }
  // The right side decays (`int *p = arr;` is the common case), and the
  // assignment converts it to the left side's type (C99 §6.5.16.1).
  // Compound assignments are nodes of their own, with an intermediate
  // type, and don't come here.
  decay_in_place(e.rval);
  convert_in_place(e.rval, tl);
  e.data_type = tl;
  return tl;
}

// Checks `++` and `--`, which modify their operand, so it must be a
// modifiable lvalue (C99 §6.5.2.4p1, §6.5.3.1p1), which a const one
// isn't (§6.3.2.1p1).
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
  // `p++` steps by one element, which needs a complete pointee, as the
  // binary `+` and `-` do (§6.5.6p2). `void` has no size.
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
  // Each branch decays by itself, so in `cond ? arr : ptr` both are
  // pointers.
  decay_in_place(e.true_clause);
  decay_in_place(e.false_clause);
  std::optional<Type> tt = checked_type(*e.true_clause);
  std::optional<Type> tf = checked_type(*e.false_clause);
  // C99 §6.5.15p5: "If both the second and third operands have void
  // type, the result has void type." Neither branch is converted, and
  // the conditional only sequences their side effects.
  if (opt_void(tt) && opt_void(tf)) {
    e.data_type = Type(ir::Void{});
    return ir::Void{};
  }
  if (opt_void(tt) || opt_void(tf)) {
    fail("conditional branches must both be void or both be value-producing; got " +
         repr_or_none(tt) + " vs " + repr_or_none(tf));
  }
  // Both branches of a struct or union type must have the same type
  // (C99 §6.5.15p3, "both operands have compatible structure or union
  // types"), and it must be complete, since an incomplete struct has no
  // value. The result is that type, with no conversion.
  if (opt_struct_or_union(tt) || opt_struct_or_union(tf)) {
    // The result is an rvalue, so each branch's qualifiers go (C99
    // §6.3.2.1p2).
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
  // C99 §6.5.15p6 takes the pointer cases first, since `common_type`
  // knows only arithmetic types. Two pointers must have one type, or one
  // of them is `void *`. A pointer and a null pointer constant give the
  // pointer. Anything else goes to the usual arithmetic conversions.
  const bool tt_ptr = is_pointer_type(*tt);
  const bool tf_ptr = is_pointer_type(*tf);
  // C23 §6.5.16p3 allows a `nullptr_t` branch with another `nullptr_t`,
  // which gives a `nullptr_t`, or with a pointer, which gives the
  // pointer's type (§6.5.16p7). Footnote 109: "If a second or third
  // operand of type nullptr_t is used and the other operand is not a
  // pointer and does not have type nullptr_t itself, a constraint is
  // violated even if that other operand is a null pointer constant such
  // as 0."
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
      // `void *` and any object pointer meet at `void *` (C99
      // §6.5.15p6).
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
    // The integer promotions (C99 §6.3.1.1p2) come before the usual
    // arithmetic conversions, as for the binary operators.
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

// Checks `left, right` (C99 §6.5.17), which evaluates `left` for its
// side effects and then `right`, whose type and value it has. Neither
// side is converted. `left`'s value is discarded, as an expression
// statement's is, so an incomplete struct or union isn't usable there.
Type check_comma(TypeChecker& tc, ir::Comma& e) {
  tc.check_exp(*e.left);
  tc.require_complete_value(*e.left, "left operand of comma operator");
  tc.check_exp(*e.right);
  decay_in_place(e.right);
  const std::optional<Type> t = checked_type(*e.right);
  e.data_type = t;
  return need(t, "comma right operand");
}

// Checks a call by name. The callee is a function (`foo()`) or a
// function pointer (`fp()`). C99 §6.5.2.2 requires a pointer to a
// function, and §6.3.2.1p4 decays a function name to one, so both
// shapes give the `FunType` the arguments and the result are checked
// against. Any other type is an error.
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

// Checks `*e`, whose operand must be a pointer and whose type is the
// pointee (C99 §6.5.3.2p4). Whether the result is an lvalue is a matter
// of the node, which assignment, `&`, and `++` accept, and the type
// doesn't record it.
Type check_dereference(TypeChecker& tc, ir::Dereference& e) {
  tc.check_exp(*e.exp);
  // `*arr` decays the array first, so it reads `arr[0]`, as `*(arr + 0)`
  // does.
  decay_in_place(e.exp);
  const std::optional<Type> t_inner = checked_type(*e.exp);
  if (!opt_pointer(t_inner)) {
    fail("unary '*' requires a pointer operand, got " + class_name(t_inner));
  }
  // `int * const p; *p` reads the int through p, so the pointer's own
  // qualifiers go. The pointee's stay, which makes `*p` a const lvalue
  // for a `const int *p`.
  const Type pointee = *std::get<ir::Pointer>(strip_quals(*t_inner)).referenced_type;
  const Type& pointee_uq = strip_quals(pointee);
  // The value behind a pointer to an incomplete struct or union has no
  // size, no members, and no storage to address. `&*p`, which is `p`
  // (C99 §6.5.3.2p3), is the one shape where an incomplete pointee is
  // fine, and the `AddressOf` case checks it without coming here.
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

// Checks `sizeof e`. The operand is checked, which stamps its types and
// evaluates nothing, so C99 §6.5.3.4p2's rule that the operand isn't
// evaluated holds, and c99_to_tac doesn't translate it. An array operand
// doesn't decay (§6.3.2.1p3), since `sizeof` of an array is the array's
// size, and `check_exp` leaves the outer node undecayed.
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
  // An array of unknown size (`extern T a[];`) is incomplete, which C99
  // §6.5.3.4p1 rejects. The operand of `sizeof` is exempt from decay,
  // and not from that rule.
  if (const auto* a = std::get_if<ir::Array>(&strip_quals(*inner_t)); a != nullptr && a->size == 0) {
    fail("sizeof of an incomplete array type is illegal (C99 §6.5.3.4.1)");
  }
  // The result is a `size_t`, which is `unsigned int` in c6502, since no
  // object is larger than the 64K address space.
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
  // An array's element type must be complete, as in `sizeof (void[3])`,
  // and a struct or union must be complete and in scope.
  tc.check_well_formed(t, "sizeof type",
                       asdl::isa<ir::Structure, ir::Union, ir::Array>(strip_quals(t)));
  // `size_t`, which is `unsigned int` in c6502, as for `sizeof e`.
  e.data_type = Type(ir::UInt{});
  return ir::UInt{};
}

// Checks `E1[E2]`, which C99 §6.5.2.1p2 defines as `*((E1)+(E2))`. The
// `+` of a pointer and an integer commutes, so one operand must be a
// pointer (or an array, which decays) and the other an integer, either
// way round, and `arr[3]` and `3[arr]` are both valid C. The operands
// are put in the order (pointer, integer), so c99_to_tac's pointer
// arithmetic, stores, and `&` see one shape.
Type check_subscript(TypeChecker& tc, ir::Subscript& e) {
  tc.check_exp(*e.array);
  tc.check_exp(*e.index);
  decay_in_place(e.array);
  decay_in_place(e.index);
  const std::optional<Type> ta = checked_type(*e.array);
  const std::optional<Type> ti = checked_type(*e.index);
  if (opt_pointer(ta) && opt_integer(ti)) {
    // In order.
  } else if (opt_pointer(ti) && opt_integer(ta)) {
    // `int[ptr]`, swapped so that `array` holds the pointer.
    std::swap(*e.array, *e.index);
  } else {
    fail("subscript needs a pointer/array operand and an integer operand; got " +
         repr_or_none(ta) + " and " + repr_or_none(ti));
  }
  // `int * const p; p[i]` has a const pointer, and the element isn't
  // const for it, so the pointer's own qualifiers go. The pointee's
  // qualifiers stay in the result.
  const Type ptr_type_uq = strip_quals(need(checked_type(*e.array), "subscript pointer"));
  const Type& referenced = *std::get<ir::Pointer>(ptr_type_uq).referenced_type;
  if (asdl::isa<ir::FunType>(referenced)) {
    fail("subscript of pointer-to-function is not supported");
  }
  // `v[0]` on a `void *` is `*(v + 0)`, pointer arithmetic on a pointer
  // to void (C99 §6.5.2.1p1).
  if (is_void(referenced)) {
    tc.require_complete_pointee(ptr_type_uq, "subscript");
  }
  // The index widens to `int`, as wide as a pointer, so c99_to_tac
  // computes the address with one 2-byte add.
  convert_in_place(e.index, ir::Int{});
  e.data_type = referenced;
  return referenced;
}

// Checks `e.m` (C99 §6.5.2.3p1), whose operand must be a struct or union
// and whose type is the member's. A qualified operand qualifies the
// result (§6.5.2.3p3, "If the first expression has qualified type, the
// result has the so-qualified version of the type of the designated
// member.").
Type check_dot(TypeChecker& tc, ir::Dot& e) {
  tc.check_exp(*e.operand);
  const Type t_op = need(checked_type(*e.operand), "member access operand");
  const MemberInfo m = tc.lookup_member(t_op, e.member, "member access '." + e.member + "'");
  const Type result = propagate_quals(m.type, t_op);
  e.data_type = result;
  return result;
}

// Checks `p->m` (C99 §6.5.2.3p2), whose operand must be a pointer to a
// struct or union. It is `(*p).m`, so the pointee's qualifiers reach the
// member. `const struct S *p; p->m` makes `p->m` const, and `struct S *
// const p; p->m` doesn't.
Type check_arrow(TypeChecker& tc, ir::Arrow& e) {
  tc.check_exp(*e.operand);
  decay_in_place(e.operand);
  const std::optional<Type> t_op = checked_type(*e.operand);
  // `int * const p; p->m` drops the pointer's own qualifiers before
  // reaching the pointee.
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

// Checks `&e`, whose type is a pointer to the operand's (C99
// §6.5.3.2p3). identifier_resolution checks that the operand is an
// lvalue. A function's name is the one operand of `&` that has a
// function type, which gives a function pointer, so that case doesn't
// go through the operand's check.
Type check_address_of(TypeChecker& tc, ir::AddressOf& e) {
  Exp& inner = *e.exp;
  if (const auto* cl = std::get_if<ir::CompoundLiteral>(&inner)) {
    if (cl->storage_class && asdl::isa<ir::Register>(*cl->storage_class)) {
      fail("can't take the address of a `register` compound literal (C99 §6.5.3.2.1)");
    }
  }
  if (auto* v = std::get_if<ir::Var>(&inner)) {
    if (const Symbol* sym = tc.symbols.find(v->name)) {
      // C99 §6.5.3.2p1: the operand of `&` shall not be declared with
      // the `register` storage class.
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
  // `&*p` is `p` (C99 §6.5.3.2p3), so the pointer operand is checked
  // directly, past the dereference's test of an incomplete pointee,
  // which would reject `&*p` on a pointer to an incomplete struct. The
  // dereference's type is still stamped, so the tree describes itself.
  if (auto* d = std::get_if<ir::Dereference>(&inner)) {
    const Type ptr_type = tc.check_exp(*d->exp);
    if (is_pointer_type(ptr_type)) {
      const Type stripped = strip_quals(ptr_type);
      d->data_type = *std::get<ir::Pointer>(stripped).referenced_type;
      e.data_type = stripped;
      return stripped;
    }
    // An operand of `*` that isn't a pointer goes on to the dereference's
    // own check, which reports it. That checks the operand a second time,
    // as Python does.
  }
  // `&arr` on an array has the type `Pointer(Array(elem, N))` (C99
  // §6.5.3.2p3). c99_to_tac collapses a pointer to its width, and the
  // sizes recurse into the array. The `AddressOf` that `decay_if_array`
  // makes has its type set there and isn't checked again.
  const Type t_inner = tc.check_exp(inner);
  Type result = ir::Pointer{.referenced_type = t_inner};
  e.data_type = result;
  return result;
}

}  // namespace

// Returns the common type of `==` or `!=` with a pointer operand. C99
// §6.5.9p2 allows a pointer with a pointer of the same type, which gives
// that type, `void *` with any object pointer, which gives `void *`
// ("one operand is a pointer to an object type and the other is a
// pointer to a qualified or unqualified version of void"), and a pointer
// with a null pointer constant, which gives the pointer's type, in
// either order. Anything else (two pointers of different non-void types,
// a pointer and a nonzero integer, a pointer and a floating value)
// throws. The caller has checked that `tl` or `tr` is a pointer.
Type TypeChecker::pointer_equality_common_type(Exp& lhs, Exp& rhs, const Type& tl,
                                               const Type& tr) {
  const bool l_ptr = is_pointer_type(tl);
  const bool r_ptr = is_pointer_type(tr);
  // The pointer's own qualifiers (`int * const`) don't change equality,
  // so they go before the pointee is read.
  const Type& tl_p = strip_quals(tl);
  const Type& tr_p = strip_quals(tr);
  if (l_ptr && r_ptr) {
    // `void *` meets any object pointer at `void *`.
    auto common = composite_pointer(tl, tr);
    if (!common) {
      fail("comparison of distinct pointer types: " + asdl::py_repr(tl) + " vs " +
           asdl::py_repr(tr));
    }
    return std::move(*common);
  }
  // One operand is a pointer, and the other must be a null pointer
  // constant or, in C23, have type `nullptr_t` (§6.5.10p2). `nullptr` is
  // both.
  if (l_ptr && (is_null_pointer_constant(rhs) || is_nullptr_type(tr))) {
    return ir::Pointer{.referenced_type = *std::get<ir::Pointer>(tl_p).referenced_type};
  }
  if (r_ptr && (is_null_pointer_constant(lhs) || is_nullptr_type(tl))) {
    return ir::Pointer{.referenced_type = *std::get<ir::Pointer>(tr_p).referenced_type};
  }
  fail("comparison between pointer and non-null-constant non-pointer: " + asdl::py_repr(tl) +
       " vs " + asdl::py_repr(tr));
}

// Types a binary operator other than `&&` and `||` with a `nullptr_t`
// operand, or returns none to leave `exp` to the pointer rules when the
// other operand is a pointer.
//
// C23 §6.5.10p2 lets `==` and `!=` compare two `nullptr_t` operands, a
// `nullptr_t` with a null pointer constant, and a pointer with a
// `nullptr_t`. §6.5.10p5-6 make the first two equal, and the third equal
// if and only if the pointer is null. The first two convert both
// operands to `void *`, which makes each a null pointer, and the third is
// a pointer comparison. Every other operator takes arithmetic, integer,
// real, or pointer operands, and `nullptr_t` is none of those (§6.5.9p2
// for the relational operators).
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
      // A string literal has type `char[N+1]` (C99 §6.4.5p6), one element
      // per byte and the null. string_lifting has made most literals
      // file-scope statics by now, and the ones left initialize a
      // character array directly, which the declaration's check
      // handles. The array type lets a stray literal decay cleanly. The
      // C++ node has no field for it (`checked_type`).
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
      // A braced list belongs only in the initializer of a declaration,
      // which `check_block_var` and `check_for_init` read directly. One
      // here is `{1, 2}` where it doesn't belong, such as a return value,
      // an argument, or the right side of an assignment.
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
