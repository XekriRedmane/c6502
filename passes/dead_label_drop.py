"""Drop labels that no `Jump` / `Branch` / `Phi` references.

# Motivating shape

Various upstream passes leave behind `Label` instances whose name
no longer appears in any control-flow target. The asm-SSA
construction emits per-block `.<fn>@asm_ssa_block@N` markers; SSA
destruction's parallel-copy edges sometimes have no other
target; `apply_branch_invert`'s rewrite drops the Jump but keeps
its former target's Label, which may now be orphaned.

Orphaned labels are pure noise in the emitted asm. They're also
optimization blockers: `apply_branch_invert` matches
`Branch / Jump / Label` consecutive, so an orphan Label sitting
between Branch and Jump prevents the rewrite from firing.
Dropping orphans first exposes the consecutive pattern.

# Algorithm

Per function, walk the instructions twice:

  1. Collect all target names: every `Jump.target`,
     `Branch.target`, and `PhiArg.pred_label`.
  2. Emit the instructions in order, dropping any `Label` whose
     name isn't in the target set.

A function's entry isn't an `asm_ast.Label` (it's the `Function`'s
`name` field), so dropping a Label is never observable from
outside the function.

# Soundness

A `Label` instance is a marker only — it has no runtime effect
of its own. Control flow reaches a Label's successor either by
fall-through (in which case the Label is irrelevant) or via a
Jump/Branch targeting that name. If no Jump/Branch targets the
Label, fall-through is the only path; the Label can be dropped
without changing semantics. Phis reference predecessor labels
by name and must be considered (the pass should run AFTER SSA
destruction or include Phi-arg names defensively if Phis remain).

# Where to run

Inside the asm-peephole fixed-point loop, ideally early so
downstream peepholes (notably `apply_branch_invert`) see the
cleaned-up label set. Always-on — works at both unoptimized and
optimized pipelines since it never adds code.
"""
from __future__ import annotations

import asm_ast


def apply_dead_label_drop(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    targets = _collect_targets(fn.instructions)
    out: list[asm_ast.Type_instruction] = []
    for instr in fn.instructions:
        if isinstance(instr, asm_ast.Label) and instr.name not in targets:
            continue
        out.append(instr)
    if len(out) == len(fn.instructions):
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _collect_targets(
    instrs: list[asm_ast.Type_instruction],
) -> set[str]:
    """Set of label names referenced by any `Jump.target`,
    `Branch.target`, or `Phi.args[k].pred_label`."""
    out: set[str] = set()
    for instr in instrs:
        if isinstance(instr, (asm_ast.Jump, asm_ast.Branch)):
            out.add(instr.target)
        elif isinstance(instr, asm_ast.Phi):
            for arg in instr.args:
                out.add(arg.pred_label)
    return out
