"""Asm pass: promote a multiply-LDX'd ZP-resident Data operand into
Reg(Y), loaded once at function entry.

# Motivating shape

After this session's TAC sinker + ADC-commute + cross-block A
tracking land, `apply_bobble`'s body looks like:

    LDX __zpabi_apply_bobble_p1   ; X = bobble_idx
    LDA rescue_bobble,X
    BPL .if_else@1
.add:
    AND #$7F
    LDX __zpabi_apply_bobble_p0    ; <-- slot, reloaded
    CLC
    ADC entity_floor_pos,X
    STA entity_floor_pos,X
    JMP .if_end@0
.if_else@1:
    ...
    LDX __zpabi_apply_bobble_p0    ; <-- slot, reloaded
    LDA entity_floor_pos,X
    ...

`__zpabi_apply_bobble_p0` (slot) is loaded into X in both branches
because X is already taken at function entry by `bobble_idx`.
Pinning slot to Y at function entry and switching the
`entity_floor_pos,X` accesses to `,Y` collapses the two LDX
reloads into one LDY at entry — saves one `LDX abs` (3 bytes) in
the code; cycle-neutral here since only one branch fires per
call, but the saving compounds in functions with more uses-per-
branch.

# Eligibility

A Data symbol `D` qualifies for Y-promotion in a function iff:

  1. `D` appears as the source of TWO OR MORE `Mov(Data(D, 0),
     Reg(X))` instructions in the function. (A single LDX has
     nothing to fold; promoting it would just shift the load
     register.)
  2. Reg(Y) is unused elsewhere in the function. Any of the
     following disqualifies:
       - any `Mov` whose src or dst is `Reg(Y)`,
       - any `Inc` / `Dec` of `Reg(Y)`,
       - any `Pop` into `Reg(Y)`,
       - any `Compare` with `Reg(Y)` as `left`,
       - any `IndexedData(_, _, Y())` operand,
       - any `Indirect` / `IndirectY` / `IndirectZpY` operand
         (these consume Y by addressing mode),
       - any `Frame` / `Stack` operand (use indirect-Y at emit).
  3. At least one OTHER source loads X — i.e., X is already
     "taken" by some non-D value. Without competition for X,
     just keeping D in X has the same cost.
  4. Every IndexedData,X access in the LDX(D)-to-next-X-clobber
     range is in an instruction slot that supports the
     corresponding IndexedData,Y form. The 6502 doesn't have
     `LDX abs,X` (irrelevant — we're going the other way),
     `LDY abs,Y` (ditto), or `INC abs,Y` / `DEC abs,Y` /
     `ASL abs,Y` / `LSR abs,Y` / `ROL abs,Y` / `ROR abs,Y`. If
     any access lives at one of those positions, the rewrite
     would produce un-encodable asm; bail.

# Rewrite

  1. Insert `Mov(Data(D, 0), Reg(Y))` at the start of the
     function body — after any leading `Label` and
     `FunctionPrologue` / `AllocateStack`, before the first
     true body instruction.
  2. Walk the rest of the body forward:
       - At each `Mov(Data(D, 0), Reg(X))`: drop the LDX. Flip
         a `promote_active` flag to True so subsequent
         IndexedData,X operands get rewritten to ,Y.
       - At any instruction that re-writes X (LDX of a
         different source, INX, DEX, PLX): set `promote_active`
         False so subsequent IndexedData,X operands stay ,X
         (they read THAT new value, not D's).
       - At a control-flow boundary (Label, Jump, Branch, Call,
         Ret, Return): set `promote_active` False. X's value
         across blocks isn't tracked here; subsequent IndexedData
         accesses see whatever X holds at the new entry.
       - For instructions in the active range: rewrite each
         `IndexedData(name, offset, X())` operand to
         `IndexedData(name, offset, Y())`.

# Soundness

`Y` is unused by the function pre-rewrite (gated). Loading D into
Y at function entry doesn't disturb anything because no
instruction observed Y before. After the rewrite, every
IndexedData,Y in the function body reads the byte at `name +
offset + (D's stored value)`, which equals `name + offset +
(X's stored value)` would have been at that point in the original
— because we only rewrite in ranges where X was loaded with the
same D.

For the soft-stack ABI: Y is not part of the calling convention
between functions. Callers don't expect a specific Y value on
return, so pinning Y inside the function body is invisible to
the call site.

# Cycle / byte effect

Each dropped `LDX abs` saves 3 cycles and 3 bytes (zp variant
would save 2 of each). The added `LDY abs` at entry costs 3
cycles / 3 bytes per call. The trade is neutral when only one
LDX would fire per call (apply_bobble's branch case — cycle-
neutral, 3 bytes saved overall because two LDXs in the code
became one LDY) and positive when multiple LDXs fire on the same
path (a loop using the same index across many accesses).

# Where to run

After `_peephole_fixedpoint` has converged and before
`expand_long_branches`. We only shrink code (one fewer LDX per
duplicate, plus the rewritten operands are the same size); no
long-branch displacement check is invalidated.
"""
from __future__ import annotations

import asm_ast


# Instruction kinds whose dst operand position doesn't support
# IndexedData,Y on the NMOS 6502. The 6502 has `INC abs,X` /
# `DEC abs,X` / shift-on-abs,X but not the abs,Y forms; same for
# LDY/STY at abs,X-only and LDX/STX at abs,Y-only.
#
# At the asm_ast level, the relevant operand position is the
# `dst` field. If we'd rewrite an `IndexedData(_, _, X())` in a
# `dst` slot of one of these kinds, the resulting instruction
# wouldn't assemble.
_DST_X_ONLY_KINDS: tuple[type, ...] = (
    asm_ast.Inc,
    asm_ast.Dec,
    asm_ast.ArithmeticShiftLeft,
    asm_ast.LogicalShiftRight,
    asm_ast.RotateLeft,
    asm_ast.RotateRight,
)


def apply_dual_index_promotion(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every function; pick the best Y-promotion candidate
    (if any) and apply the rewrite. Single forward pass per
    function — at most one Y-promotion fires per function (Y can
    only hold one value at a time)."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = list(fn.instructions)
    # Gate 2: Y must be unused.
    if _y_is_used(instrs):
        return fn
    # Gate 1: find candidates with 2+ LDX uses.
    candidates = _ldx_data_candidates(instrs)
    if not candidates:
        return fn
    # Deterministic candidate pick: by count descending, then name
    # ascending. (Stable for tests.)
    candidates.sort(key=lambda nc: (-nc[1], nc[0]))
    promoted_name = candidates[0][0]
    # Gate 3: X must have some other live source. Without that, no
    # LDX reloads are happening — the LDX(D) chain could already
    # stay in X without conflict, and Y-promotion just shifts the
    # work.
    if not _x_has_other_uses(instrs, promoted_name):
        return fn
    # Gate 4: every IndexedData,X access in the rewrite range
    # must be in a position where IndexedData,Y assembles.
    if _rewrite_would_be_unencodable(instrs, promoted_name):
        return fn
    return _do_promote(fn, instrs, promoted_name)


# ---------------------------------------------------------------------------
# Eligibility helpers
# ---------------------------------------------------------------------------


def _y_is_used(instrs: list[asm_ast.Type_instruction]) -> bool:
    """True iff any instruction in `instrs` reads or writes Reg(Y)
    or relies on Y being a specific value via indirect-Y
    addressing."""
    for instr in instrs:
        if _references_y(instr):
            return True
    return False


def _references_y(instr: asm_ast.Type_instruction) -> bool:
    for op in _operands_in(instr):
        if isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.Y):
            return True
        if isinstance(op, asm_ast.IndexedData) and isinstance(
            op.index, asm_ast.Y,
        ):
            return True
        # Indirect-Y addressing modes consume Y implicitly.
        if isinstance(op, (
            asm_ast.Indirect, asm_ast.IndirectY,
            asm_ast.IndirectZpY,
            asm_ast.Frame, asm_ast.Stack,
        )):
            return True
    return False


def _ldx_data_candidates(
    instrs: list[asm_ast.Type_instruction],
) -> list[tuple[str, int]]:
    """For each Data symbol that appears as the source of
    `Mov(Data(name, 0), Reg(X))`, count occurrences. Return only
    those with 2+ occurrences."""
    counts: dict[str, int] = {}
    for instr in instrs:
        name = _ldx_data_source_name(instr)
        if name is None:
            continue
        counts[name] = counts.get(name, 0) + 1
    return [(n, c) for n, c in counts.items() if c >= 2]


def _ldx_data_source_name(
    instr: asm_ast.Type_instruction,
) -> str | None:
    """If `instr` is `Mov(Data(name, 0), Reg(X))`, return `name`;
    else None. The offset-0 gate ensures we're pinning a single-
    byte symbol (multi-byte values can't ride in Y as a whole)."""
    if not isinstance(instr, asm_ast.Mov):
        return None
    if not isinstance(instr.dst, asm_ast.Reg):
        return None
    if not isinstance(instr.dst.reg, asm_ast.X):
        return None
    src = instr.src
    if not isinstance(src, asm_ast.Data):
        return None
    if src.offset != 0:
        return None
    return src.name


def _x_has_other_uses(
    instrs: list[asm_ast.Type_instruction], promoted_name: str,
) -> bool:
    """True iff some `Mov(_, Reg(X))` in the function loads X from
    a source OTHER than `Data(promoted_name, 0)`. Without that, X
    only ever holds `promoted_name`'s value, and Y-promotion is a
    no-op."""
    for instr in instrs:
        if not isinstance(instr, asm_ast.Mov):
            continue
        if not isinstance(instr.dst, asm_ast.Reg):
            continue
        if not isinstance(instr.dst.reg, asm_ast.X):
            continue
        src = instr.src
        if (
            isinstance(src, asm_ast.Data)
            and src.name == promoted_name
            and src.offset == 0
        ):
            continue
        # Any other LDX source.
        return True
    return False


def _rewrite_would_be_unencodable(
    instrs: list[asm_ast.Type_instruction], promoted_name: str,
) -> bool:
    """Simulate the forward walk; flag any IndexedData,X in the
    rewrite range that sits in an instruction slot where
    IndexedData,Y wouldn't assemble (Inc/Dec/shifts have no abs,Y
    form on the NMOS 6502)."""
    promote_active = False
    for instr in instrs:
        if _ldx_data_source_name(instr) == promoted_name:
            promote_active = True
            continue
        if _writes_x(instr) or _is_block_boundary(instr):
            promote_active = False
            continue
        if not promote_active:
            continue
        # Check `dst` slot of unsupported kinds for IndexedData,X.
        if isinstance(instr, _DST_X_ONLY_KINDS):
            dst = getattr(instr, "dst", None)
            if _is_indexed_data_x(dst):
                return True
    return False


def _is_indexed_data_x(op) -> bool:
    return (
        isinstance(op, asm_ast.IndexedData)
        and isinstance(op.index, asm_ast.X)
    )


# ---------------------------------------------------------------------------
# Rewrite
# ---------------------------------------------------------------------------


def _do_promote(
    fn: asm_ast.Function,
    instrs: list[asm_ast.Type_instruction],
    promoted_name: str,
) -> asm_ast.Function:
    """Insert `LDY <promoted_name>` at the function-entry insertion
    point; drop every `LDX(promoted_name)`; rewrite IndexedData,X
    to ,Y in the active range."""
    insert_at = _entry_insertion_point(instrs)
    ldy = asm_ast.Mov(
        src=asm_ast.Data(name=promoted_name, offset=0),
        dst=asm_ast.Reg(reg=asm_ast.Y()),
        is_volatile=False,
    )
    new_instrs: list[asm_ast.Type_instruction] = []
    new_instrs.extend(instrs[:insert_at])
    new_instrs.append(ldy)
    promote_active = False
    for instr in instrs[insert_at:]:
        if _ldx_data_source_name(instr) == promoted_name:
            promote_active = True
            continue
        if _writes_x(instr) or _is_block_boundary(instr):
            promote_active = False
        if promote_active:
            instr = _rewrite_x_to_y(instr)
        new_instrs.append(instr)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=new_instrs,
    )


def _entry_insertion_point(
    instrs: list[asm_ast.Type_instruction],
) -> int:
    """First index past leading entry-decoration instructions
    (Label / FunctionPrologue / AllocateStack). The LDY we
    prepend goes at this position."""
    i = 0
    while i < len(instrs) and isinstance(
        instrs[i],
        (asm_ast.Label, asm_ast.FunctionPrologue, asm_ast.AllocateStack),
    ):
        i += 1
    return i


def _writes_x(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` modifies Reg(X). Covers LDX (`Mov(_, X)`),
    INX (`Inc(X)`), DEX (`Dec(X)`), PLX (`Pop(X)`; NMOS 6502
    doesn't have PLX but we defensively cover it)."""
    if isinstance(instr, asm_ast.Mov):
        if isinstance(instr.dst, asm_ast.Reg) and isinstance(
            instr.dst.reg, asm_ast.X,
        ):
            return True
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        if isinstance(instr.dst, asm_ast.Reg) and isinstance(
            instr.dst.reg, asm_ast.X,
        ):
            return True
    if isinstance(instr, asm_ast.Pop):
        if isinstance(instr.dst, asm_ast.Reg) and isinstance(
            instr.dst.reg, asm_ast.X,
        ):
            return True
    return False


def _is_block_boundary(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` ends or starts a basic block — control
    flow can enter or leave here, so X's value across the
    boundary isn't trackable by a linear walk."""
    return isinstance(instr, (
        asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
        asm_ast.Call, asm_ast.Ret, asm_ast.Return,
        asm_ast.FunctionPrologue, asm_ast.AllocateStack,
        asm_ast.LoadAddress,
    ))


def _rewrite_x_to_y(
    instr: asm_ast.Type_instruction,
) -> asm_ast.Type_instruction:
    """Rebuild `instr` with every `IndexedData(_, _, X())` operand
    rewritten to `IndexedData(_, _, Y())`. Instruction kinds that
    don't carry IndexedData,X operands pass through unchanged."""
    if isinstance(instr, asm_ast.Mov):
        return asm_ast.Mov(
            src=_fix_op(instr.src),
            dst=_fix_op(instr.dst),
            is_volatile=instr.is_volatile,
        )
    if isinstance(instr, asm_ast.Add):
        return asm_ast.Add(src=_fix_op(instr.src), dst=_fix_op(instr.dst))
    if isinstance(instr, asm_ast.Sub):
        return asm_ast.Sub(src=_fix_op(instr.src), dst=_fix_op(instr.dst))
    if isinstance(instr, asm_ast.And):
        return asm_ast.And(src=_fix_op(instr.src), dst=_fix_op(instr.dst))
    if isinstance(instr, asm_ast.Or):
        return asm_ast.Or(src=_fix_op(instr.src), dst=_fix_op(instr.dst))
    if isinstance(instr, asm_ast.Xor):
        return asm_ast.Xor(
            src1=_fix_op(instr.src1),
            src2=_fix_op(instr.src2),
            dst=_fix_op(instr.dst),
        )
    if isinstance(instr, asm_ast.Compare):
        return asm_ast.Compare(
            left=_fix_op(instr.left),
            right=_fix_op(instr.right),
        )
    # Inc / Dec / ASL / LSR / ROL / ROR with IndexedData,X dst are
    # blocked by `_rewrite_would_be_unencodable` — we never reach
    # here for those in active mode.
    return instr


def _fix_op(op: asm_ast.Type_operand) -> asm_ast.Type_operand:
    """If `op` is `IndexedData(_, _, X())`, return the same operand
    with index swapped to `Y()`; else return `op` unchanged."""
    if (
        isinstance(op, asm_ast.IndexedData)
        and isinstance(op.index, asm_ast.X)
    ):
        return asm_ast.IndexedData(
            name=op.name, offset=op.offset, index=asm_ast.Y(),
        )
    return op


def _operands_in(instr: asm_ast.Type_instruction):
    """Yield every operand of `instr`. Used by Y-detection only."""
    if isinstance(instr, asm_ast.Mov):
        yield instr.src
        yield instr.dst
        return
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub,
        asm_ast.And, asm_ast.Or,
    )):
        yield instr.src
        yield instr.dst
        return
    if isinstance(instr, asm_ast.Xor):
        yield instr.src1
        yield instr.src2
        yield instr.dst
        return
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        yield instr.dst
        return
    if isinstance(instr, (
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        yield instr.dst
        return
    if isinstance(instr, asm_ast.Push):
        yield instr.src
        return
    if isinstance(instr, asm_ast.Pop):
        yield instr.dst
        return
    if isinstance(instr, asm_ast.Compare):
        yield instr.left
        yield instr.right
        return
    if isinstance(instr, asm_ast.BitTest):
        yield instr.src
        return
    if isinstance(instr, asm_ast.LoadAddress):
        yield instr.src
        yield instr.dst
        return
