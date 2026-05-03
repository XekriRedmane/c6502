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
"""

from __future__ import annotations

import argparse
import io
import os
import re
import sys

from pcpp.preprocessor import Action, OutputDirective, Preprocessor

PCPP_VERSION = "1.30"

# Bundled standard headers (limits.h, etc.) live alongside this
# module. Added to pcpp's search path AFTER any user `-I` paths
# so user-supplied headers can shadow ours, but `#include
# <limits.h>` still resolves without the user passing `-I`.
_BUNDLED_INCLUDE_DIR = os.path.join(
    os.path.dirname(os.path.abspath(__file__)), "include",
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
        super().__init__()
        self.args = args

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

    def run(self, source: str) -> str:
        self.parse(source)
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

    def on_unknown_macro_in_expr(self, ident):
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
        if ifpassthru:
            return None
        return super().on_directive_unknown(
            directive, toks, ifpassthru, precedingtoks)

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


def preprocess(source: str, argv: list[str] | None = None) -> str:
    """Preprocess `source`. `argv` accepts pcpp/pcmd.py-style flags minus -o.

    With no argv (or `[]`): strip comments and suppress `#line` markers.
    """
    return CompilePreprocessor(parse_args(argv)).run(source)
