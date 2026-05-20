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

1. Build the call graph: a directed edge `caller → callee` for
   every `tac_ast.FunctionCall` in a TAC function's body, where
   `callee` is a zp_abi function visible in the abi dict.
   Recursion / indirect-call / address-taken rejections are
   already enforced upstream by `select_abi`, so the graph is
   guaranteed to be a DAG of zp_abi functions.

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
) -> tuple[dict[str, ParamLayout], dict[str, int]]:
    """Assign concrete ZP (or spill-region) addresses to every
    `ZpLayout` slot symbol in `abi` such that no two functions
    on a common call path share a slot byte.

    Returns `(updated_abi, sym_to_addr)`. The dict has the same
    keys as `abi`; `ZpLayout` entries have their `addrs` field
    replaced with the allocator's choices, with `slot_symbols`
    preserved. `sym_to_addr` maps every emitted slot symbol to
    its assigned numeric address — fed into `asm_emit` to print
    the EQU directives at the top of the output."""
    if pool is None:
        pool = Pool()
    zp_window = pool.caller_saved()  # default $80..$BF
    zp_start, zp_end = zp_window.start, zp_window.stop

    callgraph, ancestors = _build_callgraph(prog, abi)

    # Process in topological order (roots first). For each
    # function, the set of forbidden addresses is the union of
    # already-allocated transitive callers' ranges. Siblings can
    # share addresses; descendants pick disjoint from us.
    order = _topological_order(callgraph)

    addrs_by_fn: dict[str, list[int]] = {}
    for fn_name in order:
        layout = abi.get(fn_name)
        if not isinstance(layout, ZpLayout):
            continue
        n_bytes = len(layout.slot_symbols)
        if n_bytes == 0:
            addrs_by_fn[fn_name] = []
            continue
        forbidden: set[int] = set()
        for anc in ancestors.get(fn_name, ()):
            forbidden.update(addrs_by_fn.get(anc, ()))
        addrs = _find_free_range(forbidden, n_bytes, zp_start, zp_end)
        if addrs is None:
            addrs = _find_free_range(
                forbidden, n_bytes, spill_start, spill_end,
            )
        if addrs is None:
            raise ZpSlotAllocationError(
                f"can't allocate {n_bytes}-byte slot range for "
                f"function `{fn_name}`: no contiguous gap in "
                f"either the ZP window "
                f"(${zp_start:02X}-${zp_end - 1:02X}) or the "
                f"spill region "
                f"(${spill_start:04X}-${spill_end - 1:04X}) "
                f"avoids every already-allocated caller's range",
            )
        addrs_by_fn[fn_name] = addrs

    sym_to_addr: dict[str, int] = {}
    updated_abi: dict[str, ParamLayout] = {}
    for fn_name, layout in abi.items():
        if not isinstance(layout, ZpLayout):
            updated_abi[fn_name] = layout
            continue
        new_addrs = addrs_by_fn.get(fn_name, list(layout.addrs))
        new_layout = ZpLayout(
            slot_symbols=list(layout.slot_symbols),
            addrs=new_addrs,
            param_registers=list(layout.param_registers),
            return_register=layout.return_register,
        )
        updated_abi[fn_name] = new_layout
        for sym, addr in zip(new_layout.slot_symbols, new_addrs):
            sym_to_addr[sym] = addr
    return updated_abi, sym_to_addr


# ---------------------------------------------------------------------------
# Call-graph construction.
# ---------------------------------------------------------------------------


def _build_callgraph(
    prog: tac_ast.Program, abi: dict[str, ParamLayout],
) -> tuple[dict[str, set[str]], dict[str, set[str]]]:
    """Returns `(callees_of, transitive_ancestors_of)` restricted
    to zp_abi functions. Edges come from `tac_ast.FunctionCall`
    occurrences in each function's body. Externs (no
    `tac_ast.Function`) are leaf nodes — they appear as edge
    targets but never as edge sources. `transitive_ancestors_of`
    is the reflexive-transitive caller relation MINUS self
    (the entry on each function lists its transitive callers,
    not itself)."""
    zp_names = {
        n for n, layout in abi.items()
        if isinstance(layout, ZpLayout)
    }
    callees_of: dict[str, set[str]] = {n: set() for n in zp_names}
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        if tl.name not in zp_names:
            continue
        for instr in tl.instructions:
            if (
                isinstance(instr, tac_ast.FunctionCall)
                and instr.name in zp_names
            ):
                callees_of[tl.name].add(instr.name)
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
