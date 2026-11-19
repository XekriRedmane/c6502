"""Tests for `passes/slot_coalescing.py`: a zero-page local holding a
copy of a parameter takes the parameter's own slot."""

from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.abi_selection import ZpLayout
from passes.slot_coalescing import apply_slot_coalescing
from passes.slot_sharing import SlotShare
from sim.harness import build_sim

_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_P = asm_ast.Data(name="__zpabi_f__p", offset=0)
_L = asm_ast.Data(name="__local_f__l", offset=0)
_M = asm_ast.Data(name="__local_f__m", offset=0)
_SLOTS = {"__zpabi_f__p": 0x80, "__local_f__l": 0x81, "__local_f__m": 0x82}


def _mov(src, dst, volatile=False):
    return asm_ast.Mov(src=src, dst=dst, is_volatile=volatile)


def _ret(save_a=True):
    return asm_ast.Return(save_a=save_a)


def _run(instrs, shares=()):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )])
    abi = {"f": ZpLayout(slot_symbols=["__zpabi_f__p"], addrs=[0x80],
                         param_registers=[None], pinned=[None])}
    out, written = apply_slot_coalescing(prog, abi, list(shares), _SLOTS)
    return out.top_level[0].instructions, written


class TestSlotCoalescing(unittest.TestCase):

    def _copy_then_shift(self):
        # `l = p; l <<= 1; return l;`
        return [
            _mov(_P, _A), _mov(_A, _L),
            asm_ast.ArithmeticShiftLeft(dst=_L),
            _mov(_L, _A), _ret(),
        ]

    def test_local_takes_the_parameter_slot(self):
        out, written = _run(self._copy_then_shift())
        self.assertEqual(out, [
            _mov(_P, _A),
            asm_ast.ArithmeticShiftLeft(dst=_P),
            _mov(_P, _A), _ret(),
        ])
        self.assertEqual(written, {"f": ["__zpabi_f__p"]})

    def test_parameter_read_after_the_local_changes(self):
        # `l = p; l++; return l + p;` keeps both.
        instrs = [
            _mov(_P, _A), _mov(_A, _L), asm_ast.Inc(dst=_L),
            _mov(_L, _A), asm_ast.ClearCarry(),
            asm_ast.Add(src=_P, dst=_A), _ret(),
        ]
        out, written = _run(list(instrs))
        self.assertEqual(out, instrs)
        self.assertEqual(written, {})

    def test_unrelated_local_left_alone(self):
        # `m` never holds the parameter's value.
        instrs = [
            _mov(asm_ast.Imm(value=3), _A), _mov(_A, _M),
            asm_ast.Inc(dst=_M), _mov(_M, _A), _ret(),
        ]
        out, _ = _run(list(instrs))
        self.assertEqual(out, instrs)

    def test_shared_slot_left_alone(self):
        share = SlotShare(members=(("f", ("__zpabi_f__p",)),
                                   ("g", ("__zpabi_g__q",))))
        instrs = self._copy_then_shift()
        out, _ = _run(list(instrs), shares=[share])
        self.assertEqual(out, instrs)

    def test_volatile_local_left_alone(self):
        instrs = [
            _mov(_P, _A), _mov(_A, _L, volatile=True),
            asm_ast.Dec(dst=_L), _mov(_L, _A, volatile=True), _ret(),
        ]
        out, _ = _run(list(instrs))
        self.assertEqual(out, instrs)

    def test_local_used_as_a_pointer_left_alone(self):
        # `(l),Y` names the local by its address, which a rename of
        # `__local_f__l` wouldn't reach.
        instrs = [
            _mov(_P, _A), _mov(_A, _L),
            _mov(asm_ast.Imm(value=0), asm_ast.Reg(reg=asm_ast.Y())),
            _mov(asm_ast.IndirectZpY(address=0x81), _A), _ret(),
        ]
        out, _ = _run(list(instrs))
        self.assertEqual(out, instrs)

    def test_copy_after_a_branch_goes(self):
        # check_sign's shape: the parameter is loaded before a branch,
        # and the copy is on one side of it. Once the local is the
        # slot, that store writes the slot's own value.
        instrs = [
            _mov(_P, _A),
            asm_ast.Branch(cond=asm_ast.PL(), target=".split"),
            asm_ast.SetCarry(),
            _mov(asm_ast.Imm(value=0), _A), asm_ast.Sub(src=_P, dst=_A),
            _mov(_A, _L),
            asm_ast.Label(name=".join"),
            _mov(_L, _X), _ret(),
            asm_ast.Label(name=".split"),
            _mov(_A, _L),
            asm_ast.Jump(target=".join", reg_args=[]),
        ]
        out, _ = _run(list(instrs))
        self.assertNotIn(_L, [getattr(i, "dst", None) for i in out])
        self.assertEqual(out[-2:], [
            asm_ast.Label(name=".split"),
            asm_ast.Jump(target=".join", reg_args=[]),
        ])


_G = asm_ast.Data(name="__zpabi_g__q", offset=0)
_H = asm_ast.Data(name="__zpabi_h__r", offset=0)


def _run_caller(instrs):
    """`f` calls `g` and `h`, each with one parameter byte. The unit
    defines both, since a slot of a function defined elsewhere might be
    reached by a pointer this unit can't see."""
    callee = [asm_ast.Return(save_a=False)]
    prog = asm_ast.Program(top_level=[
        asm_ast.Function(name="f", is_global=True, params=[],
                         instructions=instrs),
        asm_ast.Function(name="g", is_global=False, params=[],
                         instructions=list(callee)),
        asm_ast.Function(name="h", is_global=False, params=[],
                         instructions=list(callee)),
    ])
    abi = {
        "f": ZpLayout(slot_symbols=["__zpabi_f__p"], addrs=[0x80],
                      param_registers=[None], pinned=[None]),
        "g": ZpLayout(slot_symbols=["__zpabi_g__q"], addrs=[0x90],
                      param_registers=[None], pinned=[None]),
        "h": ZpLayout(slot_symbols=["__zpabi_h__r"], addrs=[0x90],
                      param_registers=[None], pinned=[None]),
    }
    slots = dict(_SLOTS, __zpabi_g__q=0x90, __zpabi_h__r=0x90)
    out, _ = apply_slot_coalescing(prog, abi, [], slots)
    return out.top_level[0].instructions


def _call(name):
    return asm_ast.Call(name=name, reg_args=[], preserves=[])


class TestCalleeSlot(unittest.TestCase):
    """A local copied into a callee's parameter slot for the call."""

    def test_local_computed_in_the_callee_slot(self):
        # `l = m + 1; g(l);`
        instrs = [
            _mov(_M, _A), asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=_A), _mov(_A, _L),
            _mov(_L, _A), _mov(_A, _G), _call("g"), _ret(False),
        ]
        out = _run_caller(instrs)
        self.assertNotIn(_L, [getattr(i, "dst", None) for i in out])
        self.assertIn(_mov(_A, _G), out)

    def test_not_live_across_a_call(self):
        # `l` is still read after the call to h, which may write g's byte.
        instrs = [
            _mov(_M, _A), _mov(_A, _L), _call("h"),
            _mov(_L, _A), _mov(_A, _G), _call("g"), _ret(False),
        ]
        self.assertEqual(_run_caller(list(instrs)), instrs)

    def test_not_live_across_another_callees_argument(self):
        # h's slot shares g's byte here, and the linker could make any
        # two siblings' slots share one.
        instrs = [
            _mov(_M, _A), _mov(_A, _L),
            _mov(asm_ast.Imm(value=5), _A), _mov(_A, _H), _call("h"),
            _mov(_L, _A), _mov(_A, _G), _call("g"), _ret(False),
        ]
        self.assertEqual(_run_caller(list(instrs)), instrs)

    def test_tail_call_reads_the_slot(self):
        instrs = [
            _mov(_M, _A), _mov(_A, _L), _mov(_L, _A), _mov(_A, _G),
            asm_ast.Jump(target="g", reg_args=[]),
        ]
        out = _run_caller(list(instrs))
        self.assertNotIn(_L, [getattr(i, "dst", None) for i in out])
        self.assertEqual(out[-1], asm_ast.Jump(target="g", reg_args=[]))


# `mul` takes the starting product first, which arrives in A and X, so
# that `a` and `b` arrive in their slots, where `a <<= 1` and `b >>= 1`
# shift them in place.
_MULU16 = """
#include <stdint.h>
uint16_t t[6] = {0, 1, 7, 255, 300, 65535};
static uint16_t mul(uint16_t product, uint16_t a, uint16_t b) {
    while (b != 0) {
        if (b & 1)
            product += a;
        a <<= 1;
        b >>= 1;
    }
    return product;
}
static uint16_t div(uint16_t dividend, uint16_t divisor) {
    uint16_t quotient = dividend;
    uint16_t partial = 0;
    for (uint8_t i = 17; i != 0; i--) {
        uint8_t fits = partial >= divisor;
        if (fits)
            partial -= divisor;
        uint8_t out = (quotient & 0x8000) != 0;
        quotient = (quotient << 1) | fits;
        partial = (partial << 1) | out;
    }
    return quotient;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 6; i++)
        for (uint8_t j = 0; j < 6; j++)
            n = (uint16_t)(n * 3u + mul(0, t[i], t[j])
                           + (t[j] ? div(t[i], t[j]) : 0));
    return n;
}
"""


def _expected() -> int:
    t = [0, 1, 7, 255, 300, 65535]
    n = 0
    for a in t:
        for b in t:
            q = a // b if b else 0
            n = (n * 3 + (a * b) + q) & 0xFFFF
    return n


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRuns(unittest.TestCase):

    def test_multiply_and_divide(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(_MULU16, optimize=optimize).run()
                self.assertEqual(r.return_int(), _expected())

    def test_no_copy_of_the_parameters(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(_MULU16), optimize=True)
        body = asm[asm.index("\nmul:"):asm.index("\ndiv:")]
        self.assertNotIn("__local_mul__a", body)
        self.assertIn("ASL   __zpabi_mul__a_0", body)
        self.assertIn("changes=__zpabi_mul__a_0", asm)


if __name__ == "__main__":
    unittest.main()
