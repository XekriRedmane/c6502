"""Disassembles an ELF object or executable into dasm text, for
`objdump.py -d`.

The listing assembles with dasm. Each instruction and data line
carries its address (its offset, in an object) and its bytes in a
trailing comment, unless they're turned off:

    apply_bobble:
       SUBROUTINE
       LDY   __zpabi_apply_bobble__slot  ; 0000  A4 00

Labels are the file's symbols. A `.` label is local, so `SUBROUTINE`
follows every function's label, as `asm_emit` writes it. In an object,
an operand that a relocation fills in is written as the relocation's
symbol and addend (`#<(buf+2)`, `.case@4-1`, `__local_f__x`). In an
executable, an address operand is written as the symbol at that
address when there is one, preferring a label of the function the
instruction is in, then a global symbol, or else as an offset into
the object it's inside (`src+1`). A section's size symbol
(`__text_size`) isn't an address and never names one. An immediate is always
written as a number there, since nothing records which immediates are
halves of addresses.

Code and data are told apart by section: a section with
`SHF_EXECINSTR` is code, except between a `$d` mapping symbol and the
next `$c` (a jump table). A byte that isn't the start of a documented
instruction is written as `DC.B`.

An absolute instruction whose operand is below $100 is written with
dasm's `.w` extension (`LDA.w $0080`), which keeps dasm from choosing
zero-page addressing. In an executable, the listing starts with each
output section's `SEG` and `ORG`, and with `RORG` when the section
runs at another address than it's loaded at, so that dasm puts every
byte where the executable has it.
"""
from __future__ import annotations

from dataclasses import dataclass

from binutils import elf as E
from binutils.opcodes import DECODE, SIZES

_OPCODE_COL = 3
_OPERAND_COL = 9
_COMMENT_COL = 40


def _line(op: str, operand: str = "", comment: str = "") -> str:
    text = " " * _OPCODE_COL + op
    if operand:
        text = text.ljust(_OPERAND_COL) + operand
    if comment:
        text = text.ljust(_COMMENT_COL - 1) + " ; " + comment
    return text


def _hex(v: int, width: int) -> str:
    return f"${v:0{2 * width}X}"


def _sym_expr(name: str, addend: int) -> str:
    if addend == 0:
        return name
    return f"{name}+{addend}" if addend > 0 else f"{name}-{-addend}"


@dataclass
class Options:
    """What the listing shows: `raw` the bytes and `addresses` the
    address of each line."""
    raw: bool = True
    addresses: bool = True


class _Section:
    """One section's labels, relocations, and mapping symbols."""

    def __init__(self, elf: E.ElfFile, sec: E.Section) -> None:
        self.sec = sec
        self.exe = elf.type == E.ET_EXEC
        base = sec.addr if self.exe else 0
        self.base = base
        self.labels: dict[int, list[E.Symbol]] = {}
        self.mapping: dict[int, str] = {}
        for s in elf.symbols:
            if s.section is not sec or s.type in (E.STT_FILE, E.STT_SECTION):
                continue
            off = s.value - base
            if s.name in ("$d", "$c"):
                self.mapping[off] = s.name
                continue
            self.labels.setdefault(off, []).append(s)
        # A function starts code and an object starts data, since a
        # linker script can put both in one output section.
        for off, syms in self.labels.items():
            for s in syms:
                if s.type == E.STT_FUNC:
                    self.mapping.setdefault(off, "$c")
                elif s.type == E.STT_OBJECT:
                    self.mapping.setdefault(off, "$d")
        for syms in self.labels.values():
            # The item's own symbol first, then local labels.
            syms.sort(key=lambda s: (s.type == E.STT_NOTYPE, s.name))
        self.relocs = {r.offset: r for r in sec.relocs}


class Disassembler:
    def __init__(self, elf: E.ElfFile, options: Options | None = None) -> None:
        self.elf = elf
        self.opt = options or Options()
        self.exe = elf.type == E.ET_EXEC
        # Addresses of symbols in an executable, for naming operands.
        self.by_addr: dict[int, list[E.Symbol]] = {}
        self.objects = [
            s for s in elf.symbols
            if self.exe and s.defined and s.type == E.STT_OBJECT and s.size > 1
        ]
        if self.exe:
            for s in elf.symbols:
                if not s.defined or s.type in (E.STT_FILE, E.STT_SECTION):
                    continue
                if s.name in ("$d", "$c"):
                    continue
                if s.name.startswith("__") and s.name.endswith("_size"):
                    # A section's size (passes/link_layout.py), which
                    # isn't an address.
                    continue
                self.by_addr.setdefault(s.value, []).append(s)

    # Operands ------------------------------------------------------------

    def reloc_text(self, r: E.Reloc) -> str:
        name = r.symbol.name
        if r.type == E.R_MOS_ADDR16_LO:
            inner = _sym_expr(name, r.addend)
            return f"<{inner}" if r.addend == 0 else f"<({inner})"
        if r.type == E.R_MOS_ADDR16_HI:
            inner = _sym_expr(name, r.addend)
            return f">{inner}" if r.addend == 0 else f">({inner})"
        if r.type == E.R_MOS_PCREL_8:
            return _sym_expr(name, r.addend + 1)
        return _sym_expr(name, r.addend)

    def addr_name(self, addr: int,
                  function: E.Symbol | None) -> str | None:
        """The symbol to write for address `addr` in an executable."""
        cands = self.by_addr.get(addr, [])
        if not cands:
            # A byte inside an object: `name+k`.
            inside = sorted(
                (s for s in self.objects
                 if s.value < addr < s.value + s.size),
                key=lambda s: (not s.is_global, s.name),
            )
            if inside:
                s = inside[0]
                return f"{s.name}+{addr - s.value}"
            return None
        if function is not None:
            local = [s for s in cands if s.name.startswith(".")
                     and s.section is function.section
                     and function.value <= s.value
                     < function.value + max(function.size, 1)]
            if local:
                return local[0].name
        named = [s for s in cands if not s.name.startswith(".")]
        if not named:
            return None
        named.sort(key=lambda s: (not s.is_global, s.type != E.STT_FUNC,
                                  s.type != E.STT_OBJECT, s.name))
        return named[0].name

    # Listing -------------------------------------------------------------

    def comment(self, addr: int, data: bytes) -> str:
        parts = []
        if self.opt.addresses:
            parts.append(f"{addr:04X}")
        if self.opt.raw:
            parts.append(" ".join(f"{b:02X}" for b in data))
        return "  ".join(parts)

    def section_lines(self, sec: E.Section) -> list[str]:
        info = _Section(self.elf, sec)
        out: list[str] = []
        size = sec.size
        code = bool(sec.flags & E.SHF_EXECINSTR)
        # Branch targets with no label get one.
        synthetic: dict[int, str] = {}
        if code and sec.type != E.SHT_NOBITS:
            self.scan_targets(info, synthetic)
        function: E.Symbol | None = None
        off = 0
        in_data = not code
        while off < size or (off == size and off in info.labels):
            for s in info.labels.get(off, []):
                if s.type == E.STT_FUNC:
                    function = s
                out.append(f"{s.name}:")
                if s.type == E.STT_FUNC:
                    out.append(_line("SUBROUTINE"))
            if off in synthetic and off not in info.labels:
                out.append(f"{synthetic[off]}:")
            if off >= size:
                break
            if code and off in info.mapping:
                in_data = info.mapping[off] == "$d"
            if sec.type == E.SHT_NOBITS:
                end = self.next_stop(info, off, size, synthetic)
                out.append(_line("DS.B", str(end - off),
                                 self.comment(info.base + off, b"")
                                 if self.opt.addresses else ""))
                off = end
                continue
            if in_data:
                off = self.data_line(info, off, size, out, synthetic)
            else:
                off = self.instr_line(info, off, size, out, function,
                                      synthetic)
        return out

    def next_stop(self, info: _Section, off: int, size: int,
                  synthetic: dict[int, str]) -> int:
        end = off + 1
        while end < size and end not in info.labels \
                and end not in info.mapping and end not in synthetic:
            end += 1
        return end

    def data_line(self, info: _Section, off: int, size: int,
                  out: list[str], synthetic: dict[int, str]) -> int:
        data = info.sec.data
        r = info.relocs.get(off)
        if r is not None:
            width = E.RELOC_SIZES.get(r.type, 1)
            op = "DC.W" if width == 2 else "DC.B"
            out.append(_line(op, self.reloc_text(r), self.comment(
                info.base + off, bytes(data[off:off + width]))))
            return off + width
        end = off
        values = []
        while end < size and len(values) < 8:
            if end != off and (end in info.labels or end in info.mapping
                               or end in synthetic):
                break
            if end in info.relocs:
                break
            values.append(data[end])
            end += 1
        out.append(_line("DC.B", ", ".join(_hex(v, 1) for v in values),
                         self.comment(info.base + off,
                                      bytes(data[off:end]))))
        return end

    def decode(self, info: _Section, off: int, size: int):
        data = info.sec.data
        op = data[off]
        if op not in DECODE:
            return None
        mnemonic, mode = DECODE[op]
        n = SIZES[mode]
        if off + n > size:
            return None
        # An instruction can't run over a label or a mapping symbol.
        for k in range(off + 1, off + n):
            if k in info.labels or k in info.mapping:
                return None
        return mnemonic, mode, n

    def scan_targets(self, info: _Section, synthetic: dict[int, str]) -> None:
        off = 0
        in_data = False
        data = info.sec.data
        while off < len(data):
            if off in info.mapping:
                in_data = info.mapping[off] == "$d"
            d = None if in_data else self.decode(info, off, len(data))
            if d is None:
                off += 1
                continue
            _, mode, n = d
            if mode == "rel" and off + 1 not in info.relocs:
                disp = data[off + 1]
                target = off + 2 + (disp - 256 if disp >= 128 else disp)
                if 0 <= target <= len(data) and target not in info.labels:
                    synthetic.setdefault(target, f".L{info.base + target:04X}")
            off += n

    def instr_line(self, info: _Section, off: int, size: int,
                   out: list[str], function: E.Symbol | None,
                   synthetic: dict[int, str]) -> int:
        data = info.sec.data
        d = self.decode(info, off, size)
        if d is None:
            out.append(_line("DC.B", _hex(data[off], 1), self.comment(
                info.base + off, bytes(data[off:off + 1]))))
            return off + 1
        mnemonic, mode, n = d
        raw = bytes(data[off:off + n])
        operand = ""
        if mode not in ("imp", "acc"):
            r = info.relocs.get(off + 1)
            width = n - 1
            value = int.from_bytes(raw[1:], "little")
            if r is not None:
                text = self.reloc_text(r)
            elif mode == "rel":
                disp = value - 256 if value >= 128 else value
                target = off + 2 + disp
                labels = info.labels.get(target)
                if labels:
                    local = [s for s in labels if s.name.startswith(".")]
                    text = (local or labels)[0].name
                elif target in synthetic:
                    text = synthetic[target]
                else:
                    text = _hex(info.base + target, 2)
            elif mode == "imm":
                text = _hex(value, 1)
            else:
                text = None
                if self.exe:
                    text = self.addr_name(value, function)
                if text is None:
                    text = _hex(value, width)
                    if width == 2 and value < 0x100 and mode != "ind":
                        mnemonic += ".w"
            if mode == "imm":
                operand = "#" + text
            elif mode in ("zpx", "absx"):
                operand = text + ",X"
            elif mode in ("zpy", "absy"):
                operand = text + ",Y"
            elif mode == "ind":
                operand = f"({text})"
            elif mode == "indx":
                operand = f"({text},X)"
            elif mode == "indy":
                operand = f"({text}),Y"
            else:
                operand = text
        out.append(_line(mnemonic, operand,
                         self.comment(info.base + off, raw)))
        return off + n


def disassemble(
    elf: E.ElfFile, *, all_sections: bool = False,
    only: set[str] | None = None, options: Options | None = None,
) -> list[str]:
    """The listing of `elf`'s code sections, or of every loaded section
    with `all_sections`, or only of the sections named in `only`."""
    d = Disassembler(elf, options)
    out: list[str] = []
    kind = "executable" if d.exe else "object"
    out.append(f"; elf32-mos {kind}")
    out.append("   PROCESSOR 6502")
    text = elf.link_text()
    if text and not d.exe:
        out.append("")
        out.extend(text.rstrip("\n").splitlines())
    if d.exe:
        absolute = sorted(
            (s for s in elf.symbols
             if s.section == E.SHN_ABS and s.type != E.STT_FILE
             and not s.name.startswith(".")),
            key=lambda s: (s.value, s.name),
        )
        if absolute:
            out.append("")
            seen = set()
            for s in absolute:
                if s.name in seen:
                    continue
                seen.add(s.name)
                width = 1 if s.value < 0x100 else 2
                out.append(f"{s.name}\tEQU\t{_hex(s.value, width)}")
    load = {}
    for seg in elf.segments:
        load[seg.vaddr] = seg.paddr
    for sec in elf.sections:
        if not sec.flags & E.SHF_ALLOC:
            continue
        if only is not None and sec.name not in only:
            continue
        if only is None and not all_sections and not (
            sec.flags & E.SHF_EXECINSTR
        ):
            continue
        if sec.size == 0 and not any(
            s.section is sec for s in elf.symbols
        ):
            continue
        out.append("")
        out.append(f"; Disassembly of section {sec.name}"
                   + (f" at ${sec.addr:04X}" if d.exe else ""))
        if d.exe:
            out.append(_line("SEG.U" if sec.type == E.SHT_NOBITS else "SEG",
                             _seg_name(sec.name)))
            lma = load.get(sec.addr, sec.addr)
            out.append(_line("ORG", _hex(lma, 2)))
            if lma != sec.addr:
                out.append(_line("RORG", _hex(sec.addr, 2)))
        out.extend(d.section_lines(sec))
        if d.exe and load.get(sec.addr, sec.addr) != sec.addr:
            out.append(_line("REND"))
    return out


def _seg_name(name: str) -> str:
    import re
    return re.sub(r"[^A-Za-z0-9_]", "_", name.lstrip(".")) or "seg"
