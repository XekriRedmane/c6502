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
same bytes at the same addresses (see cpp/src/binutils/disasm.h). The
disassembly of an object shows each relocation's symbol in the
operand it fills in, as `objdump -dr` does.
"""
from __future__ import annotations

import sys


def main(argv: list[str]) -> int:
    """Shows what's in the files the command line `argv` names (the
    program's name first) and returns the exit status. The C++ port does
    it (`c6502cpp objdump`, through `cpp_backend.run_tool`). A bad
    command line ends with SystemExit(2), as argparse's does, and the
    help returns 0."""
    import cpp_backend
    return cpp_backend.run_tool("objdump", argv)


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
