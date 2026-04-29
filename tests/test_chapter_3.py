"""End-to-end harness for the chapter_3 corpus.

chapter_3 covers binary arithmetic operators (`+`, `-`, `*`, `/`,
`%`) and bitwise/shift operators in extra_credit. Two buckets:
valid/ and invalid_parse/. Every file passes as written.
"""

import shutil
import unittest
from pathlib import Path

from compile import _run_stage
from lark.exceptions import UnexpectedInput
from lexer import LexError
from parser import ParserError, parse
from preprocessor import preprocess


_TESTS_DIR = Path(__file__).parent
_C3 = _TESTS_DIR / "chapter_3"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter3Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C3 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_3 valid files found")
        for path in files:
            rel = str(path.relative_to(_C3 / "valid"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter3InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C3 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C3 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)
