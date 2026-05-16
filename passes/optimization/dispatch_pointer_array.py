"""Inline-switch dispatch for `static const T * const arr[N]` subscript chains.

When `arr` is a small file-scope `static const T * const[N]` whose
elements are all `AddressInit(some_named_static)`, and the program
reads `arr[i][j]` for runtime `i` and `j`, the generic c99_to_tac
lowering builds an indirect:

    %scaled    = Binary(LeftShift, %i, 1, _)        ; i * 2
    %ptr       = IndexedLoad(arr, %scaled, _)       ; 2-byte pointer load
    %val       = IndirectIndexedLoad(%ptr, %j, _)   ; deref *(ptr + j)

That lowers to `LDA arr,X; STA dptr; LDA arr+1,X; STA dptr+1;
LDA (dptr),Y` (5 instructions) plus the `i*2` scaling — and
critically, ties up X (the abs,X index), Y (the (zp),Y offset),
AND DPTR (the indirect base) at once.

This pass recognizes the chain and rewrites it to a CMP/BEQ
dispatch on `i`:

    JumpIfCmp(Equal, %i, ConstChar(0), case_0)
    JumpIfCmp(Equal, %i, ConstChar(1), case_1)
    ... (N-1 checks) ...
    ; fallthrough is case_{N-1}
    IndexedLoad(target_{N-1}, %j, %val)
    Jump(end)
    Label(case_0):
    IndexedLoad(target_0, %j, %val)
    Jump(end)
    ... (one block per case) ...
    Label(end):

Each case-arm is a single direct array load — no DPTR staging,
no pointer indirection. The dispatch frees X and Y from the
dual-index conflict; the loop counter (if any) can stay in X
across the dispatch.

# Eligibility

A chain is recognized when:

  * `IndirectIndexedLoad(ptr=Var(%P), index=Var(%J), dst=Var(%V))`
    appears in the function.
  * The latest in-order def of `%P` is `IndexedLoad(name=arr,
    index=Var(%I_scaled), dst=Var(%P))`, and `%P` has exactly
    that one def and one use.
  * The latest in-order def of `%I_scaled` is `Binary(LeftShift,
    Var(%I), Constant(1), Var(%I_scaled))` or `Binary(Multiply,
    Var(%I), Constant(2), Var(%I_scaled))`, and `%I_scaled` is
    single-def / single-use.
  * `arr` resolves to a `StaticVariable` whose `init` is a list
    of `AddressInit(target_k, offset=0)` entries.
  * The list length N is at most `_DISPATCH_THRESHOLD` (8 today).

# Why post-from_ssa

This pass runs once, after `from_ssa`, so each case-arm can
write to the same dst `%V` without inserting Phis. The downstream
code reads `%V` unchanged.

# Single-use constraint

The chain instructions are deleted from the IR; if `%I_scaled`
or `%P` had other uses, the rewrite would be unsound. The
eligibility check enforces single-use on both.
"""

from __future__ import annotations

import tac_ast


# Maximum number of cases to dispatch inline. 8 covers every
# realistic small-table case in c6502's corpus today; larger
# tables (16+) are rare in our model and the dispatch chain
# would dominate the access cost.
_DISPATCH_THRESHOLD = 8


def dispatch_const_pointer_arrays(
    prog: tac_ast.Program, symbols=None,
) -> tac_ast.Program:
    """Top-level entry. Builds the pointer-array map from the
    program's StaticVariables, then rewrites each Function."""
    pointer_arrays = _collect_pointer_arrays(prog)
    if not pointer_arrays:
        return prog
    label_counter = [0]
    new_top: list[tac_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function):
            new_top.append(
                _rewrite_function(tl, pointer_arrays, label_counter)
            )
        else:
            new_top.append(tl)
    return tac_ast.Program(top_level=new_top)


def _collect_pointer_arrays(
    prog: tac_ast.Program,
) -> dict[str, list[str]]:
    """Map each eligible `static const T * const[N]` name to the
    ordered list of `AddressInit` target names. Eligibility:
      * Internal linkage (`is_global == False`).
      * `init` is a non-empty list of `AddressInit(name, offset=0)`
        entries — pointers to other named statics, no offset.
      * `len(init) <= _DISPATCH_THRESHOLD`.
    """
    out: dict[str, list[str]] = {}
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.StaticVariable):
            continue
        if tl.is_global:
            continue
        init = tl.init
        if not init or len(init) > _DISPATCH_THRESHOLD:
            continue
        targets: list[str] = []
        ok = True
        for entry in init:
            if not isinstance(entry, tac_ast.AddressInit):
                ok = False
                break
            if entry.offset != 0:
                ok = False
                break
            targets.append(entry.name)
        if not ok:
            continue
        out[tl.name] = targets
    return out


def _rewrite_function(
    fn: tac_ast.Function,
    pointer_arrays: dict[str, list[str]],
    label_counter: list[int],
) -> tac_ast.Function:
    """Iteratively find and rewrite eligible chains until no more
    are found. Each iteration's rewrite invalidates the use-count
    map for subsequent iterations, so we recompute from scratch
    each round."""
    instrs = list(fn.instructions)
    while True:
        chain = _find_one_chain(instrs, pointer_arrays)
        if chain is None:
            break
        instrs = _apply_dispatch(
            instrs, chain, pointer_arrays, label_counter,
        )
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


def _find_one_chain(
    instrs: list[tac_ast.Type_instruction],
    pointer_arrays: dict[str, list[str]],
):
    """Walk `instrs` looking for an eligible chain. Returns a dict
    describing the chain on success, or None on no match."""
    use_count = _count_uses(instrs)
    def_of: dict[str, int] = {}
    for k, inst in enumerate(instrs):
        dst = _instr_dst_name(inst)
        if dst is not None:
            def_of[dst] = k
    for k, inst in enumerate(instrs):
        if not isinstance(inst, tac_ast.IndirectIndexedLoad):
            continue
        if not isinstance(inst.ptr, tac_ast.Var):
            continue
        # Volatile pointee or volatile pointer-table: refuse to
        # dispatch. The collapse would replace one observable
        # access with N observable accesses (or zero, for the
        # untaken cases), changing the program's volatile-access
        # trace. Per C99 §6.7.3.6 that's a semantic change.
        if inst.is_volatile:
            continue
        p_name = inst.ptr.name
        if use_count.get(p_name, 0) != 1:
            continue
        ptr_def_idx = def_of.get(p_name)
        if ptr_def_idx is None or ptr_def_idx >= k:
            continue
        ptr_def = instrs[ptr_def_idx]
        if not isinstance(ptr_def, tac_ast.IndexedLoad):
            continue
        if ptr_def.is_volatile:
            continue
        arr_name = ptr_def.name
        if arr_name not in pointer_arrays:
            continue
        if not isinstance(ptr_def.index, tac_ast.Var):
            continue
        scaled_name = ptr_def.index.name
        if use_count.get(scaled_name, 0) != 1:
            continue
        scaled_def_idx = def_of.get(scaled_name)
        if scaled_def_idx is None or scaled_def_idx >= ptr_def_idx:
            continue
        scaled_def = instrs[scaled_def_idx]
        i_val = _match_scale_by_two(scaled_def)
        if i_val is None:
            continue
        if not isinstance(inst.dst, tac_ast.Var):
            continue
        return {
            'scaled_idx': scaled_def_idx,
            'indexed_load_idx': ptr_def_idx,
            'ind_load_idx': k,
            'i_val': i_val,
            'j_val': inst.index,
            'v_name': inst.dst.name,
            'arr_name': arr_name,
        }
    return None


def _apply_dispatch(
    instrs: list[tac_ast.Type_instruction],
    chain,
    pointer_arrays: dict[str, list[str]],
    label_counter: list[int],
) -> list[tac_ast.Type_instruction]:
    """Build the dispatch block and splice it into `instrs`,
    removing the three original chain instructions."""
    scaled_idx = chain['scaled_idx']
    indexed_load_idx = chain['indexed_load_idx']
    ind_load_idx = chain['ind_load_idx']
    i_val = chain['i_val']
    j_val = chain['j_val']
    v_name = chain['v_name']
    arr_name = chain['arr_name']
    targets = pointer_arrays[arr_name]
    n = label_counter[0]
    label_counter[0] += 1

    case_labels = [f".dispatch@{n}@case@{k}" for k in range(len(targets))]
    end_label = f".dispatch@{n}@end"

    dispatch: list[tac_ast.Type_instruction] = []
    # CMP/BEQ chain on `i_val` — the last case is fallthrough so
    # we emit checks only for cases 0..N-2.
    for k in range(len(targets) - 1):
        dispatch.append(tac_ast.JumpIfCmp(
            op=tac_ast.Equal(),
            src1=i_val,
            src2=tac_ast.Constant(const=tac_ast.ConstChar(value=k)),
            target=case_labels[k],
        ))
    # Fallthrough — case N-1.
    last = len(targets) - 1
    dispatch.append(tac_ast.IndexedLoad(
        name=targets[last], index=j_val,
        dst=tac_ast.Var(name=v_name),
    ))
    dispatch.append(tac_ast.Jump(target=end_label))
    # Cases 0..N-2.
    for k in range(len(targets) - 1):
        dispatch.append(tac_ast.Label(name=case_labels[k]))
        dispatch.append(tac_ast.IndexedLoad(
            name=targets[k], index=j_val,
            dst=tac_ast.Var(name=v_name),
        ))
        dispatch.append(tac_ast.Jump(target=end_label))
    dispatch.append(tac_ast.Label(name=end_label))

    # Splice: replace [scaled_idx, indexed_load_idx, ind_load_idx]
    # with the dispatch block. The three indices are distinct and
    # ordered (scaled < indexed_load < ind_load), but they may not
    # be contiguous — there could be unrelated instructions between
    # them. Conservative: rebuild the list, dropping those three
    # positions and inserting the dispatch at the position of the
    # IndirectIndexedLoad (the last in source order).
    drop = {scaled_idx, indexed_load_idx, ind_load_idx}
    new_instrs: list[tac_ast.Type_instruction] = []
    for k, inst in enumerate(instrs):
        if k in drop:
            if k == ind_load_idx:
                new_instrs.extend(dispatch)
            continue
        new_instrs.append(inst)
    return new_instrs


def _count_uses(instrs):
    counts: dict[str, int] = {}
    def add(v):
        if isinstance(v, tac_ast.Var):
            counts[v.name] = counts.get(v.name, 0) + 1
    for inst in instrs:
        for v in _instr_use_vals(inst):
            add(v)
    return counts


def _instr_dst_name(inst) -> str | None:
    if hasattr(inst, 'dst'):
        v = getattr(inst, 'dst')
        if isinstance(v, tac_ast.Var):
            return v.name
    return None


def _instr_use_vals(inst):
    """Yield every Val in a USE position of `inst`."""
    if isinstance(inst, tac_ast.Copy):
        yield inst.src
    elif isinstance(inst, tac_ast.Binary):
        yield inst.src1
        yield inst.src2
    elif isinstance(inst, tac_ast.Unary):
        yield inst.src
    elif isinstance(inst, (tac_ast.SignExtend, tac_ast.ZeroExtend,
                           tac_ast.Truncate, tac_ast.IntToFloat,
                           tac_ast.IntToDouble, tac_ast.FloatToInt,
                           tac_ast.DoubleToInt, tac_ast.FloatToDouble,
                           tac_ast.DoubleToFloat)):
        yield inst.src
    elif isinstance(inst, tac_ast.JumpIfTrue):
        yield inst.condition
    elif isinstance(inst, tac_ast.JumpIfFalse):
        yield inst.condition
    elif isinstance(inst, tac_ast.JumpIfCmp):
        yield inst.src1
        yield inst.src2
    elif isinstance(inst, tac_ast.JumpIfMasked):
        yield inst.val
    elif isinstance(inst, tac_ast.IndexedLoad):
        yield inst.index
    elif isinstance(inst, tac_ast.IndexedStore):
        yield inst.index
        yield inst.src
    elif isinstance(inst, tac_ast.IndexedSymbolStore):
        yield inst.index
        yield inst.src
    elif isinstance(inst, tac_ast.IndexedConstLoad):
        yield inst.index
    elif isinstance(inst, tac_ast.IndirectIndexedLoad):
        yield inst.ptr
        yield inst.index
    elif isinstance(inst, tac_ast.IndirectIndexedStore):
        yield inst.ptr
        yield inst.index
        yield inst.src
    elif isinstance(inst, tac_ast.Load):
        yield inst.src_ptr
    elif isinstance(inst, tac_ast.Store):
        yield inst.dst_ptr
        yield inst.src
    elif isinstance(inst, tac_ast.FunctionCall):
        for a in inst.args:
            yield a
    elif isinstance(inst, tac_ast.IndirectCall):
        yield inst.ptr
        for a in inst.args:
            yield a
    elif isinstance(inst, tac_ast.Ret):
        if inst.val is not None:
            yield inst.val
    elif isinstance(inst, tac_ast.Phi):
        for a in inst.args:
            yield a.source if hasattr(a, 'source') else a


def _match_scale_by_two(inst):
    """If `inst` is a Binary that scales a Var by 2 — either
    `LeftShift(v, 1)` or `Multiply(v, 2)` (Multiply in either
    operand order) — return the Var operand. Otherwise None."""
    if not isinstance(inst, tac_ast.Binary):
        return None
    op = inst.op
    s1, s2 = inst.src1, inst.src2
    if isinstance(op, tac_ast.LeftShift):
        if (isinstance(s1, tac_ast.Var)
            and _is_int_constant(s2, 1)):
            return s1
        return None
    if isinstance(op, tac_ast.Multiply):
        if (isinstance(s1, tac_ast.Var)
            and _is_int_constant(s2, 2)):
            return s1
        if (isinstance(s2, tac_ast.Var)
            and _is_int_constant(s1, 2)):
            return s2
    return None


def _is_int_constant(val, target: int) -> bool:
    if not isinstance(val, tac_ast.Constant):
        return False
    c = val.const
    if hasattr(c, 'value'):
        return c.value == target
    return False
