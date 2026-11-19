"""Tests for `passes/ret_slot_coalescing.py`: a local pair holding a
copy of `F@ret` becomes `F@ret`."""

from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.ret_slot_coalescing import apply_ret_slot_coalescing
from sim.harness import build_sim

_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_Y = asm_ast.Reg(reg=asm_ast.Y())
_R0 = asm_ast.Data(name="f@ret", offset=0)
_R1 = asm_ast.Data(name="f@ret", offset=1)
_L0 = asm_ast.Data(name="__local_g__p_0", offset=0)
_L1 = asm_ast.Data(name="__local_g__p_1", offset=0)
_SLOTS = {"__local_g__p_0": 0x80, "__local_g__p_1": 0x81}
_ZP = {"f@ret": 0x40}


def _mov(src, dst):
    return asm_ast.Mov(src=src, dst=dst, is_volatile=False)


def _call(name, args=()):
    return asm_ast.Call(name=name, reg_args=list(args), preserves=[])


def _fn(name, instrs, is_global=False):
    return asm_ast.Function(name=name, is_global=is_global, params=[],
                            instructions=instrs)


_F = _fn("f", [_mov(_A, _R0), _mov(_X, _R1),
               asm_ast.Return(save_a=False)])
def _walk(between=()):
    """`p = f(p)` in a loop until `*p` is 0, with `between` after the
    copy and before the test."""
    return [
        _mov(_A, _L0), _mov(_X, _L1),
        asm_ast.Jump(target=".test", reg_args=[]),
        asm_ast.Label(name=".loop"),
        _mov(_L1, _X), _mov(_L0, _A), _call("f", "AX"),
        _mov(_R0, _A), _mov(_A, _L0), _mov(_R1, _A), _mov(_A, _L1),
        *between,
        asm_ast.Label(name=".test"),
        _mov(asm_ast.Imm(value=0), _Y),
        _mov(asm_ast.IndirectZpY(address=0x80), _A),
        asm_ast.Branch(cond=asm_ast.NE(), target=".loop"),
        asm_ast.Return(save_a=False),
    ]


def _run(g_instrs, *, others=(), g_global=True):
    ret = asm_ast.StaticVariable(name="f@ret", is_global=False, init=[])
    prog = asm_ast.Program(top_level=[
        _F, _fn("g", g_instrs, g_global), *others, ret,
    ])
    out = apply_ret_slot_coalescing(
        prog, slot_symbols=_SLOTS, zp_addrs=_ZP,
        eligible={"g"} | {o.name for o in others},
    )
    return next(tl for tl in out.top_level
                if getattr(tl, "name", None) == "g").instructions


def _names(instrs) -> set[str]:
    from passes.asm_aliasing import _instruction_operands
    return {op.name for ins in instrs for op in _instruction_operands(ins)
            if isinstance(op, asm_ast.Data)}


class TestRetSlotCoalescing(unittest.TestCase):

    def test_pair_becomes_the_result(self):
        out = _run(_walk())
        self.assertNotIn("__local_g__p_0", _names(out))
        self.assertIn(_mov(_A, _R0), out)
        self.assertIn(_mov(asm_ast.IndirectZpY(address=0x40), _A), out)

    def test_live_across_a_call_of_f_kept(self):
        # The pair is still read after a second call of f.
        instrs = _walk([_call("f"), _mov(_L0, asm_ast.Data(name="g0",
                                                           offset=0))])
        self.assertEqual(_run(list(instrs)), instrs)

    def test_live_across_a_call_out_of_the_unit(self):
        # `h` is global and calls f, and doesn't lead to g, so `ext`
        # might call it while g's pair is in f@ret.
        instrs = _walk([_call("ext")])
        h = _fn("h", [_call("f"), asm_ast.Return(save_a=False)],
                is_global=True)
        self.assertEqual(_run(list(instrs), others=[h]), instrs)

    def test_call_out_of_the_unit_with_every_entry_leading_to_g(self):
        # `h` calls f too, but it calls g, so `ext` calling h from g
        # would be a cycle, which the linker rejects.
        instrs = _walk([_call("ext")])
        h = _fn("h", [_call("f"), _call("g"),
                      asm_ast.Return(save_a=False)], is_global=True)
        self.assertNotIn("__local_g__p_0",
                         _names(_run(list(instrs), others=[h])))

    def test_function_that_does_not_call_f_left_alone(self):
        instrs = [_mov(_R0, _A), _mov(_A, _L0), _mov(_R1, _A),
                  _mov(_A, _L1), asm_ast.Return(save_a=False)]
        self.assertEqual(_run(list(instrs)), instrs)


_SRC = """
#include <stdint.h>
static const uint8_t text[] = {2, 'a', 'b', 1, 'c', 0, 3, 'd', 'e', 'f', 0};
uint16_t total;
static const uint8_t *next(const uint8_t *s) { return s + s[0] + 1; }
static void add(uint8_t c) { total = total * 3 + c; }
static const uint8_t *group(uint8_t index) {
    const uint8_t *p = text;
    for (; index != 0; index--) {
        while (*p != 0)
            p = next(p);
        p++;
    }
    return p;
}
static void walk(const uint8_t *p) {
    while (*p != 0) {
        add(*p);
        add(p[1]);
        p = next(p);
        if (*p == 0)
            add(0xFF);
    }
}
uint16_t main(void) {
    walk(group(0));
    walk(group(1));
    return total;
}
"""


def _expected() -> int:
    total = 0

    def add(c):
        nonlocal total
        total = (total * 3 + c) & 0xFFFF
    text = [2, ord('a'), ord('b'), 1, ord('c'), 0, 3, ord('d'), ord('e'),
            ord('f'), 0]
    for start in (0, 6):
        p = start
        while text[p] != 0:
            add(text[p])
            add(text[p + 1])
            p = p + text[p] + 1
            if text[p] == 0:
                add(0xFF)
    return total


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRuns(unittest.TestCase):

    def test_values(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(_SRC, optimize=optimize).run()
                self.assertEqual(r.return_int(), _expected())

    def test_no_copy_of_the_result(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(_SRC), optimize=True)
        body = asm[asm.index("\ngroup:"):asm.index("\nmain:")]
        self.assertNotIn("LDA   next@ret\n   STA   __local", body)
        self.assertIn("(next@ret),Y", body)


if __name__ == "__main__":
    unittest.main()
