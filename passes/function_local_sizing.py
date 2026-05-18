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


def compute_address_taken_bytes(
    prog: asm_ast.Program, symbols=None, types=None,
) -> dict[str, int]:
    """Return `{function_name: byte_count}` summing the sizes of
    address-taken Pseudo locals per function.

    An address-taken local is a Pseudo whose name appears as
    `LoadAddress.src.name` somewhere in the function body. These
    Pseudos are excluded from SSA renaming and from regalloc
    coloring (because they need a stable, addressable storage
    location), so they don't appear in
    `compute_local_bytes`. To put them in ZP (instead of forcing a
    Frame slot via `replace_pseudoregisters`), the function's
    private local pool needs to be sized to include them, and
    `replace_pseudoregisters_bare_exit` needs to know which Pseudos
    to route into ZP slot symbols.

    Run on the preliminary post-regalloc asm IR (same as
    `compute_local_bytes`). `symbols` / `types` are the type-
    checker tables, used to size each address-taken Pseudo by its
    declared C type. Params and static-storage objects can also
    appear as `LoadAddress.src` but are NOT counted here — they
    have their own (non-private-pool) storage already. We exclude
    them by checking the function's `params` list and the program-
    wide statics set; the caller passes `statics` explicitly so
    this module stays free of symbol-table imports.
    """
    if symbols is None:
        return {tl.name: 0 for tl in prog.top_level
                if isinstance(tl, asm_ast.Function)}
    statics = _statics_set(symbols)
    out: dict[str, int] = {}
    for tl in prog.top_level:
        if not isinstance(tl, asm_ast.Function):
            continue
        names = _address_taken_local_names(tl, statics)
        total = 0
        for name in names:
            total += _size_of_name(name, symbols, types)
        out[tl.name] = total
    return out


def compute_address_taken_local_names(
    prog: asm_ast.Program, symbols=None,
) -> dict[str, list[str]]:
    """Like `compute_address_taken_bytes` but returns the ordered
    list of address-taken local Pseudo names per function (instead
    of the sum of their byte sizes). Used by the replace-pseudos
    stage to know which Pseudos to route into ZP slot symbols.
    """
    statics = _statics_set(symbols) if symbols is not None else frozenset()
    out: dict[str, list[str]] = {}
    for tl in prog.top_level:
        if not isinstance(tl, asm_ast.Function):
            continue
        out[tl.name] = _address_taken_local_names(tl, statics)
    return out


def _statics_set(symbols) -> frozenset[str]:
    from passes.type_checking import StaticAttr
    return frozenset(
        n for n, s in symbols.items()
        if isinstance(s.attrs, StaticAttr)
    )


def _size_of_name(name, symbols, types) -> int:
    from passes.replace_pseudoregisters import size_of_name
    return size_of_name(name, symbols, types)


def _address_taken_local_names(
    fn: asm_ast.Function, statics: frozenset[str],
) -> list[str]:
    """Ordered, de-duplicated list of Pseudo names appearing as
    `LoadAddress.src.name` in `fn`'s body, EXCLUDING param names
    (those have their own Frame storage) and static names. Order
    is first-occurrence order in the instruction stream so the
    downstream slot-symbol minting is stable across compiles."""
    seen: set[str] = set()
    out: list[str] = []
    param_set = set(fn.params)
    for instr in fn.instructions:
        if isinstance(instr, asm_ast.LoadAddress):
            if not isinstance(instr.src, asm_ast.Pseudo):
                continue
            name = instr.src.name
            if name in param_set:
                continue
            if name in statics:
                continue
            if name in seen:
                continue
            seen.add(name)
            out.append(name)
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
