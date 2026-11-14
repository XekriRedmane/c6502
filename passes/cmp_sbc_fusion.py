"""Fuse `LDA M; CMP N; B<cond> L; Label L2; SEC; SBC N` (same N) by
moving the `SEC; SBC` ahead of the branch and dropping the `CMP`.

# Pattern

Six consecutive instructions (or five, without the label, and
without the `SEC` after a `BCC`, whose fall-through has C set, as
`carry_known` leaves it):

    [a]   Mov(<X>, Reg(A))      # LDA M (or any other A-load)
    [a+1] Compare(Reg(A), N)    # CMP N
    [a+2] Branch(<cond>, L1)    # B<cond>  (cond ∈ {CC, CS, EQ, NE, MI, PL})
    [a+3] Label(L2)             # fall-through only — no other branches target L2
    [a+4] SetCarry()            # SEC
    [a+5] Sub(N, Reg(A))        # SBC N    (same N as the CMP)

→

    Mov(<X>, Reg(A))
    SetCarry()
    Sub(N, Reg(A))
    Branch(<cond>, L1)
    Label(L2)

# Why this is sound

`CMP N` sets the C, Z, N flags based on `A - N`:
  * C = 1 iff A >= N (no borrow)
  * Z = 1 iff A == N
  * N = bit7(A - N)

`SEC; SBC N` (with A unchanged from the LDA) sets the same C, Z, N
flags (because SBC reads C_in=1 and computes `A - N - (1 - C_in) =
A - N`). It also writes A and sets V — but V wasn't readable before
(CMP doesn't set V on the 6502), and the branch conditions in the
allowed set don't read V either.

So the `CMP` is redundant once the SBC's flags reach the branch.
Drop the CMP, move the SBC up.

# Eligibility

  * Branch condition must be CC / CS / EQ / NE / MI / PL — readers
    of C, Z, N. BVC / BVS are excluded: SBC sets V, CMP doesn't,
    so the flag at the branch would differ.
  * `Label(L2)` must NOT be a `Branch` / `Jump` target elsewhere
    in the function. Otherwise some other path could reach L2 with
    a different A value, and our re-ordering would corrupt it for
    those paths.
  * The CMP's right operand and the SBC's src operand must be
    structurally identical (`Data` / `ZP` / `Imm` with same fields).
  * The leading instruction must write `Reg(A)` and set N/Z based
    on A's new value — any `Mov` to A or any of the A-arithmetic
    ops qualify (analogous to the "A-writer" predicate in
    `const_arith_fold`).
  * A and V must be dead at the branch target L1, which must be a
    label in the function. The rewrite runs the SBC on the taken
    path too, so A there holds M - N instead of M, and V holds the
    SBC's overflow instead of its earlier value.

# Where to run

Inside the asm-peephole fixed-point loop. Earlier passes
(mem_const_prop, redundant_load with cross-block-fall-through
tracking) need to have done their work first to expose the
canonical six-instruction shape — the raw `tac_to_asm` output
has extra STA/LDA round-trips that obscure it.

# Motivating case

`if (a >= b) { x = a - b; ... }`:

    LDA a
    CMP b
    BCC .end           ; skip when a < b
    .ssa_block:
    SEC
    SBC b
    STA x

→ 5 bytes / 6 cycles saved per occurrence (the CMP and the
unreachable SEC overlap with the SBC's effect)."""

from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at, v_dead_at


_ALLOWED_BRANCHES: tuple[type, ...] = (
    asm_ast.CC, asm_ast.CS, asm_ast.EQ, asm_ast.NE,
    asm_ast.MI, asm_ast.PL,
)


def apply_cmp_sbc_fusion(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    branch_targets = _collect_branch_targets(instrs)
    labels = {
        instr.name: k for k, instr in enumerate(instrs)
        if isinstance(instr, asm_ast.Label)
    }
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        # The fall-through label is optional.
        j = i + 3
        if (j < len(instrs) and isinstance(instrs[j], asm_ast.Label)
                and instrs[j].name not in branch_targets):
            j += 1
        # After `BCC`, C is set on the fall-through, so `carry_known`
        # may have dropped the SEC; the moved SBC needs it back.
        sec = j < len(instrs) and isinstance(instrs[j], asm_ast.SetCarry)
        if not sec and not (
            i + 2 < len(instrs)
            and isinstance(instrs[i + 2], asm_ast.Branch)
            and isinstance(instrs[i + 2].cond, asm_ast.CC)
        ):
            j = len(instrs)
        sub = j + 1 if sec else j
        if (sub < len(instrs)
                and _writes_a_with_flag_effect(instrs[i])
                and _is_cmp_against(instrs[i + 1])
                and _is_eligible_branch(instrs[i + 2])
                and _is_sub_against(instrs[sub])
                and _operands_equal(instrs[i + 1].right, instrs[sub].src)
                and _taken_path_safe(instrs, labels, instrs[i + 2].target)):
            # Emit: LDA, SEC, SBC, Branch, Label.
            out.append(instrs[i])
            out.append(asm_ast.SetCarry())
            out.append(instrs[sub])       # SBC N
            out.append(instrs[i + 2])     # Branch
            out.extend(instrs[i + 3:j])   # Label
            i = sub + 1
            continue
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _taken_path_safe(
    instrs: list[asm_ast.Type_instruction],
    labels: dict[str, int],
    target: str,
) -> bool:
    """Returns True iff A and V are dead at the branch target. On the
    taken path the original leaves A holding M and V untouched, while
    the rewrite has already run the `SBC`, so A holds M - N and V is
    the subtraction's overflow. The fall-through path runs the `SBC`
    either way. A target that isn't a label in this function is
    treated as live."""
    idx = labels.get(target)
    if idx is None:
        return False
    return a_dead_at(instrs, idx) and v_dead_at(instrs, idx)


def _collect_branch_targets(instrs) -> set[str]:
    out: set[str] = set()
    for instr in instrs:
        if isinstance(instr, (asm_ast.Jump, asm_ast.Branch)):
            out.add(instr.target)
    return out


def _is_reg_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _writes_a_with_flag_effect(instr) -> bool:
    if isinstance(instr, asm_ast.Mov):
        return _is_reg_a(instr.dst)
    if isinstance(instr, (asm_ast.And, asm_ast.Or,
                          asm_ast.Add, asm_ast.Sub)):
        return _is_reg_a(instr.dst)
    if isinstance(instr, asm_ast.Xor):
        return _is_reg_a(instr.dst)
    return False


def _is_cmp_against(instr) -> bool:
    """Match `Compare(Reg(A), <op>)`. Caller validates that the
    right operand matches the SBC's source."""
    return (isinstance(instr, asm_ast.Compare)
            and _is_reg_a(instr.left))


def _is_sub_against(instr) -> bool:
    """Match `Sub(<op>, Reg(A))`."""
    return (isinstance(instr, asm_ast.Sub)
            and _is_reg_a(instr.dst))


def _is_eligible_branch(instr) -> bool:
    return (isinstance(instr, asm_ast.Branch)
            and isinstance(instr.cond, _ALLOWED_BRANCHES))


def _operands_equal(a, b) -> bool:
    """Structural equality of two memory or immediate operands."""
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    if isinstance(a, asm_ast.Imm) and isinstance(b, asm_ast.Imm):
        return a.value == b.value
    return False
