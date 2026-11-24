"""Tests for `passes.prune_unused_slots.prune_unused_locals`.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

from __future__ import annotations

import re
import shutil
import unittest


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestEmittedLocalsDefined(unittest.TestCase):

    def test_every_referenced_local_has_an_equ(self):
        from compile import _run_stage
        src = """
            int main(void) {
                int a[4] = {1, 2, 3, 4}; int i = 0;
                a[i++] += 10; a[i++] *= 3; a[--i] <<= 1;
                return a[0] + a[1] + i + a[2] - 1;
            }
        """
        out = _run_stage("codegen", src, optimize=True)
        body = "\n".join(
            ln for ln in out.splitlines() if not ln.startswith(";")
        )
        defined = set(re.findall(r"^(__local_\w+)\s+EQU", body, re.M))
        used = set(re.findall(r"\b(__local_\w+?)(?:\+\d+)?\b", body))
        self.assertTrue(used)
        self.assertEqual(used - defined, set())


if __name__ == "__main__":
    unittest.main()
