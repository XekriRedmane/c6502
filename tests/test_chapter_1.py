"""End-to-end harness for the chapter_1 corpus.

chapter_1 covers the minimal `int main(void) { return N; }` program
form. Three buckets:

  valid/         — must compile through `--codegen`.
  invalid_lex/   — must fail at the lexer.
  invalid_parse/ — must fail at lex or parse.

These were previously held in tests/{valid,invalid_lex,invalid_parse}/
with bespoke test classes in test_lexer.py and test_parser.py;
moved under tests/chapter_1/ to match the per-chapter layout used by
chapters 2 onwards.
"""

import shutil
import unittest
from pathlib import Path

from compile import _run_stage
from lark.exceptions import UnexpectedInput
from lexer import LexError, tokenize
from parser import ParserError, parse
from preprocessor import preprocess


_TESTS_DIR = Path(__file__).parent
_C1 = _TESTS_DIR / "chapter_1"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter1Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C1 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_1 valid files found")
        for path in files:
            rel = str(path.relative_to(_C1 / "valid"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter1InvalidLex(unittest.TestCase):
    def test_lex_fails(self):
        files = sorted((_C1 / "invalid_lex").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_lex files found")
        for path in files:
            rel = str(path.relative_to(_C1 / "invalid_lex"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(LexError):
                    list(tokenize(source))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter1InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C1 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C1 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)
