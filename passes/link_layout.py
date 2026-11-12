"""Laying a linked program out by a linker script (`compile.py --link -T
script.ld`; the script language is in passes/link_script.py).

Each unit's body is split into its items, one per function and
object (`item_chunks`), using the `item` lines of its link metadata
(passes/sections.py). `layout_lines` then writes the output sections in
script order, each one a dasm segment:

    SEG text                    ; SEG.U for a NOLOAD section
    ORG <load address>
__text_load:                    ; only with AT> or AT(...)
    RORG <run address>
__text_start:
    <the items whose section matches, unit by unit, in order>
__text_end:
    REND
__text_load_end:

Sizes aren't known until dasm assembles the program, so every address
the script computes is a dasm expression: a section in a region
starts at the region's origin or at the end label of the region's
previous section. `RORG` gives the code its run address while the
bytes go at the load address, which is how `AT>` puts `.data` in ROM
to be copied to RAM. Each section's symbols are named after it, with
the dots turned into underscores (`.text.boot` gives
`__text_boot_start`); `ADDR`, `LOADADDR`, and `SIZEOF` in the script
refer to them. `. = expr` inside a section skips to `expr` with a
`DS.B`, which works the same in a relocated and an uninitialized
segment.

After each section, an `IF ... ERR` block makes dasm stop when the
section runs past the end of its region, or its load bytes past the
end of the load region.

`startup_lines` is code for `__zp_init`, which the boot code calls
before `main`: it copies each `AT>` section from its load address to
its run address and clears each `NOLOAD` section, using `DPTR` and
the first four bytes of `HARGS`, which hold nothing yet then.
"""
from __future__ import annotations

import dataclasses
import fnmatch
import re
from dataclasses import dataclass

from passes.link_script import (
    Assign, Binary, Call, Dot, Input, Num, OutputSection, Script, Sym, Unary,
)
from passes.zp_link_metadata import ItemMeta


class LayoutError(Exception):
    """A program the script can't lay out."""


@dataclass
class Unit:
    """One input file: its name for messages, its items in output
    order, and each item's lines."""
    path: str
    items: list[ItemMeta]
    chunks: dict[str, list[str]]


def item_chunks(body: list[str], items: list[ItemMeta]) -> dict[str, list[str]]:
    """Splits `body` at the label line of each item in `items`. Each
    item's lines run from its label to the next item's, without
    trailing blank lines."""
    names = {it.name for it in items}
    out: dict[str, list[str]] = {}
    current: list[str] | None = None
    for line in body:
        if line.endswith(":") and line[:-1] in names:
            current = out.setdefault(line[:-1], [])
        if current is not None:
            current.append(line)
        elif line.strip():
            raise LayoutError(
                f"line {line!r} comes before any item in its unit"
            )
    for lines in out.values():
        while lines and lines[-1] == "":
            lines.pop()
    missing = names - set(out)
    if missing:
        raise LayoutError(
            f"the unit's metadata lists {sorted(missing)}, which its body "
            f"doesn't define"
        )
    return out


_LABEL_CHARS = r"A-Za-z0-9_.@"


def rename_locals(units: list[tuple], taken: set[str]) -> list[tuple]:
    """Gives every internal item and internal zero-page object whose
    name another unit also uses, or that `taken` (the program's other
    names) holds, a name of its own: `name.k`, for unit `k`. Each unit
    is `(items, zp_objects, body)`, and comes back with the names
    rewritten in all three. Raises `LayoutError` when two units define
    the same external name."""
    owner: dict[str, int] = {}
    users: dict[str, set[int]] = {}
    for k, (items, zp_objects, _) in enumerate(units):
        for it in items:
            users.setdefault(it.name, set()).add(k)
            if not it.local:
                if it.name in owner:
                    raise LayoutError(
                        f"`{it.name}` is defined in multiple TUs"
                    )
                owner[it.name] = k
        for z in zp_objects:
            users.setdefault(z.name, set()).add(k)
    out = []
    for k, (items, zp_objects, body) in enumerate(units):
        local_names = (
            [it.name for it in items if it.local]
            + [z.name for z in zp_objects if z.local]
        )
        renames = {
            name: f"{name}.{k}" for name in local_names
            if len(users[name]) > 1 or name in taken
        }
        if renames:
            pattern = re.compile(
                rf"(?<![{_LABEL_CHARS}])("
                + "|".join(re.escape(n) for n in renames)
                + rf")(?![{_LABEL_CHARS}])"
            )
            body = [pattern.sub(lambda m: renames[m.group(1)], line)
                    for line in body]
            items = [
                ItemMeta(name=renames.get(it.name, it.name),
                         section=it.section, local=it.local, zero=it.zero)
                for it in items
            ]
            zp_objects = [
                dataclasses.replace(z, name=renames.get(z.name, z.name))
                for z in zp_objects
            ]
        out.append((items, zp_objects, body))
    return out


def _sym(section: str) -> str:
    """The part of a section's symbols named after it."""
    return re.sub(r"[^A-Za-z0-9_]", "_", section.lstrip("."))


def _expr(e, script: Script, dot: str | None) -> str:
    """`e` as a dasm expression. `dot` is what `.` stands for."""
    def go(e) -> str:
        match e:
            case Num(value=v):
                return f"${v & 0xFFFFFFFF:04X}"
            case Sym(name=n):
                return n
            case Dot():
                if dot is None:
                    raise LayoutError("`.` isn't defined here")
                return dot
            case Unary(op=op, operand=o):
                return f"[{op}{go(o)}]"
            case Binary(op=op, left=l, right=r):
                return f"[{go(l)} {op} {go(r)}]"
            case Call(fn="ALIGN", args=args):
                if len(args) == 1:
                    base, n = dot, args[0]
                    if base is None:
                        raise LayoutError("`ALIGN(n)` needs `.`")
                else:
                    base, n = go(args[0]), args[1]
                if not isinstance(n, Num) or n.value & (n.value - 1):
                    raise LayoutError(
                        "ALIGN takes a power of two written as a number"
                    )
                return _align(base, n.value)
            case Call(fn=fn, args=[Sym(name=name)]) if fn in (
                "ORIGIN", "LENGTH",
            ):
                region = script.regions.get(name)
                if region is None:
                    raise LayoutError(f"{fn}({name}) names no region")
                value = region.origin if fn == "ORIGIN" else region.length
                return f"${value:04X}"
            case Call(fn=fn, args=[Sym(name=name)]) if fn in (
                "ADDR", "LOADADDR", "SIZEOF",
            ):
                s = _sym(name)
                if fn == "ADDR":
                    return f"__{s}_start"
                if fn == "LOADADDR":
                    return f"__{s}_load"
                return f"[__{s}_end - __{s}_start]"
        raise LayoutError(f"can't use {e!r} in a linker script expression")
    return go(e)


def _align(base: str, n: int) -> str:
    if n <= 1:
        return base
    return f"[[{base} + {n - 1}] & ${0xFFFF & ~(n - 1):04X}]"


def layout_lines(
    script: Script, units: list[Unit], instr_line,
) -> list[str]:
    """The program's body laid out by `script`. `instr_line(opcode,
    operand)` formats one line the way asm_emit does. Raises
    `LayoutError` for an item no output section takes, an item that a
    NOLOAD section can't hold, or a section with nowhere to go."""
    placed: set[tuple[int, str]] = set()
    cursor = {
        name: f"${r.origin:04X}" for name, r in script.regions.items()
    }
    prev_end: str | None = None
    out: list[str] = []
    for stmt in script.statements:
        if isinstance(stmt, Assign):
            out.append(
                f"{stmt.symbol}\tEQU\t{_expr(stmt.expr, script, prev_end)}"
            )
            continue
        sec: OutputSection = stmt
        s = _sym(sec.name)
        if sec.address is not None:
            vma = _expr(sec.address, script, prev_end)
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
                raise LayoutError(
                    f"ALIGN on {sec.name} has to be a number"
                )
            vma = _align(vma, sec.align.value)
        relocated = sec.at is not None or sec.load_region is not None
        if sec.at is not None:
            lma = _expr(sec.at, script, prev_end)
        elif sec.load_region is not None:
            lma = cursor[sec.load_region]
        else:
            lma = vma
        out.append("")
        out.append(instr_line("SEG.U" if sec.noload else "SEG", s))
        out.append(instr_line("ORG", lma))
        if relocated:
            out.append(f"__{s}_load:")
            out.append(instr_line("RORG", vma))
        out.append(f"__{s}_start:")
        for c in sec.contents:
            if isinstance(c, Assign):
                if c.symbol == ".":
                    target = _expr(c.expr, script, ".")
                    out.append(instr_line("DS.B", f"[{target} - .]"))
                elif isinstance(c.expr, Dot):
                    out.append(f"{c.symbol}:")
                else:
                    out.append(
                        f"{c.symbol}\tEQU\t{_expr(c.expr, script, '.')}"
                    )
                continue
            assert isinstance(c, Input)
            for k, unit in enumerate(units):
                for it in unit.items:
                    if (k, it.name) in placed:
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
                    placed.add((k, it.name))
                    out.append("")
                    out.extend(unit.chunks[it.name])
        out.append(f"__{s}_end:")
        if relocated:
            out.append(instr_line("REND"))
            out.append(f"__{s}_load_end:")
        else:
            out.append(f"__{s}_load\tEQU\t__{s}_start")
        for region, end_label in (
            (sec.region, f"__{s}_end"),
            (sec.load_region, f"__{s}_load_end"),
        ):
            if region is None:
                continue
            r = script.regions[region]
            out.extend(_overflow_check(sec.name, region, r.end, end_label,
                                       instr_line))
            cursor[region] = end_label
        prev_end = f"__{s}_end"
    unplaced = [
        f"`{it.name}` (section {it.section}, {unit.path})"
        for k, unit in enumerate(units) for it in unit.items
        if (k, it.name) not in placed
    ]
    if unplaced:
        raise LayoutError(
            "no output section in the script takes "
            + ", ".join(unplaced)
        )
    return out


def _overflow_check(section, region, end, end_label, instr_line):
    return [
        instr_line("IF", f"{end_label} > ${end:04X}"),
        instr_line(
            "ECHO",
            f'"section {section} runs past the end of region {region} by",'
            f" [{end_label} - ${end:04X}]d, \"bytes\"",
        ),
        instr_line("ERR"),
        instr_line("ENDIF"),
    ]


def startup_lines(script: Script, instr_line) -> list[str]:
    """`__zp_init` code that copies each relocated output section to
    its run address and clears each NOLOAD one. Empty when there are
    none."""
    out: list[str] = []
    for k, sec in enumerate(script.output_sections()):
        relocated = sec.at is not None or sec.load_region is not None
        if not (relocated or sec.noload):
            continue
        s = _sym(sec.name)
        size = f"[__{s}_end - __{s}_start]"
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
