"""RuleSet — declarative, multi-rule peephole base.

Where ``WindowPass`` is *one* pattern + *one* ``rewrite`` method, a
``RuleSet`` carries a list of ``Rule`` triples ``(pattern, where,
build)`` and tries them in order at each instruction position. It is
the table-based counterpart to ``WindowPass``: the per-pass scaffolding
(the window scan, the shared SSA analyses, the recurring guards) lives
in the base, and a pass is reduced to its rules.

Two payoffs over hand-written ``rewrite`` methods:

  * **Shared guards are declarative.** The recurring soundness checks
    — "this temp is single-use", "we have a symbol table" — are named
    combinators in the ``where=[...]`` list, not boilerplate repeated
    at the top of every ``rewrite``.
  * **Disjoint rules compose into one pass.** Several peepholes that
    match different producer opcodes (e.g. the jump-fold family:
    ``LogicalNot`` / comparison / ``BitwiseAnd`` producers feeding a
    ``JumpIf``) can be carried as separate rules in a single
    ``RuleSet`` and applied in one sweep, instead of one pass each.

A ``Rule`` whose replacement is purely structural (reassemble captured
fields) needs only a one-line ``build``; a rule whose replacement is
computational (fold a constant, narrow a width, walk a def-chain)
keeps a real ``build`` callable — the base doesn't try to turn value
computation into a meta-language. Declarative guards, imperative math.

Subclass / construction interface::

    LNOT_RULE = Rule(
        name="fold_lnot_jump",
        pattern=producer_then_jump(
            m_Unary(op=tac_ast.LogicalNot,
                    src=m_Any(capture='src'),
                    dst=m_Var(capture='t')),
            on='t',
        ),
        where=[single_use('t')],
        build=_build_lnot_jump,   # (m, env, ctx) -> list | None
    )

    # One rule, for the standalone entry point / unit tests:
    def fold_lnot_jump(fn, *, symbols=None):
        return RuleSet(LNOT_RULE).run(fn, PassContext())

    # Several disjoint rules, for the pipeline (one sweep, not three):
    _FOLD_JUMPS = RuleSet(CMP_ZERO_RULE, AND_ZERO_RULE, LNOT_RULE,
                          name="fold_producer_jumps")

Iteration mirrors ``WindowPass``: at each position the rules are tried
in order; the first whose pattern matches, whose ``where`` guards all
pass, and whose ``build`` returns a (possibly empty) list wins. On a
win, iteration advances by that rule's window size; otherwise by 1.
The rewrite output is not re-matched in the same sweep — the outer
fixedpoint handles re-iteration.
"""
from __future__ import annotations

from collections import Counter
from dataclasses import dataclass
from typing import Callable, Sequence

import tac_ast
from passes.optimization.var_visit import count_uses, defs_in
from passes.optimization.framework.base import PassContext
from passes.optimization.framework.phases import FixedpointPass
from passes.optimization.framework.patterns import (
    MatchResult, Pattern, m_OneOf, m_JumpIfTrue, m_JumpIfFalse, m_Specific,
)


@dataclass
class RuleEnv:
    """Per-run analyses shared by every rule in a sweep: SSA use-counts
    and a name → defining-instruction index. Built once per ``run``
    call. Both are sound only for SSA-renamed names (single def); rules
    that walk back through ``def_of`` gate on that themselves."""
    use_count: Counter[str]
    def_idx: dict[str, int]
    instructions: list

    def def_of(self, var: object):
        """The instruction defining ``var``'s name, or None."""
        if not isinstance(var, tac_ast.Var):
            return None
        i = self.def_idx.get(var.name)
        return None if i is None else self.instructions[i]

    def is_single_use(self, var: object) -> bool:
        """True iff ``var`` is a Var used exactly once across the
        function (the canonical fold soundness gate)."""
        return (
            isinstance(var, tac_ast.Var)
            and self.use_count.get(var.name, 0) == 1
        )


# A guard inspects the match + env and returns True to allow the rule.
Guard = Callable[[MatchResult, RuleEnv, PassContext], bool]
# A build produces the replacement instruction list, or None to decline.
Build = Callable[[MatchResult, RuleEnv, PassContext], "list | None"]


@dataclass
class Rule:
    """One ``(pattern, where, build)`` rewrite rule.

      * ``pattern`` — a window of DSL patterns; ``pattern[k]`` matches
        the instruction at offset ``k``. A single ``Pattern`` is
        accepted and wrapped to a 1-element window.
      * ``where`` — guard combinators, all of which must pass after the
        pattern matches and before ``build`` is called.
      * ``build`` — ``(m, env, ctx) -> list | None``. Returns the
        replacement list (empty = delete the window) or None to decline.
      * ``name`` — human-readable id (used for the RuleSet's debug name
        when it carries a single rule)."""
    pattern: Sequence[Pattern]
    build: Build
    where: Sequence[Guard] = ()
    name: str = ""

    def __post_init__(self):
        if isinstance(self.pattern, Pattern):
            self.pattern = [self.pattern]

    @property
    def window_size(self) -> int:
        return len(self.pattern)


class RuleSet(FixedpointPass):
    """A FixedpointPass driven by a list of Rules. Owns the window
    scan, the shared per-run analyses (``RuleEnv``), and guard
    evaluation; the rules supply the matching and the rewrites."""

    def __init__(self, *rules: Rule, name: str | None = None):
        if not rules:
            raise ValueError("RuleSet needs at least one Rule")
        self.rules = rules
        self.name = name or rules[0].name or "ruleset"

    def run(self, fn: tac_ast.Function, ctx: PassContext) -> tac_ast.Function:
        instrs = fn.instructions
        N = len(instrs)
        env = RuleEnv(
            use_count=count_uses(instrs),
            def_idx=_def_index(instrs),
            instructions=instrs,
        )
        out: list = []
        i = 0
        changed = False
        while i < N:
            rep, ws = self._try_rules(instrs, i, N, env, ctx)
            if rep is not None:
                out.extend(rep)
                i += ws
                changed = True
            else:
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

    def _try_rules(self, instrs, i, N, env, ctx):
        """Return ``(replacement, window_size)`` for the first rule that
        fires at position ``i``, or ``(None, 0)`` if none do."""
        for rule in self.rules:
            ws = rule.window_size
            if i + ws > N:
                continue
            m = MatchResult()
            if not all(
                rule.pattern[k].match(instrs[i + k], m) for k in range(ws)
            ):
                continue
            if not all(g(m, env, ctx) for g in rule.where):
                continue
            rep = rule.build(m, env, ctx)
            if rep is not None:
                return rep, ws
        return None, 0


def _def_index(instrs) -> dict[str, int]:
    """Map each defined Var name to its defining-instruction index,
    via the canonical ``var_visit.defs_in``. Single-def for SSA names;
    for non-SSA names this keeps the last def (callers don't walk those
    back)."""
    out: dict[str, int] = {}
    for i, instr in enumerate(instrs):
        for d in defs_in(instr):
            out[d.name] = i
    return out


# -- Guard combinators ---------------------------------------------------

def single_use(capture: str) -> Guard:
    """Allow the rule only if the Var bound at ``capture`` is used
    exactly once across the function. The standard fold gate: the
    producer's dst is dead once its sole consumer is folded away."""
    return lambda m, env, ctx: env.is_single_use(m.bindings.get(capture))


def have_symbols() -> Guard:
    """Allow the rule only when a symbol table is present (rules that
    need type/width queries are no-ops without one)."""
    return lambda m, env, ctx: ctx.symbols is not None


# -- Window-pattern helpers ----------------------------------------------

def producer_then_jump(
    producer: Pattern, *, on: str, jump_capture: str = "jmp",
) -> list[Pattern]:
    """A 2-instruction window: a ``producer`` whose dst is captured as
    ``on`` (via ``dst=m_Var(capture=on)``), immediately followed by a
    ``JumpIfTrue`` / ``JumpIfFalse`` whose condition is that same Var.
    The JumpIf node is bound at ``jump_capture`` (default ``'jmp'``).

    This is the shared shape of the jump-fold family — a single-use
    boolean producer consumed by the very next conditional jump."""
    return [
        producer,
        m_OneOf(
            m_JumpIfTrue(condition=m_Specific(on), capture=jump_capture),
            m_JumpIfFalse(condition=m_Specific(on), capture=jump_capture),
        ),
    ]
