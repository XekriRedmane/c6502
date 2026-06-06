"""ABI selection / validation for the per-function ZP-passing
calling convention.

Given a TAC program (for body inspection) and the c99 AST (for the
`abi_annotation` field on each `FunctionDecl` / `VarDecl`), produce
a `dict[str, ParamLayout]` mapping each function name to its calling
convention. The ABI is **driven by the programmer's annotation** —
without `__attribute__((zp_abi))` a function gets the soft-stack
ABI; with the annotation, the function gets ZP-passing after
validation.

Functions defined in this TU and zp_abi-annotated externs both
land in the returned dict. The extern case is the canonical
cross-TU path: a header declares
`__attribute__((zp_abi)) extern T fn(...)`, every TU that
includes it sees the annotation, and call sites in this TU use
ZP-passing for `fn` even though its body isn't visible.

Validation of a `zp_abi` function (rejected with a clear error if
any check fails):

  1. **No indirect calls.** The TAC body must contain zero
     `IndirectCall` instructions — the callee's ABI can't be
     known at an indirect call site, so the callee might
     reenter `fn` and clobber its parameter ZP slots.
  2. **Not on a call-graph cycle.** Direct `FunctionCall`s
     inside the body are fine, BUT `fn` must not be reachable
     from itself via the static direct-call graph. A recursive
     call (direct or transitive) would overwrite the outer
     activation's parameter ZP slots before it returned.
     Non-recursive calls are safe: the optimizer's regalloc
     already blocks every zp_abi callee's parameter ZP slots
     from being used as the caller's locals (see
     `passes/optimization_asm/optimizer.py::_blocked_addrs_for`),
     so the callee can't alias the caller's params.
  3. **Address not taken.** No `Var(name=fn)` appears anywhere
     in the program. If the function's address is taken, an
     indirect call site can't know its custom ABI; only the
     soft-stack convention can be assumed at indirect call
     sites.
  4. **Params fit.** The total parameter byte count fits in
     the configured ZP window (`pool.zp_param_window()` —
     defaults to the caller-saved range $80–$BF).

The returned dict has an entry for every `Function` top-level in
`prog`, plus one entry per zp_abi-annotated extern. Functions
defined in this TU without an annotation get `SoftStackLayout`;
unannotated externs are absent from the dict, and the caller-
side lookup in `tac_to_asm` falls back to `SoftStackLayout` for
any missing name.

`select_abi` is the entry point. The intermediate helper
`_address_taken` walks the program once to compute the
address-taken set — used by the per-function validation step.
"""
from __future__ import annotations

import dataclasses
from dataclasses import dataclass, field

import c99_ast
import tac_ast
from passes.optimization.pool import Pool
from passes.optimization.var_visit import vals_in
from passes.zp_slot_naming import param_slot_symbols


class AbiSelectionError(Exception):
    """Raised when a `__attribute__((zp_abi))` annotation can't be
    honored: the function makes an indirect call, sits on a cycle
    in the direct call graph (recursion), has its address taken,
    or has too many parameter bytes."""


# ---------------------------------------------------------------------------
# ParamLayout: how a function's parameters are passed.
# ---------------------------------------------------------------------------


@dataclass
class SoftStackLayout:
    """The existing convention: each parameter byte sits at
    `Frame(M + 3 + j_offset + k)` on the callee's frame. Caller
    pushes args via `AllocateStack` + `Stack(off)` writes."""


@dataclass
class ZpLayout:
    """The ZP-passing convention: each parameter byte lives at a
    fixed address resolved at the asm-emit stage. `slot_symbols[i]`
    names the i-th parameter byte's slot (low byte of param 0 first,
    then high byte of param 0, then low byte of param 1, etc.).
    `addrs[i]` is the concrete numeric address that each symbol
    will resolve to at assembly time — populated by
    `passes.zp_slot_allocation` after the call graph is known, so
    no two functions on a common call path share a slot.

    The asm emit produces `<symbol> EQU $<addr>` directives at the
    top of the output and uses `Data(slot_symbols[i])` for every
    caller-side arg-write and callee-side param-read. dasm picks
    zp vs. absolute addressing automatically from each symbol's
    resolved value — so when ZP saturates and the allocator spills
    a function's slots above `$FF`, no code changes are needed.

    `addrs` is what the asm-level regalloc reads via
    `_blocked_addrs_for` to keep body locals disjoint from the
    function's own param storage. Both fields are kept in sync.

    `param_registers` is a parallel list to `slot_symbols`; each
    entry is None (the byte arrives via the ZP slot) or "A"/"X"
    (the byte arrives in that 6502 register at the call boundary).
    Registers are assigned POSITIONALLY: the first register arg-byte
    goes in A, the second in X — so a 1-byte `register` param gets
    one A/X entry, and a 2-byte `register` param spreads its low byte
    to A and high byte to X. The whole arg list has at most two
    register bytes (A, X). When a byte's `param_registers[i]` is set,
    the slot symbol still exists and gets a ZP byte — the callee body
    reads it like any other zp_abi param byte — but the caller does
    NOT write to it: instead the caller loads the byte into the
    register before `JSR`, and the callee's entry stub stores the
    register into the slot.

    The return value's register placement is NOT recorded here — it's
    type-driven and computed identically on both sides in tac_to_asm
    (a non-pointer return of <=2 bytes returns low byte in A, high
    byte in X)."""
    slot_symbols: list[str] = field(default_factory=list)
    addrs: list[int] = field(default_factory=list)
    param_registers: list[str | None] = field(default_factory=list)


ParamLayout = SoftStackLayout | ZpLayout


# ---------------------------------------------------------------------------
# Pool helper: which ZP addresses can be used for parameter passing.
# ---------------------------------------------------------------------------


def _zp_param_window(pool: Pool) -> range:
    """The ZP window available for parameter passing. Today this
    overlaps with the caller-saved range — the `Pool` already
    partitions $80–$BF as caller-saved by default. Extracted here
    as a helper so a future change can split the param window
    from the body's caller-saved scratch pool."""
    return pool.caller_saved()


# ---------------------------------------------------------------------------
# Address-taken set computation.
# ---------------------------------------------------------------------------


def _address_taken(prog: tac_ast.Program) -> set[str]:
    """Set of function names that appear as a `Var(name=fn)` in
    any instruction's operand position. Direct calls
    (`FunctionCall.name`) and indirect-call pointers
    (`IndirectCall.ptr` — itself a Var holding a previously-
    GetAddress'd value) don't count: the address-taken site is
    the GetAddress that produced the pointer, caught here via the
    `vals_in` walk."""
    function_names: set[str] = {
        tl.name for tl in prog.top_level
        if isinstance(tl, tac_ast.Function)
    }
    if not function_names:
        return set()
    taken: set[str] = set()
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        for instr in tl.instructions:
            for v in vals_in(instr):
                if (
                    isinstance(v, tac_ast.Var)
                    and v.name in function_names
                    and v.name != tl.name
                ):
                    taken.add(v.name)
    return taken


# ---------------------------------------------------------------------------
# Per-function param byte size.
# ---------------------------------------------------------------------------


def _param_byte_count(fn_decl: c99_ast.Type_function_decl, types) -> int:
    """Total bytes occupied by the function's parameters. Reads
    each parameter's type from the function's `data_type` (which
    is a `FunType`) and sums via `replace_pseudoregisters.sizeof`.
    The `types` table is needed for struct/union sizing."""
    from passes.replace_pseudoregisters import sizeof
    fun_type = fn_decl.data_type
    if not isinstance(fun_type, c99_ast.FunType):
        # Defensive — should never happen for a FunctionDecl.
        return 0
    return sum(sizeof(p, types) for p in fun_type.params)


def _per_param_byte_sizes(
    fn_decl: c99_ast.Type_function_decl, types,
) -> list[int]:
    """Byte size per parameter, in declaration order — the parallel
    list to `fn_decl.params` (which holds resolved names). Used by
    the zp_abi slot-symbol minter to derive per-byte symbol names."""
    from passes.replace_pseudoregisters import sizeof
    fun_type = fn_decl.data_type
    if not isinstance(fun_type, c99_ast.FunType):
        return []
    return [sizeof(p, types) for p in fun_type.params]


# The arg-passing registers, in positional order: the first register
# arg-byte goes in A, the second in X. (Y is reserved for `register`
# locals; the return value reuses A/X, computed separately.)
_ARG_REGISTERS = ("A", "X")


def _strip_quals(t):
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return t


def _assign_param_registers(
    fn_decl: c99_ast.Type_function_decl,
    per_param_bytes: list[int],
    where: str,
) -> list[str | None]:
    """Assign 6502 registers to the bytes of `register`-marked
    parameters, positionally: the first register arg-byte goes in A,
    the second in X. So a 1-byte `register` param gets one entry, and
    a 2-byte `register` param spreads its low byte to A and high byte
    to X.

    Returns a per-byte list parallel to the function's flattened param
    bytes (the `slot_symbols` order — low byte of param 0 first), each
    entry "A" / "X" or None.

    Raises `AbiSelectionError` if a `register` param is a pointer, is
    wider than 2 bytes, or the marked params together need more than
    the two available arg registers (A, X). `where` is a short label
    for error messages (e.g. the function name)."""
    fun_type = fn_decl.data_type
    markers = list(fn_decl.param_registers)
    while len(markers) < len(per_param_bytes):
        markers.append(0)
    per_byte: list[str | None] = []
    next_reg = 0
    for i, (n_bytes, marker) in enumerate(zip(per_param_bytes, markers)):
        if not marker:
            per_byte.extend([None] * n_bytes)
            continue
        param_t = (
            _strip_quals(fun_type.params[i])
            if isinstance(fun_type, c99_ast.FunType) else None
        )
        pname = fn_decl.params[i] if i < len(fn_decl.params) else f"#{i}"
        if isinstance(param_t, c99_ast.Pointer):
            raise AbiSelectionError(
                f"function `{where}` parameter `{pname}` is a pointer "
                f"and can't be passed in a register"
            )
        if n_bytes > 2:
            raise AbiSelectionError(
                f"function `{where}` parameter `{pname}` is {n_bytes} "
                f"bytes — a `register` parameter must fit in at most 2 "
                f"bytes (A, X)"
            )
        for _k in range(n_bytes):
            if next_reg >= len(_ARG_REGISTERS):
                raise AbiSelectionError(
                    f"function `{where}`: too many `register` argument "
                    f"bytes — at most two are available (A, X)"
                )
            per_byte.append(_ARG_REGISTERS[next_reg])
            next_reg += 1
    return per_byte


def _check_forward_def_match(c99_prog: c99_ast.Program) -> None:
    """Across multiple declarations / definitions of the same
    function name, every forward decl + definition must agree on
    `param_registers` (the per-param `register` markers). Mismatch is
    a hard error (an `extern T fn(...)` in a header and the body
    `T fn(...) { ... }` in the source must declare the same ABI).
    Raises `AbiSelectionError` on mismatch."""
    seen: dict[str, c99_ast.Type_function_decl] = {}
    for d in c99_prog.declaration:
        if not isinstance(d, c99_ast.FunctionDecl):
            continue
        fn = d.function_decl
        prior = seen.get(fn.name)
        if prior is None:
            seen[fn.name] = fn
            continue
        # `register` markers are 0/1 ints parallel to `params`. Pad to
        # the longer list's length so an empty `param_registers=[]`
        # (older AST construction sites) compares cleanly with a
        # parser-built decl.
        a = [bool(r) for r in prior.param_registers]
        b = [bool(r) for r in fn.param_registers]
        if len(a) < len(b):
            a = a + [False] * (len(b) - len(a))
        if len(b) < len(a):
            b = b + [False] * (len(a) - len(b))
        if a != b:
            raise AbiSelectionError(
                f"function `{fn.name}`: `register` parameter markers "
                f"differ between declarations — {a!r} vs {b!r}; the "
                f"calling convention must match"
            )
        # Adopt whichever had a body (or just keep prior).
        if fn.body is not None:
            seen[fn.name] = fn


# ---------------------------------------------------------------------------
# Annotation extraction.
# ---------------------------------------------------------------------------


def _annotation_map(
    c99_prog: c99_ast.Program,
) -> dict[str, str | None]:
    """`name → abi_annotation` for every `FunctionDecl` in the c99
    program. If multiple declarations / definitions of the same
    function name exist, the merged annotation is the first
    non-None one encountered (annotations on declaration AND
    definition agree by definition; one annotated and one not is
    fine — the annotated value wins)."""
    out: dict[str, str | None] = {}
    for d in c99_prog.declaration:
        if not isinstance(d, c99_ast.FunctionDecl):
            continue
        fn = d.function_decl
        existing = out.get(fn.name)
        if existing is None and fn.abi_annotation is not None:
            out[fn.name] = fn.abi_annotation
        else:
            out.setdefault(fn.name, existing)
    return out


def _function_decls_by_name(
    c99_prog: c99_ast.Program,
) -> dict[str, c99_ast.Type_function_decl]:
    """Return one `Type_function_decl` per function name. When
    multiple declarations exist (e.g. forward decl + definition),
    prefer the one with a body — `data_type` and `params` should
    agree across them. Used for parameter-byte-size lookup."""
    out: dict[str, c99_ast.Type_function_decl] = {}
    for d in c99_prog.declaration:
        if not isinstance(d, c99_ast.FunctionDecl):
            continue
        fn = d.function_decl
        if fn.name not in out or fn.body is not None:
            out[fn.name] = fn
    return out


# ---------------------------------------------------------------------------
# Per-function validation.
# ---------------------------------------------------------------------------


def _has_indirect_call(fn: tac_ast.Function) -> bool:
    return any(isinstance(i, tac_ast.IndirectCall) for i in fn.instructions)


def _build_callgraph(prog: tac_ast.Program) -> dict[str, set[str]]:
    """`name -> set of direct callees`. Edges come from
    `FunctionCall.name` only; `IndirectCall` is reported per-
    function via `_has_indirect_call`. Only edges into functions
    defined in this program are recorded — calls to externs (no
    matching `tac_ast.Function`) can't introduce a cycle that
    reenters the current TU."""
    fn_names = {
        tl.name for tl in prog.top_level
        if isinstance(tl, tac_ast.Function)
    }
    cg: dict[str, set[str]] = {name: set() for name in fn_names}
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


def _on_cycle(name: str, callgraph: dict[str, set[str]]) -> bool:
    """True iff `name` is reachable from itself in the direct-call
    graph (i.e. participates in a cycle — direct or mutual
    recursion). Iterative DFS starting from each direct callee of
    `name`."""
    targets = callgraph.get(name, set())
    if not targets:
        return False
    visited: set[str] = set()
    stack = list(targets)
    while stack:
        n = stack.pop()
        if n == name:
            return True
        if n in visited:
            continue
        visited.add(n)
        stack.extend(callgraph.get(n, ()))
    return False


def _validate_zp_abi(
    fn: tac_ast.Function,
    fn_decl: c99_ast.Type_function_decl,
    address_taken: set[str],
    callgraph: dict[str, set[str]],
    pool: Pool,
    types,
) -> ZpLayout:
    """Validate that `fn` can be given the ZP-passing ABI; return
    the computed `ZpLayout`. Raises `AbiSelectionError` on any
    violation."""
    # Struct/union return: c99_to_tac prepends a hidden sret-pointer
    # param to the TAC param list (see `c99_to_tac.py` near
    # `make_temporary_variable_name` in the FunctionCall arm). The
    # FunType's `params` list doesn't include this hidden param, so
    # `_param_byte_count` undercounts and the call site's arg
    # iteration runs off the end of the ZpLayout's slot_symbols.
    # The cleanest accommodation is to reject struct/union returns
    # outright — the unannotated default-zp_abi path falls back to
    # soft-stack, which already handles sret correctly.
    fun_type = fn_decl.data_type
    if isinstance(fun_type, c99_ast.FunType) and isinstance(
        fun_type.ret, (c99_ast.Structure, c99_ast.Union),
    ):
        raise AbiSelectionError(
            f"function `{fn.name}` declared "
            f"`__attribute__((zp_abi))` but returns a struct / "
            f"union; the implicit sret-pointer parameter doesn't "
            f"fit the ZP-layout slot scheme",
        )
    if _has_indirect_call(fn):
        raise AbiSelectionError(
            f"function `{fn.name}` declared "
            f"`__attribute__((zp_abi))` but its body contains an "
            f"indirect call; the callee's ABI can't be known at "
            f"the call site, so a ZP-passing function can't make "
            f"indirect calls",
        )
    if _on_cycle(fn.name, callgraph):
        raise AbiSelectionError(
            f"function `{fn.name}` declared "
            f"`__attribute__((zp_abi))` but is reachable from "
            f"itself through the static call graph (direct or "
            f"mutual recursion); a recursive call would clobber "
            f"the parameter ZP slots before the outer activation "
            f"returned",
        )
    if fn.name in address_taken:
        raise AbiSelectionError(
            f"function `{fn.name}` declared "
            f"`__attribute__((zp_abi))` but its address is taken "
            f"somewhere in the program; ZP-passing functions "
            f"can't be reached through a function pointer",
        )
    byte_count = _param_byte_count(fn_decl, types)
    window = _zp_param_window(pool)
    available = len(window)
    if byte_count > available:
        raise AbiSelectionError(
            f"function `{fn.name}` declared "
            f"`__attribute__((zp_abi))` has {byte_count} parameter "
            f"bytes, exceeding the {available}-byte ZP window "
            f"(${window.start:02X}-${window.stop - 1:02X})",
        )
    addrs = [window.start + k for k in range(byte_count)]
    per_param_bytes = _per_param_byte_sizes(fn_decl, types)
    symbols = param_slot_symbols(
        fn.name,
        list(fn_decl.params),
        per_param_bytes,
    )
    # Register params: assign A / X positionally over the marked
    # params' bytes, yielding a per-byte list parallel to `symbols`.
    per_byte_regs = _assign_param_registers(
        fn_decl, per_param_bytes, where=fn.name,
    )
    return ZpLayout(
        slot_symbols=symbols, addrs=addrs,
        param_registers=per_byte_regs,
    )


def _has_register_local(node) -> bool:
    """True iff a `register`-storage local variable appears anywhere
    in `node` (recursively). Used to detect a `register` local in a
    function body so the default-zp_abi path can upgrade
    ineligibility from a silent fallback to a hard error."""
    if isinstance(node, c99_ast.Type_var_decl):
        if isinstance(node.storage_class, c99_ast.Register):
            return True
    if dataclasses.is_dataclass(node):
        for f in dataclasses.fields(node):
            if _has_register_local(getattr(node, f.name)):
                return True
    elif isinstance(node, (list, tuple)):
        for it in node:
            if _has_register_local(it):
                return True
    return False


def _has_register_request(fn_decl: c99_ast.Type_function_decl) -> bool:
    """True iff `fn_decl` requests a 6502 register for any parameter
    or body local (the `register` keyword). Used in the default-zp_abi
    path to upgrade ineligibility from a silent fallback to a hard
    error — a function with a `register` object can't be served by
    SoftStackLayout (params in A/X, a local pinned to Y)."""
    if any(fn_decl.param_registers):
        return True
    return _has_register_local(fn_decl.body)


def _validate_zp_abi_extern(
    name: str,
    fn_decl: c99_ast.Type_function_decl,
    address_taken: set[str],
    pool: Pool,
    types,
) -> ZpLayout:
    """Validate a zp_abi annotation on an extern declaration (no
    TAC body in this TU). Only the address-taken and param-fit
    checks apply — the IndirectCall and recursion checks need a
    body to inspect, and for cross-TU functions we trust the
    programmer's annotation."""
    fun_type = fn_decl.data_type
    if isinstance(fun_type, c99_ast.FunType) and isinstance(
        fun_type.ret, (c99_ast.Structure, c99_ast.Union),
    ):
        raise AbiSelectionError(
            f"function `{name}` declared "
            f"`__attribute__((zp_abi))` but returns a struct / "
            f"union; the implicit sret-pointer parameter doesn't "
            f"fit the ZP-layout slot scheme",
        )
    if name in address_taken:
        raise AbiSelectionError(
            f"function `{name}` declared "
            f"`__attribute__((zp_abi))` but its address is taken "
            f"somewhere in the program; ZP-passing functions "
            f"can't be reached through a function pointer",
        )
    byte_count = _param_byte_count(fn_decl, types)
    window = _zp_param_window(pool)
    available = len(window)
    if byte_count > available:
        raise AbiSelectionError(
            f"function `{name}` declared "
            f"`__attribute__((zp_abi))` has {byte_count} parameter "
            f"bytes, exceeding the {available}-byte ZP window "
            f"(${window.start:02X}-${window.stop - 1:02X})",
        )
    addrs = [window.start + k for k in range(byte_count)]
    per_param_bytes = _per_param_byte_sizes(fn_decl, types)
    symbols = param_slot_symbols(
        name,
        list(fn_decl.params),
        per_param_bytes,
    )
    per_byte_regs = _assign_param_registers(
        fn_decl, per_param_bytes, where=name,
    )
    return ZpLayout(
        slot_symbols=symbols, addrs=addrs,
        param_registers=per_byte_regs,
    )


# ---------------------------------------------------------------------------
# Top-level entry point.
# ---------------------------------------------------------------------------


def select_abi(
    prog: tac_ast.Program,
    c99_prog: c99_ast.Program,
    types=None,
    *,
    pool: Pool | None = None,
) -> dict[str, ParamLayout]:
    """Compute per-function ABI for every function in `prog`.
    Returns `dict[name, ParamLayout]`. Raises `AbiSelectionError`
    if any explicit `__attribute__((zp_abi))` annotation can't be
    honored.

    Under `--optimize`, every function defaults to zp_abi: an
    unannotated function attempts zp_abi and SILENTLY falls back
    to `SoftStackLayout` on ineligibility (indirect calls,
    recursion, address taken, params don't fit). An annotated
    function keeps the strict contract — ineligibility raises.

    `types` is the type-checker's struct/union TypeTable, needed
    for parameter-byte-size computation when parameters have
    aggregate types.

    `pool` defaults to the standard caller-saved $80–$BF range."""
    if pool is None:
        pool = Pool()
    # Cross-declaration consistency: forward decls and the
    # definition must agree on reg(...) annotations. Raises if
    # any function has mismatched per-param or return registers.
    _check_forward_def_match(c99_prog)
    annotations = _annotation_map(c99_prog)
    decls = _function_decls_by_name(c99_prog)
    address_taken = _address_taken(prog)
    callgraph = _build_callgraph(prog)
    out: dict[str, ParamLayout] = {}
    defined: set[str] = set()
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        defined.add(tl.name)
        ann = annotations.get(tl.name)
        fn_decl = decls.get(tl.name)
        if ann == "zp_abi":
            if fn_decl is None:
                raise AbiSelectionError(
                    f"function `{tl.name}` declared "
                    f"`__attribute__((zp_abi))` but its c99 "
                    f"declaration is missing — internal error",
                )
            out[tl.name] = _validate_zp_abi(
                tl, fn_decl, address_taken, callgraph, pool, types,
            )
        elif ann is None:
            # Default-zp_abi: try, silently fall back to soft-stack
            # on any eligibility failure. The error messages from
            # `_validate_zp_abi` mention the annotation explicitly,
            # but they're swallowed here, so the user-facing
            # behavior is just "this function got soft-stack."
            #
            # Exception: if the function requests a `register` object
            # (param or local), ineligibility is a HARD error — the
            # SoftStackLayout fallback can't honor register-passing.
            if fn_decl is None:
                out[tl.name] = SoftStackLayout()
                continue
            try:
                out[tl.name] = _validate_zp_abi(
                    tl, fn_decl, address_taken, callgraph, pool, types,
                )
            except AbiSelectionError as exc:
                if _has_register_request(fn_decl):
                    # If the validator failed for a reason that's
                    # specific to the register request (pointer in a
                    # register, too many register bytes, ...), the
                    # error already explains the problem in user
                    # terms — re-raise it as-is. Otherwise wrap a more
                    # general explanation that names the `register`
                    # constraint.
                    msg = str(exc)
                    reg_specific = "register" in msg
                    if reg_specific:
                        raise
                    raise AbiSelectionError(
                        f"function `{tl.name}` declares a `register` "
                        f"parameter or local but isn't eligible for "
                        f"the ZP-passing ABI: {exc}"
                    ) from exc
                out[tl.name] = SoftStackLayout()
        else:
            # Defensive — the parser already filters annotation
            # names, so any value other than "zp_abi" or None
            # shouldn't reach here.
            raise AbiSelectionError(
                f"function `{tl.name}` has unrecognized abi "
                f"annotation {ann!r}",
            )
    # Externs declared in this TU but defined elsewhere. Call
    # sites in this TU need the callee's `ZpLayout` so arg writes
    # go to the callee's pinned ZP slots instead of the soft
    # stack. Annotated externs raise on ineligibility (matching
    # the strict contract); unannotated externs attempt zp_abi
    # and silently fall back, mirroring the definition-site
    # default-zp_abi policy.
    for name, ann in annotations.items():
        if name in defined:
            continue
        fn_decl = decls.get(name)
        if fn_decl is None:
            continue
        if ann == "zp_abi":
            out[name] = _validate_zp_abi_extern(
                name, fn_decl, address_taken, pool, types,
            )
    for name, fn_decl in decls.items():
        if name in defined or name in out:
            continue
        if annotations.get(name) is not None:
            continue
        try:
            out[name] = _validate_zp_abi_extern(
                name, fn_decl, address_taken, pool, types,
            )
        except AbiSelectionError:
            # Extern got rejected (address-taken or param window
            # overflow). Don't record an entry — the call site
            # will fall back to soft-stack via the `abi.get(name)
            # is None` path in tac_to_asm.
            #
            # Exception: an extern with a `register` parameter MUST
            # be served by ZpLayout — the caller can't synthesize
            # register-passing on a SoftStackLayout callee. Re-raise.
            if _has_register_request(fn_decl):
                raise AbiSelectionError(
                    f"extern function `{name}` declares a `register` "
                    f"parameter but isn't eligible for the ZP-passing "
                    f"ABI; `register` requires zp_abi eligibility"
                )
    return out
