"""Promote a uchar loop counter to the X register, with full
cross-call save/restore and Y-pivot of transient indexed accesses.

# Motivating case

`refresh_hit_entities` outer loop. Original (pre-this-pass):

    LDA p0
    STA b4
.loop_start:
    LDX b4                      ; reload counter at top
    LDA entity_hit_y,X
    ...
    LDX p2                      ; sprite_xref — clobbers X
    LDA hit_spr_*,X
    ...
    LDX b4                      ; mid-iter reload after clobber
    LDA entity_hit_row,X
    ...
    STA p5
    JSR draw_sprite_opaque       ; callee clobbers X
    DEC b4
.loop_continue:
    BPL .loop_start

Hand-written equivalent (Drol's REFRESH_HIT_ENTITIES at $631D):

    LDX ZP_HIT_MAX              ; init X
HIT_DRAW_BODY:
    LDA ENTITY_HIT_Y,X
    ...
    LDY ZP_SPRITE_XREF          ; sprite_xref in Y (not X!)
    LDA HIT_SPR_*,Y
    ...
    LDA ENTITY_HIT_ROW,X        ; X still has counter
    ...
    STX ZP_DRAW_LOOP_IDX        ; save before JSR
    JSR DRAW_SPRITE_OPAQUE
    LDX ZP_DRAW_LOOP_IDX        ; restore after JSR
    JMP HIT_DRAW_TAIL
HIT_DRAW_TAIL:
    DEX
    BPL HIT_DRAW_BODY

Three transformations combine to bridge the gap:

  1. **Y-pivot** any `LDX <other>; LDA arr,X; ...` block to
     `LDY <other>; LDA arr,Y; ...`, so X isn't clobbered by the
     transient indexed access.
  2. **Cross-Call save/restore**: insert `STX M` before each
     `Call` in the body, and `LDX M` after — explicitly preserving
     the counter across the callee's clobber.
  3. **Counter promotion**: drop the loop-top `LDX M` (X carries
     from prior iter's DEX or init), drop the now-redundant
     mid-iter `LDX M` reloads (X is preserved via Y-pivot and
     save/restore), append `TAX` to the init, replace `DEC M`
     with `DEX`.

# Eligibility

The slot M is eligible iff:

  * Exactly one Mov whose dst is M (the init).
  * Exactly one `Dec(M)` (the decrement).
  * One or more `Mov(M, Reg(X))` (LDX M reloads).
  * No other uses of M anywhere.
  * Every other X-write in the function is one of:
    - An `LDX <other>` whose surrounding basic-block range
      contains only IndexedData(index=X) consumers AND no Y-use.
      (Pivotable.)
    - A `Call` instruction (save/restore handled).
  * Other X-writers (INX, DEX, TAX) disqualify.

# Soundness sketch

After the transformation, X holds M's current value at every
program point that originally read M via `LDX M` (or any other X
consumer). Specifically:

  * Init: `LDA <src>; STA M; TAX` — X = M = initial.
  * Loop-top: X carries from the previous iter's DEX (back-edge)
    or the init's TAX. The dropped LDX M is unobservable.
  * Mid-body: Y-pivoted ranges leave X untouched; the M-canonical
    home is read implicitly via the same X. Mid-iter LDX M was
    only there to restore after an X-clobber; with no clobber,
    drop.
  * Around Call: STX M before the Call writes the canonical home
    so the LDX M after restores X correctly.
  * Loop tail: DEX decrements X (= M). No STX needed because
    either (a) the next iter starts at loop-top with X carrying,
    or (b) before the next Call, an STX will sync M.

# Where to run

After `replace_pseudoregisters` (operands are concrete) and after
the asm-peephole fixed-point loop (so `dec_inc_branch_fold` has
already simplified `DEC M; LDA M; B<cond>` to `DEC M; B<cond>`).
Before `expand_long_branches`."""

from __future__ import annotations

import asm_ast


_FLAG_NZ_BRANCHES: tuple[type, ...] = (
    asm_ast.EQ, asm_ast.NE, asm_ast.MI, asm_ast.PL,
)


def apply_loop_counter_to_x(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _is_reg(op, regtype) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, regtype)


def _is_reg_a(op) -> bool:
    return _is_reg(op, asm_ast.A)


def _is_reg_x(op) -> bool:
    return _is_reg(op, asm_ast.X)


def _is_reg_y(op) -> bool:
    return _is_reg(op, asm_ast.Y)


def _operands_equal(a, b) -> bool:
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    return False


def _operand_key(op) -> tuple | None:
    if isinstance(op, asm_ast.Data):
        return ("data", op.name, op.offset)
    if isinstance(op, asm_ast.ZP):
        return ("zp", op.address, op.offset)
    return None


def _mem_op_from_key(key):
    kind = key[0]
    if kind == "data":
        return asm_ast.Data(name=key[1], offset=key[2])
    return asm_ast.ZP(address=key[1], offset=key[2])


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    plan = _plan_promotion(instrs)
    if plan is None:
        return fn
    return _apply_plan(fn, plan)


def _plan_promotion(instrs):
    """Identify a candidate counter slot M and verify the
    transformation can be applied. Returns a plan dict or None.

    Plan keys:
      'm_key'           — counter slot key (memory op).
      'init_idx'        — index of init Mov(_, M).
      'dec_idx'         — index of Dec(M).
      'loop_top_lda_idx' — index of the loop-top LDX M to drop.
      'mid_lda_indices' — indices of mid-iter LDX M reloads to drop.
      'pivot_ranges'    — list of (start, end) ranges to Y-pivot.
      'call_indices'    — indices of Call instructions to wrap.
    """
    # Step 1: find counter-slot candidates by use-pattern.
    cand = _find_counter_candidate(instrs)
    if cand is None:
        return None
    m_key = cand['m_key']
    m_op = _mem_op_from_key(m_key)

    # Step 2: scan all X-writes. Classify each.
    pivot_ranges: list[tuple[int, int]] = []
    call_indices: list[int] = []
    mid_lda_indices: list[int] = []
    loop_top_lda_idx = cand['loop_top_lda_idx']
    dec_idx = cand['dec_idx']

    i = 0
    while i < len(instrs):
        instr = instrs[i]
        if _is_x_write_other_than(instr, m_key):
            # Some non-counter X-write. Must be Y-pivotable or a Call.
            if isinstance(instr, asm_ast.Call):
                call_indices.append(i)
                i += 1
                continue
            if _is_ldx_to_x(instr):
                # Try to collect a pivot range.
                end = _try_collect_pivot_range(instrs, i)
                if end is None:
                    return None
                pivot_ranges.append((i, end))
                i = end
                continue
            # Some other X-write (INX, DEX, TAX, PLX): disqualify.
            return None
        # LDX M reload: track if it's mid-iter (not the loop-top one).
        if (isinstance(instr, asm_ast.Mov)
                and _is_reg_x(instr.dst)
                and _operand_key(instr.src) == m_key
                and i != loop_top_lda_idx):
            mid_lda_indices.append(i)
        i += 1

    return {
        'm_key': m_key,
        'm_op': m_op,
        'init_idx': cand['init_idx'],
        'dec_idx': dec_idx,
        'loop_top_lda_idx': loop_top_lda_idx,
        'mid_lda_indices': mid_lda_indices,
        'pivot_ranges': pivot_ranges,
        'call_indices': call_indices,
        'lda_m_indices': cand['lda_m_indices'],
    }


def _is_x_write_other_than(instr, m_key) -> bool:
    """True iff `instr` writes Reg(X) AND the write isn't an LDX
    from the counter's canonical home M."""
    if isinstance(instr, asm_ast.Mov):
        if _is_reg_x(instr.dst):
            # LDX from M is the canonical reload — not "other".
            if _operand_key(instr.src) == m_key:
                return False
            return True
        return False
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        return _is_reg_x(instr.dst)
    if isinstance(instr, asm_ast.Call):
        return True
    if isinstance(instr, asm_ast.Pop):
        return _is_reg_x(instr.dst)
    return False


def _is_ldx_to_x(instr) -> bool:
    return (isinstance(instr, asm_ast.Mov)
            and _is_reg_x(instr.dst)
            and not isinstance(instr.src, asm_ast.Reg))


def _try_collect_pivot_range(instrs, start: int):
    """Verify a Y-pivot range starting at instrs[start] is valid.
    Returns the exclusive end index, or None if no valid range
    exists from here.

    Validity:
      * No Y use or Y write in the range (we're about to put a
        value in Y).
      * Every X use in the range is via IndexedData(index=X).
      * Range ends at: another X-write, a control-flow boundary,
        or end-of-function."""
    j = start + 1
    while j < len(instrs):
        instr = instrs[j]
        # Control-flow / call boundaries end the range BEFORE this
        # instruction.
        if isinstance(instr, (asm_ast.Label, asm_ast.Jump,
                              asm_ast.Branch, asm_ast.Call,
                              asm_ast.Ret, asm_ast.Return)):
            return j
        # Another X-write ends the range BEFORE this instruction.
        if isinstance(instr, asm_ast.Mov):
            if _is_reg_x(instr.dst):
                return j
            if _is_reg_y(instr.dst):
                # Y-write — would conflict.
                return None
            if _is_reg_y(instr.src):
                # TYA or read-Y — conflicts.
                return None
            if _is_reg_x(instr.src):
                # TXA or STX — uses X as a value, not as index.
                return None
            # Check IndexedData operands for Y use.
            for op in (instr.src, instr.dst):
                if isinstance(op, asm_ast.IndexedData):
                    if isinstance(op.index, asm_ast.Y):
                        return None
                # Indirect operands (Indirect / IndirectY /
                # IndirectZp / IndirectZpY) all read Y as their
                # addressing-mode index. A pivot that overwrites Y
                # would corrupt these reads.
                if isinstance(op, (asm_ast.Indirect, asm_ast.IndirectY,
                                   asm_ast.IndirectZp,
                                   asm_ast.IndirectZpY)):
                    return None
        elif isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
            if _is_reg_x(instr.dst):
                return j
            if _is_reg_y(instr.dst):
                return None
        elif isinstance(instr, asm_ast.Compare):
            if _is_reg_x(instr.left) or _is_reg_y(instr.left):
                return None
        elif isinstance(instr, asm_ast.Push):
            if _is_reg_x(instr.src) or _is_reg_y(instr.src):
                return None
        j += 1
    return j


def _find_counter_candidate(instrs):
    """Identify a memory slot M with the loop-counter use pattern.
    Returns a dict with 'm_key', 'init_idx', 'dec_idx',
    'loop_top_lda_idx', 'lda_m_indices', or None."""
    init_idx: dict[tuple, int] = {}
    init_count: dict[tuple, int] = {}
    dec_idx: dict[tuple, int] = {}
    dec_count: dict[tuple, int] = {}
    ldx_idx: dict[tuple, list[int]] = {}
    lda_idx: dict[tuple, list[int]] = {}
    disqualified: set[tuple] = set()

    for i, instr in enumerate(instrs):
        for role, op in _operand_roles(instr):
            key = _operand_key(op)
            if key is None:
                continue
            if role == 'init':
                init_count[key] = init_count.get(key, 0) + 1
                init_idx[key] = i
            elif role == 'ldx':
                ldx_idx.setdefault(key, []).append(i)
            elif role == 'lda':
                lda_idx.setdefault(key, []).append(i)
            elif role == 'dec':
                dec_count[key] = dec_count.get(key, 0) + 1
                dec_idx[key] = i
            else:
                disqualified.add(key)

    for key in list(init_count) + list(dec_count) + list(ldx_idx):
        if key in disqualified:
            continue
        if init_count.get(key, 0) != 1:
            continue
        if dec_count.get(key, 0) != 1:
            continue
        if not ldx_idx.get(key):
            continue
        loop_top = _find_loop_top_lda(
            instrs, dec_idx[key], ldx_idx[key],
        )
        if loop_top is None:
            continue
        return {
            'm_key': key,
            'init_idx': init_idx[key],
            'dec_idx': dec_idx[key],
            'loop_top_lda_idx': loop_top,
            'lda_m_indices': lda_idx.get(key, []),
        }
    return None


def _stable_mem_eq(a, b) -> bool:
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    return False


def _operand_roles(instr):
    """Classify each memory operand's role for the candidate scan."""
    if isinstance(instr, asm_ast.Mov):
        src, dst = instr.src, instr.dst
        if _stable_mem_eq(src, dst):
            return  # self-Mov
        if _is_reg_x(dst) and not isinstance(src, asm_ast.Reg):
            yield ('ldx', src)
            return
        if _is_reg_a(dst) and not isinstance(src, asm_ast.Reg):
            # `Mov(M, Reg(A))` — LDA M. Doesn't disqualify the
            # counter slot; the apply step rewrites it to TXA
            # (since X = M is the promotion invariant).
            yield ('lda', src)
            return
        if not isinstance(dst, asm_ast.Reg):
            yield ('init', dst)
            return
        if not isinstance(src, asm_ast.Reg):
            yield ('other', src)
        return
    if isinstance(instr, asm_ast.Dec):
        if not isinstance(instr.dst, asm_ast.Reg):
            yield ('dec', instr.dst)
        return
    if isinstance(instr, asm_ast.Inc):
        if not isinstance(instr.dst, asm_ast.Reg):
            yield ('other', instr.dst)
        return
    if isinstance(instr, (asm_ast.Add, asm_ast.Sub,
                          asm_ast.And, asm_ast.Or)):
        if not isinstance(instr.src, asm_ast.Reg):
            yield ('other', instr.src)
        if not isinstance(instr.dst, asm_ast.Reg):
            yield ('other', instr.dst)
        return
    if isinstance(instr, asm_ast.Compare):
        if not isinstance(instr.left, asm_ast.Reg):
            yield ('other', instr.left)
        if not isinstance(instr.right, asm_ast.Reg):
            yield ('other', instr.right)
        return


def _find_loop_top_lda(instrs, dec_idx, ldx_indices):
    """Verify the DEC is followed by a flag-NZ Branch whose target
    chain lands at one of the ldx_indices. Returns that index or
    None."""
    branch_targets: set[str] = set()
    for inst in instrs:
        if isinstance(inst, (asm_ast.Jump, asm_ast.Branch)):
            branch_targets.add(inst.target)
    j = dec_idx + 1
    while j < len(instrs) and isinstance(instrs[j], asm_ast.Label):
        if instrs[j].name in branch_targets:
            return None
        j += 1
    if j >= len(instrs):
        return None
    br = instrs[j]
    if not (isinstance(br, asm_ast.Branch)
            and isinstance(br.cond, _FLAG_NZ_BRANCHES)):
        return None
    target = br.target
    label_to_idx = {
        inst.name: k for k, inst in enumerate(instrs)
        if isinstance(inst, asm_ast.Label)
    }
    seen: set[str] = set()
    while target in label_to_idx and target not in seen:
        seen.add(target)
        k = label_to_idx[target] + 1
        while k < len(instrs):
            inst = instrs[k]
            if isinstance(inst, asm_ast.Label):
                k += 1
                continue
            if (isinstance(inst, asm_ast.Mov)
                    and _stable_mem_eq(inst.src, inst.dst)):
                k += 1
                continue
            break
        if k >= len(instrs):
            return None
        if isinstance(instrs[k], asm_ast.Jump):
            target = instrs[k].target
            continue
        if k in ldx_indices:
            return k
        return None
    return None


def _apply_plan(fn, plan):
    """Walk the function's instructions, applying all rewrites from
    the plan. Output is a fresh instruction list."""
    instrs = fn.instructions
    m_op = plan['m_op']
    init_idx = plan['init_idx']
    dec_idx = plan['dec_idx']
    loop_top_lda_idx = plan['loop_top_lda_idx']
    mid_lda_set = set(plan['mid_lda_indices'])
    call_set = set(plan['call_indices'])
    lda_m_set = set(plan['lda_m_indices'])
    # Map each pivot range start to its end.
    pivot_start_to_end: dict[int, int] = {
        s: e for s, e in plan['pivot_ranges']
    }
    pivot_in_range: dict[int, int] = {}
    for s, e in plan['pivot_ranges']:
        for j in range(s, e):
            pivot_in_range[j] = s

    reg_x = asm_ast.Reg(reg=asm_ast.X())
    reg_y = asm_ast.Reg(reg=asm_ast.Y())
    reg_a = asm_ast.Reg(reg=asm_ast.A())

    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        if i == loop_top_lda_idx:
            i += 1
            continue
        if i in mid_lda_set:
            i += 1
            continue
        if i in call_set:
            # Wrap: STX M; <Call>; LDX M.
            out.append(asm_ast.Mov(src=reg_x, dst=m_op))
            out.append(instrs[i])
            out.append(asm_ast.Mov(src=m_op, dst=reg_x))
            i += 1
            continue
        if i in pivot_in_range:
            # Y-pivot: rewrite the leading LDX→LDY and each
            # IndexedData(X) consumer in the range.
            out.append(_pivot_rewrite(instrs[i]))
            i += 1
            continue
        if i in lda_m_set:
            # `LDA M` rewritten to `TXA` — X = M is the promotion
            # invariant, so this is value-equivalent.
            out.append(asm_ast.Mov(src=reg_x, dst=reg_a))
            i += 1
            continue
        if i == dec_idx:
            # Replace Dec(M) with Dec(Reg(X)) (DEX).
            out.append(asm_ast.Dec(dst=reg_x))
            i += 1
            continue
        if i == init_idx:
            # Append TAX after init so X holds M's value.
            out.append(instrs[i])
            out.append(asm_ast.Mov(src=reg_a, dst=reg_x))
            i += 1
            continue
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _pivot_rewrite(instr):
    """Rewrite a pivot-range instruction: the leading LDX→LDY,
    and any IndexedData(index=X) consumer to index=Y."""
    if isinstance(instr, asm_ast.Mov):
        new_src = _rewrite_op_for_pivot(instr.src)
        new_dst = _rewrite_op_for_pivot(instr.dst)
        # The leading LDX (dst=Reg(X), non-Reg src) becomes LDY.
        if (_is_reg_x(new_dst)
                and not isinstance(new_src, asm_ast.Reg)):
            new_dst = asm_ast.Reg(reg=asm_ast.Y())
        if new_src is instr.src and new_dst is instr.dst:
            return instr
        return asm_ast.Mov(src=new_src, dst=new_dst)
    if isinstance(instr, (asm_ast.Add, asm_ast.Sub,
                          asm_ast.And, asm_ast.Or)):
        new_src = _rewrite_op_for_pivot(instr.src)
        new_dst = _rewrite_op_for_pivot(instr.dst)
        if new_src is instr.src and new_dst is instr.dst:
            return instr
        return type(instr)(src=new_src, dst=new_dst)
    return instr


def _rewrite_op_for_pivot(op):
    if (isinstance(op, asm_ast.IndexedData)
            and isinstance(op.index, asm_ast.X)):
        return asm_ast.IndexedData(
            name=op.name, offset=op.offset, index=asm_ast.Y(),
        )
    return op
