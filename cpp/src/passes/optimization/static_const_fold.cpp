/* The TAC-level fold of reads of scalar `const` statics.

The pass replaces each `Var(name)` that an instruction reads with
`Constant(value)` when `name` is a static-storage object that is

  * const-qualified at the top level (a `Const(...)` around its type in
    the symbol table), and not volatile,
  * a scalar, which is one of the integer types, `float`, `double`, or
    a pointer, and not an array, a structure, or a union,
  * initialized with a literal `Initial(int_or_float)` value. An
    `AddressInit` is resolved at link time and can't be a Constant, and
    a tuple is an aggregate.

The c99 type system already rejects writes to `const` lvalues, so in a
single translation unit the value is fixed at link time. Statics of
both internal and external linkage qualify, since other units could
read an external `const` object but not write it. The asm-level
`fold_const_statics` (passes/optimization_asm/const_static_fold.py)
drops the `StaticVariable` storage when nothing refers to it, and this
pass removes the runtime reads that would otherwise keep those
references alive.

The bigger gain is in what constant folding can do next. Once
`Var(hires_page1)` is `Constant(0x2000)` and
`IndexedLoad(interlace_p1_offsets, Constant(2))` is `Constant(0x01D0)`
(the fold of a constant array's element in constant_folding), the
existing fold makes `Binary(Add, Constant(0x2000), Constant(0x01D0))`
the one immediate `Constant(0x21D0)`, where there had been several
runtime steps of an address computation.

It runs once, before the TAC fixed point. Only operands that are read
are replaced. A definition of such a static shouldn't exist, since it
is `const`, and an `IndexedLoad`'s `name` names an array without
reading its value, so both are left alone. */
#include "passes/optimization/static_const_fold.h"

#include <map>
#include <string>

#include "c99_to_tac.h"
#include "errors.h"
#include "fp_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
namespace c99 = ::c6502::c99_ast;
using TK = c99::Kind_data_type;

// Returns the scalar type under `t` when `t` is const-qualified and not
// volatile-qualified, or null (Python's `_scalar_type`). The `const` is
// the gate, since a static without it may be changed at runtime even
// when the program doesn't. A volatile static is left alone even when it
// is `const`, since each access to a volatile object is a side effect
// (C99 §6.7.3.6), and one Constant for two reads would erase one.
const c99::Type_data_type* scalar_type(const c99::Type_data_type& type) {
  bool has_const = false;
  const c99::Type_data_type* inner = &type;
  for (;;) {
    if (asdl::isa<c99::Volatile>(*inner)) {
      return nullptr;
    }
    if (const auto* c = std::get_if<c99::Const>(inner)) {
      has_const = true;
      inner = &*c->referenced_type;
      continue;
    }
    break;
  }
  if (!has_const) {
    return nullptr;
  }
  switch (c99::kind(*inner)) {
    case TK::Char:
    case TK::SChar:
    case TK::UChar:
    case TK::Int:
    case TK::UInt:
    case TK::Long:
    case TK::ULong:
    case TK::LongLong:
    case TK::ULongLong:
    case TK::Float:
    case TK::Double:
    case TK::Pointer:
    case TK::BitInt:
      return inner;
    default:
      return nullptr;
  }
}

// Returns Python's `int(v)` of an initial value that is an int or a
// float, which truncates a float toward zero.
asdl::Int python_int(const ValueTree& v) {
  if (v.kind == ValueTree::Kind::kInt) {
    return v.integer;
  }
  const auto t = fp_arith::double_bits_to_int(asdl::Int(v.float_bits));
  const auto exact = t ? t->exact() : std::nullopt;
  if (!exact) {
    throw InternalError("cannot convert float NaN or infinity to integer");
  }
  return *exact;
}

// Returns the map from each foldable name to its Constant (Python's
// `_build_cache`), from one walk of the symbol table.
std::map<std::string, ir::Constant, std::less<>> build_cache(const SymbolTable& symbols) {
  std::map<std::string, ir::Constant, std::less<>> cache;
  for (const auto& [name, sym] : symbols) {
    const auto* attrs = std::get_if<StaticAttr>(&sym.attrs);
    if (attrs == nullptr) {
      continue;
    }
    const auto* initial = std::get_if<Initial>(&attrs->initial_value);
    if (initial == nullptr) {
      continue;
    }
    // An `AddressInit` is a link-time symbol and a tuple an aggregate,
    // and neither can be a TAC Constant.
    const ValueTree& v = initial->value;
    if (v.kind != ValueTree::Kind::kInt && v.kind != ValueTree::Kind::kFloat) {
      continue;
    }
    const c99::Type_data_type* t = scalar_type(sym.type);
    if (t == nullptr) {
      continue;
    }
    ir::Type_const c;
    if (asdl::isa<c99::Float>(*t)) {
      // A floating static's `Initial` holds its IEEE 754 bits, where
      // `tac_const_for` takes a number.
      c = ir::ConstFloat{python_int(v)};
    } else if (asdl::isa<c99::Double>(*t)) {
      c = ir::ConstDouble{python_int(v)};
    } else {
      c = c99_to_tac::tac_const_for(*t, python_int(v));
    }
    cache.insert_or_assign(name, ir::Constant{.const_ = std::move(c)});
  }
  return cache;
}

}  // namespace

tac_ast::Function fold_static_const_reads(tac_ast::Function fn,
                                          const framework::PassContext& ctx) {
  if (ctx.symbols == nullptr) {
    return fn;
  }
  const auto cache = build_cache(*ctx.symbols);
  return framework::run_operand_rewrite(
      std::move(fn), [&](const ir::Type_val& v) -> std::optional<ir::Type_val> {
        const auto* var = std::get_if<ir::Var>(&v);
        if (var == nullptr) {
          return std::nullopt;
        }
        const auto it = cache.find(var->name);
        if (it == cache.end()) {
          return std::nullopt;
        }
        return it->second;
      });
}

}  // namespace c6502::passes
