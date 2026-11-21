"""Tests for tools/lalr_table.py, the C++ port's copy of the tables Lark
computes from c99.lark (cpp/generated/c99_lalr.cpp)."""
from __future__ import annotations

import os
import subprocess
import sys
import unittest
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(_ROOT / "tools"))

import lalr_table  # noqa: E402


class TestCheckedInTable(unittest.TestCase):
    def test_regenerating_gives_the_checked_in_file(self):
        # After editing c99.lark, regenerate with
        # `uv run python tools/lalr_table.py`.
        with open(lalr_table.OUTPUT, "rb") as f:
            checked_in = f.read().decode("utf-8")
        self.assertEqual(lalr_table.generate(), checked_in)

    def test_the_numbering_doesnt_depend_on_string_hashes(self):
        # Lark numbers its states by iterating sets of strings, so a
        # build in another process numbers them differently. The file's
        # numbering is canonical, so a build there with another hash
        # seed, and without the cached table, gives the same text.
        env = dict(os.environ, PYTHONHASHSEED="12345")
        code = (
            "import sys, pathlib\n"
            "sys.path.insert(0, 'tools')\n"
            "import lark_cache\n"
            "lark_cache._FORMAT = 'test-no-cache'\n"
            "lark_cache._save = lambda *args: None\n"
            "import lalr_table\n"
            "sys.stdout.buffer.write(lalr_table.generate().encode('utf-8'))\n"
        )
        out = subprocess.run(
            [sys.executable, "-c", code], cwd=_ROOT, env=env,
            capture_output=True, check=True,
        ).stdout.decode("utf-8")
        self.assertEqual(out, lalr_table.generate())


class TestCppString(unittest.TestCase):
    def test_escapes_quotes_backslashes_and_control_characters(self):
        self.assertEqual(lalr_table._cpp_string('a"\\\t\x0bz'),
                         '"a\\"\\\\\\011\\013z"')


if __name__ == "__main__":
    unittest.main()
