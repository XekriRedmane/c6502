"""passes/shared_tails.py: code that two functions of a unit end with
alike goes in a function of its own, which each jumps to.

`TestPass` runs the pass on hand-written asm. `TestPrograms` compiles
programs whose functions end alike, checks that a tail was shared, and
runs them in the simulator, compiled on their own and linked, where
the linker puts one function right before the tail and drops its
jump."""

from __future__ import annotations

import shutil
import tempfile
import unittest
from pathlib import Path

import asm_ast
from passes.shared_tails import apply_shared_tails
from sim.harness import build_sim

_A = asm_ast.Reg(reg=asm_ast.A())
_HAVE_PCPP = shutil.which("pcpp") is not None


def _store_tail():
    """`STA v_0; LDA #0; STA v_1; JMP store`, 9 bytes in zero page."""
    return [
        asm_ast.Mov(src=_A, dst=asm_ast.Data(name="__zpabi_store__v_0", offset=0)),
        asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
        asm_ast.Mov(src=_A, dst=asm_ast.Data(name="__zpabi_store__v_1", offset=0)),
        asm_ast.Jump(target="store"),
    ]


def _fn(name, instrs):
    return asm_ast.Function(name=name, is_global=True, params=[],
                            instructions=instrs)


def _load(name):
    return asm_ast.Mov(src=asm_ast.Data(name=name, offset=0), dst=_A)


class TestPass(unittest.TestCase):

    def test_two_functions_share_their_tail(self):
        prog = asm_ast.Program(top_level=[
            _fn("f", [_load("a"), *_store_tail()]),
            _fn("g", [_load("b"), *_store_tail()]),
        ])
        out = apply_shared_tails(prog, {}).top_level
        self.assertEqual([tl.name for tl in out], ["f", "f@tail0", "g"])
        self.assertEqual(out[1].instructions, _store_tail())
        for fn in (out[0], out[2]):
            self.assertEqual(fn.instructions[-1].target, "f@tail0")
            self.assertEqual(len(fn.instructions), 2)

    def test_short_tail_of_two_left_alone(self):
        # `STA v; JMP store`, 5 bytes, would take 5 + 3 + 3.
        tail = [asm_ast.Mov(src=_A, dst=asm_ast.Data(name="v", offset=0)),
                asm_ast.Jump(target="store")]
        prog = asm_ast.Program(top_level=[
            _fn("f", [_load("a"), *tail]), _fn("g", [_load("b"), *tail]),
        ])
        self.assertEqual(apply_shared_tails(prog, {}), prog)

    def test_labels_inside_are_matched_by_position(self):
        # Each ends in a loop on a label of its own name.
        def ends_in_loop(fn, label):
            return _fn(fn, [
                _load("a"),
                asm_ast.Call(name="h"), asm_ast.Call(name="k"),
                asm_ast.Label(name=label), asm_ast.Jump(target=label),
            ])
        prog = asm_ast.Program(top_level=[
            ends_in_loop("f", ".loop@1"), ends_in_loop("g", ".loop@2"),
        ])
        out = apply_shared_tails(prog, {}).top_level
        self.assertEqual(out[1].instructions[-2:], [
            asm_ast.Label(name=".loop@1"), asm_ast.Jump(target=".loop@1"),
        ])

    def test_segment_starts_after_a_label_reached_from_outside(self):
        # `g` branches into the middle of its tail, so the segment
        # starts after `.mid`, which stays in `g` and names the jump.
        tail = _store_tail()
        g = [
            _load("b"), asm_ast.Branch(cond=asm_ast.EQ(), target=".mid"),
            tail[0], asm_ast.Label(name=".mid"), *tail[1:],
        ]
        f = [_load("a"), tail[0], asm_ast.Label(name=".x"), *tail[1:]]
        prog = asm_ast.Program(top_level=[_fn("f", f), _fn("g", g)])
        out = apply_shared_tails(prog, {}).top_level
        self.assertEqual(out[1].instructions, tail[1:])
        self.assertEqual(out[2].instructions[2:], [
            tail[0], asm_ast.Label(name=".mid"),
            asm_ast.Jump(target="f@tail0", reg_args=["A", "X", "Y"]),
        ])

    def test_function_with_a_frame_left_alone(self):
        frame = asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=2,
                                         callee_saved_addrs=[])
        prog = asm_ast.Program(top_level=[
            _fn("f", [frame, _load("a"), *_store_tail()]),
            _fn("g", [_load("b"), *_store_tail()]),
        ])
        self.assertEqual(apply_shared_tails(prog, {}), prog)


_SRC = """
#include <stdint.h>
uint16_t acc;
uint8_t tab[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
void store(uint16_t v) { acc = acc * 3 + v; }
void f1(uint8_t i) { store(tab[i]); }
void f2(uint8_t i) { store((uint8_t)(tab[i] + 1)); }
void f3(uint8_t i) { store((uint8_t)(tab[i] << 1)); }
int main(void) {
    f1(2); f2(5); f3(7); f1(0);
    return acc;
}
"""
_EXPECTED = (((3 * 3 + 7) * 3 + 16) * 3 + 1)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestPrograms(unittest.TestCase):

    def test_shared_and_runs(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(_SRC), optimize=True)
        self.assertIn("@tail0:", asm)
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_SRC, optimize=optimize).run()
                self.assertEqual(result.return_int(), _EXPECTED)

    def test_linked_program_runs(self):
        from sim.linked import build_linked_sim
        from tests.link_helpers import compile_object, link, symbols
        from binutils import elf as E
        with tempfile.TemporaryDirectory() as tmp:
            obj = Path(tmp) / "a.o"
            exe = Path(tmp) / "a.out"
            self.assertEqual(compile_object(_SRC, obj), 0)
            self.assertEqual(link([obj], exe), 0)
            syms = symbols(exe)
            tails = [n for n in syms if "@tail" in n]
            self.assertTrue(tails)
            result = build_linked_sim(E.read_file(exe)).run()
            self.assertEqual(result.return_int(), _EXPECTED)


if __name__ == "__main__":
    unittest.main()
