"""Tests for zero-page globals (`[[c6502::zeropage]]`,
`passes/zp_globals.py`).

Covers parsing of both forms in both positions, the static-storage
rule, placement (fixed and automatic, overlaps, ranges, `extern`),
the generated code (EQU binding, no storage, `__zp_init`, `(zp),Y`
on a zero-page pointer, pinned bytes kept out of the compiler's
pools), simulator runs of initialization and aliasing in both
pipelines, promotion of busy globals under `--optimize`, and
linking units that share zero-page objects (`compile.py --link`),
including putting back a promoted object that doesn't fit, run
through dasm when it's installed.
"""
from __future__ import annotations

import io
import re
import shutil
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import c99_ast
from parser import ParserError, parse
from passes.type_checking import TypeCheckError
from passes.zp_globals import ZpGlobalError
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


def _placements(src: str) -> dict[str, object]:
    prog = parse(src)
    out = {}
    for d in prog.declaration:
        if isinstance(d, c99_ast.VarDecl):
            out[d.var_decl.name] = d.var_decl.placement
    return out


def _codegen(src: str, optimize: bool = True) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _equ(asm: str) -> dict[str, int]:
    return {
        m.group(1): int(m.group(2), 16)
        for m in re.finditer(r"^(\S+)\tEQU\t\$([0-9A-F]+)$", asm, re.M)
    }


class TestParse(unittest.TestCase):

    def test_prefix_and_postfix_forms(self) -> None:
        got = _placements(
            "[[c6502::zeropage]] unsigned char a;\n"
            "unsigned char b [[c6502::zeropage(0xBA)]];\n"
            "unsigned char c;\n"
        )
        self.assertEqual(got["a"], c99_ast.ZeroPage(address=None))
        self.assertEqual(got["b"], c99_ast.ZeroPage(address=0xBA))
        self.assertIsNone(got["c"])

    def test_prefix_applies_to_every_declarator(self) -> None:
        got = _placements("[[c6502::zeropage]] unsigned char a, b;\n")
        self.assertEqual(got["a"], c99_ast.ZeroPage(address=None))
        self.assertEqual(got["b"], c99_ast.ZeroPage(address=None))

    def test_rejected_forms(self) -> None:
        bad = [
            "[[c6502::zeropage]] int f(void) { return 0; }\n",
            "[[c6502::zp_abi]] unsigned char a;\n",
            'unsigned char a [[c6502::zeropage("x")]];\n',
            "unsigned char a [[c6502::zeropage(0x100)]];\n",
            "[[c6502::zeropage]] unsigned char a "
            "[[c6502::zeropage]];\n",
            "unsigned char a [[c6502::nonsense]];\n",
        ]
        for src in bad:
            with self.subTest(src=src):
                with self.assertRaises(ParserError):
                    parse(src)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestPlacement(unittest.TestCase):

    def test_automatic_addresses_follow_fixed_ones(self) -> None:
        asm = _codegen(
            "unsigned char a [[c6502::zeropage]];\n"
            "unsigned int b [[c6502::zeropage(0x27)]];\n"
            "unsigned char c [[c6502::zeropage]][3];\n"
            "int main(void) { return a + b + c[1]; }\n"
        )
        equ = _equ(asm)
        self.assertEqual(equ["b"], 0x27)
        self.assertEqual(equ["a"], 0x26)
        self.assertEqual(equ["c"], 0x29)
        # No storage in the data segment.
        self.assertNotRegex(asm, r"(?m)^a:|^b:|^c:")

    def test_static_storage_required(self) -> None:
        bad = [
            "int main(void) { unsigned char a "
            "[[c6502::zeropage]]; a = 1; return a; }\n",
            "int main(void) { for ([[c6502::zeropage]] int i = 0; "
            "i < 2; i++) ; return 0; }\n",
        ]
        for src in bad:
            with self.subTest(src=src):
                with self.assertRaises((TypeCheckError, ParserError)):
                    _codegen(src)

    def test_placement_errors(self) -> None:
        bad = {
            "overlap": "unsigned int a [[c6502::zeropage(0x40)]];\n"
                       "unsigned char b [[c6502::zeropage(0x41)]];\n",
            "past $FF": "unsigned int a [[c6502::zeropage(0xFF)]];\n",
            "full": "unsigned char a [[c6502::zeropage]][91];\n",
            "two addresses":
                "unsigned char a [[c6502::zeropage(0x40)]];\n"
                "unsigned char a [[c6502::zeropage(0x41)]];\n",
        }
        for what, decls in bad.items():
            with self.subTest(what=what):
                with self.assertRaises(TypeCheckError):
                    _codegen(decls + "int main(void) { return a; }\n")

    def test_extern_without_address_is_left_to_the_linker(self) -> None:
        # Another unit defines it; this one neither places nor
        # initializes it, and lists it for the linker.
        asm = _codegen(
            "extern unsigned char a [[c6502::zeropage]];\n"
            "int main(void) { return a; }\n"
        )
        self.assertNotIn("a", _equ(asm))
        self.assertNotIn("__zp_init", asm)
        self.assertIn("; zp a size=1 at=auto def=false init=", asm)

    def test_errors_are_zp_global_errors(self) -> None:
        with self.assertRaises(ZpGlobalError):
            _codegen(
                "unsigned char a [[c6502::zeropage]][91];\n"
                "int main(void) { return a[0]; }\n"
            )

    def test_extern_with_address_is_reserved_not_initialized(self) -> None:
        asm = _codegen(
            "extern unsigned char k [[c6502::zeropage(0x90)]];\n"
            "int main(void) { return k; }\n"
        )
        self.assertEqual(_equ(asm)["k"], 0x90)
        self.assertNotIn("__zp_init", asm)

    def test_redeclaration_accumulates(self) -> None:
        asm = _codegen(
            "extern unsigned char a [[c6502::zeropage(0x50)]];\n"
            "unsigned char a = 4;\n"
            "int main(void) { return a; }\n"
        )
        self.assertEqual(_equ(asm)["a"], 0x50)
        self.assertIn("__zp_init", asm)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestCodegen(unittest.TestCase):

    def test_pointer_is_indirect_base(self) -> None:
        asm = _codegen(
            "unsigned char buf[4] = {1, 2, 3, 4};\n"
            "unsigned char *p [[c6502::zeropage]];\n"
            "int main(void) { p = buf; return p[2]; }\n"
        )
        self.assertIn("(p),Y", asm)
        self.assertNotIn("DPTR", asm.split("main:")[1])

    def test_init_routine(self) -> None:
        asm = _codegen(
            "unsigned int w [[c6502::zeropage]] = 0x1234;\n"
            "unsigned char z [[c6502::zeropage]];\n"
            "int main(void) { return w + z; }\n"
        )
        init = asm.split("__zp_init:")[1]
        self.assertIn("LDA   #$00\n   STA   z", init)
        self.assertIn("LDA   #$34\n   STA   w\n", init)
        self.assertIn("LDA   #$12\n   STA   w+1", init)

    def test_pinned_pool_bytes_not_reused(self) -> None:
        # Pin $80-$8F, where zp_abi parameters and private pools
        # would start. None of the compiler's slots may land there.
        src = (
            "unsigned char g [[c6502::zeropage(0x80)]][16];\n"
            "unsigned int f(unsigned int a, unsigned int b) {\n"
            "    unsigned int t = a * 3 + b;\n"
            "    return t ^ (a + 1);\n"
            "}\n"
            "int main(void) { g[3] = 9; return f(g[3], 2) + g[3]; }\n"
        )
        equ = _equ(_codegen(src))
        for name, addr in equ.items():
            if name.startswith(("__zpabi_", "__local_")):
                self.assertFalse(
                    0x80 <= addr < 0x90, msg=f"{name} at ${addr:02X}",
                )


# A pointer and a counter used in a loop, a constant, an array, and a
# variable read once.
_PROMOTE = (
    "#include <stdint.h>\n"
    "uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
    "uint8_t *p = buf;\n"
    "uint16_t total;\n"
    "const uint8_t step = 1;\n"
    "uint8_t once = 9;\n"
    "int main(void) {\n"
    "    for (uint8_t i = 0; i < 8; i += step) total += p[i] + buf[i];\n"
    "    return total + once;\n"
    "}\n"
)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestPromotion(unittest.TestCase):
    """`--optimize` moves busy scalar globals into zero page."""

    def test_busy_scalars_promoted(self) -> None:
        asm = _codegen(_PROMOTE)
        equ = _equ(asm)
        self.assertIn("total", equ)
        self.assertIn("p", equ)
        # The pointer is the `(zp),Y` base directly.
        self.assertIn("(p),Y", asm)
        self.assertNotIn("\np:", asm)
        self.assertRegex(asm, r"; zp total size=2 at=auto def=true "
                              r"init=\$00,\$00 weight=\d+")
        init = asm[asm.index("__zp_init:"):]
        self.assertIn("STA   total+1", init)
        self.assertIn("LDA   #<buf", init)

    def test_others_left_in_place(self) -> None:
        equ = _equ(_codegen(_PROMOTE))
        for name in ("step", "buf", "once"):
            self.assertNotIn(name, equ)

    def test_declared_objects_go_first(self) -> None:
        src = ("#include <stdint.h>\n"
               "uint8_t fill [[c6502::zeropage]][89];\n" + _PROMOTE)
        equ = _equ(_codegen(src))
        self.assertEqual(equ["fill"], 0x26)
        # One byte is left, and both candidates are 2 bytes.
        self.assertNotIn("total", equ)
        self.assertNotIn("p", equ)

    def test_not_without_optimize(self) -> None:
        equ = _equ(_codegen(_PROMOTE, optimize=False))
        self.assertNotIn("total", equ)

    def test_runs(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(_PROMOTE, optimize=optimize).run()
                self.assertEqual(got.return_int(), 2 * 36 + 9)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSim(unittest.TestCase):

    def _both(self, src: str, want: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, want)

    def test_initial_values(self) -> None:
        src = (
            "struct s { unsigned char a; unsigned int b; };\n"
            "unsigned char tab [[c6502::zeropage]][3] = {7, 8, 9};\n"
            "struct s st [[c6502::zeropage]] = {1, 300};\n"
            "unsigned long l [[c6502::zeropage]] = 70000;\n"
            "unsigned char z [[c6502::zeropage]];\n"
            "unsigned char other = 5;\n"
            "unsigned char *ptr [[c6502::zeropage]] = &other;\n"
            "int main(void) {\n"
            "    if (tab[0] + tab[1] + tab[2] != 24) return 1;\n"
            "    if (st.a != 1 || st.b != 300) return 2;\n"
            "    if (l != 70000) return 3;\n"
            "    if (z != 0) return 4;\n"
            "    if (*ptr != 5) return 5;\n"
            "    return 0;\n"
            "}\n"
        )
        self._both(src, 0)

    def test_write_through_pointer_to_zp_global(self) -> None:
        # `n` is written through a pointer, so a cached copy of it
        # can't be reused across the store.
        src = (
            "unsigned char n [[c6502::zeropage(0xA0)]] = 1;\n"
            "unsigned char *p [[c6502::zeropage]];\n"
            "int main(void) {\n"
            "    unsigned char before = n;\n"
            "    p = &n;\n"
            "    *p = 5;\n"
            "    return before * 10 + n;\n"
            "}\n"
        )
        self._both(src, 15)

    def test_pointer_global_walks_buffer(self) -> None:
        src = (
            "unsigned char buf[6] = {1, 2, 3, 4, 5, 6};\n"
            "unsigned char *p [[c6502::zeropage(0xBA)]];\n"
            "unsigned char sum [[c6502::zeropage]];\n"
            "void add(unsigned char k) { sum += p[k]; }\n"
            "int main(void) {\n"
            "    p = buf;\n"
            "    for (unsigned char i = 0; i < 6; i++) add(i);\n"
            "    p = buf + 3;\n"
            "    add(0);\n"
            "    return sum;\n"
            "}\n"
        )
        self._both(src, 21 + 4)

    def test_block_static_counter(self) -> None:
        src = (
            "unsigned char tick(void) {\n"
            "    static unsigned char c [[c6502::zeropage]] = 10;\n"
            "    return ++c;\n"
            "}\n"
            "int main(void) { tick(); tick(); return tick(); }\n"
        )
        self._both(src, 13)

    def test_global_in_pool_range_survives_calls(self) -> None:
        # The global sits where a function's locals would otherwise
        # go, and keeps its value across calls that use locals.
        src = (
            "unsigned int keep [[c6502::zeropage(0x80)]] = 0;\n"
            "unsigned int f(unsigned int a) {\n"
            "    unsigned int x = a * 5, y = a + 7;\n"
            "    return x ^ y;\n"
            "}\n"
            "int main(void) {\n"
            "    keep = 1234;\n"
            "    unsigned int r = f(3) + f(4);\n"
            "    return keep - 1234 + (r == ((15u ^ 10u) + (20u ^ 11u)));\n"
            "}\n"
        )
        self._both(src, 1)


_HEADER = (
    "#include <stdint.h>\n"
    "extern uint8_t *src [[c6502::zeropage]];\n"
    "extern uint8_t total [[c6502::zeropage]];\n"
    "extern uint16_t big [[c6502::zeropage(0x90)]];\n"
    "uint8_t sum(uint8_t n);\n"
)
# Defines `src`, `total`, and `big`, and calls `sum`.
_UNIT_A = _HEADER + (
    "uint8_t *src [[c6502::zeropage]];\n"
    "uint8_t total [[c6502::zeropage]] = 5;\n"
    "uint16_t big [[c6502::zeropage(0x90)]] = 1000;\n"
    "uint8_t buf[4] = {1, 2, 3, 4};\n"
    "int main(void) { src = buf; return sum(4) + big; }\n"
)
# Defines `count`, which it places at $26 on its own, where unit A
# puts `src`; and reads and writes A's objects.
_UNIT_B = _HEADER + (
    "uint8_t count [[c6502::zeropage]] = 2;\n"
    "uint8_t sum(uint8_t n) {\n"
    "    for (uint8_t i = 0; i < n; i++) total += src[i] * count;\n"
    "    big += total;\n"
    "    return total;\n"
    "}\n"
)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestLink(unittest.TestCase):
    """`compile.py --link` places the zero-page objects of every unit
    for the whole program and merges their `__zp_init`s."""

    def _link(self, *sources: str) -> tuple[int, str, str]:
        """Compiles each source with `--optimize` and links them.
        Returns the exit code, the linked text, and stderr."""
        from compile import main as compile_main
        with tempfile.TemporaryDirectory() as tmp:
            paths = []
            for k, src in enumerate(sources):
                path = Path(tmp) / f"u{k}.asm"
                with patch("sys.stdin", io.StringIO(src)):
                    self.assertEqual(compile_main([
                        "compile.py", "-", "--codegen", "--optimize",
                        "-o", str(path),
                    ]), 0)
                paths.append(str(path))
            out = Path(tmp) / "out.asm"
            err = io.StringIO()
            with patch("sys.stderr", err):
                rc = compile_main(["compile.py", "--link", *paths,
                                   "-o", str(out)])
            text = out.read_text() if rc == 0 else ""
        return rc, text, err.getvalue()

    def test_objects_placed_for_the_whole_program(self) -> None:
        rc, text, err = self._link(_UNIT_A, _UNIT_B)
        self.assertEqual(rc, 0, err)
        equ = _equ(text)
        self.assertEqual(
            {n: equ[n] for n in ("src", "total", "count", "big")},
            {"src": 0x26, "total": 0x28, "count": 0x29, "big": 0x90},
        )
        # The slots keep off `big`'s bytes.
        slots = [a for n, a in equ.items() if n.startswith("__")]
        self.assertFalse({0x90, 0x91} & set(slots))

    def test_one_init_routine(self) -> None:
        rc, text, err = self._link(_UNIT_A, _UNIT_B)
        self.assertEqual(rc, 0, err)
        self.assertEqual(text.count("__zp_init:"), 1)
        init = text[text.index("__zp_init:"):]
        for store in ("STA   src+1", "STA   total", "STA   count",
                      "STA   big+1"):
            self.assertIn(store, init)

    @unittest.skipUnless(shutil.which("dasm"), "dasm not installed")
    def test_linked_program_runs(self) -> None:
        from sim.linked import build_linked_sim
        rc, text, err = self._link(_UNIT_A, _UNIT_B)
        self.assertEqual(rc, 0, err)
        # total = 5 + (1+2+3+4) * 2 = 25, big = 1000 + 25.
        self.assertEqual(build_linked_sim(text).run().return_int(),
                         25 + 1025)

    # Unit A leaves 4 bytes below $80; unit B promotes three busy
    # 2-byte counters, so one goes back in the data segment.
    _FULL = (
        "#include <stdint.h>\n"
        "uint8_t fill [[c6502::zeropage]][86];\n"
        "uint16_t count(void);\n"
        "int main(void) { fill[85] = 3; return count() + fill[85]; }\n"
    )
    _COUNTERS = (
        "#include <stdint.h>\n"
        "uint16_t a = 1, b = 2, c = 3;\n"
        "uint16_t count(void) {\n"
        "    for (uint8_t i = 0; i < 10; i++) { a += 1; b += a; c += b; }\n"
        "    return a + b + c;\n"
        "}\n"
    )

    def test_promoted_object_that_doesnt_fit_goes_back(self) -> None:
        rc, text, err = self._link(self._FULL, self._COUNTERS)
        self.assertEqual(rc, 0, err)
        equ = _equ(text)
        placed = [n for n in ("a", "b", "c") if n in equ]
        self.assertEqual(len(placed), 2)
        (back,) = {"a", "b", "c"} - set(placed)
        self.assertIn(f"\n{back}:\n   DC.B", text)
        self.assertNotIn(f"STA   {back}\n", text[text.index("__zp_init:"):])

    @unittest.skipUnless(shutil.which("dasm"), "dasm not installed")
    def test_promoted_object_that_doesnt_fit_runs(self) -> None:
        from sim.linked import build_linked_sim
        rc, text, err = self._link(self._FULL, self._COUNTERS)
        self.assertEqual(rc, 0, err)
        a, b, c = 1, 2, 3
        for _ in range(10):
            a += 1
            b += a
            c += b
        self.assertEqual(build_linked_sim(text).run().return_int(),
                         (a + b + c + 3) & 0xFFFF)

    def test_promoted_pointer_has_to_fit(self) -> None:
        # Both pointers are `(zp),Y` bases, and only one fits.
        full = ("unsigned char fill [[c6502::zeropage]][88];\n"
                "unsigned char sum(void);\n"
                "int main(void) { return sum(); }\n")
        pointers = (
            "unsigned char buf[4] = {1, 2, 3, 4};\n"
            "unsigned char *p = buf, *q = buf;\n"
            "unsigned char sum(void) {\n"
            "    unsigned char s = 0;\n"
            "    for (unsigned char i = 0; i < 4; i++) s += p[i] + q[i];\n"
            "    return s;\n"
            "}\n"
        )
        rc, _, err = self._link(full, pointers)
        self.assertEqual(rc, 1)
        self.assertIn("(zp),Y", err)

    def test_errors(self) -> None:
        main = "int main(void) { return 0; }\n"
        cases = {
            "declared but not defined": (
                "extern unsigned char g [[c6502::zeropage]];\n"
                "unsigned char f(void) { return g; }\n" + main,
                "no unit defines it",
            ),
            "defined twice": (
                "unsigned char g [[c6502::zeropage]];\n" + main,
                "defined in 2 units",
                "unsigned char g [[c6502::zeropage]];\n"
                "unsigned char f(void) { return g; }\n",
            ),
            "sizes differ": (
                "unsigned char g [[c6502::zeropage]];\n" + main,
                "different sizes",
                "extern unsigned int g [[c6502::zeropage]];\n"
                "unsigned int f(void) { return g; }\n",
            ),
            "addresses differ": (
                "unsigned char g [[c6502::zeropage(0x40)]];\n" + main,
                "different addresses",
                "extern unsigned char g [[c6502::zeropage(0x41)]];\n"
                "unsigned char f(void) { return g; }\n",
            ),
            "fixed objects overlap": (
                "unsigned int g [[c6502::zeropage(0x40)]];\n" + main,
                "overlaps",
                "unsigned char h [[c6502::zeropage(0x41)]];\n"
                "unsigned char f(void) { return h; }\n",
            ),
        }
        for what, (first, message, *rest) in cases.items():
            with self.subTest(what=what):
                rc, _, err = self._link(first, *rest)
                self.assertEqual(rc, 1)
                self.assertIn(message, err)


if __name__ == "__main__":
    unittest.main()
