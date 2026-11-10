"""Tests for the runtime's zero-page layout (`passes/runtime_layout.py`):
where SSP, FP, HARGS, and DPTR go, how HARGS is sized, the EQU lines
that bind them, linking, and simulator runs of programs that reserve
the bytes the classic layout uses.
"""
from __future__ import annotations

import io
import re
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import asm_ast
from passes.runtime_layout import (
    CLASSIC, RuntimeLayout, RuntimeLayoutError, choose_layout,
    helper_footprints, hargs_need,
)
from passes.type_checking import TypeCheckError
from sim.harness import build_sim


def _codegen(src: str, optimize: bool = True) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _equ(asm: str) -> dict[str, int]:
    return {
        m.group(1): int(m.group(2), 16)
        for m in re.finditer(r"^(\S+)\tEQU\t\$([0-9A-F]+)$", asm, re.M)
    }


def _layout_of(asm: str) -> RuntimeLayout:
    e = _equ(asm)
    return RuntimeLayout(
        ssp=e["SSP"], fp=e["FP"], hargs=e["HARGS"], dptr=e["DPTR"],
    )


# The Monitor's zero-page bytes in the unenhanced Apple IIe F8 ROM,
# $1F through $49 (one array, so they're reserved as one object).
_MONITOR = (
    "extern volatile unsigned char monitor_zp[0x2B] "
    "__attribute__((address(0x1F)));\n"
)


class TestChooseLayout(unittest.TestCase):

    def test_classic_when_nothing_collides(self) -> None:
        calls = []
        layout = choose_layout(
            {0x26, 0x40, 0x90}, lambda: calls.append(1) or 5,
        )
        self.assertEqual(layout, CLASSIC)
        # HARGS isn't measured when the block stays put.
        self.assertEqual(calls, [])

    def test_moves_around_reserved_bytes(self) -> None:
        reserved = set(range(0x20, 0x26))
        layout = choose_layout(reserved, lambda: 6)
        self.assertEqual(layout.hargs_size, 6)
        self.assertFalse(layout.bytes() & reserved)
        # HARGS first, then SSP, FP, DPTR, each in the lowest free run.
        self.assertEqual(
            (layout.hargs, layout.ssp, layout.fp, layout.dptr),
            (0x00, 0x06, 0x08, 0x0A),
        )

    def test_unused_hargs_still_gets_a_byte(self) -> None:
        layout = choose_layout({0x24}, lambda: 0)
        self.assertEqual(layout.hargs_size, 1)
        self.assertEqual(len(layout.bytes()), 7)

    def test_no_room(self) -> None:
        reserved = set(range(0x02, 0x80))
        with self.assertRaises(RuntimeLayoutError) as cm:
            choose_layout(reserved, lambda: 4)
        self.assertIn("$02-$7F", str(cm.exception))


class TestHargsSize(unittest.TestCase):

    def test_helper_footprints(self) -> None:
        fp = helper_footprints()
        # mul32 works in HARGS+12..19, sdivmod32 stashes signs at
        # +20 / +21, and sdivmod8 calls udivmod8.
        self.assertEqual(fp["mul32"], 20)
        self.assertEqual(fp["sdivmod32"], 22)
        self.assertEqual(fp["sdivmod8"], 6)
        self.assertGreaterEqual(fp["sdivmod8"], fp["udivmod8"])

    def test_need_counts_offsets_and_calls(self) -> None:
        def prog(*instrs):
            return asm_ast.Program(top_level=[asm_ast.Function(
                name="f", is_global=True, params=[],
                instructions=list(instrs),
            )])
        h = lambda k: asm_ast.Data(name="HARGS", offset=k)
        a = asm_ast.Reg(reg=asm_ast.A())
        self.assertEqual(hargs_need(prog()), 0)
        self.assertEqual(hargs_need(prog(asm_ast.Mov(src=a, dst=h(9)))), 10)
        self.assertEqual(
            hargs_need(prog(asm_ast.Call(name="mul32", reg_args=[]))), 20,
        )

    def test_relocated_hargs_fits_the_program(self) -> None:
        cases = {
            "no helper": ("int f(int a) { return a + 1; }\n", 1),
            "8-bit multiply":
                ("unsigned char f(unsigned char a, unsigned char b) "
                 "{ return a * b; }\n", 5),
            "32-bit divide":
                ("long f(long a, long b) { return a / b; }\n", 22),
        }
        for label, (src, size) in cases.items():
            with self.subTest(label):
                asm = _codegen(_MONITOR + src)
                layout = _layout_of(asm)
                hargs = layout.hargs
                # HARGS is the program's need, placed clear of the
                # Monitor's bytes and of the other three.
                others = RuntimeLayout(
                    ssp=layout.ssp, fp=layout.fp, dptr=layout.dptr,
                    hargs=hargs, hargs_size=size,
                ).bytes()
                self.assertFalse(others & set(range(0x1F, 0x4A)))
                self.assertEqual(len(others), 6 + size)


class TestCodegen(unittest.TestCase):

    def test_classic_equs(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _codegen("int main(void) { return 0; }\n", optimize)
                self.assertEqual(_layout_of(asm), CLASSIC)
                # The four runtime lines start the output.
                self.assertTrue(asm.startswith(
                    "SSP\tEQU\t$00\nFP\tEQU\t$02\nHARGS\tEQU\t$04\n"
                    "DPTR\tEQU\t$24\n"
                ))

    def test_cursor_reserved(self) -> None:
        # Declaring the Monitor's cursor moves DPTR off $24-$25, and a
        # zero-page global goes in the bytes left over.
        asm = _codegen(
            "extern unsigned char CH __attribute__((address(0x24)));\n"
            "extern unsigned char CV __attribute__((address(0x25)));\n"
            "unsigned char g __attribute__((zeropage));\n"
            "unsigned char peek(unsigned char *p) { return *p + CH + g; }\n",
            optimize=False,
        )
        equ = _equ(asm)
        self.assertEqual((equ["CH"], equ["CV"]), (0x24, 0x25))
        # No helper is called, so HARGS gets one byte. HARGS, SSP, FP,
        # and DPTR take the lowest free runs, and g the next byte.
        self.assertEqual(
            [equ[n] for n in ("HARGS", "SSP", "FP", "DPTR", "g")],
            [0x00, 0x01, 0x03, 0x05, 0x07],
        )


class TestSim(unittest.TestCase):

    _SRC = (
        "unsigned char buf[8];\n"
        "unsigned long lm(unsigned long a, unsigned long b) "
        "{ return a * b / 7; }\n"
        "int sum(unsigned char *p, unsigned char n) {\n"
        "  int s = 0;\n"
        "  while (n--) s += *p++;\n"
        "  return s;\n"
        "}\n"
        "int rec(int n) { return n ? n + rec(n - 1) : 0; }\n"
        "int main(void) {\n"
        "  for (unsigned char i = 0; i < 8; i++) buf[i] = i * 3;\n"
        "  float f = (float)sum(buf, 8);\n"
        "  int g = (int)f;\n"
        "  return (int)(lm(100000, 3) & 0x7FFF) + g + rec(10);\n"
        "}\n"
    )
    _WANT = ((100000 * 3 // 7) & 0x7FFF) + 84 + 55

    def _run(self, src: str, optimize: bool):
        sim = build_sim(src, optimize=optimize)
        pattern = {a: (0xA5 ^ a) & 0xFF for a in range(0x1F, 0x4A)}
        for a, v in pattern.items():
            sim.mpu.memory[a] = v
        result = sim.run()
        changed = [a for a, v in pattern.items() if result.memory[a] != v]
        return result.return_int(), changed

    def test_monitor_bytes_untouched(self) -> None:
        # Pointer accesses (DPTR), recursion (SSP, FP), a 32-bit
        # multiply and divide (6502 helpers with HARGS scratch), and an
        # int / float round trip (Python hooks) all run with the block
        # moved clear of $1F-$49.
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got, changed = self._run(_MONITOR + self._SRC, optimize)
                self.assertEqual(got, self._WANT)
                self.assertEqual(changed, [])

    def test_classic_layout_writes_those_bytes(self) -> None:
        # Without the reservation, the unoptimized build writes into
        # $1F-$49 (the top of HARGS and DPTR are there), which is what
        # the test above checks doesn't happen. The optimized build
        # keeps its pointers in zero-page locals instead of DPTR, and
        # its helpers stay below $1F, so it doesn't.
        got, changed = self._run(self._SRC, optimize=False)
        self.assertEqual(got, self._WANT)
        self.assertNotEqual(changed, [])


class TestMonitorHeaders(unittest.TestCase):
    """`<apple2/monitor_io.h>` reserves the zero page of the Monitor
    routines a program calls, and `<apple2/interactive_monitor.h>` adds the
    bytes only the Monitor's interactive part uses."""

    # The bytes of the routines a program calls: text, keyboard, line
    # input, lo-res graphics, the IRQ entry, and INIT.
    _ROUTINE_BYTES = (
        list(range(0x1F, 0x2F)) + [0x30, 0x32, 0x33]
        + list(range(0x35, 0x3A)) + [0x45, 0x48]
    )
    # Every byte the Monitor uses.
    _ALL_BYTES = [0x00, 0x01] + list(range(0x1F, 0x4A))

    _HEADERS = {
        "monitor_io.h": ("#include <apple2/monitor_io.h>\n", _ROUTINE_BYTES),
        "interactive_monitor.h":
            ("#include <apple2/interactive_monitor.h>\n", _ALL_BYTES),
    }

    def test_layout_clear_of_the_reserved_bytes(self) -> None:
        for header, (include, reserved) in self._HEADERS.items():
            for optimize in (False, True):
                with self.subTest(header, optimize=optimize):
                    asm = _codegen(include + TestSim._SRC, optimize)
                    equ = _equ(asm)
                    self.assertEqual(
                        (equ["CH"], equ["CSW"]), (0x24, 0x36),
                    )
                    for name in ("SSP", "FP", "DPTR"):
                        self.assertFalse(
                            {equ[name], equ[name] + 1} & set(reserved),
                        )

    def test_reserved_bytes_untouched(self) -> None:
        for header, (include, reserved) in self._HEADERS.items():
            for optimize in (False, True):
                with self.subTest(header, optimize=optimize):
                    sim = build_sim(
                        include + TestSim._SRC, optimize=optimize,
                    )
                    pattern = {a: (0x5A ^ a) & 0xFF for a in reserved}
                    for a, v in pattern.items():
                        sim.mpu.memory[a] = v
                    result = sim.run()
                    self.assertEqual(result.return_int(), TestSim._WANT)
                    self.assertEqual(
                        [a for a, v in pattern.items()
                         if result.memory[a] != v],
                        [],
                    )

    def test_routine_header_leaves_tool_bytes_free(self) -> None:
        # With only monitor_io.h, a zero-page global can go in a byte only
        # the interactive Monitor uses, such as $00 or $2F.
        asm = _codegen(
            "#include <apple2/monitor_io.h>\n"
            "unsigned char g __attribute__((zeropage(0x2F)));\n"
            "unsigned char h __attribute__((zeropage(0x00)));\n"
            "int main(void) { return g + h; }\n",
        )
        self.assertEqual((_equ(asm)["g"], _equ(asm)["h"]), (0x2F, 0x00))
        with self.assertRaises(TypeCheckError):
            _codegen(
                "#include <apple2/interactive_monitor.h>\n"
                "unsigned char g __attribute__((zeropage(0x2F)));\n"
                "int main(void) { return g; }\n",
            )


class TestLink(unittest.TestCase):

    def _compile(self, src: str, path: Path) -> None:
        from compile import main as compile_main
        with patch("sys.stdin", io.StringIO(src)):
            self.assertEqual(compile_main([
                "compile.py", "-", "--codegen", "--optimize",
                "-o", str(path),
            ]), 0)

    def _link(self, paths, out: Path) -> tuple[int, str]:
        from compile import main as compile_main
        err = io.StringIO()
        with patch("sys.stderr", err):
            rc = compile_main(
                ["compile.py", "--link", *map(str, paths), "-o", str(out)],
            )
        return rc, err.getvalue()

    def test_layout_bound_once(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            a, b, out = (Path(tmp) / n for n in ("a.asm", "b.asm", "o.asm"))
            self._compile("int g(int x);\n"
                          "int main(void) { return g(2); }\n", a)
            self._compile("int g(int x) { return x + 1; }\n", b)
            rc, err = self._link([a, b], out)
            self.assertEqual(rc, 0, err)
            text = out.read_text()
            self.assertEqual(text.count("DPTR\tEQU\t$24"), 1)
            self.assertEqual(text.count("SSP\tEQU\t$00"), 1)

    def test_different_layouts_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            a, b, out = (Path(tmp) / n for n in ("a.asm", "b.asm", "o.asm"))
            self._compile("int main(void) { return 0; }\n", a)
            self._compile("int g(int x) { return x + 1; }\n", b)
            # Stand in for a unit compiled with another layout.
            b.write_text(b.read_text().replace(
                "DPTR\tEQU\t$24", "DPTR\tEQU\t$30",
            ))
            rc, err = self._link([a, b], out)
            self.assertEqual(rc, 1)
            self.assertIn("runtime zero-page layout", err)


if __name__ == "__main__":
    unittest.main()
