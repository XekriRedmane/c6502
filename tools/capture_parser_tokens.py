"""Records the tokens that parser.py's lexer gives Lark's parser, and
what lexer.py's `tokenize` gives, so that the C++ lexer and recognizer
(cpp/src/parser_context.h) can be checked on them before the C++
parser builds trees.

    # The corpus: record its calls of `parse`, then their tokens.
    uv run python tools/capture_passes.py --passes parse -j 12
    uv run python tools/capture_parser_tokens.py captures

    # The Python tests' calls of `parse` and `tokenize`, as a pytest plugin.
    C6502_TOKEN_CAPTURES=captures PYTHONPATH=tools \\
        uv run pytest -p capture_parser_tokens -n 12 -q

    cpp/build/bin/c6502cpp check-captures captures/parse_tokens captures/tokenize

The records have the format of tools/capture_passes.py. A record of
`parse_tokens` has the source as its `input-json` line, and its
arguments are what the parser's actions decide and the lexer only
reads: the positions, among the tokens lexed, of the identifiers that
named a typedef in scope when they were lexed, and of the lookaheads
that named one when a `for` statement ended (`retype_lookahead`).
Whether such an identifier became a TYPEDEF_NAME depends on the parse
table, which is what the C++ side computes. Its result, on an
`output-json` line, is `[tokens, outcome]`. Each token is `[type,
value, line, column, scopes]` as the parser was fed it, with the
number of scopes open after it was lexed. The outcome is None for a
parse that succeeded, `[kind, type, message]` for one that raised a
syntax error (kind "syntax", from Lark, with parser.py's rewording) or
a LexError from the lexer's check of a number (kind "lex"), and
`["action", fed]` for one whose error came from an action of the
parser, which the C++ recognizer has no part in, with the number of
tokens the parser had shifted before it.

A record of `tokenize` has the source as its `input-json` line and the
tokens `[kind, value, line, col]` as its result, or the LexError as an
`output-error` section.
"""
from __future__ import annotations

import json
import os
import sys
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
if str(_ROOT) not in sys.path:
    sys.path.insert(0, str(_ROOT))
if str(_ROOT / "tools") not in sys.path:
    sys.path.insert(0, str(_ROOT / "tools"))

import capture_passes  # noqa: E402
import lexer  # noqa: E402
import parser  # noqa: E402
from lark.exceptions import UnexpectedInput  # noqa: E402

_REAL_PARSE = parser.parse
_REAL_TOKENIZE = lexer.tokenize


class _Trace:
    """What one parse lexed, and what the parser's lookups answered."""

    def __init__(self):
        self.tokens: list = []
        self.depths: list[int] = []
        self.lex_typedefs: list[int] = []
        self.retype_typedefs: list[int] = []
        self.calls = 0
        self.phase: str | None = None
        self.found: bool | None = None
        self.error_kind: str | None = None


_TRACE: _Trace | None = None


def _install_tracing() -> None:
    """Wraps the parser's lexer, `retype_lookahead`, and `lookup`, which
    record into the current trace while one is open."""
    real_next = parser._TypedefLexer.next_token
    real_retype = parser._ParseContext.retype_lookahead
    real_lookup = parser._ParseContext.lookup

    def lookup(self, name):
        binding = real_lookup(self, name)
        t = _TRACE
        if t is not None and t.phase is not None:
            t.found = binding is not None and binding.is_typedef
        return binding

    def next_token(self, lex_state, parser_state=None):
        t = _TRACE
        if t is None:
            return real_next(self, lex_state, parser_state)
        t.calls += 1
        t.phase, t.found = "lex", None
        try:
            token = real_next(self, lex_state, parser_state)
        finally:
            t.phase = None
        if t.found:
            t.lex_typedefs.append(len(t.tokens))
        t.tokens.append(token)
        t.depths.append(len(parser._CTX.scopes))
        return token

    def retype_lookahead(self, rule_origin):
        t = _TRACE
        if t is None:
            return real_retype(self, rule_origin)
        t.phase, t.found = "retype", None
        try:
            real_retype(self, rule_origin)
        finally:
            t.phase = None
        if t.found and t.tokens:
            t.retype_typedefs.append(len(t.tokens) - 1)

    parser._TypedefLexer.next_token = next_token
    parser._ParseContext.retype_lookahead = retype_lookahead
    parser._ParseContext.lookup = lookup


class _LarkProxy:
    """Stands in for parser.py's `_LARK` while a parse is traced, to see
    what the Lark parse itself raised before `parse` rewords it."""

    def __init__(self, real):
        self.real = real

    def parse(self, *args, **kwargs):
        try:
            return self.real.parse(*args, **kwargs)
        except UnexpectedInput:
            _TRACE.error_kind = "syntax"
            raise
        except lexer.LexError:
            _TRACE.error_kind = "lex"
            raise
        except Exception:
            _TRACE.error_kind = "action"
            raise


_installed = False


def traced_parse(source: str):
    """Parses `source` as `parser.parse` does, and returns the result or
    the exception it raised, the trace, and the record's result."""
    global _TRACE, _installed
    if not _installed:
        _install_tracing()
        _installed = True
    trace = _Trace()
    _TRACE = trace
    real_lark = parser._LARK
    parser._LARK = _LarkProxy(real_lark)
    result = error = None
    try:
        result = _REAL_PARSE(source)
    except Exception as e:  # noqa: BLE001 - recorded and raised again
        error = e
    finally:
        parser._LARK = real_lark
        _TRACE = None
    tokens = [[tok.type, str(tok), tok.line, tok.column, depth]
              for tok, depth in zip(trace.tokens, trace.depths)]
    if error is None:
        outcome = None
    elif trace.error_kind in ("syntax", "lex"):
        outcome = [trace.error_kind, type(error).__name__, str(error)]
    else:
        outcome = ["action", trace.calls - 1]
    return result, error, trace, [tokens, outcome]


def record_parse(source: str):
    """Parses `source`, records its `parse_tokens` record, and returns
    the result or raises the error, as `parser.parse` does."""
    result, error, trace, recorded = traced_parse(source)
    capture_passes._record(
        "parse_tokens",
        "input-json\n" + json.dumps(capture_passes.encode(source)) + "\n",
        json.dumps({"args": [trace.lex_typedefs, trace.retype_typedefs], "kwargs": {}}),
        recorded,
        context=capture_passes.context_json(),
    )
    if error is not None:
        raise error
    return result


def record_tokenize(source: str):
    """Runs `lexer.tokenize` on `source`, records its `tokenize` record,
    and returns the tokens or raises the error."""
    error = None
    tokens = []
    try:
        tokens = list(_REAL_TOKENIZE(source))
    except Exception as e:  # noqa: BLE001 - recorded and raised again
        error = e
    capture_passes._record(
        "tokenize",
        "input-json\n" + json.dumps(capture_passes.encode(source)) + "\n",
        json.dumps({"args": [], "kwargs": {}}),
        [[t.kind.value, t.value, t.line, t.col] for t in tokens],
        context=capture_passes.context_json(),
        error=error,
    )
    if error is not None:
        raise error
    return iter(tokens)


def from_parse_captures(captures: Path) -> int:
    """Writes a `parse_tokens` record for the source of each `parse`
    record under `captures`, and returns how many it read."""
    capture_passes._OUT = captures
    n = 0
    for path in sorted((captures / "parse").glob("*.txt")):
        lines = path.read_bytes().decode("utf-8").split("\n")
        source_line = lines[lines.index("input-json") + 1]
        src = next(l for l in lines if l.startswith("source "))
        capture_passes._SOURCE = src[len("source "):]
        source = json.loads(source_line)
        try:
            record_parse(source)
        except Exception:  # noqa: BLE001 - the record holds it
            pass
        n += 1
    return n


# --- As a pytest plugin ---

if os.environ.get("C6502_TOKEN_CAPTURES") and "pytest" in sys.modules:
    capture_passes._OUT = Path(os.environ["C6502_TOKEN_CAPTURES"])
    capture_passes._SOURCE = "the Python tests"
    parser.parse = record_parse
    lexer.tokenize = record_tokenize


def main(argv: list[str] | None = None) -> int:
    import argparse
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("captures", help="the capture directory, with its parse records")
    ns = ap.parse_args(argv)
    n = from_parse_captures(Path(ns.captures))
    print(f"{n} sources of parse records traced")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
