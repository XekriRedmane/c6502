"""Writes the C++ port's copy of the runtime helpers of rtlib/helpers.py:
each helper's `asm_ast` function in the dump format, in the order of
`all_helper_functions`, and the names in `DECIMAL_MODE_HELPERS`.

    uv run python tools/rtlib_table.py            # writes cpp/generated/rtlib_helpers.cpp
    uv run python tools/rtlib_table.py --check    # exits 1 when the file differs

The helpers' code stays in rtlib/helpers.py, where the simulator reads
it, until the simulator is ported (step 9 of docs/cpp_port.md). The C++
runtime library (src/passes/runtime_library.h) and the HARGS footprints
(`passes::helper_footprints`) read this copy, which is compiled into
the program, so that a compile through c6502cpp needs nothing from
Python to know them. tests/test_rtlib_table.py regenerates the file and
fails when it differs from the one checked in, so the two can't drift
apart.

The call edges between the helpers aren't in the file, since the C++
side finds them in the functions' `Call` and `Jump` instructions, as
passes/runtime_library.py and passes/runtime_layout.py do. Python's
`helper_footprints` follows the calls in the order of a set of strings,
which changes with the process's string hashes, and the order could
change a footprint only if the helpers called each other in a cycle, so
the script refuses a set of helpers with one.
"""
from __future__ import annotations

import argparse
import sys
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(_ROOT))

OUTPUT = _ROOT / "cpp" / "generated" / "rtlib_helpers.cpp"


def _cpp_string(text: str) -> str:
    """The C++ string literal of `text`, which holds only ASCII. A
    control character is written as a three-digit octal escape, and a
    digit after one starts a literal of its own, which MSVC would warn
    about otherwise (C4125)."""
    out = ['"']
    for i, ch in enumerate(text):
        code = ord(ch)
        if code > 0x7E:
            raise ValueError(f"non-ASCII character in {text!r}")
        if ch.isdigit() and i > 0 and (ord(text[i - 1]) < 0x20 or text[i - 1] == "\x7f"):
            out.append('""')
        if ch in '"\\':
            out.append("\\" + ch)
        elif code < 0x20 or code == 0x7F:
            out.append(f"\\{code:03o}")
        else:
            out.append(ch)
    out.append('"')
    return "".join(out)


def _wrap(items: list[str], indent: str = "    ", width: int = 79) -> list[str]:
    """Lines holding `items` separated by commas, each line at most
    `width` columns."""
    lines: list[str] = []
    line = indent
    for item in items:
        piece = item + ","
        if len(line) + len(piece) + (line != indent) > width and line != indent:
            lines.append(line)
            line = indent
        line += ("" if line == indent else " ") + piece
    if line != indent:
        lines.append(line)
    return lines


def _check_acyclic(fns) -> None:
    """Raises ValueError when a helper reaches itself through the
    helpers it calls or jumps to."""
    import asm_ast
    names = {fn.name for fn in fns}
    edges = {
        fn.name: sorted(
            {i.name for i in fn.instructions
             if isinstance(i, asm_ast.Call) and i.name in names}
            | {i.target for i in fn.instructions
               if isinstance(i, asm_ast.Jump) and i.target in names}
        )
        for fn in fns
    }
    done: set[str] = set()

    def visit(name: str, path: list[str]) -> None:
        if name in path:
            cycle = " -> ".join(path[path.index(name):] + [name])
            raise ValueError(f"the runtime helpers call each other in a cycle: {cycle}")
        if name in done:
            return
        for callee in edges[name]:
            visit(callee, path + [name])
        done.add(name)

    for fn in fns:
        visit(fn.name, [])


def generate() -> str:
    """Returns the text of cpp/generated/rtlib_helpers.cpp."""
    import asm_ast
    import ir_dump
    from rtlib.helpers import DECIMAL_MODE_HELPERS, all_helper_functions
    fns = all_helper_functions()
    _check_acyclic(fns)
    dump = ir_dump.dump(asm_ast.Program(top_level=fns))
    lines = dump.split("\n")
    if lines[-1] == "":
        lines.pop()
    out = [
        "// Generated from rtlib/helpers.py by tools/rtlib_table.py.",
        "// Do not edit. tests/test_rtlib_table.py regenerates it and compares.",
        '#include "rtlib/helpers.h"',
        "",
        "#include <iterator>",
        "#include <string>",
        "#include <utility>",
        "#include <variant>",
        "",
        '#include "asdl/runtime.h"',
        "",
        "namespace c6502::rtlib {",
        "",
        "namespace {",
        "",
        f"// The dump of the program of the {len(fns)} helpers, one line of it",
        "// to a string.",
        "const char* const kDump[] = {",
    ]
    out.extend(f"    {_cpp_string(line)}," for line in lines)
    out.append("};")
    out.append("")
    out.append("// `DECIMAL_MODE_HELPERS`, a frozenset, sorted.")
    out.append("const char* const kDecimalModeHelpers[] = {")
    out.extend(_wrap([_cpp_string(n) for n in sorted(DECIMAL_MODE_HELPERS)]))
    out.append("};")
    out.append("")
    out.append("}  // namespace")
    out.append("")
    out.append("std::string helper_dump() {")
    out.append("  std::string text;")
    out.append("  for (const char* line : kDump) {")
    out.append("    text += line;")
    out.append("    text += '\\n';")
    out.append("  }")
    out.append("  return text;")
    out.append("}")
    out.append("")
    out.append("const std::vector<asm_ast::Function>& all_helper_functions() {")
    out.append("  static const std::vector<asm_ast::Function> functions = [] {")
    out.append("    std::vector<asm_ast::Function> out;")
    out.append("    for (auto& tl : asdl::load<asm_ast::Program>(helper_dump()).top_level) {")
    out.append("      out.push_back(std::move(std::get<asm_ast::Function>(tl)));")
    out.append("    }")
    out.append("    return out;")
    out.append("  }();")
    out.append("  return functions;")
    out.append("}")
    out.append("")
    out.append("const std::vector<std::string>& decimal_mode_helpers() {")
    out.append("  static const std::vector<std::string> names(std::begin(kDecimalModeHelpers),")
    out.append("                                              std::end(kDecimalModeHelpers));")
    out.append("  return names;")
    out.append("}")
    out.append("")
    out.append("}  // namespace c6502::rtlib")
    return "\n".join(out) + "\n"


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--check", action="store_true",
                    help="compare with the file instead of writing it")
    ns = ap.parse_args(argv)
    text = generate()
    if ns.check:
        current = OUTPUT.read_bytes().decode("utf-8") if OUTPUT.exists() else ""
        if current != text:
            print(f"{OUTPUT} differs from what rtlib/helpers.py gives; run "
                  "tools/rtlib_table.py", file=sys.stderr)
            return 1
        return 0
    with open(OUTPUT, "wb") as f:
        f.write(text.encode("utf-8"))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
