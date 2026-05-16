"""Multi-byte INC peephole — collapse `mem += 1` add-carry chains
into `INC + BNE` chains.

`tac_to_asm` lowers `Binary(Add, x, 1, x)` (in-place add-1) on a
2-byte / 4-byte / 8-byte value as a per-byte ADC chain:

    Mov(M[0], A); CLC; Add(Imm(1), A); Mov(A, M[0])
    Mov(M[1], A); Add(Imm(0), A);      Mov(A, M[1])
    Mov(M[2], A); Add(Imm(0), A);      Mov(A, M[2])
    ...

where `M[k]` is the k-th byte of the operand and the carry from
each ADC threads into the next (LDA only sets N/Z, leaves C
intact). For `Data(name, k)` (static-storage operands) and
`ZP(addr+k, 0)` operands (regalloc-assigned locals), the 6502
offers a much shorter encoding: `INC m` is a 2-byte (zp) /
3-byte (abs) read-modify-write that increments the addressed
byte in place and sets Z=1 iff the result wrapped to zero. Using
that, the multi-byte add-1 collapses to:

    INC M[0]
    BNE done
    INC M[1]
    BNE done
    ...
    INC M[N-1]
done:

For 16-bit absolute that's 8 bytes / 9-14 cycles vs 17 bytes / 22
cycles for the ADC chain (per-byte savings grow with width).
1-byte case is even simpler: `INC m` alone.

Eligibility (per byte position):

  * Memory operand is `Data(name, k)` or `ZP(addr, 0)`. Other
    operand kinds (`Frame`, `Stack`, `Indirect`, `IndexedData`)
    use addressing modes INC doesn't support — `(ind),Y` and
    indirect-X aren't INC-able. Skip those.
  * The pattern's per-byte LDA source equals the STA destination
    (in-place RMW). If they differ — common after SSA destruction
    routes through a temp — INC is unsound (it'd write where we
    don't want, or fail to update the temp). Skip those too.

The bytes don't need to be at consecutive memory addresses.
Byte-granular asm SSA + regalloc may place the bytes of one
multi-byte value at non-adjacent ZP slots, but the structural
pattern (CLC-ADC#1 on the first byte, ADC#0 on each continuation
byte with no intervening CLC, every byte in-place RMW'd) is only
emitted by the multi-byte add-1 lowering — so wherever the bytes
live, INC + BNE on the matching addresses preserves semantics.

Soundness re flags. The ADC chain leaves C set per the final ADC
result; the INC chain doesn't touch C. c6502's codegen never reads
C across separate operations (every comparison emits its own LDA
that resets N/Z, and SEC/CLC before each SBC/ADC), so the
difference is invisible to subsequent instructions. The Z flag
left at `done:` is also unreliable in the INC chain (depending on
which path was taken), but the same is true of the ADC chain.

Where to run. After `replace_pseudoregisters` (operands have to
be concrete `Data`/`ZP` so we can decide eligibility) and before
`expand_long_branches` (so the BNEs we introduce participate in
that pass's displacement check). Slots cleanly into both
optimized and unoptimized pipelines — its win comes from
addressing-mode awareness, not from any post-regalloc state.
"""

from __future__ import annotations

import asm_ast


def apply_inc_peephole(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every Function top-level and rewrite multi-byte add-1
    chains into INC chains where eligible. `StaticVariable`s and
    other top-levels pass through unchanged."""
    counter = 0
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_fn, counter = _rewrite_function(tl, counter)
            new_top.append(new_fn)
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(
    fn: asm_ast.Function, counter: int,
) -> tuple[asm_ast.Function, int]:
    """Walk the function's instruction list. At each position try to
    match the multi-byte add-1 pattern; on a match, splice in the
    INC chain and skip past the matched window. Otherwise copy one
    instruction and advance."""
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        match = _try_match(instrs, i)
        if match is None:
            out.append(instrs[i])
            i += 1
            continue
        n_consumed, mem_ops = match
        replacement, counter = _build_inc_chain(mem_ops, counter)
        out.extend(replacement)
        i += n_consumed
    return (
        asm_ast.Function(
            name=fn.name, is_global=fn.is_global,
            params=list(fn.params), instructions=out,
        ),
        counter,
    )


# ---------------------------------------------------------------------------
# Pattern matching
# ---------------------------------------------------------------------------


_REG_A = asm_ast.Reg(reg=asm_ast.A())


def _try_match(
    instrs: list[asm_ast.Type_instruction], start: int,
) -> tuple[int, list[asm_ast.Type_operand]] | None:
    """Match the multi-byte add-1 pattern starting at `instrs[start]`.

    Returns (n_consumed, [byte0_op, byte1_op, ...]) on success, or
    None on failure.

    The first byte uses ADC #1 (`Mov(M, A); CLC; Add(Imm(1), A);
    Mov(A, M)` — 4 instructions), each subsequent byte uses ADC #0
    (`Mov(M, A); Add(Imm(0), A); Mov(A, M)` — 3 instructions, no
    CLC since carry threads). All M operands must be `Data` or
    `ZP`, must be in-place (LDA src equals STA dst), and must be
    consecutive bytes."""
    if start + 4 > len(instrs):
        return None
    head = _try_match_first_byte(instrs, start)
    if head is None:
        return None
    mem_ops = [head]
    consumed = 4
    while True:
        nxt = _try_match_continuation_byte(instrs, start + consumed)
        if nxt is None:
            break
        mem_ops.append(nxt)
        consumed += 3
    return (consumed, mem_ops)


def _try_match_first_byte(
    instrs: list[asm_ast.Type_instruction], start: int,
) -> asm_ast.Type_operand | None:
    """Match the 4-instruction first-byte sequence:
        Mov(M, A); CLC; Add(Imm(1), A); Mov(A, M)
    Return M on success, None on failure."""
    if start + 4 > len(instrs):
        return None
    i0, i1, i2, i3 = instrs[start:start + 4]
    if not (
        isinstance(i0, asm_ast.Mov)
        and not i0.is_volatile
        and _is_inc_eligible_operand(i0.src)
        and i0.dst == _REG_A
    ):
        return None
    if not isinstance(i1, asm_ast.ClearCarry):
        return None
    if not (
        isinstance(i2, asm_ast.Add)
        and i2.src == asm_ast.Imm(value=1)
        and i2.dst == _REG_A
    ):
        return None
    # The store half can't be volatile either — the original 4-insn
    # sequence has one read + one write per byte; folding to `INC M`
    # would yield the same access count but a volatile RMW is its own
    # category (the compiler can't safely combine read+write into
    # one read-modify-write instruction when the cell can change
    # asynchronously). Refuse.
    if not (
        isinstance(i3, asm_ast.Mov)
        and not i3.is_volatile
        and i3.src == _REG_A
        and _operands_equal(i3.dst, i0.src)
    ):
        return None
    return i0.src


def _try_match_continuation_byte(
    instrs: list[asm_ast.Type_instruction], start: int,
) -> asm_ast.Type_operand | None:
    """Match the 3-instruction continuation-byte sequence:
        Mov(M, A); Add(Imm(0), A); Mov(A, M)
    Return M on success, None on failure. The CLC isn't here — the
    carry from the prior ADC threads in directly. M doesn't need to
    be the address immediately after the previous byte: byte-granular
    regalloc places multi-byte values' bytes at independent ZP slots,
    so the bytes can sit anywhere; the structural pattern is what
    identifies them as part of one logical add-1. M still has to be
    INC-eligible and the pattern still has to be in-place. Volatile
    Movs make the pattern non-matching — see `_try_match_first_byte`."""
    if start + 3 > len(instrs):
        return None
    i0, i1, i2 = instrs[start:start + 3]
    if not (
        isinstance(i0, asm_ast.Mov)
        and not i0.is_volatile
        and _is_inc_eligible_operand(i0.src)
        and i0.dst == _REG_A
    ):
        return None
    if not (
        isinstance(i1, asm_ast.Add)
        and i1.src == asm_ast.Imm(value=0)
        and i1.dst == _REG_A
    ):
        return None
    if not (
        isinstance(i2, asm_ast.Mov)
        and not i2.is_volatile
        and i2.src == _REG_A
        and _operands_equal(i2.dst, i0.src)
    ):
        return None
    return i0.src


def _is_inc_eligible_operand(op: asm_ast.Type_operand) -> bool:
    """True iff `op` is a memory operand INC / INX / INY can address
    directly: `Data(name, k)` (absolute), `ZP(addr, 0)` (zero-page),
    or `Reg(X)` / `Reg(Y)` (the index registers themselves). The
    HwReg case enables `Inc(Reg(Y)) → INY` after HwReg coloring
    substitutes a Pseudo y-counter into Reg(Y); the same single-
    byte ADC chain that operates on a ZP byte now operates on an
    HwReg, and the peephole collapses it to INY/INX. Frame / Stack
    / Indirect use indirect-Y, which INC doesn't support. Pseudo
    isn't here because the peephole runs after `replace_pseudo
    registers` (or, for HwReg-pinned values, after `apply_coloring`
    has rewritten Pseudo → Reg)."""
    if isinstance(op, (asm_ast.Data, asm_ast.ZP)):
        return True
    if isinstance(op, asm_ast.Reg):
        return isinstance(op.reg, (asm_ast.X, asm_ast.Y))
    return False


def _operands_equal(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    """True iff `a` and `b` denote the same byte / register."""
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return (
            a.address == b.address
            and a.offset == b.offset
        )
    if isinstance(a, asm_ast.Reg) and isinstance(b, asm_ast.Reg):
        return type(a.reg) is type(b.reg)
    return False


# ---------------------------------------------------------------------------
# Replacement construction
# ---------------------------------------------------------------------------


def _build_inc_chain(
    mem_ops: list[asm_ast.Type_operand], counter: int,
) -> tuple[list[asm_ast.Type_instruction], int]:
    """Build the INC + BNE chain for an N-byte add-1.

    For N=1: one Inc and no branches needed (caller flow continues
    naturally).
    For N>=2: per byte k in [0, N-1) emit `Inc(M[k]); Branch(NE,
    done)`; for the last byte emit just `Inc(M[N-1]); Label(done)`.
    A fresh `.inc_done@<counter>` label is minted per chain — leading
    `.` makes it dasm-local (scoped to the SUBROUTINE), `@<digits>`
    keeps it disjoint from user labels (`.<funcname>@<ident>`) and
    from other translator-minted labels (`.if_end@<N>`, `.cmp_true@
    <N>`, `.lb_skip@<N>`)."""
    if len(mem_ops) == 1:
        return ([asm_ast.Inc(dst=mem_ops[0])], counter)
    done_label = f".inc_done@{counter}"
    counter += 1
    out: list[asm_ast.Type_instruction] = []
    for k, op in enumerate(mem_ops):
        out.append(asm_ast.Inc(dst=op))
        if k < len(mem_ops) - 1:
            out.append(asm_ast.Branch(
                cond=asm_ast.NE(), target=done_label,
            ))
    out.append(asm_ast.Label(name=done_label))
    return (out, counter)
