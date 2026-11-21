// The walks over declarator trees of part R2 of the parser's descent
// (parser_declarations.h), parser.py's `_apply_declarator`,
// `_apply_abstract_declarator`, and their helpers (parser.py:1991-2297
// and 2669-2847).
//
// C99 declarators (§6.7.5) wrap a base type with a chain of modifiers,
// pointers, array bounds, and function signatures, and name an
// identifier in the middle. The walk applies them from the outer form
// inward, by the precedence of §6.7.5: a postfix array or function
// suffix binds tighter than a prefix `*`, so `int *foo(int)` is a
// function returning `int *`, and the pointer wraps the base type
// before the suffixes do.
#include <string>
#include <variant>

#include "parser_declarations.h"
#include "pyrepr.h"

namespace c6502::parser {

namespace ir = c99_ast;

namespace {

// Returns `base` inside the pointers of `levels`, the qualifier tokens of
// each `*` from the first one written. The first `*` applies to the base
// type first, and each level's `const` and `volatile` qualify the
// pointer it makes (C99 §6.7.5.1p1), so `int * const * p` is a pointer
// to a const pointer to int (parser.py's `_wrap_pointers`).
Type wrap_pointers(const std::vector<std::vector<lark::Token>>& levels, Type base) {
  for (const auto& quals : levels) {
    base = ir::Pointer{.referenced_type = std::move(base)};
    base = apply_qualifiers(base, qualifier_kinds(quals));
  }
  return base;
}

// Throws ParserError for a function declarator whose return type is a
// function or an array type, which C99 §6.7.5.3p1 forbids.
void check_function_return_type(const Type& ret) {
  if (std::holds_alternative<ir::FunType>(ret)) {
    throw ParserError("function declarator cannot specify a function-typed return (C99 §6.7.5.3.1)");
  }
  if (std::holds_alternative<ir::Array>(ret)) {
    throw ParserError("function declarator cannot specify an array return type (C99 §6.7.5.3.1)");
  }
}

// Returns the value of an array size written as an expression, such as
// `N * 2` after macro expansion or `sizeof(long) + 1`. C99 §6.7.5.2p1
// requires an integer constant expression for an array that isn't
// variable-length, and anything else, such as a variable, which would
// make one, is rejected (parser.py's `_fold_array_size`).
asdl::Int fold_array_size(Descent& p, const Exp& size) {
  try {
    return p.fold_integer_constant(size);
  } catch (const ParserError& e) {
    throw NotImplemented(std::string("array size must be an integer constant expression: ") +
                         e.what());
  }
}

// Returns the size of an `array_suffix`, 0 for the unsized form `[]`,
// the incomplete-array sentinel, which the type checker allows only on
// the outermost type of an `extern` declaration (parser.py's
// `_array_size_from_suffix`).
asdl::Int array_size(Descent& p, const ArraySuffix& suffix) {
  if (suffix.alias != "array_size_plain") {
    throw NotImplemented("only plain array sizes are supported (got " +
                         pyrepr::string(suffix.alias) + ")");
  }
  const std::size_t children = suffix.qualifiers.size() + (suffix.size ? 1 : 0);
  if (children == 0) {
    return 0;
  }
  // A qualifier with or without a size, as in `int a[const]`.
  if (children > 1 || !suffix.size) {
    throw NotImplemented("type-qualified array sizes aren't supported");
  }
  const Exp& size = *suffix.size;
  const auto* constant = std::get_if<ir::Constant>(&size);
  if (constant == nullptr) {
    const asdl::Int value = fold_array_size(p, size);
    if (value <= 0) {
      throw ParserError("array size must be positive");
    }
    return value;
  }
  const ir::Type_const& c = constant->const_;
  asdl::Int value = 0;
  switch (ir::kind(c)) {
    case ir::Kind_const::ConstNullptr:
      // An array size is an integer constant expression (C23 §6.7.7.3p1,
      // §6.6p8), and `nullptr` has no integer type.
      throw ParserError(
          "array size must have integer type, and `nullptr` has type nullptr_t (C23 §6.7.7.3)");
    case ir::Kind_const::ConstInt:
      value = std::get<ir::ConstInt>(c).value;
      break;
    case ir::Kind_const::ConstLong:
      value = std::get<ir::ConstLong>(c).value;
      break;
    case ir::Kind_const::ConstUInt:
      value = std::get<ir::ConstUInt>(c).value;
      break;
    case ir::Kind_const::ConstULong:
      value = std::get<ir::ConstULong>(c).value;
      break;
    case ir::Kind_const::ConstBool:
      value = std::get<ir::ConstBool>(c).value;
      break;
    case ir::Kind_const::ConstBitInt:
      value = std::get<ir::ConstBitInt>(c).value;
      break;
    default:
      throw NotImplemented("array size must be an integer constant");
  }
  if (value <= 0) {
    throw ParserError("array size must be positive");
  }
  return value;
}

// Walks a direct declarator, applying its suffixes from the last one
// written, which Lark's tree holds outermost, and then its core. The
// declared function's parameters are those of the function suffix
// closest to the name, since that suffix is applied last and becomes
// the top-level FunType: in `int (*pick(int w))(int a, int b)`, `pick`'s
// parameter is `w`, and `(int a, int b)` belongs to the pointee of the
// pointer it returns. So a deeper suffix's names replace an outer one's.
Applied apply_direct(Descent& p, const Direct& dd, Type base, bool abstract) {
  std::optional<std::vector<std::optional<std::string>>> params;
  std::optional<std::vector<std::string>> registers;
  for (auto it = dd.suffixes.rbegin(); it != dd.suffixes.rend(); ++it) {
    if (it->is_array) {
      const asdl::Int size = array_size(p, it->array);
      base = ir::Array{.element_type = std::move(base), .size = size};
      continue;
    }
    const FunctionSuffix& f = it->function;
    if (abstract) {
      check_function_return_type(base);
    }
    if (f.kind == FunctionSuffix::Kind::kIdentifierList) {
      throw NotImplemented("K&R-style identifier-list functions aren't supported");
    }
    if (!abstract) {
      check_function_return_type(base);
    }
    ir::FunType fun;
    std::vector<std::optional<std::string>> names;
    std::vector<std::string> markers;
    for (const Param& param : f.params) {
      fun.params.push_back(param.type);
      names.push_back(param.name);
      markers.push_back(param.marker);
    }
    fun.ret = std::move(base);
    base = std::move(fun);
    params = std::move(names);
    registers = std::move(markers);
  }
  switch (dd.core) {
    case Direct::Core::kName:
      return {dd.name, std::move(base), std::move(params), std::move(registers)};
    case Direct::Core::kParen: {
      Applied inner = abstract ? Applied{"", apply_abstract_declarator(p, *dd.inner, base), {}, {}}
                               : apply_declarator(p, *dd.inner, base);
      if (!inner.params) {
        inner.params = std::move(params);
        inner.registers = std::move(registers);
      }
      return inner;
    }
    case Direct::Core::kNone:
      break;
  }
  return {"", std::move(base), std::move(params), std::move(registers)};
}

const Direct& named_core(const Declarator& d) {
  const Declarator* cur = &d;
  while (true) {
    if (!cur->direct) {
      throw InternalError("parser: a declarator without a name");
    }
    const Direct& dd = *cur->direct;
    if (dd.core == Direct::Core::kName) {
      return dd;
    }
    if (dd.core != Direct::Core::kParen) {
      throw InternalError("parser: a declarator without a name");
    }
    cur = dd.inner.get();
  }
}

}  // namespace

std::string declarator_name(const Declarator& d) { return named_core(d).name; }

const std::optional<Attributes>& declarator_attributes(const Declarator& d) {
  return named_core(d).attrs;
}

Applied apply_declarator(Descent& p, const Declarator& d, Type base) {
  if (d.pointer) {
    base = wrap_pointers(*d.pointer, std::move(base));
  }
  if (!d.direct) {
    throw InternalError("parser: a declarator without a direct declarator");
  }
  return apply_direct(p, *d.direct, std::move(base), false);
}

Type apply_abstract_declarator(Descent& p, const Declarator& d, Type base) {
  if (d.pointer) {
    base = wrap_pointers(*d.pointer, std::move(base));
  }
  if (!d.direct) {
    return base;
  }
  return apply_direct(p, *d.direct, std::move(base), true).type;
}

Type adjust_param_type(const Type& t) {
  // C99 §6.7.5.3p7 adjusts a parameter of array type to a pointer to its
  // element type, only the outermost array, so `int a[3][4]` becomes
  // `int (*a)[4]`. The adjustment happens here, so the FunType, the
  // symbol table, and each use of the parameter all see the pointer.
  // §6.7.5.3p8 adjusts a parameter of function type, which only a
  // typedef can give, to a pointer to the function.
  if (std::holds_alternative<ir::FunType>(t)) {
    return ir::Pointer{.referenced_type = t};
  }
  if (const auto* arr = std::get_if<ir::Array>(&t)) {
    // The array declarator must still have a complete element type
    // (C99 §6.7.5.2p1), which the type checker can't see once the array
    // has become a pointer, so `void a[3]` is rejected here.
    if (std::holds_alternative<ir::Void>(*arr->element_type)) {
      throw ParserError(
          "array parameter declarator has incomplete element type 'void' (C99 §6.7.5.2.1 — "
          "array element must be a complete object type)");
    }
    return ir::Pointer{.referenced_type = *arr->element_type};
  }
  return t;
}

}  // namespace c6502::parser
