// The lowering of increments, compound assignments, casts, the
// bit-precise padding, conversions to `bool`, and the short-circuit
// operators, and the maps of the operators, part A2 of c99_to_tac
// (src/c99_to_tac_translator.h).

#include <algorithm>
#include <string>
#include <utility>

#include "bit_precise.h"
#include "c99_to_tac_translator.h"
#include "errors.h"

namespace c6502::c99_to_tac {

namespace {

using TK = c99::Kind_data_type;
using EK = c99::Kind_exp;

// Returns whether the TAC operation on values of the bit-precise type
// `t` can give a result whose padding bits aren't the sign or zero
// extension of its N bits, when its operands' padding bits are. `+`,
// `-`, `*`, and `<<` carry into the padding, a signed `/` does for the
// minimum divided by -1, and unary `-` does for 0 minus anything. `~` of
// an unsigned value sets the padding, while `~` of a sign-extended value
// is sign-extended. `&`, `|`, `^`, `%`, `>>`, and an unsigned `/` keep
// the padding as it is.
bool changes_padding(const tac::Type_binary_operator* binop, const tac::Type_unary_operator* unop,
                     const c99::BitInt& t) {
  if (binop != nullptr) {
    if (asdl::isa<tac::Add, tac::Subtract, tac::Multiply, tac::LeftShift>(*binop)) {
      return true;
    }
    if (std::holds_alternative<tac::Divide>(*binop)) {
      return t.is_signed;
    }
    return false;
  }
  if (unop != nullptr) {
    if (std::holds_alternative<tac::Negate>(*unop)) {
      return true;
    }
    if (std::holds_alternative<tac::Complement>(*unop)) {
      return !t.is_signed;
    }
  }
  return false;
}

// The raw `value` of an integer constant, as Python reads
// `inner_val.const.value`.
asdl::Int raw_const_value(const Val& v) {
  const auto raw = bit_precise::raw_value(std::get<tac::Constant>(v).const_);
  if (!raw) {
    throw InternalError("the value of a floating constant");
  }
  return *raw;
}

}  // namespace

// Lowers `++operand`, `--operand`, `operand++`, and `operand--` as a
// read-modify-write of the operand's storage, and returns the old value
// (`return_old`, the postfix forms) or the new one. A variable is read
// and written in place, and the postfix form copies the old value into
// a temporary first, so the old value gets the lower number. Any other
// lvalue (`a[i]`, `*p`, `s.m`, `p->m`) has its address computed once,
// which is what makes `++arr[--i]` increment at the decremented index,
// then a Load, the step, and a Store through the same address. The type
// checker required an arithmetic or pointer operand, and each temporary
// takes the operand's type.
Val Translator::translate_incdec(const c99::Type_incdec_op& op, const Exp& operand, Instrs& instrs,
                                 bool return_old) {
  const std::optional<Type>& dt = data_type_of(operand);
  const Type op_type = dt ? *dt : Type{c99::Int{}};
  if (volatile_scalar(operand)) {
    // One volatile read and one volatile write.
    const std::string& name = std::get<c99::Var>(operand).name;
    const tac::Var cur = read_volatile_var(name, instrs);
    const Val next = emit_incdec_step(op, cur, op_type, instrs);
    write_volatile_var(name, next, instrs);
    return return_old ? Val{cur} : next;
  }
  if (const auto* v = std::get_if<c99::Var>(&operand)) {
    const tac::Var var{.name = v->name};
    std::optional<Val> old;
    if (return_old) {
      tac::Var o = temp_var(op_type);
      instrs.push_back(tac::Copy{.src = var, .dst = o});
      old = o;
    }
    const Val next = emit_incdec_step(op, var, op_type, instrs);
    instrs.push_back(tac::Copy{.src = next, .dst = var});
    return return_old ? *old : next;
  }
  Val addr = [&]() -> Val {
    switch (c99::kind(operand)) {
      case EK::Subscript: {
        const auto& s = std::get<c99::Subscript>(operand);
        return translate_subscript_address(*s.array, *s.index, instrs);
      }
      case EK::Dereference:
        return translate_deref_address(*std::get<c99::Dereference>(operand).exp, instrs);
      case EK::Dot: {
        const auto& d = std::get<c99::Dot>(operand);
        return translate_dot_address(*d.operand, d.member, instrs);
      }
      case EK::Arrow: {
        const auto& d = std::get<c99::Arrow>(operand);
        return translate_arrow_address(*d.operand, d.member, instrs);
      }
      default:
        throw InternalError(
            "increment/decrement operand must be Var, Subscript, Dereference, Dot, or Arrow "
            "(identifier_resolution should have enforced this)");
    }
  }();
  const tac::Var cur = temp_var(op_type);
  const bool is_vol = is_volatile_quals(op_type);
  instrs.push_back(tac::Load{.src_ptr = addr, .dst = cur, .is_volatile = is_vol});
  const Val next = emit_incdec_step(op, cur, op_type, instrs);
  instrs.push_back(tac::Store{.src = next, .dst_ptr = addr, .is_volatile = is_vol});
  return return_old ? Val{cur} : next;
}

// Computes `src ± 1` at the operand's type. A pointer's step goes
// through `translate_pointer_arithmetic`, so it scales by the size of
// what the pointer points to (C99 §6.5.6p8), and any other operand's is
// a `Binary(Add | Subtract, src, 1)`. A `bool` operand converts `src ±
// 1` back to `bool` (C99 §6.5.2.4p2), so `++` always stores 1 and `--`
// stores `src == 0`.
Val Translator::emit_incdec_step(const c99::Type_incdec_op& op, const Val& src, const Type& op_type,
                                 Instrs& instrs) {
  const Type b = c99::Bool{};
  if (std::holds_alternative<c99::Bool>(strip_quals(op_type))) {
    if (std::holds_alternative<c99::Increment>(op)) {
      return tac_const_val(b, 1);
    }
    tac::Var next = temp_var(b);
    instrs.push_back(tac::Binary{.op = tac::Equal{}, .src1 = src, .src2 = tac_const_val(b, 0), .dst = next});
    return next;
  }
  if (std::holds_alternative<c99::Pointer>(unqualified(op_type))) {
    const c99::Type_binary_operator c99_op = std::holds_alternative<c99::Increment>(op)
                                                 ? c99::Type_binary_operator{c99::Add{}}
                                                 : c99::Type_binary_operator{c99::Subtract{}};
    return translate_pointer_arithmetic(c99_op, src, tac_const_val(c99::Int{}, 1), op_type,
                                        c99::Int{}, op_type, instrs);
  }
  tac::Var next = temp_var(op_type);
  const tac::Type_binary_operator tac_op = translate_incdec_op(op);
  instrs.push_back(tac::Binary{.op = tac_op, .src1 = src, .src2 = tac_const_val(op_type, 1), .dst = next});
  return restore_padding(next, op_type, &tac_op, nullptr, instrs);
}

// Lowers `lval OP= rval` as a read-modify-write of the lvalue's storage,
// with the operation at the intermediate type the type checker stamped.
// The address of `a[i]`, `*p`, `s.m`, or `p->m` is computed once, so the
// side effects of `arr[i++] += 1`, `(*p++)++`, and `ptr++[idx++] *= 3`
// happen once, and a variable is read and written in place.
//
// The lvalue has its type T, and the type checker cast the right
// operand to the intermediate type C, so `rval`'s type is C. The value
// of the lvalue is converted to C, the operation gives a C (a shift's
// intermediate type is its promoted left operand), and the result is
// converted back to T for the store. `ptr += n` and `ptr -= n` go
// through `translate_pointer_arithmetic`, which scales `n`. The value
// returned is the lvalue's new value, of type T, so `b = (a += 5)`
// composes.
Val Translator::translate_compound_assign(const c99::Type_binary_operator& op, const Exp& lval,
                                          const Exp& rval, const std::optional<Type>& intermediate,
                                          Instrs& instrs) {
  const std::optional<Type>& dt = data_type_of(lval);
  const Type lv_type = dt ? *dt : Type{c99::Int{}};
  const Type intermediate_type = intermediate ? *intermediate : lv_type;
  const bool is_pointer_arith = asdl::isa<c99::Add, c99::Subtract>(op) &&
                                std::holds_alternative<c99::Pointer>(unqualified(lv_type));
  if (volatile_scalar(lval)) {
    // One volatile read and one volatile write.
    const std::string& name = std::get<c99::Var>(lval).name;
    const tac::Var cur = read_volatile_var(name, instrs);
    const Val next =
        compute_compound_step(op, cur, lv_type, rval, intermediate_type, is_pointer_arith, instrs);
    write_volatile_var(name, next, instrs);
    return next;
  }
  if (const auto* v = std::get_if<c99::Var>(&lval)) {
    const tac::Var lval_var{.name = v->name};
    const Val next = compute_compound_step(op, lval_var, lv_type, rval, intermediate_type,
                                           is_pointer_arith, instrs);
    instrs.push_back(tac::Copy{.src = next, .dst = lval_var});
    return next;
  }
  std::optional<Val> rval_val;
  if (calls_first(lval, rval)) {
    rval_val = translate_value(rval, instrs);
  }
  Val addr = [&]() -> Val {
    switch (c99::kind(lval)) {
      case EK::Subscript: {
        const auto& s = std::get<c99::Subscript>(lval);
        return translate_subscript_address(*s.array, *s.index, instrs);
      }
      case EK::Dereference:
        return translate_deref_address(*std::get<c99::Dereference>(lval).exp, instrs);
      case EK::Dot: {
        const auto& d = std::get<c99::Dot>(lval);
        return translate_dot_address(*d.operand, d.member, instrs);
      }
      case EK::Arrow: {
        const auto& d = std::get<c99::Arrow>(lval);
        return translate_arrow_address(*d.operand, d.member, instrs);
      }
      default:
        throw InternalError(
            "compound-assignment lval must be Var, Subscript, Dereference, Dot, or Arrow "
            "(identifier_resolution should have enforced this)");
    }
  }();
  const tac::Var cur = temp_var(lv_type);
  const bool is_vol = is_volatile_quals(lv_type);
  instrs.push_back(tac::Load{.src_ptr = addr, .dst = cur, .is_volatile = is_vol});
  const Val next = compute_compound_step(op, cur, lv_type, rval, intermediate_type,
                                         is_pointer_arith, instrs, std::move(rval_val));
  instrs.push_back(tac::Store{.src = next, .dst_ptr = addr, .is_volatile = is_vol});
  return next;
}

// Computes `(T)((C)lval_val OP rval)` and returns the value of type T,
// the lvalue's type. `rval_val` is the right operand's value when the
// caller has evaluated it already.
Val Translator::compute_compound_step(const c99::Type_binary_operator& op, const Val& lval_val,
                                      const Type& lv_type, const Exp& rval_exp,
                                      const Type& intermediate_type, bool is_pointer_arith,
                                      Instrs& instrs, std::optional<Val> rval_val) {
  if (!rval_val) {
    rval_val = translate_value(rval_exp, instrs);
  }
  if (is_pointer_arith) {
    // `ptr += n` scales `n` by the size of what `ptr` points to.
    const std::optional<Type>& rt = data_type_of(rval_exp);
    return translate_pointer_arithmetic(op, lval_val, *rval_val, lv_type,
                                        rt ? *rt : Type{c99::Int{}}, lv_type, instrs);
  }
  const Val at_inter = emit_widen_or_narrow(lval_val, lv_type, intermediate_type, instrs);
  tac::Var binop_dst = temp_var(intermediate_type);
  const tac::Type_binary_operator tac_op = translate_binop(op);
  instrs.push_back(tac::Binary{.op = tac_op, .src1 = at_inter, .src2 = *rval_val, .dst = binop_dst});
  const Val result = restore_padding(binop_dst, intermediate_type, &tac_op, nullptr, instrs);
  return emit_widen_or_narrow(result, intermediate_type, lv_type, instrs);
}

// Converts `inner_val`, of type `source`, to `target`, appending the TAC
// conversions to `instrs`, and returns the converted value. This is the
// lowering of a `Cast`, and compound assignment's conversions use it
// too. The caller has already handled `source == target`.
//
// A conversion acts on the value, which has the unqualified type (C99
// §6.3.2.1p2), so `const` and `volatile` are stripped first. Without
// that, a `volatile signed char` didn't match `SChar` and was
// zero-extended.
Val Translator::emit_cast(const Val& inner_val_in, const Type& source_in, const Type& target_in,
                          Instrs& instrs) {
  Type source = strip_quals(source_in);
  const Type target = strip_quals(target_in);
  Val inner_val = inner_val_in;
  if (std::holds_alternative<c99::Bool>(target)) {
    return emit_to_bool(inner_val, source, instrs);
  }
  if (std::holds_alternative<c99::BitInt>(source) || std::holds_alternative<c99::BitInt>(target)) {
    return emit_bit_precise_cast(inner_val, source, target, instrs);
  }
  bool src_fp = asdl::isa<c99::Float, c99::Double>(source);
  const bool tgt_fp = asdl::isa<c99::Float, c99::Double>(target);
  // The temporary holds the converted value, of the cast's target type.
  tac::Var dst = temp_var(target);
  if (src_fp || tgt_fp) {
    // A constant operand is folded here, which saves a call of a runtime
    // helper. Its signedness is in its TAC class, so the fold needs only
    // the target type.
    if (std::holds_alternative<tac::Constant>(inner_val)) {
      return tac::Constant{
          .const_ = fold_fp_cast_constant(target, std::get<tac::Constant>(inner_val).const_)};
    }
    // The runtime's floating conversions take integers of 2, 4, and 8
    // bytes, keyed by the c99 type, and none of 1 byte. So `(double)c`
    // sign-extends a `signed char` to `int` (or zero-extends a `char`,
    // `unsigned char`, or `bool` to `unsigned int`) first, and
    // `(char)d` converts to `unsigned int` (or `int` for a `signed
    // char`) and truncates, as the promotions do for arithmetic.
    const bool char_src = asdl::isa<c99::Char, c99::SChar, c99::UChar, c99::Bool>(source);
    const bool char_tgt = asdl::isa<c99::Char, c99::SChar, c99::UChar>(target);
    if (char_src) {
      // Plain `char` and `unsigned char` are unsigned in c6502, and only
      // `signed char` is signed.
      const bool src_unsigned = asdl::isa<c99::Char, c99::UChar, c99::Bool>(source);
      const Type widened_t = src_unsigned ? Type{c99::UInt{}} : Type{c99::Int{}};
      tac::Var widened = temp_var(widened_t);
      if (src_unsigned) {
        instrs.push_back(tac::ZeroExtend{.src = inner_val, .dst = widened});
      } else {
        instrs.push_back(tac::SignExtend{.src = inner_val, .dst = widened});
      }
      inner_val = widened;
      source = widened_t;
      src_fp = false;
    }
    if (char_tgt) {
      const bool tgt_unsigned = asdl::isa<c99::Char, c99::UChar>(target);
      const Type narrow_t = tgt_unsigned ? Type{c99::UInt{}} : Type{c99::Int{}};
      tac::Var intermediate = temp_var(narrow_t);
      if (std::holds_alternative<c99::Float>(source)) {
        instrs.push_back(tac::FloatToInt{.src = inner_val, .dst = intermediate});
      } else {
        instrs.push_back(tac::DoubleToInt{.src = inner_val, .dst = intermediate});
      }
      instrs.push_back(tac::Truncate{.src = intermediate, .dst = dst});
      return dst;
    }
    // Otherwise `tac_to_asm` picks the helper (signed or unsigned, 2, 4,
    // or 8 bytes on the integer side, Float or Double on the floating
    // side) from the types of the operands in the symbol table.
    if (src_fp && tgt_fp) {
      // Float to Double or back, since the same precision was caught by
      // the `source == target` test.
      if (std::holds_alternative<c99::Float>(source)) {
        instrs.push_back(tac::FloatToDouble{.src = inner_val, .dst = dst});
      } else {
        instrs.push_back(tac::DoubleToFloat{.src = inner_val, .dst = dst});
      }
    } else if (src_fp) {
      if (std::holds_alternative<c99::Float>(source)) {
        instrs.push_back(tac::FloatToInt{.src = inner_val, .dst = dst});
      } else {
        instrs.push_back(tac::DoubleToInt{.src = inner_val, .dst = dst});
      }
    } else if (std::holds_alternative<c99::Float>(target)) {
      instrs.push_back(tac::IntToFloat{.src = inner_val, .dst = dst});
    } else {
      instrs.push_back(tac::IntToDouble{.src = inner_val, .dst = dst});
    }
    return dst;
  }
  const int src_w = byte_width_of(source);
  const int tgt_w = byte_width_of(target);
  if (src_w == tgt_w) {
    // The same width with a different signedness. The bits don't change,
    // but the value's type does: `tac_to_asm` picks a signed or unsigned
    // `/`, `%`, `>>`, ordering, and widening from the operand's declared
    // type. A constant is retagged with the target's class, and a
    // variable is copied into a temporary of the target type, so
    // `(unsigned)s >> 15` shifts logically.
    if (is_unsigned_integer(source) == is_unsigned_integer(target)) {
      return inner_val;
    }
    if (std::holds_alternative<tac::Constant>(inner_val)) {
      return tac_const_val(
          target, reinterpret_int(raw_const_value(inner_val), tgt_w, !is_unsigned_integer(target)));
    }
    instrs.push_back(tac::Copy{.src = inner_val, .dst = dst});
    return dst;
  }
  if (src_w < tgt_w) {
    if (asdl::isa<c99::Int, c99::Long, c99::LongLong, c99::SChar>(source)) {
      instrs.push_back(tac::SignExtend{.src = inner_val, .dst = dst});
    } else {
      // An unsigned source fills the new high bytes with zeros. Plain
      // `char` takes this path, since it's unsigned in c6502.
      instrs.push_back(tac::ZeroExtend{.src = inner_val, .dst = dst});
    }
  } else {
    instrs.push_back(tac::Truncate{.src = inner_val, .dst = dst});
  }
  return dst;
}

// Returns `val`, the result of the TAC operation `binop` or `unop` at
// type `t`, with its padding bits brought back to the sign or zero
// extension of its N bits when `t` is a bit-precise type whose top byte
// has padding and the operation can change it (`changes_padding`,
// bit_precise.py). Any other `val` is returned as it is.
Val Translator::restore_padding(const Val& val, const std::optional<Type>& t_in,
                                const tac::Type_binary_operator* binop,
                                const tac::Type_unary_operator* unop, Instrs& instrs) {
  if (!t_in) {
    return val;
  }
  const Type t = strip_quals(*t_in);
  const auto* b = std::get_if<c99::BitInt>(&t);
  if (b == nullptr || !bit_precise::has_padding(*b) || !changes_padding(binop, unop, *b)) {
    return val;
  }
  return wrap_bit_precise(val, *b, instrs);
}

// Returns `val`, an integer of `t`'s storage width, reduced modulo 2^N
// and stored the way `t` keeps its padding bits: masked with `m` = 2^N -
// 1 for an unsigned type, and `((val & m) ^ s) - s` with the sign bit `s`
// = 2^(N-1) for a signed one, which copies bit N-1 into every bit above
// it. A constant folds. A value of a type without padding is returned as
// it is.
Val Translator::wrap_bit_precise(const Val& val, const c99::BitInt& t, Instrs& instrs) {
  if (!bit_precise::has_padding(t)) {
    return val;
  }
  if (std::holds_alternative<tac::Constant>(val)) {
    return tac_const_val(t, bit_precise::const_value(std::get<tac::Constant>(val).const_));
  }
  const auto step = [&](tac::Type_binary_operator op, const Val& src, asdl::Int k) -> Val {
    tac::Var dst = temp_var(Type{t});
    instrs.push_back(tac::Binary{.op = std::move(op), .src1 = src, .src2 = tac_storage_const(t, k), .dst = dst});
    return dst;
  };
  const int width = static_cast<int>(t.width);
  const Val masked = step(tac::BitwiseAnd{}, val, (asdl::Int(1) << width) - 1);
  if (!t.is_signed) {
    return masked;
  }
  const asdl::Int sign = asdl::Int(1) << (width - 1);
  const Val flipped = step(tac::BitwiseXor{}, masked, sign);
  return step(tac::Subtract{}, flipped, sign);
}

// Lowers a conversion with a bit-precise source or target, neither of
// them `bool` or qualified. Between integers, the value is brought to
// the target's storage width as any integer is (the source's storage
// already holds its value, so it's extended by the source's
// signedness), and then reduced modulo 2^N when the target is
// bit-precise with padding and the source has values it can't hold (C23
// §6.3.1.3; a signed target wraps, c6502's implementation-defined
// choice).
//
// The runtime's conversions to and from `float` and `double` take
// integers of 2, 4, and 8 bytes, so a bit-precise source of another size
// is extended to the next of those first, and a bit-precise target is
// converted at that size and then truncated and wrapped.
Val Translator::emit_bit_precise_cast(const Val& inner_val, const Type& source, const Type& target,
                                      Instrs& instrs) {
  const bool src_fp = asdl::isa<c99::Float, c99::Double>(source);
  const bool tgt_fp = asdl::isa<c99::Float, c99::Double>(target);
  const auto helper_type = [](const Type& t) -> Type {
    const int n = std::max(2, bit_precise::helper_bytes(byte_width_of(t)));
    return bit_precise::storage_type(c99::BitInt{.width = 8 * n, .is_signed = !is_unsigned_integer(t)});
  };
  if (const auto* c = std::get_if<tac::Constant>(&inner_val)) {
    if (src_fp || tgt_fp) {
      return tac::Constant{.const_ = fold_fp_cast_constant(target, c->const_)};
    }
    return tac_const_val(target, bit_precise::const_value(c->const_));
  }
  if (tgt_fp) {
    const Type wide_t = helper_type(source);
    const Val wide = emit_cast(inner_val, source, wide_t, instrs);
    return emit_cast(wide, wide_t, target, instrs);
  }
  if (src_fp) {
    const Type wide_t = helper_type(target);
    tac::Var wide = temp_var(wide_t);
    if (std::holds_alternative<c99::Float>(source)) {
      instrs.push_back(tac::FloatToInt{.src = inner_val, .dst = wide});
    } else {
      instrs.push_back(tac::DoubleToInt{.src = inner_val, .dst = wide});
    }
    return emit_bit_precise_cast(wide, wide_t, target, instrs);
  }
  const int src_w = byte_width_of(source);
  const int tgt_w = byte_width_of(target);
  const bool src_unsigned = is_unsigned_integer(source);
  Val val = inner_val;
  if (!(src_w == tgt_w && src_unsigned == is_unsigned_integer(target))) {
    tac::Var v = temp_var(target);
    if (src_w == tgt_w) {
      instrs.push_back(tac::Copy{.src = inner_val, .dst = v});
    } else if (src_w > tgt_w) {
      instrs.push_back(tac::Truncate{.src = inner_val, .dst = v});
    } else if (src_unsigned) {
      instrs.push_back(tac::ZeroExtend{.src = inner_val, .dst = v});
    } else {
      instrs.push_back(tac::SignExtend{.src = inner_val, .dst = v});
    }
    val = v;
  }
  if (const auto* b = std::get_if<c99::BitInt>(&target)) {
    const auto [lo, hi] = bit_precise::value_range(static_cast<int>(b->width), b->is_signed);
    const auto src_range = int_range(source);
    if (!src_range || src_range->first < lo || src_range->second > hi) {
      return wrap_bit_precise(val, *b, instrs);
    }
  }
  return val;
}

// Converts `val`, a scalar of type `source`, to `bool`: 0 if it compares
// equal to 0, else 1 (C99 §6.3.1.2). A constant folds, and anything else
// is `val != 0` into a 1-byte temporary.
Val Translator::emit_to_bool(const Val& val, const Type& source, Instrs& instrs) {
  const Type b = c99::Bool{};
  if (const auto* c = std::get_if<tac::Constant>(&val)) {
    bool nonzero = false;
    if (const auto* f = std::get_if<tac::ConstFloat>(&c->const_)) {
      nonzero = (f->bits & asdl::Int(0x7FFFFFFF)) != 0;
    } else if (const auto* d = std::get_if<tac::ConstDouble>(&c->const_)) {
      nonzero = (d->bits & asdl::Int(0x7FFFFFFFFFFFFFFFll)) != 0;
    } else {
      nonzero = raw_const_value(val) != 0;
    }
    return tac_const_val(b, nonzero ? 1 : 0);
  }
  tac::Var dst = temp_var(b);
  instrs.push_back(tac::Binary{.op = tac::NotEqual{}, .src1 = val, .src2 = tac_const_val(source, 0), .dst = dst});
  return dst;
}

// Converts `val` to `bool` when it's already 0 or 1 (see
// `is_zero_or_one`): its low byte, with no comparison.
Val Translator::emit_zero_or_one_to_bool(const Val& val, const Type& source_in, Instrs& instrs) {
  const Type source = strip_quals(source_in);
  if (std::holds_alternative<tac::Constant>(val)) {
    return tac_const_val(c99::Bool{}, raw_const_value(val) & 1);
  }
  if (byte_width_of(source) == 1) {
    return val;
  }
  tac::Var dst = temp_var(Type{c99::Bool{}});
  instrs.push_back(tac::Truncate{.src = val, .dst = dst});
  return dst;
}

// Converts `src_val`, of type `src_type`, to `dst_type` with the same
// lowering as a `Cast`, as compound assignment does where `i /= 2u`
// converts `i` to unsigned before the divide and the quotient back to
// `int`.
Val Translator::emit_widen_or_narrow(const Val& src_val, const Type& src_type, const Type& dst_type,
                                     Instrs& instrs) {
  if (src_type == dst_type) {
    return src_val;
  }
  return emit_cast(src_val, src_type, dst_type, instrs);
}

// `&&` gives 0 at the first operand that is false, and `||` gives 1 at
// the first that is true, and otherwise the two are mirror images: the
// jump that cuts the chain short, and the constants of the short and the
// long way. The result is an `int` (C99 §6.5.13p3, §6.5.14p3) whatever
// the operands' types.
Val Translator::translate_short_circuit(const Exp& left, const Exp& right, Instrs& instrs,
                                        bool short_circuit_on_true) {
  const std::string branch_label = make_label(short_circuit_on_true ? "or_true" : "and_false");
  const std::string end_label = make_label(short_circuit_on_true ? "or_end" : "and_end");
  const int short_circuit_value = short_circuit_on_true ? 1 : 0;
  const int fallthrough_value = short_circuit_on_true ? 0 : 1;
  const Type int_t = c99::Int{};
  tac::Var dst = temp_var(int_t);
  const auto jump = [&](const Val& cond) {
    if (short_circuit_on_true) {
      instrs.push_back(tac::JumpIfTrue{.condition = cond, .target = branch_label});
    } else {
      instrs.push_back(tac::JumpIfFalse{.condition = cond, .target = branch_label});
    }
  };
  const Val src1 = translate_value(left, instrs);
  jump(src1);
  const Val src2 = translate_value(right, instrs);
  jump(src2);
  instrs.push_back(tac::Copy{.src = tac_const_val(int_t, fallthrough_value), .dst = dst});
  instrs.push_back(tac::Jump{.target = end_label});
  instrs.push_back(tac::Label{.name = branch_label});
  instrs.push_back(tac::Copy{.src = tac_const_val(int_t, short_circuit_value), .dst = dst});
  instrs.push_back(tac::Label{.name = end_label});
  return dst;
}

tac::Type_unary_operator translate_unop(const c99::Type_unary_operator& op) {
  using UK = c99::Kind_unary_operator;
  switch (c99::kind(op)) {
    case UK::Complement:
      return tac::Complement{};
    case UK::Negate:
      return tac::Negate{};
    case UK::LogicalNot:
      return tac::LogicalNot{};
    default:
      throw InternalError("unexpected unop");
  }
}

// `++` and `--` lower to a `Binary(Add | Subtract, operand, 1)`.
tac::Type_binary_operator translate_incdec_op(const c99::Type_incdec_op& op) {
  if (std::holds_alternative<c99::Increment>(op)) {
    return tac::Add{};
  }
  return tac::Subtract{};
}

tac::Type_binary_operator translate_binop(const c99::Type_binary_operator& op) {
  using BK = c99::Kind_binary_operator;
  switch (c99::kind(op)) {
    case BK::Add:
      return tac::Add{};
    case BK::Subtract:
      return tac::Subtract{};
    case BK::Multiply:
      return tac::Multiply{};
    case BK::Divide:
      return tac::Divide{};
    case BK::Modulo:
      return tac::Modulo{};
    case BK::BitwiseAnd:
      return tac::BitwiseAnd{};
    case BK::BitwiseOr:
      return tac::BitwiseOr{};
    case BK::BitwiseXor:
      return tac::BitwiseXor{};
    case BK::LeftShift:
      return tac::LeftShift{};
    case BK::RightShift:
      return tac::RightShift{};
    case BK::Equal:
      return tac::Equal{};
    case BK::NotEqual:
      return tac::NotEqual{};
    case BK::LessThan:
      return tac::LessThan{};
    case BK::GreaterThan:
      return tac::GreaterThan{};
    case BK::LessOrEqual:
      return tac::LessOrEqual{};
    case BK::GreaterOrEqual:
      return tac::GreaterOrEqual{};
    default:
      throw InternalError("unexpected binop");
  }
}

}  // namespace c6502::c99_to_tac
