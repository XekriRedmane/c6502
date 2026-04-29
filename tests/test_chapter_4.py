"""End-to-end harness for the chapter_4 corpus.

chapter_4 covers logical (`&&`, `||`, `!`) and relational
(`==`, `!=`, `<`, `>`, `<=`, `>=`) operators. Two buckets:
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
_C4 = _TESTS_DIR / "chapter_4"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter4Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C4 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_4 valid files found")
        for path in files:
            rel = str(path.relative_to(_C4 / "valid"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter4InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C4 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C4 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)
