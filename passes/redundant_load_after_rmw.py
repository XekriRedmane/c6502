"""Drop a redundant `LDA M` that follows an in-place rmw on M.

After `DEC M`, `INC M`, `ASL M`, `LSR M`, `ROL M`, or `ROR M`, the
N/Z flags are set based on M's new value — exactly what an
immediately-following `LDA M` would set. If `Reg(A)`'s value isn't
consumed before A is next defined, the `LDA M` is purely a
redundant flag re-set, and we drop it.

# Motivating case

After `loop_rotate` rewrites a `for (T x = N; x >= 0; x--)` loop
to test-at-bottom, and `dec_peephole` collapses the post-step's
SBC chain to a `DEC m`, and the asm-level zero-compare special
case in `_jcmp_signed_ordering` lowers the tail test to
`LDA m; B<PL|MI> target`, the resulting sequence is

    DEC m
    LDA m
    BPL .top

This pass drops the LDA, leaving

    DEC m
    BPL .top

— the canonical 6502 idiom for a signed countdown loop.

# Pattern

Two consecutive instructions:

    Inc(dst=M) | Dec(dst=M) |
    ASL(dst=M) | LSR(dst=M) |
    ROL(dst=M) | ROR(dst=M)
    Mov(src=M, dst=Reg(A))

with the same operand `M` in both. M is one of:

  * `Data` (absolute) or `ZP` (zero-page) — `DEC mem; LDA mem`,
    the canonical memory-iv form.
  * `Reg(X)` or `Reg(Y)` — `DEX; TXA` / `DEY; TYA`, the canonical
    register-iv form. Only DEX/DEY/INX/INY exist for registers
    (no shift/rotate on X/Y), so rmw types here are restricted
    to Inc/Dec.

`Reg(A)` as M is excluded — accumulator-mode shifts have `dst=A`,
but a following `LDA M` couldn't have `M = Reg(A)` (the asm
IR's Mov representation doesn't permit `src=dst=A`), so the
combination doesn't arise.

# Soundness

* **N / Z flags.** All six rmw instructions set N/Z based on the
  result they write back to M. `LDA M` reads M and sets N/Z based
  on what it reads. If no instruction between the rmw and the LDA
  modifies M (we require strict adjacency, so by construction no
  intervening op exists), the two flag-set values are identical.
  Any subsequent `Branch` on N or Z observes the same condition.

* **C / V flags.** DEC / INC don't touch C or V — neither does
  LDA. ASL / LSR / ROL / ROR set C from the rotated-out bit and
  leave V alone — also not affected by LDA. So C/V state at the
  branch point is identical with or without the LDA.

* **Reg(A) value.** Dropping the LDA leaves `A` holding whatever
  was in it before. We require A to be dead at the deletion point
  — no read before the next write — so no later instruction can
  observe the difference.

# Where to run

After `replace_pseudoregisters` (M must be a concrete `Data` or
`ZP`; before that operands are still `Pseudo`). After
`dec_peephole` / `inc_peephole` (so the rmw form exists).
Before `expand_long_branches` (the pass shrinks code, never
introduces new branches; ordering is symmetric with the other
late peepholes).
"""

from __future__ import annotations

import asm_ast


def apply_redundant_load_after_rmw(
    prog: asm_ast.Program,
) -> asm_ast.Program:
    """Walk every Function top-level and drop redundant `LDA M`
    instructions after an in-place rmw on the same M. Other top-
    levels (`StaticVariable`) pass through unchanged."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


_RMW_TYPES: tuple[type, ...] = (
    asm_ast.Inc, asm_ast.Dec,
    asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
    asm_ast.RotateLeft, asm_ast.RotateRight,
)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    """Linear scan: at each rmw whose dst is `Data`/`ZP`/`Reg(X|Y)`,
    check if the next instruction is `Mov(M, Reg(A))` with matching
    M, and if `Reg(A)` is dead after the Mov. On a match, copy
    through the rmw and skip the Mov. Otherwise copy one
    instruction and advance."""
    instrs = fn.instructions
    label_idx = _index_labels(instrs)
    out: list[asm_ast.Type_instruction] = []
    i = 0
    n = len(instrs)
    while i < n:
        instr = instrs[i]
        if (
            i + 1 < n
            and _is_eligible_rmw(instr)
            and _is_redundant_lda_after_rmw(instr, instrs[i + 1])
            and _a_dead_after(instrs, i + 2, label_idx)
        ):
            out.append(instr)
            i += 2
            continue
        out.append(instr)
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _index_labels(
    instrs: list[asm_ast.Type_instruction],
) -> dict[str, int]:
    """Map `Label.name` to its index for fast branch-target
    resolution."""
    out: dict[str, int] = {}
    for i, instr in enumerate(instrs):
        if isinstance(instr, asm_ast.Label):
            out[instr.name] = i
    return out


def _is_eligible_rmw(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` is an in-place rmw whose dst is one of:

      * `Data` (absolute memory),
      * `ZP` (zero-page memory),
      * `Reg(X)` or `Reg(Y)` (DEX/DEY/INX/INY only — no shift /
        rotate on X/Y).

    `Frame` / `Stack` / `Indirect` use indirect-Y addressing,
    which the 6502's rmw opcodes don't support — those stay as
    SBC/ADC chains. `Reg(A)` is the accumulator-mode shift /
    rotate target; a following `LDA M` can't have `M = Reg(A)`,
    so the pair doesn't arise."""
    if not isinstance(instr, _RMW_TYPES):
        return False
    dst = instr.dst
    if isinstance(dst, (asm_ast.Data, asm_ast.ZP)):
        return True
    if isinstance(dst, asm_ast.Reg) and isinstance(
        dst.reg, (asm_ast.X, asm_ast.Y),
    ):
        return True
    return False


def _is_redundant_lda_after_rmw(
    rmw: asm_ast.Type_instruction, nxt: asm_ast.Type_instruction,
) -> bool:
    """True iff `nxt` is `Mov(M, Reg(A))` whose M equals the rmw's
    dst — a flag-redundant load of the just-modified byte. A
    volatile LDA is never redundant; the read is the observable
    side effect we'd erase by dropping it."""
    if not isinstance(nxt, asm_ast.Mov):
        return False
    if nxt.is_volatile:
        return False
    if not _is_reg_a(nxt.dst):
        return False
    return _operands_match(rmw.dst, nxt.src)


def _operands_match(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    """Structural equality on `Data` / `ZP` / `Reg` operand pairs."""
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    if isinstance(a, asm_ast.Reg) and isinstance(b, asm_ast.Reg):
        return type(a.reg) is type(b.reg)
    return False


def _is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _a_dead_after(
    instrs: list[asm_ast.Type_instruction],
    idx: int,
    label_idx: dict[str, int],
    *,
    visited: frozenset[int] = frozenset(),
) -> bool:
    """True iff `Reg(A)`'s value at position `idx` is dead — every
    forward path through the instruction stream encounters a
    write-without-read of A before any read of A.

    Walks forward within the current basic block. On a `Branch`
    boundary, recurses on BOTH the fall-through (idx+1) AND the
    branch target (resolved via `label_idx`); A is dead at the
    branch iff dead on both paths. On `Jump` continues at the
    target.

    `visited` is the set of indices whose A-liveness we're already
    computing; revisiting one means we hit a cycle in control flow
    (loop back-edge). For cycles we conservatively return True —
    sound because if A were alive on the cycle, the recursion
    would have to find a real read first, and revisiting means
    no read was found before re-entering the loop."""
    while idx < len(instrs):
        if idx in visited:
            return True
        instr = instrs[idx]
        if isinstance(instr, (asm_ast.Ret, asm_ast.Return)):
            return not instr.save_a
        if isinstance(instr, asm_ast.Branch):
            target_idx = label_idx.get(instr.target)
            if target_idx is None:
                return False
            new_visited = visited | {idx}
            return (
                _a_dead_after(
                    instrs, idx + 1, label_idx, visited=new_visited,
                )
                and _a_dead_after(
                    instrs, target_idx + 1, label_idx,
                    visited=new_visited,
                )
            )
        if isinstance(instr, asm_ast.Jump):
            target_idx = label_idx.get(instr.target)
            if target_idx is None:
                return False
            new_visited = visited | {idx}
            return _a_dead_after(
                instrs, target_idx + 1, label_idx, visited=new_visited,
            )
        if isinstance(instr, asm_ast.Label):
            # Falling into a label is fine — it's just a tag, not
            # a control transfer. Skip past.
            idx += 1
            continue
        if _reads_a(instr):
            return False
        if _kills_a(instr):
            return True
        idx += 1
    return True


def _reads_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` reads `Reg(A)` (source or rmw dst)."""
    if isinstance(instr, asm_ast.Mov):
        return _is_reg_a(instr.src)
    if isinstance(instr, asm_ast.Push):
        return _is_reg_a(instr.src)
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        return _is_reg_a(instr.src) or _is_reg_a(instr.dst)
    if isinstance(instr, asm_ast.Xor):
        return (
            _is_reg_a(instr.src1) or _is_reg_a(instr.src2)
            or _is_reg_a(instr.dst)
        )
    if isinstance(instr, asm_ast.Compare):
        return _is_reg_a(instr.left) or _is_reg_a(instr.right)
    if isinstance(instr, (
        asm_ast.Inc, asm_ast.Dec,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        return _is_reg_a(instr.dst)
    return False


def _kills_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` writes `Reg(A)` without reading it first."""
    if isinstance(instr, asm_ast.Mov):
        if _is_reg_a(instr.dst) and not _is_reg_a(instr.src):
            return True
    if isinstance(instr, asm_ast.Pop):
        return _is_reg_a(instr.dst)
    if isinstance(instr, asm_ast.Call):
        # Callees clobber A: 1-byte returns leave the result
        # there; HARGS-returning calls leave A undefined.
        return True
    if isinstance(instr, asm_ast.LoadAddress):
        return _is_reg_a(instr.dst_lo) or _is_reg_a(instr.dst_hi)
    return False
