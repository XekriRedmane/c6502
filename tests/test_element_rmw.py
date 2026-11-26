"""Tests for the read-modify-write of an array element indexed by X,
`INC t,X` and the rest: the emitters, the peepholes that make them
(`dec_peephole`, `inc_peephole`, `shift_in_a`,
`redundant_load_after_rmw`, `element_in_place`), the IR check, the
passes that had to learn that such an instruction writes an element,
and programs run in both pipelines. The object-file side (encodings,
relaxation to zp,X, and dasm) is in `test_binutils.py`.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""
from __future__ import annotations

import shutil
import unittest

import asm_ast

_HAVE_PCPP = shutil.which("pcpp") is not None

_E = asm_ast.IndexedData(name="tab", offset=0, index=asm_ast.X())
_RTS = asm_ast.Return(save_a=False, save_x=False, save_c=False)


class TestEmit(unittest.TestCase):

    def test_sim_assembler_encodings(self) -> None:
        from sim.assembler import assemble, instruction_size
        ops = [asm_ast.Inc, asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
               asm_ast.LogicalShiftRight, asm_ast.RotateLeft,
               asm_ast.RotateRight]
        instrs = [op(dst=_E) for op in ops] + [_RTS]
        prog = asm_ast.Program(top_level=[
            asm_ast.Function(name="f", is_global=True, params=[],
                             instructions=instrs),
            asm_ast.StaticVariable(name="tab", is_global=False,
                                   init=[asm_ast.ZeroInit(bytes=4)]),
        ])
        out = assemble(prog, origin=0x0800)
        tab = out.symbols["tab"]
        code = bytes(out.image[0x0800:0x0800 + 19])
        self.assertEqual(code, b"".join(
            bytes([op, tab & 0xFF, tab >> 8])
            for op in (0xFE, 0xDE, 0x1E, 0x5E, 0x3E, 0x7E)) + b"\x60")
        self.assertEqual(instruction_size(asm_ast.Inc(dst=_E)), 3)


def _asm(src: str, optimize: bool) -> str:
    from compile import _run_source
    return _run_source("codegen", src, optimize=optimize)


_PROGRAM = """
#include <stdint.h>
uint8_t a[16];
uint8_t out[24];
[[c6502::zeropage]] uint8_t flag;
void dec_unused(uint8_t i) { --a[i]; }
uint8_t dec_used(uint8_t i) { return --a[i]; }
void inc_unused(uint8_t i) { a[i]++; }
uint8_t inc_used(uint8_t i) { return ++a[i]; }
uint8_t post_dec(uint8_t i) { return a[i]--; }
uint8_t post_inc(uint8_t i) { return a[i]++; }
void shl(uint8_t i) { a[i] <<= 1; }
void shr(uint8_t i) { a[i] >>= 1; }
uint8_t shl_used(uint8_t i) { return a[i] <<= 1; }
uint8_t dec_branch(uint8_t i) { if (--a[i] == 0) return 7; return 9; }
uint8_t inc_branch(uint8_t i) { if (++a[i] == 0) return 7; return 9; }
uint8_t timer(uint8_t i) {
    if (a[i] == 0)
        return 1;
    uint8_t t = --a[i];
    if (t == 0) {
        flag = 6;
        return 2;
    }
    flag = 5;
    if (t == 20 || t == 10)
        return t;
    return 3;
}
void timers(uint8_t n) {
    for (uint8_t i = n; (int8_t)i >= 0; i--) {
        if (a[i] == 0)
            continue;
        uint8_t t = --a[i];
        if (t == 0) {
            flag = 1;
            continue;
        }
        flag = 2;
        if (t == 20 || t == 10)
            out[16 + i] = t;
    }
}
int main(void) {
    uint8_t k = 0;
    a[0] = 0;    dec_unused(0);
    a[1] = 1;    out[k++] = dec_used(1);
    a[2] = 255;  inc_unused(2);
    a[3] = 0x7F; out[k++] = inc_used(3);
    a[4] = 0;    out[k++] = post_dec(4);
    a[5] = 255;  out[k++] = post_inc(5);
    a[6] = 0x81; shl(6);
    a[7] = 0x81; shr(7);
    a[8] = 0xC0; out[k++] = shl_used(8);
    a[9] = 1;    out[k++] = dec_branch(9);
    out[k++] = dec_branch(9);
    a[10] = 255; out[k++] = inc_branch(10);
    out[k++] = inc_branch(10);
    a[11] = 0;   out[k++] = timer(11);
    a[12] = 1;   out[k++] = timer(12);
    a[13] = 21;  out[k++] = timer(13);
    a[14] = 11;  out[k++] = timer(14);
    a[15] = 6;   out[k++] = timer(15);
    out[k++] = flag;
    a[0] = 0; a[1] = 1; a[2] = 21; a[3] = 11; a[4] = 200;
    timers(4);
    out[k++] = flag;
    return k;
}
"""

# The results main stores in out[], in order.
_OUT = [0, 0x80, 0, 255, 0x80, 7, 9, 7, 9, 1, 2, 20, 10, 3, 5, 1]


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    """Each operation on an element indexed by X, across 0 and 255,
    with the result used and not, and branched on, in both pipelines.
    `timer` and `timers` have `handle_timers`'s shape."""

    def test_runs(self) -> None:
        from sim.harness import build_sim
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                result = sim.run()
                self.assertFalse(result.timed_out)
                mem = result.memory
                a = list(mem[sim.symbols["a"]:sim.symbols["a"] + 16])
                out = list(mem[sim.symbols["out"]:sim.symbols["out"] + 24])
                self.assertEqual(result.return_int(), len(_OUT))
                self.assertEqual(out[:len(_OUT)], _OUT)
                # The second batch, by timers(4): a[0] stays 0, a[1]
                # runs out, a[2] and a[3] reach 20 and 10, a[4] steps.
                self.assertEqual(a[:5], [0, 0, 20, 10, 199])
                self.assertEqual(out[16:21], [0, 0, 20, 10, 0])
                self.assertEqual(a[5:16], [0, 0x02, 0x40, 0x80, 0xFF, 1,
                                           0, 0, 20, 10, 5])

    def test_element_store_over_a_copy_of_a_pointed_to_byte(self) -> None:
        # `pointer_self_store` took `buf[1]` to hold `p->v` after `buf[i]
        # = 9` and dropped the store back through `p`, so this returned
        # 3 under -O before 19 Nov 2026.
        from sim.harness import build_sim
        src = """
        #include <stdint.h>
        struct s { uint8_t v; uint8_t w; };
        static uint8_t buf[4];
        struct s obj;
        void f(struct s *p, uint8_t i) {
            buf[1] = p->v;
            buf[i] = 9;
            p->v = buf[1];
        }
        int main(void) {
            obj.v = 3;
            f(&obj, 1);
            return obj.v;
        }
        """
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run()
                self.assertEqual(result.return_int(), 9)

    def test_optimized_code_uses_the_indexed_forms(self) -> None:
        text = _asm(_PROGRAM, True)
        for op in ("DEC", "INC", "ASL", "LSR"):
            self.assertRegex(text, rf"(?m)^   {op}   a,X$")
        body = text[text.index("timers:"):text.index("main:")]
        self.assertNotRegex(body, r"STA   a,X")
        self.assertRegex(body, r"DEC   a,X")


if __name__ == "__main__":
    unittest.main()
