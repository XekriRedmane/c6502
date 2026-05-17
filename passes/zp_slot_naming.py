"""Canonical ZP-slot symbol naming.

The compiler mints two families of symbols that bind body locals and
zp_abi parameters to concrete zero-page addresses:

- `__zpabi_<fn>__<param>[_<byte>]` — bytes of a zp_abi function's
  parameters. Minted in `passes.abi_selection`. Single-byte params
  drop the trailing `_<byte>` suffix.
- `__local_<fn>__<source>[_<byte>]` — bytes the call-graph-disjoint
  body-local allocator hands a function. The `<source>` portion is
  the source-level spelling of the C variable backing the slot, when
  it traces cleanly through SSA renaming and move coalescing.
  Compiler-only temporaries that don't trace to a source variable
  get a numeric stand-in: `__local_<fn>__<N>`.

The "first source name wins" coalescing rule kicks in when several
asm-SSA Pseudos collapsed onto a single ZP byte: the first
source-derived Pseudo encountered in the coloring map names the slot.
If every Pseudo on the byte is a compiler temp, the slot gets the
next free numeric index.

This module exposes:

- `param_slot_symbols(fn_name, params, sizes)` — per-byte symbol list
  for a zp_abi function's params, ordered by parameter order then by
  byte-within-parameter, as `passes.abi_selection` expects.
- `local_slot_names(fn_name, coloring, local_pool)` — per-address
  symbol-name map for a function's body-local pool, ordered by
  `local_pool`. Every address in the pool gets a name (unused
  addresses become numeric temps — `passes.prune_unused_slots` drops
  the dangling EQUs later, when the asm body doesn't reference them).
"""
from __future__ import annotations

from passes.optimization.register_allocation import Coloring


def source_spelling(resolved_name: str) -> str:
    """Strip `identifier_resolution`'s `@<N>.` prefix from a resolved
    name to recover the source-level spelling. Returns the name
    unchanged if it doesn't carry the prefix (e.g. a static name
    that wasn't renamed)."""
    if resolved_name.startswith("@"):
        parts = resolved_name.split(".", 1)
        if len(parts) == 2:
            return parts[1]
    return resolved_name


def parse_pseudo_name(name: str) -> tuple[str | None, int]:
    """Decompose an asm-SSA Pseudo name into
    `(source_name, byte_index)`.

    Two layers of versioning may have been applied to the
    pre-translator Var name:

    - TAC SSA construction (`passes.optimization.ssa_construction`)
      appends `.<N>` per def — `@5.sprite_x` becomes
      `@5.sprite_x.0`, `@5.sprite_x.1`, ...
    - Asm-level SSA (`passes.optimization_asm.ssa_construction`)
      appends `.b<k>.v<N>` per (byte, version) — its docstring
      formalizes the convention.

    Stripping in reverse order recovers the pre-translator Var
    name. That name is one of:

    - `@<N>.<orig>` (a source variable / parameter renamed by
      `identifier_resolution`) — yields `(<orig>, byte_index)`. The
      leading `@<N>.` segment is removed; C identifier rules
      guarantee `<orig>` itself contains no dots.
    - `%<N>` / `tmp.<N>` / other forms lacking the `@` prefix —
      yields `(None, byte_index)`.

    The byte index defaults to 0 when no `.b<k>` suffix is present.
    All SSA version numbers are discarded — slot naming is about
    source identity, not version."""
    base = name
    # Strip the optional asm-SSA `.v<N>` version suffix.
    if ".v" in base:
        head, _, tail = base.rpartition(".v")
        if tail.isdigit():
            base = head
    # Strip the optional asm-SSA `.b<k>` byte-index suffix.
    byte_index = 0
    if ".b" in base:
        head, _, tail = base.rpartition(".b")
        if tail.isdigit():
            byte_index = int(tail)
            base = head
    # Strip a single trailing `.<N>` TAC-SSA version suffix.
    if "." in base:
        head, _, tail = base.rpartition(".")
        if tail.isdigit():
            base = head
    # Source-derived names carry identifier_resolution's `@<N>.<orig>`.
    if base.startswith("@"):
        _, _, source = base.partition(".")
        if source:
            return source, byte_index
    return None, byte_index


def param_slot_symbols(
    fn_name: str,
    param_names: list[str],
    param_sizes: list[int],
) -> list[str]:
    """Per-byte zp_abi slot symbols for a function's parameters.

    Order is parameter-order then byte-within-parameter (low byte
    first), matching `passes.abi_selection`'s flat byte-index
    convention. Single-byte parameters get `__zpabi_<fn>__<param>`;
    multi-byte parameters get `__zpabi_<fn>__<param>_<k>` per byte.
    Parameter names are passed through `source_spelling` so resolved
    `@<N>.<orig>` forms surface as the original C identifier."""
    assert len(param_names) == len(param_sizes)
    out: list[str] = []
    for pname, psize in zip(param_names, param_sizes):
        spelling = source_spelling(pname)
        if psize == 1:
            out.append(f"__zpabi_{fn_name}__{spelling}")
        else:
            for k in range(psize):
                out.append(f"__zpabi_{fn_name}__{spelling}_{k}")
    return out


def local_slot_names(
    fn_name: str,
    coloring: Coloring,
    local_pool: list[int],
) -> dict[int, str]:
    """Per-address `__local_<fn>__<...>` symbol map for a function's
    body-local pool.

    Steps:

    1. Invert `coloring.assignments` so each pool address gets the
       list of Pseudo names that colored to it (in dict insertion
       order — deterministic since the regalloc is deterministic).
    2. Sweep all colored Pseudos to determine the byte-width of each
       source variable (max byte index seen + 1). This drives whether
       the slot symbol takes a trailing `_<k>` byte suffix.
    3. For each address in `local_pool` order: prefer the first
       source-derived Pseudo on the address (coalesced source vars
       follow "first source name wins"); when every Pseudo on the
       address is a compiler temp, assign the next free numeric
       index.

    Returns a fresh dict — `apply_coloring` and
    `build_local_slot_symbols` independently call this and must
    agree byte-for-byte."""
    if not local_pool:
        return {}
    pool_set = set(local_pool)
    addr_to_pseudos: dict[int, list[str]] = {}
    for pseudo_name, addr in coloring.assignments.items():
        if addr in pool_set:
            addr_to_pseudos.setdefault(addr, []).append(pseudo_name)

    # Width pass: for every source-derived Pseudo on any pool address,
    # record the max byte index seen for that source name.
    source_widths: dict[str, int] = {}
    for plist in addr_to_pseudos.values():
        for pname in plist:
            source, byte_index = parse_pseudo_name(pname)
            if source is not None:
                source_widths[source] = max(
                    source_widths.get(source, 0),
                    byte_index + 1,
                )

    # Naming pass: walk addresses in pool order. First source-derived
    # Pseudo wins; otherwise mint the next numeric temp. Each emitted
    # symbol must uniquely bind to a single address — when the
    # preferred source-derived name collides with one already emitted
    # (this happens when the regalloc kept two TAC-SSA versions of
    # the same source variable on distinct ZP bytes), the later
    # address falls back to a numeric temp so downstream peepholes
    # don't conflate the two storage cells.
    temp_counter = 0
    used_names: set[str] = set()
    name_map: dict[int, str] = {}

    def _next_temp_name() -> str:
        nonlocal temp_counter
        while True:
            candidate = f"__local_{fn_name}__{temp_counter}"
            temp_counter += 1
            if candidate not in used_names:
                return candidate

    for addr in local_pool:
        chosen: tuple[str, int] | None = None
        for pname in addr_to_pseudos.get(addr, []):
            source, byte_index = parse_pseudo_name(pname)
            if source is not None:
                chosen = (source, byte_index)
                break
        chosen_name: str | None = None
        if chosen is not None:
            source, byte_index = chosen
            width = source_widths[source]
            if width == 1:
                candidate = f"__local_{fn_name}__{source}"
            else:
                candidate = f"__local_{fn_name}__{source}_{byte_index}"
            if candidate not in used_names:
                chosen_name = candidate
        if chosen_name is None:
            chosen_name = _next_temp_name()
        name_map[addr] = chosen_name
        used_names.add(chosen_name)
    return name_map


def compute_local_slot_names(
    local_pools: dict[str, list[int]],
    colorings: dict[str, Coloring],
) -> dict[str, list[str]]:
    """Per-function ordered slot-symbol lists, parallel to each
    pool's address list. `compile.py` computes this once after the
    final optimizer pass and threads it into both
    `build_local_slot_symbols` (EQU bindings) and `build_metadata`
    (linker round-trip).

    Functions missing from `colorings` fall back to numeric temp
    naming (`__local_<fn>__<k>`)."""
    out: dict[str, list[str]] = {}
    for fn_name, pool in local_pools.items():
        coloring = colorings.get(fn_name)
        if coloring is None:
            out[fn_name] = [
                f"__local_{fn_name}__{k}" for k in range(len(pool))
            ]
            continue
        name_map = local_slot_names(fn_name, coloring, pool)
        out[fn_name] = [name_map[addr] for addr in pool]
    return out
