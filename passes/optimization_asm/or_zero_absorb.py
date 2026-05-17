"""Fold `Mov(Imm(0), A); Or(X, A)` → `Mov(X, A)` at the asm-SSA level.

The `(uint16_t)hi << 8 | (uint16_t)lo` byte-construction idiom lowers
to a series of byte-level Mov / Or atoms. The `<<8` lowering writes
`Imm(0)` into the low byte of the shifted value; the OR-with-the-
low-byte's zero-extension reads that `0` back. The combined pattern
is

    Mov(Imm(0), Reg(A))      # LDA #0
    Or(X, Reg(A))            # ORA X

which is semantically equivalent to `Mov(X, Reg(A))` (LDA X — same A
value, same N/Z flag effect). Folding it pre-coalescing exposes
direct copy chains between the operand bytes and the constructed
wide value's bytes, letting the move coalescer merge their colors.

Why not piggy-back on the existing `passes.const_arith_fold`? That
catalog pass runs in the post-coloring peephole loop AND drops
`Or(Imm(0), A)` after any A-writer as a flag-preserving identity.
The drop is sound when emit-level control flow is straight-line
within a basic block, but in the asm-SSA layer the IR still carries
SSA `Mov(Reg(A), Pseudo)` def atoms whose A-side is consumed by a
later `Or(Imm(0), A)` whose flag effect re-establishes A's N/Z for
a downstream `Branch` that joins two paths (e.g. the `!` operator's
materialize-boolean-then-test sequence). Dropping that ORA #0 at
the SSA level loses the flag re-set the join needed. We avoid the
problem by handling only the load-side absorb pattern, which leaves
the `Mov(X, A)` it produces with exactly the same flag effect the
original `LDA #0; ORA X` pair had — safe regardless of CFG joins.
"""

from __future__ import annotations

import asm_ast


def absorb_zero_load(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i + 1 < len(instrs):
        a, b = instrs[i], instrs[i + 1]
        folded = _try_absorb(a, b)
        if folded is not None:
            out.append(folded)
            i += 2
            continue
        out.append(a)
        i += 1
    while i < len(instrs):
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _try_absorb(a, b):
    """`Mov(Imm(0), A); Or(X, A)` → `Mov(X, A)`."""
    if not (isinstance(a, asm_ast.Mov)
            and isinstance(a.src, asm_ast.Imm)
            and a.src.value == 0
            and not a.is_volatile
            and _is_reg_a(a.dst)):
        return None
    if not (isinstance(b, asm_ast.Or)
            and _is_reg_a(b.dst)):
        return None
    # Drop the Mov + Or, replace with a single Mov(X, A) that loads
    # the OR's source directly. Inherit `is_volatile=False` — the
    # original Mov was non-volatile and the Or carries no volatility
    # flag of its own (volatility is a Mov-only attribute).
    return asm_ast.Mov(src=b.src, dst=a.dst, is_volatile=False)


def _is_reg_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)
