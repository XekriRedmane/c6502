"""Tests for the packed BCD builtins, `__c6502_bcd_add` and
`__c6502_bcd_sub` (builtin_functions.py, docs/builtins.md).

The arithmetic is checked against py65's decimal mode, which the
simulator runs, for every pair of bytes. The front end is checked for
the types it gives and the errors it reports, the optimizer for the
constants it folds, the back end for the code it writes, and both
pipelines for the values the simulator computes.
"""
from __future__ import annotations

import random
import shutil
import unittest

import builtin_functions as bf
import tac_ast
from passes.type_checking import check_program
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac

_HAVE_PCPP = shutil.which("pcpp") is not None


def _codegen(src: str, optimize: bool) -> str:
    from compile import _run_stage
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _body(asm: str, name: str) -> list[str]:
    """The instruction lines of function `name`, without the
    `SUBROUTINE` line, blank lines, or comments."""
    text = asm.split(f"\n{name}:\n")[1]
    out = []
    for line in text.splitlines():
        line = line.strip()
        if not line or line.startswith(";") or line == "SUBROUTINE":
            continue
        if line.endswith(":") and not line.startswith("."):
            break
        out.append(" ".join(line.split()))
    return out


def _tac(src: str, optimize: bool) -> tac_ast.Program:
    from compile import _resolved, translate_for_codegen
    from passes.optimization import optimize_program
    prog, symbols, types = check_program(
        _resolved(preprocess(src), optimize=optimize),
    )
    tac = translate_for_codegen(prog, symbols, types, optimize)
    return optimize_program(tac, symbols) if optimize else tac


class TestArithmetic(unittest.TestCase):
    """`adc_byte` and `sbc_byte` against py65's decimal mode, for
    every accumulator, operand, and carry."""

    def test_every_byte_pair_matches_py65(self) -> None:
        from py65.devices.mpu6502 import MPU
        mpu = MPU()
        for name, fn, opcode in (
            ("ADC", bf.adc_byte, 0x69), ("SBC", bf.sbc_byte, 0xE9),
        ):
            bad = []
            for a in range(256):
                for b in range(256):
                    for c in (0, 1):
                        mpu.memory[0x200] = opcode
                        mpu.memory[0x201] = b
                        mpu.pc = 0x200
                        mpu.a = a
                        mpu.p = mpu.DECIMAL | mpu.UNUSED | (
                            mpu.CARRY if c else 0
                        )
                        mpu.step()
                        got = fn(a, b, c)
                        want = (mpu.a, int(bool(mpu.p & mpu.CARRY)))
                        if got != want:
                            bad.append((a, b, c, got, want))
            with self.subTest(op=name):
                self.assertEqual(bad[:5], [])

    def test_wraps_and_borrows(self) -> None:
        self.assertEqual(bf.bcd_add(0x99, 0x01, 1), 0x00)
        self.assertEqual(bf.bcd_add(0x0099, 0x0001, 2), 0x0100)
        self.assertEqual(bf.bcd_add(0x9999, 0x0001, 2), 0x0000)
        self.assertEqual(bf.bcd_sub(0x00, 0x01, 1), 0x99)
        self.assertEqual(bf.bcd_sub(0x0100, 0x0001, 2), 0x0099)
        self.assertEqual(bf.bcd_add(0x00999999, 0x250, 4), 0x01000249)


def _binaries(prog: tac_ast.Program, fn: str) -> list:
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function) and tl.name == fn:
            return [i for i in tl.instructions if isinstance(i, tac_ast.Binary)]
    raise KeyError(fn)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestTac(unittest.TestCase):

    def test_constants_fold_in_both_pipelines(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "uint16_t f(void) { return __c6502_bcd_add(0x0999, 0x0001); }"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                tac = _tac(src, optimize)
                self.assertEqual(_binaries(tac, "f"), [])
                asm = _codegen(src, optimize)
                self.assertNotIn("SED", asm)

    def test_propagated_constants_fold(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "uint8_t f(void) { uint8_t x = 0x58; uint8_t y = 0x47;"
            " return __c6502_bcd_add(x, y); }"
        )
        tac = _tac(src, optimize=True)
        self.assertEqual(_binaries(tac, "f"), [])
        asm = _codegen(src, optimize=True)
        self.assertNotIn("SED", asm)
        self.assertIn("#$05", asm)   # 58 + 47 = 105, wrapped to 05


    def test_tac_sim(self) -> None:
        for name, src, expected in _PROGRAMS:
            with self.subTest(case=name):
                self.assertEqual(_run_tac(preprocess(src)) & 0xFFFF, expected)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestCodegen(unittest.TestCase):
    def test_score(self) -> None:
        src = (
            "#include <stdint.h>\nuint32_t score;"
            "void add(uint16_t points) {"
            " score = __c6502_bcd_add(score, points); }"
        )
        asm = _codegen(src, optimize=True)
        # `points` arrives in A and X, and `add` stores it before the
        # chain, which needs A for each byte.
        self.assertEqual(_body(asm, "add"), [
            "STA __local_add__points_0", "STX __local_add__points_1",
            "SED", "CLC",
            "LDA score", "ADC __local_add__points_0", "STA score",
            "LDA score+1", "ADC __local_add__points_1", "STA score+1",
            "LDA score+2", "ADC #$00", "STA score+2",
            "LDA score+3", "ADC #$00", "STA score+3",
            "CLD", "RTS",
        ])

    def test_member_in_place(self) -> None:
        # `in_place_member_update` computes the member in its own
        # bytes, whichever operand it is. `x` arrives in A (and X), and
        # each function stores it before the chain.
        src = (
            "#include <stdint.h>\n"
            "struct s { uint8_t tag; uint16_t score; } p;"
            "void add(uint8_t x) { p.score = __c6502_bcd_add(p.score, x); }"
            "void add2(uint16_t x) { p.score = __c6502_bcd_add(x, p.score); }"
        )
        asm = _codegen(src, optimize=True)
        self.assertEqual(_body(asm, "add"), [
            "STA __local_add__x",
            "SED", "CLC",
            "LDA p+1", "ADC __local_add__x", "STA p+1",
            "LDA p+2", "ADC #$00", "STA p+2",
            "CLD", "RTS",
        ])
        self.assertEqual(_body(asm, "add2"), [
            "STA __local_add2__x_0", "STX __local_add2__x_1",
            "SED", "CLC",
            "LDA __local_add2__x_0", "ADC p+1", "STA p+1",
            "LDA __local_add2__x_1", "ADC p+2", "STA p+2",
            "CLD", "RTS",
        ])

    def test_unused_top_bytes_go(self) -> None:
        src = (
            "#include <stdint.h>\nuint32_t s;"
            "uint8_t low(void) { return __c6502_bcd_add(s, 0x25); }"
        )
        body = _body(_codegen(src, optimize=True), "low")
        chain = body[body.index("SED"):body.index("CLD") + 1]
        self.assertEqual(chain[:5], ["SED", "CLC", "LDA s", "ADC #$25",
                                     chain[4]])
        self.assertEqual(len(chain), 6)

    def test_subtract_a_constant(self) -> None:
        src = (
            "#include <stdint.h>\nuint16_t w;"
            "void dec(void) { w = __c6502_bcd_sub(w, 1); }"
        )
        asm = _codegen(src, optimize=True)
        self.assertEqual(_body(asm, "dec"), [
            "SED", "SEC",
            "LDA w", "SBC #$01", "STA w",
            "LDA w+1", "SBC #$00", "STA w+1",
            "CLD", "RTS",
        ])

    def test_byte_result_in_a(self) -> None:
        src = (
            "#include <stdint.h>\nuint8_t a, b;"
            "uint8_t f(void) { return __c6502_bcd_add(a, b); }"
        )
        asm = _codegen(src, optimize=True)
        body = _body(asm, "f")
        self.assertEqual(body[:5], ["SED", "CLC", "LDA a", "ADC b",
                                    body[4]])
        self.assertTrue(body[4].startswith("STA "))
        self.assertEqual(body[5], "CLD")

    def test_unoptimized_soft_stack_operands(self) -> None:
        src = (
            "#include <stdint.h>\n"
            "uint16_t f(uint16_t a, uint16_t b) {"
            " return __c6502_bcd_add(a, b); }"
        )
        asm = _codegen(src, optimize=False)
        body = _body(asm, "f")
        start = body.index("SED")
        end = body.index("CLD")
        chain = body[start:end + 1]
        self.assertEqual(chain[1], "CLC")
        # Each byte is a load, an add, and a store, each of a frame
        # slot behind its own `LDY #off`.
        ops = [line.split()[0] for line in chain[2:-1]]
        self.assertTrue(all(
            line.endswith("(FP),Y") for line in chain[2:-1]
            if line.split()[0] in ("LDA", "ADC", "STA")
        ))
        self.assertEqual([o for o in ops if o not in ("LDY", "INY", "DEY")],
                         ["LDA", "ADC", "STA", "LDA", "ADC", "STA"])

    def test_decimal_flag_only_around_the_chain(self) -> None:
        src = _PROGRAMS_BY_NAME["struct_and_array"]
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _codegen(src, optimize)
                lines = [" ".join(l.split()) for l in asm.splitlines()]
                inside = False
                for line in lines:
                    op = line.split(" ")[0] if line else ""
                    if op == "SED":
                        self.assertFalse(inside)
                        inside = True
                    elif op == "CLD":
                        self.assertTrue(inside)
                        inside = False
                    elif inside:
                        self.assertIn(op, ("CLC", "SEC", "LDA", "ADC",
                                           "SBC", "STA", "LDY", "INY",
                                           "DEY"))
                self.assertFalse(inside)


# Programs whose `main` returns a 16-bit value. Each runs through
# tac_sim and the simulator in both pipelines.
_PROGRAMS = [
    ("byte_add",
     "#include <stdint.h>\nuint8_t a = 0x38, b = 0x45;"
     "int main(void) { return __c6502_bcd_add(a, b); }", 0x83),
    ("byte_wraps",
     "#include <stdint.h>\nuint8_t a = 0x99, b = 0x01;"
     "int main(void) { return __c6502_bcd_add(a, b); }", 0x00),
    ("byte_borrows",
     "#include <stdint.h>\nuint8_t a = 0x00, b = 0x01;"
     "int main(void) { return __c6502_bcd_sub(a, b); }", 0x99),
    ("word_carries",
     "#include <stdint.h>\nuint16_t a = 0x0999;"
     "int main(void) { return __c6502_bcd_add(a, 1); }", 0x1000),
    ("word_borrows",
     "#include <stdint.h>\nuint16_t a = 0x1000;"
     "int main(void) { return __c6502_bcd_sub(a, 0x0001); }", 0x0999),
    ("mixed_widths",
     "#include <stdint.h>\nuint8_t a = 0x99; uint16_t b = 0x0901;"
     "int main(void) { return __c6502_bcd_add(a, b); }", 0x1000),
    ("long_low",
     "#include <stdint.h>\nuint32_t s = 0x00999950;"
     "int main(void) { uint32_t r = __c6502_bcd_add(s, 0x0250);"
     " return (uint16_t)r; }", 0x0200),
    ("long_high",
     "#include <stdint.h>\nuint32_t s = 0x00999950;"
     "int main(void) { uint32_t r = __c6502_bcd_add(s, 0x0250);"
     " return (uint16_t)(r >> 16); }", 0x0100),
    ("long_wraps",
     "#include <stdint.h>\nuint32_t s = 0x99999999;"
     "int main(void) { s = __c6502_bcd_add(s, 1);"
     " return (uint16_t)s | (uint16_t)(s >> 16); }", 0x0000),
    ("long_borrows",
     "#include <stdint.h>\nuint32_t s = 0;"
     "int main(void) { s = __c6502_bcd_sub(s, 1);"
     " return (uint16_t)(s >> 8); }", 0x9999),
    ("as_argument",
     "#include <stdint.h>\nuint8_t a = 0x19;"
     "int twice(uint16_t v) { return v * 2; }"
     "int main(void) { return twice(__c6502_bcd_add(a, 0x0001)); }", 0x40),
    ("nested",
     "#include <stdint.h>\nuint16_t a = 0x0450, b = 0x0275;"
     "int main(void) { return __c6502_bcd_sub("
     "__c6502_bcd_add(a, b), __c6502_bcd_add(b, 0x0005)); }", 0x0445),
    ("struct_and_array",
     "#include <stdint.h>\n"
     "struct s { uint8_t tag; uint16_t score; } p = {1, 0x0995};"
     "uint16_t t[3] = {0x0001, 0x0010, 0x0100};"
     "int main(void) { for (uint8_t i = 0; i < 3; i++)"
     " p.score = __c6502_bcd_add(p.score, t[i]);"
     " t[1] = __c6502_bcd_sub(p.score, t[2]);"
     " return t[1]; }", 0x1006),
    ("invalid_digits",
     "#include <stdint.h>\nuint8_t a = 0x0F, b = 0x0F;"
     "int main(void) { return __c6502_bcd_add(a, b)"
     " | __c6502_bcd_sub(b, 0x1A) << 8; }",
     bf.bcd_add(0x0F, 0x0F, 1) | bf.bcd_sub(0x0F, 0x1A, 1) << 8),
    ("register_param",
     "#include <stdint.h>\n"
     "uint8_t f(register uint8_t x) { return __c6502_bcd_add(x, 0x01); }"
     "int main(void) { return f(0x09) + f(0x19); }", 0x30),
]
_PROGRAMS_BY_NAME = {name: src for name, src, _ in _PROGRAMS}


def _sweep_program(op: str, width: int, pairs) -> tuple[str, int]:
    """A program that applies `op` to each pair in `pairs` through
    arrays of `width`-byte operands, and returns a 16-bit hash of the
    results. Returns the source and the hash the arithmetic gives."""
    ctype = {1: "uint8_t", 2: "uint16_t", 4: "uint32_t"}[width]
    fold = bf.bcd_add if op == "add" else bf.bcd_sub
    xs = ", ".join(f"{a:#x}" for a, _ in pairs)
    ys = ", ".join(f"{b:#x}" for _, b in pairs)
    n = len(pairs)
    src = (
        "#include <stdint.h>\n"
        f"{ctype} xs[{n}] = {{{xs}}}; {ctype} ys[{n}] = {{{ys}}};"
        f"{ctype} out[{n}];"
        "int main(void) { uint16_t h = 0;"
        f" for (uint8_t i = 0; i < {n}; i++) {{"
        f" out[i] = __c6502_bcd_{op}(xs[i], ys[i]);"
        f" {ctype} r = out[i];"
        " h = (h << 3 | h >> 13) ^ (uint16_t)r ^ (uint16_t)(r >> 8);"
        " }"
        " return h; }"
    )
    h = 0
    mask = (1 << 8 * width) - 1
    for a, b in pairs:
        r = fold(a, b, width) & mask
        h = ((h << 3 | h >> 13) & 0xFFFF) ^ (r & 0xFFFF) ^ ((r >> 8) & 0xFFFF)
    return src, h


def _sweeps():
    rng = random.Random(6502)
    out = []
    for width in (1, 2, 4):
        for op in ("add", "sub"):
            pairs = [
                (rng.randrange(1 << 8 * width), rng.randrange(1 << 8 * width))
                for _ in range(24)
            ]
            out.append((f"{op}{width}", *_sweep_program(op, width, pairs)))
    return out


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSimulator(unittest.TestCase):
    def _run(self, src: str, optimize: bool):
        sim = build_sim(src, optimize=optimize)
        result = sim.run(max_cycles=3_000_000)
        self.assertFalse(result.timed_out)
        # The program ends with the decimal flag clear.
        self.assertFalse(sim.mpu.p & sim.mpu.DECIMAL)
        return result

    def test_programs(self) -> None:
        for name, src, expected in _PROGRAMS:
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = self._run(src, optimize)
                    self.assertEqual(result.return_int() & 0xFFFF, expected)

    def test_random_operands_match_the_cpu_model(self) -> None:
        # Most of these operands have a digit above 9, so this covers
        # what the NMOS 6502 does with invalid BCD.
        for name, src, expected in _sweeps():
            with self.subTest(case=name, sim="tac"):
                self.assertEqual(_run_tac(preprocess(src)) & 0xFFFF, expected)
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = self._run(src, optimize)
                    self.assertEqual(result.return_int() & 0xFFFF, expected)


if __name__ == "__main__":
    unittest.main()
