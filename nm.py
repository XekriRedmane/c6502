"""Lists the symbols of c6502 objects and executables, as GNU nm does.

    uv run python nm.py [options] file...

Each line is the symbol's value, a letter for its kind, and its name:

    U   undefined (another file defines it, or the linker places it)
    A   absolute
    T   in a code section
    R   in a read-only data section
    D   in a writable data section
    B   in an uninitialized (`.bss`) section

The letter is lowercase for a local symbol. A value is 4 hex digits,
since addresses on the 6502 are 16 bits, and an undefined symbol has
blanks in its place. Like GNU nm, the list is sorted by name.

Local labels (names that start with `.`), the `$c` and `$d` mapping
symbols, and file symbols are left out unless `-a` is given, as GNU
nm leaves out the assembler's local labels and ARM's mapping symbols.

Options:

    -a, --debug-syms     list every symbol
    -g, --extern-only    only global symbols
    -u, --undefined-only only undefined symbols
        --defined-only   only defined symbols
    -n, --numeric-sort   sort by value
    -p, --no-sort        keep the file's order
    -r, --reverse-sort   reverse the order
    -S, --print-size     print each defined symbol's size after its value
    -A, -o, --print-file-name
                         start each line with the file's name

`U` symbols marked with `STO_MOS_ZEROPAGE` (the unit reaches them
with zero-page addressing) are listed with a `z` after the letter
when `-a` is given.
"""
from __future__ import annotations

import argparse
import sys

from binutils import elf as E


def _letter(s: E.Symbol) -> str:
    if not s.defined:
        return "U"
    if s.section == E.SHN_ABS:
        c = "a"
    else:
        sec = s.section
        if sec.flags & E.SHF_EXECINSTR:
            c = "t"
        elif sec.type == E.SHT_NOBITS:
            c = "b"
        elif sec.flags & E.SHF_WRITE:
            c = "d"
        else:
            c = "r"
    return c.upper() if s.is_global else c


def _hidden(s: E.Symbol) -> bool:
    return (
        s.type in (E.STT_FILE, E.STT_SECTION)
        or s.name.startswith(".")
        or s.name in ("$c", "$d")
    )


def symbol_lines(
    elf: E.ElfFile, *, debug: bool = False, extern_only: bool = False,
    undefined_only: bool = False, defined_only: bool = False,
    sort: str = "name", reverse: bool = False, size: bool = False,
    prefix: str = "",
) -> list[str]:
    """The lines `nm` prints for `elf`."""
    syms = [s for s in elf.symbols if debug or not _hidden(s)]
    if extern_only:
        syms = [s for s in syms if s.is_global]
    if undefined_only:
        syms = [s for s in syms if not s.defined]
    if defined_only:
        syms = [s for s in syms if s.defined]
    if sort == "name":
        syms.sort(key=lambda s: (s.name, s.value))
    elif sort == "numeric":
        syms.sort(key=lambda s: (s.defined, s.value, s.name))
    if reverse:
        syms.reverse()
    out = []
    for s in syms:
        letter = _letter(s)
        if debug and not s.defined and s.other & E.STO_MOS_ZEROPAGE:
            letter += "z"
        if s.defined:
            value = f"{s.value & 0xFFFF:04X}"
            if size:
                value += f" {s.size:04X}"
        else:
            value = " " * (9 if size else 4)
        out.append(f"{prefix}{value} {letter} {s.name}")
    return out


from objdump import write_lf


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(
        prog="nm.py",
        description="List the symbols of c6502 objects and executables.",
    )
    ap.add_argument("files", nargs="+", help="object or executable files")
    ap.add_argument("-a", "--debug-syms", action="store_true",
                    help="list local labels, mapping and file symbols too")
    ap.add_argument("-g", "--extern-only", action="store_true",
                    help="only global symbols")
    ap.add_argument("-u", "--undefined-only", action="store_true",
                    help="only undefined symbols")
    ap.add_argument("--defined-only", action="store_true",
                    help="only defined symbols")
    ap.add_argument("-n", "--numeric-sort", action="store_true",
                    help="sort by value")
    ap.add_argument("-p", "--no-sort", action="store_true",
                    help="keep the file's order")
    ap.add_argument("-r", "--reverse-sort", action="store_true",
                    help="reverse the order")
    ap.add_argument("-S", "--print-size", action="store_true",
                    help="print the size of each defined symbol")
    ap.add_argument("-A", "-o", "--print-file-name", action="store_true",
                    help="start each line with the file's name")
    args = ap.parse_args(argv[1:])
    sort = "none" if args.no_sort else (
        "numeric" if args.numeric_sort else "name")
    status = 0
    many = len(args.files) > 1
    for path in args.files:
        try:
            elf = E.read_file(path)
        except (OSError, E.ElfError) as e:
            print(f"nm.py: {e}", file=sys.stderr)
            status = 1
            continue
        out: list[str] = []
        if many and not args.print_file_name:
            out += ["", f"{path}:"]
        lines = symbol_lines(
            elf, debug=args.debug_syms, extern_only=args.extern_only,
            undefined_only=args.undefined_only,
            defined_only=args.defined_only, sort=sort,
            reverse=args.reverse_sort, size=args.print_size,
            prefix=f"{path}:" if args.print_file_name else "",
        )
        out += lines
        if out:
            write_lf("\n".join(out) + "\n")
    return status


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
