"""Asm-level register allocation.

Byte-granular: every node in the interference graph is exactly 1
byte wide (since asm-SSA has already split multi-byte values into
independent byte-versioned variables). Coloring runs PEO + greedy
fit, mirroring the TAC version's design but operating on the asm
CFG and asm SSA-renamed names.

Reuses `Coloring` and `Pool` from `passes.optimization.register_
allocation` — those types are agnostic to TAC vs asm; only the
interference-graph build and the PEO derivation differ.

Pool selection follows the same rule as the TAC version:
  * `lives_across_call=True` → callee-saved first (saved by the
    function's prologue/epilogue around any nested call).
  * `lives_across_call=False` → caller-saved first (no save/restore
    overhead; clobbered by any nested call we don't make).
Spills land in `Coloring.spilled` and `replace_pseudoregisters`
falls back to Frame allocation for them.

# HwReg coloring (X / Y)

A subset of nodes can be pinned into the 6502's X or Y index
register instead of a ZP byte slot. Eligibility is precomputed by
`hwreg_eligibility.scan_function`: every def/use must be
representable as an LDX/LDY/STX/STY/INX/DEX/CPX/CPY-style op.
The pre-pass then tries to assign X or Y to eligible nodes
carrying a `hint` (typically a Pseudo that feeds the
`Mov(P, A); Mov(A, X|Y)` index-setup chain before each
IndexedData access).

Hard constraints on a HwReg-colored node:
  * Width 1, offset 0 (single-byte SSA-renamed name).
  * `lives_across_call == False` — helpers clobber A, X, Y.
  * No interference with another node already pinned to the same
    HwReg.

The X and Y registers are independent resources: an X-pinned node
and a Y-pinned node may interfere freely. Multiple non-interfering
nodes can share the same HwReg color.

HwReg colors live in `Coloring.hwreg_assignments` (a parallel dict
mapping name → "X" | "Y"); the existing ZP `assignments` and
`spilled` paths are unchanged for non-HwReg names.
"""

from __future__ import annotations

import asm_ast
from passes.optimization.pool import Pool
from passes.optimization.register_allocation import (
    Coloring,
    _blocked_bytes,
    _find_fit,
)
from passes.optimization.interference import InterferenceGraph
from passes.optimization_asm.cfg import (
    ENTRY_ID,
    build_cfg,
    dominator_tree_children,
    immediate_dominators,
)
from passes.optimization_asm.hwreg_eligibility import HwRegEligibility
from passes.optimization_asm.liveness import Liveness, _defs_in


def color_graph(
    fn: asm_ast.Function,
    graph: InterferenceGraph,
    *,
    pool: Pool | None = None,
    blocked_addrs: set[int] | None = None,
    hwreg_eligibility: HwRegEligibility | None = None,
    allowed_range: range | None = None,
    liveness: Liveness | None = None,
    rep_map: dict[str, str] | None = None,
) -> Coloring:
    """Color `graph`'s nodes onto ZP byte addresses, optionally
    pinning HwReg-eligible nodes into X / Y first. Returns a
    `Coloring` with every graph node either in `assignments`,
    in `hwreg_assignments`, or in `spilled`.

    `blocked_addrs` is an optional set of ZP byte addresses that
    must NOT be assigned to any node. Used by the ZP-ABI path to
    reserve incoming-param slots: a function declared
    `__attribute__((zp_abi))` has its parameters at fixed ZP
    addresses on entry, and body locals must avoid those colors
    so they don't clobber params mid-computation. Conservative —
    blocks the addresses for the entire function rather than only
    while params are live; the simpler approach is correct and
    the few wasted slots are typically negligible compared to
    the savings of frame elimination.

    `hwreg_eligibility`, when supplied, drives the HwReg pre-pass:
    nodes in `hints_x`/`hints_y` are tried first against the X / Y
    register, succeeding when graph constraints allow. Eligibility
    is graph-rep-level (the caller is responsible for projecting
    pre-coalescing names through any rep_map before passing in).

    `allowed_range`, when provided, REPLACES the pool-based
    caller/callee partition. Every node's color is drawn from
    this range exclusively; `lives_across_call` no longer drives
    color choice, because by construction every byte in
    `allowed_range` is safe to hold values across any call the
    caller knows about (the caller — typically the optimizer
    driver feeding the per-function private range from
    `passes.zp_local_allocation`).

    The range may extend above `$FF`; addresses there assemble
    as 3-byte absolute mode instead of 2-byte zero-page, but
    semantics are identical. HwReg-pinning (X / Y) is still
    gated by `lives_across_call` because X and Y are 6502
    hardware registers always clobbered by JSR — orthogonal to
    the ZP-byte question."""
    if pool is None:
        pool = Pool()
    blocked_addrs = blocked_addrs or set()
    peo = _perfect_elimination_order(fn, graph)
    assignments: dict[str, int] = {}
    hwreg_assignments: dict[str, str] = {}
    spilled: set[str] = set()

    if hwreg_eligibility is not None:
        _try_hwreg_assign(
            graph, hwreg_eligibility, hwreg_assignments,
            fn=fn, liveness=liveness, rep_map=rep_map,
        )

    for name in peo:
        if name in hwreg_assignments:
            continue
        node = graph.nodes[name]
        blocked = _blocked_bytes(name, graph, assignments) | blocked_addrs
        if allowed_range is not None:
            base = _find_fit(allowed_range, node.width, blocked)
        elif node.lives_across_call:
            base = _find_fit(pool.callee_saved(), node.width, blocked)
        else:
            base = _find_fit(pool.caller_saved(), node.width, blocked)
            if base is None:
                base = _find_fit(pool.callee_saved(), node.width, blocked)
        if base is None:
            spilled.add(name)
        else:
            assignments[name] = base
    return Coloring(
        assignments=assignments,
        spilled=spilled,
        pool=pool,
        hwreg_assignments=hwreg_assignments,
    )


def _try_hwreg_assign(
    graph: InterferenceGraph,
    eligibility: HwRegEligibility,
    hwreg_assignments: dict[str, str],
    *,
    fn: asm_ast.Function | None = None,
    liveness: Liveness | None = None,
    rep_map: dict[str, str] | None = None,
) -> None:
    """Greedy pre-coloring: walk hinted candidates, assigning each
    to its preferred HwReg (X for hints_x, Y for hints_y). When the
    preferred reg is blocked by an already-assigned interference
    neighbor, fall back to the OTHER reg before giving up — multiple
    candidates with overlapping live ranges and the same X-hint
    (typical in unrolled loops where every iteration's index value
    feeds an IndexedData chain) can then partition naturally between
    X and Y, saving the index-setup chain on each.

    Mutates `hwreg_assignments` in place.

    Assignment order is hints_x first (X-preferred), then hints_y
    (Y-preferred). Within each bucket we iterate in sorted name order
    for determinism. A name in BOTH hints_x and hints_y is processed
    in the X bucket (X is the tac_to_asm target).

    Note: every assignment is at most one HwReg color per name. A
    name's preferred reg is decided once, with a single fallback to
    the other reg, and that decision is final. (No re-shuffling once
    assigned — the assigned name's color satisfies all its
    constraints by construction.)"""

    # Precompute the per-instruction HwReg clobber map and the
    # def-position of every Pseudo. The interference graph alone
    # doesn't track interference with hardware registers (it only
    # has edges between Pseudos), so a Pseudo whose live range
    # crosses an instruction that writes Reg(X) / Reg(Y) — but
    # without a graph-neighbor at the writing position — would
    # otherwise slip through. Liveness is optional: callers that
    # didn't supply it get the legacy (unsafe) behavior; the
    # optimizer driver always passes it.
    #
    # `rep_map` is the coalescing result (`CoalesceResult.
    # representative`): the eligibility set is built at REP names
    # (post-coalescing), but liveness is computed at pre-coalescing
    # SSA names. We project every live-set element through `rep_map`
    # to get the rep-level live set before comparing.
    clobber_positions: dict[str, list[tuple[int, int]]] = {
        "X": [],
        "Y": [],
    }
    # All def positions per rep — a coalesced rep may have several
    # contributing defs (the original SSA names had one each, but
    # after coalescing they share a name). Any one of those is a
    # legitimate write of `rep` into `reg` after pinning, not a
    # clobber.
    def_positions_rep: dict[str, set[tuple[int, int]]] = {}
    rep_map = rep_map or {}

    def _resolve_rep(name: str) -> str:
        cur = name
        while cur in rep_map:
            cur = rep_map[cur]
        return cur

    if fn is not None and liveness is not None:
        for bid, blk in liveness.cfg.blocks.items():
            for idx, instr in enumerate(blk.instructions):
                for letter in _instr_writes_hwregs(instr):
                    clobber_positions[letter].append((bid, idx))
                for d in _defs_in(instr):
                    rep_name = _resolve_rep(d.name)
                    def_positions_rep.setdefault(
                        rep_name, set(),
                    ).add((bid, idx))

    def _is_self_transfer_chain(
        bid: int, idx: int, name: str,
    ) -> bool:
        """A `Mov(Reg(A), Reg(R))` whose immediately preceding
        instruction is `Mov(Pseudo N, Reg(A))` with `rep(N) == name`
        is the index-setup chain for `name`. After pinning `name` to
        `R`, the pair becomes `Mov(Reg(R), Reg(A)); Mov(Reg(A),
        Reg(R))` — a self-transfer that `apply_coloring._rewrite_
        redundant_transfers` later drops. So it isn't really a
        clobber of `R` for `name`'s liveness: the value placed in
        `R` IS `name`'s value."""
        blk = liveness.cfg.blocks.get(bid)
        if blk is None or idx == 0:
            return False
        instr = blk.instructions[idx]
        prev = blk.instructions[idx - 1]
        if not (
            isinstance(instr, asm_ast.Mov)
            and isinstance(instr.src, asm_ast.Reg)
            and isinstance(instr.src.reg, asm_ast.A)
        ):
            return False
        if not (
            isinstance(prev, asm_ast.Mov)
            and isinstance(prev.src, asm_ast.Pseudo)
            and isinstance(prev.dst, asm_ast.Reg)
            and isinstance(prev.dst.reg, asm_ast.A)
            and prev.src.offset == 0
        ):
            return False
        return _resolve_rep(prev.src.name) == name

    def _hwreg_clobbered_in_live_range(name: str, reg: str) -> bool:
        if liveness is None:
            return False
        positions = clobber_positions.get(reg, ())
        if not positions:
            return False
        # `name` here is a rep-level name (eligibility set is
        # post-projection). Any of its coalesced defs is fine; only
        # an instruction that writes `reg` AND isn't one of `name`'s
        # def positions counts as a clobber. Additionally, the
        # index-setup chain `Mov(Pseudo name, A); Mov(A, Reg(R))`
        # collapses to a self-transfer after pinning, so its second
        # Mov also isn't a real clobber.
        def_pos_set = def_positions_rep.get(name, set())
        for bid, idx in positions:
            if (bid, idx) in def_pos_set:
                continue
            if _is_self_transfer_chain(bid, idx, name):
                continue
            live = liveness.live_after(bid, idx)
            for live_name in live:
                if _resolve_rep(live_name) == name:
                    return True
        return False

    def _can_pin(name: str, reg: str) -> bool:
        node = graph.nodes.get(name)
        if node is None:
            return False
        if node.width != 1:
            return False
        if node.lives_across_call:
            return False
        if name in hwreg_assignments:
            return False
        for nbr in graph.neighbors(name):
            if hwreg_assignments.get(nbr) == reg:
                return False
        if _hwreg_clobbered_in_live_range(name, reg):
            return False
        return True

    def _try_pin_with_fallback(name: str, preferred: str) -> None:
        other = "Y" if preferred == "X" else "X"
        if _can_pin(name, preferred):
            hwreg_assignments[name] = preferred
            return
        if _can_pin(name, other):
            hwreg_assignments[name] = other

    # Priority: by use count (descending), then by name (alphabetical
    # — for determinism). Highest use-count wins HwReg first, since
    # pinning a many-use name eliminates many setup chains; pinning
    # a few-use name eliminates few. The classic example is a
    # column-iv used as the index for an entire run of indexed
    # stores: lots of references, lots of savings if pinned.
    def _priority(name: str) -> tuple:
        return (-eligibility.use_count.get(name, 0), name)

    # hints_x: prefer X, fall back to Y. Names already pinned (by
    # earlier iterations) skip cleanly.
    for name in sorted(eligibility.hints_x, key=_priority):
        if name not in eligibility.eligible:
            continue
        if name in hwreg_assignments:
            continue
        _try_pin_with_fallback(name, preferred="X")

    # hints_y: prefer Y, fall back to X. Skip names already
    # assigned (e.g. one in BOTH hint sets that already got X).
    for name in sorted(eligibility.hints_y, key=_priority):
        if name not in eligibility.eligible:
            continue
        if name in hwreg_assignments:
            continue
        _try_pin_with_fallback(name, preferred="Y")


def _instr_writes_hwregs(
    instr: asm_ast.Type_instruction,
) -> tuple[str, ...]:
    """Return the X / Y registers (if any) written by `instr` as a
    tuple of letters. Used by the HwReg clobber-in-live-range check
    in `_try_hwreg_assign`.

    What writes X / Y in the asm IR at this stage of the pipeline:
      * `Mov(_, Reg(X|Y))` — TAX / TAY / LDX / LDY (any addressing
        mode the emitter supports).
      * `Inc(Reg(X|Y))` / `Dec(Reg(X|Y))` — INX / INY / DEX / DEY.
      * `Pop(dst=Reg(X|Y))` — PLX / PLY (not currently emitted by
        tac_to_asm, but the IR allows it).

    Excluded:
      * `Mov(Reg(X|Y), _)` — reads X/Y, doesn't write.
      * `Push(src=Reg(X|Y))` — reads X/Y, doesn't write.
      * `Compare(Reg(X|Y), _)` — reads X/Y for CPX / CPY, doesn't
        write.
      * Add / Sub / And / Or / Xor / ASL / LSR / ROL / ROR — these
        atoms only accept Reg(A) as dst per the asm IR contract.
      * Phi — its dst is still a Pseudo at this pipeline stage.
      * Implicit clobbers from operands that emit LDY at lowering
        time (`Frame` / `Stack` / `Indirect` / `IndirectY`) — not
        included here because at this stage operands are still
        Pseudos that haven't been resolved. A separate pass would
        be needed to model those; this check covers the explicit
        case that surfaces in current generated code."""
    out: list[str] = []

    def _add_if_xy(dst: asm_ast.Type_operand) -> None:
        if isinstance(dst, asm_ast.Reg):
            if isinstance(dst.reg, asm_ast.X):
                out.append("X")
            elif isinstance(dst.reg, asm_ast.Y):
                out.append("Y")

    match instr:
        case asm_ast.Mov(dst=dst):
            _add_if_xy(dst)
        case asm_ast.Pop(dst=dst):
            _add_if_xy(dst)
        case asm_ast.Inc(dst=dst) | asm_ast.Dec(dst=dst):
            _add_if_xy(dst)
    return tuple(out)


def _perfect_elimination_order(
    fn: asm_ast.Function, graph: InterferenceGraph,
) -> list[str]:
    """Build a PEO over `graph`'s nodes via dom-tree pre-order walk
    of value definitions, then reverse. Identical algorithm to the
    TAC version, but operating on the asm CFG."""
    cfg = build_cfg(fn)
    idom = immediate_dominators(cfg)
    children = dominator_tree_children(idom)

    build: list[str] = []
    seen: set[str] = set()

    def emit(name: str) -> None:
        if name in seen or name not in graph.nodes:
            return
        seen.add(name)
        build.append(name)

    stack: list[int] = [ENTRY_ID]
    while stack:
        bid = stack.pop()
        blk = cfg.blocks.get(bid)
        if blk is not None:
            # Phi dsts first (parallel-defined at block entry).
            for instr in blk.instructions:
                if isinstance(instr, asm_ast.Phi) and isinstance(
                    instr.dst, asm_ast.Pseudo,
                ):
                    emit(instr.dst.name)
            # Then non-Phi defs in source order.
            for instr in blk.instructions:
                if isinstance(instr, asm_ast.Phi):
                    continue
                for d in _defs_in(instr):
                    emit(d.name)
        for c in reversed(children.get(bid, [])):
            stack.append(c)

    # Append any remaining nodes (unusual — mostly defensive). All
    # asm-SSA names have a defining instruction by construction;
    # only filter survivors land here. Sort for determinism.
    leftover = sorted(n for n in graph.nodes if n not in seen)
    build.extend(leftover)

    build.reverse()
    return build
