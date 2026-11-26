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

import sys


def main(argv: list[str]) -> int:
    """Lists the symbols of the files the command line `argv` names (the
    program's name first) and returns the exit status. The C++ port does
    it (`c6502cpp nm`, through `cpp_backend.run_tool`). A bad command
    line ends with SystemExit(2), as argparse's does, and the help
    returns 0."""
    import cpp_backend
    return cpp_backend.run_tool("nm", argv)


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
