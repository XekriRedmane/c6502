"""Tests for reads and writes of a named `volatile` object.

`c99_to_tac` lowers every access to a volatile scalar through a
volatile load or store (`_volatile_scalar`): `IndexedLoad` /
`IndexedSymbolStore` at index 0 for an object with static storage,
and `GetAddress` plus `Load` / `Store` for an automatic one. Before,
the access was a plain `Copy`, so `KBD;` was dropped in both
pipelines and, under --optimize, `while (!(KBD & 0x80)) ;` read KBD
once and didn't wait.

Each test runs in both pipelines. The simulator can't change memory
behind the program's back, so a wait loop is checked by whether the
run reaches the cycle cap.
"""
from __future__ import annotations

import re
import unittest

from sim.harness import build_sim

_KBD = (
    "extern volatile unsigned char KBD __attribute__((address(0xC000)));\n"
    "extern volatile unsigned char KBDSTRB "
    "__attribute__((address(0xC010)));\n"
)


def _codegen(src: str, optimize: bool) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _reads(asm: str, name: str) -> int:
    """How many instructions read `name` (LDA, CMP, or BIT)."""
    return len(re.findall(rf"(?m)^\s+(LDA|CMP|BIT)\s+{name}$", asm))


def _run(src: str, optimize: bool, mem=None, max_cycles=None):
    sim = build_sim(src, optimize=optimize)
    for addr, value in (mem or {}).items():
        sim.mpu.memory[addr] = value
    if max_cycles is None:
        return sim.run()
    return sim.run(max_cycles=max_cycles)


class TestReadsKept(unittest.TestCase):

    def test_bare_read_statement(self) -> None:
        for stmt in ("KBDSTRB;", "(void)KBDSTRB;"):
            for optimize in (False, True):
                with self.subTest(stmt, optimize=optimize):
                    asm = _codegen(
                        _KBD + f"void clear(void) {{ {stmt} }}\n", optimize,
                    )
                    self.assertEqual(_reads(asm, "KBDSTRB"), 1)

    def test_each_read_happens(self) -> None:
        src = (
            "volatile unsigned char g;\n"
            "unsigned char twice(void) { return g + g; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_reads(_codegen(src, optimize), "g"), 2)

    def test_wait_loop(self) -> None:
        src = _KBD + (
            "int main(void) {\n"
            "  while (!(KBD & 0x80)) ;\n"
            "  return KBD;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize, key="none"):
                result = _run(src, optimize, {0xC000: 0x41}, 20_000)
                self.assertTrue(result.timed_out)
            with self.subTest(optimize=optimize, key="C1"):
                result = _run(src, optimize, {0xC000: 0xC1}, 20_000)
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int(), 0xC1)

    def test_optimized_wait_loop_shape(self) -> None:
        asm = _codegen(
            _KBD + "void wait(void) { while (!(KBD & 0x80)) ; }\n", True,
        )
        self.assertRegex(
            asm, r"(?m)^(\.\S+):\n\s+LDA\s+KBD\n\s+BPL\s+\1$",
        )


class TestWrites(unittest.TestCase):

    def _both(self, src: str, want: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize).return_int(), want)

    def test_increment_and_compound(self) -> None:
        self._both(
            "volatile unsigned char g;\n"
            "int main(void) {\n"
            "  g = 5;\n"
            "  unsigned char a = g++;\n"
            "  unsigned char b = ++g;\n"
            "  g += 3; g -= 1;\n"
            "  return a * 1000 + b * 10 + g;\n"
            "}\n",
            5 * 1000 + 7 * 10 + 9,
        )

    def test_two_byte_object(self) -> None:
        self._both(
            "volatile unsigned int w;\n"
            "int main(void) { w = 0x1234; w += 1; w++; return w; }\n",
            0x1236,
        )

    def test_zero_page_object(self) -> None:
        self._both(
            "volatile unsigned char z __attribute__((zeropage)) = 3;\n"
            "int main(void) { z++; z *= 2; return z; }\n",
            8,
        )

    def test_block_static(self) -> None:
        self._both(
            "int count(void) { static volatile int n; return ++n; }\n"
            "int main(void) { count(); count(); return count(); }\n",
            3,
        )

    def test_automatic(self) -> None:
        self._both(
            "int main(void) {\n"
            "  volatile unsigned char x = 5;\n"
            "  x++; x += 2;\n"
            "  return x;\n"
            "}\n",
            8,
        )

    def test_chained_assignment_value(self) -> None:
        src = (
            "volatile unsigned char g;\n"
            "unsigned char set(void) { return g = 7; }\n"
            "int main(void) { return set(); }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize).return_int(), 7)
                # The value is the one stored, so `g` isn't read back.
                self.assertEqual(_reads(_codegen(src, optimize), "g"), 0)

    def test_self_assignment(self) -> None:
        src = (
            "volatile unsigned char g;\n"
            "void touch(void) { g = g; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _codegen(src, optimize)
                self.assertEqual(_reads(asm, "g"), 1)
                self.assertRegex(asm, r"(?m)^\s+STA\s+g$")


class TestQualifiedSignedWidening(unittest.TestCase):
    """`_emit_cast` tested the source type as written, so a `const` or
    `volatile` `signed char` didn't match `SChar` and was
    zero-extended: -1 read back as 255. Found by fuzz.stmts once it
    declared objects `volatile`."""

    def test_widening_keeps_sign(self) -> None:
        for qual in ("const", "volatile", "const volatile"):
            for optimize in (False, True):
                with self.subTest(qual=qual, optimize=optimize):
                    src = (
                        f"{qual} signed char g = -1;\n"
                        "int f(" + qual + " signed char p) { return p; }\n"
                        "int main(void) {\n"
                        "  long r = g;\n"
                        "  return (r == -1) + 2 * (f(-1) == -1);\n"
                        "}\n"
                    )
                    self.assertEqual(_run(src, optimize).return_int(), 3)


class TestVolatileSelfMov(unittest.TestCase):
    """A Mov whose operands match is a no-op, so both the emitter and
    the simulator's assembler drop it. A volatile one still reads and
    writes the byte."""

    def _prog(self, is_volatile: bool):
        import asm_ast
        g = asm_ast.Data(name="g", offset=0)
        fn = asm_ast.Function(
            name="f", is_global=True, params=[], instructions=[
                asm_ast.Mov(src=g, dst=g, is_volatile=is_volatile),
                asm_ast.Return(save_a=False, save_x=False),
            ],
        )
        return asm_ast.Program(top_level=[fn])

    def test_emitted(self) -> None:
        from asm_emit import emit_program
        from passes.asm_to_asm2 import translate_program
        for is_volatile, want in ((True, 1), (False, 0)):
            with self.subTest(is_volatile=is_volatile):
                asm = emit_program(translate_program(self._prog(is_volatile)))
                self.assertEqual(_reads(asm, "g"), want)
                self.assertEqual(len(re.findall(r"STA\s+g", asm)), want)

    def test_assembled(self) -> None:
        from sim import assembler
        for is_volatile, want in (
            (True, bytes([0xAD, 0x00, 0x30, 0x8D, 0x00, 0x30, 0x60])),
            (False, bytes([0x60])),
        ):
            with self.subTest(is_volatile=is_volatile):
                out = assembler.assemble(
                    self._prog(is_volatile), origin=0x0800,
                    extra_symbols={"g": 0x3000},
                )
                self.assertEqual(
                    bytes(out.image[0x0800:0x0800 + len(want)]), want,
                )


if __name__ == "__main__":
    unittest.main()
