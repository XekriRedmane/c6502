"""Asm peephole: drop the `STA temp; LDA mem; ADC temp; STA mem`
spill when ADC's commutativity lets us read mem directly.

# Motivating shape

A compound assignment `mem += V` where `V` was just computed into A.
The straightforward lowering spills A:

    Mov(Reg(A), temp)            ; STA temp   — spill V
    [intervening A-preserving, temp-preserving ops]
    Mov(mem, Reg(A))             ; LDA mem
    ClearCarry                   ; CLC (optional, ADC-specific)
    Add(temp, Reg(A))            ; ADC temp   — A = mem + V
    Mov(Reg(A), mem)             ; STA mem

But ADC is commutative: `mem + V == V + mem`. If A still holds V at
the ADC point (no intervening A clobber), we can fold the spill out:

    [intervening ops]
    ClearCarry
    Add(mem, Reg(A))             ; ADC mem    — A = V + mem
    Mov(Reg(A), mem)             ; STA mem

The same shape works for `And` / `Or` — both commutative,
flag-preserving in the same way. Not `Sub` (SBC isn't commutative)
or `Xor` (3-operand IR form, different shape).

# What the rewrite does

  - Drops the `LDA mem` (the load disappears — A already has V).
  - Rewrites the `ADC temp` (or `AND temp` / `ORA temp`) to read
    `mem` instead.
  - Leaves the `STA temp` in place: if `temp` is dead afterward,
    `asm_dead_store` picks it up in the next sweep; if `temp` is
    still live for some other reason, the STA stays sound.

The leftover `STA temp` is therefore not a soundness obligation
of this pass — it's a follow-up DSE job. That keeps the matching
side simple (no cross-block dead-after analysis) while still
shrinking the hot path by two instructions per occurrence.

# Match conditions

Five-position window with a flexible intervening band:

  [i]      `Mov(Reg(A), temp)` where temp ∈ {ZP, Data}, non-volatile.
  [i+1..j-1]  intervening ops, each:
              - is in the allow-list {Mov, Inc, Dec, ClearCarry,
                SetCarry, Compare, BitTest},
              - doesn't write `Reg(A)`,
              - doesn't read or write any byte that aliases `temp`.
  [j]      `Mov(mem, Reg(A))` where mem ∈ {ZP, Data, IndexedData},
           non-volatile.
  [j+1]    optional `ClearCarry` / `SetCarry`.
  [k]      `Add(temp, Reg(A))` / `And(temp, Reg(A))` / `Or(temp,
           Reg(A))` — operand structurally equal to the STA's dst.
  [k+1]    `Mov(Reg(A), mem)` — operand structurally equal to the
           LDA's src, non-volatile.

The intervening band is the key relaxation over a strict-adjacency
peephole — it accommodates the LDX/LDY index-reg setup that
typically sits between an A-spill and the matching mem load. The
band's allow-list is conservative (every kind explicitly named);
any other kind in the band aborts the match.

# Soundness

Let V be A's value entering [i].

Original semantics:
  After [i]:    A = V; temp = V; flags unchanged.
  Intervening:  A = V (preserved by gate); temp = V (no writes).
  After [j]:    A = mem-value; N/Z reflect mem-value.
  After CLC/SEC: A = mem-value; C set.
  After [k]:    A = op(mem-value, V, C); N/Z/C/V reflect result.
  After [k+1]:  mem = A; flags unchanged.

Rewrite semantics:
  After [i]:    A = V; temp = V; flags unchanged.
  Intervening:  A = V; temp = V.
  After (dropped [j]): A = V; flags = whatever intervening left.
  After CLC/SEC: A = V; C set.
  After rewritten [k]: A = op(V, mem-value, C); N/Z/C/V reflect
                         result.
  After [k+1]:  mem = A.

Both produce the same A after [k+1] (op is commutative in V and
mem-value) and the same flag state (op overwrites N/Z/C/V).

No instruction between [j] and [k+1] reads N/Z, so the dropped
[j]'s flag effect isn't observed. C is supplied by an optional
CLC/SEC, not by [j]. The index register used by `mem` (if
IndexedData) is set before [j] and not modified between [j] and
[k+1], so the same byte is targeted by the rewritten ADC mem and
the preserved STA mem.

# Where to run

Inside the asm-peephole fixed-point loop, alongside the other
read-modify-write simplifications. Composes with TAC-level
sinkers that surface the STA-then-CLC-ADC pattern out of a
memory `+= V` shape; on its own, fires anywhere the compiler
emits the spill-reload-add idiom for a single-byte RMW.
"""
from __future__ import annotations

import asm_ast
from passes.asm_aliasing import may_alias


def apply_adc_commute(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every function once; rewrite each matching window in
    a single forward pass. The peephole fixedpoint driver re-runs
    until no further rewrites fire."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = list(fn.instructions)
    n = len(instrs)
    drop: set[int] = set()
    rewrite: dict[int, asm_ast.Type_instruction] = {}
    i = 0
    while i < n:
        if i in drop or i in rewrite:
            i += 1
            continue
        match = _try_match(instrs, i, n)
        if match is None:
            i += 1
            continue
        _, lda_idx, op_idx, sta_mem_idx, mem = match
        drop.add(lda_idx)
        rewrite[op_idx] = _replace_src(instrs[op_idx], mem)
        # Skip past the matched window; the STA mem at the end is
        # preserved and could be a temp-store for a subsequent
        # match, but the structure (STA mem followed by something)
        # would have to come from new emission, not from this same
        # rewrite. Advancing avoids re-matching our own rewrite.
        i = sta_mem_idx + 1
    if not drop and not rewrite:
        return fn
    out: list[asm_ast.Type_instruction] = []
    for k, inst in enumerate(instrs):
        if k in drop:
            continue
        out.append(rewrite.get(k, inst))
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _replace_src(
    op: asm_ast.Type_instruction, new_src: asm_ast.Type_operand,
) -> asm_ast.Type_instruction:
    """Build a fresh op atom with `src` replaced by `new_src`,
    preserving `dst` and the op class."""
    if isinstance(op, asm_ast.Add):
        return asm_ast.Add(src=new_src, dst=op.dst)
    if isinstance(op, asm_ast.And):
        return asm_ast.And(src=new_src, dst=op.dst)
    if isinstance(op, asm_ast.Or):
        return asm_ast.Or(src=new_src, dst=op.dst)
    raise AssertionError(f"unexpected op class: {type(op).__name__}")


def _try_match(
    instrs: list[asm_ast.Type_instruction], i: int, n: int,
) -> tuple[int, int, int, int, asm_ast.Type_operand] | None:
    """Try to match the peephole pattern starting at `instrs[i]`.

    Returns `(sta_idx, lda_idx, op_idx, sta_mem_idx, mem)` on
    success, else None. `sta_idx` equals `i` for now (the spill is
    always at position i); the caller drops the LDA at `lda_idx`
    and rewrites the op at `op_idx`."""
    sta = instrs[i]
    if not _is_sta(sta):
        return None
    temp = sta.dst

    # Walk forward until we either hit a compatible LDA or break
    # the intervening band's invariants.
    j = i + 1
    while j < n:
        cur = instrs[j]
        if _is_lda_compat(cur):
            break
        if not _is_preserving(cur, temp):
            return None
        j += 1
    else:
        return None

    lda = instrs[j]
    mem = lda.src

    # Optional ClearCarry / SetCarry between LDA and the op.
    k = j + 1
    if k >= n:
        return None
    if _is_clc_or_sec(instrs[k]):
        k += 1
        if k >= n:
            return None

    # Commutative op reading the temp into A.
    op = instrs[k]
    if not _is_comm_op_with_temp(op, temp):
        return None

    # The matching STA mem must be immediately next.
    sta_mem_idx = k + 1
    if sta_mem_idx >= n:
        return None
    sta_mem = instrs[sta_mem_idx]
    if not _is_sta_compat(sta_mem):
        return None
    if not _operands_equal(sta_mem.dst, mem):
        return None

    return (i, j, k, sta_mem_idx, mem)


# ---------------------------------------------------------------------------
# Operand / instruction predicates
# ---------------------------------------------------------------------------


def _is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_sta(instr: asm_ast.Type_instruction) -> bool:
    """Non-volatile `Mov(Reg(A), <ZP|Data>)`. Restricting the dst
    shape to ZP/Data (no IndexedData) keeps the spill slot a simple
    fixed byte — the only shape we know how to alias-check
    cleanly."""
    return (
        isinstance(instr, asm_ast.Mov)
        and not instr.is_volatile
        and _is_reg_a(instr.src)
        and isinstance(instr.dst, (asm_ast.ZP, asm_ast.Data))
    )


def _is_lda_compat(instr: asm_ast.Type_instruction) -> bool:
    """Non-volatile `Mov(<ZP|Data|IndexedData>, Reg(A))`. The mem
    side can be IndexedData (`abs,X|Y`) — ADC supports those
    addressing modes, so the rewrite preserves encodability."""
    return (
        isinstance(instr, asm_ast.Mov)
        and not instr.is_volatile
        and _is_reg_a(instr.dst)
        and isinstance(instr.src, (
            asm_ast.ZP, asm_ast.Data, asm_ast.IndexedData,
        ))
    )


def _is_sta_compat(instr: asm_ast.Type_instruction) -> bool:
    """Non-volatile `Mov(Reg(A), <ZP|Data|IndexedData>)` — matching
    side of `_is_lda_compat`. STA supports the same address modes
    as LDA on the 6502."""
    return (
        isinstance(instr, asm_ast.Mov)
        and not instr.is_volatile
        and _is_reg_a(instr.src)
        and isinstance(instr.dst, (
            asm_ast.ZP, asm_ast.Data, asm_ast.IndexedData,
        ))
    )


def _is_clc_or_sec(instr: asm_ast.Type_instruction) -> bool:
    return isinstance(instr, (asm_ast.ClearCarry, asm_ast.SetCarry))


def _is_comm_op_with_temp(
    instr: asm_ast.Type_instruction, temp: asm_ast.Type_operand,
) -> bool:
    """`Add` / `And` / `Or` whose src structurally equals `temp` and
    whose dst is `Reg(A)`."""
    if not isinstance(instr, (asm_ast.Add, asm_ast.And, asm_ast.Or)):
        return False
    if not _is_reg_a(instr.dst):
        return False
    return _operands_equal(instr.src, temp)


def _operands_equal(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    """Structural equality on the operand kinds this peephole
    matches against. Returns False for kind mismatches."""
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    if (
        isinstance(a, asm_ast.IndexedData)
        and isinstance(b, asm_ast.IndexedData)
    ):
        return (
            a.name == b.name
            and a.offset == b.offset
            and type(a.index) is type(b.index)
        )
    return False


# Instruction kinds allowed in the intervening band between STA temp
# and LDA mem. Anything outside this set aborts the match.
#
# Mov: includes LDX / LDY / STX / STY / TXA / TYA / TAX / TAY etc.
#   We further gate via `_writes_a` to reject those that clobber A.
# Inc / Dec: memory-only effects (X/Y forms are RMW on the reg).
#   `_accesses_alias` rejects if they touch `temp`.
# ClearCarry / SetCarry: flag-only effects; never read by anything
#   between the spill and the LDA-we-drop.
# Compare: reads operands, sets flags; never writes A.
# BitTest: reads memory, sets N/V/Z; never writes A.
#
# Deliberately NOT in the set: Push / Pop / Call / FunctionPrologue /
# AllocateStack / Return / Ret / LoadAddress / arithmetic-on-memory
# RMWs that could clobber A through the indirect-Y conduit, plus all
# control flow (Label / Jump / Branch / JumpIf*).
_INTERVENING_OK: tuple[type, ...] = (
    asm_ast.Mov,
    asm_ast.Inc,
    asm_ast.Dec,
    asm_ast.ClearCarry,
    asm_ast.SetCarry,
    asm_ast.Compare,
    asm_ast.BitTest,
)


def _is_preserving(
    instr: asm_ast.Type_instruction, temp: asm_ast.Type_operand,
) -> bool:
    """True iff `instr` is safe in the intervening band: known-safe
    kind, doesn't write Reg(A), doesn't read or write `temp`."""
    if not isinstance(instr, _INTERVENING_OK):
        return False
    if _writes_a(instr):
        return False
    if _accesses_alias(instr, temp):
        return False
    return True


def _writes_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` writes `Reg(A)`. Within the
    `_INTERVENING_OK` set, only `Mov(_, Reg(A))` does."""
    if isinstance(instr, asm_ast.Mov) and _is_reg_a(instr.dst):
        return True
    return False


# Memory-shaped operands. `may_alias` is only defined for these
# kinds; register and immediate operands fall through to the
# conservative-True default if we feed them in, which would
# spuriously reject things like LDX/LDY as intervening.
_MEMORY_KINDS: tuple[type, ...] = (
    asm_ast.ZP,
    asm_ast.Data,
    asm_ast.IndexedData,
    asm_ast.Stack,
    asm_ast.Frame,
    asm_ast.Indirect,
    asm_ast.IndirectY,
    asm_ast.IndirectZp,
    asm_ast.IndirectZpY,
)


def _maybe_aliases(
    op: asm_ast.Type_operand, temp: asm_ast.Type_operand,
) -> bool:
    """`may_alias` guarded by a memory-kind check on `op`. Reg /
    Imm / ImmLabel* operands don't refer to memory and never alias
    `temp`."""
    if not isinstance(op, _MEMORY_KINDS):
        return False
    return may_alias(op, temp)


def _accesses_alias(
    instr: asm_ast.Type_instruction, temp: asm_ast.Type_operand,
) -> bool:
    """True iff any memory-shaped operand of `instr` may alias
    `temp`. Register / immediate operands are skipped — they
    don't touch memory."""
    if isinstance(instr, asm_ast.Mov):
        if _maybe_aliases(instr.src, temp):
            return True
        if _maybe_aliases(instr.dst, temp):
            return True
    elif isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        if _maybe_aliases(instr.dst, temp):
            return True
    elif isinstance(instr, asm_ast.Compare):
        if _maybe_aliases(instr.left, temp):
            return True
        if _maybe_aliases(instr.right, temp):
            return True
    elif isinstance(instr, asm_ast.BitTest):
        if _maybe_aliases(instr.src, temp):
            return True
    return False
