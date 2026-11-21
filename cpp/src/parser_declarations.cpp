// Part R2 of the parser's descent (parser_descent.h): declarations,
// declarators, specifiers, typedef names, struct, union, and enum,
// attributes, `auto`, `typeof`, and `constexpr` (parser.py:525-1678,
// 1991-2847, and the methods of 2849-3350, 3506-3606, and 3717-4169).
#include <string>

#include "parser_descent.h"

namespace c6502::parser {

namespace {

[[noreturn]] void not_ported(const char* what) {
  throw InternalError(std::string("parser: ") + what + " isn't ported yet (part R2)");
}

}  // namespace

Attributes Descent::parse_attribute_specifier_seq() { not_ported("attribute_specifier_seq"); }

DeclList Descent::parse_declaration(std::optional<Attributes>) { not_ported("declaration"); }

DeclList Descent::parse_var_decl(std::optional<Attributes>) { not_ported("var_decl"); }

Type Descent::parse_type_name() { not_ported("type_name"); }

std::pair<std::optional<c99_ast::Type_storage_class>, bool> Descent::literal_storage(
    const std::vector<lark::Token>&) {
  not_ported("_literal_storage");
}

Type Descent::constexpr_type(const Type&) { not_ported("_constexpr_type"); }

Exp Descent::enum_constant(const asdl::Int&, const std::optional<Type>&) {
  not_ported("_enum_constant");
}

void Descent::apply_statement_attributes(const Attributes&, c99_ast::Type_statement&) {
  not_ported("_apply_statement_attributes");
}

void Descent::check_label_attributes(const Attributes&) { not_ported("_check_label_attributes"); }

}  // namespace c6502::parser
