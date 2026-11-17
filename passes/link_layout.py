"""Laying a linked program out: where each input section goes, by a
linker script (`compile.py -T script.ld`; the script language is in
passes/link_script.py) or by the default script, which puts `.text`,
`.rodata`, `.data`, and `.bss` one after another from the origin.

Each unit's items (its functions and objects, `item` lines in its link
metadata, passes/sections.py) are its input sections, one each in the
object (binutils/elf.py). `lay_out` walks the script's statements in
order, keeping each memory region's next free address, and places the
items each output section takes, unit by unit in the order the units
were given, and in each unit in the order it lists them. Since every
section's size is known, each address is a number, computed here.

Every output section `.name` defines `__name_start` and `__name_end`
(its run addresses), `__name_load` and `__name_load_end` (where its
bytes are in memory before the startup code copies them, which are
the run addresses unless the section has `AT>` or `AT(...)`), and
`__name_size`. Dots and other characters that can't be in a symbol
become underscores (`.text.boot` gives `__text_boot_start`). `ADDR`,
`LOADADDR`, and `SIZEOF` in the script give those values. `. = expr`
inside a section skips to `expr`, with zero bytes in a section that's
loaded.

Inside a section, a function that ends with a `JMP` to another of its
functions goes right before that one, and the `JMP` is dropped
(`fall_through`), as hand-written 6502 code falls from one routine
into the next. A section marked `(NOREORDER)` keeps its order, and
only a function already in front of its target loses the `JMP`.
Nothing moves across an assignment in the section, since one marks
an address.

A section that runs past the end of its region, or whose load bytes
run past the end of its load region, is an error.

`startup_lines` is code for `__zp_init`, which the boot code calls
before `main`: it copies each `AT>` section from its load address to
its run address and clears each `NOLOAD` section, using `DPTR` and
the first four bytes of `HARGS`, which hold nothing yet then.
"""
from __future__ import annotations

import fnmatch
import re
from dataclasses import dataclass, field

from binutils import elf as E
from binutils.ld import OutputSection as Output, Placed
from passes.link_script import (
    Assign, Binary, Call, Dot, Input, Num, OutputSection, Script, Sym, Unary,
)
from passes.zp_link_metadata import ItemMeta


class LayoutError(Exception):
    """A program the script can't lay out."""


@dataclass
class Unit:
    """One input file: its name (for messages and the script's file
    patterns), its number in the link, its items in order, each item's
    input section, and for each function that ends in a `JMP` to a
    function, that function's name (`tail_jump`)."""
    path: str
    index: int
    items: list[ItemMeta]
    sections: dict[str, E.Section]
    tails: dict[str, str] = field(default_factory=dict)


@dataclass
class Entry:
    """An item for `fall_through`: its name, whether it's a function,
    and the function its final `JMP` goes to, if it ends in one."""
    name: str
    function: bool = False
    tail: str | None = None
    payload: object = None


@dataclass
class Layout:
    """The output sections, with every input section placed, and the
    symbols the layout defines (section bounds and the script's
    assignments)."""
    outputs: list[Output] = field(default_factory=list)
    symbols: dict[str, int] = field(default_factory=dict)


def tail_jump(sec: E.Section, symbols: list[E.Symbol]) -> str | None:
    """The name of the function `sec` jumps to at its end, when the
    function's last instruction is `JMP name` (no offset) and no label
    or data follows it, else None. `symbols` are the section's
    object's symbols."""
    size = sec.size
    if sec.type == E.SHT_NOBITS or size < 3 or sec.data[size - 3] != 0x4C:
        return None
    r = next((r for r in sec.relocs if r.offset == size - 2), None)
    if r is None or r.type != E.R_MOS_ADDR16 or r.addend != 0:
        return None
    if r.symbol.name.startswith("."):
        return None
    mapping = None
    for s in symbols:
        if s.section is not sec:
            continue
        if s.value > size - 3 and s.type != E.STT_FUNC:
            # A label on the JMP's operand or after it.
            return None
        if s.name in ("$c", "$d") and s.value <= size - 3:
            if mapping is None or s.value >= mapping[0]:
                mapping = (s.value, s.name)
    if mapping is not None and mapping[1] == "$d":
        return None
    return r.symbol.name


def fall_through(entries: list[Entry], reorder: bool = True) -> list[tuple[Entry, bool]]:
    """`entries` (in the script's order) with each function that ends
    by jumping to another function of `entries` moved right before it,
    paired with True when its jump is dropped, so the first runs into
    the second.

    A function is the target of at most one such pair, the first of
    the functions jumping to it in `entries`, and a pair that would
    close a cycle is left out. Each chain of pairs is laid down where
    its first function is, and every other entry keeps its place. With
    `reorder` false, only a function already right before its target
    loses its jump, and nothing moves.

    Dropping the jump keeps any label on it, which then names the
    target's first instruction, the address the jump went to."""
    index = {e.name: i for i, e in enumerate(entries) if e.function}
    jumps: dict[int, int] = {}
    for i, e in enumerate(entries):
        if not e.function or e.tail is None:
            continue
        j = index.get(e.tail)
        if j is None or j == i or (not reorder and j != i + 1):
            continue
        jumps[i] = j
    after: dict[int, int] = {}
    before: dict[int, int] = {}
    for i in sorted(jumps):
        j = jumps[i]
        if j in before:
            continue
        end = j
        while end in after:
            end = after[end]
        if end == i:
            continue
        after[i] = j
        before[j] = i
    out: list[tuple[Entry, bool]] = []
    for i in range(len(entries)):
        if i in before:
            continue
        n: int | None = i
        while n is not None:
            out.append((entries[n], n in after))
            n = after.get(n)
    return out


def section_symbol(section: str) -> str:
    """The part of a section's symbols named after it."""
    return re.sub(r"[^A-Za-z0-9_]", "_", section.lstrip("."))


class _Evaluator:
    def __init__(self, script: Script, symbols: dict[str, int],
                 lookup) -> None:
        self.script = script
        self.symbols = symbols
        self.lookup = lookup

    def __call__(self, e, dot: int | None) -> int:
        match e:
            case Num(value=v):
                return v
            case Sym(name=n):
                if n in self.symbols:
                    return self.symbols[n]
                v = self.lookup(n)
                if v is None:
                    raise LayoutError(
                        f"the script uses `{n}` before it has a value"
                    )
                return v
            case Dot():
                if dot is None:
                    raise LayoutError("`.` isn't defined here")
                return dot
            case Unary(op=op, operand=o):
                v = self(o, dot)
                return -v if op == "-" else ~v
            case Binary(op=op, left=l, right=r):
                a, b = self(l, dot), self(r, dot)
                return _binary(op, a, b)
            case Call(fn="ALIGN", args=args):
                if len(args) == 1:
                    if dot is None:
                        raise LayoutError("`ALIGN(n)` needs `.`")
                    base, n = dot, args[0]
                else:
                    base, n = self(args[0], dot), args[1]
                if not isinstance(n, Num) or n.value & (n.value - 1):
                    raise LayoutError(
                        "ALIGN takes a power of two written as a number"
                    )
                return _align(base, n.value)
            case Call(fn=fn, args=[Sym(name=name)]) if fn in (
                "ORIGIN", "LENGTH",
            ):
                region = self.script.regions.get(name)
                if region is None:
                    raise LayoutError(f"{fn}({name}) names no region")
                return region.origin if fn == "ORIGIN" else region.length
            case Call(fn=fn, args=[Sym(name=name)]) if fn in (
                "ADDR", "LOADADDR", "SIZEOF",
            ):
                s = section_symbol(name)
                key = {"ADDR": "start", "LOADADDR": "load",
                       "SIZEOF": "size"}[fn]
                sym = f"__{s}_{key}"
                if sym not in self.symbols:
                    raise LayoutError(
                        f"{fn}({name}) comes before section {name} is "
                        f"laid out"
                    )
                return self.symbols[sym]
        raise LayoutError(f"can't use {e!r} in a linker script expression")


def _binary(op: str, a: int, b: int) -> int:
    if op == "+":
        return a + b
    if op == "-":
        return a - b
    if op == "*":
        return a * b
    if op == "/":
        if b == 0:
            raise LayoutError("division by zero in the linker script")
        return a // b
    if op == "&":
        return a & b
    if op == "|":
        return a | b
    if op == "^":
        return a ^ b
    if op == "<<":
        return a << b
    if op == ">>":
        return a >> b
    raise LayoutError(f"unknown operator {op} in the linker script")


def _align(base: int, n: int) -> int:
    if n <= 1:
        return base
    return (base + n - 1) & ~(n - 1)


def default_script(units: list[Unit], origin: int) -> Script:
    """The script a link without `-T` uses: `.text` from `origin`,
    then `.rodata`, `.data`, `.bss`, and then each other section name
    the units use, in the order they first appear, each right after
    the one before."""
    std = [".text", ".rodata", ".data", ".bss"]
    names = list(std)
    for u in units:
        for it in u.items:
            base = it.section
            if not any(base == n or base.startswith(n + ".") for n in std) \
                    and base not in names:
                names.append(base)
    script = Script()
    for k, name in enumerate(names):
        patterns = [name, name + ".*"] if name in std else [name]
        script.statements.append(OutputSection(
            name=name, address=Num(origin) if k == 0 else None,
            contents=[Input(patterns=patterns)],
        ))
    return script


def lay_out(
    script: Script, units: list[Unit], lookup=lambda name: None,
) -> Layout:
    """Places every item of `units` by `script`. `lookup(name)` gives
    the value of a symbol the script names that the layout doesn't
    define (such as an absolute symbol from a unit), or None. Raises
    `LayoutError` for an item no output section takes, an item that a
    NOLOAD section can't hold, a section with nowhere to go, or one
    that runs past the end of its region."""
    layout = Layout()
    sym = layout.symbols
    ev = _Evaluator(script, sym, lookup)
    placed: set[tuple[int, str]] = set()
    cursor = {name: r.origin for name, r in script.regions.items()}
    prev_end: int | None = None
    for stmt in script.statements:
        if isinstance(stmt, Assign):
            if stmt.symbol == ".":
                raise LayoutError("`.` can only be set inside a section")
            sym[stmt.symbol] = ev(stmt.expr, prev_end)
            continue
        sec: OutputSection = stmt
        s = section_symbol(sec.name)
        if sec.address is not None:
            vma = ev(sec.address, prev_end)
        elif sec.region is not None:
            vma = cursor[sec.region]
        elif prev_end is not None:
            vma = prev_end
        else:
            raise LayoutError(
                f"output section {sec.name} has no address and no region"
            )
        if sec.align is not None:
            if not isinstance(sec.align, Num):
                raise LayoutError(f"ALIGN on {sec.name} has to be a number")
            vma = _align(vma, sec.align.value)
        relocated = sec.at is not None or sec.load_region is not None
        if sec.at is not None:
            lma = ev(sec.at, prev_end)
        elif sec.load_region is not None:
            lma = cursor[sec.load_region]
        else:
            lma = vma
        out = Output(name=sec.name, addr=vma, load_addr=lma, size=0,
                     noload=sec.noload)
        sym[f"__{s}_start"] = vma
        if relocated:
            sym[f"__{s}_load"] = lma
        pc = vma
        run: list[Entry] = []

        def flush() -> None:
            nonlocal pc
            for entry, dropped in fall_through(run, reorder=sec.reorder):
                unit, it_section = entry.payload
                size = it_section.size - (3 if dropped else 0)
                out.parts.append(Placed(unit.index, it_section, pc, size))
                if it_section.flags & E.SHF_EXECINSTR:
                    out.executable = True
                pc += size
            run.clear()

        for c in sec.contents:
            if isinstance(c, Assign):
                flush()
                if c.symbol == ".":
                    target = ev(c.expr, pc)
                    if target < pc:
                        raise LayoutError(
                            f"`. = ...` in {sec.name} moves the location "
                            f"back from ${pc:04X} to ${target:04X}"
                        )
                    pc = target
                else:
                    sym[c.symbol] = ev(c.expr, pc)
                continue
            assert isinstance(c, Input)
            for unit in units:
                name = unit.path.replace("\\", "/").rsplit("/", 1)[-1]
                if not fnmatch.fnmatchcase(name, c.file):
                    continue
                for it in unit.items:
                    if (unit.index, it.name) in placed:
                        continue
                    if not any(fnmatch.fnmatchcase(it.section, p)
                               for p in c.patterns):
                        continue
                    if sec.noload and not it.zero:
                        raise LayoutError(
                            f"`{it.name}` (section {it.section}, "
                            f"{unit.path}) has initial bytes, so the "
                            f"NOLOAD output section {sec.name} can't "
                            f"hold it"
                        )
                    placed.add((unit.index, it.name))
                    it_section = unit.sections[it.name]
                    run.append(Entry(
                        name=it.name,
                        function=bool(it_section.flags & E.SHF_EXECINSTR),
                        tail=unit.tails.get(it.name),
                        payload=(unit, it_section),
                    ))
        flush()
        out.size = pc - vma
        end = pc
        sym[f"__{s}_end"] = end
        sym[f"__{s}_size"] = out.size
        if relocated:
            sym[f"__{s}_load_end"] = lma + out.size
        else:
            sym[f"__{s}_load"] = vma
            sym[f"__{s}_load_end"] = end
        for region, last in (
            (sec.region, end), (sec.load_region, lma + out.size),
        ):
            if region is None:
                continue
            r = script.regions[region]
            if last > r.end:
                raise LayoutError(
                    f"section {sec.name} runs past the end of region "
                    f"{region} by {last - r.end} bytes"
                )
            cursor[region] = last
        layout.outputs.append(out)
        prev_end = end
    unplaced = [
        f"`{it.name}` (section {it.section}, {unit.path})"
        for unit in units for it in unit.items
        if (unit.index, it.name) not in placed
    ]
    if unplaced:
        raise LayoutError(
            "no output section in the script takes " + ", ".join(unplaced)
        )
    return layout


def startup_lines(script: Script, instr_line) -> list[str]:
    """`__zp_init` code that copies each relocated output section to
    its run address and clears each NOLOAD one. Empty when there are
    none."""
    out: list[str] = []
    for k, sec in enumerate(script.output_sections()):
        relocated = sec.at is not None or sec.load_region is not None
        if not (relocated or sec.noload):
            continue
        s = section_symbol(sec.name)
        size = f"__{s}_size"
        if relocated:
            out += [
                instr_line("LDA", f"#<__{s}_load"),
                instr_line("STA", "DPTR"),
                instr_line("LDA", f"#>__{s}_load"),
                instr_line("STA", "DPTR+1"),
            ]
        out += [
            instr_line("LDA", f"#<__{s}_start"),
            instr_line("STA", "HARGS"),
            instr_line("LDA", f"#>__{s}_start"),
            instr_line("STA", "HARGS+1"),
            instr_line("LDA", f"#<{size}"),
            instr_line("STA", "HARGS+2"),
            instr_line("LDA", f"#>{size}"),
            instr_line("STA", "HARGS+3"),
            instr_line("LDY", "#$00"),
            f".start{k}:",
            instr_line("LDA", "HARGS+2"),
            instr_line("ORA", "HARGS+3"),
            instr_line("BEQ", f".done{k}"),
        ]
        if relocated:
            out += [
                instr_line("LDA", "(DPTR),Y"),
                instr_line("STA", "(HARGS),Y"),
                instr_line("INC", "DPTR"),
                instr_line("BNE", f".src{k}"),
                instr_line("INC", "DPTR+1"),
                f".src{k}:",
            ]
        else:
            out += [
                instr_line("LDA", "#$00"),
                instr_line("STA", "(HARGS),Y"),
            ]
        out += [
            instr_line("INC", "HARGS"),
            instr_line("BNE", f".dst{k}"),
            instr_line("INC", "HARGS+1"),
            f".dst{k}:",
            instr_line("LDA", "HARGS+2"),
            instr_line("BNE", f".low{k}"),
            instr_line("DEC", "HARGS+3"),
            f".low{k}:",
            instr_line("DEC", "HARGS+2"),
            instr_line("JMP", f".start{k}"),
            f".done{k}:",
        ]
    return out
