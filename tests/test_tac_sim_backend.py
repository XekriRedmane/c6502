"""Tests of the C++ port's TAC interpreter through cpp_backend.py, which
`tests.test_sim_chapters._run_program`, fuzz/reduce.py, and
fuzz/tac_passes.py use under `C6502_CPP_BACKEND=1`. tools/compare_tac_sim.py
compares the two interpreters on the whole corpus, and these check the
plumbing: the results, the errors of each kind, the observer's names of
the passes, and the modules Python imports."""
from __future__ import annotations

import os
import re
import subprocess
import sys
import unittest
from pathlib import Path

from tests.test_ir_dump import cpp_cli

_ROOT = Path(__file__).resolve().parent.parent

_PROGRAMS = {
    "value": "int main(void) { int s = 0; for (int i = 0; i < 6; i++) s += i * i; return s; }\n",
    "negative": "long main(void) { long a = 2000000000; return a + a; }\n",
    "void": "void main(void) { }\n",
    "division": "int main(void) { int z = 0; return 7 / z; }\n",
    "front": "int main(void) { return x; }\n",
    "falls": "int f(int a) { if (a) return 1; }\nint main(void) { return f(0); }\n",
    "struct": (
        "struct pair { int x; int y; };\n"
        "struct pair make(int a, int b) { struct pair p; p.x = a; p.y = b; return p; }\n"
        "int sum(struct pair p) { return p.x + p.y; }\n"
        "int main(void) { return sum(make(20, 22)); }\n"
    ),
}


def _outcome(run):
    try:
        return ("value", run())
    except Exception as e:  # noqa: BLE001 - an error is a result
        return ("error", type(e).__name__, str(e))


class TestPassClasses(unittest.TestCase):
    def test_table_matches_the_driver(self):
        # tac-observe names a step by its pass's Python class, from a
        # table in the C++ source.
        from passes.optimization.optimizer import _DRIVER
        text = (_ROOT / "cpp" / "src" / "tac_sim_modes.cpp").read_text(encoding="utf-8")
        table = text[text.index("kPassClasses[]"):]
        table = table[:table.index("};")]
        cpp = re.findall(r'\{"(\w+)", "(\w+)"\}', table)
        python = [
            (p.name, type(p).__name__)
            for slot in ("pre_ssa", "pre_fixedpoint", "fixedpoint", "post_fixedpoint",
                         "post_destruction", "post_destruction_fixedpoint")
            for p in getattr(_DRIVER, slot)
        ]
        self.assertEqual(cpp, python)


@unittest.skipUnless(cpp_cli() is not None, "c6502cpp isn't built")
class TestAgainstPython(unittest.TestCase):
    def setUp(self):
        self._saved = os.environ.pop("C6502_CPP_BACKEND", None)

    def tearDown(self):
        if self._saved is not None:
            os.environ["C6502_CPP_BACKEND"] = self._saved

    def _both(self, fn, src):
        python = _outcome(lambda: fn(src))
        os.environ["C6502_CPP_BACKEND"] = "1"
        try:
            cpp = _outcome(lambda: fn(src))
        finally:
            os.environ.pop("C6502_CPP_BACKEND", None)
        return python, cpp

    def test_run_program(self):
        from tests.test_sim_chapters import _run_program
        for name, src in _PROGRAMS.items():
            with self.subTest(name):
                python, cpp = self._both(_run_program, src)
                self.assertEqual(python, cpp)

    def test_tac_results(self):
        from fuzz.reduce import _tac_results
        for name, src in _PROGRAMS.items():
            with self.subTest(name):
                python, cpp = self._both(_tac_results, src)
                self.assertEqual(python, cpp)

    def test_falls_off_end(self):
        from fuzz.reduce import falls_off_end
        for name, src in _PROGRAMS.items():
            with self.subTest(name):
                python, cpp = self._both(falls_off_end, src)
                self.assertEqual(python, cpp)
        self.assertEqual(self._both(falls_off_end, _PROGRAMS["falls"])[1], ("value", True))

    def test_first_changing_pass(self):
        from fuzz.tac_passes import first_changing_pass
        for name in ("value", "struct", "division"):
            with self.subTest(name):
                python, cpp = self._both(first_changing_pass, _PROGRAMS[name])
                self.assertEqual(python, cpp)
                self.assertEqual(cpp, ("value", None))

    def test_imports_none_of_the_front_end(self):
        code = (
            "import sys\n"
            "from tests.test_sim_chapters import _run_program\n"
            "from fuzz.reduce import _tac_results, falls_off_end\n"
            "from fuzz.tac_passes import first_changing_pass\n"
            f"src = {_PROGRAMS['value']!r}\n"
            "_run_program(src); _tac_results(src); falls_off_end(src)\n"
            "first_changing_pass(src)\n"
            "front = {'parser', 'lark', 'pcpp', 'preprocessor', 'c99_to_tac', 'tac_sim',\n"
            "         'passes.type_checking', 'passes.optimization.optimizer',\n"
            "         'passes.identifier_resolution', 'c99_ast'}\n"
            "print(sorted(m for m in sys.modules if m in front))\n"
        )
        env = {**os.environ, "C6502_CPP_BACKEND": "1"}
        done = subprocess.run(
            [sys.executable, "-c", code], cwd=_ROOT, env=env, check=True,
            capture_output=True, text=True,
        )
        self.assertEqual(done.stdout.strip(), "[]")


if __name__ == "__main__":
    unittest.main()
