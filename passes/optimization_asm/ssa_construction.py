"""Asm-level SSA construction.

Converts an `asm_ast.Function` to SSA form. Each `(Pseudo name, byte
offset)` pair is versioned INDEPENDENTLY — that's the whole point of
the asm-level SSA layer: byte 0 of a 4-byte Long can have its own
def/use chain separate from byte 3, which is what later steps need
for byte-granular optimizations (e.g. high-byte DCE on values that
fit in a byte).

A `(name, offset)` pair is **promotable** iff `name` is not in:
  * **address-taken** names — names that appear as `LoadAddress.src`.
    Address-taken values must keep their multi-byte coherence at one
    frame slot, so they can't be split across SSA versions.
  * **read-modify-write target** names — names that appear as the
    `dst` of `Inc / Dec / ASL / LSR / ROL / ROR`. These are
    defensive: today's `tac_to_asm` doesn't emit those forms with
    Pseudo dsts, but the IR allows them and a future optimization
    might. The instruction's single operand serves as both use and
    def, which doesn't decompose cleanly into separate SSA versions.

Renaming convention. A def of byte `k` of `name` mints a fresh
Pseudo `Pseudo("<name>.b<k>.v<N>", offset=0)`. The byte position is
encoded into the new name so the Pseudo's `offset` field stays at
zero — each byte is now a 1-byte virtual variable. After SSA
destruction the renamed names are kept; `replace_pseudoregisters`
allocates one byte each (the symbol-table fallback `size_of_name`
returns 1 for names it doesn't recognize).

Algorithm (Cytron et al. 1991), mirroring `passes.optimization.ssa_construction`:
  1. Build the CFG; ensure every real block has a leading `Label`
     to use as `AsmPhiArg.pred_label`.
  2. Compute idom, dominance frontiers, dom-tree children.
  3. Identify promotable (name, offset) pairs.
  4. Find Defs(v) per promotable v. Place an empty `Phi(dst=Pseudo(
     "<name>.b<offset>", 0), args=[])` at the iterated dominance
     frontier of Defs(v), pruned by liveness (a Phi at a block where
     v isn't live-in is useless and produces dead defs).
  5. Renaming. Walk the dom-tree pre-order: for each block, rename
     each Phi's dst, then rewrite each non-Phi instruction's uses and
     defs in source order. After processing the block, fill in Phi
     args at every CFG successor based on the current top-of-stack
     for each promotable variable.

Parameters' initial SSA name IS their original Pseudo name (with
its original offset). A promotable param is rare (params usually
get Frame-resident addressing and so live as `Pseudo(param_name,
offset=k)` references that DO get versioned), but the renaming
pass pre-pushes the original name on each promotable param's stack
at ENTRY so the first read resolves cleanly.
"""

from __future__ import annotations

from collections import defaultdict
from dataclasses import dataclass
from typing import Iterable

import asm_ast
from passes.optimization_asm.cfg import (
    CFG,
    ENTRY_ID,
    EXIT_ID,
    BasicBlock,
    build_cfg,
    cfg_to_function,
    dominance_frontiers,
    dominator_tree_children,
    immediate_dominators,
)


# A "byte-level variable" is identified by (name, byte offset).
ByteVar = tuple[str, int]


def to_ssa(
    fn: asm_ast.Function, *,
    statics: frozenset[str] = frozenset(),
) -> asm_ast.Function:
    """Convert `fn` to asm-level SSA form. Returns the rewritten
    function. Phis are inserted at iterated dominance frontiers of
    each promotable (name, offset) pair's defs. New versioned
    Pseudo names follow the `<name>.b<offset>.v<N>` convention.

    `statics` is the set of static-storage Pseudo names visible at
    the program top level (file-scope variables, block-scope
    statics, extern declarations). These names address fixed
    link-time storage and must NOT be versioned — every write must
    reach the actual address. Same set as the one
    `replace_pseudoregisters` consumes to lower these to
    `Data(name, offset)` operands."""
    fn = _maybe_prepend_preheader(fn)
    cfg = build_cfg(fn)
    _ensure_block_labels(cfg, fn.name)

    excluded = excluded_pseudo_names(fn) | statics
    promotable = _promotable_byte_vars(cfg, excluded)
    if not promotable:
        return cfg_to_function(fn, cfg)

    idom = immediate_dominators(cfg)
    df = dominance_frontiers(cfg)
    children = dominator_tree_children(idom)
    live_in = _compute_live_in(cfg, promotable)

    block_label_of = {
        bid: _block_label(cfg.blocks[bid])
        for bid in idom
        if bid not in (ENTRY_ID, EXIT_ID) and cfg.blocks[bid].instructions
    }

    phis_at = _place_phis(cfg, promotable, df, live_in)
    _rename(
        cfg, fn, promotable, idom, children,
        block_label_of, phis_at,
    )
    return cfg_to_function(fn, cfg)


# ---------------------------------------------------------------------------
# Pre-pass: synthetic preheader (mirrors TAC SSA's _maybe_prepend_preheader).
# ---------------------------------------------------------------------------


def _maybe_prepend_preheader(fn: asm_ast.Function) -> asm_ast.Function:
    """Prepend a synthetic Label if the function body starts with
    one. Same rationale as TAC SSA: any block whose first instruction
    is a labeled jump-target can have predecessors beyond ENTRY (a
    back-edge from a loop tail), so SSA construction needs a real
    labeled block at the head to tag entry-path Phi args with."""
    if not fn.instructions:
        return fn
    if not isinstance(fn.instructions[0], asm_ast.Label):
        return fn
    existing = {
        i.name for i in fn.instructions if isinstance(i, asm_ast.Label)
    }
    counter = 0
    while True:
        name = f".{fn.name}@asm_ssa_preheader@{counter}"
        counter += 1
        if name not in existing:
            break
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params),
        instructions=[asm_ast.Label(name=name)] + list(fn.instructions),
    )


def _ensure_block_labels(cfg: CFG, fn_name: str) -> None:
    """Prepend a fresh `Label` to every real block whose first
    instruction isn't one. Names use `.<funcname>@asm_ssa_block@<N>`
    to stay disjoint from user labels and TAC SSA's labels."""
    existing = {
        i.name for blk in cfg.blocks.values()
        for i in blk.instructions if isinstance(i, asm_ast.Label)
    }
    counter = 0
    for bid in cfg.block_order:
        blk = cfg.blocks[bid]
        if blk.instructions and isinstance(blk.instructions[0], asm_ast.Label):
            continue
        while True:
            name = f".{fn_name}@asm_ssa_block@{counter}"
            counter += 1
            if name not in existing:
                existing.add(name)
                break
        blk.instructions.insert(0, asm_ast.Label(name=name))


def _block_label(blk: BasicBlock) -> str:
    if not blk.instructions or not isinstance(blk.instructions[0], asm_ast.Label):
        raise AssertionError(
            f"asm SSA: block {blk.id} has no leading label",
        )
    return blk.instructions[0].name


# ---------------------------------------------------------------------------
# Excluded-from-SSA names.
# ---------------------------------------------------------------------------


def excluded_pseudo_names(fn: asm_ast.Function) -> set[str]:
    """Pseudo names that must keep their original spelling and
    multi-byte coherence — not eligible for byte-granular SSA.

    PUBLIC API — shared with the SSA-aware passes downstream
    (`copy_propagation`, `backward_copy_propagation`, `byte_dce`).
    All of them must agree on which Pseudos are byte-versioned;
    if `copy_propagation` treats a name as SSA-promotable while
    `ssa_construction` excluded it, the pass picks the last
    source-order write as THE definition and propagates it past
    conditional control flow — observed bug, broke
    `examples/draw_sprite_opaque.c`'s `(page_flag & 0x80) ?
    screen_row_addr_hi2 : screen_row_addr_hi` ternary.
    Includes:

    * `LoadAddress.src` — address-taken; its bytes have to stay
      contiguous at one storage location.
    * `LoadAddress.dst` — the pointer-typed local that holds the
      address. The instruction writes TWO bytes (low + high) to its
      dst's storage, but the IR only mentions byte 0 in its operand
      (the byte 1 write is implicit in the emit's `_shift_offset(dst,
      1)`). Versioning byte 0 alone leaves byte 1 as an invisible
      side effect, which regalloc could silently overlap with another
      SSA value. Excluding the whole name forces it into a contiguous
      2-byte Frame slot instead.
    * Read-modify-write targets (Inc / Dec / ASL / LSR / ROL / ROR
      `.dst`) — single-operand RMW doesn't decompose cleanly into
      separate SSA versions. Defensive: today's `tac_to_asm` doesn't
      emit these forms with Pseudo dsts.
    * **DPTR-staged pointer Pseudos** — when both bytes of a Pseudo
      P are staged through DPTR (the canonical 4-instruction
      sequence `Mov(P[0], A); STA DPTR; Mov(P[1], A); STA DPTR+1`),
      byte-versioning would split P into separate single-byte
      Pseudos that regalloc places independently — typically at
      non-contiguous or wrong-order ZP slots, blocking the
      `apply_indirect_base_prop` peephole that would otherwise
      bypass the DPTR staging and emit `(P_low_addr),Y` directly.
      Excluding P keeps it as a multi-byte Pseudo, which regalloc's
      `_categorize_names` recognizes and allocates as a contiguous
      2-byte block with byte 0 at the lower address — exactly the
      shape `apply_indirect_base_prop` needs.
    * **Pseudos accessed by any volatile-flagged Mov** — splitting
      a volatile Pseudo across SSA versions and coloring them to
      separate ZP slots would route some accesses to one cell and
      some to another, which is incoherent for a memory cell that
      external observers expect to address through a single name.
      Keep volatile Pseudos as a single name so every access hits
      the same slot."""
    excluded: set[str] = set()
    for instr in fn.instructions:
        match instr:
            case asm_ast.LoadAddress(src=src, dst=dst):
                if isinstance(src, asm_ast.Pseudo):
                    excluded.add(src.name)
                if isinstance(dst, asm_ast.Pseudo):
                    excluded.add(dst.name)
            case (
                asm_ast.Inc(dst=dst)
                | asm_ast.Dec(dst=dst)
                | asm_ast.ArithmeticShiftLeft(dst=dst)
                | asm_ast.LogicalShiftRight(dst=dst)
                | asm_ast.RotateLeft(dst=dst)
                | asm_ast.RotateRight(dst=dst)
            ):
                if isinstance(dst, asm_ast.Pseudo):
                    excluded.add(dst.name)
            case asm_ast.Mov(src=src, dst=dst, is_volatile=True):
                # Volatile-marked accesses keep the Pseudo as one
                # name so every access hits the same storage cell.
                if isinstance(src, asm_ast.Pseudo):
                    excluded.add(src.name)
                if isinstance(dst, asm_ast.Pseudo):
                    excluded.add(dst.name)
    excluded |= _dptr_staged_pointer_names(fn.instructions)
    return excluded


def _dptr_staged_pointer_names(
    instrs: list[asm_ast.Type_instruction],
) -> set[str]:
    """Detect Pseudo names whose byte 0 AND byte 1 are staged
    through DPTR in the canonical 4-instruction sequence that
    `tac_to_asm._stage_dptr` emits:

        Mov(Pseudo(P, 0), Reg(A))
        Mov(Reg(A), Data("DPTR", 0))
        Mov(Pseudo(P, 1), Reg(A))
        Mov(Reg(A), Data("DPTR", 1))

    Strict-adjacency match — pre-SSA the lowering produces this
    exact 4-instruction window. Returns the set of Pseudo names
    that appear as the source of at least one such window."""
    out: set[str] = set()
    for i in range(len(instrs) - 3):
        name = _match_dptr_stage_4(instrs, i)
        if name is not None:
            out.add(name)
    return out


def _match_dptr_stage_4(
    instrs: list[asm_ast.Type_instruction], i: int,
) -> str | None:
    a, b, c, d = instrs[i:i + 4]
    if not all(isinstance(x, asm_ast.Mov) for x in (a, b, c, d)):
        return None
    # a: Mov(Pseudo(P, 0), Reg(A))
    if not (
        isinstance(a.src, asm_ast.Pseudo) and a.src.offset == 0
        and isinstance(a.dst, asm_ast.Reg)
        and isinstance(a.dst.reg, asm_ast.A)
    ):
        return None
    # b: Mov(Reg(A), Data("DPTR", 0))
    if not (
        isinstance(b.src, asm_ast.Reg) and isinstance(b.src.reg, asm_ast.A)
        and isinstance(b.dst, asm_ast.Data)
        and b.dst.name == "DPTR" and b.dst.offset == 0
    ):
        return None
    # c: Mov(Pseudo(P, 1), Reg(A)) — same Pseudo name as `a`.
    if not (
        isinstance(c.src, asm_ast.Pseudo)
        and c.src.name == a.src.name and c.src.offset == 1
        and isinstance(c.dst, asm_ast.Reg)
        and isinstance(c.dst.reg, asm_ast.A)
    ):
        return None
    # d: Mov(Reg(A), Data("DPTR", 1))
    if not (
        isinstance(d.src, asm_ast.Reg) and isinstance(d.src.reg, asm_ast.A)
        and isinstance(d.dst, asm_ast.Data)
        and d.dst.name == "DPTR" and d.dst.offset == 1
    ):
        return None
    return a.src.name


# ---------------------------------------------------------------------------
# Promotable byte-variable identification.
# ---------------------------------------------------------------------------


def _promotable_byte_vars(
    cfg: CFG, excluded: set[str],
) -> set[ByteVar]:
    """Set of (name, offset) pairs that asm-level SSA will rename."""
    out: set[ByteVar] = set()
    for blk in cfg.blocks.values():
        for instr in blk.instructions:
            for op in _all_pseudos_in(instr):
                if op.name in excluded:
                    continue
                out.add((op.name, op.offset))
    return out


def _all_pseudos_in(
    instr: asm_ast.Type_instruction,
) -> Iterable[asm_ast.Pseudo]:
    """Yield every Pseudo operand appearing anywhere in `instr`,
    regardless of use/def role. Used for promotable-variable
    candidate collection."""
    for op in _operand_fields(instr):
        if isinstance(op, asm_ast.Pseudo):
            yield op


def _operand_fields(
    instr: asm_ast.Type_instruction,
) -> Iterable[asm_ast.Type_operand]:
    """Every operand-typed field of `instr`. The shapes here mirror
    `replace_pseudoregisters._operands_in` plus the asm-level Phi."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Add(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Sub(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.And(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Or(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            yield s1
            yield s2
            yield dst
        case asm_ast.Inc(dst=dst):
            yield dst
        case asm_ast.Dec(dst=dst):
            yield dst
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            yield dst
        case asm_ast.LogicalShiftRight(dst=dst):
            yield dst
        case asm_ast.RotateLeft(dst=dst):
            yield dst
        case asm_ast.RotateRight(dst=dst):
            yield dst
        case asm_ast.Push(src=src):
            yield src
        case asm_ast.Pop(dst=dst):
            yield dst
        case asm_ast.Compare(left=left, right=right):
            yield left
            yield right
        case asm_ast.LoadAddress(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Phi(dst=dst, args=args):
            yield dst
            for a in args:
                yield a.source


def _defs_in(
    instr: asm_ast.Type_instruction,
) -> Iterable[asm_ast.Pseudo]:
    """Yield Pseudo defs of `instr`. Pseudos can be defined by
    `Mov / Pop / LoadAddress / Phi`. Add / Sub / And / Or / Xor
    have non-Pseudo dsts (Reg(A)) in current emissions, but the
    unrestricted IR allows Pseudo dsts; we yield those defensively
    too, treating the operand as both a use and a def. Inc / Dec /
    ASL / LSR / ROL / ROR similarly — but their names are excluded
    from promotability upstream, so the renaming will skip them."""
    match instr:
        case asm_ast.Mov(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.Pop(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.LoadAddress(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case asm_ast.Phi(dst=dst):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case (
            asm_ast.Add(dst=dst) | asm_ast.Sub(dst=dst)
            | asm_ast.And(dst=dst) | asm_ast.Or(dst=dst)
            | asm_ast.Xor(dst=dst)
        ):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst
        case (
            asm_ast.Inc(dst=dst) | asm_ast.Dec(dst=dst)
            | asm_ast.ArithmeticShiftLeft(dst=dst)
            | asm_ast.LogicalShiftRight(dst=dst)
            | asm_ast.RotateLeft(dst=dst)
            | asm_ast.RotateRight(dst=dst)
        ):
            if isinstance(dst, asm_ast.Pseudo):
                yield dst


def _uses_in(
    instr: asm_ast.Type_instruction,
) -> Iterable[asm_ast.Pseudo]:
    """Yield Pseudo uses of `instr`."""
    match instr:
        case asm_ast.Mov(src=src):
            if isinstance(src, asm_ast.Pseudo):
                yield src
        case asm_ast.Push(src=src):
            if isinstance(src, asm_ast.Pseudo):
                yield src
        case asm_ast.Compare(left=left, right=right):
            if isinstance(left, asm_ast.Pseudo):
                yield left
            if isinstance(right, asm_ast.Pseudo):
                yield right
        case asm_ast.Add(src=src) | asm_ast.Sub(src=src) | asm_ast.And(src=src) | asm_ast.Or(src=src):
            if isinstance(src, asm_ast.Pseudo):
                yield src
            # Pseudo dst (if any) is also a use. Same code path as
            # the read-modify-write detector; promotability already
            # excluded these names in current emissions, but defensive.
        case asm_ast.Xor(src1=s1, src2=s2):
            if isinstance(s1, asm_ast.Pseudo):
                yield s1
            if isinstance(s2, asm_ast.Pseudo):
                yield s2
        case asm_ast.Phi(args=args):
            for a in args:
                if isinstance(a.source, asm_ast.Pseudo):
                    yield a.source


# ---------------------------------------------------------------------------
# Pruned-SSA liveness.
# ---------------------------------------------------------------------------


def _compute_live_in(
    cfg: CFG, promotable: set[ByteVar],
) -> dict[int, set[ByteVar]]:
    """Backward dataflow over `promotable` (name, offset) pairs.
    Pruned-SSA gating: a Phi at block B for v is only useful if v
    is live-in at B."""
    gen: dict[int, set[ByteVar]] = {}
    kill: dict[int, set[ByteVar]] = {}
    for bid, blk in cfg.blocks.items():
        gen_b: set[ByteVar] = set()
        kill_b: set[ByteVar] = set()
        for instr in blk.instructions:
            for u in _uses_in(instr):
                key = (u.name, u.offset)
                if key in promotable and key not in kill_b:
                    gen_b.add(key)
            for d in _defs_in(instr):
                key = (d.name, d.offset)
                if key in promotable:
                    kill_b.add(key)
        gen[bid] = gen_b
        kill[bid] = kill_b
    live_in: dict[int, set[ByteVar]] = {b: set() for b in cfg.blocks}
    live_out: dict[int, set[ByteVar]] = {b: set() for b in cfg.blocks}
    changed = True
    while changed:
        changed = False
        for bid in cfg.blocks:
            new_out: set[ByteVar] = set()
            for s in cfg.blocks[bid].successors:
                new_out |= live_in[s]
            new_in = gen[bid] | (new_out - kill[bid])
            if new_out != live_out[bid] or new_in != live_in[bid]:
                live_out[bid] = new_out
                live_in[bid] = new_in
                changed = True
    return live_in


# ---------------------------------------------------------------------------
# Phi placement.
# ---------------------------------------------------------------------------


def _place_phis(
    cfg: CFG,
    promotable: set[ByteVar],
    df: dict[int, set[int]],
    live_in: dict[int, set[ByteVar]],
) -> dict[int, dict[ByteVar, asm_ast.Phi]]:
    """Place Phis at iterated DF of each promotable variable's
    definition blocks. Returns `phis_at[block_id][(name, offset)]`
    so the renaming pass can locate Phis after their dst has been
    renamed."""
    defs: dict[ByteVar, set[int]] = defaultdict(set)
    for bid, blk in cfg.blocks.items():
        for instr in blk.instructions:
            for d in _defs_in(instr):
                key = (d.name, d.offset)
                if key in promotable:
                    defs[key].add(bid)

    phis_at: dict[int, dict[ByteVar, asm_ast.Phi]] = defaultdict(dict)
    for v, def_blocks in defs.items():
        worklist = list(def_blocks)
        already_placed: set[int] = set()
        already_visited: set[int] = set(def_blocks)
        while worklist:
            x = worklist.pop()
            for y in df.get(x, ()):
                if y in already_placed:
                    continue
                if len(cfg.blocks[y].predecessors) < 2:
                    continue
                if v not in live_in.get(y, set()):
                    continue
                # Placeholder dst — _rename rewrites it to a fresh
                # versioned name when it visits the Phi's block.
                phi = asm_ast.Phi(
                    dst=asm_ast.Pseudo(name=v[0], offset=v[1]),
                    args=[],
                )
                _insert_phi(cfg.blocks[y], phi)
                phis_at[y][v] = phi
                already_placed.add(y)
                if y not in already_visited:
                    already_visited.add(y)
                    worklist.append(y)
    return phis_at


def _insert_phi(blk: BasicBlock, phi: asm_ast.Phi) -> None:
    if blk.instructions and isinstance(blk.instructions[0], asm_ast.Label):
        pos = 1
        while pos < len(blk.instructions) and isinstance(
            blk.instructions[pos], asm_ast.Phi,
        ):
            pos += 1
        blk.instructions.insert(pos, phi)
    else:
        blk.instructions.insert(0, phi)


# ---------------------------------------------------------------------------
# Renaming.
# ---------------------------------------------------------------------------


def _rename(
    cfg: CFG,
    fn: asm_ast.Function,
    promotable: set[ByteVar],
    idom: dict[int, int],
    children: dict[int, list[int]],
    block_label_of: dict[int, str],
    phis_at: dict[int, dict[ByteVar, asm_ast.Phi]],
) -> None:
    """Walk the dominator tree, renaming each promotable byte-var's
    defs to fresh versioned Pseudos and rewriting uses to current
    top-of-stack."""
    counters: dict[ByteVar, int] = {v: 0 for v in promotable}
    stacks: dict[ByteVar, list[asm_ast.Pseudo]] = {v: [] for v in promotable}

    for p in fn.params:
        # Each (param_name, offset) starts with the source spelling
        # at the param's bytes. We don't know offsets ahead of time
        # so use only those that appear in `promotable` (the rest
        # are presumably Frame-resident through the calling
        # convention and won't be renamed). Push the original
        # Pseudo for each.
        for v in promotable:
            if v[0] == p:
                stacks[v].append(asm_ast.Pseudo(name=p, offset=v[1]))

    def fresh(orig: ByteVar) -> asm_ast.Pseudo:
        counters[orig] += 1
        new_name = f"{orig[0]}.b{orig[1]}.v{counters[orig]}"
        # New Pseudo's offset is 0 — the byte position is encoded
        # into the name now, not the offset.
        return asm_ast.Pseudo(name=new_name, offset=0)

    def visit(bid: int) -> None:
        if bid not in cfg.blocks:
            return
        blk = cfg.blocks[bid]
        pushed: list[ByteVar] = []

        # 1. Rename Phi dsts.
        for instr in blk.instructions:
            if not isinstance(instr, asm_ast.Phi):
                continue
            if not isinstance(instr.dst, asm_ast.Pseudo):
                continue
            key = (instr.dst.name, instr.dst.offset)
            if key in promotable:
                new = fresh(key)
                stacks[key].append(new)
                pushed.append(key)
                instr.dst = new

        # 2. Rename uses then defs in non-Phi instructions.
        for i, instr in enumerate(blk.instructions):
            if isinstance(instr, asm_ast.Phi):
                continue
            blk.instructions[i] = _rewrite_instruction(
                instr, stacks, promotable, fresh, pushed,
            )

        # 3. Fill in Phi args at every CFG successor.
        pred_label = block_label_of.get(bid, "")
        for succ_id in blk.successors:
            for orig_var, phi in phis_at.get(succ_id, {}).items():
                if stacks[orig_var]:
                    src = stacks[orig_var][-1]
                else:
                    # No reaching def — use the original Pseudo as
                    # a defensive fallback.
                    src = asm_ast.Pseudo(name=orig_var[0], offset=orig_var[1])
                phi.args.append(asm_ast.AsmPhiArg(
                    pred_label=pred_label,
                    source=src,
                ))

        # 4. Recurse into dom-tree children.
        for child_id in children.get(bid, []):
            visit(child_id)

        # 5. Pop stacks.
        for orig in pushed:
            stacks[orig].pop()

    visit(ENTRY_ID)


def _rewrite_instruction(
    instr: asm_ast.Type_instruction,
    stacks: dict[ByteVar, list[asm_ast.Pseudo]],
    promotable: set[ByteVar],
    fresh,  # callable
    pushed: list[ByteVar],
) -> asm_ast.Type_instruction:
    """Rewrite every promotable Pseudo use in `instr` to its current
    stack top, then mint fresh SSA names for every promotable Pseudo
    def. Returns the rewritten instruction."""

    def rewrite_use(op: asm_ast.Type_operand) -> asm_ast.Type_operand:
        if isinstance(op, asm_ast.Pseudo):
            key = (op.name, op.offset)
            if key in promotable:
                if stacks[key]:
                    return stacks[key][-1]
        return op

    def rewrite_def(op: asm_ast.Type_operand) -> asm_ast.Type_operand:
        if isinstance(op, asm_ast.Pseudo):
            key = (op.name, op.offset)
            if key in promotable:
                new = fresh(key)
                stacks[key].append(new)
                pushed.append(key)
                return new
        return op

    # By instruction shape: handle uses first (so a self-write reads
    # the OLD value), then defs.
    match instr:
        case asm_ast.Mov(src=src, dst=dst, is_volatile=v):
            new_src = rewrite_use(src)
            new_dst = rewrite_def(dst)
            return asm_ast.Mov(src=new_src, dst=new_dst, is_volatile=v)
        case asm_ast.Add(src=src, dst=dst):
            new_src = rewrite_use(src)
            new_dst = rewrite_def(dst)
            return asm_ast.Add(src=new_src, dst=new_dst)
        case asm_ast.Sub(src=src, dst=dst):
            new_src = rewrite_use(src)
            new_dst = rewrite_def(dst)
            return asm_ast.Sub(src=new_src, dst=new_dst)
        case asm_ast.And(src=src, dst=dst):
            new_src = rewrite_use(src)
            new_dst = rewrite_def(dst)
            return asm_ast.And(src=new_src, dst=new_dst)
        case asm_ast.Or(src=src, dst=dst):
            new_src = rewrite_use(src)
            new_dst = rewrite_def(dst)
            return asm_ast.Or(src=new_src, dst=new_dst)
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            new_s1 = rewrite_use(s1)
            new_s2 = rewrite_use(s2)
            new_dst = rewrite_def(dst)
            return asm_ast.Xor(src1=new_s1, src2=new_s2, dst=new_dst)
        case asm_ast.Inc(dst=dst):
            return asm_ast.Inc(dst=rewrite_use(dst))
        case asm_ast.Dec(dst=dst):
            return asm_ast.Dec(dst=rewrite_use(dst))
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            return asm_ast.ArithmeticShiftLeft(dst=rewrite_use(dst))
        case asm_ast.LogicalShiftRight(dst=dst):
            return asm_ast.LogicalShiftRight(dst=rewrite_use(dst))
        case asm_ast.RotateLeft(dst=dst):
            return asm_ast.RotateLeft(dst=rewrite_use(dst))
        case asm_ast.RotateRight(dst=dst):
            return asm_ast.RotateRight(dst=rewrite_use(dst))
        case asm_ast.Push(src=src):
            return asm_ast.Push(src=rewrite_use(src))
        case asm_ast.Pop(dst=dst):
            return asm_ast.Pop(dst=rewrite_def(dst))
        case asm_ast.Compare(left=left, right=right):
            return asm_ast.Compare(
                left=rewrite_use(left), right=rewrite_use(right),
            )
        case asm_ast.LoadAddress(src=src, dst=dst):
            # `src` is address-taken (excluded from promotion), so
            # rewrite_use will leave it untouched. `dst` may be
            # promotable.
            return asm_ast.LoadAddress(
                src=rewrite_use(src), dst=rewrite_def(dst),
            )
        case _:
            # Call / Jump / Branch / Label / Return / Ret /
            # ClearCarry / SetCarry / AllocateStack — no Pseudo
            # operands.
            return instr
