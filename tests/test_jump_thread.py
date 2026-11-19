"""Tests for `passes.optimization.jump_thread`.

The pass sends an edge into a join of a `Phi` and a test of it straight
to the successor the test picks for the constant that the edge brings.
The unit tests run it on SSA functions built by hand, and the
simulator tests check programs whose inlined functions return a
constant on some paths, through both outcomes, in both pipelines.
"""
from __future__ import annotations

import unittest

import tac_ast
from passes.optimization.framework import PassContext
from passes.optimization.jump_thread import thread_constant_jumps
from sim.harness import build_sim


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _uc(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=v))


def _fn(*instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name="f", is_global=True, params=[], instructions=list(instrs),
    )


def _run(fn: tac_ast.Function, ssa: set[str]) -> tac_ast.Function:
    return thread_constant_jumps(fn, PassContext(ssa_dsts=ssa))


def _labels(fn: tac_ast.Function) -> list[str]:
    return [i.name for i in fn.instructions if isinstance(i, tac_ast.Label)]


class TestThreading(unittest.TestCase):

    def _inlined_find(self, second_source: tac_ast.Type_val):
        """The shape an inlined `return true;` in a loop and `return
        false;` after it leave, tested by the caller with JumpIfFalse.
        `second_source` is what the loop's exit brings."""
        return _fn(
            tac_ast.Label(name=".entry"),
            tac_ast.JumpIfTrue(condition=_var("c"), target=".found"),
            tac_ast.Label(name=".loop"),
            tac_ast.JumpIfTrue(condition=_var("d"), target=".loop"),
            tac_ast.Label(name=".break"),
            tac_ast.Jump(target=".end"),
            tac_ast.Label(name=".found"),
            tac_ast.Jump(target=".end"),
            tac_ast.Label(name=".end"),
            tac_ast.Phi(dst=_var("t.1"), args=[
                tac_ast.PhiArg(pred_label=".break", source=second_source),
                tac_ast.PhiArg(pred_label=".found", source=_uc(1)),
            ]),
            tac_ast.JumpIfFalse(condition=_var("t.1"), target=".if_end"),
            tac_ast.Ret(val=_uc(7)),
            tac_ast.Label(name=".if_end"),
            tac_ast.Ret(val=_uc(9)),
        )

    def test_both_constants_thread(self):
        out = _run(self._inlined_find(_uc(0)), {"t.1"})
        ins = out.instructions
        # The loop's exit goes to `.if_end` (0 fails the test). The
        # found path goes to the `Ret` after the test, under a new
        # label, and since `.found` only passes the edge on, the jump
        # into it goes there instead.
        brk = ins.index(tac_ast.Label(name=".break"))
        self.assertEqual(ins[brk + 1], tac_ast.Jump(target=".if_end"))
        ret = ins.index(tac_ast.Ret(val=_uc(7)))
        new = ins[ret - 1]
        self.assertIsInstance(new, tac_ast.Label)
        self.assertEqual(
            ins[1], tac_ast.JumpIfTrue(condition=_var("c"), target=new.name),
        )
        # The join's Phi has no arguments left, so UCE drops it.
        end = ins.index(tac_ast.Label(name=".end"))
        self.assertEqual(ins[end + 1].args, [])

    def test_variable_argument_stays(self):
        out = _run(self._inlined_find(_var("x.1")), {"t.1", "x.1"})
        end = out.instructions.index(tac_ast.Label(name=".end"))
        phi = out.instructions[end + 1]
        self.assertEqual(
            phi.args, [tac_ast.PhiArg(pred_label=".break", source=_var("x.1"))],
        )

    def test_second_reader_blocks(self):
        fn = self._inlined_find(_uc(0))
        fn.instructions.insert(-1, tac_ast.Copy(src=_var("t.1"), dst=_var("g")))
        self.assertIs(_run(fn, {"t.1"}), fn)

    def test_name_not_ssa_blocks(self):
        fn = self._inlined_find(_uc(0))
        self.assertIs(_run(fn, set()), fn)

    def test_empty_block_reached_by_jumps(self):
        """`a && b` tested: the empty `.and_false` block is reached
        only by jumps, which go straight to the test's target."""
        fn = _fn(
            tac_ast.Label(name=".entry"),
            tac_ast.JumpIfFalse(condition=_var("a"), target=".and_false"),
            tac_ast.JumpIfFalse(condition=_var("b"), target=".and_false"),
            tac_ast.Label(name=".true"),
            tac_ast.Jump(target=".and_end"),
            tac_ast.Label(name=".and_false"),
            tac_ast.Label(name=".and_end"),
            tac_ast.Phi(dst=_var("t.1"), args=[
                tac_ast.PhiArg(pred_label=".true", source=_uc(1)),
                tac_ast.PhiArg(pred_label=".and_false", source=_uc(0)),
            ]),
            tac_ast.JumpIfFalse(condition=_var("t.1"), target=".if_end"),
            tac_ast.Ret(val=_uc(1)),
            tac_ast.Label(name=".if_end"),
            tac_ast.Ret(val=_uc(2)),
        )
        out = _run(fn, {"t.1"})
        jumps = [i for i in out.instructions
                 if isinstance(i, tac_ast.JumpIfFalse)
                 and i.condition in (_var("a"), _var("b"))]
        self.assertEqual([j.target for j in jumps], [".if_end", ".if_end"])

    def test_fall_through_successor(self):
        """0 fails `JumpIfTrue`, so the edge from `.zero` goes to the
        block after the test, which has no Phi, and the Phi at `.exit`
        keeps its arguments."""
        fn = _fn(
            tac_ast.Label(name=".entry"),
            tac_ast.JumpIfTrue(condition=_var("c"), target=".one"),
            tac_ast.Label(name=".zero"),
            tac_ast.Jump(target=".end"),
            tac_ast.Label(name=".one"),
            tac_ast.Jump(target=".end"),
            tac_ast.Label(name=".end"),
            tac_ast.Phi(dst=_var("t.1"), args=[
                tac_ast.PhiArg(pred_label=".zero", source=_uc(0)),
                tac_ast.PhiArg(pred_label=".one", source=_var("y.1")),
            ]),
            tac_ast.JumpIfTrue(condition=_var("t.1"), target=".exit"),
            tac_ast.Label(name=".mid"),
            tac_ast.Jump(target=".exit"),
            tac_ast.Label(name=".exit"),
            tac_ast.Phi(dst=_var("r.1"), args=[
                tac_ast.PhiArg(pred_label=".end", source=_var("v.1")),
                tac_ast.PhiArg(pred_label=".mid", source=_var("w.1")),
            ]),
            tac_ast.Ret(val=_var("r.1")),
        )
        out = _run(fn, {"t.1", "r.1", "y.1", "v.1", "w.1"})
        ins = out.instructions
        zero = ins.index(tac_ast.Label(name=".zero"))
        self.assertEqual(ins[zero + 1], tac_ast.Jump(target=".mid"))
        exit_phi = [i for i in ins if isinstance(i, tac_ast.Phi)
                    and i.dst == _var("r.1")]
        self.assertEqual(exit_phi[0].args, [
            tac_ast.PhiArg(pred_label=".end", source=_var("v.1")),
            tac_ast.PhiArg(pred_label=".mid", source=_var("w.1")),
        ])
        # The edge from `.one` stays, since `y.1` isn't a constant.
        self.assertIn(".end", _labels(out))

    def test_threaded_into_phi_target(self):
        """A Phi at the target that took a value from the join takes
        the same value from the block whose edge now goes there."""
        fn = _fn(
            tac_ast.Label(name=".entry"),
            tac_ast.JumpIfTrue(condition=_var("c"), target=".one"),
            tac_ast.Label(name=".zero"),
            tac_ast.Jump(target=".end"),
            tac_ast.Label(name=".one"),
            tac_ast.Jump(target=".end"),
            tac_ast.Label(name=".end"),
            tac_ast.Phi(dst=_var("t.1"), args=[
                tac_ast.PhiArg(pred_label=".zero", source=_var("y.1")),
                tac_ast.PhiArg(pred_label=".one", source=_uc(5)),
            ]),
            tac_ast.JumpIfTrue(condition=_var("t.1"), target=".exit"),
            tac_ast.Label(name=".mid"),
            tac_ast.Jump(target=".exit"),
            tac_ast.Label(name=".exit"),
            tac_ast.Phi(dst=_var("r.1"), args=[
                tac_ast.PhiArg(pred_label=".end", source=_var("v.1")),
                tac_ast.PhiArg(pred_label=".mid", source=_var("w.1")),
            ]),
            tac_ast.Ret(val=_var("r.1")),
        )
        out = _run(fn, {"t.1", "r.1", "y.1", "v.1", "w.1"})
        ins = out.instructions
        # `.one` only passes the edge on, so `.entry`'s jump into it
        # goes to `.exit`, and `.exit`'s Phi names `.entry`.
        self.assertEqual(
            ins[1], tac_ast.JumpIfTrue(condition=_var("c"), target=".exit"),
        )
        exit_phi = [i for i in ins if isinstance(i, tac_ast.Phi)
                    and i.dst == _var("r.1")][0]
        self.assertIn(
            tac_ast.PhiArg(pred_label=".entry", source=_var("v.1")),
            exit_phi.args,
        )

    def test_entering_loop_body_blocks(self):
        """The edge would enter a loop's body past its header, which is
        the join itself here, so it stays."""
        fn = _fn(
            tac_ast.Label(name=".entry"),
            tac_ast.Jump(target=".head"),
            tac_ast.Label(name=".head"),
            tac_ast.Phi(dst=_var("t.1"), args=[
                tac_ast.PhiArg(pred_label=".entry", source=_uc(1)),
                tac_ast.PhiArg(pred_label=".body", source=_var("u.1")),
            ]),
            tac_ast.JumpIfFalse(condition=_var("t.1"), target=".exit"),
            tac_ast.Label(name=".body"),
            tac_ast.Copy(src=_var("c"), dst=_var("u.1")),
            tac_ast.Jump(target=".head"),
            tac_ast.Label(name=".exit"),
            tac_ast.Ret(val=None),
        )
        self.assertIs(_run(fn, {"t.1", "u.1"}), fn)


class TestPrograms(unittest.TestCase):
    """`find` is inlined into `f`, which tests its result. Each case
    runs both the `return true;` and the `return false;` paths."""

    _SOURCE = """
        #include <stdint.h>
        #include <stdbool.h>
        uint8_t a[8], n, hits;
        static bool find(uint8_t v) {
            uint8_t i = n;
            do { if (a[i] == v) { hits++; return true; } } while (--i != 0);
            return false;
        }
        uint8_t f(uint8_t v) { if (find(v)) return 10; hits += 2; return 20; }
        int main(void) {
            n = 7;
            for (uint8_t k = 0; k < 8; k++) a[k] = k * 3;
            uint8_t r = 0;
            r += f(%d);
            return r * 256 + hits;
        }
    """

    def test_found_and_not_found(self):
        # 9 is a[3]; 4 isn't in the array; a[0] = 0 is never compared,
        # since the loop stops when i reaches 0; 21 is a[7], the first
        # compared.
        for v, expected in ((9, 10 * 256 + 1), (4, 20 * 256 + 2),
                            (0, 20 * 256 + 2), (21, 10 * 256 + 1)):
            for optimize in (False, True):
                with self.subTest(v=v, optimize=optimize):
                    result = build_sim(
                        self._SOURCE % v, optimize=optimize,
                    ).run()
                    self.assertFalse(result.timed_out)
                    self.assertEqual(result.return_int(), expected)

    def test_and_or_results(self):
        src = """
            #include <stdint.h>
            uint8_t x, y, z;
            static uint8_t pick(void) {
                if (x == 0 && (y != 0 || z != 0)) return 3;
                if (x == 5 || y == 6) return 4;
                return 5;
            }
            int main(void) {
                int r = 0;
                static const uint8_t xs[] = {0, 0, 0, 5, 1};
                static const uint8_t ys[] = {0, 1, 0, 0, 6};
                static const uint8_t zs[] = {0, 0, 1, 0, 0};
                for (uint8_t k = 0; k < 5; k++) {
                    x = xs[k]; y = ys[k]; z = zs[k];
                    r = r * 8 + pick();
                }
                return r;
            }
        """
        # x, y, z = 0, 0, 0 gives 5; 0, 1, 0 and 0, 0, 1 give 3; and
        # 5, 0, 0 and 1, 6, 0 give 4. One octal digit each.
        expected = 0o53344
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run()
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), expected)


if __name__ == "__main__":
    unittest.main()
