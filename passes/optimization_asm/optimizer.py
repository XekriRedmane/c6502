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
from passes.optimization_asm.or_zero_absorb import absorb_zero_load
from passes.optimization_asm.hwreg_eligibility import (
    HwRegEligibility,
    scan_function as scan_hwreg_eligibility,
)
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
    local_pools: dict[str, list[int]] | None = None,
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
    it, the prepass is a no-op.

    `local_pools`, when supplied, is a `dict[fn_name, list[int]]`
    from `passes.zp_local_allocation.allocate_function_locals`.
    For every function present in the dict, the asm-level
    regalloc draws colors EXCLUSIVELY from that function's
    private range (a contiguous list of byte addresses,
    typically in zero page but possibly spilled above `$FF`).
    Functions NOT in the dict fall back to the default
    caller/callee-saved partition of `Pool(start=0x80)`. This
    enables the call-graph-disjoint allocation that eliminates
    callee-save prologues for eligible functions."""
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
    # Derive `register_pins: dict[base_name, "X"|"Y"]` from the
    # c99 symbol table. Each LocalAttr.register_class identifies a
    # variable that the user pinned via `__attribute__((reg("...")))`.
    # The asm regalloc's HwReg pre-pass consumes this dict so
    # those Pseudos color directly to Reg(X) / Reg(Y) rather than
    # to a ZP byte.
    register_pins = _register_pins_from_symbols(symbols)
    new_top: list[asm_ast.Type_top_level] = []
    colorings: dict[str, Coloring] = {}
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            blocked_addrs = _blocked_addrs_for(tl, param_layouts)
            allowed_range = _allowed_range_for(tl, local_pools)
            pool_for_fn = (
                local_pools.get(tl.name)
                if local_pools is not None else None
            )
            new_fn, coloring = _optimize_function(
                tl, statics_frozen, blocked_addrs, allowed_range,
                local_pool=pool_for_fn,
                register_pins=register_pins,
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


def _allowed_range_for(
    fn: asm_ast.Function,
    local_pools: dict[str, list[int]] | None,
) -> range | None:
    """If `fn` has a private local pool, return the contiguous
    `range(min, max + 1)` that bounds it. The pool is always
    contiguous (allocator guarantees) so a `range` is the natural
    representation for `color_graph`'s `_find_fit`. Returns None
    when the function isn't in the pool dict — the regalloc then
    falls back to the default caller/callee partition."""
    if local_pools is None:
        return None
    pool = local_pools.get(fn.name)
    if not pool:
        return None
    lo = pool[0]
    hi = pool[-1] + 1
    return range(lo, hi)


def _register_pins_from_symbols(symbols) -> dict[str, str]:
    """Walk the c99 symbol table for every LocalAttr that carries a
    `register_class` attribute and return a `{name: register}` dict.
    The names are the c99 IR's resolved spellings (`@<N>.<orig>`),
    which the asm-level SSA construction will use as the base of
    each renamed version. Returns an empty dict when `symbols` is
    None or no such attributes exist."""
    if symbols is None:
        return {}
    from passes.type_checking import LocalAttr
    out: dict[str, str] = {}
    # SymbolTable exposes the underlying dict via the iterable-of-
    # items protocol (or .__iter__); access the private dict for
    # robustness across that boundary.
    table = getattr(symbols, "_table", None)
    if table is None:
        return {}
    for name, sym in table.items():
        attrs = sym.attrs
        if isinstance(attrs, LocalAttr) and attrs.register_class is not None:
            out[name] = attrs.register_class
    return out


def _optimize_function(
    fn: asm_ast.Function, statics: frozenset[str],
    blocked_addrs: set[int],
    allowed_range: range | None = None,
    local_pool: list[int] | None = None,
    *,
    register_pins: dict[str, str] | None = None,
) -> tuple[asm_ast.Function, Coloring]:
    # Pre-pass: fuse `LDA P; SEC; SBC #1; STA dst; LDA #0; CMP P;
    # B<cc>` into `LDA P; SEC; SBC #1; STA dst; B<flipped>`. Runs
    # BEFORE SSA construction so the resulting IR (no Compare-
    # against-zero) is what the eligibility scan sees — without
    # the Compare's `cmp_right` position, the iv operand becomes
    # HwReg-eligible.
    from passes.sub1_test_zero_peephole import (
        apply_sub1_test_zero_peephole,
    )
    # Wrap as a single-fn program for the pass's API.
    prog = asm_ast.Program(top_level=[fn])
    fn = apply_sub1_test_zero_peephole(prog).top_level[0]
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
        # Absorb-zero-load: collapse `Mov(Imm(0), A); Or(X, A)` to
        # `Mov(X, A)`. Done here (pre-coalescing) so the resulting
        # cleaner copy chain feeds into move coalescing. See the
        # module docstring for why this is split from the post-
        # coloring const_arith_fold pass.
        fn = absorb_zero_load(fn)
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
    # Step 7b: HwReg-pinning eligibility. Scan the function for
    # Pseudos that meet the per-instruction shape constraints
    # (eligible to live in X / Y), with hint sets for names that
    # currently feed an `Mov(P, A); Mov(A, X|Y)` index-setup chain
    # — those are the candidates with the largest payoff. We scan
    # the IR (pre-coalescing names), then project to rep level
    # using the coalescing result.
    # Locals' reg-attributes are HARD pins: the user has no
    # fallback storage so failure must surface as an error.
    # Params' reg-attributes are SOFT hints: the calling-convention
    # entry stub already writes the param's ZP slot, so the body
    # has a valid fallback if the IR shape doesn't fit the requested
    # register (e.g. `n` used in `LDA #0; CMP n` — CMP doesn't
    # accept X/Y as the right operand). When the hint succeeds the
    # entry-stub slot store becomes dead and gets dropped.
    if register_pins:
        local_pins = {
            n: r for n, r in register_pins.items() if n not in fn.params
        }
        param_hints = {
            n: r for n, r in register_pins.items() if n in fn.params
        }
    else:
        local_pins = None
        param_hints = None
    raw_eligibility = scan_hwreg_eligibility(
        fn, register_pins=local_pins, register_hints=param_hints,
    )
    rep_eligibility = _project_eligibility(
        raw_eligibility, coalesce_result, all_names=_all_pseudo_names(fn),
    )
    coloring = color_graph(
        fn, graph, blocked_addrs=blocked_addrs,
        hwreg_eligibility=rep_eligibility,
        allowed_range=allowed_range,
        liveness=liveness,
        rep_map=coalesce_result.representative,
    )
    # Project coloring through the coalescing result: every name
    # that was merged into a representative inherits the rep's
    # color. apply_coloring keys on names, so it needs the merged
    # names back in the assignments / hwreg_assignments dict.
    for name, _rep in coalesce_result.representative.items():
        rep = coalesce_result.resolve(name)
        if rep in coloring.hwreg_assignments:
            coloring.hwreg_assignments[name] = coloring.hwreg_assignments[rep]
        elif rep in coloring.assignments:
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
    fn = apply_coloring(fn, coloring, local_pool=local_pool)
    fn = from_ssa(fn)
    return fn, coloring


def _all_pseudo_names(fn: asm_ast.Function) -> set[str]:
    """Every Pseudo name appearing anywhere in `fn.instructions`.
    Used to enumerate names for the eligibility projection."""
    out: set[str] = set()
    for instr in fn.instructions:
        for op in _all_operands_in(instr):
            if isinstance(op, asm_ast.Pseudo):
                out.add(op.name)
    return out


def _all_operands_in(
    instr: asm_ast.Type_instruction,
):
    """Yield every operand appearing in `instr`. Mirrors the helpers
    in `interference._all_pseudos_in` and `liveness._defs_in`/
    `_uses_in` but yields all operands (not just Pseudos), so callers
    can filter."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            yield src; yield dst
        case asm_ast.Add(src=s, dst=d) | asm_ast.Sub(src=s, dst=d):
            yield s; yield d
        case asm_ast.And(src=s, dst=d) | asm_ast.Or(src=s, dst=d):
            yield s; yield d
        case asm_ast.Xor(src1=s1, src2=s2, dst=d):
            yield s1; yield s2; yield d
        case asm_ast.Inc(dst=d) | asm_ast.Dec(dst=d):
            yield d
        case (
            asm_ast.ArithmeticShiftLeft(dst=d)
            | asm_ast.LogicalShiftRight(dst=d)
            | asm_ast.RotateLeft(dst=d)
            | asm_ast.RotateRight(dst=d)
        ):
            yield d
        case asm_ast.Push(src=s):
            yield s
        case asm_ast.Pop(dst=d):
            yield d
        case asm_ast.Compare(left=l, right=r):
            yield l; yield r
        case asm_ast.LoadAddress(src=s, dst=d):
            yield s; yield d
        case asm_ast.Phi(dst=d, args=args):
            yield d
            for a in args:
                yield a.source


def _project_eligibility(
    raw: HwRegEligibility,
    coalesce_result,
    *, all_names: set[str],
) -> HwRegEligibility:
    """Project per-Pseudo eligibility through the coalescing result.

    A coalesced rep is in `eligible_x` iff EVERY member that maps
    to it is in `raw.eligible_x` — conservative: a single member
    that can't be X-pinned taints the rep. Same independently for
    `eligible_y`. Hints transfer from any member: the rep is in
    `hints_x` iff any member is, and similarly for hints_y. Hints
    are then restricted to the matching-HwReg eligible set.

    Names in `all_names` that aren't in `coalesce_result.
    representative` are singletons — they map to themselves and
    project trivially."""
    # Group names by rep.
    members_by_rep: dict[str, set[str]] = {}
    for name in all_names:
        rep = coalesce_result.resolve(name)
        members_by_rep.setdefault(rep, set()).add(name)
    rep_eligible_x: set[str] = set()
    rep_eligible_y: set[str] = set()
    rep_hints_x: set[str] = set()
    rep_hints_y: set[str] = set()
    rep_required_x: set[str] = set()
    rep_required_y: set[str] = set()
    rep_use_count: dict[str, int] = {}
    for rep, members in members_by_rep.items():
        if all(m in raw.eligible_x for m in members):
            rep_eligible_x.add(rep)
        if all(m in raw.eligible_y for m in members):
            rep_eligible_y.add(rep)
        if any(m in raw.hints_x for m in members):
            rep_hints_x.add(rep)
        if any(m in raw.hints_y for m in members):
            rep_hints_y.add(rep)
        # Required transfers if ANY member is required — coalescing
        # is symmetric, so a rep that swallows a pinned member is
        # itself pinned. The regalloc will check that the rep is in
        # the matching eligible set; if coalescing merged a pinned
        # name with one that isn't HwReg-eligible, the rep won't be
        # in `rep_eligible_<x|y>` and the regalloc will hard-error
        # with a useful message.
        if any(m in raw.required_x for m in members):
            rep_required_x.add(rep)
        if any(m in raw.required_y for m in members):
            rep_required_y.add(rep)
        # Use count is the sum across all merged members.
        rep_use_count[rep] = sum(
            raw.use_count.get(m, 0) for m in members
        )
    rep_eligible_any = rep_eligible_x | rep_eligible_y
    rep_hints_x &= rep_eligible_any
    rep_hints_y &= rep_eligible_any
    return HwRegEligibility(
        eligible_x=rep_eligible_x,
        eligible_y=rep_eligible_y,
        hints_x=rep_hints_x,
        hints_y=rep_hints_y,
        use_count=rep_use_count,
        required_x=rep_required_x,
        required_y=rep_required_y,
    )
