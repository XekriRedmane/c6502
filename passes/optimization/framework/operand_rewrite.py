from __future__ import annotations
import abc
from typing import ClassVar
import tac_ast
from passes.optimization.framework.base import PassContext
from passes.optimization.framework.phases import FixedpointPass
from passes.optimization.framework.patterns import MatchResult, Pattern
from passes.optimization.var_visit import rewrite_uses_in


class OperandRewritePass(FixedpointPass):
    """Walks every USE-position operand in fn's instructions,
    applies `operand_pattern` to each, and rewrites matched
    operands via `rewrite_operand`. DEF positions and non-Val
    fields are untouched (see `rewrite_uses_in` for the exact
    rule set).

    Subclass declares:
      - operand_pattern: ClassVar[Pattern] — matches against each
        candidate Type_val operand (typically m_Var or m_Constant
        with constraints).

    Required hook:
      - rewrite_operand(m, prep, ctx) -> Type_val | None
        Returns the replacement operand, or None to leave the
        matched operand unchanged (the pattern matched but a
        soundness gate failed, etc.).

    Optional hook:
      - prepare(fn, ctx) -> Any
        Whole-fn analysis result. Default: None.

    Inherits FixedpointPass. For other phases, host an instance at
    module scope and delegate from the phase-specific subclass
    (same pattern used elsewhere in the framework)."""
    operand_pattern: ClassVar[Pattern]

    def prepare(self, fn, ctx):
        return None

    @abc.abstractmethod
    def rewrite_operand(self, m: MatchResult, prep, ctx: PassContext): ...

    def run(self, fn, ctx):
        prep = self.prepare(fn, ctx)

        def mapper(v):
            m = MatchResult()
            if not self.operand_pattern.match(v, m):
                return v
            new = self.rewrite_operand(m, prep, ctx)
            return new if new is not None else v

        out = []
        changed = False
        for instr in fn.instructions:
            new_instr = rewrite_uses_in(instr, mapper)
            if new_instr is not instr:
                changed = True
            out.append(new_instr)
        if not changed:
            return fn
        return tac_ast.Function(
            name=fn.name, is_global=fn.is_global,
            params=list(fn.params), instructions=out,
        )
