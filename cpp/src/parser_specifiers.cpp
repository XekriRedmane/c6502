// The specifiers, qualifiers, and attributes of part R2 of the parser's
// descent (parser_declarations.h): parser.py's `_resolve_data_type`,
// `_split_specifiers`, `_apply_qualifiers`, the checks of the C23
// attributes (parser.py:1091-1678), and the helpers of the shared
// interface that build types and constants from them.
#include <algorithm>
#include <cstdint>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "bit_precise.h"
#include "parser_declarations.h"
#include "passes/type_checking_types.h"
#include "pyrepr.h"

namespace c6502::parser {

namespace ir = c99_ast;
namespace tct = passes::type_checking;

namespace {

using AttrList = std::vector<std::pair<std::string, AttributeArg>>;

std::string repr(std::string_view s) { return pyrepr::string(s); }

// The c6502 attributes that go before a statement, with the statements
// each applies to, in messages (parser.py's `_STATEMENT_ATTRIBUTES`).
const std::vector<std::pair<std::string_view, std::string_view>>& statement_attributes() {
  static const std::vector<std::pair<std::string_view, std::string_view>> kAttrs = {
      {"unroll", "a `for` statement"},
      {"keep", "a `for` statement"},
      {"save_hardware_stack",
       "an expression statement, a `return` statement, or a declaration inside a function"},
  };
  return kAttrs;
}

const std::string_view* statement_attribute(std::string_view name) {
  for (const auto& [n, where] : statement_attributes()) {
    if (n == name) {
      return &where;
    }
  }
  return nullptr;
}

// What a standard attribute may apply to, and whether it takes a string
// literal (parser.py's `_STANDARD_ATTRIBUTES`).
struct StandardAttribute {
  std::string_view name;
  std::set<std::string_view> kinds;
};

const std::vector<StandardAttribute>& standard_attributes() {
  static const std::set<std::string_view> kDeclared = {
      "object", "function", "parameter", "typedef", "tag", "member", "enumerator"};
  static const std::vector<StandardAttribute> kAttrs = [] {
    std::set<std::string_view> unused = kDeclared;
    unused.insert("label");
    return std::vector<StandardAttribute>{
        {"deprecated", kDeclared},
        {"maybe_unused", unused},
        {"nodiscard", {"function", "tag"}},
        {"noreturn", {"function"}},
        {"_Noreturn", {"function"}},
        {"fallthrough", {"attribute declaration"}},
        {"unsequenced", {}},
        {"reproducible", {}},
    };
  }();
  return kAttrs;
}

std::string_view kind_name(std::string_view kind) {
  static const std::vector<std::pair<std::string_view, std::string_view>> kNames = {
      {"object", "an object"},
      {"function", "a function"},
      {"parameter", "a parameter"},
      {"typedef", "a typedef name"},
      {"tag", "a struct, union, or enum"},
      {"member", "a member"},
      {"enumerator", "an enumeration constant"},
      {"attribute declaration", "an attribute declaration"},
      {"statement", "a statement"},
      {"label", "a label"},
  };
  for (const auto& [k, n] : kNames) {
    if (k == kind) {
      return n;
    }
  }
  throw InternalError("parser: no kind " + std::string(kind));
}

int count_type(const std::vector<Spec>& specs, int type) {
  return static_cast<int>(
      std::count_if(specs.begin(), specs.end(), [type](const Spec& s) { return s.type == type; }));
}

// Returns parser.py's `_bit_precise_type` of a specifier list with
// `_BitInt(N)` in it, which may have one `signed` or one `unsigned`
// besides (C23 §6.7.3.1p2). N must be at least 2 for `_BitInt` and at
// least 1 for `unsigned _BitInt` (§6.7.3.1p4).
Type bit_precise_type(const std::vector<Spec>& type_specs) {
  const Terminals& t = terminals();
  std::vector<const Spec*> bitints;
  std::vector<const Spec*> others;
  for (const Spec& s : type_specs) {
    (s.kind == Spec::Kind::kBitInt ? bitints : others).push_back(&s);
  }
  const bool bad_other = std::any_of(others.begin(), others.end(), [&t](const Spec* s) {
    return s->type != t.SIGNED && s->type != t.UNSIGNED;
  });
  if (bitints.size() > 1 || others.size() > 1 || bad_other) {
    throw ParserError(
        "'_BitInt(N)' combines only with one 'signed' or one 'unsigned' (C23 §6.7.3.1p2)");
  }
  const bool is_signed = !(!others.empty() && others[0]->type == t.UNSIGNED);
  const asdl::Int width = bitints[0]->width;
  const int least = is_signed ? 2 : 1;
  if (width < least) {
    throw ParserError(std::string("the width of ") +
                      (is_signed ? "_BitInt" : "unsigned _BitInt") + " must be at least " +
                      std::to_string(least) + ", and it's " + pyrepr::integer(width) +
                      " (C23 §6.7.3.1p4)");
  }
  return ir::BitInt{.width = width, .is_signed = is_signed};
}

bool has_outer_const(const Type& t) {
  const Type* p = &t;
  while (const auto* v = std::get_if<ir::Volatile>(p)) {
    p = &*v->referenced_type;
  }
  return std::holds_alternative<ir::Const>(*p);
}

bool has_outer_volatile(const Type& t) {
  const Type* p = &t;
  while (const auto* c = std::get_if<ir::Const>(p)) {
    p = &*c->referenced_type;
  }
  return std::holds_alternative<ir::Volatile>(*p);
}

void type_tags_into(const Type& t, std::vector<std::pair<std::string, bool>>& out) {
  switch (ir::kind(t)) {
    case ir::Kind_data_type::Structure:
      out.emplace_back(std::get<ir::Structure>(t).tag, false);
      break;
    case ir::Kind_data_type::Union:
      out.emplace_back(std::get<ir::Union>(t).tag, true);
      break;
    case ir::Kind_data_type::FunType: {
      const auto& f = std::get<ir::FunType>(t);
      for (const Type& p : f.params) {
        type_tags_into(p, out);
      }
      type_tags_into(*f.ret, out);
      break;
    }
    case ir::Kind_data_type::Pointer:
      type_tags_into(*std::get<ir::Pointer>(t).referenced_type, out);
      break;
    case ir::Kind_data_type::Array:
      type_tags_into(*std::get<ir::Array>(t).element_type, out);
      break;
    case ir::Kind_data_type::Const:
      type_tags_into(*std::get<ir::Const>(t).referenced_type, out);
      break;
    case ir::Kind_data_type::Volatile:
      type_tags_into(*std::get<ir::Volatile>(t).referenced_type, out);
      break;
    default:
      break;
  }
}

// The value of the integer constant `spelling`, with any number of
// digits, as limbs of 32 bits from the lowest. The lexer has checked
// the spelling, so it is a prefix, digits with separators, and a
// suffix of `u`s and `l`s.
std::vector<std::uint32_t> wide_value(std::string_view spelling) {
  unsigned base = 10;
  std::size_t i = 0;
  if (spelling.size() > 1 && spelling[0] == '0' &&
      (spelling[1] == 'x' || spelling[1] == 'X')) {
    base = 16;
    i = 2;
  } else if (spelling.size() > 1 && spelling[0] == '0' &&
             (spelling[1] == 'b' || spelling[1] == 'B')) {
    base = 2;
    i = 2;
  } else if (!spelling.empty() && spelling[0] == '0') {
    base = 8;
  }
  std::vector<std::uint32_t> limbs;
  for (; i < spelling.size(); ++i) {
    const char c = spelling[i];
    unsigned digit = 0;
    if (c == '\'') {
      continue;
    }
    if (c >= '0' && c <= '9') {
      digit = static_cast<unsigned>(c - '0');
    } else if (base == 16 && c >= 'a' && c <= 'f') {
      digit = static_cast<unsigned>(c - 'a' + 10);
    } else if (base == 16 && c >= 'A' && c <= 'F') {
      digit = static_cast<unsigned>(c - 'A' + 10);
    } else {
      break;  // the suffix
    }
    std::uint64_t carry = digit;
    for (std::uint32_t& limb : limbs) {
      const std::uint64_t v = static_cast<std::uint64_t>(limb) * base + carry;
      limb = static_cast<std::uint32_t>(v);
      carry = v >> 32;
    }
    if (carry != 0) {
      limbs.push_back(static_cast<std::uint32_t>(carry));
    }
  }
  return limbs;
}

// Returns the digits of `limbs` in `base`, 10 or 16, with uppercase
// hexadecimal digits, as Python's `str` and `format(x, "X")` write them.
std::string wide_digits(std::vector<std::uint32_t> limbs, unsigned base) {
  std::string out;
  while (!limbs.empty()) {
    std::uint64_t rem = 0;
    for (auto it = limbs.rbegin(); it != limbs.rend(); ++it) {
      const std::uint64_t v = (rem << 32) | *it;
      *it = static_cast<std::uint32_t>(v / base);
      rem = v % base;
    }
    out.push_back("0123456789ABCDEF"[rem]);
    while (!limbs.empty() && limbs.back() == 0) {
      limbs.pop_back();
    }
  }
  if (out.empty()) {
    out = "0";
  }
  return {out.rbegin(), out.rend()};
}

std::string arg_repr(const AttributeArg& arg) {
  switch (arg.kind) {
    case AttributeArg::Kind::kNone:
      return "None";
    case AttributeArg::Kind::kInt:
      if (!arg.integer.fits()) {
        return wide_digits(wide_value(arg.spelling), 10);
      }
      return pyrepr::integer(arg.integer.value);
    case AttributeArg::Kind::kString:
      return repr(arg.string);
  }
  return "None";
}

// Returns whether the integer argument `arg` lies in 0..`high`.
bool int_in_range(const AttributeArg& arg, asdl::Int high) {
  return arg.integer.fits() && arg.integer.value >= 0 && arg.integer.value <= high;
}

std::string arg_hex(const AttributeArg& arg) {
  if (!arg.integer.fits()) {
    // Python prints every digit of a value wider than 127 bits.
    return wide_digits(wide_value(arg.spelling), 16);
  }
  return pyrepr::hex(arg.integer.value, 0);
}

// Returns parser.py's `_fixed_address`: the address `address(...)` gives
// on `where`, checked to be an integer in the 6502's 16-bit space.
asdl::Int fixed_address(const AttributeArg& arg, std::string_view where) {
  if (arg.kind != AttributeArg::Kind::kInt) {
    throw ParserError("`[[c6502::address(...)]]` takes an integer address; saw " +
                      std::string(arg.is_none() ? "none" : "a string") + " on " +
                      std::string(where));
  }
  if (!int_in_range(arg, 0xFFFF)) {
    throw ParserError("address $" + arg_hex(arg) + " on " + std::string(where) +
                      " is outside $0000-$FFFF");
  }
  return arg.integer.value;
}

}  // namespace

bool DeclSpecs::is_typedef() const {
  const int typedef_kw = terminals().TYPEDEF;
  return std::any_of(specs.begin(), specs.end(),
                     [typedef_kw](const Spec& s) { return s.type == typedef_kw; });
}

bool is_type_specifier_type(int k) {
  const Terminals& t = terminals();
  return k == t.INT || k == t.LONG || k == t.SHORT || k == t.SIGNED || k == t.UNSIGNED ||
         k == t.FLOAT || k == t.DOUBLE || k == t.CHAR || k == t.BOOL || k == t.VOID ||
         k == t.STRUCT || k == t.UNION || k == t.TYPEDEF_NAME || k == t.ENUM || k == t.BITINT;
}

bool is_type_qualifier_type(int k) {
  const Terminals& t = terminals();
  return k == t.CONST || k == t.VOLATILE || k == t.RESTRICT;
}

std::vector<int> qualifier_kinds(const std::vector<Spec>& specs) {
  std::vector<int> out;
  for (const Spec& s : specs) {
    if (s.kind == Spec::Kind::kToken && is_type_qualifier_type(s.type)) {
      out.push_back(s.type);
    }
  }
  return out;
}

std::vector<int> qualifier_kinds(const std::vector<lark::Token>& tokens) {
  std::vector<int> out;
  for (const lark::Token& tok : tokens) {
    if (is_type_qualifier_type(tok.type)) {
      out.push_back(tok.type);
    }
  }
  return out;
}

bool has_auto(const std::vector<Spec>& specs) { return count_type(specs, terminals().AUTO) > 0; }

bool has_constexpr(const std::vector<Spec>& specs) {
  return count_type(specs, terminals().CONSTEXPR) > 0;
}

std::string storage_name(Storage storage) {
  switch (storage) {
    case Storage::kStatic:
      return "static";
    case Storage::kExtern:
      return "extern";
    case Storage::kRegister:
      return "register";
    case Storage::kTypedef:
      return "typedef";
    case Storage::kNone:
      break;
  }
  // Python's `type(None).__name__.lower()`.
  return "nonetype";
}

std::optional<ir::Type_storage_class> storage_class(Storage storage) {
  switch (storage) {
    case Storage::kStatic:
      return ir::Static{};
    case Storage::kExtern:
      return ir::Extern{};
    case Storage::kRegister:
      return ir::Register{};
    case Storage::kNone:
      return std::nullopt;
    case Storage::kTypedef:
      break;
  }
  throw InternalError("parser: `typedef` isn't a c99 storage class");
}

Type resolve_data_type(const std::vector<Spec>& type_specs) {
  const Terminals& t = terminals();
  // A typedef name or an enum specifier is a whole type on its own (C99
  // §6.7.2.2).
  for (const Spec& spec : type_specs) {
    if (!spec.is_typedef_like()) {
      continue;
    }
    if (type_specs.size() != 1) {
      const std::string what = spec.kind == Spec::Kind::kTypedef
                                   ? "typedef name " + repr(spec.name)
                                   : spec.name;
      throw ParserError(what + " cannot combine with another type specifier");
    }
    return *spec.data_type;
  }
  // Struct/union specifiers don't combine with any other type specifier
  // (C99 §6.7.2 is exhaustive, and a struct-or-union-specifier is one
  // whole combination).
  const Spec* tag = nullptr;
  int tags = 0;
  bool bitint = false;
  for (const Spec& spec : type_specs) {
    if (spec.kind == Spec::Kind::kTag) {
      tag = tag == nullptr ? &spec : tag;
      ++tags;
    }
    bitint = bitint || spec.kind == Spec::Kind::kBitInt;
  }
  if (tag != nullptr) {
    if (tags > 1) {
      throw ParserError("at most one struct or union specifier in a declaration");
    }
    if (type_specs.size() != 1) {
      throw ParserError("struct/union specifier cannot combine with another type specifier");
    }
    if (tag->is_union) {
      return ir::Union{.tag = tag->tag};
    }
    return ir::Structure{.tag = tag->tag};
  }
  if (bitint) {
    return bit_precise_type(type_specs);
  }
  const int int_count = count_type(type_specs, t.INT);
  const int long_count = count_type(type_specs, t.LONG);
  const int signed_count = count_type(type_specs, t.SIGNED);
  const int unsigned_count = count_type(type_specs, t.UNSIGNED);
  const int float_count = count_type(type_specs, t.FLOAT);
  const int double_count = count_type(type_specs, t.DOUBLE);
  const int char_count = count_type(type_specs, t.CHAR);
  const int void_count = count_type(type_specs, t.VOID);
  const int bool_count = count_type(type_specs, t.BOOL);
  const int short_count = count_type(type_specs, t.SHORT);
  if (bool_count > 0) {
    if (type_specs.size() != 1) {
      // Named as the program spelled it, `bool` or `_Bool`.
      std::string spelling;
      for (const Spec& s : type_specs) {
        if (s.type == t.BOOL) {
          spelling = s.text;
          break;
        }
      }
      // `int bool;` declared an `int` named `bool` before C23.
      const std::string note =
          spelling == "bool"
              ? "; `bool` is a keyword in C23 (§6.4.1), so it can't be a name either"
              : "";
      throw ParserError("'" + spelling + "' cannot combine with another type specifier" + note);
    }
    return ir::Bool{};
  }
  const bool is_fp = float_count > 0 || double_count > 0;
  const bool is_integer = int_count > 0 || signed_count > 0 || unsigned_count > 0 ||
                          char_count > 0 || short_count > 0;
  if (void_count > 0) {
    if (void_count > 1) {
      throw ParserError("'void' specified more than once");
    }
    if (is_fp || is_integer || long_count > 0) {
      throw ParserError("'void' cannot combine with any other type specifier");
    }
    return ir::Void{};
  }
  if (is_fp && is_integer) {
    throw ParserError("floating type cannot combine with 'int' / 'signed' / 'unsigned' / 'char'");
  }
  if (float_count > 1) {
    throw ParserError("'float' specified more than once");
  }
  if (double_count > 1) {
    throw ParserError("'double' specified more than once");
  }
  if (float_count == 1 && double_count == 1) {
    throw ParserError("'float' and 'double' cannot both appear in a declaration");
  }
  if (double_count == 1 && long_count >= 1) {
    throw ParserError(
        "'long double' is not supported (only 'float' and 'double' are modeled today)");
  }
  if (float_count == 1 && long_count >= 1) {
    throw ParserError("'long float' is not a valid type");
  }
  if (signed_count > 0 && unsigned_count > 0) {
    throw ParserError("'signed' and 'unsigned' cannot both appear in a declaration");
  }
  if (signed_count > 1) {
    throw ParserError("'signed' specified more than once");
  }
  if (unsigned_count > 1) {
    throw ParserError("'unsigned' specified more than once");
  }
  if (char_count > 1) {
    throw ParserError("'char' specified more than once");
  }
  if (char_count == 1 && (int_count > 0 || long_count > 0 || short_count > 0)) {
    throw ParserError("'char' cannot combine with 'int' / 'long' / 'short'");
  }
  if (short_count > 1) {
    throw ParserError("'short' specified more than once");
  }
  if (short_count == 1 && long_count > 0) {
    throw ParserError("'short' cannot combine with 'long'");
  }
  if (!is_fp && int_count == 0 && long_count == 0 && signed_count == 0 && unsigned_count == 0 &&
      char_count == 0 && short_count == 0) {
    throw ParserError("missing type specifier");
  }
  if (int_count > 1) {
    throw ParserError(
        "multiple type specifiers in a declaration (at most one 'int' is permitted)");
  }
  if (long_count > 2) {
    throw ParserError(
        "'long' specified more than twice (no integer type wider than 'long long')");
  }
  if (float_count == 1) {
    return ir::Float{};
  }
  if (double_count == 1) {
    return ir::Double{};
  }
  const bool is_unsigned = unsigned_count == 1;
  const bool is_signed = signed_count == 1;
  if (char_count == 1) {
    if (is_unsigned) {
      return ir::UChar{};
    }
    if (is_signed) {
      return ir::SChar{};
    }
    return ir::Char{};
  }
  if (short_count == 1) {
    return is_unsigned ? Type{ir::UShort{}} : Type{ir::Short{}};
  }
  if (long_count == 2) {
    return is_unsigned ? Type{ir::ULongLong{}} : Type{ir::LongLong{}};
  }
  if (long_count == 1) {
    return is_unsigned ? Type{ir::ULong{}} : Type{ir::Long{}};
  }
  if (is_unsigned) {
    return ir::UInt{};
  }
  return ir::Int{};
}

Type apply_qualifiers(const Type& data_type, const std::vector<int>& qualifiers) {
  const Terminals& t = terminals();
  if (const auto* arr = std::get_if<ir::Array>(&data_type)) {
    return ir::Array{.element_type = apply_qualifiers(*arr->element_type, qualifiers),
                     .size = arr->size};
  }
  const bool has_const = std::find(qualifiers.begin(), qualifiers.end(), t.CONST) != qualifiers.end();
  const bool has_volatile =
      std::find(qualifiers.begin(), qualifiers.end(), t.VOLATILE) != qualifiers.end();
  Type out = data_type;
  if (has_volatile && !has_outer_volatile(out)) {
    out = ir::Volatile{.referenced_type = std::move(out)};
  }
  if (has_const && !has_outer_const(out)) {
    out = ir::Const{.referenced_type = std::move(out)};
  }
  return out;
}

SplitSpecs split_specifiers(const std::vector<Spec>& specs) {
  const Terminals& t = terminals();
  std::vector<Spec> type_specs;
  SplitSpecs out;
  bool constexpr_ = false;
  bool auto_ = false;
  for (const Spec& spec : specs) {
    if (is_type_specifier_type(spec.type)) {
      type_specs.push_back(spec);
    } else if (is_type_qualifier_type(spec.type)) {
      out.qualifiers.push_back(spec);
    } else if (spec.type == t.CONSTEXPR) {
      if (constexpr_) {
        throw ParserError("`constexpr` appears twice in a declaration");
      }
      constexpr_ = true;
    } else if (spec.type == t.THREAD_LOCAL || spec.type == t.ALT_THREAD_LOCAL) {
      throw ParserError("`" + spec.text + "` gives each thread its own object, and c6502 has no " +
                        "threads, so it doesn't take `" + spec.text + "`");
    } else if (spec.type == t.AUTO) {
      if (auto_) {
        throw ParserError("`auto` appears twice in a declaration");
      }
      auto_ = true;
    } else {
      Storage s = Storage::kNone;
      if (spec.type == t.STATIC) {
        s = Storage::kStatic;
      } else if (spec.type == t.EXTERN) {
        s = Storage::kExtern;
      } else if (spec.type == t.REGISTER) {
        s = Storage::kRegister;
      } else if (spec.type == t.TYPEDEF) {
        s = Storage::kTypedef;
      } else {
        throw InternalError("parser: an unknown specifier " + spec.text);
      }
      if (out.storage != Storage::kNone) {
        throw ParserError("at most one storage-class specifier permitted in a declaration");
      }
      out.storage = s;
    }
  }
  if (constexpr_ && out.storage != Storage::kNone && out.storage != Storage::kStatic &&
      out.storage != Storage::kRegister) {
    throw ParserError("`constexpr` can't go with `" + storage_name(out.storage) +
                      "`; of the other storage-class specifiers, only `static` and `register` "
                      "may come with it (C23 §6.7.2p2)");
  }
  if (auto_) {
    if (out.storage == Storage::kTypedef) {
      throw ParserError("`auto` can't go with `typedef` (C23 §6.7.2p2)");
    }
    if (type_specs.empty()) {
      // The type comes from the initializer (C23 §6.7.10).
      out.inferred = true;
      return out;
    }
    if (out.storage != Storage::kNone || constexpr_) {
      const std::string other =
          out.storage == Storage::kNone ? "constexpr" : storage_name(out.storage);
      throw ParserError("`auto` goes with `" + other +
                        "` only when it infers the type from the initializer, and this "
                        "declaration has a type specifier (C23 §6.7.2p4)");
    }
  }
  out.type = apply_qualifiers(resolve_data_type(type_specs), qualifier_kinds(out.qualifiers));
  out.qualifiers.clear();
  return out;
}

std::vector<std::pair<std::string, bool>> type_tags(const Type& t) {
  std::vector<std::pair<std::string, bool>> out;
  type_tags_into(t, out);
  return out;
}

Type unqualified_type(const Type& t) {
  const Type& u = unqualified(t);
  if (const auto* arr = std::get_if<ir::Array>(&u)) {
    return ir::Array{.element_type = unqualified_type(*arr->element_type), .size = arr->size};
  }
  return u;
}

// --- Attributes ---

std::optional<Attributes> merge_attributes(const std::optional<Attributes>& a,
                                           const std::optional<Attributes>& b) {
  if (!a && !b) {
    return std::nullopt;
  }
  Attributes out;
  for (const auto* x : {&a, &b}) {
    if (*x) {
      out.c6502.insert(out.c6502.end(), (*x)->c6502.begin(), (*x)->c6502.end());
      out.standard.insert(out.standard.end(), (*x)->standard.begin(), (*x)->standard.end());
    }
  }
  return out;
}

std::optional<AttrList> c6502_attributes(const std::optional<Attributes>& attrs,
                                         std::string_view kind, std::string_view where) {
  if (!attrs) {
    return std::nullopt;
  }
  for (const auto& [name, arg] : attrs->standard) {
    const auto& table = standard_attributes();
    const auto it = std::find_if(table.begin(), table.end(),
                                 [&](const StandardAttribute& s) { return s.name == name; });
    if (it == table.end()) {
      throw InternalError("parser: no standard attribute " + name);
    }
    if (!it->kinds.contains(kind)) {
      if (name == "unsequenced" || name == "reproducible") {
        throw ParserError("`[[" + name +
                          "]]` applies to a function's type, after its parameter list, where "
                          "c6502 doesn't accept attributes; saw it on " +
                          std::string(where));
      }
      throw ParserError("the standard attribute `" + name + "` can't apply to " +
                        std::string(kind_name(kind)) + "; saw it on " + std::string(where));
    }
  }
  if (kind != "statement") {
    for (const auto& [name, arg] : attrs->c6502) {
      if (const std::string_view* goes = statement_attribute(name)) {
        throw ParserError("`c6502::" + name + "` goes before " + std::string(*goes) +
                          "; it doesn't apply to " + std::string(where));
      }
    }
  }
  if (attrs->c6502.empty()) {
    return std::nullopt;
  }
  return attrs->c6502;
}

void no_c6502_attributes(const std::optional<Attributes>& attrs, std::string_view kind,
                         std::string_view where) {
  const std::optional<AttrList> specs = c6502_attributes(attrs, kind, where);
  if (specs) {
    throw ParserError("`c6502::" + (*specs)[0].first + "` doesn't apply to " +
                      std::string(where));
  }
}

void no_enum_attributes(const std::optional<Attributes>& attrs) {
  const std::optional<AttrList> specs = c6502_attributes(attrs, "tag", "an enum");
  if (specs) {
    throw ParserError("`c6502::" + (*specs)[0].first + "` doesn't apply to an enum");
  }
}

bool is_noreturn(const std::optional<Attributes>& attrs) {
  if (!attrs) {
    return false;
  }
  return std::any_of(attrs->standard.begin(), attrs->standard.end(), [](const auto& a) {
    return a.first == "noreturn" || a.first == "_Noreturn";
  });
}

bool take_hw_stack_save(std::optional<Attributes>& attrs) {
  if (!attrs) {
    return false;
  }
  std::vector<const AttributeArg*> saves;
  for (const auto& [name, arg] : attrs->c6502) {
    if (name == "save_hardware_stack") {
      saves.push_back(&arg);
    }
  }
  if (saves.empty()) {
    return false;
  }
  if (saves.size() > 1) {
    throw ParserError("`c6502::save_hardware_stack` is given twice on one declaration");
  }
  if (!saves[0]->is_none()) {
    throw ParserError("`c6502::save_hardware_stack` doesn't take an argument");
  }
  Attributes rest;
  for (const auto& a : attrs->c6502) {
    if (a.first != "save_hardware_stack") {
      rest.c6502.push_back(a);
    }
  }
  rest.standard = attrs->standard;
  if (rest.c6502.empty() && rest.standard.empty()) {
    attrs.reset();
  } else {
    attrs = std::move(rest);
  }
  return true;
}

std::string register_marker(const std::optional<AttrList>& specs, bool is_register) {
  if (!specs || specs->empty()) {
    return is_register ? "auto" : "";
  }
  if (specs->size() != 1) {
    throw ParserError(
        "a parameter takes one attribute: `c6502::in_a`, `c6502::in_x`, `c6502::in_y`, or a "
        "pair such as `c6502::in_ay`, naming the register it arrives in, or "
        "`c6502::zeropage(addr)`, naming its zero-page address");
  }
  const auto& [name, arg] = (*specs)[0];
  if (name == "zeropage") {
    if (is_register) {
      throw ParserError(
          "a `register` parameter arrives in a register, so it can't also have "
          "`[[c6502::zeropage(...)]]`");
    }
    if (arg.kind != AttributeArg::Kind::kInt) {
      throw ParserError(
          "`[[c6502::zeropage(...)]]` on a parameter takes the integer zero-page address it "
          "arrives at");
    }
    if (!int_in_range(arg, 0xFF)) {
      throw ParserError("zero-page address $" + arg_hex(arg) +
                        " for a parameter is outside $00-$FF");
    }
    return "@" + pyrepr::integer(arg.integer.value);
  }
  std::string regs;
  if (name.starts_with("in_")) {
    regs = name.substr(3);
    for (char& c : regs) {
      if (c >= 'a' && c <= 'z') {
        c = static_cast<char>(c - 'a' + 'A');
      }
    }
  }
  const bool in_axy = std::all_of(regs.begin(), regs.end(),
                                  [](char c) { return c == 'A' || c == 'X' || c == 'Y'; });
  if (regs.empty() || regs.size() > 2 || !in_axy) {
    throw ParserError("`c6502::" + name +
                      "` doesn't apply to a parameter; a parameter takes `c6502::in_a`, "
                      "`c6502::in_x`, or `c6502::in_y`, or for a 2-byte parameter a pair such as "
                      "`c6502::in_ay` (low byte in A, high byte in Y), or "
                      "`c6502::zeropage(addr)`");
  }
  if (!arg.is_none()) {
    throw ParserError("`c6502::" + name + "` doesn't take an argument");
  }
  if (regs.size() == 2 && regs[0] == regs[1]) {
    throw ParserError("`c6502::" + name + "` names " + regs.substr(0, 1) +
                      " twice; each byte of a parameter needs its own register");
  }
  return regs;
}

FunctionAttributes extract_function_attributes(const std::optional<AttrList>& specs,
                                               std::string_view where) {
  FunctionAttributes out;
  std::set<std::string> seen;
  if (specs) {
    for (const auto& [name, arg] : *specs) {
      if (seen.contains(name)) {
        throw ParserError("`c6502::" + name + "` is given twice on " + std::string(where));
      }
      seen.insert(name);
      if (name == "section") {
        continue;
      }
      if (name == "zeropage") {
        throw ParserError(
            "`[[c6502::zeropage]]` places an object, not a function; saw it on " +
            std::string(where));
      }
      if (name == "zp_abi" || name == "return_bool_in_carry" || name == "preserves_x" ||
          name == "preserves_y") {
        if (!arg.is_none()) {
          throw ParserError("`c6502::" + name + "` doesn't take an argument");
        }
      }
      if (name == "zp_abi") {
        out.abi_annotation = "zp_abi";
      } else if (name == "return_bool_in_carry") {
        out.carry_return = true;
      } else if (name == "address") {
        out.address = fixed_address(arg, where);
      } else if (name == "preserves_x") {
        out.preserves.push_back("X");
      } else if (name == "preserves_y") {
        out.preserves.push_back("Y");
      } else {
        throw ParserError("`c6502::" + name + "` doesn't apply to " + std::string(where));
      }
    }
  }
  std::sort(out.preserves.begin(), out.preserves.end());
  return out;
}

std::optional<ir::Type_placement> extract_object_placement(const std::optional<AttrList>& specs,
                                                            std::string_view name) {
  std::optional<ir::Type_placement> placement;
  std::set<std::string> seen;
  if (!specs) {
    return placement;
  }
  const std::string n(name);
  for (const auto& [spec_name, arg] : *specs) {
    if (seen.contains(spec_name)) {
      throw ParserError("`c6502::" + spec_name + "` is given twice on object declaration `" + n +
                        "`");
    }
    seen.insert(spec_name);
    if (spec_name == "section") {
      continue;
    }
    if (spec_name == "zp_abi" || spec_name == "return_bool_in_carry" ||
        spec_name == "preserves_x" || spec_name == "preserves_y") {
      throw ParserError("`[[c6502::" + spec_name +
                        "]]` is only valid on function declarations; saw it on object "
                        "declaration `" +
                        n + "`");
    }
    if (spec_name != "zeropage" && spec_name != "address") {
      throw ParserError("`c6502::" + spec_name + "` doesn't apply to object declaration `" + n +
                        "`");
    }
    if (placement) {
      throw ParserError(
          "`[[c6502::zeropage]]` and `[[c6502::address(...)]]` both given on `" + n + "`");
    }
    if (spec_name == "address") {
      placement = ir::Absolute{.address = fixed_address(arg, "object `" + n + "`")};
      continue;
    }
    if (arg.kind == AttributeArg::Kind::kString) {
      throw ParserError(
          "`[[c6502::zeropage(...)]]` takes an integer address; got a string on `" + n + "`");
    }
    if (arg.kind == AttributeArg::Kind::kInt && !int_in_range(arg, 0xFF)) {
      throw ParserError("zero-page address $" + arg_hex(arg) + " for `" + n +
                        "` is outside $00-$FF");
    }
    ir::ZeroPage zp;
    if (arg.kind == AttributeArg::Kind::kInt) {
      zp.address = arg.integer.value;
    }
    placement = zp;
  }
  return placement;
}

std::optional<std::string> extract_section(const std::optional<AttrList>& specs,
                                           std::string_view where) {
  if (!specs) {
    return std::nullopt;
  }
  for (const auto& [name, arg] : *specs) {
    if (name != "section") {
      continue;
    }
    if (arg.kind != AttributeArg::Kind::kString) {
      throw ParserError(
          "`[[c6502::section(...)]]` takes the section's name as a string, such as "
          "`section(\".text.boot\")`; saw " +
          arg_repr(arg) + " on " + std::string(where));
    }
    // A section name: letters, digits, `_`, and `.`, as GNU ld script
    // patterns can match, such as `.text.boot` or `vectors`.
    const std::string& s = arg.string;
    const auto word = [](char c, bool first) {
      return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '.' ||
             (!first && c >= '0' && c <= '9');
    };
    bool ok = !s.empty();
    for (std::size_t i = 0; ok && i < s.size(); ++i) {
      ok = word(s[i], i == 0);
    }
    if (!ok) {
      throw ParserError("section name " + repr(s) + " on " + std::string(where) +
                        " has to be letters, digits, `_`, and `.`, not starting with a digit");
    }
    return s;
  }
  return std::nullopt;
}

// --- The helpers of the shared interface ---

std::pair<std::optional<ir::Type_storage_class>, bool> Descent::literal_storage(
    const std::vector<lark::Token>& tokens) {
  std::set<int> seen;
  std::optional<ir::Type_storage_class> storage;
  bool is_constexpr = false;
  for (const lark::Token& tok : tokens) {
    if (tok.type == t.THREAD_LOCAL || tok.type == t.ALT_THREAD_LOCAL) {
      throw ParserError("`" + tok.value + "` gives each thread its own object, and c6502 has no " +
                        "threads, so it doesn't take `" + tok.value + "`");
    }
    if (seen.contains(tok.type)) {
      throw ParserError("`" + tok.value + "` appears twice in a compound literal");
    }
    seen.insert(tok.type);
    if (tok.type == t.CONSTEXPR) {
      is_constexpr = true;
    } else if (tok.type == t.STATIC || tok.type == t.REGISTER) {
      if (storage) {
        throw ParserError("a compound literal can't be both `static` and `register`");
      }
      if (tok.type == t.STATIC) {
        storage = ir::Static{};
      } else {
        storage = ir::Register{};
      }
    } else {
      throw ParserError("`" + tok.value +
                        "` can't go on a compound literal, which takes only `constexpr`, "
                        "`static`, `register`, and `thread_local` (C23 §6.5.3.6p7)");
    }
  }
  return {storage, is_constexpr};
}

Type Descent::constexpr_type(const Type& data_type) {
  return apply_qualifiers(data_type, {t.CONST});
}

Exp Descent::enum_constant(const asdl::Int& value, const std::optional<Type>& enum_type) {
  if (!enum_type) {
    return ir::Constant{.const_ = ir::ConstInt{.value = value & asdl::Int(0xFFFF)}};
  }
  const Type& ty = *enum_type;
  const auto masked = [&]() -> asdl::Int {
    const int bits = tct::int_bits(ty);
    return value & ((asdl::Int(1) << bits) - 1);
  };
  switch (ir::kind(ty)) {
    case ir::Kind_data_type::Int:
      return ir::Constant{.const_ = ir::ConstInt{.value = masked()}};
    case ir::Kind_data_type::UInt:
      return ir::Constant{.const_ = ir::ConstUInt{.value = masked()}};
    case ir::Kind_data_type::Long:
      return ir::Constant{.const_ = ir::ConstLong{.value = masked()}};
    case ir::Kind_data_type::ULong:
      return ir::Constant{.const_ = ir::ConstULong{.value = masked()}};
    case ir::Kind_data_type::LongLong:
      return ir::Constant{.const_ = ir::ConstLongLong{.value = masked()}};
    case ir::Kind_data_type::ULongLong:
      return ir::Constant{.const_ = ir::ConstULongLong{.value = masked()}};
    case ir::Kind_data_type::UChar:
      return ir::Constant{.const_ = ir::ConstUChar{.value = masked()}};
    case ir::Kind_data_type::Bool:
      return ir::Constant{.const_ = ir::ConstBool{.value = masked()}};
    default:
      break;
  }
  ir::Type_const inner = ir::ConstInt{.value = value & asdl::Int(0xFFFF)};
  if (std::holds_alternative<ir::UShort>(ty)) {
    inner = ir::ConstUInt{.value = value};
  }
  return ir::Cast{.target_type = ty, .exp = ir::Constant{.const_ = inner}};
}

void Descent::apply_statement_attributes(const Attributes& attrs, ir::Type_statement& stmt) {
  std::string what;
  switch (ir::kind(stmt)) {
    case ir::Kind_statement::Return:
      what = "a `return` statement";
      break;
    case ir::Kind_statement::Expression:
      what = "an expression statement";
      break;
    case ir::Kind_statement::IfStmt:
      what = "an `if` statement";
      break;
    case ir::Kind_statement::Compound:
      what = "a block";
      break;
    case ir::Kind_statement::BreakStmt:
      what = "a `break` statement";
      break;
    case ir::Kind_statement::ContinueStmt:
      what = "a `continue` statement";
      break;
    case ir::Kind_statement::WhileStmt:
      what = "a `while` statement";
      break;
    case ir::Kind_statement::DoWhileStmt:
      what = "a `do` statement";
      break;
    case ir::Kind_statement::ForStmt:
      what = "a `for` statement";
      break;
    case ir::Kind_statement::SwitchStmt:
      what = "a `switch` statement";
      break;
    case ir::Kind_statement::Goto:
      what = "a `goto` statement";
      break;
    default:
      throw InternalError("parser: attributes before a statement that isn't unlabeled");
  }
  std::set<std::string> seen;
  const std::optional<AttrList> specs = c6502_attributes(attrs, "statement", what);
  if (!specs) {
    return;
  }
  for (const auto& [name, arg] : *specs) {
    if (statement_attribute(name) == nullptr) {
      throw ParserError("`c6502::" + name + "` doesn't apply to a statement; saw it on " + what);
    }
    if (seen.contains(name)) {
      throw ParserError("`c6502::" + name + "` is given twice on " + what);
    }
    seen.insert(name);
    if (!arg.is_none()) {
      throw ParserError("`c6502::" + name + "` doesn't take an argument");
    }
    if (name == "unroll" || name == "keep") {
      auto* loop = std::get_if<ir::ForStmt>(&stmt);
      if (loop == nullptr) {
        throw ParserError("`c6502::" + name + "` goes before a `for` statement; saw it on " +
                          what);
      }
      if (loop->loop_attribute) {
        throw ParserError(
            "`c6502::unroll` and `c6502::keep` are both given on one `for` statement. An "
            "unrolled loop leaves no loop to keep");
      }
      loop->loop_attribute = name;
      continue;
    }
    // Any other statement holds statements of its own, and the
    // attribute would cover the calls nested in them.
    if (!std::holds_alternative<ir::Expression>(stmt) && !std::holds_alternative<ir::Return>(stmt)) {
      throw ParserError(
          "`[[c6502::save_hardware_stack]]` goes before an expression statement, a `return` "
          "statement, or a declaration, so that it covers only the calls in that one; saw it "
          "on " +
          what);
    }
    mark_hw_stack_save(stmt);
  }
}

void Descent::check_label_attributes(const Attributes& attrs) {
  for (const auto& [name, arg] : attrs.c6502) {
    if (statement_attribute(name) != nullptr) {
      throw ParserError("attributes before a label apply to the label, and `c6502::" + name +
                        "` doesn't apply to one. Put it after the label, just before the "
                        "statement, as in `L: [[c6502::" +
                        name + "]] ...`");
    }
    throw ParserError("`c6502::" + name + "` doesn't apply to a label");
  }
  c6502_attributes(attrs, "label", "a label");
}

}  // namespace c6502::parser
