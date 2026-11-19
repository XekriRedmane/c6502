"""Tests for `passes.optimization.known_compare_thread`: a jump whose
target tests what the jump's own path decided goes past the test, a
conditional jump that the code before it decided becomes a `Jump` or
goes, and a conditional jump's target block is placed right after the
`Jump` that follows it."""

from __future__ import annotations

import shutil
import unittest

import c99_ast
import tac_ast
from passes.optimization.known_compare_thread import thread_known_compares
from passes.type_checking import LocalAttr, StaticAttr, Symbol, SymbolTable
from passes.type_checking import Tentative
from sim.harness import build_sim
from tests.no_inlining import no_inlining


def _var(name):
    return tac_ast.Var(name=name)


def _uc(v):
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=v))


def _jcmp(op, name, v, target):
    return tac_ast.JumpIfCmp(op=op, src1=_var(name), src2=_uc(v),
                             target=target)


def _eq(name, v, target):
    return _jcmp(tac_ast.Equal(), name, v, target)


def _ne(name, v, target):
    return _jcmp(tac_ast.NotEqual(), name, v, target)


def _label(name):
    return tac_ast.Label(name=name)


def _jump(target):
    return tac_ast.Jump(target=target)


def _copy(v, dst):
    return tac_ast.Copy(src=_uc(v), dst=_var(dst))


def _ret():
    return tac_ast.Ret(val=None)


def _symbols(volatile=()):
    syms = SymbolTable()
    for name in ("x", "y", "r"):
        t = c99_ast.UChar()
        if name in volatile:
            t = c99_ast.Volatile(referenced_type=t)
        syms[name] = Symbol(type=t, attrs=LocalAttr())
    return syms


def _run(instrs, symbols=None):
    fn = tac_ast.Function(name="f", is_global=True, params=[],
                          instructions=list(instrs))
    out = thread_known_compares(fn, symbols or _symbols())
    return out.instructions


# The `if (x == 20 || x == 10) r = x == 20 ? 1 : 2;` shape the
# short-circuit fold leaves.
def _or_then_test():
    return [
        _eq("x", 20, ".s"),
        _eq("x", 10, ".s"),
        _jump(".end"),
        _label(".s"),
        _ne("x", 20, ".else"),
        _label(".then"),
        _copy(1, "r"),
        _jump(".join"),
        _label(".else"),
        _copy(2, "r"),
        _label(".join"),
        _label(".end"),
        _ret(),
    ]


class TestThreading(unittest.TestCase):

    def test_taken_equal_jumps_past_the_test(self):
        # x == 20 on the first jump's edge: the `!= 20` at `.s` isn't
        # taken. x == 10 on the second's: it is.
        out = _run(_or_then_test())
        self.assertEqual(out[0], _eq("x", 20, ".then"))
        self.assertEqual(out[1], _eq("x", 10, ".else"))

    def test_unreached_test_goes_and_target_follows_the_jump(self):
        out = _run(_or_then_test())
        self.assertEqual(out, [
            _eq("x", 20, ".then"),
            _eq("x", 10, ".else"),
            _jump(".end"),
            _label(".else"),
            _copy(2, "r"),
            _jump(".join"),
            _label(".then"),
            _copy(1, "r"),
            _label(".join"),
            _label(".end"),
            _ret(),
        ])

    def test_not_taken_not_equal_decides_the_next_test(self):
        # Falling through `x != 10` leaves x == 10, so `x != 20` is
        # always taken and `x == 20` never is.
        out = _run([
            _ne("x", 10, ".out"),
            _ne("x", 20, ".a"),
            _ret(),
            _label(".a"),
            _ret(),
            _label(".out"),
            _ret(),
        ])
        self.assertEqual(out[1], _jump(".a"))
        out = _run([
            _ne("x", 10, ".out"),
            _eq("x", 20, ".a"),
            _ret(),
            _label(".a"),
            _ret(),
            _label(".out"),
            _ret(),
        ])
        self.assertEqual(out[:2], [_ne("x", 10, ".out"), _ret()])

    def test_known_unequal_decides_a_test(self):
        # Falling through `x == 10` leaves x != 10, so `x != 10` is
        # always taken.
        out = _run([
            _eq("x", 10, ".out"),
            _ne("x", 10, ".a"),
            _ret(),
            _label(".a"),
            _ret(),
            _label(".out"),
            _ret(),
        ])
        self.assertEqual(out[1], _jump(".a"))

    def test_label_with_two_ways_in_drops_facts(self):
        # `.m` is reached by the fall-through, where x == 10, and by
        # `JumpIfTrue(y, .m)`, where x may be anything.
        instrs = [
            tac_ast.JumpIfTrue(condition=_var("y"), target=".m"),
            _ne("x", 10, ".out"),
            _label(".m"),
            _ne("x", 20, ".a"),
            _ret(),
            _label(".a"),
            _ret(),
            _label(".out"),
            _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_jump_to_a_label_threads_only_its_own_edge(self):
        # The first jump to `.t` has x == 20. No fact holds at the second,
        # since the facts are dropped at the label `.m` before it.
        out = _run([
            _eq("x", 20, ".t"),
            _label(".m"),
            tac_ast.JumpIfTrue(condition=_var("y"), target=".t"),
            _ret(),
            _label(".t"),
            _ne("x", 20, ".a"),
            _copy(1, "r"),
            _ret(),
            _label(".a"),
            _ret(),
        ])
        self.assertEqual(out[0], _eq("x", 20, ".f@cthread@0"))
        self.assertEqual(out[2].target, ".t")
        self.assertEqual(out[6], _label(".f@cthread@0"))
        self.assertEqual(out[7], _copy(1, "r"))

    def _copy_before_test(self, read_r_after: bool):
        # `.s` starts with a copy into r, which both arms overwrite,
        # unless the else arm reads it.
        return [
            _eq("x", 20, ".s"),
            _eq("x", 10, ".s"),
            _jump(".end"),
            _label(".s"),
            _copy(0, "r"),
            _ne("x", 20, ".else"),
            _copy(1, "r"),
            _jump(".join"),
            _label(".else"),
            (tac_ast.Copy(src=_var("r"), dst=_var("y")) if read_r_after
             else _copy(2, "r")),
            _label(".join"),
            _label(".end"),
            tac_ast.Ret(val=_var("r")),
        ]

    def test_dead_copy_passed(self):
        out = _run(self._copy_before_test(read_r_after=False))
        self.assertEqual(out[1], _eq("x", 10, ".else"))

    def test_live_copy_stops_the_walk(self):
        # On the second jump's edge the else arm reads r, so the copy
        # has to run.
        out = _run(self._copy_before_test(read_r_after=True))
        self.assertEqual(out[1], _eq("x", 10, ".s"))

    def test_write_in_between_drops_the_fact(self):
        instrs = [
            _ne("x", 10, ".out"),
            _copy(20, "x"),
            _ne("x", 20, ".a"),
            _ret(),
            _label(".a"),
            _ret(),
            _label(".out"),
            _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_write_of_another_name_keeps_the_fact(self):
        out = _run([
            _ne("x", 10, ".out"),
            _copy(20, "y"),
            _ne("x", 20, ".a"),
            _ret(),
            _label(".a"),
            _ret(),
            _label(".out"),
            _ret(),
        ])
        self.assertEqual(out[2], _jump(".a"))

    def test_call_drops_facts(self):
        # A call may write a static x.
        syms = _symbols()
        syms["x"] = Symbol(type=c99_ast.UChar(),
                           attrs=StaticAttr(initial_value=Tentative(),
                                            is_global=True))
        instrs = [
            _ne("x", 10, ".out"),
            tac_ast.FunctionCall(name="g", args=[], dst=None,
                                 hw_stack_save=False, noreturn=False),
            _ne("x", 20, ".a"),
            _ret(),
            _label(".a"),
            _ret(),
            _label(".out"),
            _ret(),
        ]
        self.assertEqual(_run(instrs, syms), instrs)

    def test_volatile_left_alone(self):
        instrs = _or_then_test()
        out = _run(instrs, _symbols(volatile=("x",)))
        self.assertEqual(out, instrs)

    def test_ordering_compare_left_alone(self):
        instrs = [
            _ne("x", 10, ".out"),
            _jcmp(tac_ast.LessThan(), "x", 20, ".a"),
            _ret(),
            _label(".a"),
            _ret(),
            _label(".out"),
            _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_jump_if_false_is_a_test_of_zero(self):
        # Falling through `JumpIfTrue(x)` leaves x == 0.
        out = _run([
            tac_ast.JumpIfTrue(condition=_var("x"), target=".out"),
            _eq("x", 0, ".a"),
            _ret(),
            _label(".a"),
            _ret(),
            _label(".out"),
            _ret(),
        ])
        self.assertEqual(out[1], _jump(".a"))


class TestPlacement(unittest.TestCase):

    def test_block_falling_into_the_join_isnt_moved(self):
        # `.else` already follows the `Jump`.
        instrs = [
            tac_ast.JumpIfTrue(condition=_var("y"), target=".then"),
            _eq("x", 10, ".else"),
            _jump(".end"),
            _label(".else"),
            _copy(2, "r"),
            _jump(".join"),
            _label(".then"),
            _copy(1, "r"),
            _label(".join"),
            _label(".end"),
            _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_target_with_a_jump_out_isnt_moved(self):
        instrs = [
            tac_ast.JumpIfTrue(condition=_var("x"), target=".then"),
            tac_ast.JumpIfTrue(condition=_var("y"), target=".else"),
            _jump(".end"),
            _label(".then"),
            _copy(1, "r"),
            _jump(".join"),
            _label(".else"),
            _copy(2, "r"),
            tac_ast.JumpIfTrue(condition=_var("r"), target=".end"),
            _label(".join"),
            _label(".end"),
            _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)


def _program(test: str, kind: str = "uint8_t") -> str:
    return (
        "#include <stdint.h>\n"
        f"volatile {kind} sink;\n"
        f"static uint8_t f({kind} t) {{\n"
        "    uint8_t r = 0;\n"
        f"    {test}\n"
        "    return r;\n"
        "}\n"
        "int main(void) {\n"
        "    uint16_t n = 0;\n"
        "    for (uint16_t v = 0; v < 600; v++)\n"
        f"        n = (uint16_t)(n * 3u + f(({kind})v));\n"
        "    return n;\n"
        "}\n"
    )


def _expected(test: str, kind: str) -> int:
    n = 0
    bits = 8 if kind == "uint8_t" else 16
    for v in range(600):
        t = v & ((1 << bits) - 1)
        n = (n * 3 + _model(test, t)) & 0xFFFF
    return n


_TESTS = {
    "or_then_test": (
        "if (t == 20 || t == 10) { sink = t; r = t == 20 ? 0x37 : 0x38; }",
        lambda t: (0x37 if t == 20 else 0x38) if t in (20, 10) else 0),
    "or_then_other_test": (
        "if (t == 20 || t == 10) { sink = t; r = t == 10 ? 1 : 2; }",
        lambda t: (1 if t == 10 else 2) if t in (20, 10) else 0),
    "three_way_or": (
        "if (t == 1 || t == 2 || t == 3) r = t == 2 ? 5 : t == 3 ? 6 : 7;",
        lambda t: {1: 7, 2: 5, 3: 6}.get(t, 0)),
    "and_not": (
        "if (t != 4 && t != 9) r = 1; else r = t == 4 ? 2 : 3;",
        lambda t: 1 if t not in (4, 9) else (2 if t == 4 else 3)),
    "changed_between": (
        "if (t == 20 || t == 10) { t = (uint8_t)(t + sink); "
        "r = t == 20 ? 1 : 2; }",
        None),
}


def _model(test, t):
    for body, model in _TESTS.values():
        if body == test:
            return model(t)
    raise KeyError(test)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
@no_inlining()
class TestRuns(unittest.TestCase):

    def test_values(self):
        for name, (body, model) in _TESTS.items():
            if model is None:
                continue
            for kind in ("uint8_t", "uint16_t"):
                for optimize in (False, True):
                    with self.subTest(shape=name, kind=kind,
                                      optimize=optimize):
                        r = build_sim(_program(body, kind),
                                      optimize=optimize).run()
                        self.assertEqual(r.return_int(),
                                         _expected(body, kind))

    def test_value_changed_in_between(self):
        # `sink` is 0 when f reads it here, so t keeps its value, but
        # the store and the read put the test after a write of t.
        body = _TESTS["changed_between"][0]
        src = _program(body).replace(
            "volatile uint8_t sink;", "uint8_t sink;")
        expected = 0
        for v in range(600):
            t = v & 0xFF
            r = (1 if t == 20 else 2) if t in (20, 10) else 0
            expected = (expected * 3 + r) & 0xFFFF
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(src, optimize=optimize).run()
                self.assertEqual(r.return_int(), expected)

    def test_second_compare_goes(self):
        # handle_timers in examples/lode_runner/timers.c: one compare
        # with 20 before the `if` and none in it.
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(_program(
            "if (t == 20 || t == 10) r = t == 20 ? 0x37 : 0x38;")),
            optimize=True)
        start = asm.index("\nf:")
        body = asm[start:asm.index("\nmain:", start)]
        self.assertEqual(body.count("CMP   #$14"), 1)
        self.assertEqual(body.count("JMP"), 0)


if __name__ == "__main__":
    unittest.main()
