"""Runs the checks of the C++ port that need no Python compiler
(docs/cpp_port.md, "The checks after Python"): `c6502cpp check-golden`
on the golden files under golden/, which tools/write_golden.py wrote
with the frozen Python compiler, and `c6502cpp check-frozen` on the
frozen captures under cpp/tests/frozen, which tools/freeze_captures.py
kept. Both are skipped when c6502cpp isn't built."""
from __future__ import annotations

import os
import subprocess
import unittest
from pathlib import Path

from tests.test_ir_dump import cpp_cli

_ROOT = Path(__file__).resolve().parent.parent

# The worker processes of check-golden. Its 8,944 jobs take about a
# minute with 12.
_WORKERS = max(1, min(12, os.cpu_count() or 1))


@unittest.skipUnless(cpp_cli() is not None, "c6502cpp isn't built")
class TestChecksAfterPython(unittest.TestCase):
    def _run(self, *args: str) -> subprocess.CompletedProcess:
        return subprocess.run(
            [str(cpp_cli()), *args], capture_output=True, text=True,
            encoding="utf-8", errors="replace", cwd=_ROOT,
        )

    def test_golden(self):
        done = self._run("check-golden", "--root", str(_ROOT), "-j", str(_WORKERS))
        self.assertEqual(done.returncode, 0, done.stdout[-4000:] + done.stderr)
        self.assertIn(" 0 differ", done.stdout.splitlines()[-1])

    def test_frozen(self):
        done = self._run("check-frozen", str(_ROOT / "cpp" / "tests" / "frozen"))
        self.assertEqual(done.returncode, 0, done.stdout[-4000:] + done.stderr)
        self.assertNotIn("MISMATCH", done.stdout)
        self.assertNotIn("DIFFER", done.stdout)


if __name__ == "__main__":
    unittest.main()
