"""Tests for zero-page globals (`__attribute__((zeropage))`,
`passes/zp_globals.py`).

Covers parsing of both forms in both positions, the static-storage
rule, placement (fixed and automatic, overlaps, ranges, `extern`),
the generated code (EQU binding, no storage, `__zp_init`, `(zp),Y`
on a zero-page pointer, pinned bytes kept out of the compiler's
pools), and simulator runs of initialization and aliasing in both
pipelines.
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
            "__attribute__((zeropage)) unsigned char a;\n"
            "unsigned char b __attribute__((zeropage(0xBA)));\n"
            "unsigned char c;\n"
        )
        self.assertEqual(got["a"], c99_ast.ZeroPage(address=None))
        self.assertEqual(got["b"], c99_ast.ZeroPage(address=0xBA))
        self.assertIsNone(got["c"])

    def test_prefix_applies_to_every_declarator(self) -> None:
        got = _placements("__attribute__((zeropage)) unsigned char a, b;\n")
        self.assertEqual(got["a"], c99_ast.ZeroPage(address=None))
        self.assertEqual(got["b"], c99_ast.ZeroPage(address=None))

    def test_rejected_forms(self) -> None:
        bad = [
            "__attribute__((zeropage)) int f(void) { return 0; }\n",
            "__attribute__((zp_abi)) unsigned char a;\n",
            'unsigned char a __attribute__((zeropage("x")));\n',
            "unsigned char a __attribute__((zeropage(0x100)));\n",
            "__attribute__((zeropage)) unsigned char a "
            "__attribute__((zeropage));\n",
            "unsigned char a __attribute__((nonsense));\n",
        ]
        for src in bad:
            with self.subTest(src=src):
                with self.assertRaises(ParserError):
                    parse(src)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestPlacement(unittest.TestCase):

    def test_automatic_addresses_follow_fixed_ones(self) -> None:
        asm = _codegen(
            "unsigned char a __attribute__((zeropage));\n"
            "unsigned int b __attribute__((zeropage(0x27)));\n"
            "unsigned char c[3] __attribute__((zeropage));\n"
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
            "__attribute__((zeropage)); a = 1; return a; }\n",
            "int main(void) { for (__attribute__((zeropage)) int i = 0; "
            "i < 2; i++) ; return 0; }\n",
        ]
        for src in bad:
            with self.subTest(src=src):
                with self.assertRaises((TypeCheckError, ParserError)):
                    _codegen(src)

    def test_placement_errors(self) -> None:
        bad = {
            "overlap": "unsigned int a __attribute__((zeropage(0x40)));\n"
                       "unsigned char b __attribute__((zeropage(0x41)));\n",
            "runtime bytes": "unsigned char a __attribute__((zeropage(0x24)));\n",
            "past $FF": "unsigned int a __attribute__((zeropage(0xFF)));\n",
            "extern without address":
                "extern unsigned char a __attribute__((zeropage));\n",
            "full": "unsigned char a[91] __attribute__((zeropage));\n",
            "two addresses":
                "unsigned char a __attribute__((zeropage(0x40)));\n"
                "unsigned char a __attribute__((zeropage(0x41)));\n",
        }
        for what, decls in bad.items():
            with self.subTest(what=what):
                with self.assertRaises(TypeCheckError):
                    _codegen(decls + "int main(void) { return a; }\n")

    def test_errors_are_zp_global_errors(self) -> None:
        with self.assertRaises(ZpGlobalError):
            _codegen(
                "unsigned char a[91] __attribute__((zeropage));\n"
                "int main(void) { return a[0]; }\n"
            )

    def test_extern_with_address_is_reserved_not_initialized(self) -> None:
        asm = _codegen(
            "extern unsigned char k __attribute__((zeropage(0x90)));\n"
            "int main(void) { return k; }\n"
        )
        self.assertEqual(_equ(asm)["k"], 0x90)
        self.assertNotIn("__zp_init", asm)

    def test_redeclaration_accumulates(self) -> None:
        asm = _codegen(
            "extern unsigned char a __attribute__((zeropage(0x50)));\n"
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
            "unsigned char *p __attribute__((zeropage));\n"
            "int main(void) { p = buf; return p[2]; }\n"
        )
        self.assertIn("(p),Y", asm)
        self.assertNotIn("DPTR", asm.split("main:")[1])

    def test_init_routine(self) -> None:
        asm = _codegen(
            "unsigned int w __attribute__((zeropage)) = 0x1234;\n"
            "unsigned char z __attribute__((zeropage));\n"
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
            "unsigned char g[16] __attribute__((zeropage(0x80)));\n"
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
            "unsigned char tab[3] __attribute__((zeropage)) = {7, 8, 9};\n"
            "struct s st __attribute__((zeropage)) = {1, 300};\n"
            "unsigned long l __attribute__((zeropage)) = 70000;\n"
            "unsigned char z __attribute__((zeropage));\n"
            "unsigned char other = 5;\n"
            "unsigned char *ptr __attribute__((zeropage)) = &other;\n"
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
            "unsigned char n __attribute__((zeropage(0xA0))) = 1;\n"
            "unsigned char *p __attribute__((zeropage));\n"
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
            "unsigned char *p __attribute__((zeropage(0xBA)));\n"
            "unsigned char sum __attribute__((zeropage));\n"
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
            "    static unsigned char c __attribute__((zeropage)) = 10;\n"
            "    return ++c;\n"
            "}\n"
            "int main(void) { tick(); tick(); return tick(); }\n"
        )
        self._both(src, 13)

    def test_global_in_pool_range_survives_calls(self) -> None:
        # The global sits where a function's locals would otherwise
        # go, and keeps its value across calls that use locals.
        src = (
            "unsigned int keep __attribute__((zeropage(0x80))) = 0;\n"
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


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestLinkRejects(unittest.TestCase):

    def test_link_with_zp_global_fails(self) -> None:
        from compile import main as compile_main
        src = (
            "unsigned char g __attribute__((zeropage));\n"
            "unsigned char f(void) { return g; }\n"
        )
        with tempfile.TemporaryDirectory() as tmp:
            a = Path(tmp) / "a.asm"
            out = Path(tmp) / "out.asm"
            with patch("sys.stdin", io.StringIO(src)):
                self.assertEqual(compile_main([
                    "compile.py", "-", "--codegen", "--optimize",
                    "-o", str(a),
                ]), 0)
            err = io.StringIO()
            with patch("sys.stderr", err):
                rc = compile_main([
                    "compile.py", "--link", str(a), "-o", str(out),
                ])
            self.assertEqual(rc, 1)
            self.assertIn("zero-page globals", err.getvalue())


if __name__ == "__main__":
    unittest.main()
