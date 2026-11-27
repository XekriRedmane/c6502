"""Tests for linker scripts (`compile.py -T`) and
`[[c6502::section("name")]]`.

Covers linked programs laid out by a script
(cpp/src/passes/link_layout.h), run in the simulator with `.data`
copied from ROM, a NOLOAD `.bss` cleared, a section at a fixed
address, and page alignment, a function that ends by jumping to
another unit's falling into it (`link_layout.fall_through`), and
internal names two units share. The tests that read the `-S` text or
the linked program's text, and the link errors, are doctest cases in
cpp/tests/test_linker_script_source.cpp.
"""
from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tests.cpp_build import needs_c6502cpp


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


@needs_c6502cpp
class TestScriptedLink(unittest.TestCase):

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


@needs_c6502cpp
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

    def test_runs(self) -> None:
        from sim.linked import build_linked_sim
        rc, text, err = _link([self._MAIN, self._FINISH], self._SCRIPT % "")
        self.assertEqual(rc, 0, err)
        self.assertEqual(build_linked_sim(text.exe).run().return_int(),
                         3 + 11 + 5)


@needs_c6502cpp
class TestNames(unittest.TestCase):
    """Internal names two units share are renamed in one of them."""

    _A = ("static unsigned char hits = 1;\n"
          "unsigned char b(void);\n"
          "int main(void) { const char *s = \"A\"; hits++; "
          "return hits + s[0] + b(); }\n")
    _B = ("static unsigned char hits = 10;\n"
          "unsigned char b(void) { const char *s = \"B\"; hits++; "
          "return hits + s[0]; }\n")

    def test_runs(self) -> None:
        from sim.linked import build_linked_sim
        rc, text, err = _link([self._A, self._B])
        self.assertEqual(rc, 0, err)
        self.assertEqual(build_linked_sim(text.exe).run().return_int(),
                         2 + ord("A") + 11 + ord("B"))


if __name__ == "__main__":
    unittest.main()
