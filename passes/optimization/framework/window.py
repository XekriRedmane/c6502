"""WindowPass — adjacency-based peephole base class.

A ``WindowPass`` slides a fixed-size window over a function's
instruction list, matching each position against a DSL pattern
sequence and calling ``rewrite`` when the window matches.

Subclass interface::

    class MyPeephole(WindowPass):
        name = "my_peephole"
        window_size = 2           # how many instructions to inspect

        # For window_size == 1: a single Pattern.
        # For window_size > 1: a sequence of len == window_size.
        pattern = [
            m_Unary(op=tac_ast.LogicalNot, dst=m_Var(capture='t')),
            m_JumpIfTrue(condition=m_Specific('t'), capture='jmp'),
        ]

        def prepare(self, fn, ctx):
            # Optional: whole-function analysis (e.g. use-counts).
            # Return value is passed as `prep` to every rewrite call.
            return None

        def rewrite(self, m, prep, ctx):
            # m.bindings holds captured values.
            # Return a replacement list (may be empty = delete window)
            # or None to leave the window unchanged and advance by 1.
            ...

Iteration: advances by ``window_size`` on a successful rewrite (one
whose ``rewrite`` returns a list), by 1 on a non-match or declined
rewrite (``rewrite`` returned ``None``). The output is NOT re-matched
in the same pass; the outer fixedpoint handles re-iteration.

``WindowPass`` inherits ``FixedpointPass``, so instances go in the
``fixedpoint`` slot of ``PhaseDriver`` (or ``post_destruction_
fixedpoint`` for post-SSA peepholes) and the driver iterates until
the function is structurally unchanged.
"""
from __future__ import annotations
import abc
from typing import ClassVar, Sequence
import tac_ast
from passes.optimization.framework.base import PassContext
from passes.optimization.framework.phases import FixedpointPass
from passes.optimization.framework.patterns import MatchResult, Pattern


class WindowPass(FixedpointPass):
    """Adjacency-based peephole base. Subclass declares:

      - window_size: ClassVar[int] — number of consecutive instructions
        to inspect.
      - pattern: ClassVar[Pattern | Sequence[Pattern]] — DSL match.
        For window_size == 1, a single Pattern (or 1-element sequence).
        For window_size > 1, a sequence of length `window_size`;
        patterns[k] matches instrs[i+k].

    Optional hook:
      - prepare(fn, ctx) -> Any — runs once per `run` call BEFORE
        sliding the window. Use this for whole-function analyses
        (use-counts, def maps). Whatever it returns is passed as the
        `prep` arg to `rewrite`. Default: returns None.

    Required hook:
      - rewrite(m, prep, ctx) -> list[Type_instruction] | None
        Called when the window matches the pattern. Return a
        replacement list (possibly empty = delete the window), or
        None to decline (soundness gate failed; the window is left
        untouched, iteration advances by 1).

    Iteration advances by `window_size` on a successful rewrite, by
    1 on a non-match. The rewrite output is NOT re-matched in the
    same pass (the outer fixedpoint handles re-iteration).
    """
    window_size: ClassVar[int] = 1
    pattern: ClassVar

    def prepare(self, fn, ctx):
        return None

    @abc.abstractmethod
    def rewrite(self, m: MatchResult, prep, ctx: PassContext) -> list | None: ...

    def run(self, fn, ctx):
        prep = self.prepare(fn, ctx)
        instrs = fn.instructions
        N = len(instrs)
        patterns = self._patterns_seq()
        out: list = []
        i = 0
        changed = False
        while i < N:
            if i + self.window_size <= N:
                m = MatchResult()
                if all(
                    patterns[k].match(instrs[i + k], m)
                    for k in range(self.window_size)
                ):
                    rep = self.rewrite(m, prep, ctx)
                    if rep is not None:
                        out.extend(rep)
                        i += self.window_size
                        changed = True
                        continue
            out.append(instrs[i])
            i += 1
        if not changed:
            return fn
        return tac_ast.Function(
            name=fn.name,
            is_global=fn.is_global,
            params=list(fn.params),
            instructions=out,
        )

    def _patterns_seq(self) -> Sequence[Pattern]:
        p = self.pattern
        if isinstance(p, Pattern):
            return [p]
        return list(p)
