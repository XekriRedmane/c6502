"""ZP slot allocation for `__attribute__((zp_abi))` functions.

Background
----------

`select_abi` decides WHICH functions get the ZP-passing ABI; this
pass decides WHERE each function's parameter bytes live. The
constraint is simple: no two functions that can appear on the
call stack at the same time may share a slot byte. If function
`A` calls function `B` (directly or transitively), then `A`'s
parameter slots must be disjoint from `B`'s — otherwise `B`'s
call-site arg writes would clobber `A`'s still-live params.

This pass runs after `select_abi` and before `tac_to_asm`. It
reads the TAC program's static call graph and assigns concrete
ZP addresses to each `ZpLayout`'s slot symbols such that the
disjointness invariant holds program-wide.

The asm-emit stage prints `<sym> EQU $<addr>` directives at the
top of the output; `tac_to_asm` and
`replace_pseudoregisters_bare_exit` emit `Data(<sym>, 0)`
operands for every zp_abi slot reference (caller side and
callee side). dasm picks zero-page vs absolute addressing
automatically from each symbol's resolved value — so when ZP
saturates and the allocator has to spill a function's range
above `$FF`, no IR changes are needed; the same `LDA <sym>` /
`STA <sym>` instructions just assemble as 3-byte absolute
rather than 2-byte zero-page.

Algorithm
---------

1. Build the call graph: a directed edge `caller → callee`
   between two zp_abi functions when the caller can reach the
   callee through `tac_ast.FunctionCall`s, directly or through
   functions that aren't zp_abi. A zp_abi function may call a
   soft-stack function that has a private pool, which may call
   zp_abi functions in turn, and their slots must stay apart from
   the caller's. Recursion / indirect-call / address-taken
   rejections are already enforced upstream by `select_abi`, so the
   graph is guaranteed to be a DAG of zp_abi functions.

2. Topologically order the DAG (callers before their callees,
   Kahn's algorithm) and walk it in that order. For each
   function `F`:
   - Compute `forbidden = ⋃ addrs[A]` over all `A` that are
     transitive callers of `F` and have already been assigned
     addresses. ("Transitive callers" because any ancestor's
     params are live when `F` executes — they sit on the call
     stack above `F`.)
   - Find the lowest contiguous range of `param_bytes(F)`
     addresses, starting from the configured ZP pool window
     (`Pool.zp_param_window()`, default `$80..$BF`), that's
     disjoint from `forbidden`. If no such range fits within
     ZP, spill to the configured non-ZP fallback region
     (default `$0200..$FFFF` — anywhere in RAM that the dasm
     output can address; concrete placement up to the
     programmer's linker script / runtime header).
   - Record `F.addrs`; downstream functions seeing `F` as an
     ancestor will exclude these addresses from their search.

3. Sibling functions (no caller-callee relation) MAY share
   addresses: their activations are never simultaneous on the
   call stack, so reusing slots is safe. The topological
   ordering naturally allows this — siblings each pick the
   lowest available range relative to their own ancestor set.

Slots that get no range
-----------------------

A byte passed in a register has no slot: the caller loads the
register, and `tac_to_asm` copies it into a local of the callee. A
byte of a `zeropage(addr)` parameter is bound to that address, which `reserved` already
keeps every other slot off.

Shared slots
------------

`shares` (`passes/slot_sharing.py`) lists sets of slots that hold one
value for several functions, so they get one address. A share's bytes
are allocated at the turn of the last of its members' functions in
the order, as one contiguous run, and avoid every byte already given
to a function that can be on the call stack with any function in the
share (its members' owners and its holders). The functions allocated
after it avoid it in turn, the same way. Its members' other slots are
allocated as usual.

Externs
-------

A zp_abi extern declared in this TU appears in the abi dict
(courtesy of `select_abi`'s extern path) but has no
`tac_ast.Function` body to walk. We treat externs as leaf
nodes in the local call graph — they have no outgoing edges
visible to us. Their slot ranges are allocated like any other
zp_abi function, and concrete `EQU` directives are emitted in
the same output so call sites in this TU bind to consistent
addresses. Phase 2 (cross-TU linking) will move the EQU
emission into a separate `slots.inc` and require a global
allocator that sees all TUs' call graphs.
"""
from __future__ import annotations

from collections import deque

import tac_ast
from passes.abi_selection import ParamLayout, ZpLayout
from passes.optimization.pool import Pool


class ZpSlotAllocationError(Exception):
    """Raised when the allocator can't place a zp_abi function's
    slots — either because no contiguous range of the required
    size fits in either the ZP or the spill region, or because
    the call graph has a cycle (which `select_abi` should already
    have rejected; defensive)."""


_DEFAULT_SPILL_START = 0x0200
_DEFAULT_SPILL_END = 0x10000


def allocate_zp_slots(
    prog: tac_ast.Program,
    abi: dict[str, ParamLayout],
    *,
    pool: Pool | None = None,
    spill_start: int = _DEFAULT_SPILL_START,
    spill_end: int = _DEFAULT_SPILL_END,
    reserved: frozenset[int] = frozenset(),
    window: range | None = None,
    shares=(),
) -> tuple[dict[str, ParamLayout], dict[str, int]]:
    """Assign concrete ZP (or spill-region) addresses to every
    `ZpLayout` slot symbol in `abi` such that no two functions
    on a common call path share a slot byte.

    Returns `(updated_abi, sym_to_addr)`. The dict has the same
    keys as `abi`; `ZpLayout` entries have their `addrs` field
    replaced with the allocator's choices, with `slot_symbols`
    preserved. `sym_to_addr` maps every emitted slot symbol to
    its assigned numeric address — fed into `asm_emit` to print
    the EQU directives at the top of the output.

    `reserved` is a set of bytes no slot may use, the zero-page
    globals' bytes (passes/zp_globals.py). `window` is the zero-page
    range the slots come from, the pool's caller-saved half unless
    given (the linker gives the whole pool, since a linked program
    has no soft-stack function that keeps values in the other
    half). `shares` are the `SlotShare`s whose slots get one address
    (see "Shared slots"). A register byte gets no address (None in
    `addrs`, and no entry in `sym_to_addr`), and a pinned one its
    own."""
    if pool is None:
        pool = Pool()
    zp_window = window if window is not None else pool.caller_saved()
    zp_start, zp_end = zp_window.start, zp_window.stop

    callgraph, ancestors = _build_callgraph(prog, abi)
    from passes.slot_sharing import coexist, reach_sets
    reach = reach_sets(prog)

    # Process in topological order (roots first). For each
    # function, the set of forbidden addresses is the union of
    # already-allocated transitive callers' ranges. Siblings can
    # share addresses; descendants pick disjoint from us.
    order = _topological_order(callgraph)

    shares = list(shares)
    shared = {s for share in shares for s in share.slots()}
    # Each share is allocated at the turn of the last of its members'
    # functions.
    position = {n: k for k, n in enumerate(order)}
    turn: dict[str, list] = {}
    for share in shares:
        last = max(
            (f for f, _ in share.members),
            key=lambda f: position.get(f, -1),
        )
        turn.setdefault(last, []).append(share)
    share_addrs: dict[int, list[int]] = {}

    def range_for(what: str, forbidden: set[int], n: int) -> list[int]:
        addrs = _find_free_range(forbidden, n, zp_start, zp_end)
        if addrs is None:
            addrs = _find_free_range(forbidden, n, spill_start, spill_end)
        if addrs is None:
            raise ZpSlotAllocationError(
                f"can't allocate {n}-byte slot range for {what}: no "
                f"contiguous gap in either the ZP window "
                f"(${zp_start:02X}-${zp_end - 1:02X}) or the "
                f"spill region "
                f"(${spill_start:04X}-${spill_end - 1:04X}) "
                f"avoids every already-allocated caller's range",
            )
        return addrs

    def share_bytes(fn_name: str) -> set[int]:
        # The allocated shares with a function that can be on the call
        # stack with `fn_name`.
        out: set[int] = set()
        for share in shares:
            if id(share) in share_addrs and any(
                coexist(fn_name, f, reach) for f in share.functions()
            ):
                out.update(share_addrs[id(share)])
        return out

    # Each function's own bytes: its slots outside any share.
    own_addrs: dict[str, list[int]] = {}
    sym_to_addr: dict[str, int] = {}
    addrs_by_fn: dict[str, list[int]] = {}
    for fn_name in order:
        layout = abi.get(fn_name)
        if not isinstance(layout, ZpLayout):
            continue
        n = len(layout.slot_symbols)
        pinned = layout.pinned or [None] * n
        regs = layout.param_registers or [None] * n
        mine = [
            k for k, sym in enumerate(layout.slot_symbols)
            if sym not in shared and pinned[k] is None and regs[k] is None
        ]
        for k, sym in enumerate(layout.slot_symbols):
            if pinned[k] is not None:
                sym_to_addr[sym] = pinned[k]
        forbidden: set[int] = set(reserved)
        for anc in ancestors.get(fn_name, ()):
            forbidden.update(addrs_by_fn.get(anc, ()))
        forbidden |= share_bytes(fn_name)
        addrs = (
            range_for(f"function `{fn_name}`", forbidden, len(mine))
            if mine else []
        )
        for k, addr in zip(mine, addrs):
            sym_to_addr[layout.slot_symbols[k]] = addr
        own_addrs[fn_name] = addrs + [a for a in pinned if a is not None]
        addrs_by_fn.setdefault(fn_name, []).extend(own_addrs[fn_name])
        for share in turn.get(fn_name, ()):
            fns = share.functions()
            forbidden = set(reserved)
            for other, taken in own_addrs.items():
                if any(coexist(other, f, reach) for f in fns):
                    forbidden.update(taken)
            for f in fns:
                forbidden |= share_bytes(f)
            addrs = range_for(
                f"the shared parameter bytes `{share.members[0][1][0]}`",
                forbidden, share.width,
            )
            share_addrs[id(share)] = addrs
            for f, syms in share.members:
                for sym, addr in zip(syms, addrs):
                    sym_to_addr[sym] = addr
                addrs_by_fn.setdefault(f, []).extend(addrs)

    updated_abi: dict[str, ParamLayout] = {}
    for fn_name, layout in abi.items():
        if not isinstance(layout, ZpLayout):
            updated_abi[fn_name] = layout
            continue
        if fn_name in own_addrs:
            new_addrs = [sym_to_addr.get(s) for s in layout.slot_symbols]
        else:
            new_addrs = list(layout.addrs)
            for sym, addr in zip(layout.slot_symbols, new_addrs):
                if addr is not None:
                    sym_to_addr[sym] = addr
        updated_abi[fn_name] = ZpLayout(
            slot_symbols=list(layout.slot_symbols),
            addrs=new_addrs,
            param_registers=list(layout.param_registers),
            pinned=list(layout.pinned),
        )
    return updated_abi, sym_to_addr


# ---------------------------------------------------------------------------
# Call-graph construction.
# ---------------------------------------------------------------------------


def _build_callgraph(
    prog: tac_ast.Program, abi: dict[str, ParamLayout],
) -> tuple[dict[str, set[str]], dict[str, set[str]]]:
    """Returns `(callees_of, transitive_ancestors_of)` restricted
    to zp_abi functions. Edges come from `tac_ast.FunctionCall`
    occurrences in each function's body: `callees_of[f]` holds the
    zp_abi functions `f` calls directly, or through a chain of
    calls whose functions in between aren't zp_abi. Externs (no
    `tac_ast.Function`) are leaf nodes — they appear as edge
    targets but never as edge sources. `transitive_ancestors_of`
    is the reflexive-transitive caller relation MINUS self
    (the entry on each function lists its transitive callers,
    not itself)."""
    zp_names = {
        n for n, layout in abi.items()
        if isinstance(layout, ZpLayout)
    }
    direct: dict[str, set[str]] = {}
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function):
            direct[tl.name] = {
                instr.name for instr in tl.instructions
                if isinstance(instr, tac_ast.FunctionCall)
            }
    callees_of: dict[str, set[str]] = {n: set() for n in zp_names}
    for name in zp_names:
        seen: set[str] = set()
        stack = list(direct.get(name, ()))
        while stack:
            c = stack.pop()
            if c in seen:
                continue
            seen.add(c)
            if c in zp_names:
                callees_of[name].add(c)
            else:
                stack.extend(direct.get(c, ()))
    callers_of: dict[str, set[str]] = {n: set() for n in zp_names}
    for caller, cs in callees_of.items():
        for callee in cs:
            callers_of[callee].add(caller)
    # Transitive ancestor closure.
    ancestors_of: dict[str, set[str]] = {n: set() for n in zp_names}
    # Walk in topological order so each node's ancestors are
    # already fully expanded when we expand the node.
    order = _topological_order(callees_of)
    for node in order:
        anc: set[str] = set()
        for direct_caller in callers_of[node]:
            anc.add(direct_caller)
            anc.update(ancestors_of[direct_caller])
        ancestors_of[node] = anc
    return callees_of, ancestors_of


def _topological_order(callees_of: dict[str, set[str]]) -> list[str]:
    """Kahn's algorithm. Returns nodes in caller-before-callee
    order. Raises if a cycle is present (defensive — select_abi
    rejects zp_abi recursion upstream)."""
    # In-degree = number of callers. Roots have zero callers.
    in_degree: dict[str, int] = {n: 0 for n in callees_of}
    for cs in callees_of.values():
        for c in cs:
            in_degree[c] = in_degree.get(c, 0) + 1
    queue: deque[str] = deque(
        sorted(n for n, d in in_degree.items() if d == 0)
    )
    out: list[str] = []
    while queue:
        n = queue.popleft()
        out.append(n)
        for c in sorted(callees_of.get(n, ())):
            in_degree[c] -= 1
            if in_degree[c] == 0:
                queue.append(c)
    if len(out) != len(in_degree):
        # A cycle exists. Defensive — select_abi should have
        # rejected it. Surface a clear error so the bug doesn't
        # silently misalloc.
        remaining = [n for n, d in in_degree.items() if d > 0]
        raise ZpSlotAllocationError(
            f"cycle detected in zp_abi call graph involving "
            f"{sorted(remaining)} — select_abi should have "
            f"rejected this; internal error",
        )
    return out


# ---------------------------------------------------------------------------
# Free-range search.
# ---------------------------------------------------------------------------


def _find_free_range(
    forbidden: set[int], n_bytes: int, lo: int, hi: int,
) -> list[int] | None:
    """Lowest contiguous range of `n_bytes` addresses in
    `[lo, hi)` that's disjoint from `forbidden`. Returns the
    list of byte addresses, or None if no fit."""
    cur = lo
    while cur + n_bytes <= hi:
        # Scan forward for any conflict in [cur, cur + n_bytes).
        conflict_at = None
        for k in range(n_bytes):
            if (cur + k) in forbidden:
                conflict_at = cur + k
        if conflict_at is None:
            return list(range(cur, cur + n_bytes))
        # Skip past the highest conflict (no earlier start in
        # [cur, conflict_at] would have produced a free range).
        cur = conflict_at + 1
    return None
