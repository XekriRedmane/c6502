// The port of passes/stdbit_functions.py: the definitions of the
// <stdbit.h> functions a unit refers to by name, which the type checker
// adds before it checks the program.
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "c99_ast.h"

namespace c6502::passes::stdbit_functions {

// Returns the resolved name of the parameter of the definition of
// `name`, `@<name>.value`. The symbol table holds every parameter by its
// resolved name, so each definition needs its own, and the letters after
// the `@` keep it apart from the names identifier_resolution makes,
// which have digits there.
std::string parameter(std::string_view name);

// Appends to `decls`, a program's file-scope declarations, a definition
// of each <stdbit.h> function the program declares with external
// linkage, refers to, and doesn't define, and returns the names it
// defined. Removes from `decls` the declarations of the <stdbit.h>
// functions the program doesn't refer to.
std::vector<std::string> add_definitions(std::vector<c99_ast::Type_declaration>& decls);

}  // namespace c6502::passes::stdbit_functions
