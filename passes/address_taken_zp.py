"""Allocate ZP byte ranges for address-taken local variables.

# Motivating case

`entity_proximity` in examples/companion_update.c declares a local
`uint8_t entity_row` whose address is taken (passed by pointer to
`find_active_entity`). Because the address is taken, the local
needs a stable, addressable storage location — historically that
forced it to a Frame slot via `replace_pseudoregisters`,
materializing the soft-stack prologue/epilogue and adding an
LDY/(FP),Y indirect read at every access.

For zp_abi functions on the call-graph-disjoint local-pool path,
this is unnecessary. The function's private ZP pool is already
guaranteed to be untouched by any coexisting function's code; we
can put the address-taken local in a ZP byte of that pool. The
caller computing `&entity_row` then becomes a 2-byte immediate
load (lo = ZP byte address, hi = $00) rather than a 6-byte FP+off
runtime add. `find_active_entity`'s `STA (DPTR),Y` writes to the
ZP byte directly. No frame, no prologue/epilogue.

# Algorithm

Per function:

  1. Scan the asm IR for `LoadAddress.src = Pseudo(name)`. Each
     such name is an "address-taken local" candidate. Exclude
     names that are params or statics (those have their own
     storage).
  2. Compute each candidate's byte size from the symbol table.
  3. The function's local pool is `local_pools[fn]` — a list of
     ZP byte addresses sized to include the address-taken bytes
     (the caller adds them to `local_bytes` before allocation).
  4. The asm regalloc already colored some pool bytes via
     `coloring.assignments`. The remaining bytes are free for
     address-taken use.
  5. Walk the free bytes in pool order. For each candidate, find
     a contiguous run of `size` free bytes and assign. The first
     byte of the run becomes the candidate's ZP address.

Candidates for which no contiguous run is available are returned
with no assignment — the caller falls back to Frame allocation
for them (the existing path).

# Why not extend the regalloc

Two reasons:

  1. The regalloc operates on SSA-renamed byte Pseudos.
     Address-taken locals are excluded from SSA renaming (they
     need a stable identity), so they never reach the regalloc.
     Adding them would require changing the SSA exclusion rules
     and re-validating the soundness of every downstream pass
     that relies on "address-taken names are unrenamed Pseudos".

  2. The regalloc's interference graph is built per-byte. An
     address-taken local doesn't have meaningful per-byte
     interference (its bytes can't be split across registers /
     ZP slots — they're a single contiguous addressable block).
     Modeling it in the interference graph would require
     special "keep-together" constraints.

A post-coloring allocation pass sidesteps both. The address-taken
locals get the pool bytes the regalloc didn't use, with no
interference modeling required.
"""
from __future__ import annotations

import asm_ast


def compute_address_taken_assignments(
    prog: asm_ast.Program,
    local_pools: dict[str, list[int]],
    colorings: dict[str, "Coloring"],
    symbols,
    types,
) -> dict[str, dict[str, int]]:
    """Compute address-taken local → ZP address assignments per
    function. Returns
    `{fn_name: {pseudo_name: first_byte_address}}` — names absent
    from the inner dict couldn't be placed in ZP and will fall back
    to Frame allocation in `replace_pseudoregisters`.

    `local_pools` and `colorings` come from the
    `--optimize`-mode pipeline. Functions without a local pool
    (ineligible for the private-pool model) get an empty inner
    dict — their address-taken locals stay on the soft stack.
    """
    out: dict[str, dict[str, int]] = {}
    statics = _statics_set(symbols) if symbols is not None else frozenset()
    for tl in prog.top_level:
        if not isinstance(tl, asm_ast.Function):
            continue
        fn_name = tl.name
        pool = local_pools.get(fn_name)
        if pool is None:
            out[fn_name] = {}
            continue
        coloring = colorings.get(fn_name)
        used = _coloring_used_bytes(coloring, symbols, types)
        out[fn_name] = _allocate_in_pool(
            tl, pool, used, statics, symbols, types,
        )
    return out


def _coloring_used_bytes(coloring, symbols, types) -> set[int]:
    """All ZP bytes consumed by the coloring, expanded across the
    width of each colored Pseudo. A multi-byte Pseudo occupies
    `size_of_name` consecutive bytes starting at the assignment
    address; missing this means an address-taken local would
    overlap a colored multi-byte value's continuation bytes."""
    if coloring is None or not coloring.assignments:
        return set()
    from passes.replace_pseudoregisters import size_of_name
    out: set[int] = set()
    for name, addr in coloring.assignments.items():
        size = size_of_name(name, symbols, types)
        for k in range(size):
            out.add(addr + k)
    return out


def _statics_set(symbols) -> frozenset[str]:
    from passes.type_checking import StaticAttr
    return frozenset(
        n for n, s in symbols.items()
        if isinstance(s.attrs, StaticAttr)
    )


def _allocate_in_pool(
    fn: asm_ast.Function,
    pool: list[int],
    used: set[int],
    statics: frozenset[str],
    symbols,
    types,
) -> dict[str, int]:
    candidates = _address_taken_names(fn, statics)
    if not candidates:
        return {}
    from passes.replace_pseudoregisters import size_of_name
    free = [a for a in sorted(set(pool)) if a not in used]
    # Build a list of contiguous runs over `free`.
    runs: list[list[int]] = []
    cur: list[int] = []
    for a in free:
        if cur and a == cur[-1] + 1:
            cur.append(a)
            continue
        if cur:
            runs.append(cur)
        cur = [a]
    if cur:
        runs.append(cur)
    out: dict[str, int] = {}
    for name in candidates:
        size = size_of_name(name, symbols, types)
        # First-fit allocation across runs.
        for run in runs:
            if len(run) >= size:
                out[name] = run[0]
                del run[:size]
                break
    return out


def _address_taken_names(
    fn: asm_ast.Function, statics: frozenset[str],
) -> list[str]:
    """Pseudo names appearing as `LoadAddress.src.name` in `fn`,
    excluding params and statics. Order is first-occurrence in the
    instruction stream so the allocation is deterministic."""
    seen: set[str] = set()
    out: list[str] = []
    param_set = set(fn.params)
    for instr in fn.instructions:
        if isinstance(instr, asm_ast.LoadAddress):
            if not isinstance(instr.src, asm_ast.Pseudo):
                continue
            name = instr.src.name
            if name in param_set:
                continue
            if name in statics:
                continue
            if name in seen:
                continue
            seen.add(name)
            out.append(name)
    return out


def slot_symbols(
    fn_name: str,
    assignments: dict[str, int],
    symbols,
    types,
) -> dict[str, str]:
    """For each (pseudo_name → first_byte_address) in `assignments`,
    return `{pseudo_name: slot_symbol_name}`. The slot symbol is
    `__local_<fn>__<source_name>` where `<source_name>` is the C
    source identifier recovered via `parse_pseudo_name`. For
    multi-byte address-taken locals, each byte gets a trailing
    `_<k>` suffix; the caller is expected to append the byte
    offset when resolving sub-byte refs."""
    from passes.zp_slot_naming import parse_pseudo_name
    out: dict[str, str] = {}
    for pname in assignments:
        source, _ = parse_pseudo_name(pname)
        if source is None:
            # Compiler temp — synthesize a deterministic name.
            out[pname] = f"__local_{fn_name}__addr_{pname}"
        else:
            out[pname] = f"__local_{fn_name}__{source}"
    return out
