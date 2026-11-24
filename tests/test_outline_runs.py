"""passes/outline_runs.py: a run of instructions repeated in a unit goes
in a function of its own, which each place calls.

`TestPass` runs the pass on hand-written asm. `TestPrograms` compiles a
program whose functions set up the same call, checks that the run was
outlined, and runs it in the simulator, compiled on its own and
linked.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

from __future__ import annotations

import shutil
import tempfile
import unittest
from pathlib import Path

from sim.harness import build_sim
from tests.no_inlining import no_inlining

_HAVE_PCPP = shutil.which("pcpp") is not None


_SRC = """
#include <stdint.h>
uint16_t ops[2] = { 1234, 7 };
uint16_t acc;
static uint16_t take(uint16_t a, uint16_t b) { return a * 3 + b; }
void f1(void) { acc += take(ops[0], ops[1]); acc ^= 0x55; }
void f2(void) { acc -= take(ops[0], ops[1]); acc += 9; }
void f3(void) { acc *= take(ops[0], ops[1]); }
int main(void) {
    f1(); f2(); f3(); f1();
    return (int)(acc & 0x7FFF);
}
"""


def _expected():
    acc = 0
    t = (1234 * 3 + 7) & 0xFFFF
    for f in ("f1", "f2", "f3", "f1"):
        if f == "f1":
            acc = ((acc + t) & 0xFFFF) ^ 0x55
        elif f == "f2":
            acc = ((acc - t) + 9) & 0xFFFF
        else:
            acc = (acc * t) & 0xFFFF
    return acc & 0x7FFF


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
@no_inlining()
class TestPrograms(unittest.TestCase):

    def test_outlined_and_runs(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(_SRC), optimize=True)
        self.assertIn("@run0:", asm)
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(_SRC, optimize=optimize).run()
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), _expected())

    def test_linked_program_runs(self):
        from binutils import elf as E
        from sim.linked import build_linked_sim
        from tests.link_helpers import compile_object, link
        with tempfile.TemporaryDirectory() as tmp:
            obj = Path(tmp) / "a.o"
            exe = Path(tmp) / "a.out"
            self.assertEqual(compile_object(_SRC, obj), 0)
            self.assertEqual(link([obj], exe), 0)
            out = build_linked_sim(E.read_file(exe)).run()
            self.assertEqual(out.return_int(), _expected())


if __name__ == "__main__":
    unittest.main()
