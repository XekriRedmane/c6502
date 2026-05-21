"""Promote `mem[X] + 1; CMP n` to `LDY mem,X; INY; CPY n` when Y
is free and the only consumer is the compare.

# Pattern

Five consecutive instructions:

    [i]   Mov(IndexedData(name, index=X), Reg(A))   ; LDA m,X
    [i+1] Mov(Reg(A), <Data|ZP>)                    ; STA tmp
    [i+2] Inc(<same Data|ZP>)                       ; INC tmp
    [i+3] Mov(<same Data|ZP>, Reg(A))               ; LDA tmp
    [i+4] Compare(Reg(A), <Imm|Data|ZP>)            ; CMP other

Rewrite to three instructions:

    [i]   Mov(IndexedData(name, index=X), Reg(Y))   ; LDY m,X
    [i+1] Inc(Reg(Y))                                ; INY
    [i+2] Compare(Reg(Y), <Imm|Data|ZP>)            ; CPY other

Same shape for the `-1` form (`Dec` in atom [i+2] → `Dec(Reg(Y))`
= DEY). The X-indexed-source / Y-result direction is the only
sound combination: `LDY abs,X` exists, `LDY abs,Y` does NOT (the
6502 lacks an `LDY` mode that reads via Y), so we can't mirror
the rewrite for Y-indexed sources into X-pivoted results either.

# Soundness

Original net effect (5 atoms):
  * A := mem[X] (then) + 1
  * tmp := mem[X] + 1
  * flags reflect (mem[X] + 1) CMP other
  * X unchanged, Y unchanged

Rewrite net effect (3 atoms):
  * Y := mem[X] + 1
  * flags reflect (mem[X] + 1) CPY other — same flag state, since
    CPY n sets N/Z/C identically to CMP n when comparing the same
    two byte values (Y == A in this scenario).
  * X unchanged, A unchanged, tmp unchanged.

Differences to gate:
  1. A's value differs (rewrite leaves A unchanged; original sets
     A := mem[X] + 1). Sound iff A is dead after [i+4].
  2. Y's value differs (rewrite sets Y := mem[X] + 1; original
     leaves Y unchanged). Sound iff Y's pre-pattern value is dead.
  3. tmp's value differs (rewrite leaves tmp unchanged; original
     sets tmp := mem[X] + 1). Sound iff tmp is dead at [i+5] —
     no reachable read of tmp before the next write that doesn't
     also kill the read.

The flag effect is preserved: both `CMP A, n` and `CPY Y, n` set
N/Z/C from the subtraction `(A or Y) - n`, which produces the
same flags because A == Y at the comparison point in the
original (post-LDA) and the rewrite (post-INY).

# Where this hits

The C expression `target = mem[idx] + 1; if (target == other) {
... }` lowers (post-inc_peephole) into the 5-atom shape. The
regalloc colors `target`'s storage to a ZP slot
(`__local_<fn>__<N>`) because at allocation time Y isn't a
candidate color — Y's only roles in the IR are indirect-Y
addressing and CPY operands; it's not tracked as a general-
purpose byte register. This peephole observes the post-coloring
ZP-routed shape and substitutes the Y-register lowering when
the gates pass.

Headline case: the chase-branch target computation in
`beam_target_tick` — `floor_ceil[floor_idx] + 1` compared to
`beam_y`.

# Where to run

Inside the asm-peephole fixedpoint, after `replace_pseudoregisters`
(operands concrete), after `apply_inc_peephole` (the +1 must have
been collapsed to `Inc(tmp)`), and after the `STA;LDA` round-trip
collapsers can't apply (they can't here — `STA tmp; INC tmp; LDA
tmp` isn't a simple round-trip because the INC mutates tmp in
between). Shrinks 5 atoms to 3 (saves 4 bytes / 6 cycles per
occurrence), monotone-shrinking, fine in the fixedpoint.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at, y_dead_at


_CPY_ADDRESSABLE = (asm_ast.Imm, asm_ast.Data, asm_ast.ZP)


def apply_index_inc_cmp_y(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    changed = False
    while i < len(instrs):
        match = _match_pattern(instrs, i)
        if match is not None:
            src, is_dec, other = match
            out.append(asm_ast.Mov(
                src=src,
                dst=asm_ast.Reg(reg=asm_ast.Y()),
                is_volatile=False,
            ))
            if is_dec:
                out.append(asm_ast.Dec(dst=asm_ast.Reg(reg=asm_ast.Y())))
            else:
                out.append(asm_ast.Inc(dst=asm_ast.Reg(reg=asm_ast.Y())))
            out.append(asm_ast.Compare(
                left=asm_ast.Reg(reg=asm_ast.Y()),
                right=other,
            ))
            i += 5
            changed = True
            continue
        out.append(instrs[i])
        i += 1
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _match_pattern(
    instrs: list[asm_ast.Type_instruction], i: int,
):
    if i + 4 >= len(instrs):
        return None
    a, b, c, d, e = (
        instrs[i], instrs[i + 1], instrs[i + 2],
        instrs[i + 3], instrs[i + 4],
    )
    # a: Mov(IndexedData(_, index=X), Reg(A)) — LDA m,X.
    if not (isinstance(a, asm_ast.Mov) and not a.is_volatile):
        return None
    if not (
        isinstance(a.src, asm_ast.IndexedData)
        and isinstance(a.src.index, asm_ast.X)
        and isinstance(a.dst, asm_ast.Reg)
        and isinstance(a.dst.reg, asm_ast.A)
    ):
        return None
    indexed_src = a.src
    # b: Mov(Reg(A), <Data|ZP>) — STA tmp.
    if not (isinstance(b, asm_ast.Mov) and not b.is_volatile):
        return None
    if not (
        isinstance(b.src, asm_ast.Reg)
        and isinstance(b.src.reg, asm_ast.A)
        and isinstance(b.dst, (asm_ast.Data, asm_ast.ZP))
    ):
        return None
    tmp = b.dst
    # c: Inc(tmp) or Dec(tmp).
    if not isinstance(c, (asm_ast.Inc, asm_ast.Dec)):
        return None
    if c.dst != tmp:
        return None
    is_dec = isinstance(c, asm_ast.Dec)
    # d: Mov(tmp, Reg(A)) — LDA tmp.
    if not (isinstance(d, asm_ast.Mov) and not d.is_volatile):
        return None
    if not (
        d.src == tmp
        and isinstance(d.dst, asm_ast.Reg)
        and isinstance(d.dst.reg, asm_ast.A)
    ):
        return None
    # e: Compare(Reg(A), <Imm|Data|ZP>) — CMP other.
    if not isinstance(e, asm_ast.Compare):
        return None
    if not (
        isinstance(e.left, asm_ast.Reg)
        and isinstance(e.left.reg, asm_ast.A)
    ):
        return None
    if not isinstance(e.right, _CPY_ADDRESSABLE):
        return None
    # Soundness gates.
    after = i + 5
    if not a_dead_at(instrs, after):
        return None
    if not y_dead_at(instrs, i):
        return None
    if not _operand_unused_after(instrs, after, tmp):
        return None
    return indexed_src, is_dec, e.right


def _operand_unused_after(
    instrs: list[asm_ast.Type_instruction],
    start: int,
    operand: asm_ast.Type_operand,
) -> bool:
    """True iff `operand` does not appear in any operand position
    of any instruction in `instrs[start:]`. Conservative — a write
    to `operand` would also count as "appears", but if the operand
    isn't read before such a write, it's dead by definition. The
    "appears at all" check catches both cases safely (skips
    rewrite if the operand is touched at all later)."""
    for instr in instrs[start:]:
        for op in _all_operands(instr):
            if op == operand:
                return False
    return True


def _all_operands(instr: asm_ast.Type_instruction):
    """Yield every operand position of `instr` (read or write).
    Covers the asm_ast atom set used downstream of
    replace_pseudoregisters. Atoms not yielding operands (Label,
    Branch, Jump, Ret, Return, FunctionPrologue, AllocateStack,
    ClearCarry, SetCarry, Call, Phi) are skipped — their
    contribution is structural, not operand-based."""
    if isinstance(instr, asm_ast.Mov):
        yield instr.src
        yield instr.dst
        return
    if isinstance(instr, (asm_ast.Add, asm_ast.Sub)):
        yield instr.src
        yield instr.dst
        return
    if isinstance(instr, (asm_ast.And, asm_ast.Or)):
        yield instr.src
        yield instr.dst
        return
    if isinstance(instr, asm_ast.Xor):
        yield instr.src1
        yield instr.src2
        yield instr.dst
        return
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        yield instr.dst
        return
    if isinstance(instr, (
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        yield instr.dst
        return
    if isinstance(instr, asm_ast.Compare):
        yield instr.left
        yield instr.right
        return
    if isinstance(instr, asm_ast.Push):
        yield instr.src
        return
    if isinstance(instr, asm_ast.Pop):
        yield instr.dst
        return
    if isinstance(instr, asm_ast.LoadAddress):
        yield instr.src
        yield instr.dst
        return
    if isinstance(instr, asm_ast.BitTest):
        yield instr.operand
        return
