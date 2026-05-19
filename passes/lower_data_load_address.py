"""Split Data-source LoadAddress into two single-byte ImmLabel Movs.

A `LoadAddress(src=Data(label, off), dst=<mem-op>)` writes the
2-byte address `&(label+off)` into `dst`. The emit lowering
(`asm_emit._emit_load_address`) expands it to four atoms —
`LDA #<(label+off); STA dst.lo; LDA #>(label+off); STA dst.hi` —
but does so opaquely, hidden inside a single IR atom. That
opacity blocks two peepholes that reason byte-by-byte:

  * `apply_memory_value_propagation` treats `LoadAddress` as a
    state-clearing barrier (memory_value_propagation.py: the
    Call/FunctionPrologue/AllocateStack/LoadAddress/Phi opaque
    set), so the `cells[dst.lo] = ImmLabelLowExpr(label)` and
    `cells[dst.hi] = ImmLabelHighExpr(label)` facts that the
    transparent lowering would establish are lost. Round-trip
    reads of the dst cells later in the function survive that
    would otherwise fold to the original immediates.

  * `asm_emit._emit_load_address` projects the byte-1 write onto
    `Data(dst.name, dst.offset+1)` via `_shift_offset`. When the
    byte-1 physical address has its own primary slot symbol
    (typical for numeric-temp pool addresses, where each byte
    of a 2-byte numeric-temp value gets its own counter-named
    slot — `__local_fn__0` / `__local_fn__1`), the emit form
    `dst.name+1` aliases the byte-1 slot symbol's address but
    uses a different IR identity. Name-keyed peepholes can't
    see the aliasing.

This pass rewrites each Data-source LoadAddress to a pair of
ImmLabel Movs. Frame-source LoadAddress (`&<auto-storage local
on the soft stack>`) is left alone — the `FP + off` arithmetic
genuinely requires the compound CLC/LDA/ADC chain.

Runs after `replace_pseudoregisters_bare_exit` (so the src
operand has been resolved to a concrete `Data` / `Frame`) and
before the peephole fixed-point loop (so
`apply_memory_value_propagation` sees the split form).
"""
from __future__ import annotations

import asm_ast


def lower_data_load_address(
    prog: asm_ast.Type_program,
) -> asm_ast.Type_program:
    """Walk every Function top-level and split each Data-source
    LoadAddress into two ImmLabel Movs. Non-Function top-levels
    pass through unchanged."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_lower_in_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _lower_in_function(fn: asm_ast.Function) -> asm_ast.Function:
    new_instrs: list[asm_ast.Type_instruction] = []
    changed = False
    for instr in fn.instructions:
        if (isinstance(instr, asm_ast.LoadAddress)
                and isinstance(instr.src, asm_ast.Data)
                and _is_memory_operand(instr.dst)):
            new_instrs.extend(_split(instr))
            changed = True
        else:
            new_instrs.append(instr)
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=new_instrs,
    )


def _split(
    instr: asm_ast.LoadAddress,
) -> list[asm_ast.Type_instruction]:
    """Two-Mov expansion of `LoadAddress(src=Data, dst=mem-op)`.
    Mirrors the byte order used by `asm_emit._emit_load_address`
    (low byte first, then high) so the cycle profile is
    unchanged."""
    src = instr.src
    assert isinstance(src, asm_ast.Data)
    label_name, label_off = src.name, src.offset
    dst_lo = instr.dst
    dst_hi = _shift_offset(instr.dst, 1)
    return [
        asm_ast.Mov(
            src=asm_ast.ImmLabelLow(
                name=label_name, offset=label_off,
            ),
            dst=dst_lo,
        ),
        asm_ast.Mov(
            src=asm_ast.ImmLabelHigh(
                name=label_name, offset=label_off,
            ),
            dst=dst_hi,
        ),
    ]


def _shift_offset(
    op: asm_ast.Type_operand, k: int,
) -> asm_ast.Type_operand:
    """Mirror of `asm_emit._shift_offset`. Bumps a memory operand's
    byte offset by `k`. Kept local so the dep arrow only points
    `passes/` → `asm_ast`, not `passes/` → `asm_emit`."""
    if isinstance(op, asm_ast.Data):
        return asm_ast.Data(name=op.name, offset=op.offset + k)
    if isinstance(op, asm_ast.ZP):
        return asm_ast.ZP(address=op.address, offset=op.offset + k)
    if isinstance(op, asm_ast.Frame):
        return asm_ast.Frame(offset=op.offset + k)
    if isinstance(op, asm_ast.Stack):
        return asm_ast.Stack(offset=op.offset + k)
    raise TypeError(f"can't shift offset on operand {op!r}")


def _is_memory_operand(op: asm_ast.Type_operand) -> bool:
    """Same predicate `asm_emit._emit_load_address` uses on dst."""
    return isinstance(
        op, (asm_ast.Data, asm_ast.ZP, asm_ast.Frame, asm_ast.Stack),
    )
