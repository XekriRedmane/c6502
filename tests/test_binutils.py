"""Tests for the object-file tools: ELF files (binutils/elf.py) on the
objects `compile.py -c` assembles, `nm.py`, `objdump.py`, and the
driver's object and link modes. The tests that called the assembler
and the linker's relaxation directly are doctest cases of the C++ port,
in cpp/tests/test_binutils_assembler.cpp (docs/cpp_port.md, step 8,
group G6).

`TestAgainstDasm` checks the assembler against dasm, byte for byte, on
every checked-in example, and checks that dasm assembles the listing
of each linked example into the executable's bytes. It runs only
where dasm is installed.
"""
from __future__ import annotations

import contextlib
import io
import os
import re
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

from binutils import elf as E
from tests.no_inlining import no_inlining

_ROOT = Path(__file__).parent.parent
_HAVE_PCPP = shutil.which("pcpp") is not None
_DASM = shutil.which("dasm")


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


def _image(obj: E.ElfFile, symbols: dict[str, int],
           origin: int = 0x0800) -> bytes:
    """`obj`'s loaded sections one after another from `origin`, with
    their relocations applied as the linker applies them, and with the
    names the object leaves undefined given by `symbols`."""
    alloc = [s for s in obj.sections if s.flags & E.SHF_ALLOC]
    addrs = {}
    addr = origin
    for s in alloc:
        addrs[id(s)] = addr
        addr += s.size

    def value(sym: E.Symbol) -> int:
        if not sym.defined:
            return symbols[sym.name]
        if sym.section == E.SHN_ABS:
            return sym.value
        return addrs[id(sym.section)] + sym.value

    out = bytearray()
    for s in alloc:
        if s.type == E.SHT_NOBITS:
            out += bytes(s.size)
            continue
        data = bytearray(s.data)
        for r in s.relocs:
            v = value(r.symbol) + r.addend
            if r.type == E.R_MOS_ADDR16:
                data[r.offset:r.offset + 2] = v.to_bytes(2, "little")
            elif r.type in (E.R_MOS_ADDR8, E.R_MOS_IMM8, E.R_MOS_ADDR16_LO):
                data[r.offset] = v & 0xFF
            elif r.type == E.R_MOS_ADDR16_HI:
                data[r.offset] = (v >> 8) & 0xFF
            elif r.type == E.R_MOS_PCREL_8:
                data[r.offset] = (v - (addrs[id(s)] + r.offset)) & 0xFF
            else:
                raise AssertionError(f"relocation type {r.type}")
        out += data
    return bytes(out)


# The read-modify-write instructions, which have zp,X and abs,X forms.
_RMW_OPS = ("INC", "DEC", "ASL", "LSR", "ROL", "ROR")


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


def _run(module, argv: list[str]) -> tuple[int, str]:
    out = io.StringIO()
    with contextlib.redirect_stdout(out):
        rc = module.main(argv)
    return rc, out.getvalue()


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestTools(unittest.TestCase):
    """`compile.py -c`, linking, `nm.py`, and `objdump.py` on files."""

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

    def test_object_named_after_source(self) -> None:
        self.assertEqual(self._compile("-O", "-c", "a.c", "b.c"), 0)
        for name in ("a.o", "b.o"):
            obj = E.read_file(self.dir / name)
            self.assertEqual(obj.type, E.ET_REL)
            self.assertIsNotNone(obj.link_text())

    def test_link_and_run(self) -> None:
        from sim.linked import build_linked_sim
        self.assertEqual(self._compile("-O", "-c", "a.c", "b.c"), 0)
        self.assertEqual(self._compile("a.o", "b.o"), 0)
        exe = E.read_file(self.dir / "a.out")
        self.assertEqual(exe.type, E.ET_EXEC)
        self.assertEqual(exe.entry, exe.symbol("main").value)
        self.assertEqual(build_linked_sim(exe).run().return_int(), 6)

    def test_cross_unit_zero_page_reference_relaxed(self) -> None:
        # `a.c` promotes `n` into zero page; `b.c` only declares it, so
        # it reaches `n` with absolute addressing, which the link
        # shortens to zero page.
        self.assertEqual(self._compile("-O", "-c", "a.c", "b.c"), 0)
        self.assertIn("weight=", E.read_file("a.o").link_text())
        b = E.read_file("b.o")
        main = b.symbol("main").section
        self.assertIn(0xAD, bytes(main.data))  # LDA abs n
        self.assertEqual(self._compile("a.o", "b.o", "-o", "prog"), 0)
        exe = E.read_file("prog")
        n = exe.symbol("n").value
        self.assertLess(n, 0x100)
        m = exe.symbol("main")
        seg = next(s for s in exe.segments
                   if s.vaddr <= m.value < s.vaddr + s.memsz)
        code = seg.data[m.value - seg.vaddr:m.value - seg.vaddr + m.size]
        self.assertIn(bytes([0xA5, n]), code)

    def test_origin_and_defsym(self) -> None:
        self.assertEqual(self._compile(
            "-O", "a.c", "b.c", "-Ttext=0x1000", "-o", "prog",
        ), 0)
        exe = E.read_file("prog")
        self.assertEqual(exe.sections[0].addr, 0x1000)
        src = "extern int rom(void);\nint main(void) { return rom(); }\n"
        Path("c.c").write_text(src)
        err = io.StringIO()
        with contextlib.redirect_stderr(err):
            self.assertEqual(self._compile("-O", "c.c"), 1)
        self.assertIn("undefined symbol `rom`", err.getvalue())
        self.assertEqual(self._compile(
            "-O", "c.c", "--defsym", "rom=$E000", "-o", "prog",
        ), 0)
        self.assertEqual(E.read_file("prog").symbol("rom").value, 0xE000)

    @no_inlining()
    def test_nm(self) -> None:
        # `twice` stays a function of its own, a local symbol.
        import nm
        self.assertEqual(self._compile("-O", "-c", "b.c"), 0)
        rc, out = _run(nm, ["nm.py", "b.o"])
        self.assertEqual(rc, 0)
        lines = out.splitlines()
        self.assertIn("0000 T main", lines)
        self.assertIn("0000 t twice", lines)
        self.assertIn("     U bump", lines)
        self.assertIn("     U n", lines)
        self.assertNotIn("$c", out)
        rc, out = _run(nm, ["nm.py", "-g", "-u", "b.o"])
        self.assertNotIn("main", out)
        self.assertIn("U n", out)
        self.assertEqual(self._compile("-O", "a.c", "b.c", "-o", "prog"), 0)
        rc, out = _run(nm, ["nm.py", "-n", "prog"])
        names = [line.split()[-1] for line in out.splitlines()]
        # a.c's `bump` comes first, as a.o does.
        self.assertLess(names.index("bump"), names.index("main"))
        self.assertRegex(out, r"(?m)^00[0-9A-F]{2} A n$")

    def test_objdump(self) -> None:
        import objdump
        self.assertEqual(self._compile("-O", "-c", "b.c"), 0)
        rc, out = _run(objdump, ["objdump.py", "-d", "b.o"])
        self.assertEqual(rc, 0)
        self.assertRegex(out, r"(?m)^main:\n   SUBROUTINE$")
        self.assertRegex(out, r"JSR   bump +; [0-9A-F]{4}  20 00 00")
        rc, out = _run(objdump, ["objdump.py", "-x", "b.o"])
        for part in ("file format elf32-mos", "Sections:", "SYMBOL TABLE:",
                     "RELOCATION RECORDS FOR", "R_MOS_ADDR16",
                     "@zp-link-meta-begin"):
            self.assertIn(part, out)
        self.assertEqual(self._compile("-O", "a.c", "b.c", "-o", "prog"), 0)
        rc, out = _run(objdump, ["objdump.py", "-D", "--no-show-raw-insn",
                                 "prog"])
        self.assertIn("   SEG   text\n   ORG   $0800\n", out)
        self.assertRegex(out, r"(?m)^n\tEQU\t\$[0-9A-F]{2}$")


@unittest.skipUnless(_DASM and _HAVE_PCPP, "needs dasm and pcpp")
class TestAgainstDasm(unittest.TestCase):

    def _dasm(self, text: str) -> dict[int, int]:
        """dasm's bytes for `text`, by address."""
        with tempfile.TemporaryDirectory() as tmp:
            src, out = Path(tmp) / "in.asm", Path(tmp) / "out.bin"
            src.write_text(text)
            done = subprocess.run([_DASM, str(src), "-f2", f"-o{out}"],
                                  capture_output=True, text=True)
            self.assertEqual(done.returncode, 0, done.stdout + done.stderr)
            binary = out.read_bytes()
        image, pos = {}, 0
        while pos + 4 <= len(binary):
            addr = binary[pos] | binary[pos + 1] << 8
            size = binary[pos + 2] | binary[pos + 3] << 8
            for k in range(size):
                image[addr + k] = binary[pos + 4 + k]
            pos += 4 + size
        return image

    def test_units(self) -> None:
        linked = {"interpreter.asm", "instr_save.asm", "print_zstring.asm"}
        for path in sorted((_ROOT / "examples").rglob("*.asm")):
            if path.name in linked:
                continue
            with self.subTest(unit=path.name):
                text = path.read_text()
                obj = _assemble(text, path.name)
                equ = {m.group(1): int(m.group(2), 16) for m in re.finditer(
                    r"^(\S+)\tEQU\t\$([0-9A-F]+)$", text, re.M)}
                undefined = sorted({
                    r.symbol.name for s in obj.sections for r in s.relocs
                    if not r.symbol.defined and r.symbol.name not in equ
                })
                externs = {n: 0x3000 + 16 * k for k, n in enumerate(undefined)}
                mine = _image(obj, {**equ, **externs})
                source = "\n".join(
                    ["   PROCESSOR 6502", "   ORG $0800", text]
                    + [f"{n}\tEQU\t${v:04X}" for n, v in externs.items()]
                )
                theirs = self._dasm(source)
                self.assertEqual(
                    mine, bytes(theirs.get(0x0800 + k, 0)
                                for k in range(len(mine))),
                )

    def test_element_read_modify_write(self) -> None:
        # dasm picks zp,X or abs,X for `INC t,X` and the rest as this
        # assembler does.
        text = "ZP\tEQU\t$80\nFAR\tEQU\t$1234\n" + _fn(
            *(f"{op}   {name}+1,X"
              for op in _RMW_OPS for name in ("ZP", "FAR")),
            "RTS",
        )
        mine = _image(_assemble(text), {})
        theirs = self._dasm("   PROCESSOR 6502\n   ORG $0800\n" + text)
        self.assertEqual(mine, bytes(theirs[0x0800 + k]
                                     for k in range(len(mine))))

    def test_linked_listings(self) -> None:
        from tests.link_helpers import listing
        import tests.test_zork_interpreter as zi
        with tempfile.TemporaryDirectory() as tmp:
            _, exe_path = zi._build(tmp)
            exe = E.read_file(exe_path)
            theirs = self._dasm(listing(exe_path))
        for seg in exe.segments:
            self.assertEqual(
                bytes(seg.data),
                bytes(theirs.get(seg.paddr + k, -1) & 0xFF
                      for k in range(len(seg.data))),
            )


if __name__ == "__main__":
    unittest.main()
