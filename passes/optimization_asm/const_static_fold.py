"""Fold const-qualified internal-linkage scalar statics into immediates.

A `static T const x = <const-init>;` (or, more usefully in c6502,
`static T * const p = <const-init>;`) at file scope is genuinely
immutable in a single-TU program — `static` keeps the symbol from
escaping, and `const` rejects writes to it through the type system.
If its initializer is a single scalar constant and its address
isn't taken anywhere in the program, every reference to its bytes
can be replaced with the corresponding immediate at compile time —
turning `LDA name` (3 bytes) into `LDA #imm` (2 bytes), and
freeing the storage cells of `name` itself.

Concretely the pass walks the program and tags as a candidate any
`StaticVariable` whose:
  * `is_global` is False (internal linkage — `static` at file scope
    or any block-scope `static`),
  * symbol-table type is `Const(...)` (qualifier is at the top
    level — qualifying a pointer's pointee, e.g. `const int *p`,
    isn't us; that would be `Pointer(Const(Int))` and we wouldn't
    fold the pointer's bytes anyway),
  * `init` is a single CharInit / IntInit / LongInit / LongLongInit
    / FloatInit / DoubleInit (one foldable scalar — array initializers
    and AddressInit are out of scope).

Then it disqualifies any candidate whose:
  * address appears in another static's initializer
    (`AddressInit(name=candidate, ...)`),
  * is the source of a `LoadAddress` (`&candidate`),
  * is the destination of any write instruction (defensive — a
    const lvalue is rejected at type-check, but if a write somehow
    reached this stage we wouldn't want to silently drop it),
  * appears as an `IndexedData(name=candidate, ...)` operand.

Surviving candidates are the names whose bytes can be folded. The
pass walks every function and replaces every `Pseudo(name=cand,
offset=k)` and `Data(name=cand, offset=k)` USE operand with
`Imm(byte_at(init, k))`. A `Data` read is a load through the
object's address that the TAC optimizer made a direct read, as `*pk`
after `pk = &K`. Defs and LoadAddress.src/dst aren't rewritten (the
disqualification rules above guarantee neither shape names a
candidate). Finally the pass drops the `StaticVariable` top-levels
that nothing names any more. Before `Data` reads were rewritten, the
storage went while `LDA K` still read it, and the link failed.

Runs as a program-level prepass inside `optimize_program`, BEFORE
the per-function SSA round-trip — replacing the Pseudo references
with Imms early lets the existing forward-copy-prop / DCE bracket
clean up any redundant `Mov(Imm, Pseudo)` staging the fold leaves
behind.
"""
from __future__ import annotations

import dataclasses
from typing import Iterable

import asm_ast
from passes import decimal_arith
import c99_ast


def fold_const_statics(
    prog: asm_ast.Type_program, *,
    symbols,
) -> asm_ast.Type_program:
    """Replace references to const-qualified internal-linkage scalar
    statics with `Imm` operands carrying the static's byte values,
    and drop the now-unreferenced `StaticVariable` top-levels.

    `symbols` is the c6502 symbol table — used to check the const
    qualifier and storage class. If `symbols` is None (legacy /
    test path), the pass returns `prog` unchanged.

    Internal-linkage (`is_global=False`) is the soundness check
    that the static can't be referenced from another TU: c99's
    `static` keyword keeps the symbol invisible at link time, so
    folding away every reference here lets us drop the storage
    entirely. Externally-linked statics — even if const — are
    skipped because another TU might read them by name."""
    if symbols is None:
        return prog
    candidates = _collect_candidates(prog, symbols)
    if not candidates:
        return prog
    disqualified = _scan_disqualifying(prog, candidates)
    surviving = {
        name: byts for name, byts in candidates.items()
        if name not in disqualified
    }
    if not surviving:
        return prog
    return _rewrite_program(prog, surviving)


# ---------------------------------------------------------------------------
# Candidate collection
# ---------------------------------------------------------------------------


def _collect_candidates(
    prog: asm_ast.Type_program,
    symbols,
) -> dict[str, list]:
    """Walk `StaticVariable` top-levels and pick the ones whose
    type is const-qualified, internal-linkage, and whose init is a
    single foldable scalar. Returns `{name: little-endian bytes}`."""
    candidates: dict[str, list] = {}
    for tl in prog.top_level:
        if not isinstance(tl, asm_ast.StaticVariable):
            continue
        if tl.is_global:
            continue
        sym = symbols.get(tl.name) if hasattr(symbols, "get") else None
        if sym is None:
            continue
        if not _is_const_qualified(sym.type):
            continue
        byts = _scalar_init_bytes(tl.init)
        if byts is None:
            continue
        candidates[tl.name] = byts
    return candidates


def _is_const_qualified(t) -> bool:
    """True iff the type is const-qualified AND not volatile. The
    outermost wrapper may be `Const(...)` or `Volatile(Const(...))`
    (the parser canonicalizes `const volatile T` to
    `Const(Volatile(T))`, but defensively we accept the other
    order); however a top-level Volatile rejects folding even when
    nested Const is present — per C99 §6.7.3.6 each volatile access
    is a side effect, and replacing reads with an immediate would
    erase those.

    We don't recurse past the qualifier layer — `Pointer(Const(Int))`
    is `int * const`-pointee, not a const pointer itself; folding
    the pointer's bytes would be unsound."""
    has_const = False
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        if isinstance(t, c99_ast.Volatile):
            return False
        has_const = True
        t = t.referenced_type
    return has_const


def _scalar_init_bytes(
    init: list[asm_ast.Type_static_init],
) -> list | None:
    """Return the little-endian byte representation of a single
    foldable scalar `static_init` list, or None if not foldable.
    Foldable shapes:
      * CharInit(v)        → [v & 0xFF]
      * IntInit(v)         → 2 bytes
      * LongInit(v)        → 4 bytes
      * LongLongInit(v)    → 8 bytes
      * FloatInit(bits)    → 4 bytes (IEEE 754 single bit pattern)
      * DoubleInit(bits)   → 8 bytes (IEEE 754 double bit pattern)
      * AddressInit(name, k) → the address's halves, `#<(name+k)`
        and `#>(name+k)`, so `static const uint8_t *const S_X = MSG +
        39;` reads as the address in place of two loads from S_X
    Multi-element inits (arrays / structs), StringInit, ZeroInit are
    skipped — single-element scalar only. The bytes are ints, and a
    pointer's are the label operands."""
    if len(init) != 1:
        return None
    item = init[0]
    if isinstance(item, asm_ast.AddressInit):
        return [
            asm_ast.ImmLabelLow(name=item.name, offset=item.offset),
            asm_ast.ImmLabelHigh(name=item.name, offset=item.offset),
        ]
    if isinstance(item, asm_ast.CharInit):
        return [item.value & 0xFF]
    if isinstance(item, asm_ast.IntInit):
        return [(item.value >> (8 * k)) & 0xFF for k in range(2)]
    if isinstance(item, asm_ast.LongInit):
        return [(item.value >> (8 * k)) & 0xFF for k in range(4)]
    if isinstance(item, asm_ast.LongLongInit):
        return [(item.value >> (8 * k)) & 0xFF for k in range(8)]
    if isinstance(item, asm_ast.FloatInit):
        return [(item.bits >> (8 * k)) & 0xFF for k in range(4)]
    if isinstance(item, asm_ast.DoubleInit):
        return [(item.bits >> (8 * k)) & 0xFF for k in range(8)]
    return None


# ---------------------------------------------------------------------------
# Disqualification scan
# ---------------------------------------------------------------------------


def _scan_disqualifying(
    prog: asm_ast.Type_program,
    candidates: dict[str, list],
) -> set[str]:
    """Walk the program; mark any candidate whose address is taken
    or that's written to / used as an indexed-load base."""
    disqualified: set[str] = set()
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.StaticVariable):
            for item in tl.init:
                if (
                    isinstance(item, (
                        asm_ast.AddressInit, asm_ast.AddressByteInit,
                    ))
                    and item.name in candidates
                ):
                    disqualified.add(item.name)
        elif isinstance(tl, asm_ast.Function):
            for instr in tl.instructions:
                _scan_instr(instr, candidates, disqualified)
    return disqualified


def _scan_instr(
    instr: asm_ast.Type_instruction,
    candidates: dict[str, list],
    disqualified: set[str],
) -> None:
    """Mark candidates appearing in disqualifying contexts in this
    instruction. Read-only USES (Mov src, Add src, Compare, etc.)
    are foldable; everything else is disqualifying."""
    # LoadAddress.src naming a candidate = address-taken (Frame-src
    # case; static-src LoadAddress was lowered to ImmLabel* by
    # tac_to_asm and is caught by the operand-walk below).
    if isinstance(instr, asm_ast.LoadAddress):
        if (
            isinstance(instr.src, asm_ast.Pseudo)
            and instr.src.name in candidates
        ):
            disqualified.add(instr.src.name)
        # LoadAddress.dst is always a 2-byte temp; static names
        # don't reach there.
        return
    if isinstance(instr, asm_ast.DecimalArith):
        for op in decimal_arith.writes(instr):
            if isinstance(op, asm_ast.Pseudo) and op.name in candidates:
                disqualified.add(op.name)
    # Write destinations of arithmetic / RMW atoms.
    write_dst = _write_destination(instr)
    if (
        write_dst is not None
        and isinstance(write_dst, asm_ast.Pseudo)
        and write_dst.name in candidates
    ):
        disqualified.add(write_dst.name)
    # Indexed-data references — only meaningful for arrays, but
    # defensive: a candidate appearing here means somebody is
    # indexing through it, which we can't fold.
    # ImmLabelLow / ImmLabelHigh references — `&candidate` after
    # tac_to_asm's static-LoadAddress lowering; the link-time
    # address is needed, so the storage must survive.
    for op in _operand_uses(instr):
        if (
            isinstance(op, asm_ast.IndexedData)
            and op.name in candidates
        ):
            disqualified.add(op.name)
        if (
            isinstance(op, (asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh))
            and op.name in candidates
        ):
            disqualified.add(op.name)


def _write_destination(
    instr: asm_ast.Type_instruction,
) -> asm_ast.Type_operand | None:
    """The operand that this instruction WRITES (not read-modify-
    writes' read side — we want the byte that ends up updated).
    Returns None for instructions that don't write a memory operand
    or whose dst is a register."""
    match instr:
        case asm_ast.Mov(dst=dst):
            return dst
        case (
            asm_ast.Add(dst=dst)
            | asm_ast.Sub(dst=dst)
            | asm_ast.And(dst=dst)
            | asm_ast.Or(dst=dst)
        ):
            return dst
        case asm_ast.Xor(dst=dst):
            return dst
        case (
            asm_ast.Inc(dst=dst)
            | asm_ast.Dec(dst=dst)
            | asm_ast.ArithmeticShiftLeft(dst=dst)
            | asm_ast.LogicalShiftRight(dst=dst)
            | asm_ast.RotateLeft(dst=dst)
            | asm_ast.RotateRight(dst=dst)
        ):
            return dst
        case asm_ast.Pop(dst=dst):
            return dst
    return None


def _operand_uses(
    instr: asm_ast.Type_instruction,
) -> Iterable[asm_ast.Type_operand]:
    """Yield every operand that `instr` reads. Defs aren't
    yielded (use `_write_destination` for that). Used by the
    disqualification scan to find IndexedData references."""
    match instr:
        case asm_ast.DecimalArith():
            yield from decimal_arith.reads(instr)
        case asm_ast.Mov(src=src):
            yield src
        case asm_ast.Add(src=src) | asm_ast.Sub(src=src):
            yield src
        case asm_ast.And(src=src) | asm_ast.Or(src=src):
            yield src
        case asm_ast.Xor(src1=s1, src2=s2):
            yield s1
            yield s2
        case asm_ast.Push(src=src):
            yield src
        case asm_ast.Compare(left=left, right=right):
            yield left
            yield right


# ---------------------------------------------------------------------------
# Rewrite phase
# ---------------------------------------------------------------------------


def _rewrite_program(
    prog: asm_ast.Type_program,
    surviving: dict[str, list],
) -> asm_ast.Type_program:
    """Drop surviving statics; rewrite Pseudo and Data refs to them
    as Imm operands in every function. A static that something still
    names after the rewrite, in an instruction `_rewrite_instr`
    doesn't rewrite, keeps its storage."""
    rewritten = [
        _rewrite_function(tl, surviving)
        if isinstance(tl, asm_ast.Function) else tl
        for tl in prog.top_level
    ]
    named: set[str] = set()
    for tl in rewritten:
        if isinstance(tl, asm_ast.Function):
            _names_in(tl.instructions, named)
    return asm_ast.Program(top_level=[
        tl for tl in rewritten
        if not (isinstance(tl, asm_ast.StaticVariable)
                and tl.name in surviving and tl.name not in named)
    ])


_NAMED_OPERANDS = (
    asm_ast.Pseudo, asm_ast.Data, asm_ast.IndexedData,
    asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh,
)


def _names_in(node, out: set[str]) -> None:
    """Adds the name of every operand in `node` that names an object
    (`Pseudo`, `Data`, `IndexedData`, and a label's halves) to
    `out`."""
    if isinstance(node, _NAMED_OPERANDS):
        out.add(node.name)
    elif isinstance(node, list):
        for n in node:
            _names_in(n, out)
    elif dataclasses.is_dataclass(node):
        for f in dataclasses.fields(node):
            _names_in(getattr(node, f.name), out)


def _rewrite_function(
    fn: asm_ast.Function,
    surviving: dict[str, list],
) -> asm_ast.Function:
    """Replace every `Pseudo(name=cand, offset=k)` USE in `fn` with
    `Imm(bytes[k])`. Defs aren't rewritten — disqualification has
    already excluded any candidate that appears as a def. The
    `LoadAddress.src` slot also isn't rewritten (same reason)."""
    new_instrs = [_rewrite_instr(i, surviving) for i in fn.instructions]
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=fn.params, instructions=new_instrs,
    )


def _rewrite_instr(
    instr: asm_ast.Type_instruction,
    surviving: dict[str, list],
) -> asm_ast.Type_instruction:
    """Substitute every USE-position Pseudo whose name is a
    surviving candidate with an `Imm` carrying the corresponding
    byte. DEF-position operands are left alone."""

    def sub(op: asm_ast.Type_operand,
            labels: bool = True) -> asm_ast.Type_operand:
        # A `Data` read is one the TAC optimizer made of a load
        # through the static's address, as `*p` after `p = &K`
        # becomes `IndexedLoad(K, 0)`, which tac_to_asm lowers to
        # `Data(K, 0)`.
        if (
            isinstance(op, (asm_ast.Pseudo, asm_ast.Data))
            and op.name in surviving
            and 0 <= op.offset < len(surviving[op.name])
        ):
            byte = surviving[op.name][op.offset]
            if isinstance(byte, int):
                return asm_ast.Imm(value=byte)
            # A label's half is an immediate only where `asm_emit`
            # writes one (`LDA`, `ADC`, `SBC`, `CMP`). Elsewhere the
            # read stays, and so does the storage it reads.
            return byte if labels else op
        return op

    match instr:
        case asm_ast.DecimalArith():
            return decimal_arith.map_reads(
                instr, lambda op: sub(op, labels=False))
        case asm_ast.Mov(src=src, dst=dst, is_volatile=v):
            return asm_ast.Mov(src=sub(src), dst=dst, is_volatile=v)
        case asm_ast.Add(src=src, dst=dst):
            return asm_ast.Add(src=sub(src), dst=dst)
        case asm_ast.Sub(src=src, dst=dst):
            return asm_ast.Sub(src=sub(src), dst=dst)
        case asm_ast.And(src=src, dst=dst):
            return asm_ast.And(src=sub(src, labels=False), dst=dst)
        case asm_ast.Or(src=src, dst=dst):
            return asm_ast.Or(src=sub(src, labels=False), dst=dst)
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            return asm_ast.Xor(src1=sub(s1, labels=False),
                               src2=sub(s2, labels=False), dst=dst)
        case asm_ast.Push(src=src):
            return asm_ast.Push(src=sub(src, labels=False))
        case asm_ast.Compare(left=left, right=right):
            return asm_ast.Compare(
                left=sub(left), right=sub(right),
            )
    return instr
