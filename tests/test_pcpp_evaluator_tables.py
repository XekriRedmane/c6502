"""Checks that cpp/src/pcpp_evaluator_tables.h holds the tables of the
LALR parser that pcpp's `#if` evaluator runs, as
tools/pcpp_evaluator_tables.py writes them from the live parser."""
from __future__ import annotations

import os
import unittest

from tools import pcpp_evaluator_tables

_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


class TestCheckedInTables(unittest.TestCase):
    def test_regenerating_gives_the_checked_in_header(self):
        # After a change to pcpp or PLY, regenerate with
        # `uv run python tools/pcpp_evaluator_tables.py`.
        path = os.path.join(_ROOT, "cpp", "src", "pcpp_evaluator_tables.h")
        with open(path, "rb") as f:
            checked_in = f.read().decode("utf-8")
        self.assertEqual(checked_in, pcpp_evaluator_tables.generate())


if __name__ == "__main__":
    unittest.main()
