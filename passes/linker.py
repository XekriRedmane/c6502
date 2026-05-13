"""Multi-TU linker for c6502.

`compile.py --link foo.asm bar.asm -o out.asm` reads N
optimized `.asm` files, re-allocates every `__zpabi_*` and
`__local_*` slot symbol over the **merged** call graph, and
emits a single `.asm` with one global EQU block plus the
function bodies concatenated in input order.

What "linking" does at the call-graph level:

  * Unions every TU's `LinkMetadata` (see
    `passes.zp_link_metadata`) into a program-level view of
    function definitions, zp_abi extern declarations, and call
    edges.
  * Detects cross-TU duplicate definitions (same function
    defined in two `.asm` files) — link error.
  * Detects cross-TU recursion that no per-TU compile could
    have seen — link error.
  * Detects calls to functions that aren't in any TU's `def`
    list and aren't declared `extern` zp_abi anywhere —
    "non-zp_abi external" calls. The caller is ineligible for
    the private-pool model; link errors out.
  * Builds a synthetic `tac_ast.Program` shape representing
    the merged graph and feeds it through
    `allocate_zp_slots` + `allocate_function_locals` —
    re-using the exact same allocators the single-TU compile
    runs in-process. The output is the global address binding
    for every slot symbol.

Eligibility caveat (Phase 3 MVP). The linker requires every
function defined across all input TUs to be eligible for the
private-pool model: no `IndirectCall`, no participation in
any cycle (local or cross-TU), and every callee either
defined in some TU or declared zp_abi extern. Functions that
were ineligible at per-TU compile time emitted body locals as
numeric `ZP(addr, 0)` operands (not `__local_*` symbols), so
the linker has nothing to re-allocate there — and the user's
asm probably contains hardcoded ZP addresses that the linker
can't reason about. A future Phase 4 could lift this
restriction by extending the metadata block to record
ineligible functions' numeric ZP usage and treating those
addresses as immutably blocked.

`link_files` is the top-level entry point.
"""
from __future__ import annotations

from pathlib import Path

import tac_ast
from passes.abi_selection import ParamLayout, SoftStackLayout, ZpLayout
from passes.zp_link_metadata import (
    ExternMeta,
    FunctionMeta,
    LinkMetadata,
    format_metadata,
    parse_metadata,
)
from passes.zp_local_allocation import (
    ZpLocalAllocationError, allocate_function_locals,
    build_local_slot_symbols,
)
from passes.zp_slot_allocation import (
    ZpSlotAllocationError, allocate_zp_slots,
)


class LinkError(Exception):
    """Raised on a link-time validation failure — cross-TU
    duplicate definitions, cycles spanning TUs, missing extern
    declarations, or any function ineligible for the private-pool
    model under the merged view."""


# ---------------------------------------------------------------------------
# File splitting.
# ---------------------------------------------------------------------------


_EQU_PATTERN = "\tEQU\t"
_META_BEGIN = "; @zp-link-meta-begin"
_META_END = "; @zp-link-meta-end"


def _split_asm(text: str) -> tuple[list[str], list[str], list[str]]:
    """Split `text` into (equ_lines, metadata_lines, body_lines).
    EQU lines are detected by the `<sym>\\tEQU\\t$<value>` shape
    asm_emit produces at the top. The metadata block is bracketed
    by `@zp-link-meta-{begin,end}` comment markers.

    Order matters: the per-TU emit puts EQUs first, then the
    metadata block, then a blank line, then the body. This
    splitter respects that order; lines outside both blocks
    become body (preserving blank-line separators)."""
    equ: list[str] = []
    meta: list[str] = []
    body: list[str] = []
    in_meta = False
    seen_meta = False
    for line in text.splitlines():
        stripped = line.strip()
        if stripped == _META_BEGIN:
            in_meta = True
            seen_meta = True
            meta.append(line)
            continue
        if stripped == _META_END:
            meta.append(line)
            in_meta = False
            continue
        if in_meta:
            meta.append(line)
            continue
        if _EQU_PATTERN in line and not body:
            equ.append(line)
            continue
        # First non-EQU, non-meta line ends the EQU block; the
        # blank separator after EQU isn't preserved (we re-emit a
        # blank later). Same for the blank after the metadata
        # block.
        if not body and stripped == "" and (equ or seen_meta):
            continue
        body.append(line)
    return equ, meta, body


# ---------------------------------------------------------------------------
# Metadata merge + validation.
# ---------------------------------------------------------------------------


def _merge(metadatas: list[LinkMetadata]) -> LinkMetadata:
    """Union every TU's metadata into one. Cross-TU duplicate
    definitions are NOT resolved here — `_validate` catches
    them. Externs declared in multiple TUs (with consistent
    param_bytes) collapse to one entry."""
    defs_by_name: dict[str, FunctionMeta] = {}
    duplicate_defs: list[str] = []
    for m in metadatas:
        for d in m.defs:
            if d.name in defs_by_name:
                duplicate_defs.append(d.name)
            else:
                defs_by_name[d.name] = d
    externs_by_name: dict[str, ExternMeta] = {}
    extern_param_conflicts: list[str] = []
    for m in metadatas:
        for e in m.externs:
            existing = externs_by_name.get(e.name)
            if existing is None:
                externs_by_name[e.name] = e
            elif existing.param_bytes != e.param_bytes:
                extern_param_conflicts.append(e.name)
    seen_edges: set[tuple[str, str]] = set()
    for m in metadatas:
        seen_edges.update(m.calls)
    merged = LinkMetadata(
        defs=sorted(defs_by_name.values(), key=lambda d: d.name),
        externs=sorted(externs_by_name.values(), key=lambda e: e.name),
        calls=sorted(seen_edges),
    )
    if duplicate_defs:
        raise LinkError(
            f"function(s) defined in multiple TUs: "
            f"{sorted(set(duplicate_defs))}",
        )
    if extern_param_conflicts:
        raise LinkError(
            f"zp_abi extern(s) declared with conflicting "
            f"param_bytes across TUs: "
            f"{sorted(set(extern_param_conflicts))}",
        )
    return merged


def _validate_eligibility(merged: LinkMetadata) -> None:
    """Every defined function must be eligible for the
    private-pool model under the merged view: no `IndirectCall`,
    not on any cycle (cross-TU detection happens in the
    allocator), and every direct callee either defined or
    declared zp_abi extern. Externs declared but not defined
    AND not zp_abi are not exposed by the metadata format —
    those would have appeared as `call` edges to a name not in
    `defs` or `externs`."""
    defined = {d.name for d in merged.defs}
    extern_names = {e.name for e in merged.externs}
    known = defined | extern_names
    # `IndirectCall` is fatal even at link time.
    indirect_fns = [d.name for d in merged.defs if d.indirect]
    if indirect_fns:
        raise LinkError(
            f"function(s) contain IndirectCall and can't be "
            f"linked under the private-pool model: "
            f"{sorted(indirect_fns)}",
        )
    # Cycle members from any TU's local view. Cross-TU cycles
    # are detected as a side effect of `allocate_zp_slots` /
    # `allocate_function_locals` (they raise their own errors).
    locally_cyclic = [d.name for d in merged.defs if d.in_cycle]
    if locally_cyclic:
        raise LinkError(
            f"function(s) on a cycle in their own TU's call "
            f"graph (recursion isn't supported under the "
            f"private-pool model): {sorted(locally_cyclic)}",
        )
    # Call edges to names that are neither defined nor declared
    # zp_abi extern — these are non-zp_abi externs (e.g. a regular
    # C library function). The caller can't be linked.
    bad_callees: dict[str, set[str]] = {}
    for caller, callee in merged.calls:
        if callee not in known:
            bad_callees.setdefault(caller, set()).add(callee)
    if bad_callees:
        formatted = "; ".join(
            f"{c} → {sorted(cs)}"
            for c, cs in sorted(bad_callees.items())
        )
        raise LinkError(
            f"function(s) call non-zp_abi externs that aren't "
            f"defined in any input TU: {formatted}",
        )


# ---------------------------------------------------------------------------
# Synthetic TAC program for the allocators.
# ---------------------------------------------------------------------------


def _synthesize(
    merged: LinkMetadata,
) -> tuple[tac_ast.Program, dict[str, ParamLayout], dict[str, int]]:
    """Build a `(prog, abi, local_bytes)` triple that drives the
    existing allocators against the merged metadata.

    `prog`'s functions have empty `params` lists and bodies
    consisting of `FunctionCall(name=callee)` instructions for
    every recorded call edge — enough for the allocators'
    `_build_callgraph` walks. Param/local widths come from the
    metadata; no actual code-shape information is needed."""
    # Group callees by caller.
    callees_of: dict[str, list[str]] = {d.name: [] for d in merged.defs}
    for caller, callee in merged.calls:
        if caller in callees_of:
            callees_of[caller].append(callee)
    # Synthesize one tac_ast.Function per def.
    top_level: list[tac_ast.Type_top_level] = []
    for d in merged.defs:
        instrs: list[tac_ast.Type_instruction] = []
        for callee in callees_of.get(d.name, ()):
            instrs.append(tac_ast.FunctionCall(
                name=callee, args=[], dst=None,
            ))
        top_level.append(tac_ast.Function(
            name=d.name, is_global=True, params=[],
            instructions=instrs,
        ))
    prog = tac_ast.Program(top_level=top_level)
    # Build the abi dict. Defs that have param_bytes > 0 are
    # zp_abi; param_bytes == 0 means a non-zp_abi function.
    abi: dict[str, ParamLayout] = {}
    for d in merged.defs:
        if d.param_bytes > 0:
            abi[d.name] = ZpLayout(
                slot_symbols=[
                    f"__zpabi_{d.name}_p{k}"
                    for k in range(d.param_bytes)
                ],
                addrs=[],
            )
        else:
            abi[d.name] = SoftStackLayout()
    for e in merged.externs:
        abi[e.name] = ZpLayout(
            slot_symbols=[
                f"__zpabi_{e.name}_p{k}"
                for k in range(e.param_bytes)
            ],
            addrs=[],
        )
    local_bytes = {d.name: d.local_bytes for d in merged.defs}
    return prog, abi, local_bytes


# ---------------------------------------------------------------------------
# Top-level entry.
# ---------------------------------------------------------------------------


def link_files(
    input_paths: list[Path | str],
    output_path: Path | str,
) -> None:
    """Read each input `.asm`, merge metadata, re-allocate every
    slot symbol globally, and write a single `.asm` to
    `output_path`. Raises `LinkError` on any validation failure;
    raises `ZpSlotAllocationError` / `ZpLocalAllocationError` if
    the merged graph can't be allocated within the configured ZP
    + spill region (defaults to the same as the single-TU
    pass)."""
    if not input_paths:
        raise LinkError("link requires at least one input .asm file")
    # Read + split.
    metas: list[LinkMetadata] = []
    bodies: list[list[str]] = []
    for path in input_paths:
        text = Path(path).read_text()
        _, _, body = _split_asm(text)
        metas.append(parse_metadata(text))
        bodies.append(body)
    # Merge + validate.
    merged = _merge(metas)
    _validate_eligibility(merged)
    # Synthesize the program shape the allocators expect.
    prog, abi, local_bytes = _synthesize(merged)
    # Run the global allocators.
    abi, zp_sym_to_addr = allocate_zp_slots(prog, abi)
    local_pools = allocate_function_locals(prog, abi, local_bytes)
    local_sym_to_addr = build_local_slot_symbols(local_pools)
    all_symbols = {**zp_sym_to_addr, **local_sym_to_addr}
    # Format the output.
    out_lines: list[str] = []
    out_lines.extend(_format_equ_block(all_symbols))
    out_lines.append("")
    out_lines.extend(format_metadata(merged))
    out_lines.append("")
    for i, body in enumerate(bodies):
        if i > 0:
            out_lines.append("")
        out_lines.extend(body)
    Path(output_path).write_text("\n".join(out_lines) + "\n")


def _format_equ_block(
    sym_to_addr: dict[str, int],
) -> list[str]:
    """Same convention as `asm_emit._emit_equ_block`: sorted by
    (addr, name), `<sym>\\tEQU\\t$<addr>` per line. Two-digit hex
    for ZP, four-digit for spill-region addresses (≥ $100)."""
    items = sorted(sym_to_addr.items(), key=lambda kv: (kv[1], kv[0]))
    lines: list[str] = []
    for sym, addr in items:
        if addr <= 0xFF:
            lines.append(f"{sym}\tEQU\t${addr:02X}")
        else:
            lines.append(f"{sym}\tEQU\t${addr:04X}")
    return lines
