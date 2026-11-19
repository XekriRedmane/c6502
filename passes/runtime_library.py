"""The runtime library: the 6502 helpers the compiled code calls to
multiply, divide, and shift by a variable count (`mul16`, `sdivmod16`,
`asl8`, and the rest), which the linker adds to a program as a C
toolchain links routines from libgcc.

The helpers are the `asm_ast` functions of `sim/runtime_helpers.py`
(`all_helper_functions`), the same code the simulator runs. When a
program's objects name a helper that none of them defines and no
`--defsym` gives, `library_object` writes that helper and every helper
it calls (`sdivmod16` calls `udivmod16`) as text with `asm_emit` and
assembles it with `binutils.assembler` into one object, at link time.
Each helper is a function in a section of its own, `.text`, and the
object goes by the name `runtime.o` (`LIBRARY_NAME`), so a linker
script places the helpers with an input description such as
`runtime.o(.text)`, or with a `*(.text)` that catches every unit's
code. Without a script they go in `.text` after the program's code.

The helpers read and write only HARGS, which the object leaves
undefined, so they get the program's HARGS address with the rest of
the program's units. The object's link metadata has the HARGS bytes
the helpers touch (`runtime_layout.helper_footprints`), which the
linker counts in the size of the program's block.

The helpers without 6502 code (the floating-point ones, the 64-bit
ones, and `icall`) aren't in the library. A program that calls one
links only with `--defsym` giving its address.
"""
from __future__ import annotations

from functools import lru_cache

import asm_ast
from binutils import elf as E

# The name the library's object goes by in a link, which a linker
# script's file patterns match.
LIBRARY_NAME = "runtime.o"


@lru_cache(maxsize=1)
def _functions() -> dict[str, asm_ast.Function]:
    """Each helper's function, by name, in the order
    `all_helper_functions` gives them."""
    from sim.runtime_helpers import all_helper_functions
    return {fn.name: fn for fn in all_helper_functions()}


def helper_names() -> frozenset[str]:
    """The names of the helpers the library has."""
    return frozenset(_functions())


def _callees(fn: asm_ast.Function) -> set[str]:
    """The helpers `fn` calls or jumps to."""
    names = _functions()
    out: set[str] = set()
    for i in fn.instructions:
        if isinstance(i, asm_ast.Call) and i.name in names:
            out.add(i.name)
        elif isinstance(i, asm_ast.Jump) and i.target in names:
            out.add(i.target)
    return out


def closure(names) -> list[str]:
    """`names` and every helper they call, directly or through
    another, in the library's order. Names that aren't helpers are
    left out."""
    fns = _functions()
    todo = [n for n in names if n in fns]
    seen: set[str] = set()
    while todo:
        name = todo.pop()
        if name in seen:
            continue
        seen.add(name)
        todo.extend(_callees(fns[name]) - seen)
    return [n for n in fns if n in seen]


def needed(objects: list[tuple[str, E.ElfFile]],
           defined: set[str]) -> list[str]:
    """The helpers that a link of `objects` takes from the library:
    the ones some object names and no name in `defined` (the names
    the objects define, and those `--defsym` gives) covers, and the
    helpers they call that `defined` doesn't cover either."""
    fns = _functions()
    wanted = {
        s.name for _, obj in objects for s in obj.symbols
        if not s.defined and s.name in fns and s.name not in defined
    }
    return [n for n in closure(wanted) if n not in defined]


def library_object(names: list[str]) -> E.ElfFile:
    """The object that holds the helpers `names`, which `needed`
    returned, one section each. Its link metadata lists each helper as
    an item in `.text`, and gives the number of HARGS bytes the
    helpers touch as the object's HARGS need."""
    from asm_emit import emit_program
    from binutils.assembler import assemble
    from passes.asm_to_asm2 import translate_program
    from passes.runtime_layout import helper_footprints
    from passes.zp_link_metadata import (
        ItemMeta, LinkMetadata, format_metadata,
    )
    fns = _functions()
    footprints = helper_footprints()
    need = max((footprints[n] for n in names), default=0)
    meta = LinkMetadata(
        items=[ItemMeta(name=n, section=".text") for n in names],
        hargs_size=need, hargs_need=need,
    )
    prog = asm_ast.Program(top_level=[fns[n] for n in names])
    text = emit_program(
        translate_program(prog), link_metadata_lines=format_metadata(meta),
    )
    return assemble(text, LIBRARY_NAME)


@lru_cache(maxsize=1)
def helper_sizes() -> dict[str, int]:
    """The number of bytes each helper takes in a program, alone,
    without the helpers it calls."""
    out: dict[str, int] = {}
    for name in _functions():
        obj = library_object([name])
        sym = next(s for s in obj.symbols if s.name == name)
        out[name] = sym.section.size
    return out
