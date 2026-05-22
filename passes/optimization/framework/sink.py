from __future__ import annotations
import abc
from typing import ClassVar
import tac_ast
from passes.optimization.framework.base import PassContext
from passes.optimization.framework.phases import FixedpointPass
from passes.optimization.framework.patterns import MatchResult, Pattern


class SinkPass(FixedpointPass):
    """Walks fn.instructions. For each instruction matching
    `pattern`, calls `find_target(m, instrs, src_idx, prep, ctx)`
    to compute a target index. When `target > src_idx`, the matched
    instruction is MOVED past instructions[src_idx+1 .. target]
    (inclusive) — i.e., it ends up at index `target` after the move,
    with the intervening instructions shifted left by one.

    Subclass declares:
      - pattern: ClassVar[Pattern] — matches the instruction to sink.

    Required hook:
      - find_target(m, instrs, src_idx, prep, ctx) -> int | None
        Returns the target index (must be > src_idx) or None to
        leave the instruction in place. The subclass is responsible
        for all soundness gates (live-range checks, block-boundary
        avoidance, etc.).

    Optional hook:
      - prepare(fn, ctx) -> Any. Default: None.

    Iteration semantics: a successful move advances past the new
    position (no re-matching the moved instruction in the same
    pass). The outer fixedpoint handles chains."""
    pattern: ClassVar[Pattern]

    def prepare(self, fn, ctx):
        return None

    @abc.abstractmethod
    def find_target(
        self, m: MatchResult, instrs: list, src_idx: int, prep, ctx: PassContext,
    ) -> int | None: ...

    def run(self, fn, ctx):
        prep = self.prepare(fn, ctx)
        instrs = list(fn.instructions)
        n = len(instrs)
        out = []
        i = 0
        changed = False
        while i < n:
            instr = instrs[i]
            m = MatchResult()
            if self.pattern.match(instr, m):
                target = self.find_target(m, instrs, i, prep, ctx)
                if target is not None and target > i:
                    # Move: emit instrs[i+1 .. target], then the matched
                    # instruction at position `target`.
                    for k in range(i + 1, target + 1):
                        out.append(instrs[k])
                    out.append(instr)
                    i = target + 1
                    changed = True
                    continue
            out.append(instr)
            i += 1
        if not changed:
            return fn
        return tac_ast.Function(
            name=fn.name, is_global=fn.is_global,
            params=list(fn.params), instructions=out,
        )
