"""Asm-level optimizer driver.

Runs the asm-level SSA round-trip on each `Function` top-level in
the program. Mirrors the shape of `passes.optimization.optimize_
program` but operates on `asm_ast.Program` with `Pseudo` operands
(so it must run BEFORE `replace_pseudoregisters`).

Step 7 shape:

    fn → to_ssa → byte_dce → liveness + interference + regalloc
       → from_ssa → fn'

The per-function `Coloring` is returned alongside the program
(empty entries for functions where regalloc found nothing
colorable). `compile.py` / `sim/harness.py` thread the colorings
into `replace_pseudoregisters_bare_exit`, which lowers
`Pseudo(name, 0)` operands whose name is in the coloring to
`ZP(addr, 0)`.

`StaticVariable` top-levels pass through unchanged.
"""
from __future__ import annotations

import asm_ast
from passes.optimization.register_allocation import Coloring
from passes.optimization_asm.apply_coloring import apply_coloring
from passes.optimization_asm.byte_dce import byte_dce
from passes.optimization_asm.interference import build_interference
from passes.optimization_asm.liveness import compute_liveness
from passes.optimization_asm.regalloc import color_graph
from passes.optimization_asm.ssa_construction import to_ssa
from passes.optimization_asm.ssa_destruction import from_ssa


def optimize_program(
    prog: asm_ast.Type_program, *,
    extra_statics: frozenset[str] = frozenset(),
) -> tuple[asm_ast.Type_program, dict[str, Coloring]]:
    """Walk every Function top-level and apply the asm-level SSA
    round-trip. Returns the rewritten program alongside a
    `dict[func_name, Coloring]` mapping each function to its byte-
    granular coloring (`replace_pseudoregisters_bare_exit` consumes
    the dict to lower colored Pseudos to `ZP(addr, 0)` operands).

    `StaticVariable` top-levels pass through unchanged and are
    absent from the coloring dict.

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
    colorings: dict[str, Coloring] = {}
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_fn, coloring = _optimize_function(tl, statics_frozen)
            new_top.append(new_fn)
            colorings[new_fn.name] = coloring
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top), colorings


def _optimize_function(
    fn: asm_ast.Function, statics: frozenset[str],
) -> tuple[asm_ast.Function, Coloring]:
    fn = to_ssa(fn, statics=statics)
    # Step 6: byte-granular DCE drops Movs / Phis whose dst Pseudo
    # is unused. Iterates to a fixed point internally. Statics
    # are passed through so writes to them stay live (other
    # functions may read them).
    fn = byte_dce(fn, statics=statics)
    # Step 7: byte-granular regalloc on the still-SSA function.
    # The chordal property of SSA interference graphs makes greedy
    # PEO coloring optimal at unit width.
    liveness = compute_liveness(fn)
    graph = build_interference(fn, liveness, statics=statics)
    coloring = color_graph(fn, graph)
    # Apply the coloring to the SSA function BEFORE destruction so
    # `from_ssa`'s parallel-copy ordering can detect cross-Mov
    # cycles at the physical-slot level. Two Phi-derived Movs whose
    # SSA-distinct names happen to color to the same ZP slot would
    # otherwise miss each other in the cycle check (which used to
    # compare by SSA name). After this rewrite, those Movs become
    # `Mov(ZP($A), ZP($B))` shapes that the storage-key check
    # handles correctly.
    fn = apply_coloring(fn, coloring)
    fn = from_ssa(fn)
    return fn, coloring
