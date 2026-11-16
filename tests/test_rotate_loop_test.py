"""Tests for `passes.rotate_loop_test`, which moves a loop's test from
its top to its bottom, and drops the entry `JMP` when the test is
known to pass on entry."""

import shutil
import unittest

import asm_ast
from passes.rotate_loop_test import apply_rotate_loop_test
from sim.harness import build_sim


def _X():
    return asm_ast.Reg(reg=asm_ast.X())


def _A():
    return asm_ast.Reg(reg=asm_ast.A())


def _imm(v):
    return asm_ast.Imm(value=v)


def _data(name, off=0):
    return asm_ast.Data(name=name, offset=off)


def _label(name):
    return asm_ast.Label(name=name)


def _run(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    out = apply_rotate_loop_test(asm_ast.Program(top_level=[fn]))
    return out.top_level[0].instructions


def _loop(prefix, test, body=None):
    """`<prefix>; .top: <test>; BEQ .exit; <body>; JMP .top; .exit:
    RTS`."""
    body = body or [asm_ast.Inc(dst=_X())]
    return [
        *prefix,
        _label(".top"), *test,
        asm_ast.Branch(cond=asm_ast.EQ(), target=".exit"),
        *body,
        asm_ast.Jump(target=".top"),
        _label(".exit"),
        asm_ast.Return(),
    ]


_CPX6 = [asm_ast.Compare(left=_X(), right=_imm(6))]


class TestRotation(unittest.TestCase):

    def test_known_entry_drops_jump(self):
        # X is 0 on entry and the test branches only for 6, so the
        # loop is entered at its body.
        out = _run(_loop([asm_ast.Mov(src=_imm(0), dst=_X())], _CPX6))
        self.assertEqual(out, [
            asm_ast.Mov(src=_imm(0), dst=_X()),
            _label(".f@rot@0"),
            asm_ast.Inc(dst=_X()),
            asm_ast.Compare(left=_X(), right=_imm(6)),
            asm_ast.Branch(cond=asm_ast.NE(), target=".f@rot@0"),
            _label(".exit"),
            asm_ast.Return(),
        ])

    def test_unknown_entry_jumps_to_test(self):
        out = _run(_loop(
            [asm_ast.Mov(src=_data("n"), dst=_X())], _CPX6,
        ))
        self.assertEqual(out, [
            asm_ast.Mov(src=_data("n"), dst=_X()),
            asm_ast.Jump(target=".top"),
            _label(".f@rot@0"),
            asm_ast.Inc(dst=_X()),
            _label(".top"),
            asm_ast.Compare(left=_X(), right=_imm(6)),
            asm_ast.Branch(cond=asm_ast.NE(), target=".f@rot@0"),
            _label(".exit"),
            asm_ast.Return(),
        ])

    def test_loop_at_function_start_jumps_to_test(self):
        # The function is entered at `.top`, with X unknown. Without
        # the entry JMP the first pass would skip the test (Zork's
        # A_mod_3 printed words in the wrong places that way).
        out = _run(_loop([], _CPX6))
        self.assertEqual(out[0], asm_ast.Jump(target=".top"))

    def test_entry_value_from_memory(self):
        # `LDA #$11; STA i; .top: LDA i; BEQ` passes on entry.
        out = _run(_loop(
            [asm_ast.Mov(src=_imm(0x11), dst=_A()),
             asm_ast.Mov(src=_A(), dst=_data("i"))],
            [asm_ast.Mov(src=_data("i"), dst=_A())],
            body=[asm_ast.Dec(dst=_data("i"))],
        ))
        self.assertNotIn(asm_ast.Jump(target=".top"), out)

    def test_entry_value_changed_by_a_call(self):
        out = _run(_loop(
            [asm_ast.Mov(src=_imm(0), dst=_X()), asm_ast.Call(name="g")],
            _CPX6,
        ))
        self.assertEqual(out[2], asm_ast.Jump(target=".top"))

    def test_entry_that_fails_keeps_jump(self):
        # X is 6, so the first test exits.
        out = _run(_loop([asm_ast.Mov(src=_imm(6), dst=_X())], _CPX6))
        self.assertEqual(out[1], asm_ast.Jump(target=".top"))

    def test_continue_still_reaches_test(self):
        # A `continue` jumps to `.top`, which moves with the test.
        instrs = _loop(
            [asm_ast.Mov(src=_imm(0), dst=_X())], _CPX6,
            body=[
                asm_ast.Inc(dst=_X()),
                asm_ast.Branch(cond=asm_ast.MI(), target=".top"),
                asm_ast.Inc(dst=_X()),
            ],
        )
        out = _run(instrs)
        self.assertIn(_label(".top"), out)
        top = out.index(_label(".top"))
        self.assertEqual(out[top + 1], asm_ast.Compare(left=_X(), right=_imm(6)))

    def test_exit_elsewhere_left_alone(self):
        # The test branches to `.out`, which isn't after the JMP back.
        instrs = [
            _label(".top"), *_CPX6,
            asm_ast.Branch(cond=asm_ast.EQ(), target=".out"),
            asm_ast.Inc(dst=_X()),
            asm_ast.Jump(target=".top"),
            _label(".exit"),
            asm_ast.Return(),
            _label(".out"),
            asm_ast.Return(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_long_body_left_alone(self):
        body = [asm_ast.Mov(src=_A(), dst=_data("t", k)) for k in range(40)]
        instrs = _loop([asm_ast.Mov(src=_imm(0), dst=_X())], _CPX6, body)
        self.assertEqual(_run(instrs), instrs)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRotatedLoopsRun(unittest.TestCase):
    """Loops whose test the pass moves, run in both pipelines."""

    _PROGRAM = """
    #include <stdint.h>
    uint8_t buf[8] = {5, 6, 7, 0x8D, 1, 2, 3, 4};
    uint8_t zero = 0;
    static uint8_t a_mod_3(register uint8_t a) {
        while (a >= 3) a -= 3;
        return a;
    }
    static uint8_t count(void) {
        uint8_t c = 0;
        while (buf[c] != 0x8D) c++;
        return c;
    }
    static uint8_t sum_skip(void) {
        uint8_t s = 0;
        for (uint8_t i = 0; i != 8; i++) {
            if (i == 2) continue;
            s += buf[i];
        }
        return s;
    }
    static uint8_t none(void) {
        uint8_t s = 1;
        for (uint8_t i = 0; i != zero; i++) s += i;
        return s;
    }
    int main(void) {
        for (uint8_t i = 0; i != 3; i++) buf[4 + i] = i;
        return a_mod_3(14) + 10 * count() + 100 * none()
            + 1000 * (sum_skip() & 0x0F);
    }
    """

    def test_values(self):
        # a_mod_3(14) = 2, count() = 3, none() = 1, and sum_skip() is
        # 5 + 6 + 0x8D + 0 + 1 + 2 + 4 = 159, 0x9F, whose low four
        # bits are 15.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(self._PROGRAM, optimize=optimize).run(
                    max_cycles=5_000_000,
                )
                self.assertFalse(out.timed_out)
                self.assertEqual(out.return_int(), 2 + 30 + 100 + 15000)


if __name__ == "__main__":
    unittest.main()
