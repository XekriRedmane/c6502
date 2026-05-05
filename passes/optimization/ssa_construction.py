"""TAC SSA construction.

Converts a `tac_ast.Function` to SSA form. Promotable Vars get
renamed to a fresh `<orig>.<N>` per definition and joined at control-
flow merge points by `Phi` instructions; everything else (statics,
address-taken locals, aggregate-typed Vars, function names) passes
through unchanged.

A Var is **promotable** iff:
  * its symbol is `LocalAttr` (block-scope auto, function parameter,
    or a TAC temp `%N` minted by `c99_to_tac`);
  * its type is scalar — `Int` / `Long` / `LongLong` / `UInt` /
    `ULong` / `ULongLong` / `Char` / `SChar` / `UChar` / `Float` /
    `Double` / `Pointer`. Arrays, structs, and unions are excluded
    because their member access goes through `GetAddress` + `Load` /
    `Store` and so they're effectively address-taken;
  * its address is never taken — no `GetAddress` instruction in the
    function names it as `operand`.

Algorithm (Cytron et al. 1991):
  1. Pre-step: if the function body starts with a `Label`, prepend a
     synthetic preheader `Label` so that the first real block has
     ENTRY as its sole predecessor — keeps Phi pred_label tagging
     well-defined for any later block whose predecessors include the
     ENTRY-path edge. Otherwise no change.
  2. Build the CFG and `_ensure_block_labels` so every reachable
     block has a leading `Label` to use as its `PhiArg.pred_label`.
  3. Compute idom + DF + dominator-tree children.
  4. Find Defs(v) (the set of blocks defining v) for every
     promotable v. Place an empty `Phi(dst=Var(v), args=[])` at the
     iterated dominance frontier of Defs(v), inserted right after
     the block's leading `Label`.
  5. Renaming. Walk the dominator tree from ENTRY in pre-order:
     for each block B,
       a. rename each Phi's `dst` to a fresh SSA name and push it
          onto the per-Var stack;
       b. for each non-Phi instruction in source order, rewrite
          every Var use to the current stack top, then rewrite every
          Var def to a fresh SSA name (pushing it);
       c. for each CFG successor S of B, append
          `PhiArg(pred_label=B's leading label, source=Var(top of
          stack[orig_var]))` to every Phi at S — using a sidecar map
          (orig_var → Phi instance) recorded during placement, since
          the Phi's dst has been renamed by the time we read it;
       d. recurse into B's dominator-tree children;
       e. pop everything pushed in (a) and (b).

Parameters' initial SSA name IS their original name. The renaming
pass pre-populates each promotable param's stack with the source
spelling at ENTRY, so the first read of `p` resolves to `p` (and the
function's `params` field doesn't need rewriting). Subsequent
definitions of `p` mint `p.1`, `p.2`, ...

Each fresh SSA name gets a `LocalAttr` symbol mirroring the original
Var's type, registered in the supplied `SymbolTable` so `tac_to_asm`
can size frame slots after de-SSA.
"""

from __future__ import annotations

from collections import defaultdict
from dataclasses import dataclass, replace as dc_replace
from typing import Iterable

import c99_ast
import tac_ast
from passes.optimization.cfg import (
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
from passes.optimization.var_visit import defs_in, uses_in, vals_in
from passes.type_checking import LocalAttr, Symbol, SymbolTable


_SCALAR_TYPES: tuple[type, ...] = (
    c99_ast.Int, c99_ast.Long, c99_ast.LongLong,
    c99_ast.UInt, c99_ast.ULong, c99_ast.ULongLong,
    c99_ast.Char, c99_ast.SChar, c99_ast.UChar,
    c99_ast.Float, c99_ast.Double,
    c99_ast.Pointer,
)


def to_ssa(
    fn: tac_ast.Function, symbols: SymbolTable,
) -> tuple[tac_ast.Function, set[str]]:
    """Convert `fn` to SSA form. New SSA names are registered in
    `symbols`. Returns `(rewritten_fn, ssa_dsts)` where `ssa_dsts`
    is the set of Var names whose only definition was minted by
    this pass — equivalently, the set of dsts that downstream
    SSA-aware passes (copy propagation, dead-store elimination) may
    safely treat as single-definition function-local values.

    `ssa_dsts` includes every fresh name `<orig>.<n>` produced by
    renaming. It does NOT include the original spelling of a
    parameter (the param's initial SSA value), because params are
    "defined" implicitly at function entry — there's no instruction
    we'd want to drop or substitute through. It does NOT include
    statics, address-taken locals, or aggregates — those keep their
    original names and are written through real Stores or
    cross-function calls, so neither single-def nor function-local
    holds for them."""
    fn = _maybe_prepend_preheader(fn)
    cfg = build_cfg(fn)
    _ensure_block_labels(cfg, fn.name)

    promotable = _identify_promotable(fn, cfg, symbols)
    if not promotable:
        return cfg_to_function(fn, cfg), set()

    idom = immediate_dominators(cfg)
    df = dominance_frontiers(cfg)
    children = dominator_tree_children(idom)
    live_in = _compute_live_in(cfg, promotable)
    # Only real blocks have a leading Label (ENTRY / EXIT are empty
    # sentinels). PhiArg.pred_label tagging only references real
    # blocks, so we don't need labels for the sentinels.
    block_label_of = {
        bid: _block_label(cfg.blocks[bid])
        for bid in idom
        if bid not in (ENTRY_ID, EXIT_ID) and cfg.blocks[bid].instructions
    }

    phis_at = _place_phis(cfg, promotable, df, live_in)
    ssa_dsts = _rename(
        cfg, fn, promotable, idom, children,
        block_label_of, phis_at, symbols,
    )

    return cfg_to_function(fn, cfg), ssa_dsts


# ---------------------------------------------------------------------------
# Pre-pass: synthetic preheader
# ---------------------------------------------------------------------------


def _maybe_prepend_preheader(fn: tac_ast.Function) -> tac_ast.Function:
    """If the function body starts with a Label, prepend a fresh
    synthetic Label so the first real block has ENTRY as its only
    predecessor. Otherwise return `fn` unchanged.

    Why: any block whose first instruction is a labeled jump-target
    can have predecessors beyond just ENTRY (e.g., a back-edge from a
    loop tail). In SSA construction such a block can host Phis whose
    pred_labels include the ENTRY edge — but ENTRY has no
    instructions, hence no label to tag with. The preheader gives us
    a real labeled block to point Phi pred_labels at, which de-SSA
    can later reach when emitting entry-path Copies.

    The minted name is `.<funcname>@ssa_preheader@<N>`; the function-
    name prefix matches the user-label convention (label_resolution
    produces `.<funcname>@<orig>`) and keeps SSA labels disjoint
    across functions in the same program."""
    if not fn.instructions:
        return fn
    if not isinstance(fn.instructions[0], tac_ast.Label):
        return fn
    existing = {
        i.name for i in fn.instructions if isinstance(i, tac_ast.Label)
    }
    counter = 0
    while True:
        name = f".{fn.name}@ssa_preheader@{counter}"
        counter += 1
        if name not in existing:
            break
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params),
        instructions=[tac_ast.Label(name=name)] + list(fn.instructions),
    )


# ---------------------------------------------------------------------------
# Block-label normalization
# ---------------------------------------------------------------------------


def _ensure_block_labels(cfg: CFG, fn_name: str) -> None:
    """For every real block whose first instruction isn't a `Label`,
    prepend a fresh `Label`. Phi pred_label tagging needs every
    block on the path to have an addressable label.

    Minted names use the `.<funcname>@ssa_block@<N>` convention so
    SSA labels stay disjoint across functions in the same program."""
    existing = {
        i.name for blk in cfg.blocks.values()
        for i in blk.instructions if isinstance(i, tac_ast.Label)
    }
    counter = 0
    for bid in cfg.block_order:
        blk = cfg.blocks[bid]
        if blk.instructions and isinstance(blk.instructions[0], tac_ast.Label):
            continue
        while True:
            name = f".{fn_name}@ssa_block@{counter}"
            counter += 1
            if name not in existing:
                existing.add(name)
                break
        blk.instructions.insert(0, tac_ast.Label(name=name))


def _block_label(blk: BasicBlock) -> str:
    """The block's leading `Label` name. Caller must have run
    `_ensure_block_labels` first; it's a programming error to call
    this on a block without a leading Label."""
    if not blk.instructions or not isinstance(blk.instructions[0], tac_ast.Label):
        raise AssertionError(
            f"block {blk.id} has no leading label; "
            "call _ensure_block_labels first"
        )
    return blk.instructions[0].name


# ---------------------------------------------------------------------------
# Promotable-variable identification
# ---------------------------------------------------------------------------


def _identify_promotable(
    fn: tac_ast.Function, cfg: CFG, symbols: SymbolTable,
) -> set[str]:
    """A Var is promotable iff (a) its symbol entry is `LocalAttr`,
    (b) its type is scalar (excludes `Array` / `Structure` / `Union`
    and `FunType`), and (c) it's never the operand of `GetAddress`.
    """
    address_taken: set[str] = set()
    candidates: set[str] = set()
    # Walk all instructions across all blocks to find candidate Vars
    # and to flag any that have GetAddress applied to them.
    for blk in cfg.blocks.values():
        for instr in blk.instructions:
            for v in _all_var_names_in(instr):
                candidates.add(v)
            if isinstance(instr, tac_ast.GetAddress):
                operand = instr.operand
                if isinstance(operand, tac_ast.Var):
                    address_taken.add(operand.name)

    promotable: set[str] = set()
    for name in candidates:
        if name in address_taken:
            continue
        sym = symbols.get(name)
        if sym is None:
            continue
        if not isinstance(sym.attrs, LocalAttr):
            continue
        if not isinstance(sym.type, _SCALAR_TYPES):
            continue
        promotable.add(name)
    return promotable


def _all_var_names_in(instr: tac_ast.Type_instruction) -> Iterable[str]:
    """Every Var name (use or def) that appears anywhere in `instr`.
    Used only for candidate collection in `_identify_promotable`;
    it intentionally ignores the use/def distinction."""
    for v in vals_in(instr):
        if isinstance(v, tac_ast.Var):
            yield v.name


# ---------------------------------------------------------------------------
# Phi placement
# ---------------------------------------------------------------------------


def _compute_live_in(
    cfg: CFG, promotable: set[str],
) -> dict[int, set[str]]:
    """Backward dataflow over `promotable` vars. `live_in[B]` is the
    set of promotable vars read on some path from `B`'s entry before
    being redefined. Used to prune Phi placement: a Phi at block B
    for var v is only useful if v is live-in at B (otherwise the
    Phi's dst is dead, and the Phi's predecessor-side de-SSA Copies
    introduce uninitialized reads from temps that had no value
    before the join). Standard pruned-SSA gating per Cytron §5.1."""
    gen: dict[int, set[str]] = {}
    kill: dict[int, set[str]] = {}
    for bid, blk in cfg.blocks.items():
        gen_b: set[str] = set()
        kill_b: set[str] = set()
        for instr in blk.instructions:
            for u in uses_in(instr):
                if u.name in promotable and u.name not in kill_b:
                    gen_b.add(u.name)
            for d in defs_in(instr):
                if d.name in promotable:
                    kill_b.add(d.name)
        gen[bid] = gen_b
        kill[bid] = kill_b
    live_in: dict[int, set[str]] = {b: set() for b in cfg.blocks}
    live_out: dict[int, set[str]] = {b: set() for b in cfg.blocks}
    changed = True
    while changed:
        changed = False
        for bid in cfg.blocks:
            new_out: set[str] = set()
            for s in cfg.blocks[bid].successors:
                new_out |= live_in[s]
            new_in = gen[bid] | (new_out - kill[bid])
            if new_out != live_out[bid] or new_in != live_in[bid]:
                live_out[bid] = new_out
                live_in[bid] = new_in
                changed = True
    return live_in


def _place_phis(
    cfg: CFG,
    promotable: set[str],
    df: dict[int, set[int]],
    live_in: dict[int, set[str]],
) -> dict[int, dict[str, tac_ast.Phi]]:
    """For each promotable Var v, find its definition blocks and
    insert empty Phis at every block in the iterated dominance
    frontier where v is live-in (pruned SSA, Cytron §5.1). Returns
    a mapping `phis_at[block_id][orig_var] = Phi instance` so the
    renaming pass can locate Phis after their dst has been renamed.
    """
    # Defs(v) = blocks that define v.
    defs: dict[str, set[int]] = defaultdict(set)
    for bid, blk in cfg.blocks.items():
        for instr in blk.instructions:
            for d in defs_in(instr):
                if d.name in promotable:
                    defs[d.name].add(bid)

    phis_at: dict[int, dict[str, tac_ast.Phi]] = defaultdict(dict)
    for v, def_blocks in defs.items():
        # IDF via worklist: start with Defs(v), expand by DF each
        # round, until fixed point.
        worklist = list(def_blocks)
        already_placed: set[int] = set()
        already_visited: set[int] = set(def_blocks)
        while worklist:
            x = worklist.pop()
            for y in df.get(x, ()):
                if y in already_placed:
                    continue
                # Only place a Phi in a block with multiple
                # predecessors; DF guarantees this but we double-
                # check defensively (DF[x] may include x itself for
                # a back-edge into x).
                if len(cfg.blocks[y].predecessors) < 2:
                    continue
                # Prune: skip if v isn't live-in at y. The Phi would
                # have a dead dst, and its predecessor-side de-SSA
                # Copies would read uninitialized temps for any path
                # that reaches y without first defining v.
                if v not in live_in.get(y, set()):
                    continue
                phi = tac_ast.Phi(
                    dst=tac_ast.Var(name=v), args=[],
                )
                _insert_phi(cfg.blocks[y], phi)
                phis_at[y][v] = phi
                already_placed.add(y)
                if y not in already_visited:
                    already_visited.add(y)
                    worklist.append(y)
    return phis_at


def _insert_phi(blk: BasicBlock, phi: tac_ast.Phi) -> None:
    """Insert `phi` after the block's leading `Label` (or at
    position 0 if no leading Label, though `_ensure_block_labels`
    should have ensured one)."""
    if blk.instructions and isinstance(blk.instructions[0], tac_ast.Label):
        # Insert after the Label and after any existing Phis
        # (consistent ordering).
        pos = 1
        while pos < len(blk.instructions) and isinstance(
            blk.instructions[pos], tac_ast.Phi,
        ):
            pos += 1
        blk.instructions.insert(pos, phi)
    else:
        blk.instructions.insert(0, phi)


# ---------------------------------------------------------------------------
# Renaming
# ---------------------------------------------------------------------------


def _rename(
    cfg: CFG,
    fn: tac_ast.Function,
    promotable: set[str],
    idom: dict[int, int],
    children: dict[int, list[int]],
    block_label_of: dict[int, str],
    phis_at: dict[int, dict[str, tac_ast.Phi]],
    symbols: SymbolTable,
) -> set[str]:
    """Rename promotable Vars to fresh SSA names. Returns the set of
    fresh names introduced (`ssa_dsts` in `to_ssa`'s contract)."""
    counters: dict[str, int] = {v: 0 for v in promotable}
    stacks: dict[str, list[str]] = {v: [] for v in promotable}
    ssa_dsts: set[str] = set()

    # Parameters' initial SSA name is their original name. Push it
    # onto the stack at entry so the first read resolves to it. We
    # also include the param's original name in `ssa_dsts` if the
    # param is promotable — its only "def" is the function entry,
    # so it obeys the SSA single-def invariant just like a renamed
    # value. (An address-taken param isn't promotable; it's
    # excluded automatically.)
    for p in fn.params:
        if p in promotable:
            stacks[p].append(p)
            ssa_dsts.add(p)

    def fresh(orig: str) -> str:
        counters[orig] += 1
        new_name = f"{orig}.{counters[orig]}"
        sym = symbols.get(orig)
        if sym is not None:
            symbols[new_name] = Symbol(type=sym.type, attrs=LocalAttr())
        ssa_dsts.add(new_name)
        return new_name

    def visit(bid: int) -> None:
        if bid not in cfg.blocks:
            return
        blk = cfg.blocks[bid]
        pushed: list[str] = []  # original names whose stacks we'll pop

        # 1. Rename Phi dsts in this block.
        for instr in blk.instructions:
            if not isinstance(instr, tac_ast.Phi):
                continue
            orig = instr.dst.name
            if orig in promotable:
                new = fresh(orig)
                stacks[orig].append(new)
                pushed.append(orig)
                instr.dst = tac_ast.Var(name=new)

        # 2. Rename uses then defs in non-Phi instructions.
        for i, instr in enumerate(blk.instructions):
            if isinstance(instr, tac_ast.Phi):
                continue
            blk.instructions[i] = _rewrite_instruction(
                instr, stacks, promotable, fresh, pushed,
            )

        # 3. Fill in Phi args at every CFG successor.
        pred_label = block_label_of.get(bid, _block_label(blk) if bid != ENTRY_ID and bid != EXIT_ID else "")
        for succ_id in blk.successors:
            for orig_var, phi in phis_at.get(succ_id, {}).items():
                if stacks[orig_var]:
                    src_name = stacks[orig_var][-1]
                else:
                    # Reading a Var that has no reaching definition
                    # on this path. Use the original name (safe
                    # fallback — the post-de-SSA Copy will read
                    # whatever was there). This shouldn't happen
                    # for well-formed C programs.
                    src_name = orig_var
                phi.args.append(tac_ast.PhiArg(
                    pred_label=pred_label,
                    source=tac_ast.Var(name=src_name),
                ))

        # 4. Recurse into dom-tree children.
        for child_id in children.get(bid, []):
            visit(child_id)

        # 5. Pop stacks. Each `pushed` entry corresponds to one push.
        for orig in pushed:
            stacks[orig].pop()

    visit(ENTRY_ID)
    return ssa_dsts


def _rewrite_instruction(
    instr: tac_ast.Type_instruction,
    stacks: dict[str, list[str]],
    promotable: set[str],
    fresh: callable,
    pushed: list[str],
) -> tac_ast.Type_instruction:
    """Rewrite every promotable Var use to its current stack top,
    then mint fresh SSA names for every promotable Var def. Returns
    the rewritten instruction (a new dataclass) to keep the function
    pure-ish — uses are read first so a single-instruction
    self-write like `Copy(x, x)` reads the OLD x and writes a NEW
    x."""

    def rewrite_use(v: tac_ast.Type_val) -> tac_ast.Type_val:
        if isinstance(v, tac_ast.Var) and v.name in promotable:
            if stacks[v.name]:
                return tac_ast.Var(name=stacks[v.name][-1])
            # Var read without prior def. Same defensive fallback as
            # in `_rename`'s Phi-arg step.
            return v
        return v

    def rewrite_def(v: tac_ast.Type_val) -> tac_ast.Type_val:
        if isinstance(v, tac_ast.Var) and v.name in promotable:
            new = fresh(v.name)
            stacks[v.name].append(new)
            pushed.append(v.name)
            return tac_ast.Var(name=new)
        return v

    match instr:
        case tac_ast.Ret(val=val):
            if val is None:
                return tac_ast.Ret(val=None)
            return tac_ast.Ret(val=rewrite_use(val))
        case tac_ast.SignExtend(src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.SignExtend(src=s2, dst=d2)
        case tac_ast.ZeroExtend(src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.ZeroExtend(src=s2, dst=d2)
        case tac_ast.Truncate(src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.Truncate(src=s2, dst=d2)
        case tac_ast.IntToFloat(src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.IntToFloat(src=s2, dst=d2)
        case tac_ast.IntToDouble(src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.IntToDouble(src=s2, dst=d2)
        case tac_ast.FloatToInt(src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.FloatToInt(src=s2, dst=d2)
        case tac_ast.DoubleToInt(src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.DoubleToInt(src=s2, dst=d2)
        case tac_ast.FloatToDouble(src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.FloatToDouble(src=s2, dst=d2)
        case tac_ast.DoubleToFloat(src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.DoubleToFloat(src=s2, dst=d2)
        case tac_ast.Unary(op=op, src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.Unary(op=op, src=s2, dst=d2)
        case tac_ast.Binary(op=op, src1=s1, src2=s2, dst=d):
            s1b = rewrite_use(s1)
            s2b = rewrite_use(s2)
            d2 = rewrite_def(d)
            return tac_ast.Binary(op=op, src1=s1b, src2=s2b, dst=d2)
        case tac_ast.Copy(src=s, dst=d):
            s2 = rewrite_use(s)
            d2 = rewrite_def(d)
            return tac_ast.Copy(src=s2, dst=d2)
        case tac_ast.GetAddress(operand=o, dst=d):
            # `o` names a storage cell, not a value — don't rename.
            # The promotable set excludes any Var that's a GetAddress
            # operand, so this is consistent.
            d2 = rewrite_def(d)
            return tac_ast.GetAddress(operand=o, dst=d2)
        case tac_ast.Load(src_ptr=p, dst=d):
            p2 = rewrite_use(p)
            d2 = rewrite_def(d)
            return tac_ast.Load(src_ptr=p2, dst=d2)
        case tac_ast.Store(src=s, dst_ptr=p):
            s2 = rewrite_use(s)
            p2 = rewrite_use(p)
            return tac_ast.Store(src=s2, dst_ptr=p2)
        case tac_ast.IndexedLoad(name=n, index=i, dst=d):
            i2 = rewrite_use(i)
            d2 = rewrite_def(d)
            return tac_ast.IndexedLoad(name=n, index=i2, dst=d2)
        case tac_ast.IndexedStore(address=a, index=i, src=s):
            i2 = rewrite_use(i)
            s2 = rewrite_use(s)
            return tac_ast.IndexedStore(address=a, index=i2, src=s2)
        case tac_ast.Jump(target=t):
            return tac_ast.Jump(target=t)
        case tac_ast.JumpIfTrue(condition=c, target=t):
            return tac_ast.JumpIfTrue(condition=rewrite_use(c), target=t)
        case tac_ast.JumpIfFalse(condition=c, target=t):
            return tac_ast.JumpIfFalse(condition=rewrite_use(c), target=t)
        case tac_ast.Label(name=n):
            return tac_ast.Label(name=n)
        case tac_ast.FunctionCall(name=n, args=args, dst=d):
            new_args = [rewrite_use(a) for a in args]
            new_d = rewrite_def(d) if d is not None else None
            return tac_ast.FunctionCall(name=n, args=new_args, dst=new_d)
        case tac_ast.IndirectCall(ptr=p, args=args, dst=d):
            new_p = rewrite_use(p)
            new_args = [rewrite_use(a) for a in args]
            new_d = rewrite_def(d) if d is not None else None
            return tac_ast.IndirectCall(ptr=new_p, args=new_args, dst=new_d)
    return instr
