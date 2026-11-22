"""C preprocessor wrapper around pcpp, mirroring pcpp/pcmd.py.

`preprocess(source, argv)` accepts the same flags as the `pcpp` command
line tool (pcpp/pcmd.py), with these differences:

  - `-o` is omitted: output is always returned as a string. compile.py
    has its own `-o` for the final pipeline output.
  - the `inputs` positional is omitted: source is passed as a string.
  - `--line-directive` defaults to suppressing `#line` markers, since
    the downstream lexer does not handle them. pcpp's default is `#line`.

All other flags (-D, -U, -N, -I, --passthru-*, --debug, --time,
--filetimes, --compress, --assume-input-encoding, --output-encoding,
--write-bom, --disable-auto-pragma-once) behave the same as in pcmd.py.

Like pcmd.py, the `__PCPP_VERSION__`, `__PCPP_ALWAYS_FALSE__`, and
`__PCPP_ALWAYS_TRUE__` macros are predefined.

Unknown flags are ignored with a NOTE on stderr (also matches pcmd.py).

Several things follow C23 rather than pcpp. In the expression of an
`#if` or `#elif`, the identifier `true` that is left after macro
replacement becomes 1 and `false` becomes 0, whatever the passthru
flags (C23 §6.10.2p13). Each `_Bool` in the output gets a warning
(`ALT_BOOL_WARNING`, through `diagnostics.warn`) that names its line,
since C23 makes `_Bool` an obsolescent spelling of `bool`.

pcpp's lexer reads a number as a run of digits with an optional
suffix, so it would split `1'000` into `1` and the start of a
character constant `'000...`, which can swallow the rest of the line
and hide its macros from expansion. `_PP_LEXER` replaces pcpp's number
rule with C23's preprocessing number (§6.4.8,
`pp_numbers.PP_NUMBER_RE`), which takes a `'` followed by a digit or a
letter, so `1'000` and `0b1010` are single tokens. A floating constant
is then one of those tokens too, as the standard has it. In an `#if`,
`_NumberEvaluator` reads each number with `pp_numbers.parse_integer`
before pcpp's evaluator sees it. That gives a binary constant its
value, ignores digit separators, accepts the `ll` suffixes, and makes
a hexadecimal, octal, or binary constant above `INTMAX_MAX` unsigned,
since its type list ends in `uintmax_t` (§6.10.2p13, §6.4.4.2p6). A
pp-number there that isn't a valid integer constant raises
`PreprocessorError`.
"""

from __future__ import annotations

import argparse
import codecs
import copy
import io
import os
import re
import sys

from pcpp import parser as _pcpp_rules
from pcpp.parser import lex as _ply_lex
from pcpp.preprocessor import Action, OutputDirective, Preprocessor

import diagnostics
import pp_numbers

PCPP_VERSION = "1.30"

# Bundled standard headers (limits.h, etc.) live alongside this
# module. Added to pcpp's search path AFTER any user `-I` paths
# so user-supplied headers can shadow ours, but `#include
# <limits.h>` still resolves without the user passing `-I`.
_BUNDLED_INCLUDE_DIR = os.path.join(
    os.path.dirname(os.path.abspath(__file__)), "include",
)

# The warning for each `_Bool` the preprocessor writes out. C23 §6.4.1
# lists `_Bool` as an alternative spelling of the keyword `bool`, and
# its footnote 62 makes the alternative spellings obsolescent.
ALT_BOOL_WARNING = (
    "`_Bool` is deprecated: C23 makes it an obsolescent spelling of the "
    "keyword `bool` (C23 §6.4.1). Use `bool` instead."
)

# The values C23 §6.10.2p13 gives the identifiers `true` and `false`
# when they're left in an `#if` expression after macro replacement.
# Every other identifier left there becomes 0, as pcpp already does.
_IF_KEYWORD_VALUES = {"true": 1, "false": 0}

# The largest value of `intmax_t`, a `long long` in c6502. An `#if`
# computes in `intmax_t` and `uintmax_t` (C23 §6.10.2p13).
_INTMAX_MAX = (1 << 63) - 1
_UINTMAX_MAX = (1 << 64) - 1


@_ply_lex.TOKEN(pp_numbers.PP_NUMBER_RE.pattern)
def _t_pp_number(t):
    # PLY tries the rules given as functions before the rules given as
    # strings, so this one comes before pcpp's CPP_FLOAT and CPP_DOT,
    # and `1.5` and `.5` are pp-numbers. The token keeps pcpp's type
    # CPP_INTEGER, which the rest of pcpp looks for.
    return t


class _PPLexerRules:
    """pcpp's lexer rules (pcpp/parser.py), with C23's pp-number in
    place of pcpp's integer rule. PLY reads the rules from the
    attributes of this class."""


for _name in dir(_pcpp_rules):
    if _name.startswith("t_") or _name in ("tokens", "literals"):
        setattr(_PPLexerRules, _name, getattr(_pcpp_rules, _name))
_PPLexerRules.t_CPP_INTEGER = staticmethod(_t_pp_number)

# Built once. Each `CompilePreprocessor` gets a clone, since a PLY
# lexer holds its position in its input. An empty `lextab` keeps PLY
# from reading a table module and from writing `lextab.py` into the
# current directory, which it does whenever `optimize` is false.
_PP_LEXER = _ply_lex.lex(
    module=_PPLexerRules, optimize=True, lextab="",
    errorlog=_ply_lex.NullLogger(),
)


def _if_integer(spelling: str) -> str:
    """The spelling pcpp's evaluator reads for the pp-number
    `spelling` in an `#if`, which is its value in decimal, with a `U`
    when it's unsigned. Raises pp_numbers.NumberError when `spelling`
    isn't an integer constant that fits in `uintmax_t`.

    A `wb` or `uwb` constant is a `_BitInt` or `unsigned _BitInt`, and
    in `#if` every signed integer type acts as `intmax_t` and every
    unsigned one as `uintmax_t` (C23 §6.10.2p13), so `wb` is signed
    whatever its value and `uwb` unsigned. With BITINT_MAXWIDTH at 64,
    every `wb` value that has a type fits in `intmax_t`."""
    if pp_numbers.is_floating(spelling):
        pp_numbers.check_floating(spelling)
        raise pp_numbers.NumberError(
            f"an #if expression can't contain the floating constant "
            f"{spelling!r}, since it's an integer constant expression "
            f"(C23 §6.10.2p3)"
        )
    constant = pp_numbers.parse_integer(spelling)
    if constant.bit_precise:
        pp_numbers.bit_precise_width(constant, spelling)
        return f"{constant.value}{'U' if constant.unsigned else ''}"
    if constant.value > _UINTMAX_MAX:
        raise pp_numbers.NumberError(
            f"integer constant {spelling!r} doesn't fit in uintmax_t"
        )
    if (constant.value > _INTMAX_MAX and constant.base == 10
            and not constant.unsigned):
        raise pp_numbers.NumberError(
            f"decimal constant {spelling!r} doesn't fit in intmax_t "
            f"(a `u` suffix makes it unsigned)"
        )
    unsigned = constant.unsigned or constant.value > _INTMAX_MAX
    return f"{constant.value}{'U' if unsigned else ''}"


# pcpp's evaluator expands the escapes of a character constant with
# Python's `unicode-escape` codec, at each match of its
# `_expand_escape_sequences_pat`, and one of that pattern's
# alternatives is Python's `\N{name}`. C has no such escape (C23
# §6.4.4.4), and c6502's lexer rejects `'\N{LATIN SMALL LETTER A}'`
# outside an `#if` as it rejects `'\q'`. This is pcpp's pattern without
# that alternative, with the others in the same order.
_PCPP_ESCAPES_BUT_NAMES_RE = re.compile(r"""
    ( \\U........      # 8-digit hex escapes
    | \\u....          # 4-digit hex escapes
    | \\x..            # 2-digit hex escapes
    | \\[0-7]{1,3}     # Octal escapes
    | \\[\\'"abfnrtv]  # Single-character escapes
)""", re.UNICODE | re.VERBOSE)


def _without_named_escapes(spelling: str) -> str:
    """Returns a spelling of the character constant `spelling` that
    pcpp's evaluator reads as it would read `spelling` if its pattern
    had no `\\N{name}` alternative, so that `'\\N{LATIN SMALL LETTER
    A}'` holds 24 characters and pcpp reports the TypeError of `ord()`
    for it, as it does for `'\\q'`.

    The escapes are expanded here with pcpp's own codec, and each
    backslash of the result is written as `\\\\`, which pcpp's pattern
    takes as one escape for a backslash. Doubling only the backslash
    of each `\\N` wouldn't do, since `\\u0\\N` would then give `\\u`
    the four characters its alternative takes. When an escape fails to
    decode, the result is the expansion before it followed by the
    original text from that escape on, so pcpp's pattern finds the
    same escape there and raises the same UnicodeDecodeError."""
    start = 2 if spelling.startswith("L'") else 1
    inner = spelling[start:-1]
    out = []
    pos = 0
    for m in _PCPP_ESCAPES_BUT_NAMES_RE.finditer(inner):
        out.append(inner[pos:m.start()])
        try:
            out.append(codecs.decode(m.group(0), "unicode-escape"))
        except UnicodeDecodeError:
            rest = inner[m.start():]
            break
        pos = m.end()
    else:
        out.append(inner[pos:])
        rest = ""
    expanded = "".join(out).replace("\\", "\\\\")
    return spelling[:start] + expanded + rest + spelling[-1]


class _NumberEvaluator:
    """Wraps pcpp's `#if` evaluator, and rewrites each number token
    into the spelling `_if_integer` gives before pcpp's evaluator
    reads it. pcpp catches and prints whatever its evaluator raises,
    so the message for an invalid number goes in the preprocessor's
    `number_error`, which `CompilePreprocessor.evalexpr` raises after
    pcpp returns. A character constant is rewritten by
    `_without_named_escapes`, so that pcpp doesn't expand Python's
    `\\N{name}` escape in it."""

    def __init__(self, evaluator, preprocessor) -> None:
        self.evaluator = evaluator
        self.preprocessor = preprocessor

    def __call__(self, tokens, functions={}, identifiers={}):
        rewritten = []
        for tok in tokens:
            if (tok.type == self.preprocessor.t_INTEGER
                    and isinstance(tok.value, str)):
                try:
                    value = _if_integer(tok.value)
                except pp_numbers.NumberError as e:
                    self.preprocessor.number_error = (
                        f"{self.preprocessor._where(tok)}: {e}"
                    )
                    raise
                tok = copy.copy(tok)
                tok.value = value
            elif (tok.type == self.preprocessor.t_CHAR
                    and "\\N" in tok.value):
                tok = copy.copy(tok)
                tok.value = _without_named_escapes(tok.value)
            rewritten.append(tok)
        return self.evaluator(
            rewritten, functions=functions, identifiers=identifiers,
        )


def _build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(prog="preprocess", add_help=False)
    p.add_argument("-D", dest="defines", metavar="macro[=val]",
                   nargs=1, action="append",
                   help="Predefine name as a macro [with value]")
    p.add_argument("-U", dest="undefines", metavar="macro",
                   nargs=1, action="append",
                   help="Pre-undefine name as a macro")
    p.add_argument("-N", dest="nevers", metavar="macro",
                   nargs=1, action="append",
                   help="Never define name as a macro, even if defined during preprocessing")
    p.add_argument("-I", dest="includes", metavar="path",
                   nargs=1, action="append",
                   help="Path to search for unfound #include's")
    p.add_argument("--passthru-defines", dest="passthru_defines",
                   action="store_true",
                   help="Pass through but still execute #defines and #undefs")
    p.add_argument("--passthru-unfound-includes",
                   dest="passthru_unfound_includes", action="store_true",
                   help="Pass through #includes not found without execution")
    p.add_argument("--passthru-unknown-exprs",
                   dest="passthru_undefined_exprs", action="store_true",
                   help="Unknown macros in expressions cause preprocessor logic to be passed through")
    p.add_argument("--passthru-comments", dest="passthru_comments",
                   action="store_true",
                   help="Pass through comments unmodified")
    p.add_argument("--passthru-magic-macros", dest="passthru_magic_macros",
                   action="store_true",
                   help="Pass through double underscore magic macros unmodified")
    p.add_argument("--passthru-includes", dest="passthru_includes",
                   metavar="<regex>", default=None, nargs=1,
                   help="Regex matching #includes that should not be expanded")
    p.add_argument("--disable-auto-pragma-once",
                   dest="auto_pragma_once_disabled", action="store_true",
                   default=False,
                   help="Disable the auto #pragma once heuristics")
    p.add_argument("--line-directive", dest="line_directive", metavar="form",
                   default=None, nargs="?",
                   help="Form of line directive (default here: suppress; "
                        "pcpp default is '#line')")
    p.add_argument("--debug", dest="debug", action="store_true",
                   help="Generate a pcpp_debug.log file logging execution")
    p.add_argument("--time", dest="time", action="store_true",
                   help="Print the time it took to #include each file (to stderr)")
    p.add_argument("--filetimes", dest="filetimes", metavar="path",
                   type=argparse.FileType("wt"), default=None, nargs="?",
                   help="CSV file with time spent inside each included file")
    p.add_argument("--compress", dest="compress", action="store_true",
                   help="Make output as small as possible")
    p.add_argument("--assume-input-encoding", dest="assume_input_encoding",
                   metavar="<encoding>", default=None, nargs=1,
                   help="Text encoding to assume inputs are in")
    p.add_argument("--output-encoding", dest="output_encoding",
                   metavar="<encoding>", default=None, nargs=1,
                   help="Text encoding (recorded but unused for in-memory output)")
    p.add_argument("--write-bom", dest="write_bom", action="store_true",
                   help="Prefix output with a Unicode BOM")
    return p


_PARSER = _build_parser()


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    """Parse pcpp-style flags. Unknown args are ignored (matches pcmd.py)."""
    args, unknown = _PARSER.parse_known_args(argv or [])
    for arg in unknown:
        print(f"NOTE: Argument {arg} not known, ignoring!", file=sys.stderr)
    return args


class CompilePreprocessor(Preprocessor):
    """pcpp Preprocessor configured the same way as pcpp.pcmd.CmdPreprocessor.

    Adapted from `pcpp.pcmd.CmdPreprocessor.__init__` and its `on_*`
    overrides — the file I/O, argv parsing, and `inputs`/`-o` handling
    are stripped out; the rest is preserved.
    """

    def __init__(self, args: argparse.Namespace) -> None:
        super().__init__(lexer=_PP_LEXER.clone())
        self.args = args
        # The file `run` preprocesses, which `_where` leaves unnamed.
        self.input_file: str | None = None
        # The message for an invalid number in an `#if`, which
        # `_NumberEvaluator` records and `evalexpr` raises.
        self.number_error: str | None = None
        self.evaluator = _NumberEvaluator(self.evaluator, self)

        self.define("__PCPP_VERSION__ " + PCPP_VERSION)
        self.define("__PCPP_ALWAYS_FALSE__ 0")
        self.define("__PCPP_ALWAYS_TRUE__ 1")

        if args.debug:
            self.debugout = open("pcpp_debug.log", "wt")

        self.auto_pragma_once_enabled = not args.auto_pragma_once_disabled
        self.line_directive = args.line_directive
        if (self.line_directive is not None
                and self.line_directive.lower() in ("nothing", "none", "")):
            self.line_directive = None

        if args.passthru_includes is not None:
            self.passthru_includes = re.compile(args.passthru_includes[0])
        self.compress = 2 if args.compress else 0

        if args.passthru_magic_macros:
            self.undef("__DATE__")
            self.undef("__TIME__")
            self.expand_linemacro = False
            self.expand_filemacro = False
            self.expand_countermacro = False

        if args.assume_input_encoding is not None:
            args.assume_input_encoding = args.assume_input_encoding[0]
            self.assume_encoding = args.assume_input_encoding
        if args.output_encoding is not None:
            args.output_encoding = args.output_encoding[0]

        self.bypass_ifpassthru = False
        self.potential_include_guard = None

        if args.defines:
            args.defines = [x[0] for x in args.defines]
            for d in args.defines:
                if "=" not in d:
                    d += "=1"
                d = d.replace("=", " ", 1)
                self.define(d)
        if args.undefines:
            args.undefines = [x[0] for x in args.undefines]
            for d in args.undefines:
                self.undef(d)
        if args.nevers:
            args.nevers = [x[0] for x in args.nevers]
        if args.includes:
            args.includes = [x[0] for x in args.includes]
            for d in args.includes:
                self.add_path(d)
        # Append bundled <limits.h> etc. last so user `-I` paths
        # always take precedence; user-supplied headers can shadow
        # the c6502-standard ones.
        if os.path.isdir(_BUNDLED_INCLUDE_DIR):
            self.add_path(_BUNDLED_INCLUDE_DIR)

    def run(self, source: str, filename: str | None = None) -> str:
        self.input_file = filename
        self.parse(source, filename)
        out = io.StringIO()
        if self.args.write_bom:
            out.write("\ufeff")
        self.write(out)

        if self.args.time:
            self._print_time_report(sys.stderr)
        if self.args.filetimes:
            self._write_filetimes_csv(self.args.filetimes)
            self.args.filetimes.close()

        return out.getvalue()

    def on_include_not_found(self, is_malformed, is_system_include,
                             curdir, includepath):
        if self.args.passthru_unfound_includes:
            raise OutputDirective(Action.IgnoreAndPassThrough)
        return super().on_include_not_found(
            is_malformed, is_system_include, curdir, includepath)

    def on_unknown_macro_in_defined_expr(self, tok):
        if self.args.undefines and tok.value in self.args.undefines:
            return False
        if self.args.passthru_undefined_exprs:
            return None
        return super().on_unknown_macro_in_defined_expr(tok)

    def evalexpr(self, tokens):
        """Evaluates an `#if` or `#elif` expression, as
        `Preprocessor.evalexpr` does, and raises PreprocessorError for
        a number in it that isn't a valid integer constant."""
        self.number_error = None
        result = super().evalexpr(tokens)
        if self.number_error is not None:
            raise PreprocessorError(self.number_error)
        return result

    def on_error(self, file, line, msg):
        # `evalexpr` raises the error for an invalid number in an
        # `#if`, so pcpp's report of its evaluator's failure would
        # only repeat it.
        if self.number_error is not None:
            return
        super().on_error(file, line, msg)

    def on_unknown_macro_in_expr(self, ident):
        if ident in _IF_KEYWORD_VALUES:
            return _IF_KEYWORD_VALUES[ident]
        if self.args.undefines and ident in self.args.undefines:
            return super().on_unknown_macro_in_expr(ident)
        if self.args.passthru_undefined_exprs:
            return None
        return super().on_unknown_macro_in_expr(ident)

    def on_unknown_macro_function_in_expr(self, ident):
        if self.args.undefines and ident in self.args.undefines:
            return super().on_unknown_macro_function_in_expr(ident)
        if self.args.passthru_undefined_exprs:
            return None
        return super().on_unknown_macro_function_in_expr(ident)

    def on_directive_handle(self, directive, toks, ifpassthru, precedingtoks):
        if ifpassthru:
            if directive.value in ("if", "elif", "else", "endif"):
                self.bypass_ifpassthru = any(
                    tok.value in ("__PCPP_ALWAYS_FALSE__", "__PCPP_ALWAYS_TRUE__")
                    for tok in toks
                )
            if (not self.bypass_ifpassthru
                    and directive.value in ("define", "undef")):
                if toks[0].value != self.potential_include_guard:
                    raise OutputDirective(Action.IgnoreAndPassThrough)
        if (directive.value in ("define", "undef")
                and self.args.nevers
                and toks[0].value in self.args.nevers):
            raise OutputDirective(Action.IgnoreAndPassThrough)
        if self.args.passthru_defines:
            super().on_directive_handle(
                directive, toks, ifpassthru, precedingtoks)
            return None
        return super().on_directive_handle(
            directive, toks, ifpassthru, precedingtoks)

    def on_directive_unknown(self, directive, toks, ifpassthru, precedingtoks):
        # pcpp passes `#pragma` lines other than `#pragma once` through
        # here, with the file and line they're on.
        if directive.value == "pragma":
            _check_pragma(directive, toks, self._where(directive))
        if ifpassthru:
            return None
        return super().on_directive_unknown(
            directive, toks, ifpassthru, precedingtoks)

    def _where(self, tok) -> str:
        """Names the line `tok` is on, as "line 3" in the file being
        preprocessed, or "line 3 of <path>" in a file it includes.
        compile.py's message names the file being compiled."""
        source = tok.source
        if source is None or (
            self.input_file is not None
            and os.path.normcase(os.path.abspath(source))
            == os.path.normcase(os.path.abspath(self.input_file))
        ):
            return f"line {tok.lineno}"
        return f"line {tok.lineno} of {source}"

    def token(self):
        """Returns pcpp's next output token, as `Preprocessor.token`
        does, and warns about it when it's a `_Bool`. A `_Bool` that a
        macro expands to has the line of the macro's use."""
        tok = super().token()
        if (tok is not None and tok.type == self.t_ID
                and tok.value == "_Bool"):
            diagnostics.warn(self._where(tok), ALT_BOOL_WARNING)
        return tok

    def on_potential_include_guard(self, macro):
        self.potential_include_guard = macro
        return super().on_potential_include_guard(macro)

    def on_comment(self, tok):
        if self.args.passthru_comments:
            return True
        return super().on_comment(tok)

    def _print_time_report(self, stream) -> None:
        print("\nTime report:", file=stream)
        print("============", file=stream)
        for n, t in enumerate(self.include_times):
            if n == 0:
                print(f"top level: {t.elapsed:f} seconds", file=stream)
            elif t.depth == 1:
                pct = 100 * t.elapsed / self.include_times[0].elapsed
                print(f"\n {t.included_path}: {t.elapsed:f} seconds ({pct:f}%)",
                      file=stream)
            else:
                print(f"{' ' * t.depth}{t.included_path}: {t.elapsed:f} seconds",
                      file=stream)
        print("\nPragma once files (including heuristically applied):",
              file=stream)
        print("====================================================",
              file=stream)
        for i in self.include_once:
            print(" ", i, file=stream)
        print(file=stream)

    def _write_filetimes_csv(self, stream) -> None:
        print('"Total seconds","Self seconds","File size","File path"',
              file=stream)
        filetimes: dict[str, list[float]] = {}
        currentfiles: list[str] = []
        for t in self.include_times:
            while t.depth < len(currentfiles):
                currentfiles.pop()
            if t.depth > len(currentfiles) - 1:
                currentfiles.append(t.included_abspath)
            path = currentfiles[-1]
            if path in filetimes:
                filetimes[path][0] += t.elapsed
                filetimes[path][1] += t.elapsed
            else:
                filetimes[path] = [t.elapsed, t.elapsed]
            if t.elapsed > 0 and len(currentfiles) > 1:
                filetimes[currentfiles[-2]][1] -= t.elapsed
        rows = sorted(((v[0], v[1], k) for k, v in filetimes.items()),
                      reverse=True)
        for total, self_, path in rows:
            size = os.stat(path).st_size
            print(f'{total:f},{self_:f},{size:d},"{path}"', file=stream)


class PreprocessorError(Exception):
    """A `#pragma c6502 ...` line, or a number in an `#if` that isn't
    an integer constant. c6502's directives are C23 statement
    attributes now, and the message for a pragma names the one to
    write instead. Each message names the line in its source file."""


# Match a full `#pragma ...` line (anchored to start-of-line, possibly
# preceded by whitespace). Captures the body after `#pragma` for
# inspection.
_PRAGMA_LINE_RE = re.compile(r"^[ \t]*#[ \t]*pragma\b(?P<body>.*)$")

# The `#pragma c6502` directives c6502 used to take, with the
# whitespace removed, and what to write instead of each.
_REPLACED_PRAGMAS = {
    "loopunroll(enable)": (
        "write the statement attribute `[[c6502::unroll]]` before the "
        "loop instead, as in `[[c6502::unroll]] for (...)`"
    ),
    "loopunroll(disable)": (
        "it never did anything, so delete the line"
    ),
    "loopkeep": (
        "write the statement attribute `[[c6502::keep]]` before the "
        "loop instead, as in `[[c6502::keep]] for (...)`"
    ),
    "callsave(hardware_stack)": (
        "write the statement attribute `[[c6502::save_hardware_stack]]` "
        "before the statement or declaration instead, as in "
        "`[[c6502::save_hardware_stack]] f(x);`"
    ),
}


def _check_pragma(directive, toks, where: str) -> None:
    """Raises PreprocessorError for a `#pragma c6502` directive, whose
    message names the C23 statement attribute that replaced it
    (docs/attributes.md). `directive` is pcpp's `pragma` token, `toks`
    the tokens after it, and `where` names the file the line is in,
    such as "line 3" or "line 3 of foo.h". Any other pragma passes."""
    body = "".join(tok.value for tok in toks).strip()
    words = body.split()
    if not words or words[0] != "c6502":
        return
    line = f"#pragma {body}"
    instead = _REPLACED_PRAGMAS.get("".join(words[1:]))
    if instead is not None:
        raise PreprocessorError(
            f"{where}: c6502 no longer takes `{line}`; {instead}"
        )
    raise PreprocessorError(
        f"{where}: c6502 takes no `#pragma c6502` directives, so "
        f"`{line}` is an error. Its directives are the statement "
        f"attributes `[[c6502::unroll]]` and `[[c6502::keep]]` before a "
        f"`for` loop, and `[[c6502::save_hardware_stack]]` before a "
        f"statement or declaration"
    )


def _strip_pragmas(text: str) -> str:
    """Strips every `#pragma` line pcpp passed through, as C99 §6.10.6
    lets an implementation do with a pragma it doesn't recognize.
    `CompilePreprocessor.on_directive_unknown` has already rejected the
    `#pragma c6502` lines. Each `#pragma` line becomes a blank line, so
    that the line numbers of later errors stay the same."""
    return "\n".join(
        "" if _PRAGMA_LINE_RE.match(line) else line
        for line in text.split("\n")
    )


def preprocess(
    source: str, argv: list[str] | None = None,
    filename: str | None = None,
) -> str:
    """Preprocess `source`. `argv` accepts pcpp/pcmd.py-style flags minus -o.
    `filename` is the file `source` came from, if any: a quoted
    `#include "x.h"` looks in its directory first (C99 §6.10.2.3), and
    messages name it.

    With no argv (or `[]`): strip comments and suppress `#line` markers.
    """
    return _strip_pragmas(
        CompilePreprocessor(parse_args(argv)).run(source, filename),
    )
