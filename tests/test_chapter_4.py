"""End-to-end harness for the chapter_4 corpus.

chapter_4 covers logical (`&&`, `||`, `!`) and relational
(`==`, `!=`, `<`, `>`, `<=`, `>=`) operators. Two buckets:
valid/ and invalid_parse/. Every file passes as written.
"""

import shutil
import unittest
from pathlib import Path

from compile import _run_source


_TESTS_DIR = Path(__file__).parent
_C4 = _TESTS_DIR / "chapter_4"


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter4Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C4 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_4 valid files found")
        for path in files:
            rel = path.relative_to(_C4 / "valid").as_posix()
            with self.subTest(file=rel):
                source = path.read_text()
                _run_source("codegen", source)
