"""ELF32 object and executable files for the 6502, as c6502 writes them.

`compile.py -c` writes a relocatable object (`ET_REL`), and linking
writes an executable (`ET_EXEC`). `read` and `write` convert between
the bytes and an `ElfFile`, which `nm.py`, `objdump.py`, the linker,
and the simulator all work on.

The machine number and the relocation numbers are those llvm-mos
gives the 6502 (`EM_MOS` and `R_MOS_*` in its `ELF.h` and
`ELFRelocs/MOS.def`), and `e_flags` is `EF_MOS_ARCH_6502`. The objects
carry c6502's link metadata, though, which no other linker reads, so
they aren't meant to be linked by llvm-mos's `ld.lld`.

An object has one section per function and per object, as gcc's
`-ffunction-sections` gives, so the linker can place each item on its
own. Each section is named after the section its item goes in
(`.text`, `.rodata`, `.data`, `.bss`, or a name from
`[[c6502::section]]`), so several sections share a name. GNU as does
the same with `.section name,"ax",@progbits,unique,N`. The link
metadata (`passes/zp_link_metadata.py`) is the text of the section
`.c6502.link`, which isn't loaded.

Relocations are `SHT_RELA`, each with its addend in the entry. The
value a relocation computes is S + A, where S is the symbol's address
and A the addend. `R_MOS_PCREL_8` computes S + A - P, where P is the
address of the byte it writes, and the assembler sets A to -1, so the
byte is the branch displacement from the next instruction.

Every value is little-endian, as on the 6502.
"""
from __future__ import annotations

import struct
from dataclasses import dataclass, field

# e_ident and the header.
ELFCLASS32 = 1
ELFDATA2LSB = 1
EV_CURRENT = 1
ET_REL = 1
ET_EXEC = 2
EM_MOS = 6502
EF_MOS_ARCH_6502 = 0x00000001

# Section types.
SHT_NULL = 0
SHT_PROGBITS = 1
SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4
SHT_NOBITS = 8

# Section flags.
SHF_WRITE = 0x1
SHF_ALLOC = 0x2
SHF_EXECINSTR = 0x4
SHF_INFO_LINK = 0x40

# Special section indices.
SHN_UNDEF = 0
SHN_ABS = 0xFFF1

# Symbol bindings and types.
STB_LOCAL = 0
STB_GLOBAL = 1
STT_NOTYPE = 0
STT_OBJECT = 1
STT_FUNC = 2
STT_SECTION = 3
STT_FILE = 4

# `st_other` bit llvm-mos sets on an external symbol in zero page. c6502
# sets it on every undefined symbol the unit reaches with zero-page
# addressing, so a reader can see which ones the linker has to put
# below $100.
STO_MOS_ZEROPAGE = 0x20

# Program header types and flags.
PT_LOAD = 1
PF_X = 0x1
PF_W = 0x2
PF_R = 0x4

# Relocation types.
R_MOS_NONE = 0
R_MOS_IMM8 = 1
R_MOS_ADDR8 = 2
R_MOS_ADDR16 = 3
R_MOS_ADDR16_LO = 4
R_MOS_ADDR16_HI = 5
R_MOS_PCREL_8 = 6

RELOC_NAMES = {
    R_MOS_NONE: "R_MOS_NONE",
    R_MOS_IMM8: "R_MOS_IMM8",
    R_MOS_ADDR8: "R_MOS_ADDR8",
    R_MOS_ADDR16: "R_MOS_ADDR16",
    R_MOS_ADDR16_LO: "R_MOS_ADDR16_LO",
    R_MOS_ADDR16_HI: "R_MOS_ADDR16_HI",
    R_MOS_PCREL_8: "R_MOS_PCREL_8",
}

# The number of bytes each relocation type writes.
RELOC_SIZES = {
    R_MOS_IMM8: 1, R_MOS_ADDR8: 1, R_MOS_ADDR16: 2,
    R_MOS_ADDR16_LO: 1, R_MOS_ADDR16_HI: 1, R_MOS_PCREL_8: 1,
}

# The section that holds the link metadata.
LINK_SECTION = ".c6502.link"
# The section that holds the assembly text an object was assembled
# from, which the linker assembles again when the program's addresses
# contradict the unit's.
SOURCE_SECTION = ".c6502.asm"

_EHDR = struct.Struct("<16sHHIIIIIHHHHHH")
_SHDR = struct.Struct("<IIIIIIIIII")
_PHDR = struct.Struct("<IIIIIIII")
_SYM = struct.Struct("<IIIBBH")
_RELA = struct.Struct("<IIi")


class ElfError(Exception):
    """A file that isn't an ELF file c6502 can read."""


@dataclass(eq=False)
class Section:
    """A section with contents: code or data (`SHT_PROGBITS`), space
    with none (`SHT_NOBITS`, where `size` gives its length), or the
    link metadata. `addr` is its run address in an executable, and 0
    in an object. Symbol and relocation tables aren't `Section`s; they
    are built from `ElfFile.symbols` and each section's `relocs`."""
    name: str
    type: int = SHT_PROGBITS
    flags: int = SHF_ALLOC
    data: bytearray = field(default_factory=bytearray)
    nobits_size: int = 0
    addr: int = 0
    align: int = 1
    relocs: list[Reloc] = field(default_factory=list)

    @property
    def size(self) -> int:
        return self.nobits_size if self.type == SHT_NOBITS else len(self.data)


@dataclass(eq=False)
class Symbol:
    """A symbol. `section` is the `Section` it's defined in, or
    `SHN_ABS` for an absolute value, or `SHN_UNDEF` for one another
    file defines. `value` is its offset in the section in an object,
    and its address in an executable."""
    name: str
    value: int = 0
    size: int = 0
    bind: int = STB_LOCAL
    type: int = STT_NOTYPE
    section: Section | int = SHN_UNDEF
    other: int = 0

    @property
    def defined(self) -> bool:
        return self.section != SHN_UNDEF

    @property
    def is_global(self) -> bool:
        return self.bind != STB_LOCAL


@dataclass(eq=False)
class Reloc:
    """A relocation: the `type`'s bytes at `offset` in its section get
    the value computed from `symbol` and `addend`."""
    offset: int
    type: int
    symbol: Symbol
    addend: int = 0


@dataclass(eq=False)
class Segment:
    """A loadable segment of an executable: `memsz` bytes run at
    `vaddr`, loaded at `paddr` (different from `vaddr` for a section
    the startup code copies), with the first `filesz` from `data`."""
    vaddr: int
    paddr: int
    data: bytes
    memsz: int
    flags: int = PF_R


@dataclass(eq=False)
class ElfFile:
    """An object (`ET_REL`) or executable (`ET_EXEC`). `symbols`
    excludes the null symbol at index 0."""
    type: int = ET_REL
    sections: list[Section] = field(default_factory=list)
    symbols: list[Symbol] = field(default_factory=list)
    segments: list[Segment] = field(default_factory=list)
    entry: int = 0
    flags: int = EF_MOS_ARCH_6502

    def section(self, name: str) -> Section | None:
        """The first section named `name`, or None."""
        return next((s for s in self.sections if s.name == name), None)

    def link_text(self) -> str | None:
        """The link metadata's text, or None when there's none."""
        s = self.section(LINK_SECTION)
        return None if s is None else s.data.decode("utf-8")

    def source_text(self) -> str | None:
        """The assembly text the object was assembled from, or None."""
        s = self.section(SOURCE_SECTION)
        return None if s is None else s.data.decode("utf-8")

    def symbol(self, name: str) -> Symbol | None:
        """The global symbol `name`, else the first local one, or None."""
        found = [s for s in self.symbols if s.name == name]
        return next((s for s in found if s.is_global), None) or (
            found[0] if found else None
        )


# --- Writing ---------------------------------------------------------------


class _StrTab:
    def __init__(self) -> None:
        self.data = bytearray(b"\0")
        self.index: dict[str, int] = {"": 0}

    def add(self, s: str) -> int:
        if s not in self.index:
            self.index[s] = len(self.data)
            self.data += s.encode("utf-8") + b"\0"
        return self.index[s]


def write(elf: ElfFile) -> bytes:
    """The bytes of `elf`. Local symbols are written first, as ELF
    requires, and otherwise in the order of `elf.symbols`."""
    shstr = _StrTab()
    strtab = _StrTab()
    symbols = (
        [s for s in elf.symbols if not s.is_global]
        + [s for s in elf.symbols if s.is_global]
    )
    sym_index = {id(s): i + 1 for i, s in enumerate(symbols)}
    # Section header indices: null, the content sections, then for each
    # section with relocations its .rela section, then .symtab,
    # .strtab, .shstrtab.
    sec_index = {id(s): i + 1 for i, s in enumerate(elf.sections)}
    rela_secs = [s for s in elf.sections if s.relocs]
    n_content = len(elf.sections)
    symtab_ix = 1 + n_content + len(rela_secs)
    strtab_ix = symtab_ix + 1
    shstrtab_ix = strtab_ix + 1
    shnum = shstrtab_ix + 1

    for r in (r for s in elf.sections for r in s.relocs):
        if id(r.symbol) not in sym_index:
            raise ElfError(
                f"a relocation names `{r.symbol.name}`, which isn't in "
                f"the symbol table"
            )

    phnum = len(elf.segments)
    out = bytearray(_EHDR.size + phnum * _PHDR.size)
    headers: list[tuple] = [(0,) * 10]

    def place(blob: bytes) -> int:
        off = len(out)
        out.extend(blob)
        return off

    # Segment contents, then section contents. An executable's
    # sections are the segments' bytes, so its sections point into
    # them where they match.
    phdrs = []
    seg_offsets: dict[tuple[int, bytes], int] = {}
    for seg in elf.segments:
        off = place(seg.data)
        seg_offsets.setdefault((seg.vaddr, bytes(seg.data)), off)
        phdrs.append((PT_LOAD, off, seg.vaddr, seg.paddr, len(seg.data),
                      seg.memsz, seg.flags, 1))
    for s in elf.sections:
        if s.type == SHT_NOBITS:
            off = len(out)
        else:
            off = seg_offsets.get((s.addr, bytes(s.data)))
            if off is None:
                off = place(bytes(s.data))
        headers.append((
            shstr.add(s.name), s.type, s.flags, s.addr, off, s.size,
            0, 0, s.align, 0,
        ))
    for s in rela_secs:
        blob = b"".join(
            _RELA.pack(r.offset, sym_index[id(r.symbol)] << 8 | r.type,
                       r.addend)
            for r in s.relocs
        )
        headers.append((
            shstr.add(".rela" + s.name), SHT_RELA, SHF_INFO_LINK, 0,
            place(blob), len(blob), symtab_ix, sec_index[id(s)], 4,
            _RELA.size,
        ))
    syms = bytearray(_SYM.size)
    for s in symbols:
        if isinstance(s.section, Section):
            shndx = sec_index[id(s.section)]
        else:
            shndx = s.section
        syms += _SYM.pack(
            strtab.add(s.name), s.value & 0xFFFFFFFF, s.size,
            s.bind << 4 | s.type, s.other, shndx,
        )
    first_global = 1 + sum(1 for s in symbols if not s.is_global)
    headers.append((
        shstr.add(".symtab"), SHT_SYMTAB, 0, 0, place(bytes(syms)),
        len(syms), strtab_ix, first_global, 4, _SYM.size,
    ))
    headers.append((
        shstr.add(".strtab"), SHT_STRTAB, 0, 0, place(bytes(strtab.data)),
        len(strtab.data), 0, 0, 1, 0,
    ))
    name = shstr.add(".shstrtab")
    headers.append((
        name, SHT_STRTAB, 0, 0, place(bytes(shstr.data)), len(shstr.data),
        0, 0, 1, 0,
    ))
    while len(out) % 4:
        out.append(0)
    shoff = len(out)
    for h in headers:
        out += _SHDR.pack(*h)
    assert len(headers) == shnum
    ident = b"\x7fELF" + bytes([ELFCLASS32, ELFDATA2LSB, EV_CURRENT]) + bytes(9)
    out[:_EHDR.size] = _EHDR.pack(
        ident, elf.type, EM_MOS, EV_CURRENT, elf.entry,
        _EHDR.size if phnum else 0, shoff, elf.flags, _EHDR.size,
        _PHDR.size, phnum, _SHDR.size, shnum, shstrtab_ix,
    )
    for i, p in enumerate(phdrs):
        start = _EHDR.size + i * _PHDR.size
        out[start:start + _PHDR.size] = _PHDR.pack(*p)
    return bytes(out)


# --- Reading ---------------------------------------------------------------


def read(data: bytes, name: str = "file") -> ElfFile:
    """The `ElfFile` in `data`. `name` is for error messages. Raises
    `ElfError` when `data` isn't a little-endian ELF32 file for the
    6502."""
    if len(data) < _EHDR.size or data[:4] != b"\x7fELF":
        raise ElfError(f"{name}: not an ELF file")
    (ident, e_type, machine, _, entry, phoff, shoff, flags, _, phentsize,
     phnum, shentsize, shnum, shstrndx) = _EHDR.unpack_from(data)
    if ident[4] != ELFCLASS32 or ident[5] != ELFDATA2LSB:
        raise ElfError(f"{name}: not a little-endian 32-bit ELF file")
    if machine != EM_MOS:
        raise ElfError(
            f"{name}: ELF file for machine {machine}, not the 6502 "
            f"({EM_MOS})"
        )
    shdrs = [
        _SHDR.unpack_from(data, shoff + i * shentsize) for i in range(shnum)
    ]

    def blob(h) -> bytes:
        return data[h[4]:h[4] + h[5]]

    def cstr(table: bytes, off: int) -> str:
        end = table.index(b"\0", off)
        return table[off:end].decode("utf-8")

    shstr = blob(shdrs[shstrndx]) if shnum else b"\0"
    elf = ElfFile(type=e_type, entry=entry, flags=flags)
    by_index: dict[int, Section] = {}
    for i, h in enumerate(shdrs):
        if h[1] in (SHT_PROGBITS, SHT_NOBITS):
            s = Section(
                name=cstr(shstr, h[0]), type=h[1], flags=h[2], addr=h[3],
                align=h[8],
            )
            if h[1] == SHT_NOBITS:
                s.nobits_size = h[5]
            else:
                s.data = bytearray(blob(h))
            by_index[i] = s
            elf.sections.append(s)
    symbols: list[Symbol | None] = [None]
    symtab = next((h for h in shdrs if h[1] == SHT_SYMTAB), None)
    if symtab is not None:
        strtab = blob(shdrs[symtab[6]])
        raw = blob(symtab)
        for off in range(_SYM.size, len(raw), _SYM.size):
            st_name, value, size, info, other, shndx = _SYM.unpack_from(
                raw, off,
            )
            if shndx in (SHN_UNDEF, SHN_ABS):
                section: Section | int = shndx
            elif shndx in by_index:
                section = by_index[shndx]
            else:
                # A symbol in a section c6502 doesn't keep, such as a
                # section symbol for .symtab.
                symbols.append(None)
                continue
            sym = Symbol(
                name=cstr(strtab, st_name), value=value, size=size,
                bind=info >> 4, type=info & 0xF, section=section,
                other=other,
            )
            symbols.append(sym)
            elf.symbols.append(sym)
    for h in shdrs:
        if h[1] != SHT_RELA:
            continue
        target = by_index.get(h[7])
        if target is None:
            raise ElfError(f"{name}: a relocation section names no section")
        raw = blob(h)
        for off in range(0, len(raw), _RELA.size):
            r_offset, info, addend = _RELA.unpack_from(raw, off)
            sym = symbols[info >> 8]
            if sym is None:
                raise ElfError(
                    f"{name}: a relocation in {target.name} names a "
                    f"symbol c6502 doesn't read"
                )
            target.relocs.append(Reloc(r_offset, info & 0xFF, sym, addend))
    for i in range(phnum):
        (p_type, p_offset, vaddr, paddr, filesz, memsz, pflags,
         _) = _PHDR.unpack_from(data, phoff + i * phentsize)
        if p_type == PT_LOAD:
            elf.segments.append(Segment(
                vaddr=vaddr, paddr=paddr,
                data=data[p_offset:p_offset + filesz], memsz=memsz,
                flags=pflags,
            ))
    return elf


def read_file(path) -> ElfFile:
    """The `ElfFile` in the file at `path`."""
    from pathlib import Path
    return read(Path(path).read_bytes(), str(path))


def is_elf(data: bytes) -> bool:
    return data[:4] == b"\x7fELF"
