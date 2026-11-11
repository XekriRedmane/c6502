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
       - any operand that is `Reg(Y)`, such as a `Mov` to or from
         Y, an `INY` / `DEY`, a `CPY`, or a pull into Y,
       - any `IndexedData(_, _, Y())` operand,
       - any `IndirectY` / `IndirectZpY` operand (these consume Y
         by addressing mode),
       - any `Indirect` / `IndirectZp` / `Frame` / `Stack` operand,
         which `asm_emit` lowers with an `LDY #off` in front,
       - a call or tail-call jump that passes an argument in Y.
  3. At least one OTHER source loads X — i.e., X is already
     "taken" by some non-D value. Without competition for X,
     just keeping D in X has the same cost.
  4. Every read of X that an `LDX D` reaches reads D on every
     path to it (`_plan_x_reads`). A forward dataflow over the
     control-flow graph tracks whether X may hold D, so a read
     after a label or a branch is handled the same as one in the
     LDX's own block. An `abs,X` read where X holds D on some paths
     and another value on others disqualifies, and so does a read
     of X as a value (`TXA`, `STX`, `CPX`, `INX`, a return with
     `save_x`) where X may hold D. Each `abs,X` read of D must also
     be in an instruction with an `abs,Y` form. The 6502 has no
     `INC abs,Y` / `DEC abs,Y` / `ASL abs,Y` / `LSR abs,Y` /
     `ROL abs,Y` / `ROR abs,Y`.
  5. Nothing in the function writes `D`, and the function makes
     no call. Y is loaded from `D` once at entry, so `D` has to
     hold its entry value at every LDX the rewrite drops, and Y
     has to survive until the last use. A callee may change Y,
     and may also write `D` if `D` is a global or a byte of the
     shared caller-saved zero-page range. A loop temporary that is
     stored and then reloaded into X fails this gate.
  6. N and Z are dead after every dropped `LDX D` and at the point
     where the `LDY D` goes, since both loads set them.

# Rewrite

  1. Insert `Mov(Data(D, 0), Reg(Y))` at the start of the
     function body — after any leading `Label` and
     `FunctionPrologue` / `AllocateStack`, before the first
     true body instruction.
  2. Drop every `Mov(Data(D, 0), Reg(X))`.
  3. In each instruction where gate 4 found that X holds D, rewrite
     each `IndexedData(name, offset, X())` operand to
     `IndexedData(name, offset, Y())`. An `abs,X` read where X
     holds another value stays as it is.

# Soundness

`Y` is unused by the function pre-rewrite (gated). Loading D into
Y at function entry doesn't disturb anything because no
instruction observed Y before. After the rewrite, every
IndexedData,Y in the function body reads the byte at `name +
offset + (D's stored value)`, which equals what `name + offset +
(X's stored value)` would have been at that point in the original,
because we only rewrite reads where X holds D on every path. The
reads left as `abs,X` see the same X as before, because every path
to them that passes a dropped `LDX D` also passes a later write of
X.

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
from passes.asm_liveness import flags_dead_at


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
    # Gate 5: D keeps its entry value and Y survives to every use.
    if any(isinstance(i, asm_ast.Call) for i in instrs):
        return fn
    if any(_writes_name(i, promoted_name) for i in instrs):
        return fn
    # Gate 4: every X read that an LDX of D reaches reads D on every
    # path, and has an abs,Y form.
    rewrite = _plan_x_reads(instrs, promoted_name)
    if rewrite is None:
        return fn
    # The dropped LDXs and the added LDY set N and Z.
    insert_at = _entry_insertion_point(instrs)
    if not flags_dead_at(instrs, insert_at):
        return fn
    if any(
        _ldx_data_source_name(instr) == promoted_name
        and not flags_dead_at(instrs, k + 1)
        for k, instr in enumerate(instrs)
    ):
        return fn
    return _do_promote(fn, instrs, promoted_name, rewrite)


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
    """Returns True iff `instr` reads or writes Reg(Y). That includes
    an `abs,Y` operand, the `(ind),Y` operands, and the `LDY #off`
    that `asm_emit` puts in front of an `Indirect`, `IndirectZp`,
    `Frame`, or `Stack` access. A call or tail-call jump that passes
    an argument in Y counts too."""
    if isinstance(instr, (asm_ast.Call, asm_ast.Jump)) and (
        "Y" in instr.reg_args
    ):
        return True
    for op in vars(instr).values():
        if isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.Y):
            return True
        if isinstance(op, asm_ast.IndexedData) and isinstance(
            op.index, asm_ast.Y,
        ):
            return True
        # Indirect-Y addressing modes consume Y implicitly, and the
        # ones with an offset load it first.
        if isinstance(op, (
            asm_ast.Indirect, asm_ast.IndirectY,
            asm_ast.IndirectZp, asm_ast.IndirectZpY,
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


# What X holds, as tracked by `_plan_x_reads`: the value of D,
# loaded by an LDX the rewrite drops, or anything else.
_X_IS_D = "D"
_X_IS_OTHER = "other"


def _plan_x_reads(
    instrs: list[asm_ast.Type_instruction], promoted_name: str,
) -> set[int] | None:
    """Returns the indices of the instructions whose `abs,X` operands
    read D's value, which the rewrite turns into `abs,Y`, or None if
    the rewrite can't be done.

    A forward dataflow over the control-flow graph tracks, at each
    instruction, whether X may hold D (from an `LDX D`) or something
    else. X holds something else at function entry. An `abs,X` read
    where X holds D on every path is rewritten. One where X holds D on
    some paths and something else on others can't be, since the
    rewritten code has to pick one register, so this returns None. So
    does a read of X as a value (`TXA`, `STX`, `CPX`, `INX`, a return
    with `save_x`, ...) where X may hold D, and an `abs,X` read of D
    in a position with no `abs,Y` form (`INC`, `DEC`, the shifts)."""
    label_idx = {
        ins.name: k for k, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    n = len(instrs)
    if n == 0:
        return set()
    state_in: list[frozenset[str] | None] = [None] * n
    state_in[0] = frozenset({_X_IS_OTHER})
    work = [0]
    while work:
        k = work.pop()
        instr = instrs[k]
        if _ldx_data_source_name(instr) == promoted_name:
            out = frozenset({_X_IS_D})
        elif _writes_x(instr):
            out = frozenset({_X_IS_OTHER})
        else:
            out = state_in[k]
        for succ in _successors(instrs, k, label_idx):
            old = state_in[succ]
            new = out if old is None else old | out
            if new != old:
                state_in[succ] = new
                work.append(succ)
    rewrite: set[int] = set()
    for k, instr in enumerate(instrs):
        state = state_in[k]
        if state is None or _X_IS_D not in state:
            continue  # Unreachable, or X can't hold D here.
        if _reads_x_value(instr):
            return None
        if not any(_is_indexed_data_x(op) for op in vars(instr).values()):
            continue
        if state != {_X_IS_D}:
            return None
        if isinstance(instr, _DST_X_ONLY_KINDS):
            return None
        if _rewrite_x_to_y(instr) is instr:
            return None  # An instruction kind the rewrite doesn't know.
        rewrite.add(k)
    return rewrite


def _successors(instrs, k: int, label_idx: dict[str, int]) -> list[int]:
    """Returns the indices control can reach directly after
    `instrs[k]`. A jump or branch to a name that isn't a label in the
    function (a tail call) has no successor there."""
    instr = instrs[k]
    if isinstance(instr, (asm_ast.Ret, asm_ast.Return)):
        return []
    if isinstance(instr, asm_ast.Jump):
        t = label_idx.get(instr.target)
        return [] if t is None else [t]
    out = [k + 1] if k + 1 < len(instrs) else []
    if isinstance(instr, asm_ast.Branch):
        t = label_idx.get(instr.target)
        if t is not None:
            out.append(t)
    return out


def _reads_x_value(instr: asm_ast.Type_instruction) -> bool:
    """Returns True iff `instr` reads Reg(X) other than as the index
    of an `abs,X` operand. A pure write to X (`LDX`, `TAX`, `PLX`)
    isn't a read."""
    if isinstance(instr, (asm_ast.Call, asm_ast.Jump)):
        return "X" in instr.reg_args
    if isinstance(instr, (asm_ast.Ret, asm_ast.Return)):
        return instr.save_x
    for name, op in vars(instr).items():
        if isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.X):
            if name == "dst" and isinstance(
                instr, (asm_ast.Mov, asm_ast.Pop, asm_ast.LoadAddress),
            ):
                continue
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
    rewrite: set[int],
) -> asm_ast.Function:
    """Inserts `LDY <promoted_name>` at the function-entry insertion
    point, drops every `LDX(promoted_name)`, and rewrites
    IndexedData,X to ,Y in the instructions at the `rewrite`
    indices from `_plan_x_reads`."""
    insert_at = _entry_insertion_point(instrs)
    ldy = asm_ast.Mov(
        src=asm_ast.Data(name=promoted_name, offset=0),
        dst=asm_ast.Reg(reg=asm_ast.Y()),
        is_volatile=False,
    )
    new_instrs: list[asm_ast.Type_instruction] = []
    new_instrs.extend(instrs[:insert_at])
    new_instrs.append(ldy)
    for k in range(insert_at, len(instrs)):
        instr = instrs[k]
        if _ldx_data_source_name(instr) == promoted_name:
            continue
        if k in rewrite:
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


def _writes_name(instr: asm_ast.Type_instruction, name: str) -> bool:
    """True iff `instr` writes a byte of the object `name`, directly
    or through an index."""
    dst = getattr(instr, "dst", None)
    if isinstance(instr, (asm_ast.Push, asm_ast.Compare, asm_ast.BitTest)):
        return False
    return (isinstance(dst, (asm_ast.Data, asm_ast.IndexedData))
            and dst.name == name)
