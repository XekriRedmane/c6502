"""Asm-level optimizer driver.

Runs the asm-level SSA round-trip on each `Function` top-level in
the program. Mirrors the shape of `passes.optimization.optimize_
program` but operates on `asm_ast.Program` with `Pseudo` operands
(so it must run BEFORE `replace_pseudoregisters`).

Step 5e shape — round-trip skeleton, no opts in between:

    fn → to_ssa → from_ssa → fn'

Once steps 6 (asm-level opts: byte-DCE, peepholes) and 7 (asm-level
byte-granular regalloc) land, the body grows:

    fn → to_ssa → (asm-level fixed-point opts)*
       → asm-level regalloc → from_ssa → fn'

`StaticVariable` top-levels pass through unchanged (their byte
layout is fixed at the typed-init list).
"""
from __future__ import annotations

import asm_ast
from passes.optimization_asm.ssa_construction import to_ssa
from passes.optimization_asm.ssa_destruction import from_ssa


def optimize_program(
    prog: asm_ast.Type_program, *,
    extra_statics: frozenset[str] = frozenset(),
) -> asm_ast.Type_program:
    """Walk every Function top-level and apply the asm-level SSA
    round-trip. StaticVariable top-levels pass through unchanged.

    `extra_statics` are static-storage names without a
    `StaticVariable` top-level definition in this program (e.g.
    `extern` references); these need to be excluded from byte-
    granular SSA renaming so the final asm still references the
    real link-time addresses. The static names DEFINED here (every
    `StaticVariable` top-level + every `Function` name) are added
    automatically — same set as the one `replace_program_bare_exit`
    builds — so callers only need to supply the extras."""
    statics: set[str] = set(extra_statics)
    statics |= {
        tl.name for tl in prog.top_level
        if isinstance(tl, (asm_ast.StaticVariable, asm_ast.Function))
    }
    statics_frozen = frozenset(statics)
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_optimize_function(tl, statics_frozen))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _optimize_function(
    fn: asm_ast.Function, statics: frozenset[str],
) -> asm_ast.Function:
    fn = to_ssa(fn, statics=statics)
    # Step 5e: no opts between to_ssa and from_ssa. Steps 6 / 7
    # will slot byte-level optimizations and regalloc here.
    fn = from_ssa(fn)
    return fn
