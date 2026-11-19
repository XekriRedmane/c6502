"""Tests for the object-file tools: ELF files (binutils/elf.py), the
assembler `compile.py -c` runs (binutils/assembler.py), relaxation and
relocation (binutils/ld.py), the disassembler (binutils/disasm.py),
`nm.py`, `objdump.py`, and the driver's object and link modes.

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
from binutils.assembler import AssemblerError, assemble
from binutils.ld import Placed, Resolver, relax, relocate
from tests.no_inlining import no_inlining

_ROOT = Path(__file__).parent.parent
_HAVE_PCPP = shutil.which("pcpp") is not None
_DASM = shutil.which("dasm")


def _image(obj: E.ElfFile, symbols: dict[str, int], origin: int = 0x0800,
           path: str = "t.asm") -> tuple[bytes, dict[int, int]]:
    """`obj`'s loaded sections one after another from `origin`, with
    their relocations applied, and each section's address."""
    placed = []
    addr = origin
    for s in obj.sections:
        if s.flags & E.SHF_ALLOC:
            placed.append(Placed(0, s, addr, s.size))
            addr += s.size
    res = Resolver([(path, obj)], placed, symbols)
    out = bytearray()
    for p in placed:
        out += (relocate(p, res, path) if p.section.type != E.SHT_NOBITS
                else bytes(p.size))
    return bytes(out), {id(p.section): p.addr for p in placed}


# The read-modify-write instructions and their zp,X and abs,X opcodes,
# from the MCS6500 manual's instruction summary.
_RMW_OPS = ("INC", "DEC", "ASL", "LSR", "ROL", "ROR")
_RMW_ZPX = {"INC": 0xF6, "DEC": 0xD6, "ASL": 0x16, "LSR": 0x56,
            "ROL": 0x36, "ROR": 0x76}
_RMW_ABSX = {"INC": 0xFE, "DEC": 0xDE, "ASL": 0x1E, "LSR": 0x5E,
             "ROL": 0x3E, "ROR": 0x7E}


def _fn(*body: str, name: str = "f") -> str:
    lines = [f"{name}:", "   SUBROUTINE"]
    for b in body:
        lines.append(b if b.endswith(":") else f"   {b}")
    return "\n".join(lines) + "\n"


class TestElf(unittest.TestCase):

    def test_round_trip(self) -> None:
        text = (_ROOT / "examples" / "apply_bobble.asm").read_text()
        obj = assemble(text, "apply_bobble.asm")
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
        data = E.write(assemble(_fn("RTS")))
        self.assertEqual(data[:4], b"\x7fELF")
        self.assertEqual(int.from_bytes(data[18:20], "little"), E.EM_MOS)
        with self.assertRaises(E.ElfError):
            E.read(b"not an elf file at all, no")


class TestAssembler(unittest.TestCase):

    def test_zero_page_chosen_by_value(self) -> None:
        text = "ZP\tEQU\t$80\nFAR\tEQU\t$1234\n" + _fn(
            "LDA   ZP", "LDA   FAR", "LDA   $10,X", "STX   ZP,Y",
            "LDA   ZP,Y", "LDA   later", "LDA   (ZP),Y", "RTS",
        ) + "later:\n   DC.B  $00\n"
        obj = assemble(text)
        code, _ = _image(obj, {})
        self.assertEqual(code[:18], bytes([
            0xA5, 0x80,              # LDA zp
            0xAD, 0x34, 0x12,        # LDA abs
            0xB5, 0x10,              # LDA zp,X
            0x96, 0x80,              # STX zp,Y
            0xB9, 0x80, 0x00,        # LDA abs,Y: LDA has no zp,Y
            0xAD, 0x12, 0x08,        # a label is never in zero page
            0xB1, 0x80,              # LDA (zp),Y
            0x60,
        ]))
        relocs = {(r.offset, r.symbol.name): r.type
                  for r in obj.sections[0].relocs}
        self.assertEqual(relocs[(1, "ZP")], E.R_MOS_ADDR8)
        self.assertEqual(relocs[(3, "FAR")], E.R_MOS_ADDR16)
        self.assertEqual(relocs[(13, "later")], E.R_MOS_ADDR16)
        far = obj.symbol("FAR")
        self.assertEqual((far.section, far.value), (E.SHN_ABS, 0x1234))

    def test_read_modify_write_of_an_element(self) -> None:
        # `INC t,X` and the rest take zp,X for an address in zero page
        # and abs,X otherwise, and the disassembler reads them back.
        from binutils.disasm import disassemble
        text = "ZP\tEQU\t$80\nFAR\tEQU\t$1234\n" + _fn(
            *(f"{op}   {name},X"
              for op in _RMW_OPS for name in ("ZP", "FAR")),
            "RTS",
        )
        obj = assemble(text)
        code, _ = _image(obj, {})
        expected = b""
        for op in _RMW_OPS:
            expected += bytes([_RMW_ZPX[op], 0x80])
            expected += bytes([_RMW_ABSX[op], 0x34, 0x12])
        self.assertEqual(code, expected + b"\x60")
        listing = "\n".join(disassemble(obj))
        for op in _RMW_OPS:
            self.assertRegex(
                listing, rf"{op}   ZP,X +; [0-9A-F]{{4}}  {_RMW_ZPX[op]:02X} ")
            self.assertRegex(
                listing, rf"{op}   FAR,X +; [0-9A-F]{{4}}  {_RMW_ABSX[op]:02X} ")

    def test_linker_placed_names_stay_undefined(self) -> None:
        # A slot, a runtime name, and a zero-page object take their
        # addressing mode from the unit's value, and the linker gives
        # them their addresses.
        text = (
            "HARGS\tEQU\t$04\n__local_f__x\tEQU\t$90\n"
            "; @zp-link-meta-begin\n"
            "; zp obj size=1 at=auto def=true init=$00\n"
            "; @zp-link-meta-end\n"
            "obj\tEQU\t$30\n"
        ) + _fn("LDA   HARGS+2", "STA   __local_f__x", "INC   obj",
                "LDA   SSP", "RTS")
        obj = assemble(text)
        code = bytes(obj.sections[0].data)
        self.assertEqual(code[0::2][:4], bytes([0xA5, 0x85, 0xE6, 0xA5]))
        for name in ("HARGS", "__local_f__x", "obj", "SSP"):
            sym = obj.symbol(name)
            self.assertFalse(sym.defined, name)
            self.assertTrue(sym.other & E.STO_MOS_ZEROPAGE, name)

    def test_immediates_and_data(self) -> None:
        text = _fn(
            "LDA   #<(buf+2)", "LDX   #>buf", "LDY   #$FF", "LDA   #-1",
            "JMP   .t", ".t:", "DC.W  .t-1", "DC.B  <buf, $05, \"hi\"",
            "RTS",
        ) + "buf:\n   DS.B  4\n"
        obj = assemble(text)
        sec = obj.sections[0]
        r = {r.offset: (r.type, r.symbol.name, r.addend) for r in sec.relocs}
        self.assertEqual(r[1], (E.R_MOS_ADDR16_LO, "buf", 2))
        self.assertEqual(r[3], (E.R_MOS_ADDR16_HI, "buf", 0))
        self.assertEqual(r[9], (E.R_MOS_ADDR16, ".t", 0))
        self.assertEqual(r[11], (E.R_MOS_ADDR16, ".t", -1))
        self.assertEqual(r[13], (E.R_MOS_ADDR16_LO, "buf", 0))
        self.assertEqual(sec.data[4:8], bytes([0xA0, 0xFF, 0xA9, 0xFF]))
        self.assertEqual(sec.data[14:17], b"\x05hi")
        # The table is data between `$d` and `$c`.
        maps = sorted((s.value, s.name) for s in obj.symbols
                      if s.name in ("$c", "$d"))
        self.assertEqual(maps, [(11, "$d"), (17, "$c")])
        bss = obj.sections[1]
        self.assertEqual((bss.name, bss.type, bss.size),
                         (".bss", E.SHT_NOBITS, 4))

    def test_branches_and_local_labels(self) -> None:
        # Each function has its own `.loop`.
        text = (_fn(".loop:", "DEX", "BNE   .loop", "BEQ   g", "RTS")
                + _fn(".loop:", "BNE   .loop", "RTS", name="g"))
        obj = assemble(text)
        f, g = (s for s in obj.sections if s.flags & E.SHF_ALLOC)
        self.assertEqual(bytes(f.data[:3]), bytes([0xCA, 0xD0, 0xFD]))
        # A branch to another item is relocated.
        (r,) = f.relocs
        self.assertEqual((r.offset, r.type, r.symbol.name, r.addend),
                         (4, E.R_MOS_PCREL_8, "g", -1))
        image, addrs = _image(obj, {})
        self.assertEqual(image[3:5], bytes([0xF0, 0x01]))

    def test_errors(self) -> None:
        for text, message in [
            (_fn("FROB"), "unknown instruction"),
            (_fn("LDA   ($1234),Y"), "isn't in zero page"),
            (_fn("BNE   .nowhere"), "isn't a label in this function"),
            (_fn(".a:", *["NOP"] * 130, "BNE   .a"), "out of reach"),
            ("   LDA   #$00\n", "before any label"),
            (_fn("LDA   #$1FF"), "doesn't fit"),
            (_fn("STA   #$01"), "no imm addressing mode"),
        ]:
            with self.subTest(message=message):
                with self.assertRaisesRegex(AssemblerError, message):
                    assemble(text)

    def test_linker_values_override_the_unit(self) -> None:
        # The linker assembles a unit again with its own addresses: a
        # slot that left zero page gets absolute addressing.
        text = "__local_f__x\tEQU\t$90\n" + _fn("LDA   __local_f__x", "RTS")
        self.assertEqual(assemble(text).sections[0].data[0], 0xA5)
        self.assertEqual(assemble(
            text, link_symbols={"__local_f__x": 0x0300},
        ).sections[0].data[0], 0xAD)
        self.assertEqual(assemble(
            text, link_symbols={"__local_f__x": None},
        ).sections[0].data[0], 0xAD)


class TestRelax(unittest.TestCase):

    def test_absolute_reference_to_zero_page_shortened(self) -> None:
        text = _fn(
            "LDA   ext", "BNE   .x", "STA   ext+1,X", "JMP   .x",
            ".x:", "LDA   far", "RTS",
        )
        obj = assemble(text)
        sec = obj.sections[0]
        known = {"ext": 0x42, "far": 0x1234}
        saved = relax(sec, obj.symbols, lambda s: known.get(s.name))
        self.assertEqual(saved, 2)
        image, _ = _image(obj, known)
        self.assertEqual(image, bytes([
            0xA5, 0x42,              # LDA zp
            0xD0, 0x05,              # BNE .x, re-aimed
            0x95, 0x43,              # STA zp,X
            0x4C, 0x09, 0x08,        # JMP .x, moved with the label
            0xAD, 0x34, 0x12,        # LDA abs stays
            0x60,
        ]))
        label = next(s for s in obj.symbols if s.name == ".x")
        self.assertEqual(label.value, 9)
        self.assertEqual(obj.symbol("f").size, 13)

    def test_element_read_modify_write_shortened(self) -> None:
        # `DEC t,X` on a name the linker puts in zero page becomes
        # zp,X, as `LDA t,X` does.
        text = _fn(*(f"{op}   ext,X" for op in _RMW_OPS), "RTS")
        obj = assemble(text)
        sec = obj.sections[0]
        saved = relax(sec, obj.symbols, lambda s: 0x42 if s.name == "ext"
                      else None)
        self.assertEqual(saved, len(_RMW_OPS))
        image, _ = _image(obj, {"ext": 0x42})
        self.assertEqual(image, b"".join(
            bytes([_RMW_ZPX[op], 0x42]) for op in _RMW_OPS) + b"\x60")

    def test_jump_table_left_alone(self) -> None:
        text = _fn("RTS", ".t:", "DC.W  ext")
        obj = assemble(text)
        self.assertEqual(
            relax(obj.sections[0], obj.symbols, lambda s: 0x42), 0,
        )


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
                obj = assemble(text, path.name)
                equ = {m.group(1): int(m.group(2), 16) for m in re.finditer(
                    r"^(\S+)\tEQU\t\$([0-9A-F]+)$", text, re.M)}
                undefined = sorted({
                    r.symbol.name for s in obj.sections for r in s.relocs
                    if not r.symbol.defined and r.symbol.name not in equ
                })
                externs = {n: 0x3000 + 16 * k for k, n in enumerate(undefined)}
                mine, _ = _image(obj, {**equ, **externs}, path=path.name)
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
        mine, _ = _image(assemble(text), {})
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
