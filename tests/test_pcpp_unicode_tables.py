"""Checks that cpp/src/pcpp_unicode.h is what tools/pcpp_unicode_tables.py
writes from this Python, so the C++ preprocessor's classes of Unicode
characters stay those of the Python that runs the Python preprocessor."""
from __future__ import annotations

import os
import sys
import unittest

_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(_ROOT, "tools"))

import pcpp_unicode_tables  # noqa: E402


class TestCheckedInHeader(unittest.TestCase):
    def test_regenerating_gives_the_checked_in_header(self):
        # After a change of Python's Unicode version, regenerate with
        # `uv run python tools/pcpp_unicode_tables.py`.
        with open(pcpp_unicode_tables._OUT, "rb") as f:
            self.assertEqual(
                f.read().decode("utf-8"), pcpp_unicode_tables.generate(),
            )


if __name__ == "__main__":
    unittest.main()
