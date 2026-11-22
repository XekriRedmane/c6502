// The port of passes/link_script.py, the subset of GNU ld's script
// language that `compile.py -T script.ld` reads, and of
// `linker.read_script`, which reads a script's file. The module comment
// of link_script.cpp describes the subset.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "asdl/runtime.h"
#include "ordered_map.h"

namespace c6502::passes::link_script {

// An expression of a script. Python has a dataclass for each kind
// (`Num`, `Sym`, `Dot`, `Unary`, `Binary`, and `Call`), and `kind` says
// which one this is. The values are Python's unbounded integers, held
// here in 128 bits, and a number or a result that doesn't fit throws
// `InternalError`, which says that the port's limit was reached.
struct Expr {
  enum class Kind { Num, Sym, Dot, Unary, Binary, Call };
  Kind kind = Kind::Num;
  // `Num.value`.
  asdl::Int value = 0;
  // `Sym.name`.
  std::string name;
  // `Unary.op` (`-` or `~`) and `Binary.op` (`+ - * / & | ^ << >>`).
  std::string op;
  // `Call.fn`: `ALIGN`, `ORIGIN`, `LENGTH`, `ADDR`, `LOADADDR`, or
  // `SIZEOF`.
  std::string fn;
  // `Unary.operand` as the one element, `Binary.left` and `.right` as
  // the two, and `Call.args` in order. The region and section functions
  // take a name, as a `Sym`.
  std::vector<Expr> args;

  bool operator==(const Expr&) const = default;
};

// A `MEMORY` region (Python's `Region`).
struct Region {
  std::string name;
  asdl::Int origin = 0;
  asdl::Int length = 0;

  // Returns one past the region's last byte (Python's `end` property).
  asdl::Int end() const { return origin + length; }
  bool operator==(const Region&) const = default;
};

// `symbol = expr;` (Python's `Assign`). `symbol` is `.` for a location
// assignment. `. += n;` and `sym -= n;` are held as `. = . + n` and
// `sym = sym - n`.
struct Assign {
  std::string symbol;
  Expr expr;

  bool operator==(const Assign&) const = default;
};

// `file(pattern ...)`, or `KEEP(file(pattern ...))` (Python's `Input`).
// `file` is `*` for every input file, or a pattern for the input file's
// name without its directory, such as `zstring.asm`.
struct Input {
  std::vector<std::string> patterns;
  std::string file = "*";

  bool operator==(const Input&) const = default;
};

// An item inside an output section's braces: an `Assign` or an `Input`.
using Content = std::variant<Assign, Input>;

// An output section (Python's `OutputSection` of link_script.py).
// `address` is the expression after the name, `at` that of `AT(...)`,
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

// A parsed script (Python's `Script`): the regions by name, in the
// order `MEMORY` gives them, and the statements of `SECTIONS` in order.
struct Script {
  OrderedMap<std::string, Region> regions;
  std::vector<Statement> statements;

  // Returns the output sections among `statements`, in order (Python's
  // `output_sections()`).
  std::vector<const OutputSection*> output_sections() const;
};

// Parses the linker script `text` (UTF-8, as Python's `str`). Throws
// `CompileError("ScriptError", ...)` with Python's message for anything
// outside the subset, an unknown region, or a section listed twice.
Script parse_script(std::string_view text);

// Returns the name of the encoding Python's `Path.read_text()` reads a
// file in, `locale.getpreferredencoding(False)` of compile.py run
// normally: "utf-8" under Python's UTF-8 mode (`PYTHONUTF8=1`), and
// otherwise the ANSI code page on Windows ("cp1252" here) and the
// locale's codeset elsewhere ("utf-8", which Python also chooses for
// the C and POSIX locales).
std::string locale_encoding();

// Returns the parsed linker script at `path` (Python's
// `linker.read_script`), read in `locale_encoding()` with universal
// newlines. Throws `CompileError("LinkError", ...)` with Python's
// message, `<path>: <reason>`, for a file that can't be read or decoded
// or a script `parse_script` rejects.
Script read_script(const std::string& path);

}  // namespace c6502::passes::link_script
