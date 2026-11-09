"""Tests for the carry-INC peephole (`passes/carry_inc.py`) and the
carry-byte pairs in move coalescing that set it up.

The peephole rewrites an in-place carry byte `LDA M; ADC #0; STA M`
to `BCC skip; INC M; skip:`, and a borrow byte `LDA M; SBC #0; STA
M` to `BCS skip; DEC M; skip:`, when A and every flag are dead after
the store. Coalescing puts both ends of such a byte in one slot,
including when the source is one byte of a multi-byte shift temp.

The sim tests run each add and subtract across the values where the
carry or borrow does and doesn't happen, since the two branches of
the rewrite are different code paths.
"""
from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.carry_inc import apply_carry_inc_peephole
from passes.optimization.interference import (
    InterferenceGraph,
    InterferenceNode,
)
from passes.optimization_asm.coalescing import coalesce_moves
from sim.harness import build_sim

_A = asm_ast.Reg(reg=asm_ast.A())


def _zp(addr: int) -> asm_ast.ZP:
    return asm_ast.ZP(address=addr, offset=0)


def _carry_byte(src, dst, *, sub: bool = False) -> list:
    op = asm_ast.Sub if sub else asm_ast.Add
    return [
        asm_ast.Mov(src=src, dst=_A, is_volatile=False),
        op(src=asm_ast.Imm(value=0), dst=_A),
        asm_ast.Mov(src=_A, dst=dst, is_volatile=False),
    ]


def _low_byte(m) -> list:
    """`LDA m; CLC; ADC #5; STA m`, the low byte that sets C."""
    return [
        asm_ast.Mov(src=m, dst=_A, is_volatile=False),
        asm_ast.ClearCarry(),
        asm_ast.Add(src=asm_ast.Imm(value=5), dst=_A),
        asm_ast.Mov(src=_A, dst=m, is_volatile=False),
    ]


def _ret() -> asm_ast.Return:
    return asm_ast.Return(save_a=False, save_x=False)


def _run(instrs: list) -> list:
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    prog = apply_carry_inc_peephole(asm_ast.Program(top_level=[fn]))
    return prog.top_level[0].instructions


class TestCarryIncUnit(unittest.TestCase):

    def test_in_place_add_becomes_bcc_inc(self) -> None:
        lo, hi = _zp(0x80), _zp(0x81)
        out = _run(_low_byte(lo) + _carry_byte(hi, hi) + [_ret()])
        self.assertEqual(out[4:7], [
            asm_ast.Branch(cond=asm_ast.CC(), target=".carry_inc@0"),
            asm_ast.Inc(dst=hi),
            asm_ast.Label(name=".carry_inc@0"),
        ])

    def test_in_place_subtract_becomes_bcs_dec(self) -> None:
        hi = asm_ast.Data(name="g", offset=1)
        out = _run(_carry_byte(hi, hi, sub=True) + [_ret()])
        self.assertEqual(out[:3], [
            asm_ast.Branch(cond=asm_ast.CS(), target=".carry_inc@0"),
            asm_ast.Dec(dst=hi),
            asm_ast.Label(name=".carry_inc@0"),
        ])

    def test_different_destination_unchanged(self) -> None:
        instrs = _carry_byte(_zp(0x81), _zp(0x82)) + [_ret()]
        self.assertEqual(_run(instrs), instrs)

    def test_a_read_after_unchanged(self) -> None:
        # A holds the new high byte, and the STA after reads it.
        hi = _zp(0x81)
        instrs = _carry_byte(hi, hi) + [
            asm_ast.Mov(src=_A, dst=_zp(0x90), is_volatile=False),
            _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_carry_read_after_unchanged(self) -> None:
        # A third byte continues the chain into a different
        # destination and reads the carry out of this one, which the
        # INC form doesn't produce.
        hi = _zp(0x81)
        instrs = _carry_byte(hi, hi) + _carry_byte(_zp(0x82), _zp(0x83)) + [
            _ret(),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_add_run_becomes_inc_chain(self) -> None:
        b1, b2, b3 = _zp(0x81), _zp(0x82), _zp(0x83)
        instrs = (
            _carry_byte(b1, b1) + _carry_byte(b2, b2)
            + _carry_byte(b3, b3) + [_ret()]
        )
        done = ".carry_inc@0"
        self.assertEqual(_run(instrs), [
            asm_ast.Branch(cond=asm_ast.CC(), target=done),
            asm_ast.Inc(dst=b1),
            asm_ast.Branch(cond=asm_ast.NE(), target=done),
            asm_ast.Inc(dst=b2),
            asm_ast.Branch(cond=asm_ast.NE(), target=done),
            asm_ast.Inc(dst=b3),
            asm_ast.Label(name=done),
            _ret(),
        ])

    def test_subtract_run_rewrites_top_byte_only(self) -> None:
        b1, b2 = _zp(0x81), _zp(0x82)
        instrs = (
            _carry_byte(b1, b1, sub=True) + _carry_byte(b2, b2, sub=True)
            + [_ret()]
        )
        out = _run(instrs)
        self.assertEqual(out[:3], instrs[:3])
        self.assertEqual(out[3:6], [
            asm_ast.Branch(cond=asm_ast.CS(), target=".carry_inc@0"),
            asm_ast.Dec(dst=b2),
            asm_ast.Label(name=".carry_inc@0"),
        ])

    def test_frame_operand_unchanged(self) -> None:
        # INC has no (zp),Y mode.
        m = asm_ast.Frame(offset=3)
        instrs = _carry_byte(m, m) + [_ret()]
        self.assertEqual(_run(instrs), instrs)

    def test_volatile_unchanged(self) -> None:
        hi = _zp(0x81)
        instrs = _carry_byte(hi, hi) + [_ret()]
        instrs[2] = asm_ast.Mov(src=_A, dst=hi, is_volatile=True)
        self.assertEqual(_run(instrs), instrs)

    def test_label_numbering_skips_existing(self) -> None:
        hi = _zp(0x81)
        instrs = [asm_ast.Label(name=".carry_inc@4")] + _carry_byte(
            hi, hi,
        ) + [_ret()]
        out = _run(instrs)
        self.assertEqual(out[3], asm_ast.Label(name=".carry_inc@5"))

    def test_labels_distinct_across_functions(self) -> None:
        # The sim's assembler keeps one label table for the program.
        hi = _zp(0x81)
        fns = [
            asm_ast.Function(
                name=n, is_global=True, params=[],
                instructions=_carry_byte(hi, hi) + [_ret()],
            )
            for n in ("f", "g")
        ]
        prog = apply_carry_inc_peephole(asm_ast.Program(top_level=fns))
        labels = [
            ins.name for fn in prog.top_level for ins in fn.instructions
            if isinstance(ins, asm_ast.Label)
        ]
        self.assertEqual(labels, [".carry_inc@0", ".carry_inc@1"])


def _node(name: str, width: int = 1) -> InterferenceNode:
    return InterferenceNode(name=name, width=width, lives_across_call=False)


class TestCarryByteCoalescing(unittest.TestCase):

    def _fn(self, instrs):
        return asm_ast.Function(
            name="f", is_global=True, params=[], instructions=instrs,
        )

    def test_byte_of_multibyte_source(self) -> None:
        # `t` is a 2-byte shift temp; `s` is the sum's high byte.
        instrs = _carry_byte(
            asm_ast.Pseudo(name="t", offset=1),
            asm_ast.Pseudo(name="s", offset=0),
        )
        g = InterferenceGraph()
        g.nodes["t"] = _node("t", 2)
        g.nodes["s"] = _node("s")
        result = coalesce_moves(self._fn(instrs), g)
        self.assertEqual(result.resolve("s"), "t")
        self.assertEqual(result.offset_of("s"), 1)
        self.assertNotIn("s", g.nodes)

    def test_both_single_bytes(self) -> None:
        instrs = _carry_byte(
            asm_ast.Pseudo(name="a", offset=0),
            asm_ast.Pseudo(name="b", offset=0),
            sub=True,
        )
        g = InterferenceGraph()
        g.nodes["a"] = _node("a")
        g.nodes["b"] = _node("b")
        result = coalesce_moves(self._fn(instrs), g)
        self.assertEqual(result.resolve("a"), result.resolve("b"))
        self.assertEqual(result.offset_of("a"), 0)
        self.assertEqual(result.offset_of("b"), 0)

    def test_interfering_not_merged(self) -> None:
        instrs = _carry_byte(
            asm_ast.Pseudo(name="t", offset=1),
            asm_ast.Pseudo(name="s", offset=0),
        )
        g = InterferenceGraph()
        g.nodes["t"] = _node("t", 2)
        g.nodes["s"] = _node("s")
        g.add_edge("t", "s")
        result = coalesce_moves(self._fn(instrs), g)
        self.assertEqual(result.resolve("s"), "s")

    def test_offset_survives_path_compression(self) -> None:
        # `c` is copied to `s` (a Mov pair, merged first), and then
        # `s` takes byte 1 of `t`. Resolving `c` walks c -> s -> t.
        p = lambda n, k=0: asm_ast.Pseudo(name=n, offset=k)
        instrs = [
            asm_ast.Mov(src=p("s"), dst=p("c"), is_volatile=False),
        ] + _carry_byte(p("t", 1), p("s"))
        g = InterferenceGraph()
        g.nodes["t"] = _node("t", 2)
        g.nodes["s"] = _node("s")
        g.nodes["c"] = _node("c")
        result = coalesce_moves(self._fn(instrs), g)
        self.assertEqual(result.resolve("c"), "t")
        self.assertEqual(result.offset_of("c"), 1)
        self.assertEqual(result.offset_of("s"), 1)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestCarryIncSim(unittest.TestCase):

    _BYTES = [0x00, 0x01, 0x7F, 0x80, 0xFE, 0xFF]
    _WORDS = [0x0000, 0x00FF, 0x0100, 0x7FFF, 0x80F0, 0xFF01, 0xFFFF]

    def _check(self, ctype, size, op, words, optimize) -> None:
        mask = (1 << (size * 8)) - 1
        cases = [(w, b) for w in words for b in self._BYTES]
        n = len(cases)
        ws = ", ".join(f"{w}UL" for w, _ in cases)
        bs = ", ".join(str(b) for _, b in cases)
        src = (
            f"{ctype} g;\n"
            f"{ctype} r[{n}];\n"
            f"{ctype} q[{n}];\n"
            f"{ctype} ws[{n}] = {{{ws}}};\n"
            f"unsigned char bs[{n}] = {{{bs}}};\n"
            f"void f(unsigned char c) {{ g {op}= c; }}\n"
            f"{ctype} h({ctype} t, unsigned char c) {{ return t {op} c; }}\n"
            "int main(void) {\n"
            f"    for (int i = 0; i < {n}; i++) {{\n"
            "        g = ws[i]; f(bs[i]); r[i] = g;\n"
            "        q[i] = h(ws[i], bs[i]);\n"
            "    }\n"
            "    return 0;\n}\n"
        )
        sim = build_sim(src, optimize=optimize)
        sim.run()
        mem = sim.mpu.memory
        for name in ("r", "q"):
            base = sim.symbols[name]
            for i, (w, b) in enumerate(cases):
                want = (w + b if op == "+" else w - b) & mask
                at = base + i * size
                got = int.from_bytes(bytes(mem[at:at + size]), "little")
                self.assertEqual(
                    got, want,
                    msg=f"{name}: {w:#x} {op} {b:#x}: got {got:#x}",
                )

    def test_uint_add_and_subtract(self) -> None:
        for optimize in (False, True):
            for op in "+-":
                with self.subTest(optimize=optimize, op=op):
                    self._check("unsigned int", 2, op, self._WORDS, optimize)

    def test_ulong_add_and_subtract(self) -> None:
        # The carry runs through bytes 1 and 2, which are followed by
        # another ADC and keep the ADC form, before the top byte.
        longs = [0x00000000, 0x00FFFFFF, 0x0000FFFF, 0xFFFFFFFF,
                 0x12345678, 0x01000000]
        for optimize in (False, True):
            for op in "+-":
                with self.subTest(optimize=optimize, op=op):
                    self._check("unsigned long", 4, op, longs, optimize)

    def test_times_nine(self) -> None:
        # `(x << 3) + x` puts the sum's high byte in the shift temp's
        # high byte, which the BCC / INC then updates.
        src = (
            "unsigned int r[256];\n"
            "unsigned int f(unsigned char x) { return x * 9; }\n"
            "int main(void) {\n"
            "    for (unsigned int i = 0; i < 256; i++) r[i] = f(i);\n"
            "    return 0;\n}\n"
        )
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        body = asm.split("\nf:")[1].split("RTS")[0]
        self.assertIn("BCC   .carry_inc@", body)
        sim = build_sim(src, optimize=True)
        sim.run()
        mem = sim.mpu.memory
        base = sim.symbols["r"]
        for i in range(256):
            got = mem[base + 2 * i] | (mem[base + 2 * i + 1] << 8)
            self.assertEqual(got, i * 9, msg=f"x={i}")


if __name__ == "__main__":
    unittest.main()
