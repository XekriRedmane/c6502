// Part T-B of the port of passes/type_checking.py: static initializers
// and `constexpr` objects. These are the module's functions of
// type_checking.py:491-1199, which compute the value tree of a static
// object's initializer, and the checker's methods of 3637-4438, which
// check blocks, block-scope declarations, compound literals, the
// initializers of arrays, structs, and unions, and the rules of C23's
// `constexpr` (§6.7.2).
//
// A static object's value tree is in the "natural form" of its type: an
// integer for an integer type (unbounded, so the static_init builder
// masks it to the type's width), the IEEE 754 bits of a `float` or a
// `double` as an integer, an `AddressInit` for an address constant, a
// tuple of the elements or members for an array or a struct, and for a
// union a tuple of its first member's tree or a `UnionValue`. No
// floating value of the host reaches a tree, since every conversion
// between integer and floating values goes through fp_arith.
#include <set>
#include <string>
#include <vector>

#include "builtin_functions.h"
#include "fp_arith.h"
#include "passes/designators.h"
#include "passes/type_checker.h"
#include "pyrepr.h"

namespace c6502::passes::type_checking {

namespace {

namespace ir = ::c6502::c99_ast;
using K = ir::Kind_data_type;
using E = ir::Kind_exp;
namespace ce = ::c6502::passes::constant_expression;

[[noreturn]] void fail(const std::string& message) { throw TypeCheckError(message); }

ValueTree tree_int(Int v) {
  ValueTree t;
  t.kind = ValueTree::Kind::kInt;
  t.integer = v;
  return t;
}

ValueTree tree_tuple(std::vector<ValueTree> items) {
  ValueTree t;
  t.kind = ValueTree::Kind::kTuple;
  t.items = std::move(items);
  return t;
}

ValueTree tree_address(AddressInit a) {
  ValueTree t;
  t.kind = ValueTree::Kind::kAddress;
  t.address = std::move(a);
  return t;
}

ValueTree tree_union(Int member, ValueTree value) {
  ValueTree t;
  t.kind = ValueTree::Kind::kUnion;
  t.member = member;
  t.items.push_back(std::move(value));
  return t;
}

const std::string& tag_of(const Type& t) {
  if (const auto* s = std::get_if<ir::Structure>(&t)) {
    return s->tag;
  }
  return std::get<ir::Union>(t).tag;
}

std::string struct_keyword(const Type& t) {
  return asdl::isa<ir::Union>(t) ? "union" : "struct";
}

// Returns `text` with every occurrence of `from` replaced by `to`, as
// Python's `str.replace` does.
std::string replace_all(std::string text, const std::string& from, const std::string& to) {
  if (from.empty()) {
    return text;
  }
  std::string out;
  std::size_t start = 0;
  while (true) {
    const std::size_t at = text.find(from, start);
    if (at == std::string::npos) {
      out.append(text, start, std::string::npos);
      return out;
    }
    out.append(text, start, at - start);
    out += to;
    start = at + from.size();
  }
}

std::size_t code_point_count(const std::string& s) { return pyrepr::code_points(s).size(); }

Int as_int(std::size_t n) { return Int(static_cast<long long>(n)); }

// Returns Python's `int()` of the truncated floating value with IEEE 754
// `bits`, which raises for an infinity or a NaN, with the message of
// `int(float)`.
Int floating_bits_to_int(Int bits, bool single, const Type& target) {
  const auto truncated =
      single ? fp_arith::single_bits_to_int(bits) : fp_arith::double_bits_to_int(bits);
  if (!truncated) {
    const Int exponent_mask = single ? Int(0x7F800000) : Int(0x7FF0000000000000LL);
    const Int mantissa_mask = single ? Int(0x007FFFFF) : Int(0x000FFFFFFFFFFFFFLL);
    if ((bits & exponent_mask) == exponent_mask && (bits & mantissa_mask) != 0) {
      throw CompileError("ValueError", "cannot convert float NaN to integer");
    }
    throw CompileError("OverflowError", "cannot convert float infinity to integer");
  }
  if (const auto exact = truncated->exact()) {
    return *exact;
  }
  // A value of 2^126 or more is reduced to the target's width here,
  // which Python's unbounded int and the reduction after it give too.
  if (is_integer_type(target) && !asdl::isa<ir::Bool>(target)) {
    return truncated->wrapped(int_bits(target));
  }
  throw InternalError("a floating value too large for a 128-bit integer");
}

// Returns the value tree `value`, of type `source_type`, in
// `target_type`'s form. An integer goes through `coerce_init_value`.
// An address converts only to a pointer, which leaves it as it is, or to
// `bool`, where the address of an object isn't null.
ValueTree coerce_tree(const ValueTree& value, const Type& source_type, const Type& target_type) {
  if (value.kind == ValueTree::Kind::kInt) {
    return tree_int(coerce_init_value(value.integer, source_type, target_type));
  }
  const Type& source = strip_quals(source_type);
  const Type& target = strip_quals(target_type);
  if (types_equal(source, target)) {
    return value;
  }
  if (asdl::isa<ir::Bool>(target)) {
    // `int(value != 0)` of a tuple or an `AddressInit` is 1.
    return tree_int(1);
  }
  if (is_floating_type(target) || is_integer_type(target) || is_floating_type(source)) {
    throw InternalError("TypeError: int() of a value tree that isn't an integer");
  }
  return value;
}

// The address constant `exp` gives, as Python's `_address_constant`
// computes it with its nested functions.
class AddressConstant {
 public:
  AddressConstant(const std::string& name, const SymbolTable* symbols, const TypeTable* types)
      : name_(name), symbols_(symbols), types_(types) {}

  [[noreturn]] void failure(const std::string& why) const {
    fail("initializer for static-storage object " + pyrepr::string(name_) + " " + why);
  }

  Int constant(const Exp& index) const {
    try {
      return ce::evaluate_integer_constant_expression(index).first;
    } catch (const ce::ConstantExpressionError& e) {
      failure(std::string("is not a constant expression: ") + e.what());
    }
  }

  Int pointee_size(const std::optional<Type>& t) const {
    if (!t) {
      throw InternalError("AttributeError: 'NoneType' object has no attribute 'referenced_type'");
    }
    const auto* p = std::get_if<ir::Pointer>(&strip_quals(*t));
    if (p == nullptr) {
      throw InternalError("AttributeError: no attribute 'referenced_type' on " + asdl::py_repr(*t));
    }
    return sizeof_type(*p->referenced_type, types_);
  }

  Int member_offset(const Type& type, const std::string& member) const {
    const Type& t = strip_quals(type);
    if (!asdl::isa<ir::Structure, ir::Union>(t)) {
      throw InternalError("AttributeError: no attribute 'tag' on " + asdl::py_repr(t));
    }
    const StructLayout* layout = types_ != nullptr ? types_->find(tag_of(t)) : nullptr;
    if (layout == nullptr) {
      failure("takes the address of a member of " + asdl::py_repr(t));
    }
    for (const MemberInfo& m : layout->members) {
      if (m.name == member) {
        return m.byte_offset;
      }
    }
    failure("names no member " + pyrepr::string(member) + " of " + asdl::py_repr(t));
  }

  // Returns the address of the object the lvalue `e` designates.
  AddressInit lvalue(const Exp& e) const {
    switch (ir::kind(e)) {
      case E::Var:
      case E::CompoundLiteral: {
        const bool literal = asdl::isa<ir::CompoundLiteral>(e);
        const std::string& target =
            literal ? std::get<ir::CompoundLiteral>(e).name : std::get<ir::Var>(e).name;
        if (symbols_ != nullptr) {
          const Symbol* sym = symbols_->find(target);
          if (sym == nullptr) {
            failure("references undeclared identifier " + pyrepr::string(target));
          }
          if (!std::holds_alternative<StaticAttr>(sym->attrs) &&
              !std::holds_alternative<FunAttr>(sym->attrs)) {
            const std::string what =
                literal ? "a compound literal inside a function" : pyrepr::string(target);
            failure("takes the address of " + what +
                    ", which doesn't have static storage duration");
          }
        }
        return AddressInit{.name = target, .offset = 0};
      }
      case E::Subscript: {
        const auto& s = std::get<ir::Subscript>(e);
        const AddressInit base = pointer(*s.array);
        const Int step = constant(*s.index) * pointee_size(data_type_of(*s.array));
        return AddressInit{.name = base.name, .offset = base.offset + step};
      }
      case E::Dot: {
        const auto& d = std::get<ir::Dot>(e);
        const AddressInit base = lvalue(*d.operand);
        const std::optional<Type>& t = data_type_of(*d.operand);
        if (!t) {
          throw InternalError("AttributeError: 'NoneType' object has no attribute 'tag'");
        }
        return AddressInit{.name = base.name, .offset = base.offset + member_offset(*t, d.member)};
      }
      case E::Arrow: {
        const auto& a = std::get<ir::Arrow>(e);
        const AddressInit base = pointer(*a.operand);
        const std::optional<Type>& t = data_type_of(*a.operand);
        const auto* p = t ? std::get_if<ir::Pointer>(&strip_quals(*t)) : nullptr;
        if (p == nullptr) {
          throw InternalError("AttributeError: no attribute 'referenced_type'");
        }
        return AddressInit{.name = base.name,
                           .offset = base.offset + member_offset(*p->referenced_type, a.member)};
      }
      case E::Dereference:
        return pointer(*std::get<ir::Dereference>(e).exp);
      default:
        failure("takes the address of a non-constant lvalue " + asdl::py_repr(e));
    }
  }

  // Returns the address that the pointer-valued `e` holds.
  AddressInit pointer(const Exp& e) const {
    switch (ir::kind(e)) {
      case E::AddressOf:
        return lvalue(*std::get<ir::AddressOf>(e).exp);
      case E::Cast: {
        const Exp& inner = *std::get<ir::Cast>(e).exp;
        const std::optional<Type>& t = data_type_of(inner);
        if (t && is_pointer_type(*t)) {
          return pointer(inner);
        }
        break;
      }
      case E::Binary: {
        const auto& b = std::get<ir::Binary>(e);
        const bool add = asdl::isa<ir::Add>(b.op);
        if (!add && !asdl::isa<ir::Subtract>(b.op)) {
          break;
        }
        const std::optional<Type>& tl = data_type_of(*b.left);
        const std::optional<Type>& tr = data_type_of(*b.right);
        const Exp* ptr = nullptr;
        const Exp* k = nullptr;
        if (tl && is_pointer_type(*tl)) {
          ptr = &*b.left;
          k = &*b.right;
        } else if (add && tr && is_pointer_type(*tr)) {
          ptr = &*b.right;
          k = &*b.left;
        } else {
          failure("is not an address constant: " + asdl::py_repr(e));
        }
        const std::optional<Type>& tk = data_type_of(*k);
        if (!tk || !is_integer_type(*tk)) {
          failure("is not an address constant: " + asdl::py_repr(e));
        }
        Int step = constant(*k) * pointee_size(data_type_of(*ptr));
        if (!add) {
          step = -step;
        }
        const AddressInit base = pointer(*ptr);
        return AddressInit{.name = base.name, .offset = base.offset + step};
      }
      default:
        break;
    }
    failure("is not an address constant: " + asdl::py_repr(e));
  }

 private:
  const std::string& name_;
  const SymbolTable* symbols_;
  const TypeTable* types_;
};

// The offset `_null_based_offset` computes with its nested functions.
class NullBasedOffset {
 public:
  explicit NullBasedOffset(const TypeTable* types) : types_(types) {}

  static std::optional<Int> constant(const Exp& e) {
    try {
      return ce::evaluate_integer_constant_expression(e).first;
    } catch (const ce::ConstantExpressionError&) {
      return std::nullopt;
    }
  }

  std::optional<Int> member_offset(const std::optional<Type>& type,
                                   const std::string& member) const {
    if (!type) {
      return std::nullopt;
    }
    const Type& t = strip_quals(*type);
    if (!asdl::isa<ir::Structure, ir::Union>(t) || types_ == nullptr) {
      return std::nullopt;
    }
    const StructLayout* layout = types_->find(tag_of(t));
    if (layout == nullptr) {
      return std::nullopt;
    }
    for (const MemberInfo& m : layout->members) {
      if (m.name == member) {
        return m.byte_offset;
      }
    }
    return std::nullopt;
  }

  static std::optional<Type> pointee(const std::optional<Type>& t) {
    if (!t) {
      throw InternalError("AttributeError: 'NoneType' object has no attribute 'referenced_type'");
    }
    const auto* p = std::get_if<ir::Pointer>(&strip_quals(*t));
    if (p == nullptr) {
      throw InternalError("AttributeError: no attribute 'referenced_type' on " + asdl::py_repr(*t));
    }
    return *p->referenced_type;
  }

  std::optional<Int> pointer(const Exp& e) const {
    if (const auto* c = std::get_if<ir::Cast>(&e)) {
      const std::optional<Type>& inner_t = data_type_of(*c->exp);
      if (is_pointer_type(c->target_type) && inner_t) {
        if (is_pointer_type(*inner_t)) {
          return pointer(*c->exp);
        }
        if (is_integer_type(*inner_t)) {
          const auto k = constant(*c->exp);
          return k && *k == Int(0) ? std::optional<Int>(Int(0)) : std::nullopt;
        }
      }
      return std::nullopt;
    }
    if (const auto* a = std::get_if<ir::AddressOf>(&e)) {
      return lvalue(*a->exp);
    }
    return std::nullopt;
  }

  std::optional<Int> lvalue(const Exp& e) const {
    std::optional<Int> base;
    std::optional<Int> offset;
    switch (ir::kind(e)) {
      case E::Arrow: {
        const auto& a = std::get<ir::Arrow>(e);
        base = pointer(*a.operand);
        if (!base) {
          return std::nullopt;
        }
        offset = member_offset(pointee(data_type_of(*a.operand)), a.member);
        break;
      }
      case E::Dot: {
        const auto& d = std::get<ir::Dot>(e);
        base = lvalue(*d.operand);
        if (!base) {
          return std::nullopt;
        }
        offset = member_offset(data_type_of(*d.operand), d.member);
        break;
      }
      case E::Subscript: {
        const auto& s = std::get<ir::Subscript>(e);
        base = pointer(*s.array);
        const auto k = constant(*s.index);
        if (!base || !k) {
          return std::nullopt;
        }
        offset = *k * sizeof_type(*pointee(data_type_of(*s.array)), types_);
        break;
      }
      case E::Dereference:
        return pointer(*std::get<ir::Dereference>(e).exp);
      default:
        return std::nullopt;
    }
    if (!offset) {
      return std::nullopt;
    }
    return *base + *offset;
  }

 private:
  const TypeTable* types_;
};

// Returns whether `t`, without qualifiers, is `float` or `double`.
bool is_fp(const std::optional<Type>& t) {
  return t && asdl::isa<ir::Float, ir::Double>(strip_quals(*t));
}

// Python's `_FP_FOLDS`: the floating operations a static initializer
// folds, each in its `float` and `double` forms in fp_arith. Returns
// null for any other operator.
using FpFold = Int (*)(Int, Int);
const FpFold* fp_folds(const ir::Type_binary_operator& op) {
  static const FpFold kAdd[2] = {&fp_arith::single_add, &fp_arith::double_add};
  static const FpFold kSub[2] = {&fp_arith::single_sub, &fp_arith::double_sub};
  static const FpFold kMul[2] = {&fp_arith::single_mul, &fp_arith::double_mul};
  static const FpFold kDiv[2] = {&fp_arith::single_div, &fp_arith::double_div};
  switch (ir::kind(op)) {
    case ir::Kind_binary_operator::Add:
      return kAdd;
    case ir::Kind_binary_operator::Subtract:
      return kSub;
    case ir::Kind_binary_operator::Multiply:
      return kMul;
    case ir::Kind_binary_operator::Divide:
      return kDiv;
    default:
      return nullptr;
  }
}

// Returns the value of an integer constant variant, the bit pattern it
// holds.
Int const_raw_value(const ir::Type_const& c) {
  using C = ir::Kind_const;
  switch (ir::kind(c)) {
    case C::ConstInt:
      return std::get<ir::ConstInt>(c).value;
    case C::ConstLong:
      return std::get<ir::ConstLong>(c).value;
    case C::ConstLongLong:
      return std::get<ir::ConstLongLong>(c).value;
    case C::ConstUInt:
      return std::get<ir::ConstUInt>(c).value;
    case C::ConstULong:
      return std::get<ir::ConstULong>(c).value;
    case C::ConstULongLong:
      return std::get<ir::ConstULongLong>(c).value;
    case C::ConstChar:
      return std::get<ir::ConstChar>(c).value;
    case C::ConstUChar:
      return std::get<ir::ConstUChar>(c).value;
    case C::ConstBool:
      return std::get<ir::ConstBool>(c).value;
    case C::ConstBitInt:
      return std::get<ir::ConstBitInt>(c).value;
    default:
      throw InternalError("not an integer constant: " + asdl::py_repr(c));
  }
}

// Parses the decimal digits of a `zeropage(addr)` marker's address.
Int parse_decimal(std::string_view digits) {
  Int v = 0;
  bool negative = false;
  std::size_t i = 0;
  if (!digits.empty() && (digits[0] == '-' || digits[0] == '+')) {
    negative = digits[0] == '-';
    i = 1;
  }
  if (i >= digits.size()) {
    throw InternalError("ValueError: invalid literal for int(): " + pyrepr::string(digits));
  }
  for (; i < digits.size(); ++i) {
    const char c = digits[i];
    if (c < '0' || c > '9') {
      throw InternalError("ValueError: invalid literal for int(): " + pyrepr::string(digits));
    }
    v = v * Int(10) + Int(c - '0');
  }
  return negative ? -v : v;
}

}  // namespace

// ---------------------------------------------------------------------
// The module's functions.
// ---------------------------------------------------------------------

// Maps a constant's payload into `target_type`'s natural static-init
// form: an integer for an integer type, or the IEEE 754 bits for
// `float` and `double`. The constant's own type comes from its variant,
// and a conversion across the floating boundary goes through fp_arith.
Int coerce_init_const_value(const ir::Type_const& c, const Type& target_type) {
  const Type src_type = const_source_type(c);
  Int raw = 0;
  if (asdl::isa<ir::ConstNullptr>(c)) {
    // `nullptr` has a null `void *`'s representation (C23 §7.21.2p3),
    // all zero bytes, and converts to `false` and to null pointers
    // (§6.3.2.4p1).
    raw = 0;
  } else if (const auto* f = std::get_if<ir::ConstFloat>(&c)) {
    raw = f->bits;
  } else if (const auto* d = std::get_if<ir::ConstDouble>(&c)) {
    raw = d->bits;
  } else {
    // An integer constant holds its bit pattern, so a signed one with
    // the sign bit set is negative.
    raw = coerce_int_to_type(const_raw_value(c), src_type);
  }
  return coerce_init_value(raw, src_type, target_type);
}

// Converts `value`, in `source_type`'s natural static-init form, into
// `target_type`'s. A conversion to the same type changes nothing, an
// integer converts to an integer as a cast does, and a conversion to or
// from a floating type goes through fp_arith. Qualifiers don't change a
// value's form, so they are stripped first: `static const double x =
// 1.5;` once stored the integer 1 as its bits, since `Const(Double)`
// didn't count as floating.
Int coerce_init_value(Int value, const Type& source_type, const Type& target_type) {
  const Type& source = strip_quals(source_type);
  const Type& target = strip_quals(target_type);
  if (types_equal(source, target)) {
    return value;
  }
  if (asdl::isa<ir::Bool>(target)) {
    // C99 §6.3.1.2: 0 if the value compares equal to 0, else 1.
    if (asdl::isa<ir::Float>(source)) {
      return (value & Int(0x7FFFFFFF)) != Int(0) ? 1 : 0;
    }
    if (asdl::isa<ir::Double>(source)) {
      return (value & Int(0x7FFFFFFFFFFFFFFFLL)) != Int(0) ? 1 : 0;
    }
    if (is_integer_type(source)) {
      value = coerce_int_to_type(value, source);
    }
    return value != Int(0) ? 1 : 0;
  }
  const bool src_is_fp = asdl::isa<ir::Float, ir::Double>(source);
  const bool tgt_is_fp = asdl::isa<ir::Float, ir::Double>(target);
  if (src_is_fp && tgt_is_fp) {
    // A floating value changes precision.
    if (asdl::isa<ir::Float>(source)) {
      return fp_arith::single_bits_to_double_bits(value);
    }
    return fp_arith::double_bits_to_single_bits(value);
  }
  if (!src_is_fp && tgt_is_fp) {
    // An integer value becomes the IEEE 754 bits of the target.
    if (asdl::isa<ir::Float>(target)) {
      return fp_arith::int_to_single_bits(value);
    }
    return fp_arith::int_to_double_bits(value);
  }
  if (src_is_fp && !tgt_is_fp) {
    // IEEE 754 bits become an integer, truncated toward zero (C99
    // §6.3.1.4).
    value = floating_bits_to_int(value, asdl::isa<ir::Float>(source), target);
  }
  // An integer wraps to the target's range here, since a cast in the
  // middle of a chain has to wrap at the cast's width. `static long v =
  // (unsigned char)400;` once stored 400, where the conversion gives
  // 144, and `(signed char)200` gave 200 where it gives -56. The
  // static-const folds read the value too, and the static_init builder
  // masks it to the target's width.
  if (is_integer_type(target)) {
    return coerce_int_to_type(value, target);
  }
  return value;
}

// Returns the data type of a constant variant, for the coercion of
// static initializers. A `ConstChar` gives `char` here, where the
// constant evaluator gives `signed char`.
Type const_source_type(const ir::Type_const& c) {
  using C = ir::Kind_const;
  switch (ir::kind(c)) {
    case C::ConstInt:
      return ir::Int{};
    case C::ConstUInt:
      return ir::UInt{};
    case C::ConstLong:
      return ir::Long{};
    case C::ConstULong:
      return ir::ULong{};
    case C::ConstLongLong:
      return ir::LongLong{};
    case C::ConstULongLong:
      return ir::ULongLong{};
    case C::ConstFloat:
      return ir::Float{};
    case C::ConstDouble:
      return ir::Double{};
    case C::ConstChar:
      return ir::Char{};
    case C::ConstUChar:
      return ir::UChar{};
    case C::ConstBool:
      return ir::Bool{};
    case C::ConstNullptr:
      return ir::NullptrT{};
    case C::ConstBitInt: {
      const auto& b = std::get<ir::ConstBitInt>(c);
      return ir::BitInt{.width = b.width, .is_signed = b.is_signed};
    }
  }
  throw InternalError("unexpected c99 const: " + asdl::py_repr(c));
}

// Returns the address that the pointer-valued expression `exp` gives,
// as a static object's or function's name and a byte offset from it,
// for the initializer of the static-storage object `name`. C99 §6.6p9
// forms such an address constant with `&` (or an array's or function's
// decay, which the type checker writes as an `AddressOf`), `[]`, `.`,
// `->`, `*`, pointer casts, and the addition or subtraction of an
// integer constant. So `A[5] + 2`, with `A` a `uint8_t[11][14]`, is `A`
// plus 72 bytes. Throws when `exp` isn't one, as `&p[1]` isn't for a
// pointer `p`, since its value depends on what `p` holds at run time.
AddressInit address_constant(const Exp& exp, const std::string& name, const SymbolTable* symbols,
                             const TypeTable* types) {
  return AddressConstant(name, symbols, types).pointer(exp);
}

// Returns the address that the type-checked pointer expression `exp`
// gives when it is a member or element of an object at address 0, as in
// `&((struct s *)0)->m[2]`, or none for any other expression.
// <stddef.h>'s `offsetof(type, member)` expands to such an address
// converted to `size_t`, and C23 §7.21p4 makes that an integer constant
// expression, so the type checker folds the address to a constant.
// Nothing is read at address 0, since `->`, `.`, `[]`, and `&` only
// compute addresses here.
std::optional<Int> null_based_offset(const Exp& exp, const TypeTable* types) {
  return NullBasedOffset(types).pointer(exp);
}

// Returns the value of a static-storage initializer, which C99 §6.7.8p4
// requires to be a constant expression. After the checks and the
// conversion of the initializer, `exp` is a `Constant`, a named
// constant, a `Cast` of one of these (each cast applies its conversion
// in turn, so `(int)(float)0x100` goes through float), an address
// constant (C99 §6.6p9, `_address_constant`), a floating `+`, `-`, or an
// operation of `_FP_FOLDS`, or an integer operator expression that the
// constant evaluator folds. The value is in `target_type`'s natural
// form. `symbols` tells whether the object an address names has static
// storage, since the address of a local isn't constant.
ValueTree const_init_value(const Exp& exp, const Type& target_type, const std::string& name,
                           const SymbolTable* symbols, const TypeTable* types) {
  switch (ir::kind(exp)) {
    case E::Constant:
      return tree_int(coerce_init_const_value(std::get<ir::Constant>(exp).const_, target_type));
    case E::Var:
    case E::Dot:
    case E::CompoundLiteral: {
      const auto named = ce::named_constant(exp);
      if (named && !asdl::isa<ir::Array, ir::Structure, ir::Union>(strip_quals(named->type))) {
        // A named constant (C23 §6.6p7): a `constexpr` object, or a
        // member of one, with the value the object has.
        return coerce_tree(named->value, strip_quals(named->type), target_type);
      }
      break;
    }
    case E::Cast: {
      const auto& c = std::get<ir::Cast>(exp);
      const ValueTree inner = const_init_value(*c.exp, c.target_type, name, symbols, types);
      return coerce_tree(inner, c.target_type, target_type);
    }
    default:
      break;
  }
  const std::optional<Type>& t = data_type_of(exp);
  if (asdl::isa<ir::AddressOf, ir::Binary>(exp) && t && is_pointer_type(*t)) {
    AddressInit address = address_constant(exp, name, symbols, types);
    if (asdl::isa<ir::Bool>(strip_quals(target_type))) {
      // The address of an object or function isn't null, so it converts
      // to 1 (C99 §6.3.1.2).
      return tree_int(1);
    }
    return tree_address(std::move(address));
  }
  if (const auto* u = std::get_if<ir::Unary>(&exp)) {
    if (asdl::isa<ir::Plus>(u->op) && is_fp(t)) {
      // `+1.5` is `1.5`.
      return const_init_value(*u->exp, target_type, name, symbols, types);
    }
    if (asdl::isa<ir::Negate>(u->op) && is_fp(t)) {
      // `-1.5` negates the IEEE 754 pattern by flipping its sign bit,
      // and then converts from the operand's type to the target.
      const Type fp_t = strip_quals(*t);
      const bool single = asdl::isa<ir::Float>(fp_t);
      const ValueTree bits = const_init_value(*u->exp, fp_t, name, symbols, types);
      if (bits.kind != ValueTree::Kind::kInt) {
        throw InternalError("TypeError: unsupported operand type for ^");
      }
      const Int flipped = bits.integer ^ (Int(1) << (single ? 31 : 63));
      return tree_int(coerce_init_value(flipped, fp_t, target_type));
    }
  }
  if (const auto* b = std::get_if<ir::Binary>(&exp)) {
    if (is_fp(t)) {
      if (const FpFold* folds = fp_folds(b->op)) {
        // `1.0 / 3.0`, computed as the program would compute it, in the
        // type of the operation, to which the type checker has converted
        // both operands, so that C23's `constexpr double third = 1.0 /
        // 3.0;` compiles (§6.7.2p18).
        const Type fp_t = strip_quals(*t);
        const ValueTree a = const_init_value(*b->left, fp_t, name, symbols, types);
        const ValueTree c = const_init_value(*b->right, fp_t, name, symbols, types);
        if (a.kind != ValueTree::Kind::kInt || c.kind != ValueTree::Kind::kInt) {
          throw InternalError("TypeError: a floating fold of a value that isn't an integer");
        }
        const FpFold fold = folds[asdl::isa<ir::Float>(fp_t) ? 0 : 1];
        return tree_int(coerce_init_value(fold(a.integer, c.integer), fp_t, target_type));
      }
    }
  }
  if (asdl::isa<ir::Unary, ir::Binary, ir::Conditional, ir::SizeOfExp, ir::SizeOfType,
                ir::FunctionCall>(exp) &&
      t && is_integer_type(strip_quals(*t))) {
    const auto* call = std::get_if<ir::FunctionCall>(&exp);
    if (call == nullptr || builtin_functions::is_builtin(call->name)) {
      // An integer operator expression such as `-5`, `1 << 20`, or
      // `sizeof(long)`. C99 §6.6p6 lets an arithmetic constant
      // expression use these, and the folder applies C's width and
      // signedness rules.
      try {
        const auto [value, value_type] = ce::evaluate_integer_constant_expression(exp);
        return tree_int(coerce_init_value(value, value_type, target_type));
      } catch (const ce::ConstantExpressionError& e) {
        fail("initializer for static-storage object " + pyrepr::string(name) +
             " is not a constant expression: " + e.what());
      }
    }
  }
  fail("initializer for static-storage object " + pyrepr::string(name) +
       " is not a constant expression");
}

// Returns a value tree of zeros for a static object of type `t`: 0 for
// a scalar (the bits of +0.0 are all zero, so 0 serves for `float` and
// `double` too), a tuple of `N` zeroed elements for an array, and for a
// struct a tuple with each member zeroed in declaration order. A union
// zeroes only its first member, since its bytes are all zero anyway, and
// the first member's shape gives the static_init flattening a uniform
// tree to walk. It serves tentative definitions and the trailing entries
// a partial array initializer leaves out.
ValueTree zero_aggregate(const Type& type, const TypeTable* types) {
  const Type& t = strip_quals(type);
  if (const auto* a = std::get_if<ir::Array>(&t)) {
    std::vector<ValueTree> items;
    for (Int i = 0; i < a->size; i += Int(1)) {
      items.push_back(zero_aggregate(*a->element_type, types));
    }
    return tree_tuple(std::move(items));
  }
  if (asdl::isa<ir::Structure, ir::Union>(t)) {
    if (types == nullptr) {
      return tree_tuple({});
    }
    const StructLayout* layout = types->find(tag_of(t));
    if (layout == nullptr || !layout->complete) {
      return tree_tuple({});
    }
    std::vector<ValueTree> items;
    if (asdl::isa<ir::Union>(t)) {
      if (layout->members.empty()) {
        return tree_tuple({});
      }
      items.push_back(zero_aggregate(layout->members[0].type, types));
      return tree_tuple(std::move(items));
    }
    for (const MemberInfo& m : layout->members) {
      items.push_back(zero_aggregate(m.type, types));
    }
    return tree_tuple(std::move(items));
  }
  return tree_int(0);
}

// Returns whether `t` is one of the three character element types,
// qualified or not, which a string literal may initialize an array of
// (C99 §6.7.8p14).
bool is_char_element(const Type& t) {
  return asdl::isa<ir::Char, ir::SChar, ir::UChar>(strip_quals(t));
}

std::vector<std::optional<Int>> param_zeropage(const std::vector<std::string>& markers) {
  bool any = false;
  for (const std::string& m : markers) {
    any = any || (!m.empty() && m[0] == '@');
  }
  if (!any) {
    return {};
  }
  std::vector<std::optional<Int>> out;
  for (const std::string& m : markers) {
    if (!m.empty() && m[0] == '@') {
      out.push_back(parse_decimal(std::string_view(m).substr(1)));
    } else {
      out.push_back(std::nullopt);
    }
  }
  return out;
}

// Returns `t` completed by its initializer `init` when `t` is an array
// of unknown size (C99 §6.7.8p22): a string literal for an array of
// char gives its length plus the null, and a braced list its number of
// items. Anything else, no initializer among them, leaves `t` as it is,
// and the checks after it reject the incomplete type. The empty
// initializer `{}` is an error, because C23 §6.7.11p4 states "An array
// of unknown size shall not be initialized by an empty initializer."
// `what` names the object in the message, as in "array 'a'".
Type size_from_initializer(const Type& t, const Exp* init, std::string_view what) {
  const auto* a = std::get_if<ir::Array>(&t);
  if (a == nullptr || a->size != Int(0) || init == nullptr) {
    return t;
  }
  Int size = 0;
  if (const auto* s = std::get_if<ir::String>(init); s != nullptr &&
                                                      is_char_element(*a->element_type)) {
    size = as_int(code_point_count(s->str)) + Int(1);
  } else if (const auto* list = std::get_if<ir::InitList>(init)) {
    if (list->items.empty()) {
      fail(std::string(what) +
           " has unknown size, so it can't be initialized by the empty initializer `{}`, which "
           "has no elements to give it a size (C23 §6.7.11p4)");
    }
    size = as_int(list->items.size());
  } else {
    return t;
  }
  return ir::Array{.element_type = *a->element_type, .size = size};
}

// Returns the constant that the empty initializer `{}` gives a scalar
// of type `t`, before its conversion to `t`. C23 §6.7.11p11 initializes
// a pointer to a null pointer and an arithmetic object to zero. Integer
// 0 is a null pointer constant, and it converts to every integer type,
// to bool, and to `nullptr_t` (§6.3.2.4p2). A floating object gets a
// floating zero, so no integer-to-float conversion is left for run time.
Exp empty_init_zero(const Type& type) {
  const Type& t = strip_quals(type);
  if (asdl::isa<ir::Float>(t)) {
    return ir::Constant{.const_ = ir::ConstFloat{.bits = 0}, .data_type = std::nullopt};
  }
  if (asdl::isa<ir::Double>(t)) {
    return ir::Constant{.const_ = ir::ConstDouble{.bits = 0}, .data_type = std::nullopt};
  }
  return ir::Constant{.const_ = ir::ConstInt{.value = 0}, .data_type = std::nullopt};
}

// Returns the expression that initializes a scalar of type `t` from its
// initializer `init`. C23 §6.7.11p12 states "The initializer for a
// scalar shall be a single expression, optionally enclosed in braces, or
// it shall be an empty initializer." So `{5}` and `{5,}` give the 5, and
// `{}` gives the zero of `empty_init_zero`. A list of two or more items
// and a second pair of braces (`{{5}}`) are errors. `what` names the
// scalar in the message, as in "scalar variable 'x'".
Exp unbrace_scalar_init(Exp init, const Type& t, std::string_view what) {
  auto* list = std::get_if<ir::InitList>(&init);
  if (list == nullptr) {
    return init;
  }
  if (list->items.empty()) {
    return empty_init_zero(t);
  }
  if (list->items.size() > 1) {
    fail("too many initializers for " + std::string(what) + ": " +
         std::to_string(list->items.size()) +
         " given, and a scalar's braces hold one expression (C23 §6.7.11p12)");
  }
  if (asdl::isa<ir::InitList>(list->items[0])) {
    fail("unexpected nested initializer for " + std::string(what) +
         ": a scalar's initializer is one expression, optionally in one pair of braces, or the "
         "empty initializer `{}` (C23 §6.7.11p12)");
  }
  Exp item = std::move(list->items[0]);
  return item;
}

// Returns the composite of two array types that differ only in that one
// has an unknown size (C99 §6.7.5.2p6, §6.2.7p3), which is the one with
// the size, or none when they aren't such a pair.
std::optional<Type> composite_array(const Type& a, const Type& b) {
  const auto* aa = std::get_if<ir::Array>(&a);
  const auto* ba = std::get_if<ir::Array>(&b);
  if (aa == nullptr || ba == nullptr) {
    return std::nullopt;
  }
  if (!types_equal(*aa->element_type, *ba->element_type)) {
    return std::nullopt;
  }
  if (aa->size == Int(0)) {
    return b;
  }
  if (ba->size == Int(0)) {
    return a;
  }
  return std::nullopt;
}

// Returns the value tuple of a string literal's body (one code point per
// byte, 0 to 255) for an array of `n` characters: the bytes, then zeros
// to `n`, the first of which is the null terminator. The caller has
// checked that the string isn't longer than `n`. When it is exactly `n`
// long the null is left out (C99 §6.7.8p14 and its footnote).
ValueTree string_to_value_tuple(const std::string& s, Int n) {
  std::vector<ValueTree> out;
  for (const std::uint32_t c : pyrepr::code_points(s)) {
    out.push_back(tree_int(Int(static_cast<long long>(c & 0xFF))));
  }
  while (as_int(out.size()) < n) {
    out.push_back(tree_int(0));
  }
  return tree_tuple(std::move(out));
}

// Returns the value tree (a scalar or nested tuples) of an array, struct,
// or union static initializer. Scalar leaves go through
// `const_init_value`, so every rule of constant expressions applies, and
// an array or struct recurses into its list's items, with the
// element's or member's zero for each trailing entry the list leaves out
// (C99 §6.7.8p21).
ValueTree const_init_aggregate(const Exp& init, const Type& var_type0, const std::string& name,
                               const SymbolTable* symbols, const TypeTable* types) {
  const Type& var_type = strip_quals(var_type0);
  if (const auto* a = std::get_if<ir::Array>(&var_type)) {
    // A string literal initializing a char array, at any depth, lays
    // its bytes down with zeros to the array's size.
    if (const auto* s = std::get_if<ir::String>(&init);
        s != nullptr && is_char_element(*a->element_type)) {
      return string_to_value_tuple(s->str, a->size);
    }
    const auto* list = std::get_if<ir::InitList>(&init);
    if (list == nullptr) {
      fail("static array " + pyrepr::string(name) +
           " requires a brace-enclosed initializer (`{...}`)");
    }
    std::vector<ValueTree> items;
    for (Int i = 0; i < a->size; i += Int(1)) {
      const auto index = static_cast<std::size_t>(i);
      if (index < list->items.size()) {
        items.push_back(
            const_init_aggregate(list->items[index], *a->element_type, name, symbols, types));
      } else {
        items.push_back(zero_aggregate(*a->element_type, types));
      }
    }
    return tree_tuple(std::move(items));
  }
  if (asdl::isa<ir::Structure, ir::Union>(var_type)) {
    const auto* list = std::get_if<ir::InitList>(&init);
    if (list == nullptr) {
      // A structure or union constant (C23 §6.6p13), such as a
      // `constexpr struct s` object, has the value tree of its object.
      const auto named = ce::named_constant(init);
      if (named && types_equal(strip_quals(named->type), var_type)) {
        return named->value;
      }
      fail("static struct/union " + pyrepr::string(name) +
           " requires a brace-enclosed initializer (`{...}`)");
    }
    if (types == nullptr) {
      fail("static struct/union " + pyrepr::string(name) + " initializer requires a TypeTable");
    }
    const StructLayout* found = types->find(tag_of(var_type));
    if (found == nullptr || !found->complete) {
      fail("static struct/union " + pyrepr::string(name) + " has incomplete type");
    }
    // A copy, since the recursion reads the table again.
    const StructLayout layout = *found;
    std::vector<MemberInfo> members;
    if (asdl::isa<ir::Union>(var_type)) {
      // Per C99 §6.7.8p16, a list without designators initializes only
      // the first named member of a union, and a designator may choose
      // another member.
      const int k = union_member_index(layout, *list);
      if (k != 0) {
        const Type& m_type = layout.members[static_cast<std::size_t>(k)].type;
        ValueTree value = !list->items.empty()
                              ? const_init_aggregate(list->items[0], m_type, name, symbols, types)
                              : zero_aggregate(m_type, types);
        return tree_union(k, std::move(value));
      }
      if (!layout.members.empty()) {
        members.push_back(layout.members[0]);
      }
    } else {
      members = layout.members;
    }
    std::vector<ValueTree> items;
    for (std::size_t i = 0; i < members.size(); ++i) {
      if (i < list->items.size()) {
        items.push_back(const_init_aggregate(list->items[i], members[i].type, name, symbols, types));
      } else {
        items.push_back(zero_aggregate(members[i].type, types));
      }
    }
    return tree_tuple(std::move(items));
  }
  return const_init_value(init, var_type0, name, symbols, types);
}

// Returns the index in `layout` of the union member that the list
// `init` initializes: the one `init.union_member` names, or the first
// member when it names none.
int union_member_index(const StructLayout& layout, const ir::InitList& init) {
  if (!init.union_member) {
    return 0;
  }
  for (std::size_t k = 0; k < layout.members.size(); ++k) {
    if (layout.members[k].name == *init.union_member) {
      return static_cast<int>(k);
    }
  }
  fail("union '" + layout.tag + "' has no member named " + pyrepr::string(*init.union_member));
}

// ---------------------------------------------------------------------
// Blocks and block-scope declarations.
// ---------------------------------------------------------------------

void TypeChecker::check_block(ir::Type_block& block) {
  for (ir::Type_block_item& item : std::get<ir::Block>(block).block_item) {
    check_block_item(item);
  }
}

void TypeChecker::check_block_item(ir::Type_block_item& item) {
  if (auto* s = std::get_if<ir::S>(&item)) {
    check_statement(*s->statement);
    return;
  }
  check_block_declaration(*std::get<ir::D>(item).declaration);
}

void TypeChecker::check_block_declaration(ir::Type_declaration& decl) {
  switch (ir::kind(decl)) {
    case ir::Kind_declaration::VarDecl:
      check_block_var(std::get<ir::VarDecl>(decl).var_decl);
      return;
    case ir::Kind_declaration::FunctionDecl:
      check_function_decl(*std::get<ir::FunctionDecl>(decl).function_decl, false);
      return;
    case ir::Kind_declaration::StructDecl:
      check_struct_decl(std::get<ir::StructDecl>(decl).struct_decl);
      return;
  }
}

// string_lifting has put the whole program's lists in positional form,
// so this matters for the lists the parser checks while folding a
// `constexpr` object.
Exp TypeChecker::normalize_init(Exp init, const Type& t, const std::optional<std::string>& name) {
  const designators::LayoutOf layout_of =
      [this](std::string_view tag) -> std::optional<designators::Members> {
    const StructLayout* layout = types.find(std::string(tag));
    if (layout == nullptr || !layout->complete) {
      return std::nullopt;
    }
    designators::Members members;
    for (const MemberInfo& m : layout->members) {
      members.emplace_back(m.name, m.type);
    }
    return members;
  };
  std::optional<std::string> what;
  if (name) {
    what = display_name(*name);
  }
  return designators::normalize_initializer(std::move(init), t, layout_of, what);
}

void TypeChecker::check_block_var(ir::Type_var_decl& vd) {
  if (vd.init) {
    vd.init = normalize_init(std::move(*vd.init), vd.data_type, vd.name);
  }
  if (vd.is_constexpr) {
    begin_constexpr(vd);
  }
  check_block_var_body(vd);
  if (vd.is_constexpr) {
    end_constexpr(vd);
  }
}

void TypeChecker::check_block_var_body(ir::Type_var_decl& vd) {
  vd.data_type = size_from_initializer(vd.data_type, vd.init ? &*vd.init : nullptr,
                                       "array " + pyrepr::string(display_name(vd.name)));
  if (!is_complete_object_type(vd.data_type)) {
    fail("object " + pyrepr::string(vd.name) + " declared with non-object type " +
         asdl::py_repr(vd.data_type));
  }
  // A struct or union object's tag must be complete where it is
  // declared. A pointer to a struct may point to an incomplete one, since
  // the walk passes `require_complete` false through a `Pointer`. A
  // block-scope `extern` declaration may name an incomplete struct or
  // array, whose definition is elsewhere, as at file scope.
  const bool is_extern_decl = vd.storage_class && asdl::isa<ir::Extern>(*vd.storage_class);
  const bool is_static = vd.storage_class && asdl::isa<ir::Static>(*vd.storage_class);
  const bool require_complete =
      asdl::isa<ir::Structure, ir::Union, ir::Array>(strip_quals(vd.data_type)) && !is_extern_decl;
  check_well_formed(vd.data_type, "object " + pyrepr::string(vd.name), require_complete);
  if (is_extern_decl) {
    if (vd.init) {
      fail("block-scope `extern` declaration of " + pyrepr::string(vd.name) +
           " may not have an initializer");
    }
    const Symbol* prior = symbols.find(vd.name);
    bool is_global = true;
    if (prior != nullptr && std::holds_alternative<StaticAttr>(prior->attrs)) {
      is_global = std::get<StaticAttr>(prior->attrs).is_global;
    } else if (prior != nullptr && std::holds_alternative<FunAttr>(prior->attrs)) {
      fail(pyrepr::string(vd.name) +
           " previously declared as a function, now redeclared as an object");
    }
    add_or_merge_static_object(vd.name, vd.data_type, NoInitializer{}, is_global, vd.placement,
                               vd.section);
    return;
  }
  if (vd.placement && asdl::isa<ir::Absolute>(*vd.placement)) {
    fail("`[[c6502::address(...)]]` needs an `extern` declaration, since " +
         pyrepr::string(display_name(vd.name)) + " names memory the program doesn't own");
  }
  if (vd.placement && !is_static) {
    fail("`[[c6502::zeropage]]` needs an object with static storage; " +
         pyrepr::string(display_name(vd.name)) +
         " is automatic (declare it `static` or at file scope)");
  }
  if (vd.section && !is_static) {
    fail("`[[c6502::section(...)]]` needs an object with static storage; " +
         pyrepr::string(display_name(vd.name)) +
         " is automatic (declare it `static` or at file scope)");
  }
  if (is_static) {
    InitialValue initial;
    if (!vd.init) {
      // `static int x;` and `static int a[N];` are zero-initialized (C99
      // §6.7.8p10), an array as a tuple of zeros of its size.
      initial = Initial{zero_aggregate(vd.data_type, &types)};
    } else if (auto* arr = std::get_if<ir::Array>(&vd.data_type)) {
      // `static int a[3] = {1, 2, 3};` takes a braced list, which
      // `check_array_init_list` checks and converts item by item, and
      // `const_init_aggregate` then walks the same tree for the value
      // tuple, with the missing trailing entries padded. A char array
      // initialized by a string literal (`static char a[N] = "abc";`)
      // takes its own path of §6.7.8p14.
      const ir::Array arr_type = *arr;
      if (auto* s = std::get_if<ir::String>(&*vd.init);
          s != nullptr && is_char_element(*arr_type.element_type)) {
        check_string_array_init(*s, arr_type, vd.name);
        initial = Initial{string_to_value_tuple(s->str, arr_type.size)};
      } else {
        auto* list = std::get_if<ir::InitList>(&*vd.init);
        if (list == nullptr) {
          fail("static array " + pyrepr::string(vd.name) +
               " requires a brace-enclosed initializer (`{...}`)");
        }
        check_array_init_list(*list, arr_type, vd.name);
        initial = Initial{const_init_aggregate(*vd.init, vd.data_type, vd.name, &symbols, &types)};
      }
    } else if (asdl::isa<ir::Structure, ir::Union>(strip_quals(vd.data_type))) {
      // `static struct s x = {1, 2};`, or a structure constant in place
      // of the list.
      if (auto* list = std::get_if<ir::InitList>(&*vd.init)) {
        const Type struct_type = vd.data_type;
        check_struct_init_list(*list, struct_type, vd.name);
        initial = Initial{const_init_aggregate(*vd.init, vd.data_type, vd.name, &symbols, &types)};
      } else {
        auto value = structure_constant_init(vd);
        if (!value) {
          fail("static struct/union " + pyrepr::string(vd.name) +
               " requires a brace-enclosed initializer (`{...}`)");
        }
        initial = Initial{std::move(*value)};
      }
    } else {
      // As for a file-scope static: the initializer is checked and
      // converted to the declared type (so a constant of another variant
      // gets an implicit cast), and its value is read through the casts.
      // An array or a function decays to a pointer first, as it does at
      // file scope.
      Exp init = unbrace_scalar_init(std::move(*vd.init), vd.data_type,
                                     "scalar variable " + pyrepr::string(display_name(vd.name)));
      check_exp(init);
      init = decay_if_array(std::move(init));
      vd.init = convert_to(std::move(init), vd.data_type);
      initial =
          Initial{const_init_value(*vd.init, vd.data_type, vd.name, &symbols, &types)};
    }
    check_section(vd.name, vd.section, std::nullopt, vd.placement);
    StaticAttr attr;
    attr.initial_value = std::move(initial);
    attr.is_global = false;
    attr.zeropage = vd.placement.has_value();
    if (vd.placement) {
      if (const auto* zp = std::get_if<ir::ZeroPage>(&*vd.placement)) {
        attr.zp_address = zp->address;
      } else {
        attr.zp_address = std::get<ir::Absolute>(*vd.placement).address;
      }
    }
    attr.section = vd.section;
    symbols.insert_or_assign(vd.name, Symbol{.type = vd.data_type, .attrs = std::move(attr)});
    return;
  }
  // `int x;` or `long x;`, with automatic storage. The initializer is an
  // expression evaluated at run time, which is checked and converted to
  // the declared type, so the AST has an explicit cast for any change of
  // width, as for an assignment, a return, and an argument.
  const bool is_register = vd.storage_class && asdl::isa<ir::Register>(*vd.storage_class);
  if (is_register) {
    check_register_object(vd.data_type, 1, "register local " + pyrepr::string(vd.name));
  }
  symbols.insert_or_assign(vd.name,
                           Symbol{.type = vd.data_type,
                                  .attrs = LocalAttr{.register_ = is_register, .was_bool = false}});
  if (!vd.init) {
    return;
  }
  // C99 §6.7.3p8 puts the qualifiers of an array on its elements, and a
  // struct's ride at the object, so a top-level qualifier is stripped
  // for the dispatch on the aggregate's shape.
  const Type data_type_uq = strip_quals(vd.data_type);
  if (const auto* arr = std::get_if<ir::Array>(&data_type_uq)) {
    // An array takes a braced list (`int a[3] = {1, 2, 3};`). A char
    // array initialized by a string literal (`char a[N] = "abc";`)
    // takes its own path of §6.7.8p14, which c99_to_tac lowers to a
    // store per byte.
    if (auto* s = std::get_if<ir::String>(&*vd.init);
        s != nullptr && is_char_element(*arr->element_type)) {
      check_string_array_init(*s, *arr, vd.name);
      return;
    }
    auto* list = std::get_if<ir::InitList>(&*vd.init);
    if (list == nullptr) {
      fail("array " + pyrepr::string(vd.name) +
           " requires a brace-enclosed initializer (`{...}`)");
    }
    check_array_init_list(*list, *arr, vd.name);
    return;
  }
  if (asdl::isa<ir::Structure, ir::Union>(data_type_uq)) {
    // A struct or union takes a braced list, one item per member, or an
    // expression of the same struct type, which copies it.
    if (auto* list = std::get_if<ir::InitList>(&*vd.init)) {
      check_struct_init_list(*list, data_type_uq, vd.name);
      return;
    }
    check_exp(*vd.init);
    const std::optional<Type> init_t = data_type_of(*vd.init);
    if (!init_t || !types_equal(strip_quals(*init_t), data_type_uq)) {
      fail("struct/union " + pyrepr::string(vd.name) + ": initializer has type " +
           repr_or_none(init_t) + ", expected " + asdl::py_repr(vd.data_type));
    }
    return;
  }
  // A scalar's initializer may be in braces (`int x = {5};`) or be the
  // empty initializer (`int x = {};`), and `int x = {1, 2};` is
  // rejected.
  Exp init = unbrace_scalar_init(std::move(*vd.init), vd.data_type,
                                 "scalar variable " + pyrepr::string(display_name(vd.name)));
  check_exp(init);
  init = decay_if_array(std::move(init));
  vd.init = convert_to(std::move(init), vd.data_type);
}

// ---------------------------------------------------------------------
// `constexpr` objects (C23 §6.7.2).
// ---------------------------------------------------------------------

// The evaluator in constant_expression.cpp reads this through
// `named_constant`.
std::optional<NamedConstant> TypeChecker::named_constant(const Exp& exp) {
  if (const auto* v = std::get_if<ir::Var>(&exp)) {
    const auto it = constexpr_values.find(v->name);
    return it == constexpr_values.end() ? std::nullopt : std::optional<NamedConstant>(it->second);
  }
  if (const auto* c = std::get_if<ir::CompoundLiteral>(&exp)) {
    // A `constexpr` compound literal is a compound literal constant
    // (C23 §6.6p6), recorded by its name as an object is.
    const auto it = constexpr_values.find(c->name);
    return it == constexpr_values.end() ? std::nullopt : std::optional<NamedConstant>(it->second);
  }
  const auto* dot = std::get_if<ir::Dot>(&exp);
  if (dot == nullptr) {
    return std::nullopt;
  }
  const auto base = named_constant(*dot->operand);
  if (!base) {
    return std::nullopt;
  }
  const Type& st = strip_quals(base->type);
  if (!asdl::isa<ir::Structure, ir::Union>(st)) {
    return std::nullopt;
  }
  const StructLayout* layout = types.find(tag_of(st));
  if (layout == nullptr) {
    return std::nullopt;
  }
  const ValueTree& value = base->value;
  if (value.kind == ValueTree::Kind::kUnion) {
    // A designator initialized this member of the union.
    const MemberInfo& member = layout->members.at(static_cast<std::size_t>(value.member));
    if (member.name != dot->member) {
      throw ce::ConstantExpressionError(
          "member " + pyrepr::string(dot->member) +
          " of a union constant isn't the member its initializer sets, and only that one is a "
          "constant (C23 §6.6p16)");
    }
    return NamedConstant{member.type, value.items.at(0)};
  }
  for (std::size_t i = 0; i < layout->members.size(); ++i) {
    const MemberInfo& m = layout->members[i];
    if (m.name != dot->member) {
      continue;
    }
    if (value.kind != ValueTree::Kind::kTuple) {
      throw InternalError("TypeError: len() of a value tree that isn't a tuple");
    }
    if (i >= value.items.size()) {
      // Only a union's first member has a value, since its initializer
      // sets that one.
      throw ce::ConstantExpressionError(
          "member " + pyrepr::string(m.name) +
          " of a union constant isn't the member its initializer sets, and only that one is a "
          "constant (C23 §6.6p16)");
    }
    return NamedConstant{m.type, value.items[i]};
  }
  return std::nullopt;
}

// Returns the value tree of `vd`'s initializer when it is a structure or
// union constant of `vd`'s type, as in `constexpr struct s B = A;` with
// `A` declared `constexpr`, or none.
std::optional<ValueTree> TypeChecker::structure_constant_init(ir::Type_var_decl& vd) {
  check_exp(*vd.init);
  const auto named = ce::named_constant(*vd.init);
  if (!named || !types_equal(strip_quals(named->type), strip_quals(vd.data_type))) {
    return std::nullopt;
  }
  return named->value;
}

// Checks the rules of C23 §6.7.2p5-6 for the `constexpr` object `vd`
// that don't depend on its converted initializer: no volatile type in
// it, and each value of the initializer exactly representable in the
// type it initializes.
void TypeChecker::begin_constexpr(ir::Type_var_decl& vd) {
  const std::string name = display_name(vd.name);
  if (const auto volatile_desc = volatile_part(vd.data_type)) {
    fail("`constexpr` object " + pyrepr::string(name) + " has " + *volatile_desc +
         ", and a constexpr object can't have a volatile type, or a member of one (C23 "
         "§6.7.2p5)");
  }
  if (!vd.init) {
    throw InternalError("a constexpr object without an initializer");
  }
  check_constexpr_init(*vd.init, vd.data_type, name);
}

// Records the value of the `constexpr` object `vd`, which the normal
// checks have converted to its type, so that later named constants read
// it. Every pointer in it must be null (C23 §6.7.2p6).
void TypeChecker::end_constexpr(ir::Type_var_decl& vd) {
  const std::string name = display_name(vd.name);
  const Symbol* sym = symbols.find(vd.name);
  ValueTree value;
  const StaticAttr* attr = sym != nullptr ? std::get_if<StaticAttr>(&sym->attrs) : nullptr;
  if (attr != nullptr && std::holds_alternative<Initial>(attr->initial_value)) {
    value = std::get<Initial>(attr->initial_value).value;
  } else {
    if (!vd.init) {
      throw InternalError("a constexpr object without an initializer");
    }
    try {
      value = const_init_aggregate(*vd.init, vd.data_type, vd.name, &symbols, &types);
    } catch (const TypeCheckError& e) {
      fail("the initializer of `constexpr` object " + pyrepr::string(name) +
           " must be a constant expression (C23 §6.7.2p6): " +
           replace_all(e.what(), pyrepr::string(vd.name), pyrepr::string(name)));
    }
  }
  if (has_address(value)) {
    fail("`constexpr` object " + pyrepr::string(name) +
         " holds a pointer that isn't null, and a constexpr pointer must be null (C23 §6.7.2p6)");
  }
  constexpr_values.insert_or_assign(vd.name, NamedConstant{vd.data_type, std::move(value)});
}

// Returns whether the value tree `value` holds an address.
bool TypeChecker::has_address(const ValueTree& value) {
  switch (value.kind) {
    case ValueTree::Kind::kAddress:
      return true;
    case ValueTree::Kind::kUnion:
    case ValueTree::Kind::kTuple:
      for (const ValueTree& v : value.items) {
        if (has_address(v)) {
          return true;
        }
      }
      return false;
    default:
      return false;
  }
}

// Returns a description of the volatile part of the object type `t` (the
// object itself, an element, or a member at any depth), or none. What a
// pointer points to doesn't count.
std::optional<std::string> TypeChecker::volatile_part(const Type& t, std::set<std::string>* seen) {
  std::set<std::string> own;
  if (seen == nullptr) {
    seen = &own;
  }
  switch (ir::kind(t)) {
    case K::Volatile:
      return "the volatile type " + c_type(t);
    case K::Const:
      return volatile_part(*std::get<ir::Const>(t).referenced_type, seen);
    case K::Array:
      return volatile_part(*std::get<ir::Array>(t).element_type, seen);
    case K::Structure:
    case K::Union: {
      const std::string& tag = tag_of(t);
      if (seen->count(tag) != 0) {
        return std::nullopt;
      }
      seen->insert(tag);
      const StructLayout* found = types.find(tag);
      if (found == nullptr) {
        return std::nullopt;
      }
      const std::vector<MemberInfo> members = found->members;
      for (const MemberInfo& m : members) {
        if (volatile_part(m.type, seen)) {
          return "a volatile member " + pyrepr::string(m.name);
        }
      }
      return std::nullopt;
    }
    default:
      return std::nullopt;
  }
}

// Checks each value in the initializer `init` of an object of type `t`
// in the `constexpr` object `name`: an object of integer type takes an
// integer constant expression and one of real floating type an integer
// or real floating one, and the value must be exactly representable in
// the type, since a constexpr initializer can't change a value (C23
// §6.7.2p5-6). `end_constexpr` checks pointers later. The shapes the
// normal checks reject are left for them.
void TypeChecker::check_constexpr_init(const Exp& init, const Type& t, const std::string& name) {
  const Type st = strip_quals(t);
  if (const auto* arr = std::get_if<ir::Array>(&st)) {
    const auto* list = std::get_if<ir::InitList>(&init);
    const Exp* only = list == nullptr ? &init : (list->items.size() == 1 ? &list->items[0] : nullptr);
    if (only != nullptr) {
      if (const auto* s = std::get_if<ir::String>(only); s != nullptr &&
                                                          is_char_element(*arr->element_type)) {
        check_constexpr_string(*s, *arr->element_type, name);
        return;
      }
    }
    if (list != nullptr) {
      for (const Exp& item : list->items) {
        check_constexpr_init(item, *arr->element_type, name);
      }
    }
    return;
  }
  if (asdl::isa<ir::Structure, ir::Union>(st)) {
    const StructLayout* found = types.find(tag_of(st));
    const auto* list = std::get_if<ir::InitList>(&init);
    if (list == nullptr || found == nullptr) {
      return;
    }
    const StructLayout layout = *found;
    std::vector<MemberInfo> members;
    if (asdl::isa<ir::Union>(st)) {
      const auto k = static_cast<std::size_t>(union_member_index(layout, *list));
      if (k < layout.members.size()) {
        members.push_back(layout.members[k]);
      }
    } else {
      members = layout.members;
    }
    for (std::size_t i = 0; i < list->items.size() && i < members.size(); ++i) {
      check_constexpr_init(list->items[i], members[i].type, name);
    }
    return;
  }
  if (const auto* list = std::get_if<ir::InitList>(&init)) {
    if (list->items.size() == 1) {
      check_constexpr_init(list->items[0], t, name);
    }
    return;
  }
  if (!(is_integer_type(st) || is_floating_type(st))) {
    return;
  }
  // A compound literal in the copy is checked again by the normal
  // checks, which record the same object.
  Exp leaf = init;
  check_exp(leaf);
  leaf = decay_if_array(std::move(leaf));
  std::optional<Type> source;
  if (const auto& dt = data_type_of(leaf)) {
    source = strip_quals(*dt);
  }
  const auto failure = [&](const std::string& why) {
    fail("the initializer of `constexpr` object " + pyrepr::string(name) + " " + why);
  };
  if (source && is_integer_type(*source)) {
    Int value = 0;
    try {
      value = ce::evaluate_integer_constant_expression(leaf).first;
    } catch (const ce::ConstantExpressionError& e) {
      failure(std::string("isn't an integer constant expression (C23 §6.7.2p6): ") + e.what());
    }
    bool exact = false;
    if (is_integer_type(st)) {
      exact = asdl::isa<ir::Bool>(st) ? (value == Int(0) || value == Int(1))
                                      : coerce_int_to_type(value, st) == value;
    } else {
      const Int bits = coerce_init_value(value, *source, st);
      exact = coerce_init_value(bits, st, *source) == value;
    }
    if (!exact) {
      failure("has the value " + asdl::int_to_string(value) + ", which " + c_type(st) +
              " can't hold exactly, and a constexpr initializer can't change a value (C23 "
              "§6.7.2p5)");
    }
    return;
  }
  if (!source || !is_floating_type(*source)) {
    failure("has type " + c_type_or_none(source) + ", and an object of type " + c_type(st) +
            " takes an arithmetic constant expression (C23 §6.7.2p6)");
  }
  if (is_integer_type(st)) {
    failure("has the floating type " + c_type(*source) +
            ", and an object of integer type takes an integer constant expression (C23 "
            "§6.7.2p6)");
  }
  const ValueTree bits = const_init_value(leaf, *source, name, &symbols, &types);
  if (bits.kind != ValueTree::Kind::kInt) {
    throw InternalError("a floating initializer whose value isn't an integer");
  }
  const Int converted = coerce_init_value(bits.integer, *source, st);
  if (coerce_init_value(converted, st, *source) != bits.integer) {
    failure("has a " + c_type(*source) + " value that " + c_type(st) +
            " can't hold exactly, and a constexpr initializer can't change a value (C23 "
            "§6.7.2p5)");
  }
}

// Each character of the string literal `s` must keep its value in the
// `constexpr` array's element type (C23 §6.7.2p5). A string's characters
// have type `char`, which is unsigned in c6502, so one above 127 changes
// value in a `signed char`.
void TypeChecker::check_constexpr_string(const ir::String& s, const Type& element,
                                         const std::string& name) {
  if (!is_signed(strip_quals(element))) {
    return;
  }
  for (const std::uint32_t ch : pyrepr::code_points(s.str)) {
    if (ch > 127) {
      fail("the string that initializes `constexpr` array " + pyrepr::string(name) +
           " has the character " + std::to_string(ch) + ", which " +
           c_type(strip_quals(element)) +
           " can't hold, and a constexpr initializer can't change a value (C23 §6.7.2p5)");
    }
  }
}

// `(T){...}` (C99 §6.5.2.5) declares an unnamed object named `exp.name`,
// so it is checked as the declaration `T name = {...};` would be. Inside
// a function the object is automatic (§6.5.2.5p6), and a static
// initializer can't take its address. At file scope it is static, so its
// initializer must be constant (§6.5.2.5p3). An array of unknown size
// takes its size from the initializer, as `int a[] = {1, 2};` would
// (§6.7.8p22).
Type TypeChecker::check_compound_literal(ir::CompoundLiteral& exp) {
  *exp.init = normalize_init(std::move(*exp.init), exp.target_type, std::nullopt);
  const Type t = size_from_initializer(exp.target_type, &*exp.init, "the compound literal's array");
  exp.target_type = t;
  const bool at_file_scope = !return_type.has_value();
  std::optional<ir::Type_storage_class> storage = exp.storage_class;
  if (at_file_scope) {
    // C23 §6.5.3.6p4 checks a compound literal's storage class as a
    // declaration's at its scope, and a file-scope object can't be
    // `register`.
    if (storage && asdl::isa<ir::Register>(*storage)) {
      fail("a compound literal at file scope can't be `register` (C23 §6.5.3.6p4)");
    }
    storage = ir::Static{};
  }
  ir::Type_var_decl vd;
  vd.name = exp.name;
  vd.init = *exp.init;
  vd.data_type = t;
  vd.storage_class = storage;
  vd.is_constexpr = exp.is_constexpr;
  try {
    check_block_var(vd);
  } catch (const TypeCheckError& e) {
    // The object's name is one identifier_resolution made up.
    std::string message = replace_all(e.what(), pyrepr::string(exp.name), "'(compound literal)'");
    message = replace_all(std::move(message), pyrepr::string(display_name(exp.name)),
                          "'(compound literal)'");
    fail(message);
  }
  *exp.init = std::move(*vd.init);
  exp.data_type = t;
  return t;
}

// Checks `char arr[N] = "abc";` (C99 §6.7.8p14). The literal's body has
// `len(init.str)` bytes, without the terminator. The standard allows the
// null too when N is at least that plus one, and leaves it out when N is
// exactly the length. A shorter N is a constraint violation. Python then
// stamps the String with `Array(Char, len + 1)`, an attribute the c99
// IR's `String` doesn't have, which neither the dump format nor any
// later pass reads, so the port leaves it out.
void TypeChecker::check_string_array_init(ir::String& init, const ir::Array& arr_type,
                                          const std::string& name) {
  const Int slen = as_int(code_point_count(init.str));
  if (slen > arr_type.size) {
    fail("string literal initializer for array " + pyrepr::string(name) + " has " +
         asdl::int_to_string(slen) + " bytes but the declared array size is " +
         asdl::int_to_string(arr_type.size) +
         " (C99 §6.7.8.14 requires the literal length to be ≤ the array size, with the null "
         "terminator omitted when the array has no room for it)");
  }
}

// Checks a braced initializer for a struct or union: no more items than
// members, and each item converted to its member's type with
// `convert_to`, which replaces `init.items` in place. The qualifiers of
// `const struct s x = {...};` don't change the layout, so they are
// stripped first.
void TypeChecker::check_struct_init_list(ir::InitList& init, const Type& struct_type0,
                                         const std::string& var_name) {
  const Type struct_type = strip_quals(struct_type0);
  const StructLayout* found = types.find(tag_of(struct_type));
  if (found == nullptr || !found->complete) {
    fail("struct/union " + pyrepr::string(var_name) + ": incomplete type '" +
         struct_keyword(struct_type) + " " + tag_of(struct_type) + "'");
  }
  // A copy, since checking an item can add to the type table.
  const StructLayout layout = *found;
  std::vector<MemberInfo> members;
  if (asdl::isa<ir::Union>(struct_type)) {
    // Per C99 §6.7.8p16, a list without designators initializes the
    // first named member of a union, so one item at most is allowed. A
    // designator may have chosen another member, which
    // `init.union_member` names.
    const auto k = static_cast<std::size_t>(union_member_index(layout, init));
    if (k < layout.members.size()) {
      members.push_back(layout.members[k]);
    }
    if (init.items.size() > 1) {
      fail("too many initializers for union " + pyrepr::string(var_name) + ": " +
           std::to_string(init.items.size()) +
           " given, only one (the first named member) is permitted without designators");
    }
  } else {
    members = layout.members;
    if (init.items.size() > members.size()) {
      fail("too many initializers for struct " + pyrepr::string(var_name) + ": " +
           std::to_string(init.items.size()) + " given, struct has " +
           std::to_string(members.size()) + " member" + (members.size() != 1 ? "s" : ""));
    }
  }
  for (std::size_t i = 0; i < init.items.size(); ++i) {
    if (i >= members.size()) {
      throw InternalError("IndexError: list index out of range");
    }
    const MemberInfo& m = members[i];
    const Type& elem_type = m.type;
    Exp& item = init.items[i];
    if (const auto* arr = std::get_if<ir::Array>(&strip_quals(elem_type))) {
      // A char-array member initialized by a string literal.
      if (auto* s = std::get_if<ir::String>(&item); s != nullptr &&
                                                    is_char_element(*arr->element_type)) {
        check_string_array_init(*s, *arr, var_name + "." + m.name);
        continue;
      }
      auto* list = std::get_if<ir::InitList>(&item);
      if (list == nullptr) {
        fail("expected nested initializer (`{...}`) for member " + pyrepr::string(m.name) +
             " of " + pyrepr::string(var_name) + "; member type is " + asdl::py_repr(elem_type));
      }
      check_array_init_list(*list, *arr, var_name);
    } else if (asdl::isa<ir::Structure, ir::Union>(strip_quals(elem_type))) {
      auto* list = std::get_if<ir::InitList>(&item);
      if (list == nullptr) {
        fail("expected nested initializer (`{...}`) for member " + pyrepr::string(m.name) +
             " of " + pyrepr::string(var_name) + "; member type is " + asdl::py_repr(elem_type));
      }
      check_struct_init_list(*list, elem_type, var_name + "." + m.name);
    } else {
      // A scalar member's initializer may be braced, as in `{ {5}, 1 }`,
      // or empty, as in `{ {}, 1 }`.
      Exp scalar = unbrace_scalar_init(
          std::move(item), elem_type,
          "member " + pyrepr::string(m.name) + " of " + pyrepr::string(display_name(var_name)));
      check_exp(scalar);
      scalar = decay_if_array(std::move(scalar));
      init.items[i] = convert_to(std::move(scalar), elem_type);
    }
  }
  init.data_type = struct_type;
}

// Checks a braced initializer for an array: no more items than elements
// (c99_to_tac pads a shorter list with zeros), and each item converted
// to the element type with `convert_to`, so a constant of another width
// gets an implicit cast. It replaces `init.items` in place and stamps
// `init.data_type` with the array type. A multi-dimensional array
// (`int a[2][3] = {{1,2,3},{4,5,6}};`) takes a nested list for each
// element, which the recursion checks. C99 also allows the flat form
// `{1,2,3,4,5,6}` by §6.7.8p20's rule for subaggregates, which takes a
// grouping pass at parse time that c6502 doesn't have, so only the fully
// nested form is accepted.
void TypeChecker::check_array_init_list(ir::InitList& init, const ir::Array& arr_type0,
                                        const std::string& var_name) {
  const ir::Array arr_type = arr_type0;
  if (as_int(init.items.size()) > arr_type.size) {
    fail("too many initializers for array " + pyrepr::string(var_name) + ": " +
         std::to_string(init.items.size()) + " given, array has " +
         asdl::int_to_string(arr_type.size) + " element" +
         (arr_type.size != Int(1) ? "s" : ""));
  }
  const Type& elem_type = *arr_type.element_type;
  for (std::size_t i = 0; i < init.items.size(); ++i) {
    Exp& item = init.items[i];
    const std::string index = std::to_string(i);
    if (const auto* elem_arr = std::get_if<ir::Array>(&elem_type)) {
      // Each item is a nested list, or a string literal when the inner
      // array's element type is a character type, since a char array
      // may be initialized by a string literal at any depth (C99
      // §6.7.8p14). The flat form (`{1,2,3,4,5,6}` for `int[2][3]`)
      // isn't supported.
      if (auto* s = std::get_if<ir::String>(&item); s != nullptr &&
                                                    is_char_element(*elem_arr->element_type)) {
        check_string_array_init(*s, *elem_arr, var_name + "[" + index + "]");
        continue;
      }
      auto* list = std::get_if<ir::InitList>(&item);
      if (list == nullptr) {
        fail("expected nested initializer (`{...}`) at index " + index + " of " +
             pyrepr::string(var_name) + "; element type is " + asdl::py_repr(elem_type));
      }
      check_array_init_list(*list, *elem_arr, var_name);
    } else if (asdl::isa<ir::Structure, ir::Union>(strip_quals(elem_type))) {
      // An array of structs or unions takes a nested list for each
      // element.
      auto* list = std::get_if<ir::InitList>(&item);
      if (list == nullptr) {
        fail("expected nested initializer (`{...}`) at index " + index + " of " +
             pyrepr::string(var_name) + "; element type is " + asdl::py_repr(elem_type));
      }
      check_struct_init_list(*list, elem_type, var_name + "[" + index + "]");
    } else {
      // A scalar element's initializer may be braced, as in `{ {5}, 1 }`,
      // or empty, as in `{ {}, 1 }`. It decays first, as a scalar's
      // initializer and an assignment's right side do, and then
      // `convert_to` wraps it in an implicit cast when its type differs.
      Exp scalar = unbrace_scalar_init(
          std::move(item), elem_type,
          "element " + index + " of " + pyrepr::string(display_name(var_name)));
      check_exp(scalar);
      scalar = decay_if_array(std::move(scalar));
      init.items[i] = convert_to(std::move(scalar), elem_type);
    }
  }
  init.data_type = Type(arr_type);
}

}  // namespace c6502::passes::type_checking
