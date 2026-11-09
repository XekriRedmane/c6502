"""Loop-invariant code motion for extends and truncates.

An instruction inside a natural loop whose operands are all Constants
or SSA values defined outside the loop computes the same value on
every iteration. It's moved to the end of the loop's preheader, the
one block outside the loop that flows into its header, so it runs
once. Instructions that become invariant once an earlier one moves
out are moved too.

Only the extends and `Truncate` move. They have no side effects, so
running one before a loop that would have skipped it changes only the
time taken, and they lower to byte copies (plus a zero or sign byte),
so moving one out never adds work. Moving an add out as well was
tried, and it made `floor_enemy_advance` two instructions longer:
`player_col + 9` is used once, on a conditional path, and holding it
across the loop costs a store and a load where the add in place took
none.

The motivating case is a byte that's zero-extended inside a loop to
be added to a pointer, such as `entry += search_size` with `uint8_t
search_size`. Inside the loop the extended copy has to stay live next
to the byte, which costs a copy per iteration.

The preheader must have the header as its only successor, so the
moved instructions run only on the way into the loop. A loop whose
header has more than one predecessor outside the loop is left alone.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.cfg import build_cfg, cfg_to_function, natural_loops
from passes.optimization.framework import FixedpointPass, PassContext
from passes.optimization.var_visit import defs_in

_TERMINATORS = (
    tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp, tac_ast.JumpIfMasked, tac_ast.Ret,
)


class HoistLoopInvariants(FixedpointPass):
    name = "hoist_loop_invariants"

    def run(self, fn, ctx):
        return hoist_loop_invariants(fn, ctx)


def _movable(instr) -> bool:
    return isinstance(instr, (
        tac_ast.ZeroExtend, tac_ast.SignExtend, tac_ast.Truncate,
    ))


def _operands(instr) -> list[tac_ast.Type_val]:
    return [instr.src]


def hoist_loop_invariants(
    fn: tac_ast.Function, ctx: PassContext,
) -> tac_ast.Function:
    """Returns `fn` with loop-invariant cheap pure instructions moved
    to their loop's preheader. A no-op outside SSA form."""
    ssa = ctx.ssa_dsts
    if not ssa:
        return fn
    cfg = build_cfg(fn)
    changed = False
    for header, body in natural_loops(cfg):
        outside = [
            p for p in cfg.blocks[header].predecessors if p not in body
        ]
        if len(outside) != 1:
            continue
        pre = cfg.blocks[outside[0]]
        if pre.successors != [header] or not pre.instructions:
            continue
        defined_in_loop: set[str] = set()
        for bid in body:
            for instr in cfg.blocks[bid].instructions:
                for v in defs_in(instr):
                    defined_in_loop.add(v.name)

        def invariant(v) -> bool:
            if isinstance(v, tac_ast.Constant):
                return True
            return (isinstance(v, tac_ast.Var) and v.name in ssa
                    and v.name not in defined_in_loop)

        hoisted: list[tac_ast.Type_instruction] = []
        progress = True
        while progress:
            progress = False
            for bid in sorted(body):
                blk = cfg.blocks[bid]
                keep = []
                for instr in blk.instructions:
                    dst = getattr(instr, "dst", None)
                    if (_movable(instr)
                            and isinstance(dst, tac_ast.Var)
                            and dst.name in ssa
                            and all(invariant(v) for v in _operands(instr))):
                        hoisted.append(instr)
                        defined_in_loop.discard(dst.name)
                        progress = True
                    else:
                        keep.append(instr)
                blk.instructions = keep
        if not hoisted:
            continue
        instrs = pre.instructions
        at = len(instrs)
        if isinstance(instrs[-1], _TERMINATORS):
            at -= 1
        pre.instructions = instrs[:at] + hoisted + instrs[at:]
        changed = True
    if not changed:
        return fn
    return cfg_to_function(fn, cfg)
