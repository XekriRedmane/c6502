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
    param_layouts=None,
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
    builds — so callers only need to supply the extras.

    `param_layouts` is an optional `dict[name, ParamLayout]` from
    `passes.abi_selection.select_abi`. For a function with
    `ZpLayout`, the layout's ZP addresses are reserved (`blocked
    addrs`) during regalloc so body locals don't collide with
    incoming param bytes. Without this dict, every function is
    treated as soft-stack ABI and no addresses are reserved."""
    from passes.abi_selection import ZpLayout
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
            blocked_addrs = _blocked_addrs_for(tl, param_layouts)
            new_fn, coloring = _optimize_function(
                tl, statics_frozen, blocked_addrs,
            )
            new_top.append(new_fn)
            colorings[new_fn.name] = coloring
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top), colorings


def _blocked_addrs_for(
    fn: asm_ast.Function, param_layouts,
) -> set[int]:
    """ZP addresses that this function's body regalloc must avoid:

    - The function's own incoming param slots (when it's a ZpLayout
      function): body locals must not collide with where the
      caller wrote the param bytes.
    - Every ZP-ABI callee's param slots: the function's body
      locals must not be at the addresses where outgoing-arg writes
      will land just before each `Call`. Without this, the body
      could place a live local at the same address as an
      outgoing-arg destination, and the arg writes — which alias
      that location at the call boundary — would clobber it.

    Conservative: blocks for the entire function rather than only
    during the arg-write window or the param's lifetime. The few
    wasted ZP slots are typically negligible vs. the savings of
    frame elimination."""
    from passes.abi_selection import ZpLayout
    out: set[int] = set()
    if param_layouts is None:
        return out
    own_layout = param_layouts.get(fn.name)
    if isinstance(own_layout, ZpLayout):
        out.update(own_layout.addrs)
    # Any direct call to a ZP-ABI callee in this function's body.
    for instr in fn.instructions:
        if isinstance(instr, asm_ast.Call):
            callee_layout = param_layouts.get(instr.name)
            if isinstance(callee_layout, ZpLayout):
                out.update(callee_layout.addrs)
    return out


def _optimize_function(
    fn: asm_ast.Function, statics: frozenset[str],
    blocked_addrs: set[int],
) -> tuple[asm_ast.Function, Coloring]:
    fn = to_ssa(fn, statics=statics)
    # Step 6: byte-granular DCE drops Movs / Phis whose dst Pseudo
    # is unused. Iterates to a fixed point internally. Statics
    # are passed through so writes to them stay live (other
    # functions may read them).
    fn = byte_dce(fn, statics=statics)
    # Step 7: byte-granular regalloc on the still-SSA function.
    # The chordal property of SSA interference graphs makes greedy
    # PEO coloring optimal at unit width. `blocked_addrs` carries
    # the ZP addresses occupied by ZP-ABI params (if any), so
    # body-local coloring avoids them and doesn't clobber incoming
    # param bytes mid-computation.
    liveness = compute_liveness(fn)
    graph = build_interference(fn, liveness, statics=statics)
    coloring = color_graph(fn, graph, blocked_addrs=blocked_addrs)
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
