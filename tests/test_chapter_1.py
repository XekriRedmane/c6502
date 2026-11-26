"""End-to-end harness for the chapter_1 corpus.

chapter_1 covers the minimal `int main(void) { return N; }` program
form. Three buckets:

  valid/         — must compile through `-S`.
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

from compile import _run_source


_TESTS_DIR = Path(__file__).parent
_C1 = _TESTS_DIR / "chapter_1"


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter1Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C1 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_1 valid files found")
        for path in files:
            rel = path.relative_to(_C1 / "valid").as_posix()
            with self.subTest(file=rel):
                source = path.read_text()
                _run_source("codegen", source)
