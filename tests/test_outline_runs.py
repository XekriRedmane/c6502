"""passes/outline_runs.py: a run of instructions repeated in a unit goes
in a function of its own, which each place calls.

`TestPass` runs the pass on hand-written asm. `TestPrograms` compiles a
program whose functions set up the same call, checks that the run was
outlined, and runs it in the simulator, compiled on its own and
linked."""

from __future__ import annotations

import shutil
import tempfile
import unittest
from pathlib import Path

import asm_ast
from passes.outline_runs import apply_outline_runs
from sim.harness import build_sim

_A = asm_ast.Reg(reg=asm_ast.A())
_HAVE_PCPP = shutil.which("pcpp") is not None


def _setup():
    """`LDA OPS; STA p_0; LDA OPS+1; STA p_1; JSR g`, 11 bytes in zero
    page."""
    return [
        asm_ast.Mov(src=asm_ast.Data(name="OPS", offset=0), dst=_A),
        asm_ast.Mov(src=_A, dst=asm_ast.Data(name="__zpabi_g__p_0", offset=0)),
        asm_ast.Mov(src=asm_ast.Data(name="OPS", offset=1), dst=_A),
        asm_ast.Mov(src=_A, dst=asm_ast.Data(name="__zpabi_g__p_1", offset=0)),
        asm_ast.Call(name="g"),
    ]


_ZP = frozenset({"OPS", "__zpabi_g__p_0", "__zpabi_g__p_1"})


def _fn(name, instrs):
    return asm_ast.Function(name=name, is_global=True, params=[],
                            instructions=instrs)


class TestPass(unittest.TestCase):

    def test_run_in_two_functions_is_called(self):
        tail_f = [asm_ast.Inc(dst=asm_ast.Data(name="x", offset=0)),
                  asm_ast.Return()]
        tail_h = [asm_ast.Dec(dst=asm_ast.Data(name="x", offset=0)),
                  asm_ast.Return()]
        prog = asm_ast.Program(top_level=[
            _fn("f", [*_setup(), *tail_f]), _fn("h", [*_setup(), *tail_h]),
        ])
        out = apply_outline_runs(prog, {}, zp_names=_ZP).top_level
        self.assertEqual([tl.name for tl in out], ["f", "f@run0", "h"])
        # The run ends with `JSR g`, which becomes `JMP g`.
        self.assertEqual(out[1].instructions,
                         [*_setup()[:-1], asm_ast.Jump(target="g")])
        self.assertEqual(out[0].instructions,
                         [asm_ast.Call(name="f@run0"), *tail_f])
        self.assertEqual(out[2].instructions,
                         [asm_ast.Call(name="f@run0"), *tail_h])

    def test_short_run_left_alone(self):
        # 6 bytes at two places would take 6 + 1 + 6.
        run = _setup()[:2] + [asm_ast.Call(name="g")]
        prog = asm_ast.Program(top_level=[
            _fn("f", [*run[1:], asm_ast.Return()]),
            _fn("h", [*run[1:], asm_ast.Return()]),
        ])
        self.assertEqual(apply_outline_runs(prog, {}, zp_names=_ZP), prog)

    def test_branch_ends_a_run(self):
        # A label splits the run into pieces too short to share.
        def with_label(name):
            s = _setup()
            return _fn(name, [*s[:3], asm_ast.Label(name=".l"), *s[3:],
                              asm_ast.Return()])
        prog = asm_ast.Program(top_level=[with_label("f"), with_label("h")])
        self.assertEqual(apply_outline_runs(prog, {}, zp_names=_ZP), prog)

    def test_own_slot_left_alone(self):
        def own(name):
            return _fn(name, [
                *_setup(),
                asm_ast.Mov(src=_A, dst=asm_ast.Data(
                    name=f"__local_{name}__t", offset=0)),
                asm_ast.Return(),
            ])
        out = apply_outline_runs(
            asm_ast.Program(top_level=[own("f"), own("h")]), {}, zp_names=_ZP,
        ).top_level
        for fn in out:
            for ins in fn.instructions:
                if fn.name.endswith("@run0"):
                    self.assertNotIn("__local_", repr(ins))

    def test_short_loop_left_alone(self):
        def looped(name):
            return _fn(name, [
                asm_ast.Label(name=".top"), *_setup(),
                asm_ast.Branch(cond=asm_ast.NE(), target=".top"),
                asm_ast.Return(),
            ])
        prog = asm_ast.Program(top_level=[looped("f"), looped("h")])
        self.assertEqual(apply_outline_runs(prog, {}, zp_names=_ZP), prog)


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
