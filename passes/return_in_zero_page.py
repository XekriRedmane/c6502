"""Returns a `static` function's pointer result in a zero-page pointer.

A function returning a pointer returns it in A and X
(docs/calling_conventions.md), and a caller that reads through the
pointer has to store it into a zero-page pair first, since the 6502
reads through a pointer only with `(zp),Y`. That's `STA p; STX p+1`
after each call, 4 bytes, and a callee whose high byte isn't
computed last also parks the low byte while it computes the high
one (`STA t; ...; TAX; LDA t; RTS`). Zork's `get_object_addr` leaves
the address in `SCRATCH2`, where its callers read through it.

Under `-O`, a function this unit defines that

- is `static`,
- returns a pointer, and
- whose address isn't taken, so that every call is a direct call
  from this unit,

stores its result in a zero-page pointer of its own, `F@ret`, and
returns nothing, and each call copies `F@ret` into the call's result.
In TAC, `Ret(v)` in `F` becomes `Copy(v, F@ret); Ret()`, and
`FunctionCall(F, args, d)` becomes `FunctionCall(F, args); Copy(F@ret,
d)`. Copy propagation then reads `F@ret` in place of `d` wherever
nothing between the call and the read may write `F@ret`, which is
any call to a function that may call `F`, so `p = F(); x = p[4];` is
`JSR F; LDY #$04; LDA (F@ret),Y`. Where the result is needed after
such a call, the copy stays, 8 bytes where `STA` / `STX` was 4.

`F@ret` is a `[[c6502::zeropage]]` object, so `passes/zp_globals.py`
gives it 2 bytes below `$80` and the linker keeps it in zero page. Its
`StaticVariable` has no initial bytes at all, so neither the unit's
`__zp_init` nor the linker's stores anything into it, since nothing
reads it before a call to `F` has written it.

The rewrite runs on the unoptimized TAC, before
`zp_globals.plan_zero_page`, which has to see the new objects. Its
callers are `compile.translate_for_codegen`, which the CLI and the
simulator harness both use."""

from __future__ import annotations

import c99_ast
import tac_ast
from passes.abi_selection import _address_taken
from passes.type_checking import FunAttr, StaticAttr, Symbol, Tentative


def return_slot_name(fn: str) -> str:
    """The zero-page pointer `fn` returns its result in."""
    return f"{fn}@ret"


def return_pointers_in_zero_page(
    tac: tac_ast.Program, symbols: dict,
) -> tac_ast.Program:
    """Rewrites each qualifying function and every call to it (see the
    module docstring), adds a `StaticVariable` and a symbol for each
    one's `F@ret`, and returns the new program. A program with no
    qualifying function comes back unchanged."""
    taken = _address_taken(tac)
    chosen: dict[str, c99_ast.Type_data_type] = {}
    for tl in tac.top_level:
        if not isinstance(tl, tac_ast.Function) or tl.is_global:
            continue
        if tl.name in taken:
            continue
        sym = symbols.get(tl.name)
        if sym is None or not isinstance(sym.attrs, FunAttr):
            continue
        ret = sym.type.ret
        if isinstance(ret, c99_ast.Pointer):
            chosen[tl.name] = ret
    if not chosen:
        return tac
    top: list[tac_ast.Type_top_level] = []
    for tl in tac.top_level:
        if isinstance(tl, tac_ast.Function):
            top.append(_rewrite(tl, chosen))
        else:
            top.append(tl)
    for fn, ret in chosen.items():
        name = return_slot_name(fn)
        symbols[name] = Symbol(
            type=ret,
            attrs=StaticAttr(
                initial_value=Tentative(), is_global=False, zeropage=True,
            ),
        )
        top.append(tac_ast.StaticVariable(
            name=name, is_global=False, data_type=tac_ast.Pointer(),
            init=[],
        ))
    return tac_ast.Program(top_level=top)


def _rewrite(
    fn: tac_ast.Function, chosen: dict[str, c99_ast.Type_data_type],
) -> tac_ast.Function:
    out: list[tac_ast.Type_instruction] = []
    own = return_slot_name(fn.name) if fn.name in chosen else None
    for instr in fn.instructions:
        if (own is not None and isinstance(instr, tac_ast.Ret)
                and instr.val is not None):
            out.append(tac_ast.Copy(src=instr.val, dst=tac_ast.Var(name=own)))
            out.append(tac_ast.Ret(val=None))
            continue
        if (isinstance(instr, tac_ast.FunctionCall)
                and instr.name in chosen and instr.dst is not None):
            out.append(tac_ast.FunctionCall(
                name=instr.name, args=list(instr.args), dst=None,
                hw_stack_save=instr.hw_stack_save,
                noreturn=instr.noreturn,
            ))
            out.append(tac_ast.Copy(
                src=tac_ast.Var(name=return_slot_name(instr.name)),
                dst=instr.dst,
            ))
            continue
        out.append(instr)
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=out,
    )
