"""Tests for ELF files (binutils/elf.py, which the simulator reads
executables with) on the objects `compile.py -c` assembles, and a
program compiled to objects, linked, and run in the simulator. The
tests that called the assembler and the linker's relaxation directly
are doctest cases of the C++ port, in
cpp/tests/test_binutils_assembler.cpp (docs/cpp_port.md, step 8, group
G6), and the tests of `compile.py -c`, a link, nm, and objdump on
files, and the checks of the assembler and the linked listings
against dasm, are in cpp/tests/test_binutils_source.cpp.
"""
from __future__ import annotations

import contextlib
import io
import os
import tempfile
import unittest
from pathlib import Path

from binutils import elf as E
from tests.cpp_build import needs_c6502cpp

_ROOT = Path(__file__).parent.parent


def _assemble(text: str, name: str = "t.asm") -> E.ElfFile:
    """The object `compile.py -c` assembles from `text`, written to a
    file called `name`."""
    import compile
    with tempfile.TemporaryDirectory() as tmp:
        src, out = Path(tmp) / name, Path(tmp) / "t.o"
        src.write_bytes(text.encode("utf-8"))
        err = io.StringIO()
        with contextlib.redirect_stderr(err):
            rc = compile.main(["compile.py", "-c", str(src), "-o", str(out)])
        assert rc == 0, err.getvalue()
        return E.read_file(out)


def _fn(*body: str, name: str = "f") -> str:
    lines = [f"{name}:", "   SUBROUTINE"]
    for b in body:
        lines.append(b if b.endswith(":") else f"   {b}")
    return "\n".join(lines) + "\n"


class TestElf(unittest.TestCase):

    def test_round_trip(self) -> None:
        text = (_ROOT / "examples" / "apply_bobble.asm").read_text()
        obj = _assemble(text, "apply_bobble.asm")
        data = E.write(obj)
        back = E.read(data)
        self.assertEqual(E.write(back), data)
        self.assertEqual(back.type, E.ET_REL)
        self.assertEqual(
            [(s.name, s.type, bytes(s.data)) for s in back.sections],
            [(s.name, s.type, bytes(s.data)) for s in obj.sections],
        )
        self.assertEqual(
            sorted((s.name, s.value, s.bind, s.type) for s in back.symbols),
            sorted((s.name, s.value, s.bind, s.type) for s in obj.symbols),
        )
        relocs = [(r.offset, r.type, r.symbol.name, r.addend)
                  for s in back.sections for r in s.relocs]
        self.assertIn((1, E.R_MOS_ADDR8, "__local_apply_bobble__slot", 0),
                      relocs)
        self.assertEqual(back.link_text(), obj.link_text())
        self.assertEqual(back.source_text(), text)

    def test_header(self) -> None:
        data = E.write(_assemble(_fn("RTS")))
        self.assertEqual(data[:4], b"\x7fELF")
        self.assertEqual(int.from_bytes(data[18:20], "little"), E.EM_MOS)
        with self.assertRaises(E.ElfError):
            E.read(b"not an elf file at all, no")

    def test_damaged_files(self) -> None:
        # Each of these raised struct.error, IndexError, or ValueError,
        # which nm.py, objdump.py, and compile.py printed as a
        # traceback.
        text = (_ROOT / "examples" / "apply_bobble.asm").read_text()
        data = E.write(_assemble(text, "apply_bobble.asm"))
        shoff = int.from_bytes(data[32:36], "little")
        shstrndx = 50

        def with_bytes(at: int, new: bytes) -> bytes:
            return data[:at] + new + data[at + len(new):]

        damaged = {
            "truncated": data[:len(data) // 2],
            "the section headers cut short": data[:-1],
            "the names' table index past the end": with_bytes(
                shstrndx, (0x7FFF).to_bytes(2, "little")),
            "a section's bytes past the end": with_bytes(
                shoff + 40 + 20, (0xFFFFFF).to_bytes(4, "little")),
        }
        link = E.LINK_SECTION.encode()
        at = data.index(b"; @zp-link-meta-begin")
        damaged["link metadata that isn't UTF-8"] = with_bytes(at, b"\xff")
        self.assertIn(link, data)
        for what, bad in damaged.items():
            with self.subTest(what), self.assertRaises(E.ElfError) as cm:
                E.read(bad, "x.o")
            self.assertEqual(
                str(cm.exception), "x.o: the ELF file is truncated or damaged",
            )
        import nm
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "x.o"
            path.write_bytes(damaged["truncated"])
            err = io.StringIO()
            with contextlib.redirect_stderr(err):
                self.assertEqual(nm.main(["nm.py", str(path)]), 1)
            self.assertEqual(
                err.getvalue(),
                f"nm.py: {path}: the ELF file is truncated or damaged\n",
            )

    def test_relocation_past_its_section(self) -> None:
        # The linker writes a relocation's field into its section's
        # bytes, and `_pointer_names` read the byte before an
        # R_MOS_ADDR8 field past them with an IndexError, so the reader
        # rejects such a file.
        for type_, offset in ((E.R_MOS_ADDR8, 9), (E.R_MOS_ADDR16, 3),
                              (E.R_MOS_ADDR8, 4)):
            obj = _assemble(_fn("RTS"))
            sec = next(s for s in obj.sections if s.name == ".text")
            sec.data = bytearray(4)
            ext = E.Symbol(name="ptr", bind=E.STB_GLOBAL)
            obj.symbols.append(ext)
            sec.relocs.append(E.Reloc(offset, type_, ext))
            with self.subTest(offset=offset), \
                    self.assertRaises(E.ElfError) as cm:
                E.read(E.write(obj), "x.o")
            self.assertEqual(
                str(cm.exception),
                "x.o: a relocation in .text is past the end of its bytes",
            )
        # A field that ends at the section's end is read.
        sec.relocs[-1] = E.Reloc(2, E.R_MOS_ADDR16, ext)
        self.assertEqual(len(E.read(E.write(obj)).sections[0].relocs), 1)


@needs_c6502cpp
class TestTools(unittest.TestCase):
    """Units compiled with `compile.py -c`, linked, and run."""

    _A = ("#include <stdint.h>\n"
          "uint8_t n;\n"
          "void bump(uint8_t k) { for (uint8_t i = 0; i < k; i++) n++; }\n")
    _B = ("#include <stdint.h>\n"
          "extern uint8_t n;\n"
          "void bump(uint8_t k);\n"
          "static uint8_t twice(uint8_t x) { return x + x; }\n"
          "int main(void) { bump(twice(3)); return n; }\n")

    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.dir = Path(self.tmp.name)
        for name, src in (("a", self._A), ("b", self._B)):
            (self.dir / f"{name}.c").write_text(src)
        self.cwd = os.getcwd()
        os.chdir(self.dir)

    def tearDown(self) -> None:
        os.chdir(self.cwd)
        self.tmp.cleanup()

    def _compile(self, *argv: str) -> int:
        import compile
        return compile.main(["compile.py", *argv])

    def test_link_and_run(self) -> None:
        from sim.linked import build_linked_sim
        self.assertEqual(self._compile("-O", "-c", "a.c", "b.c"), 0)
        self.assertEqual(self._compile("a.o", "b.o"), 0)
        exe = E.read_file(self.dir / "a.out")
        self.assertEqual(exe.type, E.ET_EXEC)
        self.assertEqual(exe.entry, exe.symbol("main").value)
        self.assertEqual(build_linked_sim(exe).run().return_int(), 6)


if __name__ == "__main__":
    unittest.main()
