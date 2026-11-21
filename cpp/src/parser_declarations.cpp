// Part R2 of the parser's descent (parser_descent.h): declarations,
// declarators, specifiers, typedef names, struct, union, and enum,
// attributes, `auto`, `typeof`, and `constexpr` (parser.py:525-1678,
// 1991-2847, and the methods of 2849-3350, 3506-3606, and 3717-4169).
//
// Each `parse_*` function takes the tokens of one construct of
// c99.lark and runs the construct's action, the method of parser.py's
// `_ASTBuilder` of the same name, when it has taken the construct's last
// token, which is when Lark calls it. parser_specifiers.cpp holds the
// checks of specifiers and attributes that the actions share, and
// parser_declarators.cpp the walks over declarator trees.
#include <algorithm>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <variant>

#include "bit_precise.h"
#include "parser_declarations.h"
#include "passes/type_checker.h"
#include "passes/type_checking_types.h"
#include "pyrepr.h"

namespace c6502::parser {

namespace ir = c99_ast;
namespace tct = passes::type_checking;

namespace {

using AttrList = std::vector<std::pair<std::string, AttributeArg>>;

std::string repr(std::string_view s) { return pyrepr::string(s); }

// Returns Python's `type(t).__name__` of the type `t`, its constructor's
// name.
std::string class_name(const Type& t) {
  const std::string r = asdl::py_repr(t);
  return r.substr(0, r.find('('));
}

// What `init_declarator` returns for `var_decl`: the declarator, the
// initializer, the attributes after the declarator's name, the
// struct/union tags a typedef's type introduces in this scope, and the
// type `auto` inferred.
struct InitDecl {
  Declarator declarator;
  std::optional<Exp> init;
  std::optional<Attributes> postfix;
  std::vector<std::pair<std::string, bool>> forward_tags;
  std::optional<Type> inferred;
};

// The value of `enumerator_list` so far: the last constant's value, and
// the lowest and highest.
struct EnumValues {
  asdl::Int last = 0;
  asdl::Int lowest = 0;
  asdl::Int highest = 0;
};

// The c6502 attributes, which parser.py's `_C6502_ATTRIBUTES` holds, in
// sorted order for the message that lists them.
const std::vector<std::string_view>& c6502_attribute_names() {
  static const std::vector<std::string_view> kNames = {
      "address", "keep",    "preserves_x", "preserves_y", "return_bool_in_carry",
      "save_hardware_stack", "section", "unroll",      "zeropage",    "zp_abi"};
  return kNames;
}

bool is_c6502_attribute(std::string_view name) {
  const auto& names = c6502_attribute_names();
  return std::find(names.begin(), names.end(), name) != names.end() || name.starts_with("in_");
}

// C23 §6.7.13.2 lets `__name__` spell a standard attribute `name`, and
// implementations do the same for their prefixes and attributes, so
// `[[__c6502__::__zp_abi__]]` is `[[c6502::zp_abi]]`.
std::string normalized_attribute_name(const std::string& name) {
  if (name.size() > 4 && name.starts_with("__") && name.ends_with("__")) {
    return name.substr(2, name.size() - 4);
  }
  return name;
}

// The binding of an object, function, or parameter of type `data_type`
// (none when the parser couldn't work it out), with its value when it's
// a named constant (parser.py's `_object_binding`).
Binding object_binding(const ParseContext& ctx, const std::optional<Type>& data_type,
                       std::optional<std::pair<Type, passes::ValueTree>> constant = std::nullopt) {
  Binding b;
  if (!data_type && !constant) {
    return b;
  }
  if (data_type) {
    for (const auto& [tag, is_union] : type_tags(*data_type)) {
      b.tag_scopes.insert_or_assign(tag, ctx.tag_scope(tag));
    }
  }
  b.constant = std::move(constant);
  b.object_type = data_type;
  return b;
}

Spec token_spec(const lark::Token& tok) {
  Spec s;
  s.kind = Spec::Kind::kToken;
  s.type = tok.type;
  s.text = tok.value;
  // `_Bool` is the same type as `bool` (C23 §6.4.1), so it becomes a BOOL
  // token that keeps its spelling for messages.
  if (tok.type == terminals().ALT_BOOL) {
    s.type = terminals().BOOL;
  }
  return s;
}

bool is_simple_type_keyword(const Terminals& t, int k) {
  return k == t.INT || k == t.LONG || k == t.SHORT || k == t.SIGNED || k == t.UNSIGNED ||
         k == t.FLOAT || k == t.DOUBLE || k == t.CHAR || k == t.BOOL || k == t.ALT_BOOL ||
         k == t.VOID;
}

bool is_storage_keyword(const Terminals& t, int k) {
  return k == t.STATIC || k == t.EXTERN || k == t.REGISTER || k == t.TYPEDEF ||
         k == t.CONSTEXPR || k == t.THREAD_LOCAL || k == t.ALT_THREAD_LOCAL || k == t.AUTO;
}

// The inline struct and union bodies in `specs`, as StructDecls, each
// added to the bodies the folds read, and its specifier emptied so the
// list can still give the type (parser.py's `_drain_inline_struct_bodies`).
std::vector<ir::Type_declaration> drain_inline_struct_bodies(ParseContext& ctx,
                                                             std::vector<Spec>& specs) {
  std::vector<ir::Type_declaration> out;
  for (Spec& s : specs) {
    if (s.kind != Spec::Kind::kTag || !s.body) {
      continue;
    }
    ir::Type_struct_decl sd{.tag = s.tag, .is_union = s.is_union};
    for (const auto& [name, type] : *s.body) {
      sd.members.push_back(ir::Type_member_decl{.name = name, .data_type = type});
    }
    out.push_back(ir::StructDecl{.struct_decl = sd});
    ctx.struct_bodies.push_back(sd);
    s.body.reset();
  }
  return out;
}

// Pops the specifiers of the declaration whose action runs, which its
// `decl_specs` pushed (parser.py's `_consume_specifiers`).
std::vector<Spec> consume_specifiers(ParseContext& ctx) {
  if (ctx.decl_specs.empty()) {
    throw InternalError("parser: the stack of declaration specifiers is empty");
  }
  std::shared_ptr<DeclSpecs> top = ctx.decl_specs.back();
  ctx.decl_specs.pop_back();
  return top->specs;
}

// Declares the enum tag `tag` in the current scope with the type
// `data_type`, which is its fixed underlying type when `fixed`. `defines`
// is true for the declaration with the list of constants. A tag may be
// declared again in the same scope only by `enum tag : T;` with a
// compatible T, or by a definition with one after such a declaration
// (C23 §6.7.3.3). Two integer types are compatible only when they're the
// same type, so the types are compared for equality.
void declare_enum_tag(ParseContext& ctx, const std::string& tag, const Type& data_type, bool fixed,
                      bool defines) {
  Scope& scope = *ctx.scopes.back();
  if (scope.tags.contains(tag)) {
    throw ParserError(repr(tag) +
                      " is a struct or union tag in this scope, so it can't be an enum tag too");
  }
  EnumTag* old = scope.enum_tags.find(tag);
  if (old == nullptr) {
    scope.enum_tags.insert_or_assign(tag, EnumTag{data_type, fixed, defines});
    return;
  }
  if (defines && old->defined) {
    throw ParserError("enum " + repr(tag) + " is defined twice");
  }
  if (fixed && !old->fixed) {
    throw ParserError("enum " + repr(tag) +
                      " was defined without a fixed underlying type, so it can't be declared "
                      "again with one (C23 §6.7.3.3)");
  }
  if (old->fixed && !fixed) {
    const std::string c = tct::c_type(old->data_type);
    throw ParserError("enum " + repr(tag) + " was declared with the fixed underlying type `" + c +
                      "`, so its definition must give that type too, as in `enum " + tag + " : " +
                      c + " { ... }` (C23 §6.7.3.3)");
  }
  if (!(old->data_type == data_type)) {
    throw ParserError("enum " + repr(tag) + " was declared with the underlying type `" +
                      tct::c_type(old->data_type) + "`, and here with `" +
                      tct::c_type(data_type) +
                      "`, which isn't compatible with it (C23 §6.7.3.3)");
  }
  old->defined = old->defined || defines;
}

// The least and greatest values of the integer type `t`, an enum's
// underlying type. Plain `char` is unsigned in c6502.
std::pair<asdl::Int, asdl::Int> integer_range(const Type& t) {
  if (std::holds_alternative<ir::Bool>(t)) {
    return {0, 1};
  }
  const int bits = tct::int_bits(t);
  if (tct::is_signed(t)) {
    return {-(asdl::Int(1) << (bits - 1)), (asdl::Int(1) << (bits - 1)) - 1};
  }
  return {0, (asdl::Int(1) << bits) - 1};
}

bool is_enum_underlying_type(const Type& t) {
  switch (ir::kind(t)) {
    case ir::Kind_data_type::Bool:
    case ir::Kind_data_type::Char:
    case ir::Kind_data_type::SChar:
    case ir::Kind_data_type::UChar:
    case ir::Kind_data_type::Short:
    case ir::Kind_data_type::UShort:
    case ir::Kind_data_type::Int:
    case ir::Kind_data_type::UInt:
    case ir::Kind_data_type::Long:
    case ir::Kind_data_type::ULong:
    case ir::Kind_data_type::LongLong:
    case ir::Kind_data_type::ULongLong:
      return true;
    default:
      return false;
  }
}

// The type specifiers and the qualifier tokens of a specifier-qualifier
// list, which `type_name` and `struct_declaration` split apart.
std::pair<std::vector<Spec>, std::vector<int>> split_specifier_qualifiers(
    const std::vector<Spec>& leaves) {
  std::vector<Spec> type_specs;
  std::vector<int> qualifiers;
  for (const Spec& s : leaves) {
    if (s.kind != Spec::Kind::kToken || is_type_specifier_type(s.type)) {
      type_specs.push_back(s);
    } else if (is_type_qualifier_type(s.type)) {
      qualifiers.push_back(s.type);
    }
  }
  return {type_specs, qualifiers};
}

class Declarations {
 public:
  explicit Declarations(Descent& p) : p_(p), t_(p.t), ctx_(p.ctx()) {}

  DeclList declaration(std::optional<Attributes> prefix);
  DeclList var_decl(std::optional<Attributes> prefix);
  Type type_name();

 private:
  // --- Specifiers ---
  std::vector<Spec> specifier_list(bool storage, std::optional<Spec> first,
                                   std::optional<lark::Token> first_token);
  std::shared_ptr<DeclSpecs> decl_specs(std::optional<Spec> first,
                                        std::optional<lark::Token> first_token);
  Spec typedef_name();
  Spec typeof_specifier();
  Spec bitint_specifier();
  Spec struct_or_union_specifier();
  std::optional<Spec> enum_specifier(bool allow_tag_decl);
  Type enum_type_specifier();
  void enumerator_list();
  std::vector<std::pair<std::string, Type>> struct_declaration();

  // --- Declarators ---
  enum class Mode { kNamed, kAbstract, kEither };
  Declarator declarator(Mode mode);
  std::vector<std::vector<lark::Token>> pointer();
  Suffix array_suffix();
  FunctionSuffix function_suffix(bool abstract);
  std::vector<Param> parameter_type_list(std::optional<lark::Token> first);
  Param parameter_declaration(std::optional<lark::Token> first);
  void declarator_action(const Declarator& d);

  // --- Declarations ---
  DeclList after_specifiers(std::optional<Attributes> prefix, bool function_allowed);
  DeclList tag_only_decl(const std::optional<Attributes>& prefix);
  DeclList function_decl(const std::optional<Attributes>& prefix, const Declarator& d,
                         ir::Block block);
  InitDecl init_declarator(Declarator d, std::optional<Exp> init);
  DeclList var_decl_action(std::optional<Attributes> prefix, std::vector<InitDecl>& inits);
  Type inferred_type(const SplitSpecs& marker, const Declarator& d,
                     const std::optional<Exp>& init, bool constexpr_);

  Descent& p_;
  const Terminals& t_;
  ParseContext& ctx_;
};

// --- Specifiers ---

std::vector<Spec> Declarations::specifier_list(bool storage, std::optional<Spec> first,
                                               std::optional<lark::Token> first_token) {
  std::vector<Spec> specs;
  if (first_token) {
    specs.push_back(token_spec(*first_token));
  }
  if (first) {
    specs.push_back(std::move(*first));
  }
  while (true) {
    const int k = p_.kind();
    if (is_type_qualifier_type(k) || is_simple_type_keyword(t_, k) ||
        (storage && is_storage_keyword(t_, k))) {
      specs.push_back(token_spec(p_.take()));
    } else if (k == t_.BITINT) {
      specs.push_back(bitint_specifier());
    } else if (k == t_.STRUCT || k == t_.UNION) {
      specs.push_back(struct_or_union_specifier());
    } else if (k == t_.ENUM) {
      specs.push_back(*enum_specifier(false));
    } else if (k == t_.TYPEDEF_NAME) {
      specs.push_back(typedef_name());
    } else if (k == t_.TYPEOF || k == t_.TYPEOF_UNQUAL) {
      specs.push_back(typeof_specifier());
    } else {
      break;
    }
  }
  if (specs.empty()) {
    p_.unexpected("a specifier list");
  }
  return specs;
}

std::shared_ptr<DeclSpecs> Declarations::decl_specs(std::optional<Spec> first,
                                                    std::optional<lark::Token> first_token) {
  auto specs = std::make_shared<DeclSpecs>();
  specs->specs = specifier_list(true, std::move(first), std::move(first_token));
  // The specifiers of a declaration go on the parser's stack, so
  // `init_declarator` sees them, and the declaration that consumes them
  // pops them. `struct foo;` declares the tag foo in this scope, even if
  // an outer scope declares one too (C99 §6.7.2.3p7). The parser reduces
  // `decl_specs` when it sees the `;`.
  if (ctx_.recent.second == t_.SEMICOLON && specs->specs.size() == 1 &&
      specs->specs[0].kind == Spec::Kind::kTag && !specs->specs[0].body) {
    ctx_.scopes.back()->tags.insert(specs->specs[0].tag);
  }
  ctx_.decl_specs.push_back(specs);
  return specs;
}

Spec Declarations::typedef_name() {
  const lark::Token tok = p_.expect(t_.TYPEDEF_NAME);
  return p_.action("typedef_name", [&] {
    // A use of a typedef name as a type specifier. The type goes in its
    // place, copied, so no two declarations share nodes.
    const std::string& name = tok.value;
    const Binding* binding = ctx_.lookup(name);
    if (binding == nullptr || !binding->is_typedef()) {
      throw InternalError("parser: " + name + " isn't a typedef name");
    }
    for (const auto& [tag, scope] : binding->tag_scopes) {
      if (ctx_.tag_scope(tag) != scope) {
        throw ParserError("typedef " + repr(name) + " refers to the struct or union " + repr(tag) +
                          " declared in an outer scope, which a declaration of " + repr(tag) +
                          " in an inner scope hides here; c6502 can't express that type here, "
                          "so give the inner struct or union another tag");
      }
    }
    Spec s;
    s.kind = Spec::Kind::kTypedef;
    s.type = t_.TYPEDEF_NAME;
    s.name = name;
    s.data_type = binding->data_type;
    s.enumerated = binding->enumerated;
    return s;
  });
}

Spec Declarations::typeof_specifier() {
  const lark::Token keyword = p_.take();
  p_.expect(t_.LPAREN);
  std::optional<Type> type_operand;
  std::optional<Exp> exp_operand;
  if (p_.at_type_name()) {
    type_operand = type_name();
  } else {
    exp_operand = p_.parse_exp();
  }
  p_.expect(t_.RPAREN);
  return p_.action("typeof_specifier", [&] {
    // The type a typeof specifier names (C23 §6.7.3.6): the operand
    // itself when it's a type, and otherwise the type of the expression,
    // which the type checker works out on a copy that has the types of
    // the names in scope. With `typeof_unqual` the type loses its
    // qualifiers, and an array's elements lose theirs. The operand isn't
    // evaluated, since c6502 has no variably modified types.
    const bool unqual = keyword.type == t_.TYPEOF_UNQUAL;
    Type type = type_operand ? *type_operand : p_.expression_type(*exp_operand, keyword.value);
    if (unqual) {
      type = unqualified_type(type);
    }
    Spec s;
    s.kind = Spec::Kind::kTypeof;
    s.type = t_.TYPEDEF_NAME;
    s.name = "`" + keyword.value + "`";
    s.data_type = std::move(type);
    return s;
  });
}

Spec Declarations::bitint_specifier() {
  p_.expect(t_.BITINT);
  p_.expect(t_.LPAREN);
  const Exp width_exp = p_.parse_constant_exp();
  p_.expect(t_.RPAREN);
  return p_.action("bitint_specifier", [&] {
    // N is an integer constant expression (C23 §6.7.3.1p4), folded here
    // as an array size is, and at most BITINT_MAXWIDTH. The least N
    // depends on `signed` or `unsigned`, which `resolve_data_type`
    // checks.
    asdl::Int width = 0;
    try {
      width = p_.fold_integer_constant(width_exp);
    } catch (const ParserError& e) {
      throw ParserError(
          std::string("the width N of _BitInt(N) must be an integer constant expression (C23 "
                      "§6.7.3.1p4): ") +
          e.what());
    }
    if (width > bit_precise::kBitIntMaxWidth) {
      throw ParserError("the width of _BitInt can be at most BITINT_MAXWIDTH, " +
                        std::to_string(bit_precise::kBitIntMaxWidth) + ", and it's " +
                        pyrepr::integer(width) + " (C23 §6.7.3.1p4)");
    }
    Spec s;
    s.kind = Spec::Kind::kBitInt;
    s.type = t_.BITINT;
    s.width = width;
    return s;
  });
}

Spec Declarations::struct_or_union_specifier() {
  const lark::Token kw = p_.take();
  const bool is_union = kw.type == t_.UNION;
  std::optional<Attributes> attrs;
  if (p_.at(t_.ATTR_OPEN)) {
    attrs = p_.parse_attribute_specifier_seq();
  }
  std::optional<lark::Token> tag_token;
  if (p_.at(t_.IDENTIFIER)) {
    tag_token = p_.take();
  }
  std::optional<std::vector<std::vector<std::pair<std::string, Type>>>> members;
  if (p_.at(t_.LBRACE)) {
    p_.take();
    members.emplace();
    do {
      members->push_back(struct_declaration());
    } while (!p_.at(t_.RBRACE));
    p_.take();
  } else if (!tag_token) {
    p_.unexpected("a struct or union specifier");
  }
  return p_.action(is_union ? "union_specifier" : "struct_specifier", [&] {
    // The tag is missing only with a body: `struct { ... }` gets a tag
    // of its own that no source tag can spell, so it's a distinct type.
    const std::string kind = is_union ? "union" : "struct";
    no_c6502_attributes(attrs, "tag", is_union ? "a union" : "a struct");
    std::string tag;
    if (tag_token) {
      tag = tag_token->value;
    } else {
      ctx_.anonymous_tags += 1;
      tag = "__anonymous_" + kind + "_" + std::to_string(ctx_.anonymous_tags);
    }
    if (!members && ctx_.tag_is_enum(tag).value_or(false)) {
      throw ParserError(repr(tag) + " is an enum tag, not a " + kind + " tag");
    }
    if (members && ctx_.scopes.back()->enum_tags.contains(tag)) {
      throw ParserError(repr(tag) + " is an enum tag in this scope, so it can't be a " + kind +
                        " tag too");
    }
    Spec s;
    s.kind = Spec::Kind::kTag;
    s.type = kw.type;
    s.tag = tag;
    s.is_union = is_union;
    if (!members) {
      ctx_.introduce_tag(tag);
      return s;
    }
    // A body declares the tag in this scope, hiding any outer
    // declaration of it.
    ctx_.scopes.back()->tags.insert(tag);
    std::vector<std::pair<std::string, Type>> body;
    std::set<std::string> seen;
    for (const auto& decl : *members) {
      for (const auto& [mname, mtype] : decl) {
        if (seen.contains(mname)) {
          throw ParserError("duplicate member " + repr(mname) + " in struct/union " + repr(tag));
        }
        seen.insert(mname);
        body.emplace_back(mname, mtype);
      }
    }
    if (body.empty()) {
      throw ParserError("struct/union " + repr(tag) +
                        " body is empty (C99 §6.7.2.1.7 — at least one member required)");
    }
    s.body = std::move(body);
    return s;
  });
}

std::vector<std::pair<std::string, Type>> Declarations::struct_declaration() {
  std::optional<Attributes> attrs;
  if (p_.at(t_.ATTR_OPEN)) {
    attrs = p_.parse_attribute_specifier_seq();
  }
  const std::vector<Spec> leaves = specifier_list(false, std::nullopt, std::nullopt);
  std::vector<Declarator> decls;
  decls.push_back(declarator(Mode::kNamed));
  while (p_.at(t_.COMMA)) {
    p_.take();
    decls.push_back(declarator(Mode::kNamed));
  }
  p_.expect(t_.SEMICOLON);
  return p_.action("struct_declaration", [&] {
    for (const Declarator& d : decls) {
      no_c6502_attributes(merge_attributes(attrs, declarator_attributes(d)), "member",
                          "member " + repr(declarator_name(d)));
    }
    const auto [type_specs, qualifiers] = split_specifier_qualifiers(leaves);
    // A struct or union defined inside a member's type is legal C99 but
    // rarely used, and the inner StructDecl would have nowhere to go in
    // the outer one's members.
    for (const Spec& s : type_specs) {
      if (s.kind == Spec::Kind::kTag && s.body) {
        throw ParserError(
            "nested struct/union definition inside a member isn't supported — declare the "
            "inner type at outer scope first");
      }
    }
    const Type base = apply_qualifiers(resolve_data_type(type_specs), qualifiers);
    // A member's type has to be a complete object type (C99
    // §6.7.2.1p2), which the type checker checks.
    std::vector<std::pair<std::string, Type>> out;
    for (const Declarator& d : decls) {
      Applied a = apply_declarator(p_, d, base);
      if (std::holds_alternative<ir::FunType>(a.type)) {
        throw ParserError("struct/union member " + repr(a.name) + " cannot have function type");
      }
      out.emplace_back(a.name, std::move(a.type));
    }
    return out;
  });
}

std::optional<Spec> Declarations::enum_specifier(bool allow_tag_decl) {
  p_.expect(t_.ENUM);
  std::optional<Attributes> attrs;
  if (p_.at(t_.ATTR_OPEN)) {
    attrs = p_.parse_attribute_specifier_seq();
  }
  std::optional<std::string> tag;
  if (p_.at(t_.IDENTIFIER)) {
    tag = p_.take().value;
  }
  std::optional<Type> fixed;
  if (p_.at(t_.COLON)) {
    fixed = enum_type_specifier();
  }
  if (p_.at(t_.LBRACE)) {
    p_.action("enum_head", [&] {
      // Reduced when the parser sees the `{` of the list of constants. A
      // fixed underlying type is pushed for `enumerator_list` to check
      // each value against, and a typed enum's tag is declared here,
      // since C23 §6.7.3.3 makes the type complete from the end of its
      // `: T`. An enum without one declares its tag at its `}`, once its
      // type is known.
      no_enum_attributes(attrs);
      if (fixed && tag) {
        declare_enum_tag(ctx_, *tag, *fixed, true, true);
      }
      ctx_.enum_types.push_back(fixed);
    });
    p_.take();
    enumerator_list();
    if (p_.at(t_.COMMA)) {
      p_.take();
    }
    p_.expect(t_.RBRACE);
    return p_.action("enum_specifier", [&] {
      ctx_.enum_types.pop_back();
      Type data_type = ir::Int{};
      if (fixed) {
        data_type = *fixed;
      } else if (tag) {
        declare_enum_tag(ctx_, *tag, data_type, false, true);
      }
      Spec s;
      s.kind = Spec::Kind::kEnum;
      s.type = t_.ENUM;
      s.name = tag ? "enum " + repr(*tag) : "enum";
      s.data_type = std::move(data_type);
      s.enumerated = true;
      s.defines = true;
      return std::optional<Spec>(std::move(s));
    });
  }
  if (fixed) {
    if (!allow_tag_decl || !tag || !p_.at(t_.SEMICOLON)) {
      p_.unexpected("an enum's head");
    }
    p_.take();
    // C23's declaration of an enum with a fixed underlying type apart
    // from its list of constants (§6.7.3.3). It declares the tag and
    // leaves nothing in the AST.
    p_.action("enum_tag_decl", [&] {
      no_enum_attributes(attrs);
      declare_enum_tag(ctx_, *tag, *fixed, true, false);
    });
    return std::nullopt;
  }
  if (!tag) {
    p_.unexpected("an enum specifier");
  }
  return p_.action("enum_specifier", [&] {
    // A use of a tag declared earlier.
    no_enum_attributes(attrs);
    const std::optional<bool> kind = ctx_.tag_is_enum(*tag);
    if (!kind) {
      throw ParserError("enum " + repr(*tag) +
                        " isn't defined. Only an enum with a fixed underlying type can be "
                        "declared before its constants, as in `enum " +
                        *tag + " : int;` (C23 §6.7.3.3)");
    }
    if (!*kind) {
      throw ParserError(repr(*tag) + " is a struct or union tag, not an enum tag");
    }
    Spec s;
    s.kind = Spec::Kind::kEnum;
    s.type = t_.ENUM;
    s.name = "enum " + repr(*tag);
    s.data_type = ctx_.enum_tag_type(*tag);
    s.enumerated = true;
    return std::optional<Spec>(std::move(s));
  });
}

Type Declarations::enum_type_specifier() {
  p_.expect(t_.COLON);
  std::vector<Spec> items;
  while (true) {
    const int k = p_.kind();
    if (is_type_qualifier_type(k) || is_simple_type_keyword(t_, k)) {
      items.push_back(token_spec(p_.take()));
    } else if (k == t_.BITINT) {
      items.push_back(bitint_specifier());
    } else if (k == t_.TYPEDEF_NAME) {
      items.push_back(typedef_name());
    } else if (k == t_.TYPEOF || k == t_.TYPEOF_UNQUAL) {
      items.push_back(typeof_specifier());
    } else if (k == t_.STRUCT || k == t_.UNION || k == t_.ENUM) {
      // `enum_type_tag`, which the grammar takes after an enum's `:` only
      // to report it.
      const lark::Token keyword = p_.take();
      const lark::Token tag = p_.expect(t_.IDENTIFIER);
      p_.action("enum_type_tag", [&] {
        if (keyword.type == t_.ENUM) {
          throw ParserError("the underlying type of an enum can't be an enumerated type such as "
                            "`enum " +
                            tag.value + "`. Give its integer type instead (C23 §6.7.3.3)");
        }
        throw ParserError("the underlying type of an enum must be an integer type, and `" +
                          keyword.value + " " + tag.value + "` isn't one (C23 §6.7.3.3)");
      });
    } else {
      break;
    }
  }
  if (items.empty()) {
    p_.unexpected("an enum's underlying type");
  }
  return p_.action("enum_type_specifier", [&] {
    // The underlying type must be an integer type other than an
    // enumerated type or a bit-precise one (C23 §6.7.3.3). The
    // qualifiers are dropped, since the underlying type is the
    // unqualified one.
    std::vector<Spec> specs;
    for (const Spec& s : items) {
      if (!(s.kind == Spec::Kind::kToken && is_type_qualifier_type(s.type))) {
        specs.push_back(s);
      }
    }
    for (const Spec& s : specs) {
      if (s.is_typedef_like() && s.enumerated) {
        throw ParserError("the underlying type of an enum can't be an enumerated type, and " +
                          repr(s.name) + " is one. Give its integer type instead (C23 §6.7.3.3)");
      }
    }
    Type data_type = unqualified(resolve_data_type(specs));
    if (std::holds_alternative<ir::BitInt>(data_type)) {
      throw ParserError(
          "the underlying type of an enum can't be a bit-precise integer type such as `" +
          tct::c_type(data_type) + "` (C23 §6.7.3.3)");
    }
    if (!is_enum_underlying_type(data_type)) {
      throw ParserError("the underlying type of an enum must be an integer type, and `" +
                        tct::c_type(data_type) + "` isn't one (C23 §6.7.3.3)");
    }
    return data_type;
  });
}

void Declarations::enumerator_list() {
  std::optional<EnumValues> prev;
  while (true) {
    const lark::Token name_token = p_.expect(t_.IDENTIFIER);
    std::optional<Attributes> attrs;
    if (p_.at(t_.ATTR_OPEN)) {
      attrs = p_.parse_attribute_specifier_seq();
    }
    std::optional<Exp> exp;
    if (p_.at(t_.ASSIGN)) {
      p_.take();
      exp = p_.parse_constant_exp();
    }
    const std::string name = name_token.value;
    p_.action("enumerator", [&] {
      no_c6502_attributes(attrs, "enumerator", "enumeration constant " + repr(name));
    });
    prev = p_.action("enumerator_list", [&] {
      // The constant is in scope from here on (C99 §6.2.1p7), and its
      // value is the expression's, or one more than the one before it,
      // or 0 for the first (§6.7.2.2p3). With a fixed underlying type
      // (C23 §6.7.3.3) the value must be representable in that type, and
      // the constant has that type. Without one it must be representable
      // as an `int` (§6.7.2.2p2).
      asdl::Int value = 0;
      if (exp) {
        try {
          value = p_.fold_integer_constant(*exp);
        } catch (const ParserError& e) {
          throw ParserError("the value of enumeration constant " + repr(name) +
                            " must be an integer constant expression: " + e.what());
        }
      } else {
        value = prev ? prev->last + 1 : 0;
      }
      const std::optional<Type>& fixed = ctx_.enum_types.back();
      if (!fixed) {
        if (value < -0x8000 || value > 0x7FFF) {
          throw ParserError("enumeration constant " + repr(name) + " is " +
                            pyrepr::integer(value) +
                            ", which doesn't fit in an int (-32768..32767)");
        }
      } else {
        const auto [lowest, highest] = integer_range(*fixed);
        if (value < lowest || value > highest) {
          const std::string what =
              exp ? "is " + pyrepr::integer(value)
                  : "would be " + pyrepr::integer(value) + ", one more than the constant before it";
          throw ParserError("enumeration constant " + repr(name) + " " + what +
                            ", which doesn't fit in the enum's underlying type `" +
                            tct::c_type(*fixed) + "` (" + pyrepr::integer(lowest) + ".." +
                            pyrepr::integer(highest) + ", C23 §6.7.3.3)");
        }
      }
      Binding b;
      b.enum_value = value;
      b.enum_type = fixed;
      ctx_.declare(name, std::move(b));
      if (!prev) {
        return EnumValues{value, value, value};
      }
      return EnumValues{value, std::min(prev->lowest, value), std::max(prev->highest, value)};
    });
    if (!p_.at(t_.COMMA)) {
      return;
    }
    p_.take();
    if (p_.at(t_.RBRACE)) {
      return;
    }
  }
}

// --- Declarators ---

std::vector<std::vector<lark::Token>> Declarations::pointer() {
  // `pointer: STAR type_qualifier* pointer?`.
  std::vector<std::vector<lark::Token>> levels;
  while (p_.at(t_.STAR)) {
    p_.take();
    std::vector<lark::Token> quals;
    while (is_type_qualifier_type(p_.kind())) {
      quals.push_back(p_.take());
    }
    levels.push_back(std::move(quals));
  }
  return levels;
}

Suffix Declarations::array_suffix() {
  // `[ array_suffix ]`, at the `[`.
  p_.expect(t_.LBRACKET);
  Suffix s;
  s.is_array = true;
  ArraySuffix& a = s.array;
  if (p_.at(t_.STATIC)) {
    p_.take();
    while (is_type_qualifier_type(p_.kind())) {
      a.qualifiers.push_back(p_.take());
    }
    a.alias = "array_size_static";
    a.size = p_.parse_assignment_exp();
  } else {
    while (is_type_qualifier_type(p_.kind())) {
      a.qualifiers.push_back(p_.take());
    }
    if (!a.qualifiers.empty() && p_.at(t_.STATIC)) {
      p_.take();
      a.alias = "array_size_quals_static";
      a.size = p_.parse_assignment_exp();
    } else if (p_.at(t_.STAR)) {
      // `[*]` is an unspecified size, and `[*p]` a size that starts
      // with a dereference, which the token after the `*` tells apart.
      p_.take();
      if (p_.at(t_.RBRACKET)) {
        a.alias = "array_size_unspec";
      } else {
        a.alias = "array_size_plain";
        // The descent has no way to parse the rest of an expression
        // after its first operator, so `*e` is taken as the dereference
        // of the assignment expression `e`, which gives `*p + 1` the
        // wrong shape. Such a size is a variable-length array, which the
        // fold rejects, and only its message shows the shape.
        a.size = ir::Dereference{.exp = p_.parse_assignment_exp()};
      }
    } else {
      a.alias = "array_size_plain";
      if (!p_.at(t_.RBRACKET)) {
        a.size = p_.parse_assignment_exp();
      }
    }
  }
  p_.expect(t_.RBRACKET);
  return s;
}

FunctionSuffix Declarations::function_suffix(bool abstract) {
  // `( ... )`, at the `(`.
  p_.expect(t_.LPAREN);
  FunctionSuffix f;
  if (p_.at(t_.RPAREN)) {
    f.kind = FunctionSuffix::Kind::kEmpty;
  } else if (p_.at(t_.VOID)) {
    // `(void)` lists no parameters, where `(void *p)` starts a parameter
    // declaration, and Lark's resolution of the conflict at the `)`
    // shifts it.
    lark::Token void_token = p_.take();
    if (p_.at(t_.RPAREN)) {
      f.kind = FunctionSuffix::Kind::kVoid;
    } else {
      f.kind = FunctionSuffix::Kind::kParams;
      f.params = parameter_type_list(void_token);
    }
  } else if (!abstract && p_.at(t_.IDENTIFIER)) {
    f.kind = FunctionSuffix::Kind::kIdentifierList;
    p_.take();
    while (p_.at(t_.COMMA)) {
      p_.take();
      p_.expect(t_.IDENTIFIER);
    }
  } else {
    f.kind = FunctionSuffix::Kind::kParams;
    f.params = parameter_type_list(std::nullopt);
  }
  p_.expect(t_.RPAREN);
  return f;
}

std::vector<Param> Declarations::parameter_type_list(std::optional<lark::Token> first) {
  std::vector<Param> params;
  params.push_back(parameter_declaration(std::move(first)));
  while (p_.at(t_.COMMA)) {
    p_.take();
    if (p_.at(t_.ELLIPSIS)) {
      p_.take();
      p_.action("parameter_or_ellipsis", []() {
        // c6502 has no variadic AST node.
        throw ParserError("variadic functions ('...') aren't supported yet");
      });
    }
    params.push_back(parameter_declaration(std::nullopt));
  }
  return params;
}

Param Declarations::parameter_declaration(std::optional<lark::Token> first) {
  std::optional<Attributes> attrs;
  if (!first && p_.at(t_.ATTR_OPEN)) {
    attrs = p_.parse_attribute_specifier_seq();
  }
  decl_specs(std::nullopt, std::move(first));
  Declarator d = declarator(Mode::kEither);
  return p_.action("parameter_declaration", [&] {
    // C99 §6.7.5.3p2 permits only one storage class on a parameter,
    // `register`, which routes the argument through a 6502 register. An
    // unnamed parameter, as in `int foo(int *)`, has no name.
    std::vector<Spec> specs = consume_specifiers(ctx_);
    const bool has_declarator = d.pointer || d.direct;
    const bool named = has_declarator && !d.abstract;
    std::optional<Attributes> all = attrs;
    std::string where = "a parameter";
    if (named) {
      all = merge_attributes(attrs, declarator_attributes(d));
      where = "parameter " + repr(declarator_name(d));
    }
    const std::optional<AttrList> param_specs = c6502_attributes(all, "parameter", where);
    // A struct or union defined in a parameter declaration is legal C99
    // but obscure, since the tag's scope would be the prototype's only.
    for (const Spec& s : specs) {
      if (s.kind == Spec::Kind::kTag && s.body) {
        throw ParserError("struct/union definition isn't permitted in a parameter declaration");
      }
    }
    if (has_constexpr(specs) || has_auto(specs)) {
      const std::string keyword = has_constexpr(specs) ? "constexpr" : "auto";
      throw ParserError("`" + keyword + "` isn't permitted on " + where +
                        ", where the only storage-class specifier is `register`");
    }
    const SplitSpecs split = split_specifiers(specs);
    const bool is_register = split.storage == Storage::kRegister;
    if (split.storage != Storage::kNone && !is_register) {
      throw ParserError("storage class " + repr(storage_name(split.storage)) +
                        " isn't permitted on a function parameter (C99 §6.7.5.3.2 — only "
                        "register)");
    }
    const std::string marker = register_marker(param_specs, is_register);
    const Type& base = *split.type;
    if (!has_declarator) {
      return Param{std::nullopt, adjust_param_type(base), marker};
    }
    if (named) {
      Applied a = apply_declarator(p_, d, base);
      return Param{a.name, adjust_param_type(a.type), marker};
    }
    return Param{std::nullopt, adjust_param_type(apply_abstract_declarator(p_, d, base)), marker};
  });
}

Declarator Declarations::declarator(Mode mode) {
  Declarator d;
  if (p_.at(t_.STAR)) {
    d.pointer = pointer();
  }
  Direct dd;
  bool abstract = true;
  if (mode != Mode::kAbstract && p_.at(t_.IDENTIFIER)) {
    dd.core = Direct::Core::kName;
    dd.name = p_.take().value;
    if (p_.at(t_.ATTR_OPEN)) {
      dd.attrs = p_.parse_attribute_specifier_seq();
    }
    abstract = false;
  } else if (p_.at(t_.LPAREN) && mode == Mode::kNamed) {
    p_.take();
    dd.core = Direct::Core::kParen;
    dd.inner = std::make_shared<Declarator>(declarator(Mode::kNamed));
    p_.expect(t_.RPAREN);
    abstract = false;
  } else if (p_.at(t_.LPAREN)) {
    // In an abstract declarator, `(` starts a function suffix when a
    // parameter list, `void`, or `)` follows, and a declarator in
    // parentheses otherwise.
    p_.take();
    if (p_.at(t_.RPAREN) || p_.at(t_.ATTR_OPEN) || p_.at_declaration_specifier()) {
      // The `(` is taken, so the suffix is parsed from its contents.
      Suffix s;
      FunctionSuffix& f = s.function;
      if (p_.at(t_.RPAREN)) {
        f.kind = FunctionSuffix::Kind::kEmpty;
      } else if (p_.at(t_.VOID)) {
        lark::Token void_token = p_.take();
        if (p_.at(t_.RPAREN)) {
          f.kind = FunctionSuffix::Kind::kVoid;
        } else {
          f.kind = FunctionSuffix::Kind::kParams;
          f.params = parameter_type_list(void_token);
        }
      } else {
        f.kind = FunctionSuffix::Kind::kParams;
        f.params = parameter_type_list(std::nullopt);
      }
      p_.expect(t_.RPAREN);
      dd.suffixes.push_back(std::move(s));
      dd.core = Direct::Core::kNone;
    } else {
      dd.core = Direct::Core::kParen;
      dd.inner = std::make_shared<Declarator>(declarator(mode));
      p_.expect(t_.RPAREN);
      abstract = dd.inner->abstract;
    }
  } else if (mode != Mode::kNamed && p_.at(t_.LBRACKET)) {
    dd.core = Direct::Core::kNone;
    dd.suffixes.push_back(array_suffix());
  } else {
    if (mode == Mode::kNamed) {
      p_.unexpected("a declarator");
    }
    d.abstract = true;
    return d;
  }
  while (p_.at(t_.LBRACKET) || p_.at(t_.LPAREN)) {
    if (p_.at(t_.LBRACKET)) {
      dd.suffixes.push_back(array_suffix());
    } else {
      Suffix s;
      s.function = function_suffix(abstract);
      dd.suffixes.push_back(std::move(s));
    }
  }
  d.abstract = abstract;
  d.direct = std::move(dd);
  if (!abstract) {
    declarator_action(d);
  }
  return d;
}

void Declarations::declarator_action(const Declarator& d) {
  p_.action("declarator", [&] {
    // A function definition's declarator reduces when the parser sees
    // the `{` of its body, after the lexer pushed the body's scope; the
    // parameters belong in that scope, where they hide any typedef names
    // they share.
    ScopePtr scope = ctx_.body_scope;
    if (scope == nullptr) {
      return;
    }
    ctx_.body_scope = nullptr;
    const Applied f = apply_declarator(p_, d, ir::Int{});
    // The function's name is in scope from the end of its declarator
    // (C99 §6.2.1p7), so its body can name it, as in `typeof(f)`. The
    // function definition declares it again when its body ends.
    try {
      if (!ctx_.decl_specs.empty()) {
        const SplitSpecs split = split_specifiers(ctx_.decl_specs.back()->specs);
        if (split.type) {
          Applied own = apply_declarator(p_, d, *split.type);
          if (std::holds_alternative<ir::FunType>(own.type)) {
            ctx_.declare(own.name, object_binding(ctx_, own.type), ctx_.scopes.front());
          }
        }
      }
    } catch (const CompileError& e) {
      if (e.python_type() != "ParserError" && e.python_type() != "NotImplementedError") {
        throw;
      }
    }
    std::vector<Type> param_types;
    if (const auto* fun = std::get_if<ir::FunType>(&f.type)) {
      param_types = fun->params;
    }
    if (f.params) {
      for (std::size_t i = 0; i < f.params->size(); ++i) {
        const std::optional<std::string>& param = (*f.params)[i];
        if (param) {
          std::optional<Type> type;
          if (i < param_types.size()) {
            type = param_types[i];
          }
          ctx_.declare(*param, object_binding(ctx_, type), scope);
        }
      }
    }
  });
}

// --- Declarations ---

DeclList Declarations::declaration(std::optional<Attributes> prefix) {
  if (prefix && p_.at(t_.SEMICOLON)) {
    p_.take();
    return p_.action("attribute_declaration", [&] {
      // An attribute declaration declares nothing. `[[fallthrough]];` is
      // the only standard one, and belongs in a switch statement.
      no_c6502_attributes(prefix, "attribute declaration", "an attribute declaration");
      if (!prefix->standard.empty() && ctx_.scopes.size() == 1) {
        throw ParserError("`[[fallthrough]];` belongs in a switch statement, not at file scope");
      }
      return DeclList{};
    });
  }
  std::optional<Spec> first;
  if (!prefix && p_.at(t_.ENUM)) {
    first = enum_specifier(true);
    if (!first) {
      return DeclList{};
    }
  }
  decl_specs(std::move(first), std::nullopt);
  return after_specifiers(std::move(prefix), true);
}

DeclList Declarations::var_decl(std::optional<Attributes> prefix) {
  decl_specs(std::nullopt, std::nullopt);
  return after_specifiers(std::move(prefix), false);
}

DeclList Declarations::after_specifiers(std::optional<Attributes> prefix, bool function_allowed) {
  if (p_.at(t_.SEMICOLON)) {
    p_.take();
    return tag_only_decl(prefix);
  }
  Declarator d = declarator(Mode::kNamed);
  if (function_allowed && p_.at(t_.LBRACE)) {
    ir::Block block = p_.parse_block();
    return function_decl(prefix, d, std::move(block));
  }
  std::vector<InitDecl> inits;
  while (true) {
    std::optional<Exp> init;
    if (p_.at(t_.ASSIGN)) {
      p_.take();
      init = p_.parse_initializer();
    }
    inits.push_back(init_declarator(std::move(d), std::move(init)));
    if (!p_.at(t_.COMMA)) {
      break;
    }
    p_.take();
    d = declarator(Mode::kNamed);
  }
  p_.expect(t_.SEMICOLON);
  return var_decl_action(std::move(prefix), inits);
}

DeclList Declarations::tag_only_decl(const std::optional<Attributes>& prefix) {
  return p_.action("tag_only_decl", [&] {
    // Struct and union forward declarations (`struct foo;`) and
    // definitions with no declarator (`struct foo { int a; };`). Storage
    // classes aren't legal here, and leading attributes need a
    // declarator after them (C23 §6.7.2); a struct's go after `struct`.
    if (prefix) {
      throw ParserError(
          "attributes before a declaration need a declarator to apply to; a struct's, union's, "
          "or enum's go after its keyword, as in `struct [[deprecated]] s { ... };`");
    }
    std::vector<Spec> specs = consume_specifiers(ctx_);
    DeclList out;
    out.decls = drain_inline_struct_bodies(ctx_, specs);
    if (std::any_of(specs.begin(), specs.end(), [&](const Spec& s) { return s.type == t_.TYPEDEF; })) {
      throw ParserError("typedef declaration declares no name");
    }
    if (has_constexpr(specs)) {
      throw ParserError("a `constexpr` declaration must declare an object");
    }
    const auto is_static_extern = [&](const Spec& s) {
      return s.type == t_.STATIC || s.type == t_.EXTERN;
    };
    if (std::any_of(specs.begin(), specs.end(), is_static_extern)) {
      throw ParserError(
          "storage-class specifier isn't permitted on a struct/union declaration without a "
          "declarator");
    }
    std::vector<Spec> non_storage;
    for (const Spec& s : specs) {
      if (!is_static_extern(s)) {
        non_storage.push_back(s);
      }
    }
    if (non_storage.empty()) {
      throw ParserError("empty declaration");
    }
    if (std::any_of(specs.begin(), specs.end(),
                    [](const Spec& s) { return s.kind == Spec::Kind::kEnum && s.defines; })) {
      // `enum e { A, B };` declares its constants, which the parser has
      // already done, and nothing for the AST.
      return out;
    }
    const Type ts = resolve_data_type(non_storage);
    const auto* st = std::get_if<ir::Structure>(&ts);
    const auto* un = std::get_if<ir::Union>(&ts);
    if (st == nullptr && un == nullptr) {
      throw ParserError(
          "declaration does not declare anything (a tag, a member of an enumeration, or an "
          "object/function)");
    }
    const std::string& tag = st != nullptr ? st->tag : un->tag;
    if (!out.decls.empty() &&
        std::get<ir::StructDecl>(out.decls.back()).struct_decl.tag == tag) {
      // The drained body is the declaration.
      return out;
    }
    // A forward declaration, `struct foo;`.
    out.decls.push_back(ir::StructDecl{
        .struct_decl = ir::Type_struct_decl{.tag = tag, .is_union = un != nullptr}});
    return out;
  });
}

DeclList Declarations::function_decl(const std::optional<Attributes>& prefix,
                                     const Declarator& d, ir::Block block) {
  return p_.action("function_decl", [&] {
    // A function definition. A declaration without a body is a
    // `var_decl` whose declarator composes to a FunType. The declarator
    // must compose to a FunType here too.
    std::vector<Spec> specs = consume_specifiers(ctx_);
    DeclList out;
    out.decls = drain_inline_struct_bodies(ctx_, specs);
    const SplitSpecs split = split_specifiers(specs);
    if (has_auto(specs)) {
      throw ParserError("`auto` can't go on function definition " + repr(declarator_name(d)) +
                        ": c6502 doesn't infer a function's return type (C23 §6.7.1p12 leaves it "
                        "to the implementation), and a function has no automatic storage");
    }
    Applied a = apply_declarator(p_, d, *split.type);
    const std::string where = "function definition " + repr(a.name);
    if (has_constexpr(specs)) {
      throw ParserError("`constexpr` declares a named constant, which is an object, and " + where +
                        " declares a function");
    }
    const std::optional<Attributes> def_attrs = merge_attributes(prefix, declarator_attributes(d));
    const std::optional<AttrList> attr_specs = c6502_attributes(def_attrs, "function", where);
    const FunctionAttributes fa = extract_function_attributes(attr_specs, where);
    if (!std::holds_alternative<ir::FunType>(a.type)) {
      throw ParserError(
          "function definition's declarator must compose to a function type; got " +
          class_name(a.type));
    }
    if (split.storage == Storage::kRegister || split.storage == Storage::kTypedef) {
      throw ParserError("storage class " + repr(storage_name(split.storage)) +
                        " isn't permitted on function definition " + repr(a.name));
    }
    if (!a.params) {
      throw ParserError("function definition " + repr(a.name) +
                        " has to list its parameters in its declarator; its type can't come from "
                        "a typedef (C99 §6.9.1.2)");
    }
    ctx_.declare(a.name, object_binding(ctx_, a.type), ctx_.scopes.front());
    if (fa.address) {
      throw ParserError("function " + repr(a.name) +
                        " has `[[c6502::address(...)]]`, which names code that's already at that "
                        "address, so it can't have a body");
    }
    if (!fa.preserves.empty()) {
      throw ParserError("function " + repr(a.name) +
                        " is defined here, so it can't have `preserves_x` or `preserves_y`. "
                        "Those describe a routine at a fixed address "
                        "(`[[c6502::address(...)]]`), whose code the compiler doesn't compile "
                        "and so can't check");
    }
    ir::Type_function_decl fd;
    fd.name = a.name;
    for (const auto& n : *a.params) {
      fd.params.push_back(n.value_or(""));
    }
    fd.body = ir::Type_block{std::move(block)};
    fd.data_type = a.type;
    fd.storage_class = storage_class(split.storage);
    fd.abi_annotation = fa.abi_annotation;
    fd.param_registers =
        a.registers ? *a.registers : std::vector<std::string>(a.params->size(), "");
    fd.carry_return = fa.carry_return;
    fd.section = extract_section(attr_specs, where);
    fd.noreturn = is_noreturn(def_attrs);
    out.decls.push_back(ir::FunctionDecl{.function_decl = std::move(fd)});
    return out;
  });
}

Type Declarations::inferred_type(const SplitSpecs& marker, const Declarator& d,
                                 const std::optional<Exp>& init, bool constexpr_) {
  // The type C23's `auto` infers (§6.7.10): the initializer's type after
  // lvalue conversion, which drops its qualifiers, and the conversion of
  // an array or a function to a pointer, with the qualifiers of the
  // declaration specifiers. The declarator must be a plain name with an
  // `= expression` initializer, which can't use the name, since the
  // name's scope starts after it (§6.7.10p5).
  const std::string name = declarator_name(d);
  const Applied shape = apply_declarator(p_, d, ir::Int{});
  if (!(shape.type == Type{ir::Int{}})) {
    throw ParserError("`auto` infers the type of a plain name, and the declarator of `" + name +
                      "` adds to it, which c6502 doesn't take (C23 §6.7.10p2 leaves it to the "
                      "implementation)");
  }
  if (!init) {
    throw ParserError("`auto` object `" + name +
                      "` has no initializer to infer its type from (C23 §6.7.10p2)");
  }
  if (std::holds_alternative<ir::InitList>(*init)) {
    throw ParserError("the initializer of `auto` object `" + name +
                      "` is a list in braces, and `auto` infers a type only from an expression "
                      "(C23 §6.7.10p2)");
  }
  const std::vector<std::string> names = names_in(*init);
  if (std::find(names.begin(), names.end(), name) != names.end()) {
    throw ParserError("the initializer of `auto` object `" + name + "` uses the name `" + name +
                      "`, which isn't in scope until the initializer ends and hides any `" +
                      name + "` outside (C23 §6.7.10p5)");
  }
  Type type = unqualified(
      p_.expression_type(*init, "auto", "the initializer of `auto` object `" + name + "`"));
  if (const auto* arr = std::get_if<ir::Array>(&type)) {
    type = ir::Pointer{.referenced_type = *arr->element_type};
  } else if (std::holds_alternative<ir::FunType>(type)) {
    type = ir::Pointer{.referenced_type = type};
  }
  type = apply_qualifiers(type, qualifier_kinds(marker.qualifiers));
  return constexpr_ ? p_.constexpr_type(type) : type;
}

InitDecl Declarations::init_declarator(Declarator d, std::optional<Exp> init) {
  return p_.action("init_declarator", [&] {
    // Declares the name now, so the lexer knows it from the next token
    // on: a typedef name, or a name that hides one. A typedef also lists
    // the tags of its type that live in this scope, which
    // `typedef_declaration` makes forward declarations of.
    InitDecl out{.declarator = d, .init = init, .postfix = declarator_attributes(d)};
    if (ctx_.decl_specs.empty()) {
      throw InternalError("parser: init_declarator without specifiers");
    }
    const DeclSpecs& specs = *ctx_.decl_specs.back();
    const std::string name = declarator_name(d);
    if (!specs.is_typedef()) {
      // The object's or function's type, for a `typeof` that names it,
      // and its value when it's a named constant (C23 §6.6p7), which the
      // constant expressions the parser folds may use. A type `auto`
      // infers is worked out here, before the name is declared, and goes
      // back to `var_decl`.
      const SplitSpecs split = split_specifiers(specs.specs);
      const bool constexpr_ = has_constexpr(specs.specs);
      if (split.inferred) {
        Type inferred = inferred_type(split, d, init, constexpr_);
        std::optional<std::pair<Type, passes::ValueTree>> constant;
        if (constexpr_) {
          constant = p_.fold_named_constant(inferred, *init);
        }
        ctx_.declare(name, object_binding(ctx_, inferred, std::move(constant)));
        out.inferred = std::move(inferred);
        return out;
      }
      std::optional<Type> composed;
      try {
        composed = apply_declarator(p_, d, *split.type).type;
      } catch (const CompileError& e) {
        if (e.python_type() != "ParserError" && e.python_type() != "NotImplementedError") {
          throw;
        }
      }
      std::optional<std::pair<Type, passes::ValueTree>> constant;
      if (composed && init) {
        // `int a[] = {1, 2};` takes its size from the initializer (C99
        // §6.7.8p22), so `typeof(a)` has it.
        if (const auto* arr = std::get_if<ir::Array>(&*composed); arr != nullptr && arr->size == 0) {
          try {
            composed = tct::size_from_initializer(*composed, &*init, "the array");
          } catch (const tct::TypeCheckError&) {
          }
        }
      }
      if (composed && constexpr_) {
        composed = p_.constexpr_type(*composed);
        if (init && !std::holds_alternative<ir::FunType>(*composed)) {
          constant = p_.fold_named_constant(*composed, *init);
        }
      }
      ctx_.declare(name, object_binding(ctx_, composed, std::move(constant)));
      return out;
    }
    const SplitSpecs split = split_specifiers(specs.specs);
    const Applied a = apply_declarator(p_, d, *split.type);
    Binding b;
    b.data_type = a.type;
    for (const auto& [tag, is_union] : type_tags(a.type)) {
      b.tag_scopes.insert_or_assign(tag, ctx_.tag_scope(tag));
    }
    // The typedef names an enumerated type when its type comes from an
    // enum specifier, or another such typedef name, with no pointer,
    // array, or function declarator around it.
    const bool from_enum = std::any_of(specs.specs.begin(), specs.specs.end(), [](const Spec& s) {
      return s.is_typedef_like() && s.enumerated;
    });
    b.enumerated = from_enum && unqualified(a.type) == unqualified(*split.type);
    for (const auto& [tag, is_union] : type_tags(a.type)) {
      const ScopePtr* scope = b.tag_scopes.find(tag);
      if (*scope == ctx_.scopes.back()) {
        out.forward_tags.emplace_back(tag, is_union);
      }
    }
    ctx_.declare(a.name, std::move(b));
    return out;
  });
}

DeclList Declarations::var_decl_action(std::optional<Attributes> prefix,
                                       std::vector<InitDecl>& inits) {
  return p_.action("var_decl", [&] {
    // The specifiers give the base type, and each init-declarator wraps
    // it with its pointer, array, and function modifiers and names the
    // identifier. A declarator that composes to a FunType is a function
    // declaration without a body (`int foo(int);`), which can't have an
    // initializer. An inline struct or union body in the specifiers
    // comes first in the list, then one declaration per declarator.
    std::vector<Spec> specs = consume_specifiers(ctx_);
    DeclList out;
    std::vector<ir::Type_declaration> struct_decls = drain_inline_struct_bodies(ctx_, specs);
    const SplitSpecs split = split_specifiers(specs);
    const bool is_constexpr = has_constexpr(specs);
    if (has_auto(specs) && !split.inferred && ctx_.scopes.size() == 1) {
      throw ParserError(
          "`auto` at file scope only marks a type to infer, and this declaration has a type "
          "specifier (C23 §6.7.2p4)");
    }
    // `[[c6502::save_hardware_stack]]` before a declaration in a block
    // marks the calls in its initializers.
    const bool hw_stack_save = take_hw_stack_save(prefix);
    if (split.storage == Storage::kTypedef) {
      if (hw_stack_save) {
        throw ParserError(
            "`[[c6502::save_hardware_stack]]` applies to the calls in a declaration's "
            "initializers, and a typedef declaration has none");
      }
      // A typedef declaration leaves behind only the struct/union tags
      // its types introduced into this scope, such as `foo` in `typedef
      // struct foo *P;` before any other declaration of `struct foo`. A
      // forward declaration stands in for each, so identifier resolution
      // introduces the tag in the same scope.
      std::set<std::string> declared;
      for (const auto& sd : struct_decls) {
        declared.insert(std::get<ir::StructDecl>(sd).struct_decl.tag);
      }
      out.decls = std::move(struct_decls);
      for (const InitDecl& id : inits) {
        const std::string name = declarator_name(id.declarator);
        if (id.init) {
          throw ParserError("typedef " + repr(name) + " can't have an initializer");
        }
        no_c6502_attributes(merge_attributes(prefix, id.postfix), "typedef",
                            "typedef " + repr(name));
        for (const auto& [tag, is_union] : id.forward_tags) {
          if (!declared.contains(tag)) {
            declared.insert(tag);
            out.decls.push_back(ir::StructDecl{
                .struct_decl = ir::Type_struct_decl{.tag = tag, .is_union = is_union}});
          }
        }
      }
      return out;
    }
    if (split.inferred && inits.size() > 1) {
      throw ParserError(
          "an `auto` declaration that infers its type declares one name, and c6502 doesn't take "
          "more (C23 §6.7.1p12 leaves it to the implementation)");
    }
    out.decls = std::move(struct_decls);
    for (InitDecl& id : inits) {
      std::string name;
      Type composed;
      std::optional<std::vector<std::optional<std::string>>> param_names;
      std::optional<std::vector<std::string>> param_regs;
      if (id.inferred) {
        name = declarator_name(id.declarator);
        composed = *id.inferred;
      } else {
        Applied a = apply_declarator(p_, id.declarator, *split.type);
        name = std::move(a.name);
        composed = std::move(a.type);
        param_names = std::move(a.params);
        param_regs = std::move(a.registers);
      }
      if (id.postfix && std::any_of(id.postfix->c6502.begin(), id.postfix->c6502.end(),
                                    [](const auto& x) { return x.first == "save_hardware_stack"; })) {
        throw ParserError("`[[c6502::save_hardware_stack]]` goes at the front of the declaration "
                          "of `" +
                          name +
                          "`, where it applies to the calls in its initializers, and not after "
                          "the name");
      }
      const std::optional<Attributes> attrs = merge_attributes(prefix, id.postfix);
      if (const auto* fun = std::get_if<ir::FunType>(&composed)) {
        if (id.init) {
          throw ParserError("function declaration cannot have an initializer");
        }
        if (hw_stack_save) {
          throw ParserError(
              "`[[c6502::save_hardware_stack]]` applies to the calls in a declaration's "
              "initializers, and `" +
              name + "` declares a function");
        }
        const std::string where = "function declaration " + repr(name);
        const std::optional<AttrList> attr_specs = c6502_attributes(attrs, "function", where);
        const FunctionAttributes fa = extract_function_attributes(attr_specs, where);
        const bool noreturn = is_noreturn(attrs);
        if (has_auto(specs)) {
          throw ParserError("`auto` can't go on " + where +
                            ", since a function has no automatic storage and c6502 doesn't "
                            "infer a function's type");
        }
        if (is_constexpr) {
          throw ParserError(
              "`constexpr` declares a named constant, which is an object, and " + where +
              " declares a function");
        }
        if (split.storage == Storage::kRegister) {
          throw ParserError("storage class 'register' isn't permitted on function declaration " +
                            repr(name));
        }
        // A function declared through a typedef of a function type
        // (`F f;`) has no parameter names.
        if (!param_names) {
          param_names.emplace(fun->params.size(), std::nullopt);
        }
        ir::Type_function_decl fd;
        fd.name = name;
        for (const auto& n : *param_names) {
          fd.params.push_back(n.value_or(""));
        }
        fd.data_type = composed;
        fd.storage_class = storage_class(split.storage);
        fd.abi_annotation = fa.abi_annotation;
        fd.param_registers = param_regs && !param_regs->empty()
                                 ? *param_regs
                                 : std::vector<std::string>(param_names->size(), "");
        fd.carry_return = fa.carry_return;
        fd.address = fa.address;
        fd.preserves = fa.preserves;
        fd.section = extract_section(attr_specs, where);
        fd.noreturn = noreturn;
        out.decls.push_back(ir::FunctionDecl{.function_decl = std::move(fd)});
      } else {
        const std::optional<AttrList> attr_specs =
            c6502_attributes(attrs, "object", "object declaration `" + name + "`");
        if (is_constexpr) {
          if (!id.init) {
            throw ParserError("`constexpr` object `" + name +
                              "` has no initializer, and a constexpr declaration must have one "
                              "(C23 §6.7.2p5)");
          }
          if (!id.inferred) {
            composed = p_.constexpr_type(composed);
          }
        }
        ir::Type_var_decl vd;
        vd.name = name;
        vd.init = id.init;
        vd.data_type = composed;
        vd.storage_class = storage_class(split.storage);
        vd.is_constexpr = is_constexpr;
        vd.placement = extract_object_placement(attr_specs, name);
        vd.section = extract_section(attr_specs, "`" + name + "`");
        out.decls.push_back(ir::VarDecl{.var_decl = std::move(vd)});
      }
    }
    out.hw_stack_save = hw_stack_save;
    return out;
  });
}

Type Declarations::type_name() {
  const std::vector<Spec> leaves = specifier_list(false, std::nullopt, std::nullopt);
  std::optional<Declarator> adecl;
  if (p_.at(t_.STAR) || p_.at(t_.LPAREN) || p_.at(t_.LBRACKET)) {
    adecl = declarator(Mode::kAbstract);
  }
  return p_.action("type_name", [&] {
    // The type of a cast, `sizeof (T)`, `typeof (T)`, or a compound
    // literal. A struct or union definition in one is legal C99 but
    // obscure, since the tag escapes to the scope around it, and a cast
    // or `sizeof` has no place to put the StructDecl, so c6502 rejects
    // it.
    const auto [type_specs, qualifiers] = split_specifier_qualifiers(leaves);
    for (const Spec& s : type_specs) {
      if (s.kind == Spec::Kind::kTag && s.body) {
        throw ParserError("struct/union definition isn't permitted inside a type-name (cast / sizeof)");
      }
    }
    const Type base = apply_qualifiers(resolve_data_type(type_specs), qualifiers);
    if (!adecl) {
      return base;
    }
    return apply_abstract_declarator(p_, *adecl, base);
  });
}

// --- Attributes ---

std::optional<AttributeArg> attribute_argument(Descent& p, const std::string& spelled,
                                               const std::optional<std::vector<lark::Token>>& args) {
  // None without parentheses, an int for one integer constant, a str for
  // one string literal, and an error for anything else, which none of
  // the attributes take.
  if (!args) {
    return AttributeArg{};
  }
  const Terminals& t = p.t;
  const std::vector<lark::Token>& tokens = *args;
  if (tokens.size() == 1 &&
      (tokens[0].type == t.INTEGER_CONSTANT || tokens[0].type == t.UINT_INTEGER ||
       tokens[0].type == t.LONG_INTEGER || tokens[0].type == t.ULONG_INTEGER)) {
    AttributeArg arg;
    arg.kind = AttributeArg::Kind::kInt;
    arg.integer = p.parse_integer_token(tokens[0].value).constant;
    return arg;
  }
  if (tokens.size() == 1 && tokens[0].type == t.STRING_LITERAL) {
    std::string lit = tokens[0].value;
    // The `L"..."` wide-char prefix isn't meaningful for an attribute
    // argument, so it's dropped.
    if (lit.starts_with("L")) {
      lit = lit.substr(1);
    }
    AttributeArg arg;
    arg.kind = AttributeArg::Kind::kString;
    arg.string = lit.substr(1, lit.size() - 2);
    return arg;
  }
  if (!tokens.empty()) {
    throw ParserError("the argument of `" + spelled +
                      "` has to be one integer constant or one string literal");
  }
  throw ParserError("`" + spelled + "()` has empty parentheses; leave them off");
}

}  // namespace

// --- The entry points ---

Attributes Descent::parse_attribute_specifier_seq() {
  std::vector<Attributes> specifiers;
  do {
    // `ATTR_OPEN attribute? (COMMA attribute?)* RBRACKET RBRACKET`. Each
    // attribute is `(kind, name, arg)`, with kind "c6502" or
    // "standard", or none for one c6502 ignores.
    expect(t.ATTR_OPEN);
    struct Item {
      bool standard;
      std::string name;
      AttributeArg arg;
    };
    std::vector<Item> items;
    while (true) {
      if (at(t.IDENTIFIER)) {
        const lark::Token first = take();
        std::optional<lark::Token> second;
        if (at(t.COLONCOLON)) {
          take();
          second = expect(t.IDENTIFIER);
        }
        std::optional<std::vector<lark::Token>> args;
        if (at(t.LPAREN)) {
          // `LPAREN _balanced_token* RPAREN`: the tokens between.
          take();
          args.emplace();
          int depth = 0;
          while (!(depth == 0 && at(t.RPAREN))) {
            const int k = kind();
            if (k == t.LPAREN || k == t.LBRACKET) {
              ++depth;
            } else if (k == t.ATTR_OPEN) {
              depth += 2;
            } else if (k == t.RPAREN || k == t.RBRACKET) {
              --depth;
            } else if (stream().at_end()) {
              unexpected("an attribute's arguments");
            }
            args->push_back(take());
          }
          take();
          action("attribute_args", [] {});
        }
        if (!second) {
          std::optional<Item> item = action("attribute_plain", [&]() -> std::optional<Item> {
            // A standard attribute, or one C23 reserves, which c6502
            // ignores. A name that's one of c6502's own needs the prefix.
            const std::string name = normalized_attribute_name(first.value);
            static const std::vector<std::pair<std::string_view, bool>> kStandard = {
                {"deprecated", true},   {"maybe_unused", false}, {"nodiscard", true},
                {"noreturn", false},    {"_Noreturn", false},    {"fallthrough", false},
                {"unsequenced", false}, {"reproducible", false},
            };
            for (const auto& [std_name, takes_string] : kStandard) {
              if (std_name != name) {
                continue;
              }
              AttributeArg arg = *attribute_argument(*this, name, args);
              if (!arg.is_none() && !(takes_string && arg.kind == AttributeArg::Kind::kString)) {
                if (takes_string) {
                  throw ParserError("`" + name + "` takes a string literal, such as `[[" + name +
                                    "(\"reason\")]]`, or nothing");
                }
                throw ParserError("`" + name + "` doesn't take an argument");
              }
              return Item{true, name, std::move(arg)};
            }
            if (is_c6502_attribute(name)) {
              throw ParserError("`" + name +
                                "` is a c6502 attribute, so it needs c6502's prefix: `[[c6502::" +
                                name + "]]`");
            }
            return std::nullopt;
          });
          if (item) {
            items.push_back(std::move(*item));
          }
        } else {
          std::optional<Item> item = action("attribute_prefixed", [&]() -> std::optional<Item> {
            // c6502's own with the prefix `c6502`, and any other
            // prefix's, which c6502 ignores.
            const std::string prefix = normalized_attribute_name(first.value);
            const std::string name = normalized_attribute_name(second->value);
            if (prefix != "c6502") {
              return std::nullopt;
            }
            if (name == "packed") {
              throw ParserError(
                  "`[[c6502::packed]]` was removed. C23's fixed underlying type gives an enum its "
                  "size: `enum e : uint8_t { ... }` is one byte (C23 §6.7.3.3)");
            }
            if (!is_c6502_attribute(name)) {
              std::string names;
              for (const std::string_view n : c6502_attribute_names()) {
                names += (names.empty() ? "" : ", ") + std::string(n);
              }
              throw ParserError("unknown attribute `c6502::" + name +
                                "`; c6502's attributes are " + names +
                                ", and the parameter registers `in_a`, `in_x`, `in_y`, `in_ay`, "
                                "and so on");
            }
            return Item{false, name, *attribute_argument(*this, "c6502::" + name, args)};
          });
          if (item) {
            items.push_back(std::move(*item));
          }
        }
      }
      if (at(t.COMMA)) {
        take();
        continue;
      }
      break;
    }
    expect(t.RBRACKET);
    expect(t.RBRACKET);
    specifiers.push_back(action("attribute_specifier", [&] {
      // C23 lets a standard attribute appear only once in an attribute
      // list.
      Attributes attrs;
      for (Item& item : items) {
        if (item.standard) {
          for (const auto& [n, a] : attrs.standard) {
            if (n == item.name) {
              throw ParserError("`" + item.name + "` appears twice in one attribute list");
            }
          }
          attrs.standard.emplace_back(item.name, std::move(item.arg));
        } else {
          attrs.c6502.emplace_back(item.name, std::move(item.arg));
        }
      }
      return attrs;
    }));
  } while (at(t.ATTR_OPEN));
  // One `Attributes` for them all.
  Attributes out;
  for (Attributes& a : specifiers) {
    out.c6502.insert(out.c6502.end(), a.c6502.begin(), a.c6502.end());
    out.standard.insert(out.standard.end(), a.standard.begin(), a.standard.end());
  }
  return out;
}

DeclList Descent::parse_declaration(std::optional<Attributes> prefix) {
  return Declarations(*this).declaration(std::move(prefix));
}

DeclList Descent::parse_var_decl(std::optional<Attributes> prefix) {
  return Declarations(*this).var_decl(std::move(prefix));
}

Type Descent::parse_type_name() { return Declarations(*this).type_name(); }

}  // namespace c6502::parser
