"""Sink `TXA` / `TYA` past a Branch when A is dead on the taken
arm but live on the fall-through arm.

# Pattern

Three consecutive instructions:

    [i]   <op that writes Reg(X|Y) and sets N/Z to the new value>
          — LDX/LDY/INX/INY/DEX/DEY/TAX/TAY, etc.
    [i+1] Mov(Reg(X|Y), Reg(A))     # TXA / TYA
    [i+2] Branch(cond, target)

Rewrite (swap [i+1] and [i+2]):

    [i]   <op>
    [i+1] Branch(cond, target)
    [i+2] Mov(Reg(X|Y), Reg(A))     # TXA / TYA

# Soundness

Branch reads N/Z. Original sequence: Branch sees flags set by TXA
(based on X's value). New sequence: Branch sees flags set by `op`
(also based on X's value, since `op` wrote X and set N/Z from the
written value). Same flag state → same branch direction.

A's value:
  * Original: A := X by [i+1]; both branch arms see A == X.
  * New: A := X only on the fall-through arm (after the moved
    TXA executes); the taken arm sees A = its pre-[i+1] value.

So the rewrite changes observable A on the taken arm. Sound iff A
is dead on the taken arm — i.e., `a_dead_at(target)` is True.

# When this wins

The motion saves the TXA's 2 execution cycles on the
taken-arm path. Code size is unchanged (TXA is 1 byte either
way). The pattern shows up at if-then dispatches where the
condition-tested register is also the input to the if-true arm
that doesn't need A.

Headline source: `if (state & 0x80)` lowering — `LDX state; TXA;
BPL .if_end` — the taken (chase/idle) path doesn't need A; only
the fall-through (attack) path uses A for the `AND #$0F` step.
After the motion the chase/idle path skips the TXA entirely.

# When this does NOT fire

  * `_flags_reflect_src_reg` requirement: [i] must write
    Reg(X|Y) AND set N/Z based on the new register value. LDA M;
    TXA; B?? doesn't qualify — the LDA's N/Z reflect M's bytes,
    not X's. (X is set by TXA only afterward.)
  * A live on the taken arm: moving TXA would lose the A := X
    side effect that the taken arm depends on.
  * A dead on BOTH arms: `dead_a_arith._flags_reflect_src_reg`
    handles this case (drop TXA entirely). We're complementary
    — only fire when at least one arm needs A.

# Where to run

After `replace_pseudoregisters` (operands concrete) and after
`dead_a_arith` (which would drop a fully-dead TXA before we
need to consider moving it). Inside the asm-peephole fixedpoint,
shrinks no code (same byte count) but enables downstream passes
to see the TXA in a different basic block.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at
from passes.dead_a_arith import _writes_reg_setting_n_z


def apply_transfer_past_branch(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    label_to_index = _build_label_map(instrs)
    out: list[asm_ast.Type_instruction] = []
    i = 0
    changed = False
    while i < len(instrs):
        if _can_swap(instrs, i, label_to_index):
            out.append(instrs[i])          # op
            out.append(instrs[i + 2])      # Branch (moved before TXA)
            out.append(instrs[i + 1])      # TXA (now after Branch)
            i += 3
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


def _can_swap(
    instrs: list[asm_ast.Type_instruction], i: int,
    label_to_index: dict[str, int],
) -> bool:
    if i + 2 >= len(instrs):
        return False
    op, mov, br = instrs[i], instrs[i + 1], instrs[i + 2]
    # [i+1]: Mov(Reg(X|Y), Reg(A)) — TXA / TYA.
    if not isinstance(mov, asm_ast.Mov):
        return False
    if mov.is_volatile:
        return False
    if not (
        isinstance(mov.src, asm_ast.Reg)
        and isinstance(mov.src.reg, (asm_ast.X, asm_ast.Y))
        and isinstance(mov.dst, asm_ast.Reg)
        and isinstance(mov.dst.reg, asm_ast.A)
    ):
        return False
    src_reg_type = type(mov.src.reg)
    # [i+2]: conditional Branch.
    if not isinstance(br, asm_ast.Branch):
        return False
    # [i]: writes the same X or Y register AND sets N/Z based on
    # the new value. Reuses `dead_a_arith._writes_reg_setting_n_z`.
    if not _writes_reg_setting_n_z(op, src_reg_type):
        return False
    # The original TXA's A-write must be observable on the
    # fall-through arm; otherwise `dead_a_arith._flags_reflect_
    # src_reg` would drop the TXA entirely (no motion needed).
    # We check A-dead at the target and A-live at fall-through.
    # `a_dead_at` returning False means A is live OR
    # indeterminate (conservative live).
    target_idx = label_to_index.get(br.target)
    if target_idx is None:
        # External target (tail call, long-branch trampoline) —
        # conservative: A might be read by the external code.
        # Don't move.
        return False
    # A-dead at the target (i.e., starting just after the label).
    if not a_dead_at(instrs, target_idx + 1):
        return False
    # A-live at fall-through (i.e., just after the original
    # Branch position, which in our pre-swap indexing is `i+3`).
    if a_dead_at(instrs, i + 3):
        # A is dead on BOTH arms — let `dead_a_arith` drop the
        # TXA outright. No motion required.
        return False
    return True


def _build_label_map(
    instrs: list[asm_ast.Type_instruction],
) -> dict[str, int]:
    out: dict[str, int] = {}
    for j, ins in enumerate(instrs):
        if isinstance(ins, asm_ast.Label):
            out[ins.name] = j
    return out
