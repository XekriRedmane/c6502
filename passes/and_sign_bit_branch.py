"""Fold `LDA M; AND #$80; B(EQ|NE) target` → `LDA M; B(PL|MI) target`.

# Pattern

Three consecutive instructions:

    Mov(<any src>, Reg(A))      # LDA src — sets A and N=bit7(src)
    And(Imm(0x80), Reg(A))      # AND #$80 — sets N=bit7(A & 0x80) = bit7(A)
    Branch(EQ | NE, target)     # BEQ/BNE — tests Z

→

    Mov(<any src>, Reg(A))
    Branch(PL | MI, target)     # BPL = "bit7(A) == 0"; BMI = "bit7(A) == 1"

# Soundness

After `LDA M; AND #$80`:
  * A = M & 0x80     (either 0 or 0x80)
  * N = bit7(A & 0x80) = bit7(M)
  * Z = (A & 0x80 == 0) = (bit7(M) == 0)

The Branch reads Z: BEQ fires iff Z=1 iff bit7(M)=0 iff N=0 iff BPL
fires. Symmetrically, BNE → BMI.

If we drop the AND and flip the branch, A is `M` (not `M & 0x80`).
The flag check is preserved (LDA M already set N=bit7(M)). Soundness
hinges on A being unused after the Branch — otherwise dropping the
AND corrupts the observable A value. A's liveness at the Branch's
two successor positions is checked via `a_dead_at`.

# Motivating case

`while ((x & 0x80) == 0)` (the "x is non-negative as signed" loop
tail, common after a `do { ... x--; } while (...)` rotates the
test to the bottom). c99_to_tac generates promote-to-int +
bitwise-AND + zero-test. After my high-byte DCE and round-trip
elimination, what's left in the loop tail is:

    LDA x
    AND #$80
    BEQ continue

→ `LDA x; BPL continue`. 2 bytes / 2 cycles saved per occurrence.

# Where to run

Inside the asm-peephole fixed-point loop. Earlier passes (mem-const-
prop, const-arith-fold, round-trip-load-drop, asm-dead-store) need
to run first to expose the canonical `LDA M; AND #$80; B*` shape —
the original lowering has STA/LDA round-trips and high-byte ops
that obscure it. We're a 3-instruction-window peephole, so order
within the fixed-point loop is not critical."""

from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at


def apply_and_sign_bit_branch(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _is_reg_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        # The classic 3-instruction window: LDA; AND #80; Branch.
        if (i + 2 < len(instrs)
                and _is_lda_to_a(instrs[i])
                and _is_and_imm80(instrs[i + 1])
                and _is_eq_ne_branch(instrs[i + 2])
                and a_dead_at(instrs, i + 3)):
            br = instrs[i + 2]
            new_cond = asm_ast.PL() if isinstance(br.cond, asm_ast.EQ) else asm_ast.MI()
            out.append(instrs[i])
            out.append(asm_ast.Branch(cond=new_cond, target=br.target))
            i += 3
            continue
        # The 4-instruction variant with an intermediate STA. Arises
        # after passes.split_mem_to_mem lowers the LDA+STA mem-to-mem
        # into two atoms — what used to be `Mov(M, dst_mem); AND #80;
        # Branch` (3-instr, matched by the case above via the mem-to-
        # mem-aware `_is_lda_to_a`) is now `Mov(M, A); Mov(A, dst);
        # AND #80; Branch`. STA doesn't touch A or N/Z, so the same
        # fold applies: drop AND, flip the branch.
        if (i + 3 < len(instrs)
                and _is_lda_to_a(instrs[i])
                and _is_a_store_to_mem(instrs[i + 1])
                and _is_and_imm80(instrs[i + 2])
                and _is_eq_ne_branch(instrs[i + 3])
                and a_dead_at(instrs, i + 4)):
            br = instrs[i + 3]
            new_cond = asm_ast.PL() if isinstance(br.cond, asm_ast.EQ) else asm_ast.MI()
            out.append(instrs[i])
            out.append(instrs[i + 1])
            out.append(asm_ast.Branch(cond=new_cond, target=br.target))
            i += 4
            continue
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _is_a_store_to_mem(instr) -> bool:
    """True iff `instr` is `Mov(Reg(A), <stable mem>)` — a STA that
    preserves A's value AND leaves N/Z untouched. The destination's
    addressing mode doesn't matter for this fold; we just need to
    know the atom is a STA, not something that clobbers A."""
    return (isinstance(instr, asm_ast.Mov)
            and not instr.is_volatile
            and _is_reg_a(instr.src)
            and not isinstance(instr.dst, asm_ast.Reg))


def _is_lda_to_a(instr) -> bool:
    """True iff `instr` is a Mov whose emit lowering uses `LDA` and
    leaves A with the source's value AND N/Z reflecting that value.
    Covers two shapes:

      * `Mov(<non-Reg src>, Reg(A))` — direct LDA into A.
      * `Mov(<non-Reg src>, <stable memory dst>)` — emit's LDA + STA
        pair. The LDA half still leaves A = src and N/Z = src's flag
        bits, which is what the downstream AND #$80 reads.

    Mov(Reg(A), <mem>) (a pure STA) does NOT load A — A's value and
    the flags carry over from whatever set them earlier. Skip.
    """
    if not isinstance(instr, asm_ast.Mov):
        return False
    if isinstance(instr.src, asm_ast.Reg):
        # Mov(X/Y, A) (TXA/TYA) DOES set N/Z. Allow when dst is A.
        return isinstance(instr.src.reg, (asm_ast.X, asm_ast.Y)) and _is_reg_a(instr.dst)
    # Non-Reg src: dst is either Reg(A) or memory. Both shapes emit
    # via LDA src; ... and leave A with src's flag effects.
    return True


def _is_and_imm80(instr) -> bool:
    return (isinstance(instr, asm_ast.And)
            and isinstance(instr.src, asm_ast.Imm)
            and (instr.src.value & 0xFF) == 0x80
            and _is_reg_a(instr.dst))


def _is_eq_ne_branch(instr) -> bool:
    return (isinstance(instr, asm_ast.Branch)
            and isinstance(instr.cond, (asm_ast.EQ, asm_ast.NE)))
