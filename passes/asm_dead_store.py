"""Asm-level dead-store elimination (within-block).

Walks each function's linear instruction stream. For each
`Mov(Reg, M)` (i.e. STA / STX / STY) into a memory operand `M`,
scans forward in the same basic-block segment looking for either:

  * a READ of `M` before another WRITE to `M` → the STA is LIVE,
    keep it; OR
  * a WRITE to `M` (with no intervening read), or a basic-block
    boundary (Label, Jump, Branch, Call, function exit) → the STA
    is DEAD (or "unknown to be live" — conservatively keep it
    when the boundary is hit, but for an OBSERVED kill within the
    block, drop).

# Motivating case

The c6502 pixel-cache pattern after sink-increment + redundant-
load STA-tracking:

    LDA $A30D,Y     ; A = pixels
    STA $80         ; cache pixels in ZP
    INY
    STA $240C,X     ; row 1 (uses A)
    STA $280C,X     ; row 2 (uses A — same pixels)
    ... 5 more STAs ...
    LDA $A30D,Y     ; reload A with NEW pixels
    STA $80         ; cache new pixels — overwrites the old cache

The first `STA $80` cached pixels for the run of `STA $XXXX,X`
writes. With STA-tracking redundant-load elimination, those
writes read A directly (not $80), so the cache is never read.
The second `STA $80` then overwrites the cache without anyone
having read it. The first STA is dead.

This pass walks each STA forward and detects this shape.

# Conservative aliasing

We use the same aliasing rules as `redundant_load`: ZP doesn't
alias Data / IndexedData (ZP < $0100, others ≥ $0100). Two ZPs
alias iff they have the same address. Two Datas alias iff they
have the same `name + offset`. Anything we can't classify
returns `True` (defensive — keep the STA).

# Boundary handling

Hitting a Label, Jump, Branch, Call, FunctionPrologue, Ret, or
Return ends the within-block scan. At a boundary we don't know
whether downstream code reads `M`, so we conservatively keep the
STA.

# Where to run

After `replace_pseudoregisters` (operands are concrete) and
after the existing peephole bracket (the inc/dec/sub1_test_zero/
direct_index_load/redundant_load passes set up the dead-cache
pattern). Before `expand_long_branches` (this pass shrinks code,
never grows; new branches don't appear).
"""

from __future__ import annotations

import asm_ast


_BLOCK_TERMINATORS: tuple[type, ...] = (
    asm_ast.Label,
    asm_ast.Jump,
    asm_ast.Branch,
    asm_ast.Call,
    asm_ast.Ret,
    asm_ast.Return,
    asm_ast.FunctionPrologue,
    asm_ast.AllocateStack,
    asm_ast.LoadAddress,
)


def apply_asm_dead_store(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    """Walk `fn.instructions` and drop any STA whose target memory
    is overwritten before being read within the basic-block
    segment."""
    instrs = fn.instructions
    drop = [False] * len(instrs)
    for i, instr in enumerate(instrs):
        if not _is_dse_candidate(instr):
            continue
        # `instr` is a Mov(Reg, M) where M is a stable-memory dst.
        if _is_dead(instrs, i):
            drop[i] = True
    out = [instr for i, instr in enumerate(instrs) if not drop[i]]
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _is_dse_candidate(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` is a STA / STX / STY into stable memory
    (ZP / Data) — the only shapes this DSE handles. Stack / Frame /
    Indirect destinations could be aliased by an indirect-Y read
    that we can't statically resolve, so we skip them."""
    if not isinstance(instr, asm_ast.Mov):
        return False
    if not isinstance(instr.src, asm_ast.Reg):
        return False
    if not isinstance(instr.dst, (asm_ast.ZP, asm_ast.Data)):
        return False
    return True


def _is_dead(
    instrs: list[asm_ast.Type_instruction], start: int,
) -> bool:
    """True iff the STA at `instrs[start]` is dead in the within-
    block sense: forward scan finds another write to the same
    memory cell before any read of it, with no intervening
    block-boundary instruction."""
    sta = instrs[start]
    target = sta.dst  # ZP or Data operand
    j = start + 1
    while j < len(instrs):
        nxt = instrs[j]
        if isinstance(nxt, _BLOCK_TERMINATORS):
            # Boundary — conservative: keep the STA.
            return False
        if _reads(nxt, target):
            return False
        if _writes_same(nxt, target):
            return True
        j += 1
    # End of function — conservative: keep the STA.
    return False


def _reads(instr: asm_ast.Type_instruction, target: asm_ast.Type_operand) -> bool:
    """True iff `instr` may read the byte at `target`. Conservative:
    if any operand we can't classify might alias, return True."""
    for op in _read_operands(instr):
        if _may_alias(op, target):
            return True
    return False


def _writes_same(
    instr: asm_ast.Type_instruction, target: asm_ast.Type_operand,
) -> bool:
    """True iff `instr` writes the SAME byte as `target` — i.e.
    structurally identical (Data-with-Data, ZP-with-ZP, same
    address). NOT just "may alias"; we need the kill to be exact
    so we don't conflate a partial kill with a full one."""
    dst = _write_operand(instr)
    if dst is None:
        return False
    return _operands_equal_exact(dst, target)


def _write_operand(
    instr: asm_ast.Type_instruction,
) -> asm_ast.Type_operand | None:
    """The single MEMORY destination operand of `instr`, if any.
    Reg destinations don't count (they don't kill memory)."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst) if isinstance(src, asm_ast.Reg):
            if not isinstance(dst, asm_ast.Reg):
                return dst
        case asm_ast.Inc(dst=dst) | asm_ast.Dec(dst=dst):
            if not isinstance(dst, asm_ast.Reg):
                return dst
    return None


def _read_operands(
    instr: asm_ast.Type_instruction,
):
    """Yield every memory operand `instr` may read."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            if not isinstance(src, asm_ast.Reg):
                yield src
            # A Mov to a non-reg memory operand only WRITES that dst;
            # it doesn't read it. (STA M doesn't read M.)
        case asm_ast.Add(src=src, dst=dst) | asm_ast.Sub(src=src, dst=dst) \
                | asm_ast.And(src=src, dst=dst) | asm_ast.Or(src=src, dst=dst):
            if not isinstance(src, asm_ast.Reg):
                yield src
        case asm_ast.Xor(src1=s1, src2=s2):
            if not isinstance(s1, asm_ast.Reg):
                yield s1
            if not isinstance(s2, asm_ast.Reg):
                yield s2
        case asm_ast.Push(src=src):
            if not isinstance(src, asm_ast.Reg):
                yield src
        case asm_ast.Compare(left=l, right=r):
            if not isinstance(l, asm_ast.Reg):
                yield l
            if not isinstance(r, asm_ast.Reg):
                yield r
        case asm_ast.Inc(dst=dst) | asm_ast.Dec(dst=dst):
            # INC/DEC reads its dst (RMW).
            if not isinstance(dst, asm_ast.Reg):
                yield dst
        case (
            asm_ast.ArithmeticShiftLeft(dst=dst)
            | asm_ast.LogicalShiftRight(dst=dst)
            | asm_ast.RotateLeft(dst=dst)
            | asm_ast.RotateRight(dst=dst)
        ):
            if not isinstance(dst, asm_ast.Reg):
                yield dst


def _may_alias(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    """Conservative aliasing: True iff we can't prove the two
    operands refer to disjoint memory cells. Mirrors `redundant_
    load._may_alias`'s rules."""
    if isinstance(a, asm_ast.Imm) or isinstance(b, asm_ast.Imm):
        return False
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return (a.address + a.offset) == (b.address + b.offset)
    if (isinstance(a, asm_ast.ZP)
            and isinstance(b, (asm_ast.Data, asm_ast.IndexedData))):
        return False
    if (isinstance(b, asm_ast.ZP)
            and isinstance(a, (asm_ast.Data, asm_ast.IndexedData))):
        return False
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    return True


def _operands_equal_exact(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    """Structural equality on memory operands. Used for the
    "same-address overwrite" kill check."""
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    return False
