"""TAC pass: nothing runs after a call to a function declared
`[[noreturn]]`.

# What it does

C23 §6.7.13.7 makes it undefined for a `[[noreturn]]` function to
return, so the instructions after a call to one, up to the next
label, never run. The pass puts a `Ret` right after the call and
drops those instructions. The `Ret` doesn't run either, and is there
because every path through a TAC function ends in one. On the 6502
the call and the `Ret` become `JSR f; RTS`, which `tail_call` turns
into `JMP f`.

Zork's `do_2op` (examples/zork/interpreter/decode.c) checks its
operand count with

    if (left == 0)
        internal_error();

and reads `a` and `b` after. With `internal_error` declared
`[[noreturn]]`, no call comes between the copy of `OPERANDS[0]` into
`a` at the function's entry and the reads of `a`, so the asm's
memory value propagation reads `OPERANDS` there and the copy goes.

# Rules

- A `FunctionCall` whose callee's `FunAttr.noreturn` is set, by any
  of its declarations. A call through a pointer is left alone, since
  the attribute doesn't apply to the pointer's type.
- The call is followed by a `Ret` of nothing, in place of whatever
  followed it, in a function of any return type. Since the `Ret`
  never runs, no value has to be loaded for it, and `JSR f; RTS` is
  left for `tail_call`.

# Where it runs

Before SSA construction, so that the dropped code's definitions never
take part in it. Under `-O` only.
"""
from __future__ import annotations

import tac_ast
from passes.optimization.framework import PreSsaPass
from passes.type_checking import FunAttr


class NoreturnCalls(PreSsaPass):
    name = "noreturn_calls"

    def run(self, fn, ctx):
        return noreturn_calls(fn, ctx.symbols)


def noreturn_calls(fn: tac_ast.Function, symbols) -> tac_ast.Function:
    if symbols is None:
        return fn
    ret = tac_ast.Ret(val=None)
    out: list[tac_ast.Type_instruction] = []
    skipping = False
    for ins in fn.instructions:
        if skipping:
            if not isinstance(ins, tac_ast.Label):
                continue
            skipping = False
        out.append(ins)
        if isinstance(ins, tac_ast.FunctionCall) and _is_noreturn(
            ins.name, symbols,
        ):
            out.append(ret)
            skipping = True
    if out == fn.instructions:
        return fn
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _is_noreturn(name: str, symbols) -> bool:
    sym = symbols.get(name)
    return (sym is not None and isinstance(sym.attrs, FunAttr)
            and sym.attrs.noreturn)
