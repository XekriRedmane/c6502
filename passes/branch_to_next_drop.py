"""Drop a `Branch` whose target is the immediately-following label.

# Motivating shape

When upstream passes prune a conditional diamond's arms to empty,
the residue left in the asm is:

    Branch(cond, L)
    Label(L)

Both arms reach `L`: branch-taken jumps to `L`; fall-through arrives
at `L` directly. The Branch has no other effect (no register or
memory write; the flags it reads are unchanged), so it can be
dropped.

The headline source is `_translate_sign_extend` in `tac_to_asm.py`,
which emits:

    Mov(src.high, A)
    Branch(MI, sx_neg)
    Mov(Imm(0x00), A)
    Jump(sx_done)
    Label(sx_neg)
    Mov(Imm(0xFF), A)
    Label(sx_done)
    Mov(A, dst.high)

When `dst.high` is dead (the only uses of the widened value are bit-7
tests, sub-256-byte indexing, or arithmetic followed by truncation
back to a byte), DCE drops both `Mov(#$00, A)` / `Mov(#$FF, A)` /
`Mov(A, dst.high)`. `branch_invert` collapses the `BMI sx_neg; JMP
sx_done; Label(sx_neg)` into `Branch(PL, sx_done)`. After
`dead_label_drop` removes the orphaned `Label(sx_neg)` we're left
with:

    Mov(src.high, A)
    Branch(PL, sx_done)
    Label(sx_done)

The Branch goes to the very next instruction — this pass drops it.

# Soundness

A `Branch` reads only the flag bits, never operands. Whether the
condition is true (jump to `L`) or false (fall through to the
following instruction, which IS `Label(L)`), the resulting PC is
the same. No register, no memory cell, and no flag bit changes.

# Label cleanup (load-bearing!)

When dropping `Branch(_, L); Label(L)`, the Label may end up with
no other references. We MUST drop the Label too in the same pass
— here's why:

`flags_dead_at` / `all_flags_dead_at` (in `asm_liveness.py`)
treat any `Label` they encounter on a forward walk as a safe
terminator (flags assumed dead). This relies on the codegen
convention that every basic block sets its own flags before any
Branch the lowering reads. Within a single peephole-fixedpoint
iteration, leaving an orphaned `Label(L)` immediately before
e.g. `Branch(PL, ...)` violates that invariant: `dead_a_arith`
running later in the same iteration would walk forward from the
preceding `Sub`, see the `Label` and return "flags dead" — and
drop the `Sub` whose N output the `Branch` actually consumes.

Dropping the now-orphaned Label closes that window. If the Label
is referenced by other Branch / Jump / Phi-arg sites, we leave
it intact (those sites are still legit fall-through joins; the
post-Label flag state being a join is the usual case the
convention is designed for).

# Iteration

A single forward walk drops every adjacent (`Branch(_, L)`,
`Label(L)`) pair in one sweep — the Label is dropped iff `L`
has no other references in the function. Two stacked Branches
both targeting the same following Label collapse together: the
inner pair drops on this sweep, the outer pair drops on the next
pass through the peephole fixed point.

# Where to run

Inside the asm-peephole fixed-point loop, immediately after
`branch_invert` and `dead_label_drop` — those are the passes that
typically *create* the adjacent shape. Always-on; never adds
instructions, so the fixedpoint's monotone-shrinking invariant
holds.
"""
from __future__ import annotations

import asm_ast


def apply_branch_to_next_drop(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    ref_counts = _label_ref_counts(instrs)
    out: list[asm_ast.Type_instruction] = []
    i = 0
    n = len(instrs)
    while i < n:
        a = instrs[i]
        if (
            isinstance(a, asm_ast.Branch)
            and i + 1 < n
            and isinstance(instrs[i + 1], asm_ast.Label)
            and a.target == instrs[i + 1].name
        ):
            # The Branch we're about to drop owns one reference to
            # the Label. If that's the only reference in the
            # function, the Label is about to be orphaned — drop
            # it in the same pass so `flags_dead_at`'s Label-
            # terminator behavior doesn't misclassify the
            # preceding instruction's flags as dead within the
            # same fixed-point iteration.
            label_name = instrs[i + 1].name
            if ref_counts.get(label_name, 0) <= 1:
                i += 2
            else:
                i += 1
            continue
        out.append(a)
        i += 1
    if len(out) == len(instrs):
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _label_ref_counts(
    instrs: list[asm_ast.Type_instruction],
) -> dict[str, int]:
    """Count how many `Jump.target` / `Branch.target` /
    `Phi.args[].pred_label` references each label name has."""
    out: dict[str, int] = {}
    for instr in instrs:
        if isinstance(instr, (asm_ast.Jump, asm_ast.Branch)):
            out[instr.target] = out.get(instr.target, 0) + 1
        elif isinstance(instr, asm_ast.Phi):
            for arg in instr.args:
                out[arg.pred_label] = out.get(arg.pred_label, 0) + 1
    return out
