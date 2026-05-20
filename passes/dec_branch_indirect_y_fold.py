"""Drop the redundant `LDY #$00` before an `Indirect(0)` operand
when Y is already 0 from a `Dec(Reg(Y)); Branch(NE, _)` fall-through.

Motivating shape (snd_delay_up's inner-loop exit):

    .loop@1_continue:
    DEY
    BNE .loop@1_continue
    LDY #$00                  ; redundant — Y already 0
    CMP (DPTR),Y

The asm IR doesn't have the LDY as a separate atom — it's embedded
inside the operand shape. `Compare(Reg(A), Indirect(0))` lowers in
`asm_emit` to `LDY #$00; CMP (DPTR),Y` because the `Indirect(off)`
operand variant prepends a Y-setup. The `IndirectY()` variant
skips the setup and emits just `<op> (DPTR),Y`, using whatever Y
already holds.

The rewrite: any `Indirect(0)` operand that immediately follows a
`Dec(Reg(Y)); Branch(NE, _)` pair → `IndirectY()`. After the BNE
fall-through Y == 0, so `Indirect(0)` and `IndirectY()` access the
same byte and the redundant LDY disappears.

Soundness:
  * DEY decrements Y; sets Z based on result.
  * BNE branches when Z = 0 (Y != 0). Fall-through has Z = 1
    (Y == 0).
  * `Indirect(0)` and `IndirectY()` with Y == 0 access the same
    memory byte through DPTR.
  * The LDY #$00 the emit would otherwise insert writes N/Z
    (Z=1, N=0) — same flag values DEY already set when Y=0. So
    omitting the LDY preserves N/Z at the access point.

Strict adjacency: only fires when the `Indirect(0)`-using
instruction is the IMMEDIATELY-NEXT instruction after the
Branch. A Label between them could be the target of another
branch whose taker has arbitrary Y; rewriting would change that
caller's view. The 2-cycle / 2-byte savings doesn't justify a
fuzzier match.

Where to run: in the asm-peephole fixed-point loop, after
`apply_volatile_void_read_cmp` and the loop-counter promotions
have settled into their final IR shapes.
"""
from __future__ import annotations

import asm_ast


def apply_dec_branch_indirect_y_fold(
    prog: asm_ast.Program,
) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = list(instrs)
    for i in range(len(out) - 2):
        if not _is_dec_y(out[i]):
            continue
        if not _is_branch_ne(out[i + 1]):
            continue
        # The instruction at i+2 inherits Y == 0 on the fall-
        # through path. Look for Indirect(0) operands and rewrite.
        target = out[i + 2]
        rewritten = _rewrite_indirect_zero(target)
        if rewritten is target:
            continue
        out[i + 2] = rewritten
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _is_dec_y(instr: asm_ast.Type_instruction) -> bool:
    if not isinstance(instr, asm_ast.Dec):
        return False
    return (
        isinstance(instr.dst, asm_ast.Reg)
        and isinstance(instr.dst.reg, asm_ast.Y)
    )


def _is_branch_ne(instr: asm_ast.Type_instruction) -> bool:
    return (
        isinstance(instr, asm_ast.Branch)
        and isinstance(instr.cond, asm_ast.NE)
    )


def _is_indirect_zero(op: asm_ast.Type_operand | None) -> bool:
    return isinstance(op, asm_ast.Indirect) and op.offset == 0


def _rewrite_indirect_zero(
    instr: asm_ast.Type_instruction,
) -> asm_ast.Type_instruction:
    """If any of `instr`'s operands is `Indirect(0)`, return a new
    instance with those operands replaced by `IndirectY()`. Returns
    the input unchanged when nothing matches."""
    indy = asm_ast.IndirectY()
    if isinstance(instr, asm_ast.Mov):
        new_src = indy if _is_indirect_zero(instr.src) else instr.src
        new_dst = indy if _is_indirect_zero(instr.dst) else instr.dst
        if new_src is instr.src and new_dst is instr.dst:
            return instr
        return asm_ast.Mov(
            src=new_src, dst=new_dst, is_volatile=instr.is_volatile,
        )
    if isinstance(instr, asm_ast.Compare):
        new_left = indy if _is_indirect_zero(instr.left) else instr.left
        new_right = indy if _is_indirect_zero(instr.right) else instr.right
        if new_left is instr.left and new_right is instr.right:
            return instr
        return asm_ast.Compare(left=new_left, right=new_right)
    return instr
