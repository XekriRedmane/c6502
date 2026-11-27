// The ASDL parser and the generator of the C++ IR classes in
// cpp/generated/. The build runs the generator (asdlgen_main.cpp) on
// the four .asdl files at the root, and it uses none of the generated
// code, so it can be built before them.
//
// The parser takes the subset of the Zephyr ASDL grammar that the .asdl
// files use:
//
//     module Name { <type-definitions> }
//     type = Constructor(field, ...) | Constructor | ...  [attributes (field, ...)]
//     type = (field, ...)                                  -- product type
//     field = type [? | *] [name]
//     comments start with --
//
// asdl.py parses the same grammar for the Python IR modules.
#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace c6502::asdlgen {

struct Field {
  std::string type;
  std::optional<std::string> name;
  bool optional = false;
  bool sequence = false;
};

struct Constructor {
  std::string name;
  std::vector<Field> fields;
};

struct Type {
  std::string name;
  bool is_product = false;
  // A product's fields, or a sum's attributes (empty without an
  // `attributes` clause).
  std::vector<Field> fields;
  // Empty for a product.
  std::vector<Constructor> constructors;
};

struct Module {
  std::string name;
  std::vector<Type> types;
};

// A malformed .asdl file, or a field type the C++ generator has no type
// for. A parse error's message starts with `line:col: `.
class Error : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

// Parses an .asdl file's text. Its line ends may be CR LF, as c99.asdl's
// and tac.asdl's are.
Module parse(std::string_view text);

// Returns the C++ module and namespace name for an .asdl file, which is
// the Python module's: `asm.asdl` gives `asm_ast`.
std::string cpp_module_name(std::string_view path);

// Returns the C++ files for `mod` as (file name, text) pairs: `<name>.h`
// with the types, their equality, and the declarations of the dump
// format's functions, of Python's `repr`, and of pretty.py's text, and
// then `<name>.cpp` with the dump format's readers and writers and the
// `repr` and the pretty text of each value. `source` is the .asdl
// file's name, for the first line's comment. The text's line ends are
// LF. Throws `Error` for a field of `bytes`, `float`, or `constant`.
std::vector<std::pair<std::string, std::string>> generate_cpp(const Module& mod,
                                                               std::string_view source,
                                                               std::string_view name);

}  // namespace c6502::asdlgen
