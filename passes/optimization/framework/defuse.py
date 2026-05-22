"""DefUsePass — SSA def-use-chain peephole base class.

A ``DefUsePass`` walks every instruction in a function. For each
instruction that matches ``pattern`` (the USE site), ``rewrite`` is
called with a ``DefUseEnv`` that exposes the SSA def-index so the
rewrite can walk back through producers.

Subclass interface::

    class MyPeephole(DefUsePass):
        name = "my_peephole"
        pattern = m_Cast(kind=tac_ast.Truncate, src=m_Var(capture='src'))

        def rewrite(self, m, env, ctx):
            src_var = m.bindings['src']
            cast = env.def_of(src_var)
            if not isinstance(cast, tac_ast.ZeroExtend):
                return None
            # ... return replacement instruction, Rewrite, or None

Optional hook:
  - prepare_extra(fn, ctx) -> object — stored at env.extra; use for
    whole-function analyses computed once (e.g. use-counts).

Requires SSA form (ctx.ssa_dsts is not None). Without it the pass
is a no-op (def_idx wouldn't be sound for names with multiple defs).
"""
from __future__ import annotations
import abc
from dataclasses import dataclass, field
from typing import ClassVar

import tac_ast
from passes.optimization.framework.base import PassContext
from passes.optimization.framework.phases import FixedpointPass
from passes.optimization.framework.patterns import MatchResult, Pattern


@dataclass
class Rewrite:
    """Return value for DefUsePass.rewrite when the rewrite should
    atomically drop additional defs (e.g. the inner Binary that fed
    the matched use site). The framework looks up each Var in
    drop_defs via env.def_idx and drops the resulting indices from
    the rebuilt instruction list.

    Caller's responsibility: ensure each named Var is unused after
    the rewrite (single-use gate). If two Rewrites in the same run()
    would drop the same index, the second one is skipped (conflict)."""
    replacement: object  # tac_ast.Type_instruction
    drop_defs: tuple = ()  # tuple of tac_ast.Var

    def __post_init__(self):
        # Normalize drop_defs to a tuple in case caller passed a list.
        if not isinstance(self.drop_defs, tuple):
            self.drop_defs = tuple(self.drop_defs)


@dataclass
class DefUseEnv:
    """Passed as the `env` arg to DefUsePass.rewrite. Provides
    def_idx + instructions list for walking back through SSA defs.
    `extra` holds whatever DefUsePass.prepare_extra returned."""
    def_idx: dict[str, int]
    instructions: list
    extra: object = None

    def def_of(self, var: tac_ast.Var):
        """Return the instruction that defines `var`'s name, or None."""
        i = self.def_idx.get(var.name)
        if i is None:
            return None
        return self.instructions[i]


class DefUsePass(FixedpointPass):
    """Walks every instruction. Each instruction that matches
    `pattern` is a candidate USE; `rewrite` is called with the
    match result + a DefUseEnv that exposes the SSA def-idx for
    walking back through producers.

    Subclass declares:
      - pattern: ClassVar[Pattern] — matches the USE instruction.
        Capture (via m_Var(capture=...)) any operand whose def the
        rewrite wants to inspect.

    Required hook:
      - rewrite(m, env, ctx) -> Type_instruction | None
        Called for every match. Return a replacement instruction, or
        None to leave the use untouched. Replaces a single instruction
        only — extra dead instructions left over are cleaned up by
        DSE on the next fixedpoint iteration.

    Optional hook:
      - prepare_extra(fn, ctx) -> object — stored at env.extra.

    Requires SSA form (ctx.ssa_dsts is not None). Without it, the
    pass is a no-op: def_idx wouldn't be sound for names with
    multiple defs.
    """
    pattern: ClassVar[Pattern]

    def prepare_extra(self, fn, ctx):
        return None

    @abc.abstractmethod
    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None: ...

    def run(self, fn, ctx):
        if ctx.ssa_dsts is None:
            return fn
        def_idx = _build_def_idx(fn.instructions)
        # instructions_view is a mutable list that gets updated as
        # rewrites are recorded, so subsequent env.def_of() lookups
        # see the rewritten form (critical for chained-fusion correctness,
        # e.g. A→B then B→C in a single run for reassoc_const).
        instructions_view = list(fn.instructions)
        env = DefUseEnv(
            def_idx=def_idx,
            instructions=instructions_view,
            extra=self.prepare_extra(fn, ctx),
        )
        rewrites: dict[int, object] = {}
        drop_indices: set[int] = set()
        changed = False
        for i, instr in enumerate(fn.instructions):
            m = MatchResult()
            if not self.pattern.match(instr, m):
                continue
            result = self.rewrite(m, env, ctx)
            if result is None:
                continue
            if isinstance(result, Rewrite):
                new_drops: set[int] = set()
                for v in result.drop_defs:
                    d = def_idx.get(v.name)
                    if d is not None:
                        new_drops.add(d)
                if new_drops & drop_indices:
                    # An earlier rewrite already consumed one of the
                    # same inner defs — skip this rewrite to avoid
                    # double-drop conflicts.
                    continue
                rewrites[i] = result.replacement
                instructions_view[i] = result.replacement
                drop_indices.update(new_drops)
                changed = True
            elif result is not instr:
                rewrites[i] = result
                instructions_view[i] = result
                changed = True
        if not changed:
            return fn
        out = []
        for i, instr in enumerate(fn.instructions):
            if i in drop_indices:
                continue
            out.append(rewrites.get(i, instr))
        return tac_ast.Function(
            name=fn.name, is_global=fn.is_global,
            params=list(fn.params), instructions=out,
        )


def _build_def_idx(instrs) -> dict[str, int]:
    """Map each Var name to the index of an instruction that
    defines it. For SSA names this is unique; for non-SSA names
    it's the LAST def, which the caller is expected not to walk
    back through (DefUsePass.run only honors walk-backs for
    ssa_dsts names — the subclass's rewrite enforces this)."""
    out = {}
    for i, instr in enumerate(instrs):
        if hasattr(instr, 'dst') and isinstance(instr.dst, tac_ast.Var):
            out[instr.dst.name] = i
    return out
