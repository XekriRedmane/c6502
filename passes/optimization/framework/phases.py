from __future__ import annotations
import abc
import tac_ast
from passes.optimization.framework.base import Pass, PassContext


class PreSsaPass(Pass):
    """One-shot, before to_ssa. Operates on pre-SSA TAC."""


class PreFixedpointPass(Pass):
    """One-shot, after to_ssa, before the main fixedpoint loop."""


class FixedpointPass(Pass):
    """Member of the main SSA-form fixedpoint loop. Driver iterates
    until a full sweep produces no structural change."""


class PostFixedpointPass(Pass):
    """One-shot, after the main fixedpoint converges, before from_ssa."""


class PostDestructionPass(Pass):
    """One-shot, after from_ssa. Operates on post-SSA TAC."""


class PostDestructionFixedpointPass(Pass):
    """Member of the post-destruction fixedpoint loop (currently just
    short-circuit jump folding)."""


class ProgramPass(Pass):
    """Whole-tac_ast.Program pass. run() raises — subclass implements
    run_program instead."""
    def run(self, fn, ctx):
        raise TypeError(
            f"{type(self).__name__} is a ProgramPass; call run_program instead"
        )

    @abc.abstractmethod
    def run_program(self, prog: tac_ast.Program, ctx: PassContext) -> tac_ast.Program: ...
