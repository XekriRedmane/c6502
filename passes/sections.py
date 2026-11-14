"""The section each function and static object goes in, for
`compile.py --link -T script` (passes/link_script.py, passes/linker.py).

Every function and every static-storage object the output defines is
an *item* in a section. `[[c6502::section("name")]]` names the
section. Without it, the item goes in the section GCC would use:

| Item | Section |
|---|---|
| a function, and the generated `__zp_init` | `.text` |
| a `const` object, and a string literal (`str@N`) | `.rodata` |
| an object whose initial value is all zero bytes | `.bss` |
| any other object | `.data` |

A string literal is `.rodata` although its type isn't `const`, since
changing one is undefined (C99 §6.4.5.6). A `.bss` item is laid down as
`DS.B`, which a `NOLOAD` output section can hold without bytes in the
program image; the startup code clears it (`passes/linker.py`).

The sections only matter to the linker. Without a script, the output
is laid out as before, and the link metadata block carries one `item`
line per item (`items`), which tells the linker where each one goes.
"""
from __future__ import annotations

import asm_ast
from passes.zp_link_metadata import ItemMeta


def items(prog: asm_ast.Program, symbols) -> list[ItemMeta]:
    """One `ItemMeta` for each top-level function and object of `prog`,
    the finished asm, in output order."""
    out: list[ItemMeta] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            sym = symbols.get(tl.name)
            section = getattr(sym.attrs, "section", None) if sym else None
            out.append(ItemMeta(
                name=tl.name, section=section or ".text",
                local=not tl.is_global, zero=False,
            ))
        elif isinstance(tl, asm_ast.StaticVariable):
            sym = symbols.get(tl.name)
            zero = all(isinstance(i, asm_ast.ZeroInit) for i in tl.init)
            section = getattr(sym.attrs, "section", None) if sym else None
            if section is None:
                if tl.name.startswith("str@") or (
                    sym is not None and _is_const(sym.type)
                ):
                    section = ".rodata"
                elif zero:
                    section = ".bss"
                else:
                    section = ".data"
            out.append(ItemMeta(
                name=tl.name, section=section,
                local=not tl.is_global, zero=zero,
            ))
    return out


def _is_const(t) -> bool:
    """True iff an object of type `t` is read-only: `t` is `const`, or
    an array of `const` elements."""
    import c99_ast
    while True:
        if isinstance(t, c99_ast.Const):
            return True
        if isinstance(t, c99_ast.Volatile):
            t = t.referenced_type
        elif isinstance(t, c99_ast.Array):
            t = t.element_type
        else:
            return False
