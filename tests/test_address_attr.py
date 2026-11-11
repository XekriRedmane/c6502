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
                # No pointer is staged in DPTR for the store.
                self.assertNotRegex(asm, r"STA\s+DPTR|\(DPTR\)")
                # Nothing is defined for either name.
                self.assertNotRegex(asm, r"(?m)^(COUT|scratch):")
                self.assertIn("COUT\tEQU\t$FDED", asm)
                self.assertIn("scratch\tEQU\t$0779", asm)
                # The argument is in A, so COUT's parameter slot is
                # never written and gets no EQU.
                self.assertNotIn("__zpabi_COUT", asm.split("@zp-link")[0])

    def test_without_zp_abi_uses_soft_stack(self) -> None:
        # The default zp_abi promotion under --optimize doesn't
        # apply to a function with a fixed address.
        src = (
            "__attribute__((address(0x1000))) void f(unsigned char c);\n"
            "void g(unsigned char c) { f(c); }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                asm = _codegen(src, optimize=optimize)
                call = asm.index("JSR   f")
                self.assertIn("STA   (SSP),Y", asm[:call])
                self.assertNotIn("__zpabi_f_", asm)


class TestConventionErrors(unittest.TestCase):

    def test_rejected_declarations(self) -> None:
        from passes.abi_selection import AbiSelectionError
        bad = {
            "register without zp_abi": (
                "__attribute__((address(0x1000)))\n"
                "void f(register unsigned char c);\n"
                "void g(void) { f(1); }\n"
            ),
            "parameter not in a register": (
                "__attribute__((zp_abi, address(0x1000)))\n"
                "void f(register unsigned char a, unsigned char b);\n"
                "void g(void) { f(1, 2); }\n"
            ),
            "pointer result": (
                "__attribute__((zp_abi, address(0x1000)))\n"
                "unsigned char *f(void);\n"
                "void g(void) { f(); }\n"
            ),
            "4-byte result": (
                "__attribute__((zp_abi, address(0x1000)))\n"
                "unsigned long f(void);\n"
                "void g(void) { f(); }\n"
            ),
            "address taken": (
                "__attribute__((zp_abi, address(0x1000)))\n"
                "void f(register unsigned char c);\n"
                "void g(void) { void (*p)(unsigned char) = f; p(1); }\n"
            ),
        }
        for label, src in bad.items():
            for optimize in (False, True):
                with self.subTest(label, optimize=optimize):
                    with self.assertRaises(AbiSelectionError):
                        _codegen(src, optimize=optimize)


class TestSim(unittest.TestCase):

    # A stand-in for a ROM routine, placed at $D000 before the run:
    #   CLC ; ADC #1 ; RTS
    _INC_ROUTINE = bytes([0x18, 0x69, 0x01, 0x60])

    # Swaps the bytes of the 2-byte argument in A (low) and X (high)
    # and returns the result the same way. $D0F0 is scratch.
    #   STA $D0F0 ; TXA ; LDX $D0F0 ; RTS
    _SWAP_ROUTINE = bytes([0x8D, 0xF0, 0xD0, 0x8A, 0xAE, 0xF0, 0xD0, 0x60])

    def test_call_fixed_routine(self) -> None:
        src = (
            "__attribute__((zp_abi, address(0xD000)))\n"
            "extern unsigned char inc(register unsigned char c);\n"
            "int main(void) {\n"
            "  unsigned char t = 0;\n"
            "  for (unsigned char i = 0; i < 3; i++) t += inc(i);\n"
            "  return t;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                sim.mpu.memory[0xD000:0xD004] = self._INC_ROUTINE
                self.assertEqual(sim.run().return_int(), 1 + 2 + 3)

    def test_carry_flag_result(self) -> None:
        # A routine that reports its result in the carry flag:
        #   CMP #5 ; RTS        C set iff A >= 5
        src = (
            "__attribute__((zp_abi, return_bool_in_carry, address(0xD000)))\n"
            "extern _Bool at_least_5(register unsigned char a);\n"
            "int main(void) {\n"
            "  int n = 0;\n"
            "  for (unsigned char i = 0; i < 9; i++)\n"
            "    if (at_least_5(i)) n += 10; else n += 1;\n"
            "  return n;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                sim.mpu.memory[0xD000:0xD003] = bytes([0xC9, 0x05, 0x60])
                self.assertEqual(sim.run().return_int(), 4 * 10 + 5)

    def test_two_byte_argument_and_result(self) -> None:
        src = (
            "__attribute__((zp_abi, address(0xD000)))\n"
            "extern unsigned int swap(register unsigned int v);\n"
            "unsigned int k = 0x1234;\n"
            "int main(void) { return swap(k) + 1; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                sim.mpu.memory[0xD000:0xD008] = self._SWAP_ROUTINE
                self.assertEqual(sim.run().return_int(), 0x3412 + 1)

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

    def test_poll_key(self) -> None:
        # The example in docs/attributes.md. Clearing the strobe on
        # one path used to leave A = 0 at the return under
        # --optimize, because two passes lost track of the `LDA #0`
        # inside a volatile `STA`.
        src = (
            "extern volatile unsigned char KBD "
            "__attribute__((address(0xC000)));\n"
            "extern volatile unsigned char KBDSTRB "
            "__attribute__((address(0xC010)));\n"
            "unsigned char poll_key(void) {\n"
            "  unsigned char k = KBD;\n"
            "  if (k & 0x80) KBDSTRB = 0;\n"
            "  return k;\n"
            "}\n"
            "int main(void) { return poll_key(); }\n"
        )
        for optimize in (False, True):
            for key in (0xC1, 0x41):
                with self.subTest(optimize=optimize, key=key):
                    sim = build_sim(src, optimize=optimize)
                    sim.mpu.memory[0xC000] = key
                    sim.mpu.memory[0xC010] = 0x55
                    result = sim.run()
                    self.assertEqual(result.return_int(), key)
                    self.assertEqual(
                        result.memory[0xC010],
                        0 if key & 0x80 else 0x55,
                    )

    def test_wait_through_pointer(self) -> None:
        # With no key in KBD the loop has to keep waiting, so the run
        # reaches the cycle cap. It used to test a stale A and exit.
        src = (
            "int main(void) {\n"
            "  while (!(*(volatile unsigned char *)0xC000 & 0x80)) ;\n"
            "  return 7;\n"
            "}\n"
        )
        for optimize in (False, True):
            for key, waits in ((0x00, True), (0xC1, False)):
                with self.subTest(optimize=optimize, key=key):
                    sim = build_sim(src, optimize=optimize)
                    sim.mpu.memory[0xC000] = key
                    result = sim.run(max_cycles=20_000)
                    self.assertEqual(result.timed_out, waits)
                    if not waits:
                        self.assertEqual(result.return_int(), 7)

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
