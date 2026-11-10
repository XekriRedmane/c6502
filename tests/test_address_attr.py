"""Tests for `__attribute__((address(addr)))` on functions and
`extern` objects (`passes/zp_globals.py`, "Fixed addresses outside
zero page").

Covers parsing in each position, the rules the type checker
enforces (no definition, every declaration agreeing), placement
errors, the generated `EQU` lines, simulator runs against a stand-in
routine and stand-in memory, and linking several units that use the
same fixed names.
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
from passes.zp_globals import ZpGlobalError, allocate_zp_globals
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


def _codegen(src: str, optimize: bool = True) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("codegen", preprocess(src), optimize=optimize)


def _equ(asm: str) -> dict[str, int]:
    return {
        m.group(1): int(m.group(2), 16)
        for m in re.finditer(r"^(\S+)\tEQU\t\$([0-9A-F]+)$", asm, re.M)
    }


def _decls(src: str) -> dict[str, object]:
    out = {}
    for d in parse(src).declaration:
        if isinstance(d, c99_ast.VarDecl):
            out[d.var_decl.name] = d.var_decl
        elif isinstance(d, c99_ast.FunctionDecl):
            out[d.function_decl.name] = d.function_decl
    return out


def _symbols(src: str):
    from compile import _resolved
    from passes.type_checking import check_program
    _, symbols, types = check_program(_resolved(src, optimize=True))
    return symbols, types


class TestParse(unittest.TestCase):

    def test_function_and_object_forms(self) -> None:
        got = _decls(
            "__attribute__((zp_abi, address(0xFDED)))\n"
            "extern void COUT(register unsigned char c);\n"
            "extern unsigned char KBD __attribute__((address(0xC000)));\n"
            "__attribute__((address(0x36))) extern unsigned int CSW;\n"
        )
        self.assertEqual(got["COUT"].address, 0xFDED)
        self.assertEqual(got["COUT"].abi_annotation, "zp_abi")
        self.assertEqual(got["KBD"].placement, c99_ast.Absolute(0xC000))
        self.assertEqual(got["CSW"].placement, c99_ast.Absolute(0x36))

    def test_plain_function_has_no_address(self) -> None:
        got = _decls("void f(void);\n")
        self.assertIsNone(got["f"].address)

    def test_rejected_forms(self) -> None:
        bad = {
            "no address":
                "__attribute__((address)) void f(void);",
            "string address":
                '__attribute__((address("x"))) void f(void);',
            "past $FFFF":
                "__attribute__((address(0x10000))) void f(void);",
            "function definition":
                "__attribute__((address(0x1000))) void f(void) {}",
            "both placements":
                "extern unsigned char x "
                "__attribute__((zeropage(0x40), address(0x40)));",
            "placement in both positions":
                "__attribute__((address(0xC000))) extern unsigned char x "
                "__attribute__((address(0xC000)));",
            "duplicate":
                "__attribute__((address(1), address(2))) void f(void);",
        }
        for label, src in bad.items():
            with self.subTest(label):
                with self.assertRaises(ParserError):
                    parse(src)


class TestTypeCheck(unittest.TestCase):

    def test_definitions_rejected(self) -> None:
        bad = {
            "tentative definition":
                "unsigned char x __attribute__((address(0xC000)));\n",
            "initializer":
                "extern unsigned char x __attribute__((address(0xC000)))"
                " = 1;\n",
            "later definition":
                "extern unsigned char x __attribute__((address(0xC000)));\n"
                "unsigned char x = 3;\n",
            "function defined later":
                "__attribute__((address(0x1000))) void f(void);\n"
                "void f(void) {}\n",
            "function defined earlier":
                "void f(void) {}\n"
                "__attribute__((address(0x1000))) void f(void);\n",
            "conflicting function addresses":
                "__attribute__((address(0x1000))) void f(void);\n"
                "__attribute__((address(0x2000))) void f(void);\n",
            "conflicting object addresses":
                "extern unsigned char x __attribute__((address(0xC000)));\n"
                "extern unsigned char x __attribute__((address(0xC001)));\n",
            "block-scope static":
                "int main(void) { static unsigned char x "
                "__attribute__((address(0xC000))); return x; }\n",
            "automatic":
                "int main(void) { unsigned char x "
                "__attribute__((address(0xC000))); return x; }\n",
        }
        for label, src in bad.items():
            with self.subTest(label):
                with self.assertRaises(TypeCheckError):
                    _codegen(src)

    def test_address_accumulates_across_declarations(self) -> None:
        symbols, _ = _symbols(
            "void f(void);\n"
            "__attribute__((address(0x1000))) void f(void);\n"
            "void f(void);\n"
            "extern unsigned char x;\n"
            "extern unsigned char x __attribute__((address(0xC000)));\n"
        )
        self.assertEqual(symbols["f"].attrs.address, 0x1000)
        self.assertEqual(symbols["x"].attrs.address, 0xC000)

    def test_block_scope_extern(self) -> None:
        asm = _codegen(
            "int main(void) {\n"
            "  extern unsigned char x __attribute__((address(0xC000)));\n"
            "  return x;\n"
            "}\n"
        )
        self.assertEqual(_equ(asm)["x"], 0xC000)


class TestPlacement(unittest.TestCase):

    def _zp(self, src: str):
        symbols, types = _symbols(src)
        return allocate_zp_globals(symbols, types)

    def test_fixed_names(self) -> None:
        zp = self._zp(
            "__attribute__((address(0xFDED))) void COUT(unsigned char c);\n"
            "extern unsigned int w __attribute__((address(0xC010)));\n"
            "extern unsigned char m __attribute__((address(0x90)));\n"
        )
        self.assertEqual(zp.fixed, {"COUT": 0xFDED, "w": 0xC010})
        # Below $100 the object is a zero-page object: addressed,
        # reserved from the compiler's pool, and not initialized.
        self.assertEqual(zp.addrs, {"m": 0x90})
        self.assertIn(0x90, zp.reserved)
        self.assertEqual(zp.owned, frozenset())

    def test_placement_errors(self) -> None:
        bad = {
            "overlap":
                "extern unsigned int a __attribute__((address(0xC000)));\n"
                "extern unsigned char b __attribute__((address(0xC001)));\n",
            "past $FFFF":
                "extern unsigned int a __attribute__((address(0xFFFF)));\n",
            "runtime bytes":
                "extern unsigned char a __attribute__((address(0x24)));\n",
        }
        for label, src in bad.items():
            with self.subTest(label):
                with self.assertRaises(ZpGlobalError):
                    self._zp(src)


class TestCodegen(unittest.TestCase):

    def test_equs_and_absolute_store(self) -> None:
        src = (
            "__attribute__((zp_abi, address(0xFDED)))\n"
            "extern void COUT(register unsigned char c);\n"
            "extern volatile unsigned char scratch "
            "__attribute__((address(0x0779)));\n"
            "void f(void) { COUT(0xC1); scratch = 0x91; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _codegen(src, optimize=optimize)
                equ = _equ(asm)
                self.assertEqual(equ["COUT"], 0xFDED)
                self.assertEqual(equ["scratch"], 0x0779)
                self.assertIn("JSR   COUT", asm)
                self.assertIn("STA   scratch", asm)
                self.assertNotIn("DPTR", asm)
                # Nothing is defined for either name.
                self.assertNotRegex(asm, r"(?m)^(COUT|scratch):")
                self.assertIn("COUT\tEQU\t$FDED", asm)
                self.assertIn("scratch\tEQU\t$0779", asm)


class TestSim(unittest.TestCase):

    # A stand-in for a ROM routine, placed at $D000 before the run:
    #   CLC ; ADC #1 ; RTS
    _INC_ROUTINE = bytes([0x18, 0x69, 0x01, 0x60])

    def test_call_fixed_routine(self) -> None:
        # Only the optimized pipeline passes a `register` argument
        # in A; the unoptimized one passes every argument on the
        # soft stack.
        src = (
            "__attribute__((zp_abi, address(0xD000)))\n"
            "extern unsigned char inc(register unsigned char c);\n"
            "int main(void) {\n"
            "  unsigned char t = 0;\n"
            "  for (unsigned char i = 0; i < 3; i++) t += inc(i);\n"
            "  return t;\n"
            "}\n"
        )
        sim = build_sim(src, optimize=True)
        sim.mpu.memory[0xD000:0xD004] = self._INC_ROUTINE
        self.assertEqual(sim.run().return_int(), 1 + 2 + 3)

    def test_fixed_objects(self) -> None:
        src = (
            "extern volatile unsigned char port "
            "__attribute__((address(0xC800)));\n"
            "extern unsigned int word __attribute__((address(0xC810)));\n"
            "extern unsigned char tbl[4] __attribute__((address(0xC820)));\n"
            "int main(void) {\n"
            "  port = 5;\n"
            "  unsigned int t = word;\n"
            "  for (unsigned char i = 0; i < 4; i++) t += tbl[i];\n"
            "  tbl[0] = port + 1;\n"
            "  return t;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                mem = sim.mpu.memory
                mem[0xC810], mem[0xC811] = 0x34, 0x12
                mem[0xC820:0xC824] = bytes([1, 2, 3, 4])
                result = sim.run()
                self.assertEqual(result.return_int(), 0x1234 + 10)
                self.assertEqual(result.memory[0xC800], 5)
                self.assertEqual(result.memory[0xC820], 6)

    def test_address_of_fixed_object(self) -> None:
        src = (
            "extern unsigned char port __attribute__((address(0xC800)));\n"
            "int main(void) { return (int)&port; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got & 0xFFFF, 0xC800)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestLink(unittest.TestCase):

    _ROM = (
        "__attribute__((zp_abi, address(0xFDED)))\n"
        "extern void COUT(register unsigned char c);\n"
        "extern volatile unsigned char KBD "
        "__attribute__((address(0xC000)));\n"
    )

    def _compile(self, src: str, path: Path) -> None:
        from compile import main as compile_main
        with patch("sys.stdin", io.StringIO(src)):
            self.assertEqual(compile_main([
                "compile.py", "-", "--codegen", "--optimize",
                "-o", str(path),
            ]), 0)

    def _link(self, paths: list[Path], out: Path) -> tuple[int, str]:
        from compile import main as compile_main
        err = io.StringIO()
        with patch("sys.stderr", err):
            rc = compile_main(
                ["compile.py", "--link", *map(str, paths), "-o", str(out)],
            )
        return rc, err.getvalue()

    def test_fixed_names_bound_once(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            a, b, out = (Path(tmp) / n for n in ("a.asm", "b.asm", "o.asm"))
            self._compile(
                self._ROM + "void say(unsigned char c);\n"
                "int main(void) { say(KBD); return 0; }\n", a,
            )
            self._compile(
                self._ROM + "void say(unsigned char c) { COUT(c); }\n", b,
            )
            rc, err = self._link([a, b], out)
            self.assertEqual(rc, 0, err)
            text = out.read_text()
            self.assertEqual(text.count("COUT\tEQU\t$FDED"), 1)
            self.assertEqual(text.count("KBD\tEQU\t$C000"), 1)

    def test_conflicting_addresses_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            a, b, out = (Path(tmp) / n for n in ("a.asm", "b.asm", "o.asm"))
            self._compile(
                "__attribute__((address(0xFDED))) void out(unsigned char c);\n"
                "void f(void) { out(1); }\n", a,
            )
            self._compile(
                "__attribute__((address(0xFDF0))) void out(unsigned char c);\n"
                "void g(void) { out(2); }\n", b,
            )
            rc, err = self._link([a, b], out)
            self.assertEqual(rc, 1)
            self.assertIn("'out' is at $FDF0", err)


if __name__ == "__main__":
    unittest.main()
