"""Writes the C++ port's copy of the parser's tables, which Lark computes
from c99.lark: the LALR automaton that parser.py runs, and the order
and patterns of the terminals its basic lexer tries.

    uv run python tools/lalr_table.py            # writes cpp/generated/c99_lalr.cpp
    uv run python tools/lalr_table.py --check    # exits 1 when the file differs

The C++ parser (docs/cpp_port.md, "The plan for the parser") feeds
each token to a recognizer that runs this table without actions, so
that it stops at the token where Lark stops and lists the terminals
Lark lists. tests/test_lalr_table.py regenerates the file and fails
when it differs from the one checked in, so c99.lark and the table
can't drift apart.

A table that Lark builds numbers its states by iterating sets of
strings, whose order changes with each process's string hashes, so two
builds give the same automaton with its states numbered differently.
The file numbers them canonically instead. The start states come
first, in the order of parser.py's start rules, and then every other
state in the order a breadth-first walk reaches it, taking each
state's shifts and gotos in the order of their symbols' names. The
terminals are numbered in the order of their names by code point, as
`parser._sort_expected` sorts them, so that a sorted list of terminal
numbers is a sorted list of names, and the nonterminals follow them in
the same order. The rules are numbered in the order of their origin,
their expansion, and their alias.

The lexer's part is the list of terminals in the order Lark's basic
lexer tries them (`BasicLexer.terminals` after `_create_unless`), each
with its pattern: the regular expression Lark compiles for a pattern
of type "re", and the string for one of type "str". Lark folds the
keywords, whose strings IDENTIFIER's pattern matches whole, into
IDENTIFIER's callback, and the file lists them as the strings that
retype an IDENTIFIER. lexer.py's `tokenize` runs a lexer built from
the same grammar with other start rules, and the script checks that
the two try the same terminals in the same order.
"""
from __future__ import annotations

import argparse
import sys
from collections import deque
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(_ROOT))

OUTPUT = _ROOT / "cpp" / "generated" / "c99_lalr.cpp"


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


def _scanner_terminals(lark_lexer) -> list:
    """The terminals Lark's basic lexer `lark_lexer` tries, in its order,
    and the map of IDENTIFIER's callback from keyword strings to their
    terminals."""
    scanner = lark_lexer.scanner
    callbacks = {}
    for name, cb in lark_lexer.callback.items():
        callbacks[name] = {t.pattern.value: t.name for t in cb.scanner.terminals}
    return scanner.terminals, callbacks


def generate() -> str:
    """Returns the text of cpp/generated/c99_lalr.cpp."""
    import lark
    from lark.parsers.lalr_analysis import Shift
    import lexer as lexer_module
    import parser as parser_module

    frontend = parser_module._LARK.parser
    table = frontend.parser._parse_table
    lark_lexer = frontend.lexer
    terminals, callbacks = _scanner_terminals(lark_lexer)
    tokenize_lexer = lexer_module._lark().parser.lexer
    other, other_callbacks = _scanner_terminals(tokenize_lexer)
    if ([(t.name, t.pattern.to_regexp()) for t in terminals]
            != [(t.name, t.pattern.to_regexp()) for t in other]
            or callbacks != other_callbacks):
        raise AssertionError("tokenize's lexer and the parser's differ")
    if set(callbacks) != {"IDENTIFIER"}:
        raise AssertionError(f"unexpected lexer callbacks {sorted(callbacks)}")
    if set(lark_lexer.ignore_types) != {"WS"}:
        raise AssertionError(f"unexpected ignored terminals {lark_lexer.ignore_types}")
    for t in terminals:
        if t.name.startswith("__") or t.pattern.flags:
            raise AssertionError(f"terminal {t.name} needs a form the file lacks")

    starts = ["start", "lex_only", "declarator", "type_name"]
    if sorted(table.start_states) != sorted(starts):
        raise AssertionError(f"unexpected start rules {sorted(table.start_states)}")

    # The symbols, terminals first.
    symbols: set[str] = set()
    rules = set()
    for row in table.states.values():
        for sym, (action, arg) in row.items():
            symbols.add(sym)
            if action is not Shift:
                rules.add(arg)
                symbols.add(arg.origin.name)
                symbols.update(s.name for s in arg.expansion)
    symbols.update(t.name for t in terminals if t.name != "WS")
    symbols.update(callbacks["IDENTIFIER"].values())
    term_names = sorted(s for s in symbols if s.isupper())
    nonterm_names = sorted(s for s in symbols if not s.isupper())
    names = term_names + nonterm_names
    if len(names) > 255:
        raise AssertionError("more symbols than a byte numbers")
    sym_id = {name: i for i, name in enumerate(names)}

    # The states, numbered canonically.
    order: list[int] = []
    seen: set[int] = set()
    queue = deque()
    for name in starts:
        s = table.start_states[name]
        if s not in seen:
            seen.add(s)
            order.append(s)
            queue.append(s)
    while queue:
        s = queue.popleft()
        for sym in sorted(table.states[s]):
            action, arg = table.states[s][sym]
            if action is Shift and arg not in seen:
                seen.add(arg)
                order.append(arg)
                queue.append(arg)
    if len(order) != len(table.states):
        raise AssertionError("a state isn't reachable from a start state")
    state_id = {s: i for i, s in enumerate(order)}
    if len(order) >= 0x8000:
        raise AssertionError("too many states for the encoding")

    def rule_key(r):
        return (r.origin.name, tuple(s.name for s in r.expansion), r.alias or "")

    sorted_rules = sorted(rules, key=rule_key)
    keys = [rule_key(r) for r in sorted_rules]
    if len(set(keys)) != len(keys):
        raise AssertionError("two rules have the same origin, expansion, and alias")
    rule_id = {id(r): i for i, r in enumerate(sorted_rules)}

    row_start = [0]
    entry_symbol: list[int] = []
    entry_action: list[int] = []
    for s in order:
        row = table.states[s]
        for sym in sorted(row, key=lambda n: sym_id[n]):
            action, arg = row[sym]
            entry_symbol.append(sym_id[sym])
            if action is Shift:
                entry_action.append(state_id[arg])
            else:
                entry_action.append(0x8000 | rule_id[id(arg)])
        row_start.append(len(entry_symbol))

    rule_origin = [sym_id[r.origin.name] for r in sorted_rules]
    rule_start = [0]
    rule_expansion: list[int] = []
    for r in sorted_rules:
        rule_expansion.extend(sym_id[s.name] for s in r.expansion)
        rule_start.append(len(rule_expansion))
    rule_alias = [r.alias or "" for r in sorted_rules]

    allowed = sorted(lark_lexer.scanner.allowed_types - lark_lexer.ignore_types)
    keywords = sorted(callbacks["IDENTIFIER"].items())

    out: list[str] = []
    out.append("// Generated from c99.lark by tools/lalr_table.py, with Lark "
               f"{lark.__version__}.")
    out.append("// Do not edit. tests/test_lalr_table.py regenerates it and "
               "compares.")
    out.append('#include "lark_table.h"')
    out.append("")
    out.append("namespace c6502::lark_table {")
    out.append("")
    out.append("namespace {")
    out.append("")
    out.append(f"// The {len(names)} symbols: {len(term_names)} terminals, "
               "then the nonterminals.")
    out.append("const char* const kSymbols[] = {")
    out.extend(_wrap([_cpp_string(n) for n in names]))
    out.append("};")
    out.append("")
    out.append("// Each state's row is its entries from kRowStart[state] "
               "to kRowStart[state + 1].")
    out.append("const std::uint32_t kRowStart[] = {")
    out.extend(_wrap([str(x) for x in row_start]))
    out.append("};")
    out.append("")
    out.append("const std::uint8_t kEntrySymbol[] = {")
    out.extend(_wrap([str(x) for x in entry_symbol]))
    out.append("};")
    out.append("")
    out.append("// A shift or goto to a state, or 0x8000 plus a rule to reduce.")
    out.append("const std::uint16_t kEntryAction[] = {")
    out.extend(_wrap([str(x) for x in entry_action]))
    out.append("};")
    out.append("")
    out.append("const std::uint16_t kRuleOrigin[] = {")
    out.extend(_wrap([str(x) for x in rule_origin]))
    out.append("};")
    out.append("")
    out.append("const std::uint16_t kRuleStart[] = {")
    out.extend(_wrap([str(x) for x in rule_start]))
    out.append("};")
    out.append("")
    out.append("const std::uint8_t kRuleExpansion[] = {")
    out.extend(_wrap([str(x) for x in rule_expansion]))
    out.append("};")
    out.append("")
    out.append("const char* const kRuleAlias[] = {")
    out.extend(_wrap([_cpp_string(a) for a in rule_alias]))
    out.append("};")
    out.append("")
    out.append("const StartRule kStarts[] = {")
    for name in starts:
        out.append(f"    {{{_cpp_string(name)}, {state_id[table.start_states[name]]}, "
                   f"{state_id[table.end_states[name]]}}},")
    out.append("};")
    out.append("")
    out.append("// The terminals the lexer tries, in Lark's order.")
    out.append("const LexTerminal kLexTerminals[] = {")
    for t in terminals:
        is_re = type(t.pattern).__name__ == "PatternRE"
        text = t.pattern.to_regexp() if is_re else t.pattern.value
        sym = sym_id[t.name] if t.name in sym_id else -1
        newline = "true" if t.name in lark_lexer.newline_types else "false"
        out.append(f"    {{{_cpp_string(t.name)}, {sym}, {'true' if is_re else 'false'}, "
                   f"{newline},")
        out.append(f"     {_cpp_string(text)}}},")
    out.append("};")
    out.append("")
    out.append("// The keywords, which retype an IDENTIFIER that spells one.")
    out.append("const Keyword kKeywords[] = {")
    for spelling, name in keywords:
        out.append(f"    {{{_cpp_string(spelling)}, {sym_id[name]}}},")
    out.append("};")
    out.append("")
    out.append("// The terminals an UnexpectedCharacters lists, sorted.")
    out.append("const char* const kAllowed[] = {")
    out.extend(_wrap([_cpp_string(n) for n in allowed]))
    out.append("};")
    out.append("")
    out.append("}  // namespace")
    out.append("")
    out.append("const Table& c99_table() {")
    out.append("  static const Table table{")
    out.append(f"      .terminal_count = {len(term_names)},")
    out.append("      .symbols = kSymbols,")
    out.append("      .row_start = kRowStart,")
    out.append("      .entry_symbol = kEntrySymbol,")
    out.append("      .entry_action = kEntryAction,")
    out.append("      .rule_origin = kRuleOrigin,")
    out.append("      .rule_start = kRuleStart,")
    out.append("      .rule_expansion = kRuleExpansion,")
    out.append("      .rule_alias = kRuleAlias,")
    out.append("      .starts = kStarts,")
    out.append("      .lex_terminals = kLexTerminals,")
    out.append("      .keywords = kKeywords,")
    out.append("      .allowed = kAllowed,")
    out.append("  };")
    out.append("  return table;")
    out.append("}")
    out.append("")
    out.append("}  // namespace c6502::lark_table")
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
            print(f"{OUTPUT} differs from what c99.lark gives; run "
                  "tools/lalr_table.py", file=sys.stderr)
            return 1
        return 0
    with open(OUTPUT, "wb") as f:
        f.write(text.encode("utf-8"))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
