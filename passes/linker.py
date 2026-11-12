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

Zero-page objects (`passes/zp_globals.py`). Each TU lists its
zero-page objects in the metadata, and refers to them only by name,
so the linker re-places them for the whole program
(`_place_zp_objects`): an object declared with an address keeps it,
and the rest go in the lowest free bytes below `$80`, the declared
ones first and then the ones `--optimize` promoted (`weight=` in the
metadata), densest first. A promoted object that doesn't fit goes
back in the data segment (`_data_lines`), unless some unit uses it
as an `(zp),Y` pointer, which only works in zero page. Their bytes
are kept out of the slot allocators, and each TU's `__zp_init` is
replaced with one that stores every object's initial bytes
(`_init_lines`).

Names. An internal name (a `static`, or a string literal `str@N`)
that another unit also defines, or that names something else in the
program, is renamed `name.k` in unit `k`
(`link_layout.rename_locals`); two units defining one external name
is an error.

Layout. With a linker script (`-T`, passes/link_script.py), the
output is laid out by it: each function and object is an item in a
section (passes/sections.py), and the output sections become dasm
segments at the script's addresses (passes/link_layout.py). The
startup code that copies `AT>` sections and clears `NOLOAD` ones goes
in `__zp_init`. Without a script, the units' bodies follow each other
as before, with no address.

`link_files` is the top-level entry point.
"""
from __future__ import annotations

from pathlib import Path

import tac_ast
from passes.abi_selection import ParamLayout, SoftStackLayout, ZpLayout
from passes.runtime_layout import RUNTIME_NAMES
from passes.zp_globals import AUTO_HI, INIT_FUNCTION
from passes.zp_link_metadata import (
    ExternMeta,
    FunctionMeta,
    LinkMetadata,
    ZpObjectMeta,
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


def _layout_text(layout: dict[str, int]) -> str:
    return ", ".join(
        f"{name}={addr}" if name == "HARGS size" else f"{name}=${addr:02X}"
        for name, addr in sorted(layout.items())
    ) or "(none)"


def _parse_equ(line: str) -> tuple[str, int]:
    """The name and address of a `<sym>\\tEQU\\t$<hex>` line."""
    name, _, value = line.split("\t")
    return name, int(value.strip().lstrip("$"), 16)


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
    # Build the abi dict from the metadata-embedded slot symbols.
    # Each TU baked the symbol strings into its body when emitting
    # the per-TU asm; the linker reuses them verbatim so the bodies
    # don't need rewriting — only the EQU block (which we mint
    # fresh below) and the addresses change.
    abi: dict[str, ParamLayout] = {}
    for d in merged.defs:
        if d.params:
            # Per-param register attributes preserved from per-TU
            # metadata so cross-TU callers emit the right register-
            # passing sequence at call sites. Empty-string entries
            # in `param_regs` normalize back to None for the layout.
            regs = [r if r else None for r in d.param_regs]
            # Pad to params length when the metadata was emitted
            # without param_regs (older TU output).
            while len(regs) < len(d.params):
                regs.append(None)
            abi[d.name] = ZpLayout(
                slot_symbols=list(d.params),
                addrs=[],
                param_registers=regs,
            )
        else:
            abi[d.name] = SoftStackLayout()
    defined = {d.name for d in merged.defs}
    for e in merged.externs:
        # A function defined in some unit takes its layout from the
        # definition; `_bind_extern_params` points each declaration's
        # slot symbols at the definition's.
        if e.name in defined:
            continue
        regs = [r if r else None for r in e.param_regs]
        while len(regs) < len(e.params):
            regs.append(None)
        abi[e.name] = ZpLayout(
            slot_symbols=list(e.params),
            addrs=[],
            param_registers=regs,
        )
    local_bytes = {d.name: d.local_bytes for d in merged.defs}
    return prog, abi, local_bytes


# ---------------------------------------------------------------------------
# Top-level entry.
# ---------------------------------------------------------------------------


def link_files(
    input_paths: list[Path | str],
    output_path: Path | str,
    script_path: Path | str | None = None,
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
    fixed: dict[str, int] = {}
    layout: dict[str, int] | None = None
    for path in input_paths:
        text = Path(path).read_text()
        equ, _, body = _split_asm(text)
        meta = parse_metadata(text)
        zp_names = {z.name for z in meta.zp_objects}
        # The other EQUs bind user names. One at $100 or above is a
        # function or object with `__attribute__((address(...)))`,
        # which every unit binds to the same address, so it's kept.
        # A zero-page object is listed in the metadata, and is placed
        # again below.
        unit_layout: dict[str, int] = {}
        if meta.hargs_size is not None:
            unit_layout["HARGS size"] = meta.hargs_size
        for line in equ:
            if line.startswith(("__zpabi_", "__local_")):
                continue
            name, addr = _parse_equ(line)
            if name in zp_names:
                continue
            if name in RUNTIME_NAMES:
                unit_layout[name] = addr
            elif addr < 0x100:
                raise LinkError(
                    f"{path}: {name!r} is bound to ${addr:02X} in zero "
                    f"page but isn't in the unit's link metadata; "
                    f"compile it again with this compiler"
                )
            elif fixed.setdefault(name, addr) != addr:
                raise LinkError(
                    f"{path}: {name!r} is at ${addr:04X} here and at "
                    f"${fixed[name]:04X} in an earlier unit"
                )
        # Every unit and the runtime's helpers have to agree on where
        # SSP, FP, HARGS, and DPTR are (passes/runtime_layout.py).
        if layout is None:
            layout = unit_layout
        elif unit_layout != layout:
            raise LinkError(
                f"{path}: its runtime zero-page layout "
                f"{_layout_text(unit_layout)} differs from an earlier "
                f"unit's {_layout_text(layout)}"
            )
        metas.append(meta)
        bodies.append(_without_init(body))
    from passes.link_layout import LayoutError
    script = None
    if script_path is not None:
        from passes.link_script import ScriptError, parse_script
        try:
            script = parse_script(Path(script_path).read_text())
        except ScriptError as e:
            raise LinkError(f"{script_path}: {e}") from e
        for path, meta, body in zip(input_paths, metas, bodies):
            if not meta.items and any(line.strip() for line in body):
                raise LinkError(
                    f"{path} has no section information in its link "
                    f"metadata; compile it again with this compiler"
                )
    # Give colliding internal names their own spellings.
    from passes.link_layout import rename_locals
    try:
        renamed = rename_locals(
            [([it for it in m.items if it.name != INIT_FUNCTION],
              m.zp_objects, body)
             for m, body in zip(metas, bodies)],
            set(fixed),
        )
    except LayoutError as e:
        raise LinkError(str(e)) from e
    bodies = [body for _, _, body in renamed]
    unit_items = [items for items, _, _ in renamed]
    for meta, (_, zp_objects, _) in zip(metas, renamed):
        meta.zp_objects = zp_objects
    # Merge + validate.
    merged = _merge(metas)
    _validate_eligibility(merged)
    # Place the zero-page objects before the slots, which keep off
    # their bytes.
    objects = _merge_zp_objects(metas)
    zp_addrs, demoted = _place_zp_objects(
        objects, layout or {}, bodies,
    )
    zp_bytes = frozenset(
        b for name, addr in zp_addrs.items()
        for b in range(addr, addr + objects[name][0].size)
    )
    merged.zp_objects = [
        ZpObjectMeta(
            name=name, size=objects[name][0].size, at=addr,
            defined=any(r.defined for r in objects[name]),
            init=next((r.init for r in objects[name] if r.defined), []),
        )
        for name, addr in sorted(zp_addrs.items(), key=lambda kv: kv[1])
    ]
    merged.hargs_size = (layout or {}).get("HARGS size")
    # Synthesize the program shape the allocators expect.
    prog, abi, local_bytes = _synthesize(merged)
    # Run the global allocators.
    abi, zp_sym_to_addr = allocate_zp_slots(prog, abi, reserved=zp_bytes)
    local_pools = allocate_function_locals(
        prog, abi, local_bytes, reserved=zp_bytes,
    )
    # Slot names round-trip through the per-TU metadata block;
    # reuse them verbatim so the body text doesn't need rewriting.
    slot_names_by_fn = {d.name: list(d.locals) for d in merged.defs}
    local_sym_to_addr = build_local_slot_symbols(
        local_pools, slot_names_by_fn=slot_names_by_fn,
    )
    all_symbols = {
        **{k: v for k, v in (layout or {}).items() if k in RUNTIME_NAMES},
        **zp_sym_to_addr, **local_sym_to_addr, **fixed, **zp_addrs,
    }
    _bind_extern_params(
        merged, [e for m in metas for e in m.externs], all_symbols,
    )
    # Format the output.
    out_lines: list[str] = []
    out_lines.extend(_format_equ_block(all_symbols))
    out_lines.append("")
    out_lines.extend(format_metadata(merged))
    out_lines.append("")
    if script is None:
        for i, body in enumerate(bodies):
            if i > 0:
                out_lines.append("")
            out_lines.extend(body)
        init = _init_lines(merged.zp_objects)
        if init:
            out_lines.append("")
            out_lines.extend(init)
        for name in demoted:
            record = next(r for r in objects[name] if r.defined)
            out_lines.append("")
            out_lines.extend(_data_lines(record))
    else:
        out_lines.extend(_scripted_body(
            script, input_paths, unit_items, bodies, merged, objects,
            demoted, (layout or {}).get("HARGS size"),
        ))
    Path(output_path).write_text("\n".join(out_lines) + "\n")


def _scripted_body(
    script, paths, unit_items, bodies, merged, objects, demoted,
    hargs_size,
) -> list[str]:
    """The program's body laid out by `script`, with the linker's own
    items (`__zp_init`, with the startup code, and the promoted
    objects put back in the data segment) in a unit of their own."""
    from asm_emit import _instr_line
    from passes.link_layout import (
        LayoutError, Unit, item_chunks, layout_lines, startup_lines,
    )
    from passes.zp_link_metadata import ItemMeta
    startup = startup_lines(script, _instr_line)
    if startup and hargs_size is not None and hargs_size < 4:
        raise LinkError(
            f"the startup code that copies and clears sections needs 4 "
            f"bytes of HARGS, and the runtime's block has {hargs_size}"
        )
    units = []
    try:
        for path, items, body in zip(paths, unit_items, bodies):
            units.append(Unit(
                path=str(path), items=items, chunks=item_chunks(body, items),
            ))
        own_items: list[ItemMeta] = []
        chunks: dict[str, list[str]] = {}
        init = _init_lines(merged.zp_objects, startup)
        if init:
            own_items.append(ItemMeta(name=INIT_FUNCTION, section=".text"))
            chunks[INIT_FUNCTION] = init
        for name in demoted:
            record = next(r for r in objects[name] if r.defined)
            zero = all(v == "$00" for v in record.init)
            own_items.append(ItemMeta(
                name=name, section=".bss" if zero else ".data", zero=zero,
            ))
            chunks[name] = (
                [f"{name}:", _instr_line("DS.B", str(record.size))]
                if zero else _data_lines(record)
            )
        units.append(Unit(path="the linker", items=own_items, chunks=chunks))
        return layout_lines(script, units, _instr_line)
    except LayoutError as e:
        raise LinkError(str(e)) from e


def _merge_zp_objects(
    metas: list[LinkMetadata],
) -> dict[str, list[ZpObjectMeta]]:
    """Every unit's records of each zero-page object, by name, in the
    order the units list them. Raises `LinkError` when the units
    disagree on an object's size or address, two define it, or none
    defines one that has no address (an `extern` without an address
    names an object some unit has to define and place)."""
    objects: dict[str, list[ZpObjectMeta]] = {}
    for meta in metas:
        for z in meta.zp_objects:
            objects.setdefault(z.name, []).append(z)
    for name, records in objects.items():
        if len({r.size for r in records}) > 1:
            raise LinkError(
                f"zero-page object `{name}` has different sizes in "
                f"different units"
            )
        if len({r.at for r in records if r.at is not None}) > 1:
            raise LinkError(
                f"zero-page object `{name}` is declared at different "
                f"addresses in different units"
            )
        defs = sum(r.defined for r in records)
        if defs > 1:
            raise LinkError(
                f"zero-page object `{name}` is defined in {defs} units"
            )
        if defs == 0 and all(r.at is None for r in records):
            raise LinkError(
                f"zero-page object `{name}` is declared `extern` but no "
                f"unit defines it"
            )
    return objects


def _place_zp_objects(
    objects: dict[str, list[ZpObjectMeta]],
    layout: dict[str, int],
    bodies: list[list[str]],
) -> tuple[dict[str, int], list[str]]:
    """Addresses for the zero-page objects: a declared address where
    any unit gives one, and otherwise the lowest free bytes below
    `$80`, in the order the units list the objects, around the fixed
    ones and the runtime's block (`layout`'s SSP, FP, HARGS, and
    DPTR, and its "HARGS size"). The objects every unit that lists
    them says `--optimize` promoted go last, densest first, and one
    that doesn't fit is returned in the second list, to go back in the
    data segment, unless a unit in `bodies` uses it as an `(name),Y`
    pointer. Raises `LinkError` on an overlap or a full range."""
    from passes.runtime_layout import CLASSIC, RuntimeLayout
    runtime = RuntimeLayout(
        ssp=layout.get("SSP", CLASSIC.ssp),
        fp=layout.get("FP", CLASSIC.fp),
        hargs=layout.get("HARGS", CLASSIC.hargs),
        hargs_size=layout.get("HARGS size", CLASSIC.hargs_size),
        dptr=layout.get("DPTR", CLASSIC.dptr),
    ).bytes()
    used: dict[int, str] = {}
    addrs: dict[str, int] = {}

    def claim(name: str, addr: int, size: int) -> None:
        for b in range(addr, addr + size):
            if b in used:
                raise LinkError(
                    f"zero-page object `{name}` at ${addr:02X} overlaps "
                    f"`{used[b]}` at byte ${b:02X}"
                )
        for b in range(addr, addr + size):
            used[b] = name
        addrs[name] = addr

    auto = []
    promoted = []
    for name, records in objects.items():
        size = records[0].size
        at = next((r.at for r in records if r.at is not None), None)
        if at is None:
            if all(r.weight is not None for r in records):
                weight = max(r.weight for r in records)
                promoted.append((-weight / size, name, size))
            else:
                auto.append((name, size))
            continue
        if any(b in runtime for b in range(at, at + size)):
            raise LinkError(
                f"zero-page object `{name}` at ${at:02X} overlaps the "
                f"runtime's zero-page block"
            )
        claim(name, at, size)
    def free(size: int) -> int | None:
        return next(
            (a for a in range(0, AUTO_HI - size + 1)
             if all(b not in used and b not in runtime
                    for b in range(a, a + size))),
            None,
        )

    for name, size in auto:
        addr = free(size)
        if addr is None:
            raise LinkError(
                f"no room for zero-page object `{name}` ({size} bytes) "
                f"below ${AUTO_HI:02X} in the linked program"
            )
        claim(name, addr, size)
    demoted = []
    for _, name, size in sorted(promoted):
        addr = free(size)
        if addr is not None:
            claim(name, addr, size)
            continue
        if any(f"({name})," in line for body in bodies for line in body):
            raise LinkError(
                f"no room for `{name}` ({size} bytes) below "
                f"${AUTO_HI:02X} in the linked program, and its unit "
                f"uses it as an `(zp),Y` pointer, which has to be in zero "
                f"page"
            )
        demoted.append(name)
    return addrs, demoted


def _data_lines(record: ZpObjectMeta) -> list[str]:
    """A promoted object that didn't fit, back in the data segment
    with its initial bytes."""
    from asm_emit import _instr_line
    return [f"{record.name}:", _instr_line("DC.B", ", ".join(record.init))]


def _without_init(body: list[str]) -> list[str]:
    """`body` without its `__zp_init` function, which `_init_lines`
    replaces with one for the whole program. The function runs from
    its label to the next top-level label (a line with no leading
    space or dot that ends in `:`)."""
    try:
        start = body.index(f"{INIT_FUNCTION}:")
    except ValueError:
        return body
    end = start + 1
    while end < len(body) and not (
        body[end].endswith(":") and body[end][:1] not in (" ", "\t", ".")
    ):
        end += 1
    out = body[:start] + body[end:]
    while out and out[-1] == "":
        out.pop()
    return out


def _init_lines(
    objects: list[ZpObjectMeta], startup: list[str] = (),
) -> list[str]:
    """`__zp_init` for the linked program: stores of every defined
    zero-page object's initial bytes, each value loaded once, zero
    first, the way `zp_globals.place_zp_globals` writes it for one
    unit, then `startup` (`link_layout.startup_lines`). Empty when
    there's nothing to do."""
    from asm_emit import _instr_line
    stores: dict[str, list[str]] = {}
    for z in objects:
        for k, value in enumerate(z.init):
            stores.setdefault(value, []).append(
                z.name if k == 0 else f"{z.name}+{k}"
            )
    if not stores and not startup:
        return []
    lines = [f"{INIT_FUNCTION}:", _instr_line("SUBROUTINE"), ""]
    lines.extend(startup)
    for value in sorted(stores, key=lambda v: v != "$00"):
        lines.append(_instr_line("LDA", f"#{value}"))
        lines.extend(_instr_line("STA", d) for d in stores[value])
    lines.append(_instr_line("RTS"))
    return lines


def _bind_extern_params(
    merged: LinkMetadata,
    externs: list[ExternMeta],
    symbols: dict[str, int],
) -> None:
    """Binds the parameter slot symbols of every unit's declaration
    of a zp_abi function to the addresses of the definition's, by
    position, adding them to `symbols`.

    Each unit names a parameter's slot after the name its own
    declaration gives the parameter (`__zpabi_put__x`), or after the
    parameter's position when the declaration leaves it unnamed
    (`__zpabi_put__0`), so a unit that calls `put` and the unit that
    defines it can spell the same byte differently. A function that
    no unit defines takes the first declaration's symbols."""
    canonical: dict[str, tuple[list[str], list[str]]] = {
        d.name: (list(d.params), _regs(d.param_regs, len(d.params)))
        for d in merged.defs
    }
    for e in merged.externs:
        canonical.setdefault(
            e.name, (list(e.params), _regs(e.param_regs, len(e.params))),
        )
    for e in externs:
        params, regs = canonical[e.name]
        if len(params) != len(e.params):
            raise LinkError(
                f"`{e.name}` is declared with {len(e.params)} zp_abi "
                f"parameter byte(s) in one unit and has {len(params)} "
                f"where it's defined"
            )
        if _regs(e.param_regs, len(e.params)) != regs:
            raise LinkError(
                f"`{e.name}` is declared with its parameters in "
                f"different registers in two units"
            )
        for sym, canon in zip(e.params, params):
            addr = symbols[canon]
            if symbols.setdefault(sym, addr) != addr:
                raise LinkError(
                    f"`{sym}` names two different parameter bytes of "
                    f"`{e.name}`; give the parameters the same names in "
                    f"its declarations and its definition"
                )


def _regs(param_regs: list[str], count: int) -> list[str]:
    """`param_regs` padded with "" (no register) to `count` entries."""
    return list(param_regs) + [""] * (count - len(param_regs))


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
