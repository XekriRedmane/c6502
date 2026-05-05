"""Asm-level optimizer driver.

Runs the asm-level SSA round-trip on each `Function` top-level in
the program. Mirrors the shape of `passes.optimization.optimize_
program` but operates on `asm_ast.Program` with `Pseudo` operands
(so it must run BEFORE `replace_pseudoregisters`).

Step 7 shape:

    fn → to_ssa
       → [copy_propagate → backward_copy_propagate → byte_dce]*
       → liveness + interference + regalloc → from_ssa → fn'

The `[...]*` bracket runs to a fixed point. Each pass enables the
others:
  * `copy_propagate` substitutes uses of a copy's `dst` with its
    `src`, leaving the original `Mov` dead.
  * `backward_copy_propagate` collapses a `Pseudo P` whose only
    role is bridging `Mov(Reg(A), P); ...; Mov(P, Reg(A));
    Mov(Reg(A), D)` round-trips, redirecting `P`'s def to write
    `D` directly.
  * `byte_dce` drops the now-unused defs.

Mirrors the TAC optimizer's constant-fold / UCE / copy-prop / DSE
fixed-point bracket, with the asm-level passes in place of the
TAC ones.

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
from passes.optimization_asm.backward_copy_propagation import (
    backward_copy_propagate,
)
from passes.optimization_asm.byte_dce import byte_dce
from passes.optimization_asm.coalescing import coalesce_moves
from passes.optimization_asm.const_static_fold import fold_const_statics
from passes.optimization_asm.dead_static import apply_dead_static_elimination
from passes.optimization_asm.copy_propagation import copy_propagate
from passes.optimization_asm.interference import build_interference
from passes.optimization_asm.liveness import compute_liveness
from passes.optimization_asm.regalloc import color_graph
from passes.optimization_asm.ssa_construction import to_ssa
from passes.optimization_asm.ssa_destruction import from_ssa


def optimize_program(
    prog: asm_ast.Type_program, *,
    extra_statics: frozenset[str] = frozenset(),
    param_layouts=None,
    symbols=None,
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
    treated as soft-stack ABI and no addresses are reserved.

    `symbols` is the c6502 symbol table; passed through to
    `fold_const_statics` (the const-static prepass) so it can
    check the const qualifier on each candidate static. Without
    it, the prepass is a no-op."""
    from passes.abi_selection import ZpLayout
    # Program-level prepass: replace references to const-qualified
    # internal-linkage scalar statics with Imm operands carrying
    # the static's byte values, and drop the now-unreferenced
    # StaticVariable top-levels. Runs BEFORE the per-function SSA
    # round-trip — the resulting `Mov(Imm, Pseudo)` shapes are
    # picked up by forward copy propagation in the existing fixed-
    # point bracket.
    prog = fold_const_statics(prog, symbols=symbols)
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
    # Drop any internal-linkage StaticVariable nothing references.
    # Runs AFTER the per-function loop so the per-function passes'
    # DCE / copy-prop can settle before we count references — a
    # static whose last use was a dead load shouldn't survive just
    # because the load was still in the IR upstream.
    out_prog = apply_dead_static_elimination(
        asm_ast.Program(top_level=new_top),
    )
    return out_prog, colorings


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
    # Step 6: SSA-aware forward + backward copy propagation +
    # byte-granular DCE, iterated to a fixed point. Forward
    # substitutes uses with the copy's source; backward collapses
    # `Pseudo → Reg(A) → memory` round-trips into a single direct
    # write; byte-DCE drops Movs whose dst is no longer read.
    # Each pass can expose work for the others. Statics are
    # passed through all three so external-visibility writes
    # stay live.
    while True:
        prev = fn
        fn = copy_propagate(fn, statics=statics)
        fn = backward_copy_propagate(fn, statics=statics)
        fn = byte_dce(fn, statics=statics)
        if fn.instructions == prev.instructions:
            break
    # Step 7: byte-granular regalloc on the still-SSA function.
    # The chordal property of SSA interference graphs makes greedy
    # PEO coloring optimal at unit width. `blocked_addrs` carries
    # the ZP addresses occupied by ZP-ABI params (if any), so
    # body-local coloring avoids them and doesn't clobber incoming
    # param bytes mid-computation.
    liveness = compute_liveness(fn)
    graph = build_interference(fn, liveness, statics=statics)
    # Step 7a: move coalescing. Merge non-interfering Pseudo pairs
    # connected by a Mov or Phi-arg into one node so they get the
    # same color. After Phi destruction the corresponding Movs
    # become self-Movs that asm_emit's self-Mov peephole drops —
    # eliminating the SSA-Phi-induced temp routing.
    coalesce_result = coalesce_moves(fn, graph)
    coloring = color_graph(fn, graph, blocked_addrs=blocked_addrs)
    # Project coloring through the coalescing result: every name
    # that was merged into a representative inherits the rep's
    # color. apply_coloring keys on names, so it needs the merged
    # names back in the assignments dict.
    for name, _rep in coalesce_result.representative.items():
        rep = coalesce_result.resolve(name)
        if rep in coloring.assignments:
            coloring.assignments[name] = coloring.assignments[rep]
        elif rep in coloring.spilled:
            coloring.spilled.add(name)
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
