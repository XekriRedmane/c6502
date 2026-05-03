"""ABI selection / validation for the per-function ZP-passing
calling convention.

Given a TAC program (for body inspection) and the c99 AST (for the
`abi_annotation` field on each `FunctionDecl` / `VarDecl`), produce
a `dict[str, ParamLayout]` mapping each function name to its calling
convention. The ABI is **driven by the programmer's annotation** —
without `__attribute__((zp_abi))` a function gets the soft-stack
ABI; with the annotation, the function gets ZP-passing after
validation.

Validation of a `zp_abi` function (rejected with a clear error if
any check fails):

  1. **No nested calls.** The TAC body must contain zero
     `FunctionCall` and zero `IndirectCall` instructions. A
     function that calls others can't keep its parameters at
     fixed ZP addresses across the call window — the callee
     would clobber them.
  2. **Address not taken.** No `Var(name=fn)` appears anywhere
     in the program. If the function's address is taken, an
     indirect call site can't know its custom ABI; only the
     soft-stack convention can be assumed at indirect call
     sites.
  3. **Params fit.** The total parameter byte count fits in
     the configured ZP window (`pool.zp_param_window()` —
     defaults to the caller-saved range $80–$BF).

The returned dict has an entry for every `Function` top-level in
`prog`. Functions with no annotation get `SoftStackLayout`. The
dict can also be queried for `extern`-only declarations (which
have no TAC `Function` body) — this design gives those
`SoftStackLayout` always (cross-TU functions can't be reasoned
about from inside this TU).

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


class AbiSelectionError(Exception):
    """Raised when a `__attribute__((zp_abi))` annotation can't be
    honored: the function makes nested calls, has its address
    taken, or has too many parameter bytes."""


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
    fixed ZP address. `addrs[i]` is the address holding the i-th
    parameter byte (low byte of param 0 first, then high byte of
    param 0, then low byte of param 1, etc.)."""
    addrs: list[int] = field(default_factory=list)


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


def _has_call_in_body(fn: tac_ast.Function) -> bool:
    return any(
        isinstance(i, (tac_ast.FunctionCall, tac_ast.IndirectCall))
        for i in fn.instructions
    )


def _validate_zp_abi(
    fn: tac_ast.Function,
    fn_decl: c99_ast.Type_function_decl,
    address_taken: set[str],
    pool: Pool,
    types,
) -> ZpLayout:
    """Validate that `fn` can be given the ZP-passing ABI; return
    the computed `ZpLayout`. Raises `AbiSelectionError` on any
    violation."""
    if _has_call_in_body(fn):
        raise AbiSelectionError(
            f"function `{fn.name}` declared "
            f"`__attribute__((zp_abi))` but its body contains a "
            f"call instruction; ZP-passing functions must be "
            f"leaves",
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
    return ZpLayout(addrs=addrs)


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
    if any `__attribute__((zp_abi))` annotation can't be honored.

    `types` is the type-checker's struct/union TypeTable, needed
    for parameter-byte-size computation when parameters have
    aggregate types.

    `pool` defaults to the standard caller-saved $80–$BF range."""
    if pool is None:
        pool = Pool()
    annotations = _annotation_map(c99_prog)
    decls = _function_decls_by_name(c99_prog)
    address_taken = _address_taken(prog)
    out: dict[str, ParamLayout] = {}
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
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
                tl, fn_decl, address_taken, pool, types,
            )
        elif ann is None:
            out[tl.name] = SoftStackLayout()
        else:
            # Defensive — the parser already filters annotation
            # names, so any value other than "zp_abi" or None
            # shouldn't reach here.
            raise AbiSelectionError(
                f"function `{tl.name}` has unrecognized abi "
                f"annotation {ann!r}",
            )
    return out
