"""Tests for linker scripts (`compile.py -T`) and
`[[c6502::section("name")]]`.

Covers the script parser (passes/link_script.py), the attribute in the
parser and type checker, each item's default section
(passes/sections.py), renaming internal names two units share, and
linked programs laid out by a script (passes/link_layout.py): run
in the simulator, with `.data` copied from ROM, a NOLOAD `.bss`
cleared, a section at a fixed address, page alignment, and a link
that fails on a region that overflows or a section outside the
6502's address space. It also covers functions
that end by jumping to another falling into it instead
(`link_layout.fall_through`), and `(NOREORDER)`.
"""
from __future__ import annotations

import shutil
import tempfile
import unittest
from pathlib import Path

from parser import ParserError
from passes.type_checking import TypeCheckError

_HAVE_PCPP = shutil.which("pcpp") is not None


def _codegen(src: str) -> str:
    from compile import _run_stage
    from preprocessor import preprocess
    return _run_stage("codegen", preprocess(src), optimize=True)


def _link(sources: list[str], script: str | None = None,
          extra: list[str] = ()):
    """Compiles each source with `-O` to an object and links them, with
    `script` as the linker script and `extra` on the link's command
    line. Returns the exit code, the linked program as text
    (`link_helpers.linked_text`), and stderr."""
    from tests.link_helpers import (
        compile_object, link_capture_stderr, linked_text,
    )
    with tempfile.TemporaryDirectory() as tmp:
        paths = []
        for k, src in enumerate(sources):
            path = Path(tmp) / f"u{k}.o"
            assert compile_object(src, path) == 0
            paths.append(path)
        ld = None
        if script is not None:
            ld = Path(tmp) / "prog.ld"
            ld.write_text(script)
        out = Path(tmp) / "out"
        rc, err = link_capture_stderr(paths, out, script=ld,
                                      extra=list(extra))
        text = linked_text(out) if rc == 0 else ""
    return rc, text, err


_ROM_SCRIPT = """
MEMORY
{
    ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x5800
    rom (rx)  : ORIGIN = 0x8000, LENGTH = 16K
}

SECTIONS
{
    .text : { *(.text.boot) *(.text .text.*) } > rom
    .rodata : { *(.rodata .rodata.*) } > rom
    .page ALIGN(256) : { *(.page) } > rom
    .data : { *(.data .data.*) } > ram AT> rom
    .bss (NOLOAD) : { *(.bss .bss.*) } > ram
    .vectors 0xBFFA : { KEEP(*(.vectors)) }
    __free_ram = ADDR(.bss) + SIZEOF(.bss);
}
"""

# `main` goes first; `squares` on a page of its own; `seed` is .data
# and `buf` .bss (an array, which promotion leaves alone); `vectors`
# is at $BFFA.
_UNIT_A = """
[[c6502::section(".text.boot")]] int main(void);
const unsigned char table[4] = {10, 20, 30, 40};
unsigned char squares [[c6502::section(".page")]][4] = {0, 1, 4, 9};
unsigned char seed = 7;
unsigned char buf[3];
unsigned char pick(unsigned char i);
[[c6502::section(".vectors")]]
int (*const vectors[3])(void) = {main, main, main};
int main(void) {
    return pick(2) + seed + squares[3] + buf[0] + buf[1] + buf[2];
}
"""
_UNIT_B = """
extern const unsigned char table[4];
static unsigned char hits = 100;
unsigned char pick(unsigned char i) { hits++; return table[i] + hits; }
"""
# 30 + 101 + 7 + 9, with buf cleared.
_EXPECTED = 147


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSectionAttribute(unittest.TestCase):

    def _items(self, src: str) -> dict[str, str]:
        asm = _codegen(src)
        out = {}
        for line in asm.splitlines():
            if line.startswith("; item "):
                parts = line.split()
                out[parts[2]] = dict(p.split("=") for p in parts[3:])["section"]
        return out

    def test_default_sections(self) -> None:
        items = self._items(
            "const unsigned char k[2] = {1, 2};\n"
            "unsigned char z[2];\n"
            "unsigned char d[2] = {3, 4};\n"
            "int main(void) { const char *s = \"hi\"; "
            "return k[0] + z[0] + d[0] + s[0]; }\n"
        )
        self.assertEqual(items["main"], ".text")
        self.assertEqual(items["k"], ".rodata")
        self.assertEqual(items["z"], ".bss")
        self.assertEqual(items["d"], ".data")
        self.assertEqual(items["str@0"], ".rodata")

    def test_named_sections(self) -> None:
        items = self._items(
            "[[c6502::section(\".text.boot\")]] int main(void);\n"
            "unsigned char t [[c6502::section(\".page\")]][2] = {1, 2};\n"
            "int f(void) { static int n [[c6502::section(\".noinit\")]];"
            " return ++n; }\n"
            "int main(void) { return t[0] + f(); }\n"
        )
        self.assertEqual(items["main"], ".text.boot")
        self.assertEqual(items["t"], ".page")
        self.assertEqual(items["@0.n"], ".noinit")

    def test_errors(self) -> None:
        main = "int main(void) { return 0; }\n"
        for src, error in [
            ("int g [[c6502::section(1)]];\n", ParserError),
            ("int g [[c6502::section(\"1x\")]];\n", ParserError),
            ("int f(void) { int x [[c6502::section(\".d\")]] = 1;"
             " return x; }\n", TypeCheckError),
            ("unsigned char g [[c6502::zeropage, c6502::section(\".d\")]];\n",
             TypeCheckError),
            ("extern unsigned char g [[c6502::address(0xC000), c6502::"
             "section(\".d\")]];\n", TypeCheckError),
            ("int g [[c6502::section(\".a\")]];\n"
             "int g [[c6502::section(\".b\")]];\n", TypeCheckError),
        ]:
            with self.subTest(src=src):
                with self.assertRaises(error):
                    _codegen(src + main)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestScriptedLink(unittest.TestCase):

    def test_sections_laid_out(self) -> None:
        rc, text, err = _link([_UNIT_A, _UNIT_B], _ROM_SCRIPT)
        self.assertEqual(rc, 0, err)
        self.assertIn("   SEG   text\n   ORG   $8000\n", text)
        self.assertIn("   SEG.U bss\n", text)
        self.assertIn("   RORG  $0800\n", text)
        self.assertIn("   ORG   $BFFA\n", text)
        # `main`, from .text.boot, comes before `pick`.
        self.assertLess(text.index("\nmain:"), text.index("\npick:"))

    def test_sections_by_file(self) -> None:
        # The second unit's code goes first, by its file's name.
        script = _ROM_SCRIPT.replace(
            ".text : { *(.text.boot) *(.text .text.*) } > rom",
            ".first : { u1.o(.text .text.*) } > rom\n"
            "    .text : { *(.text.boot) *(.text .text.*) } > rom",
        )
        rc, text, err = _link([_UNIT_A, _UNIT_B], script)
        self.assertEqual(rc, 0, err)
        self.assertLess(text.index("\npick:"), text.index("\nmain:"))
        self.assertIn("   SEG   first\n   ORG   $8000\n", text)

    def test_runs_with_data_copied_and_bss_cleared(self) -> None:
        from sim.linked import build_linked_sim
        rc, text, err = _link([_UNIT_A, _UNIT_B], _ROM_SCRIPT)
        self.assertEqual(rc, 0, err)
        sim = build_linked_sim(text.exe)
        sym = sim.symbols
        self.assertEqual(sym["main"], 0x8000)
        self.assertEqual(sym["squares"] & 0xFF, 0)
        self.assertEqual(sym["__page_start"], sym["squares"])
        self.assertEqual(sym["seed"], 0x0800)
        self.assertGreaterEqual(sym["__data_load"], 0x8000)
        self.assertEqual(sym["vectors"], 0xBFFA)
        self.assertEqual(sym["__free_ram"], sym["__bss_end"])
        # RAM holds garbage until the startup code copies and clears.
        sim.mpu.memory[0x0800:0x0900] = bytes([0xFF]) * 0x100
        self.assertEqual(sim.run().return_int(), _EXPECTED)
        mem = sim.mpu.memory
        self.assertEqual(mem[0xBFFA] | mem[0xBFFB] << 8, 0x8000)

    def test_region_overflow_rejected(self) -> None:
        small = _ROM_SCRIPT.replace("LENGTH = 16K", "LENGTH = 0x40")
        rc, _, err = _link([_UNIT_A, _UNIT_B], small)
        self.assertEqual(rc, 1)
        self.assertRegex(
            err, r"section \.\w+ runs past the end of region rom by \d+ bytes",
        )

    def test_origin_outside_address_space_rejected(self) -> None:
        # The last is too wide for the C++ linker's 64-bit addresses.
        for origin, shown in (("0x10000", "$10000"), ("-1", "-$0001"),
                              ("0x10000000000000000", "$10000000000000000")):
            with self.subTest(origin=origin):
                rc, _, err = _link([_UNIT_A, _UNIT_B],
                                   extra=[f"-Ttext={origin}"])
                self.assertEqual(rc, 1)
                self.assertIn(
                    f"link error: the origin {shown} is outside the "
                    f"6502's address space ($0000-$FFFF)", err,
                )

    def test_section_past_the_top_of_memory_rejected(self) -> None:
        rc, _, err = _link([_UNIT_A, _UNIT_B], extra=["-Ttext=0xFFF0"])
        self.assertEqual(rc, 1)
        self.assertRegex(
            err, r"section \.text runs at \$FFF0 with \d+ bytes, outside "
                 r"the 6502's address space \(\$0000-\$FFFF\)",
        )

    def test_load_address_outside_address_space_rejected(self) -> None:
        script = _ROM_SCRIPT.replace("ORIGIN = 0x8000", "ORIGIN = 0xFFF0")
        script = script.replace("LENGTH = 16K", "LENGTH = 1K")
        rc, _, err = _link([_UNIT_A, _UNIT_B], script)
        self.assertEqual(rc, 1)
        self.assertRegex(
            err, r"section \.\w+ (runs|loads) at \$[0-9A-F]+ with \d+ "
                 r"bytes, outside the 6502's address space",
        )

    def test_unplaced_section_rejected(self) -> None:
        script = _ROM_SCRIPT.replace("*(.page)", "*(.nothing)")
        rc, _, err = _link([_UNIT_A, _UNIT_B], script)
        self.assertEqual(rc, 1)
        self.assertIn("no output section in the script takes `squares`", err)

    def test_initialized_object_in_noload_rejected(self) -> None:
        script = _ROM_SCRIPT.replace("*(.bss .bss.*)", "*(.bss .bss.* .data)")
        script = script.replace(".data : { *(.data .data.*) }",
                                ".data : { *(.data.*) }")
        rc, _, err = _link([_UNIT_A, _UNIT_B], script)
        self.assertEqual(rc, 1)
        self.assertIn("NOLOAD", err)

    def test_bad_script_reported(self) -> None:
        rc, _, err = _link([_UNIT_B], "SECTIONS { .text : { *(.text) } > r }")
        self.assertEqual(rc, 1)
        self.assertIn("doesn't define", err)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestFallThroughLink(unittest.TestCase):
    """A function of one unit that ends by calling one of another's
    falls into it once linked."""

    _MAIN = ("unsigned char step(unsigned char x);\n"
             "unsigned char finish(unsigned char x);\n"
             "unsigned char other(void);\n"
             "int main(void) { return finish(1) + step(2) + other(); }\n"
             "unsigned char step(unsigned char x) {\n"
             "    return finish(x + 3);\n"
             "}\n")
    # `other` comes between `step` and `finish` in the script's order.
    _FINISH = ("unsigned char other(void) { return 5; }\n"
               "unsigned char finish(unsigned char x) {\n"
               "    return x * 2 + 1;\n"
               "}\n")
    _SCRIPT = """
MEMORY { ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x5800 }
SECTIONS
{
    .text%s : { *(.text .text.*) } > ram
    .rodata : { *(.rodata .rodata.*) } > ram
    .data : { *(.data .data.*) } > ram
    .bss : { *(.bss .bss.*) } > ram
}
"""

    def test_falls_into_other_unit(self) -> None:
        rc, text, err = _link([self._MAIN, self._FINISH], self._SCRIPT % "")
        self.assertEqual(rc, 0, err)
        self.assertNotIn("JMP   finish", text)
        step = text.index("\nstep:")
        finish = text.index("\nfinish:")
        self.assertLess(step, finish)
        self.assertNotIn("RTS", text[step:finish])

    def test_noreorder_keeps_jump(self) -> None:
        rc, text, err = _link([self._MAIN, self._FINISH],
                              self._SCRIPT % " (NOREORDER)")
        self.assertEqual(rc, 0, err)
        self.assertIn("JMP   finish", text)

    def test_runs(self) -> None:
        from sim.linked import build_linked_sim
        rc, text, err = _link([self._MAIN, self._FINISH], self._SCRIPT % "")
        self.assertEqual(rc, 0, err)
        self.assertEqual(build_linked_sim(text.exe).run().return_int(),
                         3 + 11 + 5)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestNames(unittest.TestCase):
    """Internal names two units share are renamed in one of them."""

    _A = ("static unsigned char hits = 1;\n"
          "unsigned char b(void);\n"
          "int main(void) { const char *s = \"A\"; hits++; "
          "return hits + s[0] + b(); }\n")
    _B = ("static unsigned char hits = 10;\n"
          "unsigned char b(void) { const char *s = \"B\"; hits++; "
          "return hits + s[0]; }\n")

    def test_renamed(self) -> None:
        rc, text, err = _link([self._A, self._B])
        self.assertEqual(rc, 0, err)
        self.assertIn("str@0.1", text)

    def test_runs(self) -> None:
        from sim.linked import build_linked_sim
        rc, text, err = _link([self._A, self._B])
        self.assertEqual(rc, 0, err)
        self.assertEqual(build_linked_sim(text.exe).run().return_int(),
                         2 + ord("A") + 11 + ord("B"))

    def test_external_name_defined_twice(self) -> None:
        rc, _, err = _link(["int g = 1;\nint main(void) { return g; }\n",
                            "int g = 2;\n"])
        self.assertEqual(rc, 1)
        self.assertIn("`g` is defined in multiple TUs", err)


if __name__ == "__main__":
    unittest.main()
