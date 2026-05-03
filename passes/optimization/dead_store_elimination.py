"""TAC SSA-aware dead-store elimination.

In SSA form, every Var has exactly one definition, so a definition's
liveness reduces to "is this Var named anywhere as a use?". DSE
walks the function:

  1. Collect `used` = set of Var names appearing as a use anywhere
     (Phi sources, instruction operands, return values, call args).
  2. Drop or simplify each instruction whose dst is unused:
     - Pure defs (`Copy`, `Binary`, `Unary`, the eight cast / FP-
       conversion variants, `Load`, `GetAddress`, `Phi`) are
       discarded entirely.
     - `FunctionCall` / `IndirectCall` retain the call (function
       execution is observable — globals, I/O, ...) but drop
       `dst` to None, signalling that the return value is ignored.
  3. After dropping defs, the inputs to those defs are no longer
     used. We re-collect `used` and iterate to fixed point.

Instructions kept regardless of `used`:
  - `Store` writes through a pointer to memory; the stored byte may
    be read by some later Load that we can't see in this function
    (could be aliased, could span the lifetime of an address-taken
    local). Always keep.
  - `Ret` is a control-flow terminator with the return value as
    payload — never dead.
  - `Jump` / `JumpIfTrue` / `JumpIfFalse` / `Label` carry no Var
    def, so they're irrelevant here (they aren't candidates for
    DSE in the first place).
  - `FunctionCall` and `IndirectCall` keep their side effects;
    only the return-value capture goes away.

This pass requires SSA form to be sound. In non-SSA TAC, a "dead"
def by name might actually be live because a later instruction
overwrites it before any read — the standard non-SSA liveness
analysis is needed there. The optimizer driver calls
`eliminate_dead_stores` only inside the SSA-in/de-SSA bracket, so
the input is guaranteed to be SSA.

Loads aren't preserved by side effect — c6502's "memory" is just
RAM, so a load through a wild pointer doesn't trap and removing a
load with a dead dst is observably equivalent to keeping it. This
is more aggressive than LLVM's load-may-trap convention but matches
the actual hardware behavior of the 6502 target.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.var_visit import uses_in


# Instructions kept regardless of dst liveness — they have observable
# effects beyond their (possibly-absent) dst Var.
_SIDE_EFFECTING_TYPES: tuple[type, ...] = (
    tac_ast.Store, tac_ast.Ret,
    tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
    tac_ast.Label,
)

# Pure-def instructions: the entire instruction can be removed if
# `dst` is unused.
_PURE_DEF_TYPES: tuple[type, ...] = (
    tac_ast.Copy,
    tac_ast.Unary, tac_ast.Binary,
    tac_ast.SignExtend, tac_ast.ZeroExtend, tac_ast.Truncate,
    tac_ast.IntToFloat, tac_ast.IntToDouble,
    tac_ast.FloatToInt, tac_ast.DoubleToInt,
    tac_ast.FloatToDouble, tac_ast.DoubleToFloat,
    tac_ast.Load, tac_ast.GetAddress,
    tac_ast.Phi,
)


def eliminate_dead_stores(
    fn: tac_ast.Function,
    *,
    ssa_dsts: set[str] | None = None,
) -> tac_ast.Function:
    """Iteratively drop pure defs whose dst is never used. Function
    calls with unused dst lose their dst (call kept). Stores, Rets,
    and control flow are always kept.

    `ssa_dsts` is the set of Var names `to_ssa` minted —
    equivalently, the set of names whose only definition lives in
    this function. We only drop a def if its dst is in `ssa_dsts`,
    because only those names obey the SSA single-def invariant.
    A write to a static variable (`globl = 4`), an address-taken
    local, or any aggregate whose name isn't SSA-renamed is
    observably effectful (other functions / aliasing pointers /
    later writes can read it back), so we keep it regardless of
    in-function uses.

    Without `ssa_dsts` (legacy / non-SSA caller), the pass is a
    no-op — there's no safe way to know which dsts are local in
    that case."""
    if ssa_dsts is None:
        return fn
    instrs = list(fn.instructions)
    while True:
        used = _collect_uses(instrs)
        new_instrs: list[tac_ast.Type_instruction] = []
        changed = False
        for instr in instrs:
            kept, was_changed = _filter(instr, used, ssa_dsts)
            if was_changed:
                changed = True
            if kept is not None:
                new_instrs.append(kept)
        if not changed:
            return tac_ast.Function(
                name=fn.name, is_global=fn.is_global,
                params=list(fn.params), instructions=new_instrs,
            )
        instrs = new_instrs


def _filter(
    instr: tac_ast.Type_instruction,
    used: set[str],
    ssa_dsts: set[str],
) -> tuple[tac_ast.Type_instruction | None, bool]:
    """Return `(kept_instr, changed)`. `kept_instr` is the new form
    (None to drop), and `changed` is True if we modified or dropped
    `instr`."""
    if isinstance(instr, _SIDE_EFFECTING_TYPES):
        return instr, False

    if isinstance(instr, _PURE_DEF_TYPES):
        dst = instr.dst
        if not isinstance(dst, tac_ast.Var):
            return instr, False
        if dst.name not in ssa_dsts:
            # Writes to non-SSA names (statics, address-taken
            # locals, aggregates) may be observed elsewhere; keep.
            return instr, False
        if dst.name in used:
            return instr, False
        return None, True

    if isinstance(instr, (tac_ast.FunctionCall, tac_ast.IndirectCall)):
        if instr.dst is None:
            return instr, False
        if not isinstance(instr.dst, tac_ast.Var):
            return instr, False
        if instr.dst.name not in ssa_dsts:
            return instr, False
        if instr.dst.name in used:
            return instr, False
        # Drop the dst but keep the call — function execution may
        # have observable effects.
        if isinstance(instr, tac_ast.FunctionCall):
            return tac_ast.FunctionCall(
                name=instr.name, args=list(instr.args), dst=None,
            ), True
        return tac_ast.IndirectCall(
            ptr=instr.ptr, args=list(instr.args), dst=None,
        ), True

    return instr, False


def _collect_uses(
    instrs: list[tac_ast.Type_instruction],
) -> set[str]:
    """Set of Var names read anywhere in `instrs`. Delegates to the
    shared `uses_in` walker."""
    used: set[str] = set()
    for instr in instrs:
        for v in uses_in(instr):
            used.add(v.name)
    return used
