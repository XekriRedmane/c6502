"""Per-function body-local ZP pool allocation.

`zp_slot_allocation` hands each zp_abi function a private byte
range for its **parameters**, disjoint from every transitive
caller's param range. This pass extends the same call-graph
analysis to **body locals**: the byte cells the asm-level
regalloc colors for Pseudos that aren't params, statics, or
spilled.

For each function that's eligible (see below), we hand back a
list of byte addresses sized to that function's `local_bytes`
demand (as measured by
`passes.function_local_sizing.compute_local_bytes` from a
preliminary regalloc run). The list is disjoint from:

  * every transitive caller's local pool — so a caller's locals
    survive when this function executes;
  * every coexisting zp_abi function's parameter slots — so an
    outgoing zp_abi call's arg-write inside an ancestor (or our
    body's call to a zp_abi descendant) doesn't trample our
    locals.

Two functions on **non-overlapping call paths** (neither is a
transitive caller of the other) MAY share addresses — their
activations are never simultaneous on the call stack, so the
same ZP bytes can hold both functions' locals in turn.

# Eligibility

A function is eligible for the private-pool model iff:

  1. It's defined in this TU (we can see its body to enumerate
     its callees and to know its local-byte demand).
  2. Its body contains no `tac_ast.IndirectCall` — we can't
     bound an indirect callee's clobber set.
  3. It is not part of any cycle in the static direct call
     graph — recursion would have a recursive call clobber the
     outer activation's still-live locals.
  4. Every direct callee defined in this TU is also eligible.
     (Transitive eligibility: an ineligible callee's body has
     unbounded writes, so the caller can't trust its private
     pool to survive the call.)
  5. Every direct callee declared `extern` in this TU is a
     declared zp_abi function. Such externs are treated as
     **leaves** with write-set equal to their advertised
     parameter slots only — we don't see the body so we trust
     the annotation. A non-zp_abi extern callee disqualifies
     the caller (its writes are unbounded).

Ineligible functions don't get an entry in the returned dict;
their body regalloc continues to use the conservative
caller/callee partition from `Pool`. They still execute
correctly — they just don't benefit from the private-pool
prologue-elimination.

# Algorithm

  1. Build the static direct call graph for every defined
     function in `prog`.
  2. Determine eligibility per the rules above.
  3. Topologically order the eligible subgraph (callers before
     callees).
  4. Walk each eligible function in topological order. The
     function's forbidden address set is the union of:
       - every transitive caller's already-allocated local
         pool;
       - every coexisting zp_abi function's parameter slot
         addresses (where "coexisting" means ancestor OR
         descendant in the call graph — both can be on the
         stack with us);
       - the function's OWN parameter slot addresses if it's
         zp_abi (its own params must not collide with its
         body locals).
     Pick the lowest contiguous range of `local_bytes(fn)`
     addresses in the ZP window disjoint from forbidden; on
     saturation spill above `$FF` into the configured fallback
     region.
  5. Record the chosen range as `fn`'s local pool.

Descendants are NOT in the forbidden set when allocating an
ancestor's pool (descendants haven't been allocated yet). The
topological order guarantees that when each descendant is
later processed, it will see its ancestors' pools in its own
forbidden set — so the disjointness invariant holds program-
wide.

The descendant-zp_abi-param-slot case is handled by reading
zp_abi param addresses up-front (from `abi`, which has them
already assigned by `zp_slot_allocation`) rather than by
topological propagation. That avoids needing two passes.

# What this enables

After this allocator runs, each eligible function's body
regalloc draws colors exclusively from `local_pools[fn]`. By
construction, the colors don't conflict with any caller's,
callee's, or coexisting function's storage. The "caller-saved
vs callee-saved" partition stops being meaningful for these
functions: there's nothing to save in the prologue (no
callee-saved byte the function clobbers belongs to anyone
else's live storage). The prologue collapses to nothing for
true leaves and to the bare `Frame` setup for functions with
address-taken locals or spilled bytes.

# Phase coupling

This pass produces only the allocation map. Two follow-on
steps actually realize the optimization:

  - The asm-level regalloc must be told to use
    `local_pools[fn]` as its pool when coloring eligible
    function `fn` (instead of `Pool.caller_saved() |
    Pool.callee_saved()`).
  - `synthesize_prologue` must drop the callee-save
    save/restore for eligible functions whose private pool
    fully contains the regalloc's chosen colors.

Steps 3 and 4 of the larger plan.
"""
from __future__ import annotations

from collections import deque

import tac_ast
from passes.abi_selection import ParamLayout, ZpLayout
from passes.optimization.pool import Pool


class ZpLocalAllocationError(Exception):
    """Raised when an eligible function's local pool can't be
    placed — no contiguous gap of the required size in either the
    ZP window or the configured spill region."""


_DEFAULT_SPILL_START = 0x0200
_DEFAULT_SPILL_END = 0x10000


def build_local_slot_symbols(
    local_pools: dict[str, list[int]],
    colorings: dict[str, "Coloring"] | None = None,
    *,
    slot_names_by_fn: dict[str, list[str]] | None = None,
) -> dict[str, int]:
    """Convert per-function pool address lists into the EQU symbol
    table the asm-emit stage expects. Each pool entry becomes
    `__local_<fn>__<source>[_<byte>]` (when the byte traces back
    to a source variable through SSA renaming + move coalescing)
    or `__local_<fn>__<N>` (compiler temporary), bound to its
    concrete byte address. The asm IR references body locals by
    these symbols (via `apply_coloring`'s `local_pool` mode), and
    the emit prepends `<sym> EQU $<addr>` for each.

    Callers supply EITHER `colorings` (the typical single-TU
    pipeline: per-function `Coloring` from the asm-SSA regalloc)
    or `slot_names_by_fn` (the multi-TU linker pipeline, where
    slot names round-trip through the per-TU metadata block since
    the linker doesn't have access to the original colorings).
    `slot_names_by_fn[fn]` is the per-pool-byte ordered list of
    slot symbols — same shape as
    `passes.zp_slot_naming.compute_local_slot_names`."""
    from passes.zp_slot_naming import compute_local_slot_names
    if slot_names_by_fn is None:
        slot_names_by_fn = compute_local_slot_names(
            local_pools, colorings or {},
        )
    out: dict[str, int] = {}
    for fn_name, pool in local_pools.items():
        names = slot_names_by_fn.get(fn_name)
        if names is None:
            names = [
                f"__local_{fn_name}__{k}" for k in range(len(pool))
            ]
        for addr, name in zip(pool, names):
            out[name] = addr
    return out


def allocate_function_locals(
    prog: tac_ast.Program,
    abi: dict[str, ParamLayout],
    local_bytes: dict[str, int],
    *,
    pool: Pool | None = None,
    spill_start: int = _DEFAULT_SPILL_START,
    spill_end: int = _DEFAULT_SPILL_END,
) -> dict[str, list[int]]:
    """Allocate per-function body-local ZP pools. Returns
    `dict[fn_name, list[int]]` for every eligible function; the
    list is `local_bytes[fn]` addresses long. Ineligible
    functions don't appear in the dict (their body regalloc
    should fall back to the conservative caller/callee
    partition)."""
    if pool is None:
        pool = Pool()
    zp_window = pool.caller_saved()
    zp_start, zp_end = zp_window.start, zp_window.stop

    callgraph = _build_callgraph(prog)
    direct_extern_callees = _build_direct_extern_callees(prog, callgraph)
    eligible = _determine_eligibility(prog, abi, callgraph)

    # Restrict to eligible — both for the topological walk and
    # for ancestor/descendant transitive closure.
    eligible_cg = {n: callgraph[n] & eligible for n in eligible}
    ancestors, descendants = _ancestors_and_descendants(eligible_cg)
    order = _topological_order(eligible_cg)

    # Pre-extract zp_abi param-slot addresses, keyed by function
    # name. We need these for both eligible and ineligible
    # zp_abi functions — an eligible function `F`'s local pool
    # must avoid the param slots of any zp_abi `G` that may
    # coexist on the call stack, even if `G` itself isn't
    # eligible for the private-pool model.
    zp_param_addrs: dict[str, frozenset[int]] = {
        n: frozenset(layout.addrs)
        for n, layout in abi.items()
        if isinstance(layout, ZpLayout)
    }

    local_pools: dict[str, list[int]] = {}
    for fn in order:
        size = local_bytes.get(fn, 0)
        if size == 0:
            local_pools[fn] = []
            continue
        forbidden: set[int] = set()
        # Ancestor local pools (already allocated in this walk).
        for anc in ancestors.get(fn, ()):
            forbidden.update(local_pools.get(anc, ()))
        # Coexisting zp_abi param slots: every zp_abi function
        # whose param slots are written while `fn` is on the call
        # stack. That's the transitive ancestor chain (their
        # params sit at their pinned slots while `fn` runs deeper)
        # PLUS the transitive descendant chain (their params get
        # written when `fn` — or anything it calls — invokes
        # them). Descendants include extern zp_abi callees, which
        # don't appear as nodes in `callgraph` but ARE reachable
        # via the FunctionCall edges from any in-TU descendant.
        coexisting_zp = _coexisting_zp_functions(
            fn, callgraph, direct_extern_callees, abi,
        )
        for other in coexisting_zp:
            forbidden.update(zp_param_addrs.get(other, ()))
        # The function's own param slots (if zp_abi).
        forbidden.update(zp_param_addrs.get(fn, ()))

        addrs = _find_free_range(forbidden, size, zp_start, zp_end)
        if addrs is None:
            addrs = _find_free_range(
                forbidden, size, spill_start, spill_end,
            )
        if addrs is None:
            raise ZpLocalAllocationError(
                f"can't allocate {size}-byte local pool for "
                f"function `{fn}`: no contiguous gap in either "
                f"ZP (${zp_start:02X}-${zp_end - 1:02X}) or "
                f"spill region "
                f"(${spill_start:04X}-${spill_end - 1:04X}) "
                f"avoids the {len(forbidden)} forbidden bytes",
            )
        local_pools[fn] = addrs
    return local_pools


# ---------------------------------------------------------------------------
# Call-graph construction.
# ---------------------------------------------------------------------------


def _build_callgraph(prog: tac_ast.Program) -> dict[str, set[str]]:
    """Direct call edges among functions defined in `prog`. Externs
    aren't keys; calls to them are dropped from the edge set (they
    show up in the eligibility check via `_extern_callees`)."""
    fn_names = {
        tl.name for tl in prog.top_level
        if isinstance(tl, tac_ast.Function)
    }
    cg: dict[str, set[str]] = {n: set() for n in fn_names}
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        for instr in tl.instructions:
            if (
                isinstance(instr, tac_ast.FunctionCall)
                and instr.name in fn_names
            ):
                cg[tl.name].add(instr.name)
    return cg


# ---------------------------------------------------------------------------
# Eligibility.
# ---------------------------------------------------------------------------


def _determine_eligibility(
    prog: tac_ast.Program,
    abi: dict[str, ParamLayout],
    callgraph: dict[str, set[str]],
) -> set[str]:
    """Set of function names eligible for the private-pool model.
    See module docstring's "Eligibility" section."""
    fn_names = set(callgraph.keys())
    has_indirect: set[str] = set()
    has_non_zp_abi_extern_callee: set[str] = set()
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        for instr in tl.instructions:
            if isinstance(instr, tac_ast.IndirectCall):
                has_indirect.add(tl.name)
            elif isinstance(instr, tac_ast.FunctionCall):
                if instr.name in fn_names:
                    continue
                # External callee. Acceptable iff it's declared
                # zp_abi (we trust the annotation that its writes
                # are bounded to its param slots).
                callee_layout = abi.get(instr.name)
                if not isinstance(callee_layout, ZpLayout):
                    has_non_zp_abi_extern_callee.add(tl.name)
    in_cycle = _find_cycle_members(callgraph)
    ineligible = has_indirect | has_non_zp_abi_extern_callee | in_cycle
    # Propagate ineligibility upward: a caller of an ineligible
    # function is ineligible (its private pool can't survive the
    # callee's unbounded writes).
    callers_of = _invert(callgraph)
    queue: deque[str] = deque(ineligible)
    while queue:
        n = queue.popleft()
        for caller in callers_of.get(n, ()):
            if caller not in ineligible:
                ineligible.add(caller)
                queue.append(caller)
    return fn_names - ineligible


def _find_cycle_members(callgraph: dict[str, set[str]]) -> set[str]:
    """Set of nodes that participate in a cycle (Tarjan's SCC,
    inlined). Includes self-loops (size-1 SCC where the node has
    an edge to itself). Non-trivial SCCs (size > 1) are always
    cycles."""
    index_counter = [0]
    stack: list[str] = []
    on_stack: set[str] = set()
    index: dict[str, int] = {}
    lowlink: dict[str, int] = {}
    in_cycle: set[str] = set()

    def strongconnect(v: str) -> None:
        index[v] = index_counter[0]
        lowlink[v] = index_counter[0]
        index_counter[0] += 1
        stack.append(v)
        on_stack.add(v)
        for w in callgraph.get(v, ()):
            if w not in index:
                strongconnect(w)
                lowlink[v] = min(lowlink[v], lowlink[w])
            elif w in on_stack:
                lowlink[v] = min(lowlink[v], index[w])
        if lowlink[v] == index[v]:
            component: list[str] = []
            while True:
                w = stack.pop()
                on_stack.discard(w)
                component.append(w)
                if w == v:
                    break
            if len(component) > 1:
                in_cycle.update(component)
            else:
                # Singleton: cycle iff self-loop.
                only = component[0]
                if only in callgraph.get(only, set()):
                    in_cycle.add(only)

    for n in callgraph:
        if n not in index:
            strongconnect(n)
    return in_cycle


def _invert(
    callgraph: dict[str, set[str]],
) -> dict[str, set[str]]:
    """Build the reverse-edge graph (callee → callers)."""
    out: dict[str, set[str]] = {n: set() for n in callgraph}
    for caller, callees in callgraph.items():
        for callee in callees:
            out.setdefault(callee, set()).add(caller)
    return out


# ---------------------------------------------------------------------------
# Ancestor / descendant closures.
# ---------------------------------------------------------------------------


def _topological_order(
    callgraph: dict[str, set[str]],
) -> list[str]:
    """Kahn's algorithm — callers before callees. Assumes the
    input is a DAG (eligibility filtering has already excluded
    cycle members)."""
    in_degree: dict[str, int] = {n: 0 for n in callgraph}
    for cs in callgraph.values():
        for c in cs:
            if c in in_degree:
                in_degree[c] += 1
    queue: deque[str] = deque(
        sorted(n for n, d in in_degree.items() if d == 0),
    )
    out: list[str] = []
    while queue:
        n = queue.popleft()
        out.append(n)
        for c in sorted(callgraph.get(n, ())):
            if c not in in_degree:
                continue
            in_degree[c] -= 1
            if in_degree[c] == 0:
                queue.append(c)
    return out


def _ancestors_and_descendants(
    callgraph: dict[str, set[str]],
) -> tuple[dict[str, set[str]], dict[str, set[str]]]:
    """Transitive closures in both directions, restricted to the
    DAG induced by `callgraph`'s nodes."""
    callers_of = _invert(callgraph)
    order = _topological_order(callgraph)
    ancestors: dict[str, set[str]] = {n: set() for n in callgraph}
    for n in order:
        for c in callers_of.get(n, ()):
            ancestors[n].add(c)
            ancestors[n].update(ancestors.get(c, ()))
    descendants: dict[str, set[str]] = {n: set() for n in callgraph}
    for n in reversed(order):
        for d in callgraph.get(n, ()):
            descendants[n].add(d)
            descendants[n].update(descendants.get(d, ()))
    return ancestors, descendants


def _build_direct_extern_callees(
    prog: tac_ast.Program,
    callgraph: dict[str, set[str]],
) -> dict[str, set[str]]:
    """Per-function set of direct callees that are NOT defined in
    this TU (i.e. would resolve to extern symbols at link time).
    The main allocator uses this to walk extern zp_abi callees
    transitively for the coexisting-zp set; eligibility uses it
    only indirectly via `_determine_eligibility`. Keyed by the
    caller's name; the set values are extern callee names."""
    in_tu = set(callgraph.keys())
    out: dict[str, set[str]] = {n: set() for n in in_tu}
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        for instr in tl.instructions:
            if (
                isinstance(instr, tac_ast.FunctionCall)
                and instr.name not in in_tu
            ):
                out[tl.name].add(instr.name)
    return out


def _coexisting_zp_functions(
    fn: str,
    full_callgraph: dict[str, set[str]],
    direct_extern_callees: dict[str, set[str]],
    abi: dict[str, ParamLayout],
) -> set[str]:
    """Set of zp_abi function names whose param slots may be live
    or get written while `fn` is on the call stack. Includes:

      * transitive ancestors of `fn` that are zp_abi (their
        params sit at pinned slots while `fn` runs deeper);
      * transitive descendants of `fn` that are zp_abi
        (their params get written when `fn` — or any function
        it calls transitively — invokes them);
      * extern zp_abi callees reachable from `fn` or any of its
        transitive in-TU descendants (these don't appear as
        nodes in `full_callgraph` because they have no body, but
        they ARE on the call stack with `fn` whenever the chain
        invokes them).

    Excludes `fn` itself; the caller adds that separately."""
    out: set[str] = set()
    # Transitive ancestors via the inverted graph.
    callers_of = _invert(full_callgraph)
    seen_anc: set[str] = set()
    stack: list[str] = list(callers_of.get(fn, ()))
    while stack:
        n = stack.pop()
        if n in seen_anc:
            continue
        seen_anc.add(n)
        if isinstance(abi.get(n), ZpLayout):
            out.add(n)
        stack.extend(callers_of.get(n, ()))
    # Transitive descendants via the forward graph. Each step,
    # we also record any direct extern callees of the current
    # node — they're "coexisting" with `fn` (will be on the
    # stack when `fn`'s subtree invokes them).
    seen_desc: set[str] = {fn}
    stack = [fn]
    while stack:
        n = stack.pop()
        for callee in full_callgraph.get(n, ()):
            if callee in seen_desc:
                continue
            seen_desc.add(callee)
            if isinstance(abi.get(callee), ZpLayout):
                out.add(callee)
            stack.append(callee)
        # Extern callees reachable directly from `n`. These have
        # no body so they don't contribute further descendants of
        # their own (we can't see them anyway).
        for ext in direct_extern_callees.get(n, ()):
            if isinstance(abi.get(ext), ZpLayout):
                out.add(ext)
    return out


# ---------------------------------------------------------------------------
# Free-range search (shared with zp_slot_allocation).
# ---------------------------------------------------------------------------


def _find_free_range(
    forbidden: set[int], n_bytes: int, lo: int, hi: int,
) -> list[int] | None:
    """Lowest contiguous range of `n_bytes` addresses in
    `[lo, hi)` disjoint from `forbidden`. Returns the address
    list or None if no fit."""
    cur = lo
    while cur + n_bytes <= hi:
        conflict_at = None
        for k in range(n_bytes):
            if (cur + k) in forbidden:
                conflict_at = cur + k
        if conflict_at is None:
            return list(range(cur, cur + n_bytes))
        cur = conflict_at + 1
    return None
