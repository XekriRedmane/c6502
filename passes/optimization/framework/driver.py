from __future__ import annotations
from dataclasses import dataclass, field
from typing import Callable, ClassVar, Sequence
import tac_ast
from passes.optimization.framework.base import Pass, PassContext
from passes.optimization.framework.phases import (
    PreSsaPass, PreFixedpointPass, FixedpointPass,
    PostFixedpointPass, PostDestructionPass, PostDestructionFixedpointPass,
)
from passes.optimization.ssa_construction import to_ssa
from passes.optimization.ssa_destruction import from_ssa
from passes import ir_check


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

    # Called as `observer(fn, after)` with the function after every
    # pass, and after SSA construction and destruction, when set.
    # `fuzz/tac_passes.py` simulates the program at each step to find
    # the pass that changes its result.
    observer: ClassVar[Callable[[tac_ast.Function, str], None] | None] = None

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
        # With `C6502_CHECK_IR=1`, the IR is checked after every pass
        # (passes/ir_check.py).
        checked = ir_check.enabled()

        def run(p, fn, in_ssa=False):
            fn = p.run(fn, ctx)
            if checked:
                ir_check.check_tac(
                    fn, after=type(p).__name__, symbols=ctx.symbols,
                    ssa_dsts=ctx.ssa_dsts if in_ssa else None,
                )
            if PhaseDriver.observer is not None:
                PhaseDriver.observer(fn, type(p).__name__)
            return fn

        if checked:
            ir_check.check_tac(fn, after="c99_to_tac", symbols=ctx.symbols)
        # Pre-SSA passes — run regardless of whether SSA construction will happen.
        for p in self.pre_ssa:
            fn = run(p, fn)
        if ctx.symbols is not None:
            fn, ctx.ssa_dsts = to_ssa(fn, ctx.symbols)
            if PhaseDriver.observer is not None:
                PhaseDriver.observer(fn, "to_ssa")
            if checked:
                ir_check.check_tac(
                    fn, after="to_ssa", symbols=ctx.symbols,
                    ssa_dsts=ctx.ssa_dsts,
                )
            for p in self.pre_fixedpoint:
                fn = run(p, fn, in_ssa=True)
        # Main fixedpoint.
        for _ in range(self.fixedpoint_cap):
            prev = fn
            for p in self.fixedpoint:
                fn = run(p, fn, in_ssa=ctx.symbols is not None)
            if fn == prev:
                break
        if ctx.symbols is not None:
            for p in self.post_fixedpoint:
                fn = run(p, fn, in_ssa=True)
            fn = from_ssa(fn, symbols=ctx.symbols)
            if PhaseDriver.observer is not None:
                PhaseDriver.observer(fn, "from_ssa")
            if checked:
                ir_check.check_tac(fn, after="from_ssa", symbols=ctx.symbols)
        # Post-destruction one-shots.
        for p in self.post_destruction:
            fn = run(p, fn)
        # Post-destruction fixedpoint.
        for _ in range(self.fixedpoint_cap):
            prev = fn
            for p in self.post_destruction_fixedpoint:
                fn = run(p, fn)
            if fn == prev:
                break
        return fn
