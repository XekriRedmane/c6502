"""Per-function ZP local-byte sizing.

Counts how many distinct zero-page byte addresses each function's
body locals occupy after the asm-level regalloc has run. The
input is the asm program returned by
`passes.optimization_asm.optimizer.optimize_program` — at that
point, every Pseudo the regalloc colored has been rewritten by
`apply_coloring` to a concrete `ZP(addr, 0)` operand, and the
remaining Pseudos (params, address-taken, spilled, HwReg-pinned)
have NOT yet been resolved by
`replace_pseudoregisters_bare_exit`. The byte footprint we count
is therefore exactly the asm regalloc's body-local coloring —
the right number to size each function's "private range" in the
upcoming call-graph local-slot allocator.

Specifically excluded from the count:

  * `Pseudo(...)` operands — these are unresolved params,
    address-taken locals, or spilled body locals. They get
    Frame slots (RAM, not ZP) later in
    `replace_pseudoregisters_bare_exit`, so they don't compete
    with body locals for ZP.
  * `Data(name, off)` operands — these are static-storage
    references (user statics, runtime symbols like `HARGS` /
    `DPTR`, and the `__zpabi_*` param slot symbols). Statics
    live at their own link-time addresses; runtime symbols and
    `__zpabi_*` slots are sized by other passes. None of them
    are part of "body local" sizing.
  * `Reg(...)` — A / X / Y are 6502 registers, not ZP bytes.
  * `IndexedData` — its base is link-time-resolved (a static
    name), not a ZP byte we manage here.
  * `Frame` / `Stack` / `IndirectY` — RAM-frame addressing, not
    ZP.
  * `Imm` / `ImmLabelLow` / `ImmLabelHigh` — immediates, no
    storage.

What we DO count: every `ZP(addr, offset)` operand. After the
asm regalloc + apply_coloring round-trip, the only `ZP`
operands in the body come from the regalloc's coloring of
Pseudo body locals, so collecting their `addr + offset` values
yields the exact set of bytes each function uses.

The count is used by `passes.zp_slot_allocation` (step 2 of the
call-graph local-slot extension) to know how big each
function's private range needs to be.
"""
from __future__ import annotations

import asm_ast


def compute_local_bytes(
    prog: asm_ast.Program,
) -> dict[str, int]:
    """Return `{function_name: byte_count}` for every `Function`
    top-level in `prog`. `byte_count` is the number of distinct
    ZP byte addresses the function's body uses for regalloc-
    colored locals; non-function top-levels are skipped, and
    functions whose body has no ZP locals get a count of 0.

    Run on `prog` AFTER `optimize_program` and BEFORE
    `replace_pseudoregisters_bare_exit`."""
    out: dict[str, int] = {}
    for tl in prog.top_level:
        if not isinstance(tl, asm_ast.Function):
            continue
        out[tl.name] = len(_zp_bytes_used(tl))
    return out


def compute_local_byte_addresses(
    prog: asm_ast.Program,
) -> dict[str, frozenset[int]]:
    """Same as `compute_local_bytes` but returns the concrete set
    of byte addresses each function's body uses instead of just
    the count. The slot allocator doesn't need the addresses
    (it picks fresh ranges), but downstream verification /
    debugging passes may want them."""
    out: dict[str, frozenset[int]] = {}
    for tl in prog.top_level:
        if not isinstance(tl, asm_ast.Function):
            continue
        out[tl.name] = frozenset(_zp_bytes_used(tl))
    return out


def _zp_bytes_used(fn: asm_ast.Function) -> set[int]:
    addrs: set[int] = set()
    for instr in fn.instructions:
        for op in _all_operands_in(instr):
            if isinstance(op, asm_ast.ZP):
                addrs.add(op.address + op.offset)
    return addrs


def _all_operands_in(instr: asm_ast.Type_instruction):
    """Yield every operand of `instr`. Mirrors the helpers in
    `optimization_asm.optimizer` / `interference` / `liveness`;
    duplicated here to keep this module self-contained (its only
    `passes` neighbor is `zp_slot_allocation`, which doesn't
    import asm-level helpers)."""
    match instr:
        case asm_ast.Mov(src=s, dst=d):
            yield s; yield d
        case (
            asm_ast.Add(src=s, dst=d)
            | asm_ast.Sub(src=s, dst=d)
            | asm_ast.And(src=s, dst=d)
            | asm_ast.Or(src=s, dst=d)
        ):
            yield s; yield d
        case asm_ast.Xor(src1=s1, src2=s2, dst=d):
            yield s1; yield s2; yield d
        case asm_ast.Inc(dst=d) | asm_ast.Dec(dst=d):
            yield d
        case (
            asm_ast.ArithmeticShiftLeft(dst=d)
            | asm_ast.LogicalShiftRight(dst=d)
            | asm_ast.RotateLeft(dst=d)
            | asm_ast.RotateRight(dst=d)
        ):
            yield d
        case asm_ast.Push(src=s):
            yield s
        case asm_ast.Pop(dst=d):
            yield d
        case asm_ast.Compare(left=l, right=r):
            yield l; yield r
        case asm_ast.LoadAddress(src=s, dst=d):
            yield s; yield d
        case asm_ast.Phi(dst=d, args=args):
            yield d
            for a in args:
                yield a.source
