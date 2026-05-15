"""Asm-level dead-store elimination (CFG-wide).

Walks each function and drops any `Mov(Reg, M)` (i.e. STA / STX /
STY) into a memory operand `M` whose value is not observed by any
instruction reachable from the store. Handles two cases:

  * Within-block: forward linear scan finds a same-address overwrite
    before any read of `M`. This was the original pass's behavior
    and still covers the simple cache-then-overwrite shape.
  * Across blocks: when the within-block scan reaches a control-
    flow boundary (Label / Jump / Branch / Ret / Return), continue
    via a CFG-wide forward DFS. The store is dead iff every path
    forward either re-overwrites `M` before reading, or terminates
    at function exit with `M` known dead-at-exit.

# Motivating case (cross-block, what triggered the cross-block work)

The b=6 group of paint_hud_strip_p1.c's unrolled body, post-
regalloc and post-redundant-load:

    LDA $A30D,Y         ; A = pixels.6
    STA $80             ; cache pixels in ZP (the candidate)
    INY
    STA $258C,X         ; row 1 (uses A — same pixels)
    ... 6 more STAs to framebuffer ...
    .loop@0_continue:   ; back-edge target, no LDA $80 anywhere
    DEX
    BPL .split          ; or fall through to RTS
    .ssa_block@0:
    RTS
    .split:
    JMP .loop@0_start

Every path forward — the fall-through to RTS, and the back-edge
through the entire unrolled body — never reads $80 before either
re-overwriting it or exiting. The store is dead.

# Dead-at-exit determination

At a `Ret` / `Return` (function exit), the store's target is
considered "dead at exit" iff its address is in the asm-level
regalloc pool (default `$80..$FF`). Locations there are
function-local: caller-saved slots `$80..$BF` are clobbered
across calls anyway, and callee-saved slots `$C0..$FF` get
restored from frame storage by the late-synthesized epilogue,
not from their current ZP value at the body's `Return`.

For ZP outside the pool (runtime infrastructure at `$00..$1F`,
notably HARGS at `$04..$1B` which holds wider-than-Int return
values) and for `Data` (link-time-addressable globals — other
functions in the program can read them), we conservatively treat
the address as live at exit. Cross-block analysis still gets to
fire if every path back-edges and overwrites before reading; it
just can't conclude "dead" purely on reaching function exit.

# Conservative aliasing and opaque instructions

Aliasing follows the same rules as `redundant_load._may_alias`:
ZP doesn't alias `Data` / `IndexedData`; two ZPs alias iff same
address; two `Data`s alias iff same name+offset; anything we
can't classify aliases conservatively.

Compound instructions (`LoadAddress`, `AllocateStack`,
`FunctionPrologue`) and `Call` are treated as opaque — they may
read any memory cell, so a path through one returns LIVE for any
target. `Push` / `Pop` write/read the hardware stack, which we
don't track; conservative LIVE.

# Where to run

After `replace_pseudoregisters` (operands are concrete) and
after the within-block peephole bracket (the
inc/dec/sub1_test_zero/direct_index_load/redundant_load passes
set up the dead-cache pattern). Before `expand_long_branches`
(this pass shrinks code, never grows; new branches don't
appear).
"""

from __future__ import annotations

import asm_ast
from passes.asm_aliasing import may_alias as _may_alias


# Asm-level regalloc pool range (default `Pool(start=0x80)` splits
# `[0x80, 0xFF]` into caller-saved `[0x80, 0xC0)` and callee-saved
# `[0xC0, 0x100)`). ZP addresses in this range are function-local,
# so they're dead at the function's exit. Hardcoded for now —
# a non-default pool would still be sound but might miss some
# cross-block kills.
_POOL_LO = 0x80
_POOL_HI = 0x100


# Compound / opaque instructions that the within-block fallback
# treats as block boundaries. The CFG walker treats them as opaque
# reads (LIVE on any path through them) so we can't optimize past
# one even cross-block.
#
# `Push` / `Pop` are deliberately NOT here: they only touch the
# 6502 hardware stack at $0100-$01FF and register A (Push src=A,
# Pop dst=A in the IR shapes tac_to_asm emits). They do not read
# or write any memory operand a DSE target can name (Data / ZP /
# Frame / Stack / Indirect / IndexedData all address other
# regions). Treating them as opaque would needlessly mark every
# STA dead inside an indirect-Y store's `PHA src; ... ; PLA` save
# bracket as live — which is exactly the shape that blocks
# `apply_indirect_base_prop`'s downstream DPTR-staging cleanup.
# `_read_operands` and `_write_operand` handle Push / Pop
# precisely below.
_OPAQUE_TYPES: tuple[type, ...] = (
    asm_ast.Call,
    asm_ast.FunctionPrologue,
    asm_ast.AllocateStack,
    asm_ast.LoadAddress,
)


def apply_asm_dead_store(
    prog: asm_ast.Program,
    *,
    zp_slot_symbols: dict[str, int] | None = None,
) -> asm_ast.Program:
    """`zp_slot_symbols`: optional map from slot-symbol name (e.g.
    `__local_<fn>_b<k>`, `__zpabi_<fn>_p<k>`) to its concrete ZP
    address. When provided, `Data(name)` operands whose name is in
    the map are treated as ZP at that address for dead-at-exit
    eligibility — so a STA to a local-pool slot at the function's
    tail is dead just like a regular `ZP(addr)` write would be."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl, zp_slot_symbols))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(
    fn: asm_ast.Function,
    zp_slot_symbols: dict[str, int] | None = None,
) -> asm_ast.Function:
    """Walk `fn.instructions` and drop or morph any STA whose target
    memory is not observed by any instruction reachable from the
    store (within-block kill, or CFG-wide forward search reaching
    only re-kills / dead-at-exit terminations).

    Two cases:
      * `Mov(Reg, <mem>)` — pure STA. If dead, drop entirely.
      * `Mov(<non-Reg src>, <mem dst>)` — emit's LDA+STA pair. The
        STA half is dead, but the LDA's side effects (A's new
        value AND the N/Z flags it sets) may still be needed by
        downstream code. Morph to `Mov(<src>, Reg(A))` (LDA only),
        keeping the load while dropping the store."""
    instrs = fn.instructions
    label_to_index = _build_label_map(instrs)
    out: list[asm_ast.Type_instruction] = []
    for i, instr in enumerate(instrs):
        kind = _dse_candidate_kind(instr)
        if kind is None:
            out.append(instr)
            continue
        if not _is_dead_cfg(instrs, i, label_to_index, zp_slot_symbols):
            out.append(instr)
            continue
        if kind == "pure_sta":
            # Drop entirely.
            continue
        # kind == "mem_to_mem": keep the LDA half by morphing the
        # dst to Reg(A).
        out.append(asm_ast.Mov(
            src=instr.src, dst=asm_ast.Reg(reg=asm_ast.A()),
        ))
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _dse_candidate_kind(instr: asm_ast.Type_instruction) -> str | None:
    """Classify `instr` for dead-store handling:
      * "pure_sta"  — `Mov(Reg, <stable mem>)`: drop on dead.
      * "mem_to_mem" — `Mov(<non-Reg>, <stable mem>)`: morph on
        dead (preserve the LDA half).
      * None        — not a candidate."""
    if not isinstance(instr, asm_ast.Mov):
        return None
    if not isinstance(instr.dst, (asm_ast.ZP, asm_ast.Data)):
        return None
    if isinstance(instr.src, asm_ast.Reg):
        return "pure_sta"
    # Non-Reg src into stable memory: emit produces LDA src; STA dst.
    # We can drop the STA half by morphing dst to Reg(A), preserving
    # the load and its flag effects.
    return "mem_to_mem"


def _build_label_map(
    instrs: list[asm_ast.Type_instruction],
) -> dict[str, int]:
    out: dict[str, int] = {}
    for i, ins in enumerate(instrs):
        if isinstance(ins, asm_ast.Label):
            out[ins.name] = i
    return out


def _successors(
    instrs: list[asm_ast.Type_instruction], i: int,
    label_to_index: dict[str, int],
) -> list[int]:
    """Indices of instructions that can execute immediately after
    `instrs[i]`. Empty for `Ret` / `Return` (function exit)."""
    ins = instrs[i]
    if isinstance(ins, (asm_ast.Ret, asm_ast.Return)):
        return []
    if isinstance(ins, asm_ast.Jump):
        tgt = label_to_index.get(ins.target)
        return [tgt] if tgt is not None else []
    if isinstance(ins, asm_ast.Branch):
        out: list[int] = []
        if i + 1 < len(instrs):
            out.append(i + 1)
        tgt = label_to_index.get(ins.target)
        if tgt is not None:
            out.append(tgt)
        return out
    if i + 1 < len(instrs):
        return [i + 1]
    return []


def _is_dead_at_exit(
    target: asm_ast.Type_operand,
    zp_slot_symbols: dict[str, int] | None = None,
) -> bool:
    """True iff `target` is known dead at function exit.

    Dead-at-exit categories:
      * ZP in the asm-level regalloc pool (`$80..$FF`) — function-
        local scratch.
      * `Data("DPTR", _)` — DPTR is the runtime's caller-saved
        scratch indirect-pointer pair. Callers don't expect its
        value preserved across a call; the only observable side
        effect of writing to it is a subsequent indirect access
        from inside this function, which the CFG-wide forward
        DFS would have caught as a read.
      * `Data(name, off)` where `name` is in `zp_slot_symbols` and
        the resolved address (`zp_slot_symbols[name] + off`) is in
        the pool range — these are local-pool body locals and
        zp_abi param slots that the allocator carved into ZP. They
        look like `Data` operands in the IR (the linker resolves
        the EQU symbol at link time) but behave like ZP-pool ZPs
        for liveness purposes.

    Everything else (user statics, HARGS / SSP / FP runtime
    symbols that carry return values or stack state) is
    conservatively live."""
    if isinstance(target, asm_ast.ZP):
        addr = target.address + target.offset
        return _POOL_LO <= addr < _POOL_HI
    if isinstance(target, asm_ast.Data):
        if target.name == "DPTR":
            return True
        if zp_slot_symbols is not None and target.name in zp_slot_symbols:
            addr = zp_slot_symbols[target.name] + target.offset
            return _POOL_LO <= addr < _POOL_HI
    return False


def _is_dead_cfg(
    instrs: list[asm_ast.Type_instruction], start: int,
    label_to_index: dict[str, int],
    zp_slot_symbols: dict[str, int] | None = None,
) -> bool:
    """True iff the STA at `instrs[start]` is dead by CFG-wide
    forward search: every path from start+1 forward either
    overwrites `target` at the same address before any read, or
    reaches a function exit with `target` dead-at-exit. Any path
    that finds a read (or hits an opaque instruction) returns
    LIVE."""
    sta = instrs[start]
    target = sta.dst
    visited: set[int] = set()
    stack: list[int] = list(_successors(instrs, start, label_to_index))
    while stack:
        j = stack.pop()
        if j in visited:
            continue
        visited.add(j)
        nxt = instrs[j]
        # Opaque instructions: assume they may read `target`.
        if isinstance(nxt, _OPAQUE_TYPES):
            return False
        # Function exit: dead iff `target` is dead-at-exit.
        if isinstance(nxt, (asm_ast.Ret, asm_ast.Return)):
            if not _is_dead_at_exit(target, zp_slot_symbols):
                return False
            continue
        # Read of `target`: LIVE.
        if _reads(nxt, target):
            return False
        # Exact-address overwrite: kill on this path, don't
        # propagate. (Aliasing-but-not-equal writes don't count as
        # a kill — they may write some other byte that aliases.)
        if _writes_same(nxt, target):
            continue
        # Otherwise: continue to successors.
        stack.extend(_successors(instrs, j, label_to_index))
    return True


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
    Reg destinations don't count (they don't kill memory).

    Any `Mov` with a non-`Reg` dst writes the dst's byte — including
    memory-to-memory shapes like `Mov(IndexedData, Data)` and
    `Mov(Data, Data)`, which `asm_emit` lowers to `LDA src; STA dst`.
    For kill purposes the source side doesn't matter: the dst is
    overwritten regardless of where the value came from."""
    match instr:
        case asm_ast.Mov(src=_, dst=dst):
            if not isinstance(dst, asm_ast.Reg):
                return dst
        case asm_ast.Inc(dst=dst) | asm_ast.Dec(dst=dst):
            if not isinstance(dst, asm_ast.Reg):
                return dst
        case asm_ast.Pop(dst=dst):
            # PLA pops the hardware stack into `dst`. When `dst` is
            # Reg(A) (the typical IR shape) this isn't a memory kill;
            # in the defensive case where `dst` is a memory operand,
            # surface it so it can kill a same-address upstream
            # store.
            if not isinstance(dst, asm_ast.Reg):
                return dst
    return None


def _read_operands(
    instr: asm_ast.Type_instruction,
):
    """Yield every memory operand `instr` may read. Includes the
    implicit pointer-source read for indirect-Y target operands —
    e.g. `STA (DPTR),Y` writes the target byte but READS DPTR
    (and DPTR+1) to know where to write."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            if not isinstance(src, asm_ast.Reg):
                yield src
            # STA via an indirect-Y mode reads the pointer source
            # (DPTR / FP / SSP / explicit ZP base) to compute the
            # target address. Surface those as reads so DSE
            # doesn't drop a still-live pointer staging.
            yield from _ptr_source_reads(dst)
        case asm_ast.Add(src=src, dst=dst) | asm_ast.Sub(src=src, dst=dst) \
                | asm_ast.And(src=src, dst=dst) | asm_ast.Or(src=src, dst=dst):
            if not isinstance(src, asm_ast.Reg):
                yield src
        case asm_ast.Xor(src1=s1, src2=s2):
            if not isinstance(s1, asm_ast.Reg):
                yield s1
            if not isinstance(s2, asm_ast.Reg):
                yield s2
        case asm_ast.Compare(left=l, right=r):
            if not isinstance(l, asm_ast.Reg):
                yield l
            if not isinstance(r, asm_ast.Reg):
                yield r
        case asm_ast.Inc(dst=dst) | asm_ast.Dec(dst=dst):
            # INC/DEC reads its dst (RMW).
            if not isinstance(dst, asm_ast.Reg):
                yield dst
            yield from _ptr_source_reads(dst)
        case (
            asm_ast.ArithmeticShiftLeft(dst=dst)
            | asm_ast.LogicalShiftRight(dst=dst)
            | asm_ast.RotateLeft(dst=dst)
            | asm_ast.RotateRight(dst=dst)
        ):
            if not isinstance(dst, asm_ast.Reg):
                yield dst
            yield from _ptr_source_reads(dst)
        case asm_ast.Push(src=src):
            # PHA reads src (typically Reg(A); the IR allows any
            # operand). Hardware-stack write isn't visible to memory
            # DSE — the stack at $0100-$01FF isn't a target shape.
            if not isinstance(src, asm_ast.Reg):
                yield src
        # Pop has no read operands (the hardware stack pop isn't a
        # memory target the DSE tracks); its write is handled in
        # `_write_operand`.


def _ptr_source_reads(op: asm_ast.Type_operand):
    """For an indirect-Y addressing-mode operand (Indirect /
    IndirectY / IndirectZp / IndirectZpY), yield the operand(s)
    naming the ZP byte pair that holds the pointer (DPTR / FP /
    SSP / the explicit ZP base). Yields nothing for non-indirect-Y
    operands."""
    if isinstance(op, (asm_ast.Indirect, asm_ast.IndirectY)):
        yield asm_ast.Data(name="DPTR", offset=0)
        yield asm_ast.Data(name="DPTR", offset=1)
        return
    if isinstance(op, (asm_ast.IndirectZp, asm_ast.IndirectZpY)):
        yield asm_ast.ZP(address=op.address, offset=0)
        yield asm_ast.ZP(address=op.address + 1, offset=0)
        return


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
