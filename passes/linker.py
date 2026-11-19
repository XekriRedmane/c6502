"""The c6502 linker: links relocatable objects (`compile.py -c -O`)
into an executable.

    compile.py -O a.o b.o -o prog          # one section after another
    compile.py -O -T script.ld a.o b.o -o prog

Linking is where c6502 does its whole-program optimization: each unit
leaves its zero-page slots, its zero-page objects, and the runtime's
block to the linker, which places them for the program as a whole.

What "linking" does at the call-graph level:

  * Unions every unit's `LinkMetadata` (see
    `passes.zp_link_metadata`, the section `.c6502.link` of each
    object) into a program-level view of function definitions, zp_abi
    extern declarations, and call edges.
  * Detects cross-TU duplicate definitions — link error.
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

Eligibility caveat. The linker requires every function defined across
all input TUs to be eligible for the private-pool model: no
`IndirectCall`, no participation in any cycle (local or cross-TU,
except one whose every call saves on the hardware stack), and every
callee either defined in some TU or declared zp_abi extern.

Addressing modes. Each unit was assembled with the addressing modes
its own compile chose, from its own addresses for the names the
linker places. Two things can differ in the whole program.

  * A name the unit reached with zero-page addressing (`R_MOS_ADDR8`)
    can end up above $FF: a slot that no longer fits in zero page, or
    a global that `-O` promoted into zero page and that the program
    has no room for, which goes back in the data segment. Each object
    keeps the text it was assembled from (`.c6502.asm`), and the
    linker assembles such a unit again with the program's addresses
    (`_reassemble`), so its instructions reach the name with absolute
    addressing.
  * A name the unit reached with absolute addressing can end up in
    zero page: most often a global another unit promoted, which this
    unit only declared `extern`. The linker shortens each such
    instruction to its zero-page form (`binutils.ld.relax`).

Either way, each instruction gets the addressing mode dasm would have
given it from the program's addresses.

The runtime's block. Every unit names SSP, FP, HARGS, and DPTR only by
symbol, and lists how many HARGS bytes it uses, so the linker chooses
the program's layout (`_choose_layout`) the way the compiler does for
one TU, from every TU's fixed zero-page objects and the largest
need. A TU that includes `<apple2/monitor_io.h>`, which moves the
block, links with one that doesn't.

Zero-page objects (`passes/zp_globals.py`). Each unit lists its
zero-page objects in the metadata, and refers to them only by name,
so the linker places them for the whole program
(`_place_zp_objects`): an object declared with an address keeps it,
and the rest go in the lowest free bytes below `$80`, the declared
ones first and then the ones `-O` promoted (`weight=` in the
metadata), densest first. Their bytes are kept out of the slot
allocators, and each unit's `__zp_init` is left out in favor of one
the linker writes, which stores every object's initial bytes
(`_init_lines`) and runs the startup code a script needs.

Names. An internal name (a `static`, or a string literal `str@N`)
that another unit also defines, or that names something else in the
program, is renamed `name.k` in unit `k` (`_rename_locals`), so that
every item has a name of its own in the executable. A renamed
function's zero-page slots, which each unit spells after the function,
are renamed with it, so the allocators place two functions; two units
defining one external name is an error. A name no unit defines and
the linker doesn't place has to be given with `--defsym`, except a
runtime helper with 6502 code.

The runtime library (passes/runtime_library.py). The helpers the
units call to multiply, divide, and shift, which no unit defines and
no `--defsym` gives, and the helpers those call, come from the library
as one more object, `runtime.o`, with a section for each helper
(`_runtime_library`). It goes after the units, so without a script
its helpers follow their code in `.text`, and a script places them as
`runtime.o(.text)` or with a pattern such as `*(.text)`. Its metadata
gives the HARGS bytes the helpers touch, which `_choose_layout` counts
in the program's block.

Merged tables. The halves of the tables of row addresses that `-O`
makes (passes/optimization/row_address_tables.py) are marked `merge`
in the metadata, and the program keeps one copy of those with the same
bytes and relocations (`_merge_identical`), however many units make
them.

Layout (passes/link_layout.py). Each item is a section of its own in
its object. With a linker script (`-T`, passes/link_script.py), the
script places them. Without one, `.text`, `.rodata`, `.data`, and
`.bss` follow each other from the origin ($0800 unless given). A
slot that doesn't fit in zero page goes in `__spill`, an item in
`.bss`, when there's a script, instead of at the allocators' fixed
spill address ($0200 up), which the program may use for something
else.

The output is an ELF executable (binutils/elf.py) with one loadable
segment per output section, every symbol of every unit at its
address, the names the linker placed as absolute symbols, and the
merged link metadata.

`link_objects` and `link_files` are the entry points.
"""
from __future__ import annotations

from pathlib import Path

import tac_ast
from binutils import elf as E
from binutils.ld import (
    LinkError, Resolver, executable, global_definitions, relax,
)
from passes.abi_selection import ParamLayout, SoftStackLayout, ZpLayout
from passes.optimization.pool import Pool
from passes.runtime_layout import RUNTIME_NAMES
from passes.zp_globals import AUTO_HI, INIT_FUNCTION
from passes.zp_link_metadata import (
    ExternMeta,
    FunctionMeta,
    ItemMeta,
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

__all__ = [
    "LinkError", "ZpLocalAllocationError", "ZpSlotAllocationError",
    "link_files", "link_objects",
]

# Where a link without a script puts `.text`.
DEFAULT_ORIGIN = 0x0800


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
            for other in (existing, defs_by_name.get(e.name)):
                if (other is not None
                        and other.param_bytes == e.param_bytes
                        and _arrivals(other) != _arrivals(e)):
                    raise LinkError(
                        f"the units declare `{e.name}` with different "
                        f"parameters in registers or at zero-page "
                        f"addresses; every declaration has to give the "
                        f"same calling convention"
                    )
                if other is not None and other.carry != e.carry:
                    raise LinkError(_carry_conflict(e.name))
    seen_edges: set[tuple[str, str]] = set()
    saved_edges: set[tuple[str, str]] = set()
    for m in metadatas:
        seen_edges.update(m.calls)
        saved_edges.update(m.saved_calls)
    merged = LinkMetadata(
        defs=sorted(defs_by_name.values(), key=lambda d: d.name),
        externs=sorted(externs_by_name.values(), key=lambda e: e.name),
        calls=sorted(seen_edges),
        saved_calls=sorted(saved_edges),
        shares=[s for m in metadatas for s in m.shares],
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


def _carry_conflict(name: str) -> str:
    """The error for units that disagree on whether `name` returns
    its `bool` in the carry flag."""
    return (
        f"the units disagree on whether `{name}` returns its `bool` in "
        f"the carry flag. Under -O, a `bool` function with "
        f"external linkage does when it's zp_abi, so a unit that takes "
        f"its address, or one that can't give it zp_abi, sees it "
        f"differently from the others. A function that returns "
        f"`unsigned char` returns it in A in every unit"
    )


def _arrivals(meta) -> tuple:
    """Where each parameter byte of a `def` or `ext` record arrives:
    its register, its zero-page address, or neither."""
    n = meta.param_bytes
    regs = list(meta.param_regs) + [""] * (n - len(meta.param_regs))
    pinned = list(meta.pinned) + [None] * (n - len(meta.pinned))
    return tuple(zip((r or "" for r in regs), pinned))


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
    # Cycle members from any TU's local view, except a cycle whose
    # every call saves its caller's values on the hardware stack
    # (`[[c6502::save_hardware_stack]]`), which the TU's
    # compile gave pools as the allocators here do. Cross-TU cycles
    # are detected as a side effect of `allocate_zp_slots` /
    # `allocate_function_locals` (they raise their own errors).
    from passes.recursion_saves import hw_stack_cycles
    saved_cycles = hw_stack_cycles(_synthesize(merged)[0])
    locally_cyclic = [
        d.name for d in merged.defs
        if d.in_cycle and d.name not in saved_cycles
    ]
    if locally_cyclic:
        raise LinkError(
            f"function(s) on a cycle in their own TU's call "
            f"graph (recursion is supported only when every call "
            f"in the cycle is marked "
            f"`[[c6502::save_hardware_stack]]`): {sorted(locally_cyclic)}",
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
    saved = set(merged.saved_calls)
    # Synthesize one tac_ast.Function per def.
    top_level: list[tac_ast.Type_top_level] = []
    for d in merged.defs:
        instrs: list[tac_ast.Type_instruction] = []
        for callee in callees_of.get(d.name, ()):
            instrs.append(tac_ast.FunctionCall(
                name=callee, args=[], dst=None,
                hw_stack_save=(d.name, callee) in saved,
            ))
        top_level.append(tac_ast.Function(
            name=d.name, is_global=True, params=[],
            instructions=instrs,
        ))
    prog = tac_ast.Program(top_level=top_level)
    # Build the abi dict from the metadata-embedded slot symbols.
    # Each TU's object names its slots by these symbols, which it
    # leaves undefined, so only the addresses the linker gives them
    # change.
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
                pinned=list(d.pinned),
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
            pinned=list(e.pinned),
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
    *,
    origin: int = DEFAULT_ORIGIN,
    defsyms: dict[str, int] | None = None,
) -> None:
    """Links the objects at `input_paths` into the executable at
    `output_path`, laid out by the linker script at `script_path` if
    one is given. Raises `LinkError` on any failure, and
    `ZpSlotAllocationError` / `ZpLocalAllocationError` when the merged
    graph's slots can't be allocated."""
    inputs = []
    for path in input_paths:
        try:
            inputs.append((str(path), E.read_file(path)))
        except E.ElfError as e:
            raise LinkError(str(e)) from e
    script = read_script(script_path) if script_path is not None else None
    exe = link_objects(inputs, script, origin=origin, defsyms=defsyms)
    Path(output_path).write_bytes(E.write(exe))


def read_script(path: Path | str):
    """The parsed linker script at `path`."""
    from passes.link_script import ScriptError, parse_script
    try:
        return parse_script(Path(path).read_text())
    except ScriptError as e:
        raise LinkError(f"{path}: {e}") from e


def link_objects(
    inputs: list[tuple[str, E.ElfFile]],
    script=None,
    *,
    origin: int = DEFAULT_ORIGIN,
    defsyms: dict[str, int] | None = None,
) -> E.ElfFile:
    """The executable that links `inputs`, each an object and the name
    it goes by (which the script's file patterns match without its
    directory). `script` is a parsed linker script, or None for the
    default layout from `origin`. `defsyms` gives addresses for names
    no object defines, and a runtime helper it names isn't taken from
    the runtime library."""
    from passes.link_layout import (
        LayoutError, Unit, default_script, lay_out, startup_lines,
        tail_jump,
    )
    if not inputs:
        raise LinkError("linking needs at least one object")
    defsyms = dict(defsyms or {})
    metas: list[LinkMetadata] = []
    objects: list[tuple[str, E.ElfFile]] = []
    for path, obj in inputs:
        if obj.type != E.ET_REL:
            raise LinkError(f"{path} isn't a relocatable object")
        text = obj.link_text()
        try:
            meta = parse_metadata(text) if text else LinkMetadata()
        except ValueError as e:
            raise LinkError(f"{path}: {e}") from e
        meta.items = _object_items(meta, obj, path)
        objects.append((path, _without_init(obj)))
        meta.items = [it for it in meta.items if it.name != INIT_FUNCTION]
        metas.append(meta)
    # The runtime's helpers the objects call, from the library.
    library = _runtime_library(objects, defsyms)
    if library is not None:
        objects.append(library[0])
        metas.append(library[1])
    _, absolute = global_definitions(objects)
    fixed = dict(absolute)
    # Give colliding internal names their own spellings.
    renames = _rename_locals(objects, metas, set(fixed))
    # Merge + validate.
    merged = _merge(metas)
    _validate_eligibility(merged)
    _drop_unused_locals(merged, objects)
    # Place the runtime's block, then the zero-page objects, then the
    # slots, each keeping off the bytes before it.
    zp_objects = _merge_zp_objects(metas)
    startup = []
    if script is not None:
        from asm_emit import _instr_line
        startup = startup_lines(script, _instr_line)
    # The bytes where routines with a fixed address read a
    # `zeropage(addr)` parameter: nothing else of the program's may go
    # there.
    param_bytes = frozenset(
        a for m in metas for r in [*m.defs, *m.externs]
        for a in r.pinned if a is not None
    )
    layout = _choose_layout(
        metas, zp_objects, 4 if startup else 0, param_bytes,
    )
    zp_addrs, demoted = _place_zp_objects(
        zp_objects, layout, param_bytes, _pointer_names(objects),
    )
    zp_bytes = frozenset(
        b for name, addr in zp_addrs.items()
        for b in range(addr, addr + zp_objects[name][0].size)
    ) | param_bytes
    merged.zp_objects = [
        ZpObjectMeta(
            name=name, size=zp_objects[name][0].size, at=addr,
            defined=any(r.defined for r in zp_objects[name]),
            init=next((r.init for r in zp_objects[name] if r.defined), []),
        )
        for name, addr in sorted(zp_addrs.items(), key=lambda kv: kv[1])
    ]
    merged.hargs_size = layout["HARGS size"]
    merged.hargs_need = max(
        (m.hargs_need for m in metas if m.hargs_need is not None),
        default=None,
    )
    # The values kept in parameter bytes across calls, as one program's
    # (passes/slot_sharing.py).
    merged.shares = _link_shares(merged, metas)
    # Synthesize the program shape the allocators expect.
    prog, abi, local_bytes = _synthesize(merged)
    # Every linked function has a private pool (`_validate_eligibility`),
    # so no function keeps a value in the pool's callee-saved half across
    # a call, and the slots can come from the whole pool.
    pool = Pool()
    window = range(pool.start, 0x100)
    # The free bytes below the pool, around the runtime's block, the
    # zero-page objects, and the bytes a program's headers reserve
    # for its ROM and DOS, take what doesn't fit in the pool before
    # anything spills out of zero page.
    fallback = range(0, pool.start)
    low_reserved = zp_bytes | _runtime_bytes(layout)
    # With a script, a slot that doesn't fit in zero page goes in
    # `__spill`, laid out with the program's data: it gets a stand-in
    # address here, rebased below.
    spill = (
        {"spill_start": _SPILL_BASE, "spill_end": 0x10000}
        if script is not None else {}
    )
    abi, zp_sym_to_addr = allocate_zp_slots(
        prog, abi, reserved=low_reserved, window=window,
        fallback=fallback, shares=merged.shares, **spill,
    )
    local_pools = allocate_function_locals(
        prog, abi, local_bytes, reserved=low_reserved, window=window,
        fallback=fallback, shares=merged.shares, **spill,
    )
    # Slot names round-trip through the per-TU metadata block, and the
    # objects name the slots by them.
    slot_names_by_fn = {d.name: list(d.locals) for d in merged.defs}
    local_sym_to_addr = build_local_slot_symbols(
        local_pools, slot_names_by_fn=slot_names_by_fn,
    )
    runtime = {k: v for k, v in layout.items() if k in RUNTIME_NAMES}
    placed_names = {
        **runtime, **zp_sym_to_addr, **local_sym_to_addr, **zp_addrs,
    }
    _bind_extern_params(
        merged, [e for m in metas for e in m.externs], placed_names,
    )
    spilled = {
        name: addr - _SPILL_BASE for name, addr in placed_names.items()
        if script is not None and addr >= _SPILL_BASE
    }
    for name in spilled:
        del placed_names[name]

    # A unit that reaches a name with zero-page addressing, where the
    # program puts the name above $FF, is assembled again with the
    # program's addresses (`_reassemble`).
    _reassemble(objects, renames, placed_names, set(demoted) | set(spilled))

    # Instructions that reach a name the linker put in zero page with
    # absolute addressing get the zero-page form (`ld.relax`).
    section_defs, absolute = global_definitions(objects)

    def known(sym: E.Symbol) -> int | None:
        if sym.defined:
            return sym.value if sym.section == E.SHN_ABS else None
        if sym.name in section_defs:
            return None
        for table in (placed_names, absolute, defsyms):
            if sym.name in table:
                return table[sym.name]
        return None

    for _, obj in objects:
        for sec in obj.sections:
            relax(sec, obj.symbols, known)

    # The compiler's tables that several units make with the same
    # bytes are one table in the program.
    _merge_identical(objects, metas)

    # The linker's own items: `__zp_init`, `__spill`, and the promoted
    # objects put back in the data segment.
    from asm_emit import _instr_line
    own_lines = _init_lines(merged.zp_objects, startup)
    spill_size = max(spilled.values(), default=-1) + 1
    if spill_size:
        own_lines += ["", f"{_SPILL}:", _instr_line("DS.B", str(spill_size))]
    for name in demoted:
        record = next(r for r in zp_objects[name] if r.defined)
        own_lines += [""] + _data_lines(record)
    units: list[Unit] = []
    for k, ((path, obj), meta) in enumerate(zip(objects, metas)):
        units.append(_unit(k, path, obj, meta.items, tail_jump))
    if own_lines:
        from binutils.assembler import AssemblerError, assemble
        try:
            own = assemble(
                "\n".join(own_lines) + "\n", "the linker",
                link_symbols={**runtime, **zp_addrs},
            )
        except AssemblerError as e:
            raise LinkError(f"the linker's own code: {e}") from e
        own_items = _object_items(LinkMetadata(), own, "the linker")
        objects.append(("the linker", own))
        units.append(_unit(len(units), "the linker", own, own_items,
                           tail_jump))

    # Lay the program out.
    _, absolute = global_definitions(objects)

    def lookup(name: str) -> int | None:
        for table in (placed_names, absolute, defsyms):
            if name in table:
                return table[name]
        return None

    try:
        placement = lay_out(
            script if script is not None else default_script(units, origin),
            units, lookup,
        )
    except LayoutError as e:
        raise LinkError(str(e)) from e
    placed = [p for out in placement.outputs for p in out.parts]
    addr_of = {id(p.section): p for p in placed}
    if spilled:
        spill_section = units[-1].sections[_SPILL]
        base = addr_of[id(spill_section)].addr
        for name, offset in spilled.items():
            placed_names[name] = base + offset
    extra = {**placed_names, **placement.symbols, **defsyms}
    resolver = Resolver(objects, placed, extra)
    symbols = _output_symbols(
        objects, placement.outputs, addr_of, placed_names, zp_objects,
        placement.symbols, defsyms,
    )
    main = resolver.defs.get("main")
    entry = resolver.value(main[1], main[0]) if main else 0
    link_text = "\n".join(format_metadata(merged)) + "\n"
    return executable(objects, placement.outputs, resolver, symbols, entry,
                      link_text)


# Spilled slots get addresses from here up while allocating, which
# `link_objects` then rebases onto `__spill` when a script lays the
# program out.
_SPILL_BASE = 0xF000
_SPILL = "__spill"


def _runtime_library(
    objects: list[tuple[str, E.ElfFile]], defsyms: dict[str, int],
) -> tuple[tuple[str, E.ElfFile], LinkMetadata] | None:
    """The object of the runtime's helpers that `objects` call and
    neither define nor get from `defsyms`, with its name and its link
    metadata, or None when they call none (passes/runtime_library.py)."""
    from passes.runtime_library import LIBRARY_NAME, library_object, needed
    defs, absolute = global_definitions(objects)
    names = needed(objects, set(defs) | set(absolute) | set(defsyms))
    if not names:
        return None
    obj = library_object(names)
    meta = parse_metadata(obj.link_text())
    meta.items = _object_items(meta, obj, LIBRARY_NAME)
    return (LIBRARY_NAME, obj), meta


def _object_items(meta: LinkMetadata, obj: E.ElfFile, path: str) -> list[ItemMeta]:
    """The items of `obj` in order: its metadata's, or, for an object
    without metadata, one for each function and object symbol, in the
    order of its sections. Raises `LinkError` when the metadata names
    an item the object doesn't define."""
    defined = {
        s.name: s for s in obj.symbols
        if isinstance(s.section, E.Section)
        and s.type in (E.STT_FUNC, E.STT_OBJECT)
    }
    if meta.items:
        missing = [it.name for it in meta.items if it.name not in defined]
        if missing:
            raise LinkError(
                f"{path}: the link metadata lists {sorted(missing)}, which "
                f"the object doesn't define"
            )
        return list(meta.items)
    order = {id(s): i for i, s in enumerate(obj.sections)}
    syms = sorted(defined.values(), key=lambda s: order[id(s.section)])
    return [
        ItemMeta(name=s.name, section=s.section.name, local=not s.is_global,
                 zero=s.section.type == E.SHT_NOBITS)
        for s in syms
    ]


def _item_symbol(obj: E.ElfFile, name: str) -> E.Symbol:
    return next(
        s for s in obj.symbols
        if s.name == name and isinstance(s.section, E.Section)
        and s.type in (E.STT_FUNC, E.STT_OBJECT)
    )


def _without_init(obj: E.ElfFile) -> E.ElfFile:
    """`obj` without its `__zp_init` function, which the linker
    replaces with one for the whole program."""
    sym = next(
        (s for s in obj.symbols if s.name == INIT_FUNCTION
         and isinstance(s.section, E.Section) and s.type == E.STT_FUNC),
        None,
    )
    if sym is None:
        return obj
    sec = sym.section
    return E.ElfFile(
        type=obj.type,
        sections=[s for s in obj.sections if s is not sec],
        symbols=[s for s in obj.symbols if s.section is not sec],
        entry=obj.entry, flags=obj.flags,
    )


def _unit(k: int, path: str, obj: E.ElfFile, items: list[ItemMeta],
          tail_jump):
    from passes.link_layout import Unit
    sections = {it.name: _item_symbol(obj, it.name).section for it in items}
    tails = {}
    for it in items:
        sec = sections[it.name]
        if sec.flags & E.SHF_EXECINSTR:
            target = tail_jump(sec, obj.symbols)
            if target is not None:
                tails[it.name] = target
    return Unit(path=path, index=k, items=items, sections=sections,
                tails=tails)


def _rename_locals(
    objects: list[tuple[str, E.ElfFile]], metas: list[LinkMetadata],
    taken: set[str],
) -> list[dict[str, str]]:
    """Gives every internal item and internal zero-page object whose
    name another unit also uses, or that `taken` (the program's other
    names) holds, a name of its own: `name.k`, for unit `k`, in the
    unit's metadata and symbols. Returns each unit's renames. Raises
    `LinkError` when two units define the same external name.

    A renamed function is a `static` one, or a shared tail or outlined
    run (`F@tail<n>`, `F@run<n>`) named after one. Each unit spells its
    slots after the function (`__zpabi_f__x`, `__local_f__t`) and
    leaves them undefined, so two units' `f` would otherwise share
    their slots. The slots get `.k` too, and so does every record of
    the metadata that names the function or its slots (`_rename_meta`):
    its `def` line, the call edges, and the shares. The allocators then
    place two functions, each with its own place in the call graph."""
    owner: dict[str, int] = {}
    users: dict[str, set[int]] = {}
    for k, meta in enumerate(metas):
        for it in meta.items:
            users.setdefault(it.name, set()).add(k)
            if not it.local:
                if it.name in owner:
                    raise LinkError(
                        f"`{it.name}` is defined in multiple TUs"
                    )
                owner[it.name] = k
        for z in meta.zp_objects:
            users.setdefault(z.name, set()).add(k)
    out: list[dict[str, str]] = []
    for k, ((_, obj), meta) in enumerate(zip(objects, metas)):
        local_names = (
            [it.name for it in meta.items if it.local]
            + [z.name for z in meta.zp_objects if z.local]
        )
        renames = {
            name: f"{name}.{k}" for name in local_names
            if len(users[name]) > 1 or name in taken
        }
        for d in meta.defs:
            if d.name in renames:
                for sym in [*d.params, *d.locals]:
                    renames[sym] = f"{sym}.{k}"
        out.append(renames)
        if not renames:
            continue
        _apply_renames(obj, renames)
        _rename_meta(meta, renames)
    return out


def _rename_meta(meta: LinkMetadata, renames: dict[str, str]) -> None:
    """Applies `renames` (old name to new) to every function, slot,
    item, and zero-page object name in `meta`."""
    import dataclasses

    def r(name: str) -> str:
        return renames.get(name, name)

    def rs(names) -> list[str]:
        return [r(n) for n in names]

    meta.items = [dataclasses.replace(it, name=r(it.name))
                  for it in meta.items]
    meta.zp_objects = [dataclasses.replace(z, name=r(z.name))
                       for z in meta.zp_objects]
    meta.defs = [
        dataclasses.replace(
            d, name=r(d.name), params=rs(d.params), locals=rs(d.locals),
            changes=rs(d.changes),
        )
        for d in meta.defs
    ]
    meta.externs = [
        dataclasses.replace(e, name=r(e.name), params=rs(e.params))
        for e in meta.externs
    ]
    meta.calls = [(r(a), r(b)) for a, b in meta.calls]
    meta.saved_calls = [(r(a), r(b)) for a, b in meta.saved_calls]
    meta.shares = [
        dataclasses.replace(
            s,
            members=tuple((r(f), tuple(rs(syms))) for f, syms in s.members),
            holders=tuple(rs(s.holders)),
            live=tuple((r(f), tuple(rs(cs))) for f, cs in s.live),
            forwards=tuple((r(a), r(b)) for a, b in s.forwards),
            writers=tuple(rs(s.writers)),
        )
        for s in meta.shares
    ]


def _apply_renames(obj: E.ElfFile, renames: dict[str, str]) -> None:
    for s in obj.symbols:
        if s.name in renames and not (s.is_global and s.defined):
            s.name = renames[s.name]


def _pointer_names(objects: list[tuple[str, E.ElfFile]]) -> set[str]:
    """The names some unit uses as an `(name),Y` or `(name,X)`
    pointer, which has to be in zero page."""
    from binutils.opcodes import DECODE
    out: set[str] = set()
    for _, obj in objects:
        for sec in obj.sections:
            for r in sec.relocs:
                if r.type != E.R_MOS_ADDR8 or r.offset == 0:
                    continue
                op = DECODE.get(sec.data[r.offset - 1])
                if op is not None and op[1] in ("indx", "indy"):
                    out.add(r.symbol.name)
    return out


def _reassemble(
    objects: list[tuple[str, E.ElfFile]], renames: list[dict[str, str]],
    placed: dict[str, int], outside: set[str],
) -> None:
    """Assembles again, from the text it keeps (`.c6502.asm`), each
    unit that reaches one of the names the linker placed with
    zero-page addressing where the program puts that name above $FF
    or in `outside` (a slot spilled into `__spill`, or a promoted
    object put back in the data segment). Its instructions then reach
    the name with absolute addressing, as dasm assembled the linked
    text before the toolchain had objects. A unit without its text is
    left alone, and `ld.relocate` reports the relocation that doesn't
    fit."""
    from binutils.assembler import AssemblerError, assemble
    values: dict[str, int | None] = {name: None for name in outside}
    values.update(placed)
    for k, (path, obj) in enumerate(objects):
        clash = any(
            r.type == E.R_MOS_ADDR8 and not r.symbol.defined
            and (r.symbol.name in outside
                 or placed.get(r.symbol.name, 0) + r.addend > 0xFF)
            for sec in obj.sections for r in sec.relocs
        )
        text = obj.source_text()
        if not clash or text is None:
            continue
        # The unit's own names, before `_rename_locals`.
        back = {new: old for old, new in renames[k].items()}
        unit_values = {back.get(n, n): v for n, v in values.items()}
        filename = next(
            (s.name for s in obj.symbols if s.type == E.STT_FILE), path,
        )
        try:
            new = assemble(text, filename, link_symbols=unit_values)
        except AssemblerError as e:
            raise LinkError(
                f"{path}: assembling it again with the program's zero-page "
                f"addresses: {e}"
            ) from e
        new = _without_init(new)
        _apply_renames(new, renames[k])
        objects[k] = (path, new)


def _data_lines(record: ZpObjectMeta) -> list[str]:
    """A promoted object that didn't fit, back in the data segment
    with its initial bytes."""
    from asm_emit import _instr_line
    if not record.init or all(v == "$00" for v in record.init):
        return [f"{record.name}:", _instr_line("DS.B", str(record.size))]
    return [f"{record.name}:", _instr_line("DC.B", ", ".join(record.init))]


def _output_symbols(
    objects, outputs, addr_of, placed_names, zp_objects, layout_symbols,
    defsyms,
) -> list[E.Symbol]:
    """The executable's symbol table: every unit's file symbol, and
    each symbol it defines in a placed section, at its address; the
    absolute symbols, once each; and the names the linker placed or
    defined, as absolute symbols."""
    output_of = {
        id(p.section): out for out in outputs for p in out.parts
    }
    out: list[E.Symbol] = []
    seen_abs: set[str] = set()
    for path, obj in objects:
        out.append(E.Symbol(
            name=Path(path).name, bind=E.STB_LOCAL, type=E.STT_FILE,
            section=E.SHN_ABS,
        ))
        for s in obj.symbols:
            if s.type == E.STT_FILE or not s.defined:
                continue
            if s.section == E.SHN_ABS:
                if s.is_global and s.name not in seen_abs:
                    seen_abs.add(s.name)
                    out.append(E.Symbol(
                        name=s.name, value=s.value, bind=E.STB_GLOBAL,
                        section=E.SHN_ABS,
                    ))
                continue
            p = addr_of.get(id(s.section))
            if p is None:
                continue
            if s.value >= p.size and s.type not in (E.STT_FUNC, E.STT_OBJECT) \
                    and p.size < s.section.size:
                # A label on the `JMP` the layout dropped, which is the
                # next function's address; that function's label names
                # it in the executable.
                continue
            out.append(E.Symbol(
                name=s.name, value=p.addr + s.value,
                size=p.size if s.type in (E.STT_FUNC, E.STT_OBJECT)
                else s.size,
                bind=s.bind, type=s.type,
                section=output_of[id(s.section)], other=s.other,
            ))
    sizes = {name: records[0].size for name, records in zp_objects.items()}
    for table in (placed_names, layout_symbols, defsyms):
        for name, value in sorted(table.items(), key=lambda kv: kv[1]):
            if name in seen_abs:
                continue
            seen_abs.add(name)
            out.append(E.Symbol(
                name=name, value=value & 0xFFFF, size=sizes.get(name, 0),
                bind=E.STB_GLOBAL,
                type=E.STT_OBJECT if name in sizes else E.STT_NOTYPE,
                section=E.SHN_ABS,
            ))
    return out


def _runtime_bytes(layout: dict[str, int]) -> frozenset[int]:
    """The bytes of the runtime's block (SSP, FP, DPTR, and HARGS)
    in `layout`, as `_choose_layout` returns it."""
    size = layout["HARGS size"]
    out: set[int] = set()
    for name, n in (("SSP", 2), ("FP", 2), ("DPTR", 2), ("HARGS", size)):
        out.update(range(layout[name], layout[name] + n))
    return frozenset(out)


def _choose_layout(
    metas, objects, startup_need: int,
    param_bytes: frozenset[int] = frozenset(),
) -> dict[str, int]:
    """The program's runtime layout, as SSP, FP, HARGS, DPTR, and
    "HARGS size": `runtime_layout.choose_layout` for the program's
    fixed zero-page bytes and the largest HARGS need of any unit (at
    least `startup_need`). A unit without metadata needs the classic
    block's HARGS. Raises `LinkError` when the block doesn't fit."""
    from passes.runtime_layout import (
        CLASSIC, RuntimeLayoutError, choose_layout,
    )
    needs = [
        m.hargs_need if m.hargs_need is not None else CLASSIC.hargs_size
        for m in metas
    ]
    reserved = frozenset(
        b for records in objects.values() for r in records
        if r.at is not None for b in range(r.at, r.at + r.size)
    ) | param_bytes
    need = max(needs + [startup_need])
    try:
        chosen = choose_layout(reserved, lambda: need)
    except RuntimeLayoutError as e:
        raise LinkError(str(e)) from e
    if chosen.hargs_size < need:
        raise LinkError(
            f"the program needs {need} HARGS bytes, and the runtime's "
            f"block has {chosen.hargs_size}"
        )
    return {**chosen.addrs(), "HARGS size": chosen.hargs_size}


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
    param_bytes: frozenset[int] = frozenset(),
    pointers: set[str] = frozenset(),
) -> tuple[dict[str, int], list[str]]:
    """Addresses for the zero-page objects: a declared address where
    any unit gives one, and otherwise the lowest free bytes below
    `$80`, in the order the units list the objects, around the fixed
    ones and the runtime's block (`layout`'s SSP, FP, HARGS, and
    DPTR, and its "HARGS size"). The objects every unit that lists
    them says `-O` promoted go last, densest first, and one
    that doesn't fit is returned in the second list, to go back in the
    data segment, unless some unit uses it as an `(name),Y` pointer
    (it's in `pointers`), which only works in zero page. The objects
    without an address also keep off `param_bytes`, where functions
    take `zeropage(addr)` parameters. Raises `LinkError` on an overlap
    or a full range."""
    from passes.runtime_layout import RuntimeLayout
    runtime = RuntimeLayout(
        ssp=layout["SSP"], fp=layout["FP"], hargs=layout["HARGS"],
        hargs_size=layout["HARGS size"], dptr=layout["DPTR"],
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
                    and b not in param_bytes
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
        if name in pointers:
            raise LinkError(
                f"no room for `{name}` ({size} bytes) below "
                f"${AUTO_HI:02X} in the linked program, and its unit "
                f"uses it as an `(zp),Y` pointer, which has to be in zero "
                f"page"
            )
        demoted.append(name)
    return addrs, demoted


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



def _drop_unused_locals(
    merged: LinkMetadata, objects: list[tuple[str, E.ElfFile]],
) -> None:
    """Leaves out of each function's local pool the slots no unit's
    code names, which the peepholes emptied after the pool was sized,
    so they don't take zero-page bytes. A function that takes a local's
    address (`#<` or `#>` of one of its slots) keeps its whole pool,
    since a pointer can reach the bytes after the one it names."""
    relocs = [r for _, obj in objects for s in obj.sections for r in s.relocs]
    named = {r.symbol.name for r in relocs}
    addressed = {
        r.symbol.name for r in relocs
        if r.type in (E.R_MOS_ADDR16_LO, E.R_MOS_ADDR16_HI)
    }
    for d in merged.defs:
        if not any(n in addressed for n in d.locals):
            d.locals = [n for n in d.locals if n in named]


def _merge_identical(
    objects: list[tuple[str, E.ElfFile]], metas: list[LinkMetadata],
) -> None:
    """Keeps one copy of the items marked `merge` (`ItemMeta.merge`)
    whose sections have the same contents, the first in the order of
    the units and their items, and points every other copy's
    references at it.

    Such an item is a plane of a table of row addresses
    (passes/optimization/row_address_tables.py), an object of the
    compiler's own that nothing writes and nothing takes the address
    of, so a program behaves the same whichever copy its lookups read.
    Its contents are
    its bytes and its relocations, and a relocation's symbol counts
    as the same in two units when it names the same thing: an
    undefined or global symbol by its name, which every unit resolves
    alike, and a local one only within its own unit. `_rename_locals`
    has run, so two units' `static` arrays of one name have different
    names here, and their tables stay apart.

    The kept copy's symbol becomes global. No other unit defines its
    name, since `_rename_locals` gave every internal item a name no
    other unit uses. Each other copy's section goes, and its symbol becomes
    an undefined reference to the kept one, so its unit's relocations,
    with their addends (`A@hi+1,X`), resolve to the kept copy."""
    kept: dict[tuple, E.Symbol] = {}
    for k, ((_, obj), meta) in enumerate(zip(objects, metas)):
        merged: set[str] = set()
        for it in meta.items:
            if not it.merge:
                continue
            sym = _item_symbol(obj, it.name)
            key = _contents(k, obj, sym)
            if key is None:
                continue
            first = kept.setdefault(key, sym)
            if first is sym:
                continue
            first.bind = E.STB_GLOBAL
            sec = sym.section
            obj.sections.remove(sec)
            sym.name = first.name
            sym.section = E.SHN_UNDEF
            sym.value = 0
            sym.size = 0
            sym.bind = E.STB_GLOBAL
            sym.type = E.STT_NOTYPE
            merged.add(it.name)
        if merged:
            meta.items = [it for it in meta.items if it.name not in merged]


def _contents(k: int, obj: E.ElfFile, sym: E.Symbol) -> tuple | None:
    """What the section of item `sym` of unit `k` holds, for
    `_merge_identical`, or None when another symbol is defined inside
    it, which a reference could name."""
    sec = sym.section
    if any(s.section is sec and s is not sym for s in obj.symbols):
        return None

    def target(s: E.Symbol):
        if s.defined and not s.is_global:
            return (k, s.name)
        return s.name

    relocs = tuple(sorted(
        (r.offset, r.type, target(r.symbol), r.addend) for r in sec.relocs
    ))
    return (sec.name, sec.type, sec.flags, sec.align, sec.size,
            bytes(sec.data), relocs)


def _link_shares(merged: LinkMetadata, metas: list[LinkMetadata]) -> list:
    """Every unit's shares joined into the program's, with each
    declaration's spelling of a slot replaced by the definition's, and
    checked against the program's call graph. Raises `LinkError` when a
    value a unit keeps in a callee's parameter bytes, on the strength of
    a `const` declaration, isn't safe there."""
    from passes.slot_sharing import check_linked_shares, merge_shares
    shares = [s for m in metas for s in m.shares]
    if not shares:
        return []
    defs = {d.name: d for d in merged.defs}
    canonical: dict[str, str] = {}
    for m in metas:
        for e in m.externs:
            d = defs.get(e.name)
            if d is not None:
                canonical.update(zip(e.params, d.params))
    try:
        out = merge_shares(shares, canonical)
        check_linked_shares(
            out, merged.calls, set(defs),
            {d.name: set(d.changes) for d in merged.defs},
        )
    except ValueError as e:
        raise LinkError(str(e)) from e
    return out


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
        for sym, canon, reg in zip(e.params, params, regs):
            if reg:
                # Passed in a register: no slot.
                continue
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

