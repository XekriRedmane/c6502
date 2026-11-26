// The subset of GNU ld's script language that `compile.py -T script.ld`
// reads, and `read_script`, which reads a script's file. The comment at
// the top of link_script.cpp describes the subset.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "asdl/runtime.h"
#include "ordered_map.h"

namespace c6502::passes::link_script {

// An expression of a script, of the kind `kind` gives. The values are
// held in 128 bits, and a number or a result that doesn't fit throws
// `InternalError`.
struct Expr {
  enum class Kind { Num, Sym, Dot, Unary, Binary, Call };
  Kind kind = Kind::Num;
  asdl::Int value = 0;
  std::string name;
  // `-` or `~` for a `Unary`, and `+ - * / & | ^ << >>` for a `Binary`.
  std::string op;
  // `ALIGN`, `ORIGIN`, `LENGTH`, `ADDR`, `LOADADDR`, or `SIZEOF`.
  std::string fn;
  // A `Unary`'s operand as the one element, a `Binary`'s left and right
  // operands as the two, and a `Call`'s arguments in order. The region
  // and section functions take a name, as a `Sym`.
  std::vector<Expr> args;

  bool operator==(const Expr&) const = default;
};

struct Region {
  std::string name;
  asdl::Int origin = 0;
  asdl::Int length = 0;

  asdl::Int end() const { return origin + length; }
  bool operator==(const Region&) const = default;
};

// `symbol = expr;`. `symbol` is `.` for a location assignment. `. += n;` and `sym -= n;` are held as `. = . + n` and
// `sym = sym - n`.
struct Assign {
  std::string symbol;
  Expr expr;

  bool operator==(const Assign&) const = default;
};

// `file(pattern ...)`, or `KEEP(file(pattern ...))`.
// `file` is `*` for every input file, or a pattern for the input file's
// name without its directory, such as `zstring.asm`.
struct Input {
  std::vector<std::string> patterns;
  std::string file = "*";

  bool operator==(const Input&) const = default;
};

// An item inside an output section's braces: an `Assign` or an `Input`.
using Content = std::variant<Assign, Input>;

// An output section. `address` is the expression after the name, `at` that of `AT(...)`,
// and `align` that of `ALIGN(...)`, each absent when the script gives
// none. `reorder` is false for `(NOREORDER)`, which keeps the section's
// functions in the script's order (link_layout's `fall_through`).
// `region` is the region after `>`, and `load_region` the one after
// `AT>`.
struct OutputSection {
  std::string name;
  std::optional<Expr> address;
  bool noload = false;
  bool reorder = true;
  std::optional<Expr> at;
  std::optional<Expr> align;
  std::vector<Content> contents;
  std::optional<std::string> region;
  std::optional<std::string> load_region;

  bool operator==(const OutputSection&) const = default;
};

// A statement of `SECTIONS`: an `Assign` or an `OutputSection`.
using Statement = std::variant<Assign, OutputSection>;

// A parsed script: the regions by name, in the order `MEMORY` gives
// them, and the statements of `SECTIONS` in order.
struct Script {
  OrderedMap<std::string, Region> regions;
  std::vector<Statement> statements;

  std::vector<const OutputSection*> output_sections() const;
};

// Parses the linker script `text`, in UTF-8. Throws
// `CompileError("ScriptError", ...)` for anything outside the subset, an
// unknown region, or a section listed twice.
Script parse_script(std::string_view text);

// Returns the name of the encoding `read_script` reads a file in:
// "utf-8" when `PYTHONUTF8=1`, and otherwise the ANSI code page on
// Windows and the locale's codeset elsewhere ("utf-8" for the C and
// POSIX locales).
std::string locale_encoding();

// Returns the parsed linker script at `path`, read in
// `locale_encoding()` with universal newlines. Throws
// `CompileError("LinkError", ...)` with the message `<path>: <reason>`
// for a file that can't be read or decoded
// or a script `parse_script` rejects.
Script read_script(const std::string& path);

}  // namespace c6502::passes::link_script
