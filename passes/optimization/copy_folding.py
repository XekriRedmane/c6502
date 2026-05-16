"""Copy folding — fuse `<producer dst=%t>; Copy(%t, X)` pairs.

`tac_to_asm` lowers `x += 1` (where x is a non-SSA-promoted name —
a static or address-taken local) as

    Binary(Add, x, 1, %t)
    Copy(%t, x)

The temp `%t` exists only because every TAC operation writes to a
single dst by convention; `%t` is never written or read again
beyond the Copy. At asm level, the Copy lowers to N extra Mov
atoms (`LDA %t.lo; STA x.lo; LDA %t.hi; STA x.hi; ...`) — typically
4 instructions / 13 bytes / 14 cycles for a 16-bit value, more if
%t spilled to Frame.

This pass eliminates the temp by redirecting the producer's dst
to `X` directly:

    Binary(Add, x, 1, x)

Same observable behavior, no temp. The fused instruction is an
in-place RMW on `x`, which then composes with the multi-byte INC
peephole (`passes/inc_peephole.py`) to collapse the per-byte ADC
chain into `INC + BNE` for `Data` / `ZP` `x` operands.

# What we fuse

Any TAC instruction with a single dst-position whose dst is a Var
(call it `%t`) followed by `Copy(%t, X)` where `%t` has exactly
one use in the function (the Copy itself). The redirect rewrites
the producer's dst from `%t` to `X` and drops the Copy.

Eligible producers: SignExtend, ZeroExtend, Truncate, the six
FP-conversion casts, Unary, Binary, Copy (chained-copy
elimination), GetAddress, Load, IndexedLoad, FunctionCall (when
its dst is non-None), IndirectCall (same).

Phi is deliberately excluded — Phi.dst is always an SSA-renamed
name in the IR shape this pass sees (between to_ssa and from_ssa),
and SSA construction's invariants (every Phi.dst has one def =
the Phi) want it to stay that way until SSA destruction. Copy-
propagation + DSE already eliminate `Phi(...) → %t; Copy(%t, %r)`
patterns where `%r` is renamed; we don't need a separate path.

# When it fires

Inside the TAC optimizer's fixed-point loop, after copy_propagation
and dead_store_elimination — same level as the rest of the SSA-
aware passes. The new fusion can be enabled by:
  * Strength reduction rewriting Multiply → LeftShift (the Shift
    feeds the same Copy as the original Multiply did).
  * Constant folding folding a Cast (the leftover Copy is
    foldable).
  * Copy-prop + DSE clearing intermediate temps on a chain.

# Soundness

The fusion is sound when:

  * `%t`'s def is the producer being fused (SSA guarantees this
    when `%t` is renamed; for non-renamed `%t`, see the use-count
    check).
  * `%t` has exactly one use in the entire function (the Copy).
    This guarantees no other reader observes `%t`'s value, so
    redirecting the producer's dst doesn't break any other use.
  * The Copy is the immediately-next instruction after the
    producer. With adjacency there's no intervening op that could
    write `X` (and have its write clobbered by the Copy in the
    unfused version) or read `X` (and see different values pre-
    fusion vs post-fusion).

The use-count check makes this safe even outside SSA: if `%t` had
multiple defs but only one use (the Copy), redirecting the
last-before-Copy def to `X` leaves the other defs writing to a
name that's no longer read — DSE picks them up next iteration of
the fixed-point loop.

`X` doesn't need to be SSA-renamed; the fusion handles both
cases. SSA-renamed `X` already gets handled by copy_propagation
+ DSE, so this pass's unique contribution is the non-renamed case
(the static / address-taken-local pattern that couldn't otherwise
be cleaned up).
"""

from __future__ import annotations

from collections import Counter

import tac_ast
from passes.optimization.var_visit import uses_in


def fold_copies(fn: tac_ast.Function) -> tac_ast.Function:
    """Walk the function once, fuse adjacent `producer; Copy` pairs
    where the producer's dst is a single-use Pseudo. Returns a new
    Function with the fusions applied. Pure: doesn't mutate `fn`."""
    use_counts = _count_uses(fn.instructions)
    new_instrs: list[tac_ast.Type_instruction] = []
    i = 0
    while i < len(fn.instructions):
        curr = fn.instructions[i]
        if i + 1 < len(fn.instructions):
            nxt = fn.instructions[i + 1]
            fused = _try_fuse(curr, nxt, use_counts)
            if fused is not None:
                new_instrs.append(fused)
                i += 2
                continue
        new_instrs.append(curr)
        i += 1
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=new_instrs,
    )


def _count_uses(
    instrs: list[tac_ast.Type_instruction],
) -> Counter[str]:
    """Count how many times each Var name appears in a USE position
    across `instrs`. The Copy's `src` counts as one use of its name;
    multiple-use names are excluded from fusion."""
    counts: Counter[str] = Counter()
    for instr in instrs:
        for v in uses_in(instr):
            counts[v.name] += 1
    return counts


def _try_fuse(
    producer: tac_ast.Type_instruction,
    consumer: tac_ast.Type_instruction,
    use_counts: Counter[str],
) -> tac_ast.Type_instruction | None:
    """If `consumer` is `Copy(%t, X)`, and `producer` has a single
    dst-position Var named `%t`, and `%t` is used exactly once in
    the function (this very Copy), return the producer with its
    dst redirected to `X`. Otherwise None."""
    if not isinstance(consumer, tac_ast.Copy):
        return None
    if not isinstance(consumer.src, tac_ast.Var):
        return None
    src_name = consumer.src.name
    if use_counts.get(src_name, 0) != 1:
        return None
    return _redirect_dst(producer, src_name, consumer.dst)


def _redirect_dst(
    producer: tac_ast.Type_instruction,
    dst_name: str,
    new_dst: tac_ast.Type_val,
) -> tac_ast.Type_instruction | None:
    """If `producer` is one of the redirectable single-dst
    instruction kinds and its dst is `Var(dst_name)`, return a copy
    of `producer` with `dst` replaced by `new_dst`. Otherwise None.

    Phi is intentionally NOT redirectable — Phi.dst is always an
    SSA-renamed name in this pipeline and copy-prop + DSE handle
    Phi cleanup."""
    def is_target(d: tac_ast.Type_val) -> bool:
        return isinstance(d, tac_ast.Var) and d.name == dst_name

    match producer:
        case tac_ast.SignExtend(src=s, dst=d) if is_target(d):
            return tac_ast.SignExtend(src=s, dst=new_dst)
        case tac_ast.ZeroExtend(src=s, dst=d) if is_target(d):
            return tac_ast.ZeroExtend(src=s, dst=new_dst)
        case tac_ast.Truncate(src=s, dst=d) if is_target(d):
            return tac_ast.Truncate(src=s, dst=new_dst)
        case tac_ast.IntToFloat(src=s, dst=d) if is_target(d):
            return tac_ast.IntToFloat(src=s, dst=new_dst)
        case tac_ast.IntToDouble(src=s, dst=d) if is_target(d):
            return tac_ast.IntToDouble(src=s, dst=new_dst)
        case tac_ast.FloatToInt(src=s, dst=d) if is_target(d):
            return tac_ast.FloatToInt(src=s, dst=new_dst)
        case tac_ast.DoubleToInt(src=s, dst=d) if is_target(d):
            return tac_ast.DoubleToInt(src=s, dst=new_dst)
        case tac_ast.FloatToDouble(src=s, dst=d) if is_target(d):
            return tac_ast.FloatToDouble(src=s, dst=new_dst)
        case tac_ast.DoubleToFloat(src=s, dst=d) if is_target(d):
            return tac_ast.DoubleToFloat(src=s, dst=new_dst)
        case tac_ast.Unary(op=op, src=s, dst=d) if is_target(d):
            return tac_ast.Unary(op=op, src=s, dst=new_dst)
        case tac_ast.Binary(
            op=op, src1=s1, src2=s2, dst=d,
        ) if is_target(d):
            return tac_ast.Binary(
                op=op, src1=s1, src2=s2, dst=new_dst,
            )
        case tac_ast.Copy(src=s, dst=d) if is_target(d):
            return tac_ast.Copy(src=s, dst=new_dst)
        case tac_ast.GetAddress(operand=o, dst=d) if is_target(d):
            return tac_ast.GetAddress(operand=o, dst=new_dst)
        case tac_ast.Load(src_ptr=p, dst=d, is_volatile=v) if is_target(d):
            return tac_ast.Load(src_ptr=p, dst=new_dst, is_volatile=v)
        case tac_ast.IndexedLoad(
            name=n, index=idx, dst=d, is_volatile=v,
        ) if is_target(d):
            return tac_ast.IndexedLoad(
                name=n, index=idx, dst=new_dst, is_volatile=v,
            )
        case tac_ast.FunctionCall(
            name=n, args=args, dst=d,
        ) if d is not None and is_target(d):
            return tac_ast.FunctionCall(
                name=n, args=args, dst=new_dst,
            )
        case tac_ast.IndirectCall(
            ptr=p, args=args, dst=d,
        ) if d is not None and is_target(d):
            return tac_ast.IndirectCall(
                ptr=p, args=args, dst=new_dst,
            )
    return None
