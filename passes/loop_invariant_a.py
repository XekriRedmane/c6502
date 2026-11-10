"""Keep a loop-invariant compare operand in A across a loop.

# Motivating shape

A search loop compares each table entry against a key that doesn't
change. `tac_to_asm` loads the varying side into A and compares it
against the key:

    .loop:                         ; loop header
       LDA   $2200,Y               ; table entry           4 cycles
       CMP   key                   ; loop-invariant        3 cycles
       BNE   .next
       ...                         ; rarer path, clobbers A
    .next:
       INY
       CPY   n
       BNE   .loop

The hand-written 6502 idiom keeps the key in A for the whole loop and
compares the entry against it (`CMP $2200,Y`, 4 cycles), reloading A
only on the paths that overwrite it. Zork I's page-table search is
written that way.

# Rewrite

1. **Swap the compare.** `LDA x; CMP k; Bxx` with Bxx = BEQ / BNE
   becomes `LDA k; CMP x; Bxx`. Equality is symmetric, so Z is
   unchanged. N and C do change, and A ends up holding `k` instead of
   `x`, so the swap requires A and every flag to be dead after the
   branch on both of its successors.
2. **Hoist the load.** The `LDA k` at the top of the header is
   deleted. An `LDA k` goes in front of the loop, on the fall-through
   entry, and on every edge inside the loop that reaches the header
   with A no longer equal to `k` (a path that overwrote A). The hot
   path, which doesn't touch A, gets no load at all.

# Eligibility

- The loop is a range `[h, b]` of the instruction list: `Label(H)` at
  `h` and the last backward branch or jump to H at `b`. Nothing
  outside the range jumps into it; it's entered only by falling into
  `Label(H)`. `c99_to_tac` lays every loop out this way.
- The header starts `Label(H); Mov(x, A); Compare(A, k);
  Branch(EQ|NE, _)`, with `k` a `Data` / `ZP` operand and `x` an
  operand CMP can address.
- No instruction in the range writes anything that may alias `k`
  (`passes.asm_aliasing.may_alias`), and the range has no `Call`.
- A reload on a taken branch edge P -> S needs a new label in front
  of S's label. That's only possible when the instruction before
  S's label never falls through (a `Jump`, `Return`, or `Ret`), so
  the reload runs only on the redirected edge. Otherwise the pass
  leaves the loop alone.
- Blocks between an A-overwriting path and the header must not read
  or write A, so that a reload on their incoming edge can't change
  what they compute.

# Where it runs

After the final peephole fixedpoint and before
`expand_long_branches`, so it sees concrete operands and the layout
the peepholes settled on, and the stubs it adds get long-branch
expansion like everything else.
"""

from __future__ import annotations

import asm_ast
from passes.asm_aliasing import may_alias
from passes.asm_liveness import a_dead_at, sets_flags

_A = asm_ast.Reg(reg=asm_ast.A())
_TERMINATORS = (asm_ast.Jump, asm_ast.Return, asm_ast.Ret)
_CMP_OPERANDS = (
    asm_ast.Imm, asm_ast.Data, asm_ast.ZP, asm_ast.IndexedData,
    asm_ast.Frame, asm_ast.Stack, asm_ast.Indirect, asm_ast.IndirectY,
    asm_ast.IndirectZp, asm_ast.IndirectZpY,
)


def apply_loop_invariant_a(
    prog: asm_ast.Program,
    zp_slot_symbols: dict[str, int] | None = None,
) -> asm_ast.Program:
    out = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            instrs = list(tl.instructions)
            counter = [0]
            changed = True
            while changed:
                new = _rewrite_one(instrs, tl.name, counter, zp_slot_symbols)
                changed = new is not None
                if changed:
                    instrs = new
            tl = asm_ast.Function(
                name=tl.name, is_global=tl.is_global,
                params=list(tl.params), instructions=instrs,
            )
        out.append(tl)
    return asm_ast.Program(top_level=out)


def _target(instr) -> str | None:
    if isinstance(instr, (asm_ast.Jump, asm_ast.Branch)):
        return instr.target
    return None


def _writes_a(instr) -> bool:
    if isinstance(instr, (asm_ast.Mov, asm_ast.Pop)):
        return instr.dst == _A
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or, asm_ast.Xor,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        return instr.dst == _A
    return isinstance(instr, (
        asm_ast.Call, asm_ast.FunctionPrologue, asm_ast.AllocateStack,
    ))


def _touches_a(instr) -> bool:
    """True iff `instr` reads or writes A, counting the A-staging
    that a memory-to-memory Mov or a Push / Compare of A implies."""
    if _writes_a(instr):
        return True
    if isinstance(instr, asm_ast.Mov):
        # Mem-to-mem Movs are emitted as LDA src; STA dst.
        return instr.src == _A or not isinstance(instr.dst, asm_ast.Reg)
    if isinstance(instr, asm_ast.Compare):
        return instr.left == _A
    if isinstance(instr, asm_ast.Push):
        return instr.src == _A
    if isinstance(instr, asm_ast.LoadAddress):
        return True
    return False


def _written_memory(instr) -> list:
    """Memory operands `instr` may write."""
    if isinstance(instr, (asm_ast.Mov, asm_ast.Pop, asm_ast.LoadAddress)):
        return [] if isinstance(instr.dst, asm_ast.Reg) else [instr.dst]
    if isinstance(instr, (
        asm_ast.Inc, asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
        asm_ast.LogicalShiftRight, asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        return [] if isinstance(instr.dst, asm_ast.Reg) else [instr.dst]
    return []


_C_WRITERS = (
    asm_ast.Compare, asm_ast.SetCarry, asm_ast.ClearCarry,
    asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
)
_C_READERS = (
    asm_ast.Add, asm_ast.Sub, asm_ast.RotateLeft, asm_ast.RotateRight,
)


def _n_c_dead_at(instrs, idx, label_at) -> bool:
    """True iff neither N nor C at `idx` is read before being
    overwritten, on any path. Swapping a CMP's operands changes those
    two flags (Z stays the same), so this is what the swap needs.
    Readers: BMI / BPL read N, BCC / BCS and ADC / SBC / ROL / ROR read
    C. A return or call ends the path, since no flag carries a value
    across one, except that a carry-flag return reads C. Any
    instruction that sets N/Z overwrites N; CMP / CPX / CPY, SEC / CLC,
    and ASL / LSR overwrite C."""
    seen: set[tuple[int, bool, bool]] = set()
    stack = [(idx, True, True)]
    while stack:
        i, n_live, c_live = stack.pop()
        while i < len(instrs) and (n_live or c_live):
            if (i, n_live, c_live) in seen:
                break
            seen.add((i, n_live, c_live))
            ins = instrs[i]
            if isinstance(ins, asm_ast.Branch):
                if n_live and isinstance(ins.cond, (asm_ast.MI, asm_ast.PL)):
                    return False
                if c_live and isinstance(ins.cond, (asm_ast.CC, asm_ast.CS)):
                    return False
                t = label_at.get(ins.target)
                if t is None:
                    return False
                stack.append((t, n_live, c_live))
                i += 1
                continue
            if isinstance(ins, asm_ast.Jump):
                t = label_at.get(ins.target)
                if t is None:
                    return False
                i = t
                continue
            if c_live and isinstance(ins, (asm_ast.Return, asm_ast.Ret))                     and ins.save_c:
                # A carry-flag return reads C.
                return False
            if isinstance(ins, (asm_ast.Return, asm_ast.Ret, asm_ast.Call)):
                # Only a carry-flag return carries a flag, and a
                # callee doesn't read its caller's flags.
                break
            if c_live and isinstance(ins, _C_READERS):
                return False
            if sets_flags(ins):
                n_live = False
            if isinstance(ins, _C_WRITERS + _C_READERS):
                c_live = False
            i += 1
    return True


def _rewrite_one(instrs, fn_name, counter, zp_slot_symbols):
    """Apply the rewrite to the first eligible loop, returning the
    new instruction list, or None when no loop qualifies."""
    label_at = {
        ins.name: k for k, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    for h, head in enumerate(instrs):
        if not isinstance(head, asm_ast.Label):
            continue
        backs = [
            k for k, ins in enumerate(instrs)
            if k > h and _target(ins) == head.name
        ]
        if not backs:
            continue
        b = max(backs)
        result = _try_loop(instrs, h, b, label_at, fn_name, counter,
                           zp_slot_symbols)
        if result is not None:
            return result
    return None


def _try_loop(instrs, h, b, label_at, fn_name, counter, zp_slot_symbols):
    if h + 3 > b:
        return None
    lda, cmp_, br = instrs[h + 1], instrs[h + 2], instrs[h + 3]
    if not (
        isinstance(lda, asm_ast.Mov) and lda.dst == _A
        and not lda.is_volatile
        and isinstance(lda.src, _CMP_OPERANDS)
        and isinstance(cmp_, asm_ast.Compare) and cmp_.left == _A
        and isinstance(cmp_.right, (asm_ast.Data, asm_ast.ZP))
        and isinstance(br, asm_ast.Branch)
        and isinstance(br.cond, (asm_ast.EQ, asm_ast.NE))
    ):
        return None
    k = cmp_.right
    x = lda.src
    in_range = range(h, b + 1)

    # Single entry: no jump from outside the range lands inside it,
    # including on the header itself.
    names_in = {
        instrs[i].name for i in in_range if isinstance(instrs[i], asm_ast.Label)
    }
    for i, ins in enumerate(instrs):
        if i in in_range:
            continue
        if _target(ins) in names_in:
            return None
    # The header's label must not be reached by falling out of a
    # block that doesn't end the entry path, other than the entry.
    # (Falling into Label(H) from h-1 is the entry edge.)

    # `k` is invariant over the loop.
    for i in in_range:
        ins = instrs[i]
        if isinstance(ins, (asm_ast.Call, asm_ast.FunctionPrologue,
                            asm_ast.AllocateStack, asm_ast.Ret)):
            return None
        for w in _written_memory(ins):
            if may_alias(w, k, zp_slot_symbols=zp_slot_symbols):
                return None

    # The swap needs A and every flag dead after the branch, on both
    # successors.
    tgt = label_at.get(br.target)
    if tgt is None:
        return None
    if not a_dead_at(instrs, h + 4) or not a_dead_at(instrs, tgt):
        return None
    if not _n_c_dead_at(instrs, h + 4, label_at) or not _n_c_dead_at(instrs, tgt, label_at):
        return None

    # Basic blocks of the range.
    leaders = {h}
    for i in in_range:
        ins = instrs[i]
        if isinstance(ins, asm_ast.Label):
            leaders.add(i)
        if isinstance(ins, (asm_ast.Branch,) + _TERMINATORS) and i + 1 <= b:
            leaders.add(i + 1)
    starts = sorted(leaders)
    blocks = []      # (start, end) inclusive
    for n, s in enumerate(starts):
        e = (starts[n + 1] - 1) if n + 1 < len(starts) else b
        blocks.append((s, e))
    block_of_label = {}
    for bi, (s, e) in enumerate(blocks):
        if isinstance(instrs[s], asm_ast.Label):
            block_of_label[instrs[s].name] = bi

    # Successor edges inside the range: (pred block, succ block, kind)
    # where kind is "fall" or "jump" (and the index of the jumping
    # instruction for "jump").
    edges = []
    for bi, (s, e) in enumerate(blocks):
        last = instrs[e]
        t = _target(last)
        if t is not None and t in block_of_label:
            edges.append((bi, block_of_label[t], "jump", e))
        if not isinstance(last, _TERMINATORS) and bi + 1 < len(blocks):
            edges.append((bi, bi + 1, "fall", e))

    header = 0
    # A == k at block exit, assuming A == k at header entry (after the
    # header's own LDA k is deleted). Blocks that write A end with
    # A == k only when their last A write is `LDA k`.
    def exit_state(bi, entry):
        s, e = blocks[bi]
        state = entry
        for i in range(s, e + 1):
            ins = instrs[i]
            if i == h + 1:
                continue        # the LDA k being hoisted
            if _writes_a(ins):
                state = (
                    isinstance(ins, asm_ast.Mov) and ins.src == k
                    and ins.dst == _A
                )
        return state

    preds = {bi: [] for bi in range(len(blocks))}
    for p, q, _, _ in edges:
        preds[q].append(p)

    # need[bi]: A must equal k at bi's entry, because bi reaches the
    # header without touching A.
    def pure(bi):
        s, e = blocks[bi]
        return not any(
            _touches_a(instrs[i]) for i in range(s, e + 1)
            if not isinstance(instrs[i], asm_ast.Label)
        )

    need = {bi: False for bi in range(len(blocks))}
    need[header] = True
    changed = True
    while changed:
        changed = False
        for p, q, _, _ in edges:
            if need[q] and not need[p] and p != header and pure(p):
                need[p] = True
                changed = True

    # A == k at each block's entry. A block that needs k gets it on
    # every incoming edge (from the edge's source, or from a reload
    # placed on the edge), so its entry has it by construction. Other
    # blocks have it when all their predecessors end with it.
    entry_ok = {bi: True for bi in range(len(blocks))}
    changed = True
    while changed:
        changed = False
        for bi in range(len(blocks)):
            if need[bi]:
                continue
            v = all(exit_state(p, entry_ok[p]) for p in preds[bi]) if preds[bi] else False
            if v != entry_ok[bi]:
                entry_ok[bi] = v
                changed = True

    # Edges that must get a reload: into a block that needs k, from a
    # block whose exit doesn't have it.
    reloads = []
    for p, q, kind, at in edges:
        if not need[q]:
            continue
        if exit_state(p, entry_ok[p]):
            continue
        if q != header and not need[p] and not pure(q):
            return None
        reloads.append((p, q, kind, at))
    # Every reload target block must be one the analysis can satisfy:
    # a pure block, or the header.
    for p, q, kind, at in reloads:
        if q != header and not pure(q):
            return None
    # A block that needs k but is entered from a block that neither
    # has k nor gets a reload can't happen: every such edge is in
    # `reloads`.

    # Plan the edits. Each is (insert_before_index, [instrs]) plus
    # branch retargets.
    reload_ins = asm_ast.Mov(src=k, dst=_A, is_volatile=False)
    inserts: dict[int, list] = {}
    retarget: dict[int, str] = {}
    # Entry: before Label(H).
    entry_stub = [reload_ins]
    header_stub_label = None
    for p, q, kind, at in reloads:
        s_q = blocks[q][0]
        if kind == "fall":
            if q == header:
                return None
            # Before the block's label (if any), so only this
            # fall-through edge runs it.
            inserts.setdefault(s_q, []).append(reload_ins)
            continue
        # Taken edge into the header: share the entry load by giving
        # it a label of its own.
        if q == header:
            if header_stub_label is None:
                counter[0] += 1
                header_stub_label = f".{fn_name}@lia@{counter[0]}"
                entry_stub = [asm_ast.Label(name=header_stub_label), reload_ins]
            retarget[at] = header_stub_label
            continue
        counter[0] += 1
        stub_label = f".{fn_name}@lia@{counter[0]}"
        if s_q == 0 or not isinstance(instrs[s_q - 1], _TERMINATORS):
            return None
        if not isinstance(instrs[s_q], asm_ast.Label):
            return None
        inserts.setdefault(s_q, []).extend(
            [asm_ast.Label(name=stub_label), reload_ins],
        )
        retarget[at] = stub_label
    # The entry load sets N/Z; the header's first remaining
    # instruction is the CMP, which overwrites them.
    out = []
    for i, ins in enumerate(instrs):
        if i == h:
            out.extend(entry_stub)
        if i in inserts and i != h:
            out.extend(inserts[i])
        if i == h + 1:
            continue                     # drop the header's LDA x
        if i == h + 2:
            out.append(asm_ast.Compare(left=_A, right=x))   # swapped CMP
            continue
        if i in retarget:
            t = retarget[i]
            if isinstance(ins, asm_ast.Branch):
                ins = asm_ast.Branch(cond=ins.cond, target=t)
            else:
                ins = asm_ast.Jump(target=t, reg_args=list(ins.reg_args))
        out.append(ins)
    return out
