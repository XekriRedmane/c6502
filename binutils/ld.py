"""The parts of linking that know nothing about c6502's link metadata:
resolving symbols across objects, applying relocations, and writing
the executable. `passes/linker.py` decides where each section goes
(by the linker script, or one after another) and which addresses the
linker-placed names get, then calls `relocate` and `executable`.

A `Placed` section is one input section at its run address. Every
symbol a relocation names resolves to:

* a symbol defined in the same object, at its section's address plus
  its offset (local symbols resolve only this way);
* else a global symbol another object defines, or an absolute one;
* else a name the linker itself defines (`extra`), such as a slot
  address or `__text_start`.

An absolute symbol that several objects define has to have one value.
"""
from __future__ import annotations

import re
from dataclasses import dataclass, field

from binutils import elf as E


class LinkError(Exception):
    """A program that can't be linked."""


@dataclass(eq=False)
class Placed:
    """Input section `section` of object number `unit`, running at
    `addr`. `size` is its size in the output, which is smaller than
    the section's when the linker drops a final `JMP` to the function
    after it."""
    unit: int
    section: E.Section
    addr: int
    size: int


def global_definitions(
    objects: list[tuple[str, E.ElfFile]],
) -> tuple[dict[str, tuple[int, E.Symbol]], dict[str, int]]:
    """The global symbols the objects define in sections, as (unit,
    symbol), and the absolute ones, as values. Raises `LinkError` for
    a name two objects define, or an absolute name given two values."""
    defs: dict[str, tuple[int, E.Symbol]] = {}
    absolute: dict[str, int] = {}
    abs_where: dict[str, str] = {}
    for k, (path, obj) in enumerate(objects):
        for s in obj.symbols:
            if not s.is_global or not s.defined:
                continue
            if s.section == E.SHN_ABS:
                if s.name in absolute and absolute[s.name] != s.value:
                    raise LinkError(
                        f"{path}: {s.name!r} is at ${s.value:04X} here "
                        f"and at ${absolute[s.name]:04X} in "
                        f"{abs_where[s.name]}"
                    )
                absolute[s.name] = s.value
                abs_where[s.name] = path
                continue
            if s.name in defs:
                raise LinkError(
                    f"`{s.name}` is defined in multiple TUs: "
                    f"{objects[defs[s.name][0]][0]} and {path}"
                )
            defs[s.name] = (k, s)
    for name in absolute:
        if name in defs:
            raise LinkError(
                f"`{name}` is an address in {abs_where[name]} and is "
                f"defined in {objects[defs[name][0]][0]}"
            )
    return defs, absolute


class Resolver:
    """Gives each symbol an address, once every section is placed."""

    def __init__(
        self, objects: list[tuple[str, E.ElfFile]],
        placed: list[Placed], extra: dict[str, int],
    ) -> None:
        self.objects = objects
        self.addr = {id(p.section): p.addr for p in placed}
        self.defs, self.absolute = global_definitions(objects)
        self.extra = extra

    def section_addr(self, sec: E.Section) -> int | None:
        return self.addr.get(id(sec))

    def value(self, sym: E.Symbol, unit: int) -> int:
        """The address of `sym`, which object `unit` names."""
        if sym.defined:
            if sym.section == E.SHN_ABS:
                return sym.value
            base = self.section_addr(sym.section)
            if base is None:
                raise LinkError(
                    f"{self.objects[unit][0]}: `{sym.name}` is in a "
                    f"section the link left out"
                )
            return base + sym.value
        if sym.name in self.defs:
            k, d = self.defs[sym.name]
            return self.value(d, k)
        if sym.name in self.absolute:
            return self.absolute[sym.name]
        if sym.name in self.extra:
            return self.extra[sym.name]
        raise LinkError(
            f"{self.objects[unit][0]}: undefined symbol `{sym.name}`"
            + _undefined_hint(sym.name)
        )


# The names of the runtime's helpers, which the compiler calls. The
# linker adds the ones the runtime library has
# (passes/runtime_library.py) to a program that calls them, so a name
# here that is still undefined is one of the helpers without 6502 code
# (CLAUDE.md, "Not yet in the repo").
_HELPER = re.compile(
    r"(mul|udivmod|sdivmod|asl|asr|lsr)(8|16|32|64)|icall"
    r"|[fd](add|sub|mul|div|lt|le)"
    r"|(i|u|l|ul|ll|ull|f|d)2(f|d|i|u|l|ul|ll|ull)"
)


def _undefined_hint(name: str) -> str:
    """A hint to add to the error for an undefined `name`."""
    if _HELPER.fullmatch(name):
        return (
            " (a runtime helper that has no 6502 code in the runtime "
            "library yet, so give its address with --defsym)"
        )
    return ""


def relax(sec: E.Section, symbols: list[E.Symbol], value_of) -> int:
    """Shortens each absolute instruction in code section `sec` whose
    operand the linker has put in zero page to its zero-page form,
    and returns the number of bytes saved.

    A unit reaches a name another unit defines, such as a global that
    the defining unit's `-O` moved into zero page, with absolute
    addressing, since it can't know where the name goes. When
    `value_of(symbol)` gives the name's address (it returns None for
    names whose address isn't known before layout) and the address is
    below $100, the instruction loses its operand's high byte and its
    relocation becomes `R_MOS_ADDR8`. The bytes after it move down:
    every branch within the section is re-aimed, and the relocations,
    the labels and mapping symbols (`symbols`, the object's), and the
    item's size move with them. A branch only gets shorter, so it
    stays in reach. A jump table (between `$d` and `$c`) isn't
    decoded."""
    from binutils.opcodes import DECODE, OPCODES, SIZES, ZP_FORM
    if sec.type == E.SHT_NOBITS or not sec.flags & E.SHF_EXECINSTR:
        return 0
    data = sec.data
    own = [s for s in symbols if s.section is sec]
    mapping = sorted(
        (s.value, s.name) for s in own if s.name in ("$c", "$d")
    )
    relocs = {r.offset: r for r in sec.relocs}

    def in_data(off: int) -> bool:
        state = "$c"
        for at, name in mapping:
            if at > off:
                break
            state = name
        return state == "$d"

    # Decode the instructions and choose the ones to shorten.
    instrs: list[tuple[int, str, str, int]] = []
    shorten: set[int] = set()
    off = 0
    while off < len(data):
        if in_data(off) or data[off] not in DECODE:
            off += 1
            continue
        mnemonic, mode = DECODE[data[off]]
        n = SIZES[mode]
        instrs.append((off, mnemonic, mode, n))
        r = relocs.get(off + 1)
        zp_mode = ZP_FORM.get(mode)
        if (r is not None and r.type == E.R_MOS_ADDR16
                and zp_mode is not None
                and (mnemonic, zp_mode) in OPCODES):
            v = value_of(r.symbol)
            if v is not None and 0 <= v + r.addend <= 0xFF:
                shorten.add(off)
        off += n
    if not shorten:
        return 0
    deleted = sorted(off + 2 for off in shorten)

    def moved(old: int) -> int:
        import bisect
        return old - bisect.bisect_left(deleted, old)

    out = bytearray(data)
    # Re-aim the branches resolved at assembly, from the old bytes.
    for off, mnemonic, mode, n in instrs:
        if mode == "rel" and off + 1 not in relocs:
            disp = data[off + 1] - (256 if data[off + 1] >= 128 else 0)
            target = off + 2 + disp
            new_disp = moved(target) - moved(off + 2)
            out[off + 1] = new_disp & 0xFF
    for off in shorten:
        mnemonic, mode = DECODE[data[off]]
        out[off] = OPCODES[(mnemonic, ZP_FORM[mode])]
        relocs[off + 1].type = E.R_MOS_ADDR8
    for at in reversed(deleted):
        del out[at]
    sec.data = out
    for r in sec.relocs:
        r.offset = moved(r.offset)
    for s in own:
        if s.type in (E.STT_FUNC, E.STT_OBJECT):
            s.size = len(out)
        else:
            s.value = moved(s.value)
    return len(deleted)


def relocate(
    p: Placed, resolver: Resolver, path: str,
) -> bytearray:
    """The bytes of placed section `p` with its relocations applied.
    Raises `LinkError` when a value doesn't fit its field."""
    sec = p.section
    data = bytearray(sec.data[:p.size]) if sec.type != E.SHT_NOBITS \
        else bytearray()
    for r in sec.relocs:
        if r.offset >= p.size:
            # In the dropped `JMP` at the end of the section.
            continue
        s = resolver.value(r.symbol, p.unit)
        v = s + r.addend
        where = f"{path}: {sec.name}+${r.offset:04X}"
        if r.type == E.R_MOS_ADDR16:
            if not 0 <= v <= 0xFFFF:
                raise LinkError(f"{where}: address ${v:X} of "
                                f"`{r.symbol.name}` is out of range")
            data[r.offset:r.offset + 2] = v.to_bytes(2, "little")
        elif r.type == E.R_MOS_ADDR8:
            if not 0 <= v <= 0xFF:
                raise LinkError(
                    f"{where}: `{r.symbol.name}` is at ${v:04X}, and this "
                    f"instruction was assembled with zero-page addressing "
                    f"for it"
                    + (", since it was in zero page when its unit was "
                       "compiled" if r.symbol.name.startswith(
                           ("__zpabi_", "__local_")) else "")
                )
            data[r.offset] = v
        elif r.type == E.R_MOS_IMM8:
            if not -128 <= v <= 0xFF:
                raise LinkError(f"{where}: `{r.symbol.name}` + {r.addend} "
                                f"= {v} doesn't fit in a byte")
            data[r.offset] = v & 0xFF
        elif r.type == E.R_MOS_ADDR16_LO:
            data[r.offset] = v & 0xFF
        elif r.type == E.R_MOS_ADDR16_HI:
            data[r.offset] = (v >> 8) & 0xFF
        elif r.type == E.R_MOS_PCREL_8:
            disp = v - (p.addr + r.offset)
            if not -128 <= disp <= 127:
                raise LinkError(
                    f"{where}: the branch to `{r.symbol.name}` is {disp} "
                    f"bytes away, out of reach"
                )
            data[r.offset] = disp & 0xFF
        else:
            raise LinkError(f"{where}: unknown relocation type {r.type}")
    return data


@dataclass
class OutputSection:
    """An output section of the executable: its name, run address,
    load address, size, the placed input sections in order, whether
    it has no bytes in the file (NOLOAD), and whether it holds code.
    `executable()` sets `elf_section` to the section it writes."""
    name: str
    addr: int
    load_addr: int
    size: int
    parts: list[Placed] = field(default_factory=list)
    noload: bool = False
    executable: bool = False
    elf_section: E.Section | None = None


def executable(
    objects: list[tuple[str, E.ElfFile]],
    outputs: list[OutputSection],
    resolver: Resolver,
    symbols: list[E.Symbol],
    entry: int,
    link_text: str | None = None,
) -> E.ElfFile:
    """The executable for `outputs`, with every input section's
    relocations applied. `symbols` are the symbol table entries. A
    symbol whose `section` is an `OutputSection` is moved to the
    section written for it."""
    exe = E.ElfFile(type=E.ET_EXEC, entry=entry)
    for out in outputs:
        data = bytearray(out.size)
        for p in out.parts:
            off = p.addr - out.addr
            data[off:off + p.size] = relocate(p, resolver, objects[p.unit][0]) \
                if p.section.type != E.SHT_NOBITS else bytes(p.size)
        flags = E.SHF_ALLOC | (E.SHF_EXECINSTR if out.executable
                               else E.SHF_WRITE)
        if out.noload:
            sec = E.Section(name=out.name, type=E.SHT_NOBITS, flags=flags,
                            nobits_size=out.size, addr=out.addr)
        else:
            sec = E.Section(name=out.name, flags=flags, data=data,
                            addr=out.addr)
        out.elf_section = sec
        exe.sections.append(sec)
        if out.size:
            exe.segments.append(E.Segment(
                vaddr=out.addr, paddr=out.load_addr,
                data=b"" if out.noload else bytes(data), memsz=out.size,
                flags=E.PF_R | (E.PF_X if out.executable else E.PF_W),
            ))
    for s in symbols:
        if isinstance(s.section, OutputSection):
            s.section = s.section.elf_section
    exe.symbols = symbols
    if link_text is not None:
        exe.sections.append(E.Section(
            name=E.LINK_SECTION, flags=0,
            data=bytearray(link_text.encode("utf-8")),
        ))
    return exe
