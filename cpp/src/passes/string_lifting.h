#pragma once

#include "c99_ast.h"

namespace c6502::passes {

// Replaces each `String` of the program it takes by value with a `Var`
// naming a new file-scope `static char str@N[len+1]` that the string
// initializes, except a string that initializes a character array
// directly, which stays in place. The new declarations go in front of
// the program's, numbered from 0. A list with designators is first
// rewritten into positional form (`designators::normalize_initializer`),
// which throws `CompileError` of type `TypeCheckError` for a designator
// the type doesn't take.
c99_ast::Program lift_strings(c99_ast::Program prog);

}  // namespace c6502::passes
