from __future__ import annotations
from dataclasses import dataclass, field
from typing import Sequence
import tac_ast
from passes.optimization.framework.base import Pass, PassContext
from passes.optimization.framework.phases import (
    PreSsaPass, PreFixedpointPass, FixedpointPass,
    PostFixedpointPass, PostDestructionPass, PostDestructionFixedpointPass,
)
from passes.optimization.ssa_construction import to_ssa
from passes.optimization.ssa_destruction import from_ssa


@dataclass
class PhaseDriver:
    """Runs an ordered, phase-typed pipeline of Passes over one
    tac_ast.Function. Phase membership is enforced by the ABC of each
    list element — passing a FixedpointPass in the pre_ssa slot is a
    type error caught by a constructor check.

    Calling apply without a SymbolTable skips SSA construction and the
    SSA-form pipeline entirely (legacy synthetic-Function tests);
    pre_ssa passes still run, since they're pre-SSA by definition."""

    pre_ssa: Sequence[PreSsaPass] = field(default_factory=list)
    pre_fixedpoint: Sequence[PreFixedpointPass] = field(default_factory=list)
    fixedpoint: Sequence[FixedpointPass] = field(default_factory=list)
    post_fixedpoint: Sequence[PostFixedpointPass] = field(default_factory=list)
    post_destruction: Sequence[PostDestructionPass] = field(default_factory=list)
    post_destruction_fixedpoint: Sequence[PostDestructionFixedpointPass] = field(default_factory=list)

    fixedpoint_cap: int = 256  # safety net; current driver has none

    def __post_init__(self):
        # Enforce phase membership at construction (the type system
        # only enforces it on the caller's static types; the runtime
        # check makes it visible in tests / debugging too).
        for slot, base in [
            ("pre_ssa", PreSsaPass),
            ("pre_fixedpoint", PreFixedpointPass),
            ("fixedpoint", FixedpointPass),
            ("post_fixedpoint", PostFixedpointPass),
            ("post_destruction", PostDestructionPass),
            ("post_destruction_fixedpoint", PostDestructionFixedpointPass),
        ]:
            for p in getattr(self, slot):
                if not isinstance(p, base):
                    raise TypeError(
                        f"{slot} pass {type(p).__name__} doesn't inherit {base.__name__}"
                    )

    def apply(self, fn: tac_ast.Function, ctx: PassContext) -> tac_ast.Function:
        # Pre-SSA passes — run regardless of whether SSA construction will happen.
        for p in self.pre_ssa:
            fn = p.run(fn, ctx)
        if ctx.symbols is not None:
            fn, ctx.ssa_dsts = to_ssa(fn, ctx.symbols)
            for p in self.pre_fixedpoint:
                fn = p.run(fn, ctx)
        # Main fixedpoint.
        for _ in range(self.fixedpoint_cap):
            prev = fn
            for p in self.fixedpoint:
                fn = p.run(fn, ctx)
            if fn == prev:
                break
        if ctx.symbols is not None:
            for p in self.post_fixedpoint:
                fn = p.run(fn, ctx)
            fn = from_ssa(fn, symbols=ctx.symbols)
        # Post-destruction one-shots.
        for p in self.post_destruction:
            fn = p.run(fn, ctx)
        # Post-destruction fixedpoint.
        for _ in range(self.fixedpoint_cap):
            prev = fn
            for p in self.post_destruction_fixedpoint:
                fn = p.run(fn, ctx)
            if fn == prev:
                break
        return fn
