// The port of passes/designators.py, which rewrites a braced
// initializer with designators (`[2] = v`, `.x = v`, `.a[2].b = v`, C23
// §6.7.11) into the positional form every later pass handles: one item
// per subobject in order, `{}` for each subobject nothing initializes,
// and for a union one item with `InitList.union_member` naming the
// member. string_lifting calls it for every declaration and compound
// literal, and the type checker for the lists the parser checks while
// folding a `constexpr` object.
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "c99_ast.h"

namespace c6502::passes::designators {

// The members of a struct or union as `(name, type)` pairs, in
// declaration order.
using Members = std::vector<std::pair<std::string, c99_ast::Type_data_type>>;

// Returns the members of the struct or union `tag`, or none when the tag
// has no body (Python's `LayoutOf`).
using LayoutOf = std::function<std::optional<Members>(std::string_view tag)>;

// Returns whether the braced list `init`, or a list nested in it, has a
// designation.
bool has_designation(const c99_ast::Type_exp& init);

// Returns the initializer `init` of an object of type `t` with every
// designation rewritten into positional form. A list with no
// designation in it comes back unchanged, as does an initializer that
// isn't a list. `what` names the object in messages as the program
// spells it, and is none for a compound literal, which the messages
// call `(T){...}`. Throws TypeCheckError for a designator that doesn't
// fit the type it applies to, an array index past the end, a positional
// item that would need brace elision, and an overridden initializer
// with side effects.
c99_ast::Type_exp normalize_initializer(c99_ast::Type_exp init, const c99_ast::Type_data_type& t,
                                        const LayoutOf& layout_of,
                                        const std::optional<std::string>& what);

}  // namespace c6502::passes::designators
