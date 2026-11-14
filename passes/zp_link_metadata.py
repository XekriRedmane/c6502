"""Build and parse the `@zp-link-meta` block emitted alongside the
EQU directives at the top of every optimized `.asm` file.

The block carries the program-level information `compile.py
--link` needs to globally re-allocate `__zpabi_*` and
`__local_*` symbols across multiple TUs:

  * Which zp_abi functions are **defined** in this TU and how
    many parameter bytes each takes.
  * Which zp_abi functions are declared `extern` in this TU
    (their definition lives elsewhere) and their parameter
    byte counts.
  * Which **non-zp_abi** functions are defined in this TU,
    with the size of their private body-local pool — the
    number of `__local_<fn>_b<k>` slot symbols emitted by
    `apply_coloring`. Functions whose body uses numeric ZP
    addresses (ineligible per `zp_local_allocation`) report
    a local count of `0` here; their numeric addresses are
    immutable at link time.
  * The static **call graph** — every direct caller→callee
    edge, both in-TU and to externs.
  * Per-function flags the linker needs to recompute global
    eligibility: whether the body contains an `IndirectCall`,
    and whether the function appeared on a cycle in the TU's
    direct call graph (the linker re-detects cycles globally
    but a TU-level cycle is a sufficient disqualifier).
  * Every function and object the TU defines, an **item**, with the
    section it goes in (`passes/sections.py`), whether its name is
    internal to the TU, and whether it's all zero bytes, for a link
    with a script (`compile.py --link -T`).
  * The TU's **zero-page objects** (`passes/zp_globals.py`): each
    one's size, its fixed address if it was declared with one,
    whether this TU defines it, and its initial bytes, so the
    linker can place them for the whole program and initialize
    them.

The block is written as a sequence of comment lines bracketed
by `@zp-link-meta-begin` / `@zp-link-meta-end`. Dasm ignores
comments, so the asm assembles unchanged. The parser is liberal
about whitespace.

Format
------

```
; @zp-link-meta-begin
; def <fn> param_bytes=<N> local_bytes=<M> indirect=<bool> in_cycle=<bool>
; ext <fn> param_bytes=<N>
; call <caller> -> <callee> [saved]
; item <name> section=<name> local=<bool> zero=<bool>
; zp <name> size=<N> at=<$XX|auto> def=<bool> init=<byte,...> [weight=<N>] [local=true]
; runtime hargs_size=<N> [hargs_need=<N>]
; @zp-link-meta-end
```

`def` lines describe in-TU function definitions (any function,
not just zp_abi). `ext` lines describe zp_abi externs (functions
declared `[[c6502::zp_abi]]` but without a body in this
TU). `call` lines record every direct call edge from a defined
function to any other named function — including calls to
non-zp_abi externs (the linker uses these to detect when a TU's
function calls something that can't be globally bounded). `saved`
marks an edge whose every call is under `#pragma c6502 call
save(hardware_stack)`, so the linker can take a cycle of such calls
as the compile did (`passes/recursion_saves.py`). `zp`
lines describe zero-page objects; each `init` byte is written as the
operand of an `LDA #` (`$05`, `<buf`, `>(buf+2)`), and `weight`
marks an object `--optimize` promoted, which the linker may put back
in the data segment. The `runtime`
line gives the size of the runtime's HARGS block, which the EQU
lines don't show, and how many of its bytes the TU's code uses
(`passes/runtime_layout.py`), so the linker can choose a layout for
the program. A `share` line lists slots that hold one value for
several functions, and the functions that keep a local there
(`passes/slot_sharing.py`), as `member=<fn>:<slot>,<slot>` and
`holder=<fn>` tokens, then what the linker checks the share by:
`live=<fn>:<callee>,<callee>` for the calls a function's value there
is live across, `forward=<caller>><owner>` for a caller whose every
call to the owner passes its own value there, and `writer=<fn>` for a
holder that assigns its value. A `def` or `ext` line's `pinned` list
gives the address of each byte of a `zeropage(addr)` parameter, with
an empty entry for the others, and a `def` line's `changes` list the
slots of the parameters the function assigns.
"""
from __future__ import annotations

from dataclasses import dataclass, field

import tac_ast
from passes.abi_selection import ParamLayout, ZpLayout


@dataclass
class FunctionMeta:
    """Per-defined-function metadata. `params` is the flat
    per-byte list of zp_abi parameter slot symbols (empty for
    non-zp_abi functions); `locals` is the per-pool-byte list of
    body-local slot symbols (empty for functions without a private
    pool). Slot count is `len(...)` of each list, so `param_bytes`
    and `local_bytes` are derived properties for backward-readable
    code.

    `param_regs` is a parallel list to `params`: each entry is "",
    "A", or "X" — the register the corresponding param byte is passed
    in at the calling convention, or "" if the byte rides the default
    zp_abi ZP-slot path. Produced from the function's
    `ZpLayout.param_registers` so a cross-TU caller can synthesize the
    right caller-side register-passing for a `register`-param extern.
    (The return value's register placement isn't serialized — it's
    type-driven and recomputed identically on both sides.)"""
    name: str
    params: list[str]
    locals: list[str]
    indirect: bool
    in_cycle: bool
    param_regs: list[str] = field(default_factory=list)
    pinned: list[int | None] = field(default_factory=list)
    changes: list[str] = field(default_factory=list)

    @property
    def param_bytes(self) -> int:
        return len(self.params)

    @property
    def local_bytes(self) -> int:
        return len(self.locals)


@dataclass
class ExternMeta:
    """Per-declared-extern metadata (zp_abi externs only). `params`
    carries the flat per-byte slot symbols the caller TU references
    at call sites — the linker re-binds them to fresh ZP addresses
    while preserving the strings. `param_regs` mirrors FunctionMeta's
    register-passing layout."""
    name: str
    params: list[str]
    param_regs: list[str] = field(default_factory=list)
    pinned: list[int | None] = field(default_factory=list)

    @property
    def param_bytes(self) -> int:
        return len(self.params)


@dataclass
class ItemMeta:
    """A function or object the TU defines. `section` is the section
    it goes in, `local` is whether its name has internal linkage (a
    `static`, or a string literal), so another TU's item may have the
    same name, and `zero` is whether it's an object whose bytes are
    all zero, laid down as `DS.B`."""
    name: str
    section: str
    local: bool = False
    zero: bool = False


@dataclass
class ZpObjectMeta:
    """A zero-page object of the TU. `at` is its address when it was
    declared with one (`zeropage(addr)`), else None, and the linker
    places it. `defined` is whether this TU defines it, and `init` is
    its initial bytes then, each written as an `LDA #` operand.
    `weight` is set when `--optimize` promoted it into zero page
    (`zp_globals.access_weights`), and None when the program asked.
    `local` is whether its name has internal linkage, so another TU
    may define one of the same name."""
    name: str
    size: int
    at: int | None = None
    defined: bool = False
    init: list[str] = field(default_factory=list)
    weight: int | None = None
    local: bool = False


@dataclass
class LinkMetadata:
    """Aggregate per-TU metadata. The linker reads one of these
    per `.asm` input file and merges them into a program-level
    view before re-running the allocators."""
    defs: list[FunctionMeta] = field(default_factory=list)
    externs: list[ExternMeta] = field(default_factory=list)
    calls: list[tuple[str, str]] = field(default_factory=list)
    # The edges in `calls` whose every call saves the caller's values
    # on the hardware stack.
    saved_calls: list[tuple[str, str]] = field(default_factory=list)
    zp_objects: list[ZpObjectMeta] = field(default_factory=list)
    hargs_size: int | None = None
    hargs_need: int | None = None
    items: list[ItemMeta] = field(default_factory=list)
    shares: list = field(default_factory=list)


# ---------------------------------------------------------------------------
# Build.
# ---------------------------------------------------------------------------


def build_metadata(
    prog: tac_ast.Program,
    abi: dict[str, ParamLayout],
    local_pools: dict[str, list[int]],
    *,
    slot_names_by_fn: dict[str, list[str]] | None = None,
    changes: dict[str, list[str]] | None = None,
) -> LinkMetadata:
    """Collect link metadata from the TAC program + per-function
    allocator outputs. Run after `select_abi`,
    `allocate_zp_slots`, and `allocate_function_locals`.

    `slot_names_by_fn`, when supplied, provides the per-function
    body-local slot symbols (one per pool byte, in pool order) the
    asm-SSA regalloc + naming logic produced. The linker reads
    these strings from metadata and reuses them verbatim when
    minting the global EQU block. When absent (e.g. callers that
    don't run the optimizer), we fall back to numeric temp names."""
    in_tu_names = {
        tl.name for tl in prog.top_level
        if isinstance(tl, tac_ast.Function)
    }
    # Cycle members in the in-TU direct call graph. The linker
    # re-detects cycles across the merged graph, but a TU-local
    # cycle is itself enough to disqualify the function.
    callgraph: dict[str, set[str]] = {n: set() for n in in_tu_names}
    indirect_per_fn: dict[str, bool] = {n: False for n in in_tu_names}
    # Whether every call along each edge is marked to save on the
    # hardware stack.
    saved: dict[tuple[str, str], bool] = {}
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        for instr in tl.instructions:
            if isinstance(instr, tac_ast.IndirectCall):
                indirect_per_fn[tl.name] = True
            elif isinstance(instr, tac_ast.FunctionCall):
                callgraph[tl.name].add(instr.name)
                edge = (tl.name, instr.name)
                saved[edge] = saved.get(edge, True) and instr.hw_stack_save
    in_cycle = _cycle_members_in(callgraph)

    out = LinkMetadata()
    # Per-defined-function entries.
    params_for: dict[str, list[str]] = {
        n: list(layout.slot_symbols)
        for n, layout in abi.items()
        if isinstance(layout, ZpLayout)
    }
    param_regs_for: dict[str, list[str]] = {
        n: ["" if r is None else r for r in layout.param_registers]
        for n, layout in abi.items()
        if isinstance(layout, ZpLayout)
    }
    for name in sorted(in_tu_names):
        locals_list: list[str]
        if slot_names_by_fn is not None and name in slot_names_by_fn:
            locals_list = list(slot_names_by_fn[name])
        else:
            locals_list = [
                f"__local_{name}__{k}"
                for k in range(len(local_pools.get(name, ())))
            ]
        out.defs.append(FunctionMeta(
            name=name,
            params=params_for.get(name, []),
            locals=locals_list,
            indirect=indirect_per_fn.get(name, False),
            in_cycle=name in in_cycle,
            param_regs=param_regs_for.get(name, []),
            pinned=list(getattr(abi.get(name), "pinned", [])),
            changes=list((changes or {}).get(name, [])),
        ))
    # zp_abi extern declarations: ZpLayout entries in `abi` whose
    # name isn't defined in this TU.
    for name in sorted(abi):
        if name in in_tu_names:
            continue
        layout = abi[name]
        if isinstance(layout, ZpLayout):
            out.externs.append(ExternMeta(
                name=name,
                params=list(layout.slot_symbols),
                param_regs=param_regs_for.get(name, []),
                pinned=list(layout.pinned),
            ))
    # Call edges. Sorted for deterministic output.
    seen_calls: set[tuple[str, str]] = set()
    for caller, callees in callgraph.items():
        for callee in callees:
            edge = (caller, callee)
            if edge in seen_calls:
                continue
            seen_calls.add(edge)
            out.calls.append(edge)
            if saved.get(edge):
                out.saved_calls.append(edge)
    out.calls.sort()
    out.saved_calls.sort()
    return out


def _cycle_members_in(
    callgraph: dict[str, set[str]],
) -> set[str]:
    """Tarjan SCC + self-loop detection. Returns the set of
    nodes participating in a cycle (any non-trivial SCC plus
    singleton SCCs with self-edges)."""
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
            if w not in callgraph:
                continue  # extern; no edge in the SCC graph
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
                only = component[0]
                if only in callgraph.get(only, set()):
                    in_cycle.add(only)

    for n in callgraph:
        if n not in index:
            strongconnect(n)
    return in_cycle


# ---------------------------------------------------------------------------
# Format / parse.
# ---------------------------------------------------------------------------


_BEGIN = "; @zp-link-meta-begin"
_END = "; @zp-link-meta-end"


def format_metadata(meta: LinkMetadata) -> list[str]:
    """Return the metadata block as a list of asm-output lines
    (each prefixed with `; ` so dasm ignores them). The emit
    stage prepends these alongside the EQU directives.

    `param_regs` and `return_reg` are emitted only when at least
    one entry is non-default, keeping the metadata block backward-
    compatible with TUs that pre-date the reg attribute (the
    parser tolerates missing fields)."""
    lines = [_BEGIN]
    for d in meta.defs:
        parts = [
            f"; def {d.name}",
            f"params={','.join(d.params)}",
            f"locals={','.join(d.locals)}",
            f"indirect={'true' if d.indirect else 'false'}",
            f"in_cycle={'true' if d.in_cycle else 'false'}",
        ]
        if d.param_regs and any(d.param_regs):
            parts.append(f"param_regs={','.join(d.param_regs)}")
        if any(a is not None for a in d.pinned):
            parts.append(f"pinned={_format_pinned(d.pinned)}")
        if d.changes:
            parts.append(f"changes={','.join(d.changes)}")
        lines.append(" ".join(parts))
    for e in meta.externs:
        parts = [
            f"; ext {e.name}",
            f"params={','.join(e.params)}",
        ]
        if e.param_regs and any(e.param_regs):
            parts.append(f"param_regs={','.join(e.param_regs)}")
        if any(a is not None for a in e.pinned):
            parts.append(f"pinned={_format_pinned(e.pinned)}")
        lines.append(" ".join(parts))
    saved_calls = set(meta.saved_calls)
    for caller, callee in meta.calls:
        mark = " saved" if (caller, callee) in saved_calls else ""
        lines.append(f"; call {caller} -> {callee}{mark}")
    for share in meta.shares:
        tokens = [f"member={f}:{','.join(syms)}" for f, syms in share.members]
        tokens += [f"holder={h}" for h in share.holders]
        tokens += [f"live={f}:{','.join(cs)}" for f, cs in share.live]
        tokens += [f"forward={a}>{b}" for a, b in share.forwards]
        tokens += [f"writer={w}" for w in share.writers]
        lines.append(f"; share {' '.join(tokens)}")
    for it in meta.items:
        lines.append(
            f"; item {it.name} section={it.section} "
            f"local={'true' if it.local else 'false'} "
            f"zero={'true' if it.zero else 'false'}"
        )
    for z in meta.zp_objects:
        at = "auto" if z.at is None else f"${z.at:02X}"
        weight = "" if z.weight is None else f" weight={z.weight}"
        local = " local=true" if z.local else ""
        lines.append(
            f"; zp {z.name} size={z.size} at={at} "
            f"def={'true' if z.defined else 'false'} "
            f"init={','.join(z.init)}{weight}{local}"
        )
    if meta.hargs_size is not None:
        need = ("" if meta.hargs_need is None
                else f" hargs_need={meta.hargs_need}")
        lines.append(f"; runtime hargs_size={meta.hargs_size}{need}")
    lines.append(_END)
    return lines


def parse_metadata(asm_text: str) -> LinkMetadata:
    """Extract the metadata block from a `.asm` file's text.
    Returns a fresh `LinkMetadata` (empty if no block is
    present). Raises `ValueError` on a malformed block."""
    out = LinkMetadata()
    in_block = False
    for raw in asm_text.splitlines():
        line = raw.strip()
        if line == _BEGIN:
            if in_block:
                raise ValueError(
                    "nested @zp-link-meta-begin without "
                    "preceding @zp-link-meta-end",
                )
            in_block = True
            continue
        if line == _END:
            if not in_block:
                raise ValueError(
                    "@zp-link-meta-end without preceding "
                    "@zp-link-meta-begin",
                )
            in_block = False
            continue
        if not in_block:
            continue
        # Inside the block: line should start with "; " followed
        # by the record kind.
        if not line.startswith("; "):
            raise ValueError(
                f"unexpected line inside zp-link-meta block: {line!r}",
            )
        record = line[2:].strip()
        _parse_record(record, out)
    if in_block:
        raise ValueError(
            "@zp-link-meta-begin without matching @zp-link-meta-end",
        )
    return out


def _parse_record(record: str, out: LinkMetadata) -> None:
    parts = record.split()
    if not parts:
        return
    kind = parts[0]
    if kind == "def":
        if len(parts) < 6:
            raise ValueError(f"malformed def record: {record!r}")
        name = parts[1]
        kvs = dict(_split_kv(p) for p in parts[2:])
        out.defs.append(FunctionMeta(
            name=name,
            params=_split_csv(kvs["params"]),
            locals=_split_csv(kvs["locals"]),
            indirect=kvs["indirect"] == "true",
            in_cycle=kvs["in_cycle"] == "true",
            param_regs=_split_csv(kvs.get("param_regs", "")),
            pinned=_parse_pinned(kvs.get("pinned", "")),
            changes=_split_csv(kvs.get("changes", "")),
        ))
    elif kind == "ext":
        if len(parts) < 3:
            raise ValueError(f"malformed ext record: {record!r}")
        name = parts[1]
        kvs = dict(_split_kv(p) for p in parts[2:])
        out.externs.append(ExternMeta(
            name=name,
            params=_split_csv(kvs["params"]),
            param_regs=_split_csv(kvs.get("param_regs", "")),
            pinned=_parse_pinned(kvs.get("pinned", "")),
        ))
    elif kind == "share":
        from passes.slot_sharing import SlotShare
        members, holders, live, forwards, writers = [], [], [], [], []
        for token in parts[1:]:
            key, value = _split_kv(token)
            if key == "member":
                fn, _, syms = value.partition(":")
                members.append((fn, tuple(_split_csv(syms))))
            elif key == "holder":
                holders.append(value)
            elif key == "live":
                fn, _, callees = value.partition(":")
                live.append((fn, tuple(_split_csv(callees))))
            elif key == "forward":
                caller, _, owner = value.partition(">")
                forwards.append((caller, owner))
            elif key == "writer":
                writers.append(value)
            else:
                raise ValueError(f"malformed share record: {record!r}")
        if not members:
            raise ValueError(f"malformed share record: {record!r}")
        out.shares.append(SlotShare(
            members=tuple(members), holders=tuple(holders),
            live=tuple(live), forwards=tuple(forwards),
            writers=tuple(writers),
        ))
    elif kind == "zp":
        if len(parts) < 3:
            raise ValueError(f"malformed zp record: {record!r}")
        kvs = dict(_split_kv(p) for p in parts[2:])
        at = kvs["at"]
        out.zp_objects.append(ZpObjectMeta(
            name=parts[1],
            size=int(kvs["size"]),
            at=None if at == "auto" else int(at.lstrip("$"), 16),
            defined=kvs["def"] == "true",
            init=_split_csv(kvs.get("init", "")),
            weight=int(kvs["weight"]) if "weight" in kvs else None,
            local=kvs.get("local") == "true",
        ))
    elif kind == "item":
        if len(parts) < 3:
            raise ValueError(f"malformed item record: {record!r}")
        kvs = dict(_split_kv(p) for p in parts[2:])
        out.items.append(ItemMeta(
            name=parts[1], section=kvs["section"],
            local=kvs.get("local") == "true",
            zero=kvs.get("zero") == "true",
        ))
    elif kind == "runtime":
        kvs = dict(_split_kv(p) for p in parts[1:])
        out.hargs_size = int(kvs["hargs_size"])
        if "hargs_need" in kvs:
            out.hargs_need = int(kvs["hargs_need"])
    elif kind == "call":
        # Format: `call <caller> -> <callee> [saved]`
        if (len(parts) not in (4, 5) or parts[2] != "->"
                or parts[4:] not in ([], ["saved"])):
            raise ValueError(f"malformed call record: {record!r}")
        out.calls.append((parts[1], parts[3]))
        if parts[4:]:
            out.saved_calls.append((parts[1], parts[3]))
    else:
        raise ValueError(f"unknown record kind {kind!r}: {record!r}")


def _format_pinned(pinned: list[int | None]) -> str:
    return ",".join("" if a is None else f"${a:02X}" for a in pinned)


def _parse_pinned(value: str) -> list[int | None]:
    if not value:
        return []
    return [
        int(v.lstrip("$"), 16) if v else None for v in value.split(",")
    ]


def _split_kv(token: str) -> tuple[str, str]:
    if "=" not in token:
        raise ValueError(f"expected key=value, got {token!r}")
    k, v = token.split("=", 1)
    return k, v


def _split_csv(value: str) -> list[str]:
    """Comma-separated value list. Empty string -> empty list."""
    if not value:
        return []
    return value.split(",")
