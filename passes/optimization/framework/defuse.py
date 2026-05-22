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
            # ... return replacement instruction or None

Optional hook:
  - prepare_extra(fn, ctx) -> object — stored at env.extra; use for
    whole-function analyses computed once (e.g. use-counts).

Requires SSA form (ctx.ssa_dsts is not None). Without it the pass
is a no-op (def_idx wouldn't be sound for names with multiple defs).
"""
from __future__ import annotations
import abc
from dataclasses import dataclass
from typing import ClassVar

import tac_ast
from passes.optimization.framework.base import PassContext
from passes.optimization.framework.phases import FixedpointPass
from passes.optimization.framework.patterns import MatchResult, Pattern


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
        env = DefUseEnv(
            def_idx=def_idx,
            instructions=fn.instructions,
            extra=self.prepare_extra(fn, ctx),
        )
        out = []
        changed = False
        for instr in fn.instructions:
            m = MatchResult()
            if self.pattern.match(instr, m):
                rep = self.rewrite(m, env, ctx)
                if rep is not None and rep is not instr:
                    out.append(rep)
                    changed = True
                    continue
            out.append(instr)
        if not changed:
            return fn
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
