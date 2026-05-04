"""TAC SSA-aware copy propagation.

In SSA form, every Var is defined exactly once, so a `Copy(src, dst)`
asserts the equation `dst ≡ src` everywhere `dst` is used —
unconditionally, with no killing. Copy propagation rewrites every
use of `dst` to `src`. After propagation, the Copy itself becomes a
dead store (its dst no longer has any reads); dead-store elimination
picks it up on the next cycle.

Algorithm:
  1. Walk every instruction. For each `Copy(src, dst)` where `dst`
     is a `Var`, record the mapping `copy_src[dst.name] = src`.
     `Phi` is NOT a copy — it merges multiple values, not just one
     — so Phi dsts don't end up in the map.
  2. Resolve chains: for each `dst → src` entry, follow `src` if
     it's a Var and is itself in the map, until we hit a base
     value (a Constant, or a Var that isn't a Copy dst). SSA
     guarantees no cycles in the chain (each Var has one def, so
     `x ← y; y ← x` is unrepresentable).
  3. Walk every instruction again. For each Var operand, if its
     name is in the (chain-resolved) map, substitute the base
     value. Phi.source operands are rewritten the same way.

The pass is deliberately conservative: it doesn't propagate across
non-Copy defs (e.g., `Binary(Add, x, 0, y)` doesn't propagate y →
x, even though that fold is correct — that's constant-folding's
job). It only propagates Copies because those are the
unambiguous "x is exactly y" assertions in SSA.

This pass requires SSA form to be sound. In non-SSA TAC, a Copy's
dst can be reassigned later, breaking the `dst ≡ src` invariant.
The optimizer driver calls `copy_propagate` only inside the
SSA-in/de-SSA bracket, so the input is guaranteed to be SSA.
"""

from __future__ import annotations

import tac_ast


def copy_propagate(
    fn: tac_ast.Function,
    *,
    ssa_dsts: set[str] | None = None,
) -> tac_ast.Function:
    """Substitute every use of a Copy's dst with its (chain-resolved)
    src. Phi dsts are not propagated.

    `ssa_dsts` is the set of Var names that `to_ssa` minted —
    equivalently, the set of names whose only definition lives in
    this function and won't be re-defined. A Copy is only a
    propagation candidate if its dst is in `ssa_dsts`, because
    only those names obey the SSA single-def invariant. Static
    variables (`globl = ...`), address-taken locals, and any other
    non-promoted name can be re-written by a function call, a
    `Store`, or a sibling assignment, so propagating their Copy's
    src across uses would observe stale values.

    Without `ssa_dsts` (legacy / non-SSA caller), the pass is a
    no-op — there's no safe way to identify which Copies are SSA
    in that case."""
    if ssa_dsts is None:
        return fn
    copy_src = _collect_copy_sources(fn, ssa_dsts)
    if not copy_src:
        return fn
    resolved = _resolve_chains(copy_src)
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params),
        instructions=[_rewrite(i, resolved) for i in fn.instructions],
    )


def _collect_copy_sources(
    fn: tac_ast.Function, ssa_dsts: set[str],
) -> dict[str, tac_ast.Type_val]:
    """Build the `copy_src[dst_name] = src_val` map. A `Copy(src,
    dst)` contributes only when:

      * `dst` is a Var in `ssa_dsts` — only SSA-renamed names obey
        the single-def invariant; writes to statics or
        address-taken locals are observable elsewhere.
      * `src` is a Constant OR a Var that's also in `ssa_dsts`
        (or is a parameter's original-spelling SSA initial value).
        Reading a non-SSA name (static, address-taken local) gives
        a value at a specific program point — propagating that
        through to other reads is wrong, because the underlying
        memory can be written between this read and the next.

    Parameters' initial SSA value reuses the original spelling
    (e.g. `@0.p`) and `to_ssa` adds those names to `ssa_dsts` when
    the param is promotable (not address-taken), so the same
    `name in ssa_dsts` check covers both renamed-temp and
    param-stable cases uniformly."""
    out: dict[str, tac_ast.Type_val] = {}
    for instr in fn.instructions:
        if not isinstance(instr, tac_ast.Copy):
            continue
        if not isinstance(instr.dst, tac_ast.Var):
            continue
        if instr.dst.name not in ssa_dsts:
            continue
        src = instr.src
        if isinstance(src, tac_ast.Constant):
            out[instr.dst.name] = src
            continue
        if isinstance(src, tac_ast.Var) and src.name in ssa_dsts:
            out[instr.dst.name] = src
            continue
        # src is a non-SSA Var (static, address-taken local) — its
        # value can change between reads. Skip.
    return out


def _resolve_chains(
    copy_src: dict[str, tac_ast.Type_val],
) -> dict[str, tac_ast.Type_val]:
    """Follow each `dst → src` entry through any chain of further
    Copies until we reach a base value (Constant or non-Copy Var).
    SSA guarantees no cycles."""
    resolved: dict[str, tac_ast.Type_val] = {}
    for dst_name in copy_src:
        seen: set[str] = set()
        cur: tac_ast.Type_val = copy_src[dst_name]
        while isinstance(cur, tac_ast.Var) and cur.name in copy_src:
            if cur.name in seen:
                # Defensive — SSA shouldn't admit cycles, but if a
                # malformed input slips through, bail at the cycle
                # rather than spin.
                break
            seen.add(cur.name)
            cur = copy_src[cur.name]
        resolved[dst_name] = cur
    return resolved


def _rewrite(
    instr: tac_ast.Type_instruction,
    resolved: dict[str, tac_ast.Type_val],
) -> tac_ast.Type_instruction:
    """Return `instr` with every Var-use rewritten to its resolved
    base value. Var-defs are left alone (their names ARE the SSA
    identity we're propagating *from*)."""

    def sub(v: tac_ast.Type_val) -> tac_ast.Type_val:
        if isinstance(v, tac_ast.Var) and v.name in resolved:
            return resolved[v.name]
        return v

    match instr:
        case tac_ast.Ret(val=val):
            return tac_ast.Ret(val=sub(val) if val is not None else None)
        case tac_ast.SignExtend(src=s, dst=d):
            return tac_ast.SignExtend(src=sub(s), dst=d)
        case tac_ast.ZeroExtend(src=s, dst=d):
            return tac_ast.ZeroExtend(src=sub(s), dst=d)
        case tac_ast.Truncate(src=s, dst=d):
            return tac_ast.Truncate(src=sub(s), dst=d)
        case tac_ast.IntToFloat(src=s, dst=d):
            return tac_ast.IntToFloat(src=sub(s), dst=d)
        case tac_ast.IntToDouble(src=s, dst=d):
            return tac_ast.IntToDouble(src=sub(s), dst=d)
        case tac_ast.FloatToInt(src=s, dst=d):
            return tac_ast.FloatToInt(src=sub(s), dst=d)
        case tac_ast.DoubleToInt(src=s, dst=d):
            return tac_ast.DoubleToInt(src=sub(s), dst=d)
        case tac_ast.FloatToDouble(src=s, dst=d):
            return tac_ast.FloatToDouble(src=sub(s), dst=d)
        case tac_ast.DoubleToFloat(src=s, dst=d):
            return tac_ast.DoubleToFloat(src=sub(s), dst=d)
        case tac_ast.Unary(op=op, src=s, dst=d):
            return tac_ast.Unary(op=op, src=sub(s), dst=d)
        case tac_ast.Binary(op=op, src1=s1, src2=s2, dst=d):
            return tac_ast.Binary(
                op=op, src1=sub(s1), src2=sub(s2), dst=d,
            )
        case tac_ast.Copy(src=s, dst=d):
            return tac_ast.Copy(src=sub(s), dst=d)
        case tac_ast.GetAddress(operand=o, dst=d):
            # GetAddress reads a name (storage cell), not a value.
            # Don't substitute — even if `o` happens to be a Copy
            # dst, the operand here is the storage location's name,
            # which can't be replaced with a Constant anyway.
            return instr
        case tac_ast.Load(src_ptr=p, dst=d):
            return tac_ast.Load(src_ptr=sub(p), dst=d)
        case tac_ast.Store(src=s, dst_ptr=p):
            return tac_ast.Store(src=sub(s), dst_ptr=sub(p))
        case tac_ast.IndexedLoad(name=n, index=i, dst=d):
            return tac_ast.IndexedLoad(name=n, index=sub(i), dst=d)
        case tac_ast.JumpIfTrue(condition=c, target=t):
            return tac_ast.JumpIfTrue(condition=sub(c), target=t)
        case tac_ast.JumpIfFalse(condition=c, target=t):
            return tac_ast.JumpIfFalse(condition=sub(c), target=t)
        case tac_ast.JumpIfCmp(op=op, src1=s1, src2=s2, target=t):
            return tac_ast.JumpIfCmp(
                op=op, src1=sub(s1), src2=sub(s2), target=t,
            )
        case tac_ast.FunctionCall(name=n, args=args, dst=d):
            return tac_ast.FunctionCall(
                name=n, args=[sub(a) for a in args], dst=d,
            )
        case tac_ast.IndirectCall(ptr=p, args=args, dst=d):
            return tac_ast.IndirectCall(
                ptr=sub(p), args=[sub(a) for a in args], dst=d,
            )
        case tac_ast.Phi(dst=d, args=args):
            return tac_ast.Phi(
                dst=d,
                args=[
                    tac_ast.PhiArg(
                        pred_label=a.pred_label, source=sub(a.source),
                    )
                    for a in args
                ],
            )
    return instr
