// The parts of passes/replace_pseudoregisters.py ported so far: the
// frame dimensions it returns for each function, and `sizeof` and
// `size_of_name`, which most of the code generation passes read.
#pragma once

#include <string_view>
#include <vector>

#include "asdl/runtime.h"
#include "c99_ast.h"
#include "passes/type_checking.h"

namespace c6502::passes {

// Returns the bytes a value of type `t` occupies: 1 for the character
// types, 2 for `int`, `unsigned int`, and pointers (the 6502's address
// width), 4 for the `long`s and `float`, 8 for the `long long`s and
// `double`, ceil(N/8) for `_BitInt(N)`, the element's size times the
// count for an array, and the layout's size for a struct or union.
// `Const` and `Volatile` are transparent. A struct or union with no
// type table (`types` null, Python's None), or one whose layout is
// missing or incomplete, counts as 1 byte, and so does any other type.
// Python names it `sizeof`, which is a C++ keyword.
asdl::Int sizeof_type(const c99_ast::Type_data_type& t, const TypeTable* types);

// Returns the bytes the pseudo `name` occupies, `sizeof_type` of its
// symbol's type, or 1 when `symbols` is null or has no entry for it,
// which is the default the unit tests of synthetic programs rely on.
asdl::Int size_of_name(std::string_view name, const SymbolTable* symbols, const TypeTable* types);

// A function's frame, as `replace_program_bare_exit` computes it for
// `prologue_synthesis` (Python's `FrameDims`): the bytes of its
// arguments and of its locals in the frame, and the callee-saved
// zero-page bytes it saves, which are the payload of
// `FunctionPrologue` and `Ret`.
struct FrameDims {
  asdl::Int arg_bytes = 0;
  asdl::Int local_bytes = 0;
  std::vector<asdl::Int> callee_saved_addrs;
  bool operator==(const FrameDims&) const = default;
};

}  // namespace c6502::passes
