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
; call <caller> -> <callee>
; @zp-link-meta-end
```

`def` lines describe in-TU function definitions (any function,
not just zp_abi). `ext` lines describe zp_abi externs (functions
declared `__attribute__((zp_abi))` but without a body in this
TU). `call` lines record every direct call edge from a defined
function to any other named function — including calls to
non-zp_abi externs (the linker uses these to detect when a TU's
function calls something that can't be globally bounded).
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
    code."""
    name: str
    params: list[str]
    locals: list[str]
    indirect: bool
    in_cycle: bool

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
    while preserving the strings."""
    name: str
    params: list[str]

    @property
    def param_bytes(self) -> int:
        return len(self.params)


@dataclass
class LinkMetadata:
    """Aggregate per-TU metadata. The linker reads one of these
    per `.asm` input file and merges them into a program-level
    view before re-running the allocators."""
    defs: list[FunctionMeta] = field(default_factory=list)
    externs: list[ExternMeta] = field(default_factory=list)
    calls: list[tuple[str, str]] = field(default_factory=list)


# ---------------------------------------------------------------------------
# Build.
# ---------------------------------------------------------------------------


def build_metadata(
    prog: tac_ast.Program,
    abi: dict[str, ParamLayout],
    local_pools: dict[str, list[int]],
    *,
    slot_names_by_fn: dict[str, list[str]] | None = None,
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
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        for instr in tl.instructions:
            if isinstance(instr, tac_ast.IndirectCall):
                indirect_per_fn[tl.name] = True
            elif isinstance(instr, tac_ast.FunctionCall):
                callgraph[tl.name].add(instr.name)
    in_cycle = _cycle_members_in(callgraph)

    out = LinkMetadata()
    # Per-defined-function entries.
    params_for: dict[str, list[str]] = {
        n: list(layout.slot_symbols)
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
    out.calls.sort()
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
    stage prepends these alongside the EQU directives."""
    lines = [_BEGIN]
    for d in meta.defs:
        lines.append(
            f"; def {d.name} "
            f"params={','.join(d.params)} "
            f"locals={','.join(d.locals)} "
            f"indirect={'true' if d.indirect else 'false'} "
            f"in_cycle={'true' if d.in_cycle else 'false'}"
        )
    for e in meta.externs:
        lines.append(
            f"; ext {e.name} params={','.join(e.params)}"
        )
    for caller, callee in meta.calls:
        lines.append(f"; call {caller} -> {callee}")
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
        ))
    elif kind == "ext":
        if len(parts) < 3:
            raise ValueError(f"malformed ext record: {record!r}")
        name = parts[1]
        kvs = dict(_split_kv(p) for p in parts[2:])
        out.externs.append(ExternMeta(
            name=name, params=_split_csv(kvs["params"]),
        ))
    elif kind == "call":
        # Format: `call <caller> -> <callee>`
        if len(parts) != 4 or parts[2] != "->":
            raise ValueError(f"malformed call record: {record!r}")
        out.calls.append((parts[1], parts[3]))
    else:
        raise ValueError(f"unknown record kind {kind!r}: {record!r}")


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
