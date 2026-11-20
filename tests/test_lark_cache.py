"""lark_cache.py: the LALR tables of c99.lark, computed once and loaded
from a file by later processes."""
from __future__ import annotations

import os
import shutil
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from lark import Lark
from lark.exceptions import UnexpectedToken

import lark_cache
import parser as c_parser

_GRAMMAR = Path(c_parser.__file__).parent / "c99.lark"


def _table(lark: Lark):
    """The parse table of `lark` as nested lists, in the order of its
    dicts, so that two tables compare equal only when they hold the same
    actions in the same order."""
    states = lark.parser.parser.parser.parse_table.states
    return [
        (s, [(t, (str(kind), arg)) for t, (kind, arg) in acts.items()])
        for s, acts in states.items()
    ]


class TestLarkCache(unittest.TestCase):

    def setUp(self) -> None:
        self.dir = Path(tempfile.mkdtemp())
        self.grammar = self.dir / "c99.lark"
        shutil.copyfile(_GRAMMAR, self.grammar)

    def tearDown(self) -> None:
        shutil.rmtree(self.dir, ignore_errors=True)

    def _open(self):
        return lark_cache.open_lalr(
            self.grammar, lexer="basic", start=["start", "lex_only"],
        )

    def _files(self):
        return sorted((self.dir / "__pycache__").glob("*.lark.pickle"))

    def test_load_gives_the_built_table(self) -> None:
        built = self._open()
        self.assertEqual(len(self._files()), 1)
        with mock.patch.object(Lark, "__init__",
                               side_effect=AssertionError("rebuilt")):
            loaded = self._open()
        self.assertEqual(_table(loaded), _table(built))
        source = "int main(void) { return 0x1F + 'a'; }"
        self.assertEqual(
            [(t.type, str(t)) for t in loaded.lex(source)],
            [(t.type, str(t)) for t in built.lex(source)],
        )

    def test_parses_match_a_parser_built_in_memory(self) -> None:
        # The parser of parser.py, which may come from the cache file,
        # against one Lark builds from the grammar with its options. A
        # table's state numbers and the order of each state's actions
        # come from the iteration order of sets of strings, which
        # differs between processes with their string hashes, so the
        # two are compared by what they parse: the AST, or the syntax
        # error's token and set of expected tokens.
        fresh = Lark.open(
            str(_GRAMMAR), parser="lalr", lexer=c_parser._TypedefLexer,
            transformer=c_parser._BUILDER,
            start=["start", "lex_only", "declarator", "type_name"],
        )
        root = _GRAMMAR.parent / "tests"
        sources = sorted(root.glob("chapter_*/invalid_parse/*.c"))
        sources += sorted(root.glob("chapter_1[0-9]/valid/*.c"))[:40]
        self.assertGreater(len(sources), 100)

        def outcome(source):
            try:
                return ("ast", c_parser.parse(source))
            except UnexpectedToken as e:
                return ("token", str(e.token), e.line, e.column,
                        frozenset(e.expected))
            except Exception as e:  # noqa: BLE001
                return ("error", type(e).__name__, str(e))

        for path in sources:
            source = path.read_text(encoding="utf-8")
            cached = outcome(source)
            with mock.patch.object(c_parser, "_LARK", fresh):
                built = outcome(source)
            self.assertEqual(cached, built, path.name)

    def test_unwritable_directory_builds_in_memory(self) -> None:
        # A file where the cache directory would be makes mkdir fail.
        (self.dir / "__pycache__").write_text("not a directory")
        parser = self._open()
        self.assertEqual(
            [t.type for t in parser.lex("int x;")],
            ["INT", "IDENTIFIER", "SEMICOLON"],
        )
        self.assertEqual(sorted(os.listdir(self.dir)),
                         ["__pycache__", "c99.lark"])

    def test_bad_file_is_rebuilt(self) -> None:
        self._open()
        (path,) = self._files()
        path.write_bytes(b"truncated")
        parser = self._open()
        self.assertEqual([t.type for t in parser.lex("x")], ["IDENTIFIER"])
        self.assertGreater(path.stat().st_size, 1000)

    def test_changed_grammar_replaces_the_file(self) -> None:
        self._open()
        (old,) = self._files()
        with open(self.grammar, "a", encoding="utf-8") as f:
            f.write("\n// changed\n")
        self._open()
        (new,) = self._files()
        self.assertNotEqual(old.name, new.name)

    def test_options_have_files_of_their_own(self) -> None:
        self._open()
        lark_cache.open_lalr(self.grammar, lexer="basic", start=["start"])
        self.assertEqual(len(self._files()), 2)


if __name__ == "__main__":
    unittest.main()
