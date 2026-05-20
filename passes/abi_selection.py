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
    entry is None (the byte arrives via the ZP slot) or "A"/"X"/"Y"
    (the byte arrives in the named 6502 register at the call
    boundary). When a byte's `param_registers[i]` is set, the slot
    symbol still exists and gets a ZP byte — the callee body reads
    it like any other zp_abi param byte — but the caller does NOT
    write to it: instead the caller loads the byte into the named
    register before `JSR`, and the callee's entry stub stores the
    register into the slot. v1 supports register-passed params for
    1-byte types only, so `param_registers[i]` is set on exactly
    one slot per reg-attributed parameter (the parameter's single
    byte).

    `return_register` names the 6502 register the callee leaves
    the result in just before `RTS`: None (the default A), or
    "A"/"X"/"Y" explicitly. The caller captures from the named
    register immediately after `JSR`. v1 supports a register
    return for 1-byte return types only."""
    slot_symbols: list[str] = field(default_factory=list)
    addrs: list[int] = field(default_factory=list)
    param_registers: list[str | None] = field(default_factory=list)
    return_register: str | None = None


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


def _is_one_byte_type(t) -> bool:
    """True iff `t` is a 1-byte integer type — the only types that
    fit a single 6502 register. Strips Const/Volatile wrappers."""
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar))


def _validate_reg_attributes(
    fn_decl: c99_ast.Type_function_decl, where: str,
) -> tuple[list[str | None], str | None]:
    """Validate `fn_decl.param_registers` and `fn_decl.return_register`
    against the v1 contract (1-byte types only; no overlap among
    registers used by simultaneously-live values). Returns
    `(per_param_register_list, return_register)`, with the empty-
    string sentinel from `param_registers` normalized back to None.

    Raises `AbiSelectionError` on:
      - reg("...") on a param whose type isn't 1-byte.
      - reg("...") on a return whose type isn't 1-byte.
      - The same register named on two different params, or on a
        param and the return at the same time. "Same time" means
        both values are alive at the call boundary, which is true
        for every (param-in, return-out) pair (the caller must
        save the param register's content before reading the
        return register from the same register). v1 takes the
        conservative line: no overlap allowed.

    `where` is a short label for error messages (e.g. the function
    name)."""
    fun_type = fn_decl.data_type
    assert isinstance(fun_type, c99_ast.FunType)
    # Normalize the empty-string sentinel back to None for cleaner
    # downstream code.
    per_param: list[str | None] = [
        r if r else None for r in fn_decl.param_registers
    ]
    # If param_registers wasn't filled (older AST construction sites),
    # default to an empty annotation per parameter.
    while len(per_param) < len(fun_type.params):
        per_param.append(None)
    # 1-byte check for each reg-attributed param.
    for i, (param_t, reg) in enumerate(zip(fun_type.params, per_param)):
        if reg is None:
            continue
        if not _is_one_byte_type(param_t):
            raise AbiSelectionError(
                f"function `{where}` parameter {i} declared "
                f"`__attribute__((reg({reg!r})))` but its type "
                f"isn't 1-byte (Char/SChar/UChar required); v1 "
                f"can't fit a multi-byte value in a single 6502 "
                f"register"
            )
    # 1-byte check for the return register.
    if (
        fn_decl.return_register is not None
        and not _is_one_byte_type(fun_type.ret)
    ):
        raise AbiSelectionError(
            f"function `{where}` declared "
            f"`__attribute__((reg({fn_decl.return_register!r})))` "
            f"on its return slot but the return type isn't 1-byte "
            f"(Char/SChar/UChar required)"
        )
    # Overlap check. Two reg-attributed params can't share a
    # register (both values are simultaneously live at the call
    # boundary on the caller side). A param register can't also be
    # the return register (the param value must survive past the
    # function body's use of it into the return slot, but the
    # callee body is free to clobber any register, so the caller
    # must save the param value before the JSR — see Task #8
    # ordering).
    seen: dict[str, str] = {}
    for i, reg in enumerate(per_param):
        if reg is None:
            continue
        slot = f"param {i} (`{fn_decl.params[i]}`)"
        if reg in seen:
            raise AbiSelectionError(
                f"function `{where}`: register {reg!r} is named on "
                f"{seen[reg]} AND on {slot}; each reg(...) register "
                f"must be unique across parameters"
            )
        seen[reg] = slot
    if fn_decl.return_register is not None:
        if fn_decl.return_register in seen:
            raise AbiSelectionError(
                f"function `{where}`: return register "
                f"{fn_decl.return_register!r} conflicts with "
                f"{seen[fn_decl.return_register]} — caller can't "
                f"hold the parameter and the return in the same "
                f"register at the same time"
            )
    return per_param, fn_decl.return_register


def _check_forward_def_match(c99_prog: c99_ast.Program) -> None:
    """Across multiple declarations / definitions of the same
    function name, every forward decl + definition must agree on
    `param_registers` and `return_register`. Mismatch is a hard
    error (an `extern T fn(...)` in a header and the body
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
        # Normalize the empty-string sentinel for comparison so
        # `[""]` vs `[None]` doesn't false-positive.
        a = [r or None for r in prior.param_registers]
        b = [r or None for r in fn.param_registers]
        # Pad to the shorter list's length so old AST construction
        # sites (which leave `param_registers=[]`) compare cleanly
        # with parser-built decls that fill the list.
        if len(a) < len(b):
            a = a + [None] * (len(b) - len(a))
        if len(b) < len(a):
            b = b + [None] * (len(a) - len(b))
        if a != b:
            raise AbiSelectionError(
                f"function `{fn.name}`: param_registers attribute "
                f"differs between declarations — {a!r} vs {b!r}; "
                f"the calling convention must match"
            )
        if prior.return_register != fn.return_register:
            raise AbiSelectionError(
                f"function `{fn.name}`: return_register attribute "
                f"differs between declarations — "
                f"{prior.return_register!r} vs {fn.return_register!r}"
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
    # Reg-attribute layout: validate types + uniqueness, then expand
    # the per-parameter register list to a per-byte list parallel to
    # `symbols`.
    per_param_regs, return_register = _validate_reg_attributes(
        fn_decl, where=fn.name,
    )
    per_byte_regs = _expand_param_registers_to_per_byte(
        per_param_regs, per_param_bytes,
    )
    return ZpLayout(
        slot_symbols=symbols, addrs=addrs,
        param_registers=per_byte_regs,
        return_register=return_register,
    )


def _expand_param_registers_to_per_byte(
    per_param: list[str | None], per_param_bytes: list[int],
) -> list[str | None]:
    """Expand a per-parameter register-name list (parallel to
    `params`) to a per-byte list parallel to `slot_symbols`. Each
    reg-attributed param is 1-byte (validated upstream) so it
    contributes exactly one entry; non-reg params contribute their
    full byte width of None entries. Used by both the in-TU and
    extern zp_abi validators."""
    out: list[str | None] = []
    for reg, n_bytes in zip(per_param, per_param_bytes):
        if reg is None:
            out.extend([None] * n_bytes)
        else:
            assert n_bytes == 1, (
                f"reg-attributed param expected 1 byte, got "
                f"{n_bytes} — should have been rejected upstream"
            )
            out.append(reg)
    return out


def _has_reg_attributes(fn_decl: c99_ast.Type_function_decl) -> bool:
    """True iff `fn_decl` carries any `reg("...")` annotation on its
    return slot or on any parameter. Used in the default-zp_abi path
    to upgrade ineligibility from a silent fallback to a hard error
    — a reg-attributed function can't be served by SoftStackLayout."""
    if fn_decl.return_register is not None:
        return True
    return any(r for r in fn_decl.param_registers)


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
    per_param_regs, return_register = _validate_reg_attributes(
        fn_decl, where=name,
    )
    per_byte_regs = _expand_param_registers_to_per_byte(
        per_param_regs, per_param_bytes,
    )
    return ZpLayout(
        slot_symbols=symbols, addrs=addrs,
        param_registers=per_byte_regs,
        return_register=return_register,
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
            # Exception: if the function carries any reg(...)
            # annotation, ineligibility is a HARD error — the
            # SoftStackLayout fallback can't honor register-passing.
            if fn_decl is None:
                out[tl.name] = SoftStackLayout()
                continue
            try:
                out[tl.name] = _validate_zp_abi(
                    tl, fn_decl, address_taken, callgraph, pool, types,
                )
            except AbiSelectionError as exc:
                if _has_reg_attributes(fn_decl):
                    # If the validator failed for a reason that's
                    # specific to reg(...) (1-byte type, register
                    # conflict, ...), the error already explains
                    # the problem in user terms — re-raise it as-is.
                    # Otherwise wrap a more general explanation
                    # that names the reg(...) constraint.
                    msg = str(exc)
                    reg_specific = (
                        "reg(" in msg
                        or "1-byte" in msg
                        or "register" in msg
                    )
                    if reg_specific:
                        raise
                    raise AbiSelectionError(
                        f"function `{tl.name}` carries "
                        f"`__attribute__((reg(...)))` but isn't "
                        f"eligible for the ZP-passing ABI: {exc}"
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
            # Exception: an extern carrying reg(...) attributes
            # MUST be served by ZpLayout — the caller can't
            # synthesize register-passing on a SoftStackLayout
            # callee. Re-raise.
            if _has_reg_attributes(fn_decl):
                raise AbiSelectionError(
                    f"extern function `{name}` carries "
                    f"`__attribute__((reg(...)))` but isn't "
                    f"eligible for the ZP-passing ABI; reg(...) "
                    f"requires zp_abi eligibility"
                )
    return out
