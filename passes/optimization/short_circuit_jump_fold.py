"""TAC pass: fold the short-circuit `&&` / `||` 0-or-1 materialize
tail + adjacent JumpIf consumer into direct conditional branches.

# Motivating idiom

C99 `if (a && b) ...` lowers through `c99_to_tac.translate_short_
circuit` (and survives SSA destruction) as:

    JumpIfFalse(a, .and_false@N)
    JumpIfFalse(b, .and_false@N)
    Copy(1, %t)
    Jump(.and_end@N)
    Label(.and_false@N)
    Copy(0, %t)
    Label(.and_end@N)
    JumpIfFalse(%t, .if_end@M)

The five-instruction tail (the two Copies, the in-between Jump, and
the two Labels) materializes a Boolean 0 or 1 into `%t`; the
immediately-following consumer re-tests that value. The Boolean is
invisible at the C level — every short-circuit jump in the chain
already controls flow the same way the consumer's branch will. So
the asm lowering ends up emitting two layers of `BEQ`/`BNE` for one
logical decision:

    BCC .and_false@N        ; chain jump 1
    BCS .and_false@N        ; chain jump 2
    LDA #$01                ; Copy(1, %t)
    STA __local_t
    JMP .and_end@N
    .and_false@N:
    LDA #$00                ; Copy(0, %t)
    STA __local_t
    .and_end@N:
    LDA __local_t           ; consumer LDA
    BEQ .if_end@M           ; consumer branch

The companion case (`||` with `JumpIfFalse`, `&&` with `JumpIfTrue`,
`||` with `JumpIfTrue`) has the same shape with different
constants and a different consumer kind. This pass collapses all
four variants to a single round of branches that target the
consumer's destination directly.

# What this pass does

When `instrs[i:i+5]` matches the canonical tail:

    Copy(C_ft, %t)
    Jump(end_label)
    Label(branch_label)
    Copy(C_sc, %t)
    Label(end_label)

with `(C_ft, C_sc)` being `(ConstInt 1, ConstInt 0)` or
`(ConstInt 0, ConstInt 1)`, immediately followed by a
`JumpIf{True,False}(%t, T)` consumer, the pass derives two
destinations from `(C_ft, C_sc, consumer.kind)`:

  D_sc = where control should go when the chain fired
         (i.e. when `%t == C_sc` reaches the consumer).
  D_ft = where control should go when no chain jump fired
         (i.e. when `%t == C_ft` reaches the consumer).

Each is either `T` (the consumer's target) or "next" (the
instruction immediately after the consumer in the function), since
the consumer is a single conditional jump.

  - Case A (D_sc == T, D_ft == next): retarget every Jump /
    JumpIf{True,False,Cmp,Masked} that targets `branch_label`
    to `T`, and delete the six-instruction tail+consumer.

  - Case B (D_sc == next, D_ft == T): mint a fresh
    `.<funcname>@scfold@<N>` label, retarget the chain to it, and
    replace the six-instruction tail+consumer with
    `Jump(T); Label(.<funcname>@scfold@<N>)`. The synthetic Jump
    routes the fall-through (no-chain-fired) case to T.

# Width-agnostic

The chain's short-circuit jumps already test the original operands
at their declared width — `JumpIf{True,False}` and `JumpIfCmp`'s
lowerings walk every byte (ORA chain, per-byte CMP/SBC) and branch
once. The fold preserves that behavior; nothing about the rewrite
depends on `%t`'s width.

# Soundness gates

  - The two Copies write to the same `%t`.
  - The in-tail `Jump`'s target matches the second `Label`'s name.
  - `(C_ft, C_sc)` are `ConstInt 0` and `ConstInt 1` (in either
    order) — the canonical short-circuit shape.
  - `%t` is used exactly once across the function (the consumer).
    Any other reader would observe the 0/1 value we're dropping.
  - `end_label` is jumped to exactly once (by the in-tail Jump).
    Any other reference would dangle after the deletion.
  - The consumer is `JumpIfTrue(%t, T)` or `JumpIfFalse(%t, T)`
    directly on `%t` (no Cast, no intervening op).

`branch_label` is allowed to have any number of jump references —
those ARE the chain we're folding. Retargeting them is sound for
each: jumping to `branch_label` currently executes
`Copy(C_sc, %t); fall through; consumer routes per C_sc`;
post-rewrite the same jump goes directly to D_sc, the same final
destination (with %t's dead write dropped).

# Nested patterns

`(a && b) || c` and similar produce two foldable patterns whose
temps don't overlap. The single sweep below catches all
non-overlapping patterns at once. Patterns that become foldable
only AFTER an earlier rewrite (very rare in this shape — the
consumer's identity doesn't depend on its target) would need
another sweep; the driver re-runs until convergence.

The post-SSA-destruction pipeline position is deliberate: pre-SSA-
destruction the tail is split across two SSA-renamed defs of `%t`
merged by a Phi, which complicates pattern matching. Post-SSA-
destruction the fold_copies pass has already collapsed the
per-arm Copy chains into the canonical 5-instruction tail above.
"""
from __future__ import annotations

from typing import Iterable

import tac_ast


def fold_short_circuit_jump(
    fn: tac_ast.Function,
    *,
    symbols=None,
) -> tac_ast.Function:
    """Walk `fn.instructions` once, identify every non-overlapping
    short-circuit tail + consumer pattern, and rewrite them in place.
    Returns a new Function if anything changed; the same `fn`
    otherwise. `symbols` is accepted for signature uniformity with
    other folds in the optimizer driver."""
    del symbols
    use_count = _count_var_uses(fn)
    jump_target_count = _count_jump_targets(fn)
    existing_labels = {
        i.name for i in fn.instructions if isinstance(i, tac_ast.Label)
    }
    label_counter = 0

    retarget_map: dict[str, str] = {}
    new_instrs: list[tac_ast.Type_instruction] = []
    i = 0
    while i < len(fn.instructions):
        fold = _try_fold(fn, i, use_count, jump_target_count)
        if fold is None:
            new_instrs.append(fn.instructions[i])
            i += 1
            continue
        kind, branch_label, target = fold
        if kind == "natural":
            retarget_map[branch_label] = target
        else:
            while True:
                new_label = f".{fn.name}@scfold@{label_counter}"
                label_counter += 1
                if new_label not in existing_labels:
                    existing_labels.add(new_label)
                    break
            retarget_map[branch_label] = new_label
            new_instrs.append(tac_ast.Jump(target=target))
            new_instrs.append(tac_ast.Label(name=new_label))
        i += 6  # consume tail (5) + consumer (1)

    if not retarget_map:
        return fn

    # Transitive closure: nested short-circuits chain through a
    # shared label — the inner fold retargets its branch_label to
    # the outer's branch_label, which the outer fold has ALSO
    # retargeted onward. A single substitution would leave the
    # inner chain pointing at a label that's about to be deleted;
    # follow the chain until it lands on a non-key.
    resolved_map: dict[str, str] = {}
    for key in retarget_map:
        target = retarget_map[key]
        seen = {key}
        while target in retarget_map and target not in seen:
            seen.add(target)
            target = retarget_map[target]
        resolved_map[key] = target
    new_instrs = [
        _retarget_instruction(instr, resolved_map) for instr in new_instrs
    ]
    return tac_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=new_instrs,
    )


def _try_fold(
    fn: tac_ast.Function,
    i: int,
    use_count: dict[str, int],
    jump_target_count: dict[str, int],
) -> tuple[str, str, str] | None:
    """If `fn.instructions[i:i+6]` is `Copy / Jump / Label / Copy /
    Label / JumpIf{True,False}` in the short-circuit shape, return
    `(kind, branch_label, T)` where `kind` is `"natural"` or
    `"flipped"`. Otherwise None."""
    instrs = fn.instructions
    if i + 5 >= len(instrs):
        return None
    cp_ft = instrs[i]
    jump_in_tail = instrs[i + 1]
    label_branch = instrs[i + 2]
    cp_sc = instrs[i + 3]
    label_end = instrs[i + 4]
    consumer = instrs[i + 5]

    if not isinstance(cp_ft, tac_ast.Copy):
        return None
    if not isinstance(jump_in_tail, tac_ast.Jump):
        return None
    if not isinstance(label_branch, tac_ast.Label):
        return None
    if not isinstance(cp_sc, tac_ast.Copy):
        return None
    if not isinstance(label_end, tac_ast.Label):
        return None
    if not isinstance(consumer, (tac_ast.JumpIfTrue, tac_ast.JumpIfFalse)):
        return None

    if not isinstance(cp_ft.dst, tac_ast.Var):
        return None
    if not isinstance(cp_sc.dst, tac_ast.Var):
        return None
    if cp_ft.dst.name != cp_sc.dst.name:
        return None
    t_name = cp_ft.dst.name

    c_ft = _const_int_value(cp_ft.src)
    c_sc = _const_int_value(cp_sc.src)
    if c_ft is None or c_sc is None:
        return None
    if {c_ft, c_sc} != {0, 1}:
        return None

    end_label = label_end.name
    if jump_in_tail.target != end_label:
        return None
    branch_label = label_branch.name

    if not isinstance(consumer.condition, tac_ast.Var):
        return None
    if consumer.condition.name != t_name:
        return None

    if use_count.get(t_name, 0) != 1:
        return None
    if jump_target_count.get(end_label, 0) != 1:
        return None

    consumer_branch_value = (
        1 if isinstance(consumer, tac_ast.JumpIfTrue) else 0
    )
    target = consumer.target
    if c_sc == consumer_branch_value:
        return ("natural", branch_label, target)
    return ("flipped", branch_label, target)


def _retarget_instruction(
    instr: tac_ast.Type_instruction, retarget_map: dict[str, str],
) -> tac_ast.Type_instruction:
    """Return a copy of `instr` with any jump-target field whose
    value is a key in `retarget_map` rewritten to the mapped value.
    Non-jump instructions pass through unchanged."""
    match instr:
        case tac_ast.Jump(target=t) if t in retarget_map:
            return tac_ast.Jump(target=retarget_map[t])
        case tac_ast.JumpIfTrue(condition=c, target=t) if t in retarget_map:
            return tac_ast.JumpIfTrue(condition=c, target=retarget_map[t])
        case tac_ast.JumpIfFalse(condition=c, target=t) if t in retarget_map:
            return tac_ast.JumpIfFalse(condition=c, target=retarget_map[t])
        case tac_ast.JumpIfCmp(
            op=op, src1=s1, src2=s2, target=t,
        ) if t in retarget_map:
            return tac_ast.JumpIfCmp(
                op=op, src1=s1, src2=s2, target=retarget_map[t],
            )
        case tac_ast.JumpIfMasked(
            val=v, mask=m, jump_when_nonzero=j, target=t,
        ) if t in retarget_map:
            return tac_ast.JumpIfMasked(
                val=v, mask=m, jump_when_nonzero=j,
                target=retarget_map[t],
            )
    return instr


def _const_int_value(val) -> int | None:
    """If `val` is a `Constant` wrapping a `ConstInt`, return its
    value; else None. Restricting to `ConstInt` matches the canonical
    short-circuit lowering (`_tac_const_val(c99_ast.Int(), 0|1)`)."""
    if not isinstance(val, tac_ast.Constant):
        return None
    if not isinstance(val.const, tac_ast.ConstInt):
        return None
    return val.const.value


def _count_jump_targets(fn: tac_ast.Function) -> dict[str, int]:
    """Count how many instructions target each label name (across
    every conditional / unconditional jump variant)."""
    out: dict[str, int] = {}
    for instr in fn.instructions:
        target = _jump_target_of(instr)
        if target is not None:
            out[target] = out.get(target, 0) + 1
    return out


def _jump_target_of(instr) -> str | None:
    if isinstance(instr, (
        tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
        tac_ast.JumpIfCmp, tac_ast.JumpIfMasked,
    )):
        return instr.target
    return None


def _count_var_uses(fn: tac_ast.Function) -> dict[str, int]:
    """Count `Var` uses by name across the whole function. Mirrors
    the use-site enumeration in
    `cmp_zero_jump_fold` / `lnot_jump_fold`."""
    out: dict[str, int] = {}
    for instr in fn.instructions:
        for v in _vars_used_in(instr):
            out[v.name] = out.get(v.name, 0) + 1
    return out


def _vars_used_in(instr) -> Iterable[tac_ast.Var]:
    """Yield every `Var` read by `instr`. Must enumerate every TAC
    variant — missing one would silently inflate or deflate the
    use_count for a candidate temp and either over- or under-fire
    the gate. Kept in sync with the sibling fold passes'
    `_vars_used_in`."""
    match instr:
        case tac_ast.Ret(val=val):
            if isinstance(val, tac_ast.Var):
                yield val
        case tac_ast.SignExtend(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.ZeroExtend(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.Truncate(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.IntToFloat(src=s) | tac_ast.IntToDouble(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.FloatToInt(src=s) | tac_ast.DoubleToInt(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.FloatToDouble(src=s) | tac_ast.DoubleToFloat(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.GetAddress():
            return
        case tac_ast.Load(src_ptr=p):
            if isinstance(p, tac_ast.Var):
                yield p
        case tac_ast.Store(src=s, dst_ptr=p):
            if isinstance(s, tac_ast.Var):
                yield s
            if isinstance(p, tac_ast.Var):
                yield p
        case tac_ast.IndexedLoad(index=idx):
            if isinstance(idx, tac_ast.Var):
                yield idx
        case tac_ast.IndexedStore(index=idx, src=s):
            if isinstance(idx, tac_ast.Var):
                yield idx
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.IndexedSymbolStore(index=idx, src=s):
            if isinstance(idx, tac_ast.Var):
                yield idx
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.IndexedConstLoad(index=idx):
            if isinstance(idx, tac_ast.Var):
                yield idx
        case tac_ast.IndirectIndexedLoad(ptr=p, index=idx):
            if isinstance(p, tac_ast.Var):
                yield p
            if isinstance(idx, tac_ast.Var):
                yield idx
        case tac_ast.IndirectIndexedStore(ptr=p, index=idx, src=s):
            if isinstance(p, tac_ast.Var):
                yield p
            if isinstance(idx, tac_ast.Var):
                yield idx
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.Unary(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.Binary(src1=s1, src2=s2):
            if isinstance(s1, tac_ast.Var):
                yield s1
            if isinstance(s2, tac_ast.Var):
                yield s2
        case tac_ast.Copy(src=s):
            if isinstance(s, tac_ast.Var):
                yield s
        case tac_ast.JumpIfTrue(condition=c) | tac_ast.JumpIfFalse(condition=c):
            if isinstance(c, tac_ast.Var):
                yield c
        case tac_ast.JumpIfCmp(src1=s1, src2=s2):
            if isinstance(s1, tac_ast.Var):
                yield s1
            if isinstance(s2, tac_ast.Var):
                yield s2
        case tac_ast.JumpIfMasked(val=v):
            if isinstance(v, tac_ast.Var):
                yield v
        case tac_ast.FunctionCall(args=args):
            for a in args:
                if isinstance(a, tac_ast.Var):
                    yield a
        case tac_ast.IndirectCall(ptr=p, args=args):
            if isinstance(p, tac_ast.Var):
                yield p
            for a in args:
                if isinstance(a, tac_ast.Var):
                    yield a
        case tac_ast.Phi(args=args):
            for a in args:
                if isinstance(a.source, tac_ast.Var):
                    yield a.source
