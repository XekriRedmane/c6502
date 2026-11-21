// passes/identifier_resolution.py: the resolution of every identifier
// to a declaration, with each declaration's linkage (C99 §6.2.2).
#pragma once

#include "c99_ast.h"

namespace c6502::passes {

// Resolves every identifier of the program, which it takes by value and
// returns. A name with no linkage (a block-scope object, a parameter, a
// compound literal) becomes `@N.<name>`, and a struct or union tag
// declared in a block becomes `@N.<tag>`, from one counter over the
// program that starts at 0, in the order of the walk. Names with
// internal or external linkage and file-scope tags keep their spelling.
// Throws `CompileError` of type `IdentifierResolutionError` for an
// undeclared name, a duplicate declaration, a conflict of linkage, a
// builtin declared or used as a value, `static` on a block-scope
// function, a storage class on a for-init declaration, an unnamed
// parameter of a definition, and an operand that isn't an lvalue where
// one is required.
c99_ast::Program resolve_identifiers(c99_ast::Program prog);

}  // namespace c6502::passes
