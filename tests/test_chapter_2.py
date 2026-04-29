"""End-to-end harness for the chapter_2 corpus.

chapter_2 covers unary operators (`-`, `~`) and bitwise expressions.
Two buckets: valid/ and invalid_parse/.

`bitwise_int_min.c` and `negate_int_max.c` use 2147483647 (INT_MAX
in upstream's 4-byte int world); c6502's `int` is 1 byte. Both go in
`_INCOMPATIBLE_VALID`.
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
_C2 = _TESTS_DIR / "chapter_2"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)


_INCOMPATIBLE_VALID = frozenset([
    "bitwise_int_min.c",
    "negate_int_max.c",
])


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter2Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C2 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_2 valid files found")
        for path in files:
            rel = str(path.relative_to(_C2 / "valid"))
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter2InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C2 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C2 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)
