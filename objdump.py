"""Shows what's in c6502 objects and executables, as GNU objdump does,
with the disassembly in dasm's syntax.

    uv run python objdump.py [options] file...

Options:

    -d, --disassemble      disassemble the code sections
    -D, --disassemble-all  disassemble every loaded section, data as DC.B
    -j, --section NAME     only the sections named NAME (repeatable)
    -f, --file-headers     the file's type, machine, flags, and entry
    -h, --section-headers  the sections, with sizes and addresses
    -t, --syms             the symbol table
    -r, --reloc            the relocations
    -s, --full-contents    every loaded section's bytes in hex
    -x, --all-headers      -f, -h, -t, -r, and the link metadata
        --link-metadata    the c6502 link metadata
        --no-show-raw-insn leave the bytes out of the disassembly
        --no-addresses     leave the addresses out of the disassembly

The disassembly of an executable (`-D`) assembles with dasm into the
same bytes at the same addresses (see `binutils/disasm.py`). The
disassembly of an object shows each relocation's symbol in the
operand it fills in, as `objdump -dr` does.
"""
from __future__ import annotations

import argparse
import sys

from binutils import elf as E
from binutils.disasm import Options, disassemble

def write_lf(text: str) -> None:
    """Writes `text` to stdout with LF line endings on every platform,
    so that a listing redirected to a file matches the checked-in one
    byte for byte on Windows too."""
    buffer = getattr(sys.stdout, "buffer", None)
    if buffer is None:
        sys.stdout.write(text)
    else:
        sys.stdout.flush()
        buffer.write(text.encode("utf-8"))
        buffer.flush()


_TYPES = {E.ET_REL: "relocatable object", E.ET_EXEC: "executable"}
_SYMTYPES = {
    E.STT_NOTYPE: "", E.STT_OBJECT: "O", E.STT_FUNC: "F",
    E.STT_SECTION: "S", E.STT_FILE: "f",
}


def _section_name(s: E.Symbol) -> str:
    if s.section == E.SHN_UNDEF:
        return "*UND*"
    if s.section == E.SHN_ABS:
        return "*ABS*"
    return s.section.name


def _flags(sec: E.Section) -> str:
    out = []
    if sec.type != E.SHT_NOBITS:
        out.append("CONTENTS")
    if sec.flags & E.SHF_ALLOC:
        out.append("ALLOC")
        if sec.type != E.SHT_NOBITS:
            out.append("LOAD")
    if sec.relocs:
        out.append("RELOC")
    if not sec.flags & E.SHF_WRITE and sec.flags & E.SHF_ALLOC:
        out.append("READONLY")
    if sec.flags & E.SHF_EXECINSTR:
        out.append("CODE")
    elif sec.flags & E.SHF_ALLOC:
        out.append("DATA")
    return ", ".join(out)


def file_header(elf: E.ElfFile, path: str) -> list[str]:
    return [
        f"{path}:     file format elf32-mos",
        f"architecture: mos6502, flags 0x{elf.flags:08X}:",
        f"{_TYPES.get(elf.type, f'type {elf.type}')}",
        f"start address 0x{elf.entry:04X}",
    ]


def section_headers(elf: E.ElfFile) -> list[str]:
    load = {seg.vaddr: seg.paddr for seg in elf.segments}
    out = ["Sections:",
           "Idx Name                 Size  VMA   LMA   Algn"]
    for i, sec in enumerate(elf.sections):
        lma = load.get(sec.addr, sec.addr)
        out.append(
            f"{i:3d} {sec.name:<20} {sec.size:04X}  {sec.addr:04X}  "
            f"{lma:04X}  2**{max(sec.align, 1).bit_length() - 1}"
        )
        out.append(f"{'':25}{_flags(sec)}")
    return out


def symbol_table(elf: E.ElfFile) -> list[str]:
    out = ["SYMBOL TABLE:"]
    for s in elf.symbols:
        bind = "g" if s.is_global else "l"
        kind = _SYMTYPES.get(s.type, "?")
        out.append(
            f"{s.value & 0xFFFF:04X} {bind:<2}{kind:<2} "
            f"{_section_name(s):<12} {s.size:04X} {s.name}"
        )
    return out


def relocations(elf: E.ElfFile, only: set[str] | None) -> list[str]:
    out = []
    for sec in elf.sections:
        if not sec.relocs or (only is not None and sec.name not in only):
            continue
        owner = next(
            (s.name for s in elf.symbols
             if s.section is sec and s.type in (E.STT_FUNC, E.STT_OBJECT)),
            "",
        )
        out.append(f"RELOCATION RECORDS FOR [{sec.name}]"
                   + (f" ({owner})" if owner else "") + ":")
        out.append("OFFSET TYPE              VALUE")
        for r in sec.relocs:
            name = E.RELOC_NAMES.get(r.type, str(r.type))
            add = ""
            if r.addend:
                add = f"+{r.addend}" if r.addend > 0 else f"-{-r.addend}"
            out.append(f"{r.offset:04X}   {name:<17} {r.symbol.name}{add}")
        out.append("")
    return out


def full_contents(elf: E.ElfFile, only: set[str] | None) -> list[str]:
    out = []
    for sec in elf.sections:
        if sec.type == E.SHT_NOBITS:
            continue
        if only is not None and sec.name not in only:
            continue
        if only is None and not sec.flags & E.SHF_ALLOC \
                and sec.name != E.LINK_SECTION:
            continue
        out.append(f"Contents of section {sec.name}:")
        data = bytes(sec.data)
        for off in range(0, len(data), 16):
            chunk = data[off:off + 16]
            hexes = " ".join(
                chunk[k:k + 4].hex() for k in range(0, len(chunk), 4)
            )
            text = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
            out.append(f" {sec.addr + off:04x} {hexes:<35}  {text}")
    return out


def main(argv: list[str]) -> int:
    """Shows what's in the files the command line `argv` names (the
    program's name first) and returns the exit status. Under
    `C6502_CPP_BACKEND=1` the C++ port does it (`c6502cpp objdump`,
    through `cpp_backend.run_tool`), with the same output, messages, and
    exit status."""
    import os
    if os.environ.get("C6502_CPP_BACKEND") == "1":
        import cpp_backend
        return cpp_backend.run_tool("objdump", argv)
    ap = argparse.ArgumentParser(
        prog="objdump.py",
        description="Show what's in c6502 objects and executables.",
        add_help=False,
    )
    ap.add_argument("--help", action="help",
                    help="show this message and exit")
    ap.add_argument("files", nargs="+")
    ap.add_argument("-d", "--disassemble", action="store_true")
    ap.add_argument("-D", "--disassemble-all", action="store_true")
    ap.add_argument("-j", "--section", action="append", dest="sections")
    ap.add_argument("-f", "--file-headers", action="store_true")
    ap.add_argument("-h", "--section-headers", action="store_true")
    ap.add_argument("-t", "--syms", action="store_true")
    ap.add_argument("-r", "--reloc", action="store_true")
    ap.add_argument("-s", "--full-contents", action="store_true")
    ap.add_argument("-x", "--all-headers", action="store_true")
    ap.add_argument("--link-metadata", action="store_true")
    ap.add_argument("--no-show-raw-insn", action="store_true")
    ap.add_argument("--no-addresses", action="store_true")
    args = ap.parse_args(argv[1:])
    if not any((args.disassemble, args.disassemble_all, args.file_headers,
                args.section_headers, args.syms, args.reloc,
                args.full_contents, args.all_headers, args.link_metadata)):
        print("objdump.py: give at least one of -d -D -f -h -t -r -s -x "
              "--link-metadata", file=sys.stderr)
        return 2
    only = set(args.sections) if args.sections else None
    status = 0
    for path in args.files:
        try:
            elf = E.read_file(path)
        except (OSError, E.ElfError) as e:
            print(f"objdump.py: {e}", file=sys.stderr)
            status = 1
            continue
        lines: list[str] = []
        if args.file_headers or args.all_headers:
            lines += file_header(elf, path) + [""]
        if args.section_headers or args.all_headers:
            lines += section_headers(elf) + [""]
        if args.syms or args.all_headers:
            lines += symbol_table(elf) + [""]
        if args.reloc or args.all_headers:
            lines += relocations(elf, only)
        if args.link_metadata or args.all_headers:
            text = elf.link_text()
            lines += (text.rstrip("\n").splitlines() if text
                      else ["(no link metadata)"]) + [""]
        if args.full_contents:
            lines += full_contents(elf, only) + [""]
        if args.disassemble or args.disassemble_all:
            lines += disassemble(
                elf, all_sections=args.disassemble_all, only=only,
                options=Options(raw=not args.no_show_raw_insn,
                                addresses=not args.no_addresses),
            )
        while lines and lines[-1] == "":
            lines.pop()
        write_lf("\n".join(lines) + "\n")
    return status


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
