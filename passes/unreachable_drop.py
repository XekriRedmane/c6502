"""Drops instructions that no path reaches.

An instruction right after a `Jump`, `Return`, or `Ret`, with no
referenced `Label` in between, can't be reached: nothing falls into
it, and no branch can target it. This pass removes every such
instruction, along with any unreferenced label among them, up to the
next referenced label.

Removing a dead block can remove the only jump to a later label,
which makes the code after that label dead too. The pass repeats
until nothing changes, because the peephole fixed point would
otherwise spend a round per label on such a chain, and a chain
longer than its 16-round cap fails the compile (fuzz.stmts seeds
4411, 4593, and 4793, without --optimize).

Copying a return tail ahead of its label (`carry_return.py`) is what
leaves such code behind. When the last path that jumped to the label
gets its own copy, `dead_label_drop` removes the label, and the
original tail is left after a return with no label in front of it.

Runs in the asm-peephole fixed point, in both pipelines, since it
only ever removes code.
"""

from __future__ import annotations

import asm_ast
from passes.dead_label_drop import _collect_targets

_TERMINATORS = (asm_ast.Jump, asm_ast.Return, asm_ast.Ret)


def apply_unreachable_drop(prog: asm_ast.Program) -> asm_ast.Program:
    """Removes the unlabeled instructions after each terminator."""
    top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl = _rewrite_function(tl)
        top.append(tl)
    return asm_ast.Program(top_level=top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = list(fn.instructions)
    while True:
        targets = _collect_targets(instrs)
        out: list[asm_ast.Type_instruction] = []
        dead = False
        for instr in instrs:
            if isinstance(instr, asm_ast.Label) and instr.name in targets:
                dead = False
            elif dead:
                continue
            out.append(instr)
            if isinstance(instr, _TERMINATORS):
                dead = True
        if len(out) == len(instrs):
            break
        instrs = out
    if len(instrs) == len(fn.instructions):
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )
