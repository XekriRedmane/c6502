"""Builds a Lark LALR parser once and keeps its tables in a file.

Building a Lark LALR parser for c99.lark computes its parse table
(`compute_lalr`), which takes about half a second, and the lexer and
the parser each built one in every process that imported them. Lark's
own `cache=True` option writes its file in place, so a pytest-xdist
worker or a fuzzer worker could read a file another one was still
writing.

`open_lalr` saves the built parser's tables, as `Lark.save` does, to
`__pycache__/<stem>-<options>-<key>.lark.pickle` next to the grammar,
and every later process loads them from there with Lark's
`_load_from_dict`, passing the transformer again, since a transformer
isn't saved. `<options>` is a hash of the options that shape the
tables (`parser`, `lexer`, `start`), and `<key>` a hash of those, the
grammar's text, Lark's version, Python's version, and the file
format. A file is valid only for the grammar, options, and versions it
was built from, and a change to any of them changes the key, so a
stale file is never read.

A load gives the table the build pickled, state for state and action
for action in the same order. A table Lark builds numbers its states
and orders each state's actions by iterating sets of strings, whose
order differs between processes with their string hashes, so two
processes that each build in memory get different tables already.
Every such table parses an input to the same tree and stops at the
same token of a bad input with the same set of expected tokens
(tests/test_lark_cache.py compares them), so a table loaded from the
file gives the parses and syntax errors of one built in memory.

The file is written to a temporary name in the same directory and
then renamed over the final name (`os.replace`), so a reader sees
either no file or a whole one. When the directory can't be written,
or a file can't be read for any reason, the parser is built in memory
as before. When a new file is written, the files of the same grammar
built with the same options under other keys are removed.
"""
from __future__ import annotations

import hashlib
import os
import pickle
import sys
import tempfile
from pathlib import Path

import lark
from lark import Lark
from lark.grammar import Rule
from lark.lexer import TerminalDef

# Bumped when the layout of the cache file changes.
_FORMAT = 1


def _digest(*parts: str) -> str:
    h = hashlib.sha256()
    for part in parts:
        h.update(part.encode("utf-8"))
        h.update(b"\0")
    return h.hexdigest()[:32]


def _options(parser: str, lexer, start) -> str:
    """A hash of the options that shape the tables, which tells apart
    the cache files of one grammar built two ways."""
    lexer_name = (
        f"{lexer.__module__}.{lexer.__qualname__}"
        if isinstance(lexer, type) else str(lexer)
    )
    return _digest(parser, lexer_name, repr(list(start)))[:8]


def _key(grammar: str, options: str) -> str:
    """The hash that names the cache file of one grammar built with the
    options `options` hashes, under this Lark and this Python."""
    return _digest(str(_FORMAT), lark.__version__, sys.version, options,
                   grammar)


def open_lalr(grammar_path: Path, *, lexer, start, transformer=None) -> Lark:
    """Returns the LALR parser for the grammar at `grammar_path`, with
    `lexer` (a name or a lexer class), the start rules `start`, and
    `transformer`, loaded from its cache file when there is one and
    built and saved there otherwise (see the module docstring)."""
    # Read as `Lark.open` reads it, with CRLF line ends made LF.
    grammar = grammar_path.read_text(encoding="utf-8")
    options = _options("lalr", lexer, start)
    key = _key(grammar, options)
    cache_dir = grammar_path.parent / "__pycache__"
    prefix = f"{grammar_path.stem}-{options}-"
    cache_file = cache_dir / f"{prefix}{key}.lark.pickle"
    extra = {} if transformer is None else {"transformer": transformer}
    try:
        with open(cache_file, "rb") as f:
            saved = pickle.load(f)
        if saved["key"] == key:
            return Lark._load_from_dict(saved["data"], saved["memo"], **extra)
    except Exception:  # noqa: BLE001 -- a missing or bad file is rebuilt
        pass
    parser = Lark(
        grammar, parser="lalr", lexer=lexer, start=list(start),
        source_path=str(grammar_path), **extra,
    )
    _save(parser, key, cache_dir, prefix, cache_file)
    return parser


def _save(parser: Lark, key: str, cache_dir: Path, prefix: str,
          cache_file: Path) -> None:
    """Writes `parser`'s tables to `cache_file` atomically, and removes
    the files with the same `prefix` under other keys. Does nothing
    when the directory can't be written."""
    data, memo = parser.memo_serialize([TerminalDef, Rule])
    # The transformer is passed again on each load.
    data["options"] = {
        n: v for n, v in data["options"].items() if n != "transformer"
    }
    tmp = None
    try:
        cache_dir.mkdir(exist_ok=True)
        fd, tmp = tempfile.mkstemp(dir=cache_dir, prefix=prefix,
                                   suffix=".tmp")
        with os.fdopen(fd, "wb") as f:
            pickle.dump({"key": key, "data": data, "memo": memo}, f,
                        protocol=pickle.HIGHEST_PROTOCOL)
        os.replace(tmp, cache_file)
        tmp = None
        for old in cache_dir.glob(f"{prefix}*.lark.pickle"):
            if old != cache_file:
                old.unlink(missing_ok=True)
    except OSError:
        pass
    finally:
        if tmp is not None:
            try:
                os.unlink(tmp)
            except OSError:
                pass
