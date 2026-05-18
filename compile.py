"""Top-level c6502 compiler driver.

Runs the preprocessor (preprocessor.preprocess, our wrapper around the
pcpp library), then continues the pipeline up to the stage requested by
exactly one of:

  --lex      stop after tokenization; one `line:col<tab>kind<tab>value`
             line per token
  --parse    stop after parsing; pretty-print the c99_ast tree
  --resolve  stop after name resolution (identifier resolution, label
             resolution, then loop labeling); pretty-print the
             rewritten c99_ast (user variables -> `@N.orig`, labels ->
             `@<funcname>.<orig>`, loops -> `.loop@N`). Type checking
             is *not* run for this stage so the rewritten AST surfaces
             cleanly even when the program would later be rejected by
             the type checker.
  --tac      stop after TAC translation; pretty-print the tac_ast tree.
             Type checking runs first.
  --codegen  go all the way to 6502 assembly text. Type checking
             runs first.

`--optimize` (orthogonal to the stage flag) runs the optimizer
pipeline: TAC-level fixed-point opts (constant folding, strength
reduction, comparison-against-zero / jump fold, UCE, copy prop,
DSE) followed by the asm-level SSA round-trip with byte-granular
copy-prop / backward copy-prop / DCE, byte-granular regalloc, and
late prologue / epilogue synthesis. Effective for `--tac` and
`--codegen`. Also enables the `__attribute__((zp_abi))` calling-
convention optimization (frame elimination on annotated leaves).

Output goes to stdout by default, or to the file named by `-o`. With
`--codegen`, the output file (if any) must have a `.asm` suffix.

Any flag not recognized by this driver is forwarded to the preprocessor.
That includes the full pcpp command-line surface (`-D`, `-U`, `-N`, `-I`,
`--passthru-*`, `--line-directive`, etc.) — see `preprocessor.py`. pcpp's
own `-o` is not forwarded; this driver's `-o` is for the final output.
"""

from __future__ import annotations

import argparse
import sys

from asm_emit import emit_program
from tac_to_asm import translate_program as translate_to_asm
from lexer import tokenize
from parser import parse
from preprocessor import preprocess
from pretty import pretty
from passes.asm_to_asm2 import translate_program as lower_to_asm2
from passes.direct_index_load import apply_direct_index_load
from passes.dead_pha_pla import apply_dead_pha_pla
from passes.asm_dead_store import apply_asm_dead_store
from passes.inc_peephole import apply_inc_peephole
from passes.dec_peephole import apply_dec_peephole
from passes.branch_invert import apply_branch_invert
from passes.const_arith_fold import apply_const_arith_fold
from passes.mem_const_prop import apply_mem_const_prop
from passes.round_trip_load import apply_round_trip_load_drop
from passes.and_sign_bit_branch import apply_and_sign_bit_branch
from passes.self_store_drop import apply_self_store_drop
from passes.adc_commute import apply_adc_commute
from passes.dual_index_promotion import apply_dual_index_promotion
from passes.prune_unused_slots import prune_unused_locals
from passes.cmp_sbc_fusion import apply_cmp_sbc_fusion
from passes.dec_inc_branch_fold import apply_dec_inc_branch_fold
from passes.tail_call import apply_tail_call
from passes.loop_counter_to_x import apply_loop_counter_to_x
from passes.x_save_slot_load import apply_x_save_slot_load
from passes.asm_licm import apply_licm
from passes.sub1_test_zero_peephole import apply_sub1_test_zero_peephole
from passes.cpx_cpy_peephole import apply_cpx_cpy_peephole
from passes.dead_a_arith import apply_dead_a_arith_elimination
from passes.indirect_base_prop import apply_indirect_base_prop
from passes.redundant_load import apply_redundant_load_elimination
from passes.redundant_load_after_rmw import (
    apply_redundant_load_after_rmw,
)
from passes.redundant_store import apply_redundant_store_elimination
from passes.asm_remat import apply_remat
from passes.label_resolution import resolve_program as resolve_labels
from passes.long_branches import expand_program as expand_long_branches
from passes.loop_labeling import label_program as label_loops
from passes.abi_selection import select_abi
from passes.function_local_sizing import (
    compute_local_bytes, compute_address_taken_bytes,
)
from passes.address_taken_zp import (
    compute_address_taken_assignments, slot_symbols as _addr_taken_slot_symbols,
)
from passes.zp_link_metadata import build_metadata, format_metadata
from passes.zp_local_allocation import (
    allocate_function_locals, build_local_slot_symbols,
)
from passes.zp_slot_allocation import allocate_zp_slots
from passes.optimization import optimize_program as optimize_tac
from passes.optimization.dispatch_pointer_array import (
    dispatch_const_pointer_arrays,
)
from passes.optimization_asm import optimizer as asm_opt
from passes.prologue_synthesis import synthesize_program as synthesize_prologue
from passes.replace_pseudoregisters import (
    replace_program as replace_pseudoregs,
    replace_program_bare_exit as replace_pseudoregs_bare_exit,
)
from passes.identifier_resolution import resolve_program as resolve_identifiers
from passes.optimization_ast.unroll import unroll_program
from passes.string_lifting import lift_program as lift_strings
from passes.type_checking import (
    StaticAttr,
    check_program as type_check_program,
)
from c99_to_tac import translate_program as translate_to_tac


def _resolved(source: str, *, unroll: bool = False):
    """Run parse + (optional unroll) + name resolution + string
    lifting. Order matters:
      1. parse — c99 AST
      2. (optional) unroll — when --optimize --unroll is set, every
         for-loop carrying `#pragma c6502 loop unroll(enable)` is
         fully unrolled here, BEFORE identifier_resolution, so each
         cloned body's locals get fresh per-iteration `@N.<name>`
         rewrites for free.
      3. identifier_resolution — user names get unique
         `@N.<orig>` rewrites; string literals pass through.
      4. string_lifting — every non-direct-array-init String
         becomes a `Var(.str@N)` referring to a fresh file-scope
         static (prepended to the program's declaration list).
         Runs AFTER identifier_resolution so the lifted names use
         a disjoint character (`.`) and don't get re-renamed.
      5. label_resolution — user `goto` labels mangle to
         `.<funcname>@<orig>`.
      6. loop_labeling — iteration / switch / case / default
         labels get `.loop@<N>` etc.
    """
    parsed = parse(source)
    if unroll:
        parsed = unroll_program(parsed)
    resolved = resolve_identifiers(parsed)
    lifted = lift_strings(resolved)
    label_resolved = resolve_labels(lifted)
    return label_loops(label_resolved)


# Defensive cap on the asm-peephole fixed-point loop. None of the
# three peepholes can grow a program — they only delete or fuse —
# so the iteration count is bounded by the number of deletable
# instructions. The cap exists to surface bugs (an unsound pass
# making a no-op rewrite that the equality check classifies as a
# change) rather than to constrain real workloads.
_PEEPHOLE_FIXEDPOINT_CAP = 16


def _peephole_fixedpoint(prog, *, zp_slot_symbols=None):
    """Run apply_inc_peephole → apply_dec_peephole → apply_direct_
    index_load → apply_redundant_load_elimination →
    apply_redundant_store_elimination in sequence, repeating until
    a full sweep produces no further change. Each pass can enable
    the next: `inc_peephole` / `dec_peephole` may shorten chains
    that `direct_index_load` then collapses; `direct_index_load`'s
    rewrite of `LDA M; TAX` to `LDX M` exposes redundant `LDX M`
    loads downstream; `redundant_load`'s deletions can leave new
    `LDA; TAX` pairs adjacent. `redundant_store` catches memory-
    to-memory transfer redundancies (e.g. repeated DPTR staging
    in an unrolled body) that `redundant_load`'s A-tracking can't
    see across an intervening A clobber. Order: inc/dec → direct →
    redundant_load → redundant_store matches the natural enabling
    chain. `redundant_load_after_rmw` runs after dec/inc — it
    needs the rmw form to exist to recognize its pattern."""
    for _ in range(_PEEPHOLE_FIXEDPOINT_CAP):
        new_prog = apply_inc_peephole(prog)
        new_prog = apply_dec_peephole(new_prog)
        new_prog = apply_sub1_test_zero_peephole(new_prog)
        new_prog = apply_direct_index_load(new_prog)
        new_prog = apply_dead_pha_pla(new_prog)
        new_prog = apply_cpx_cpy_peephole(new_prog)
        new_prog = apply_indirect_base_prop(
            new_prog, zp_symbol_addrs=zp_slot_symbols,
        )
        new_prog = apply_redundant_load_after_rmw(new_prog)
        new_prog = apply_redundant_load_elimination(new_prog)
        new_prog = apply_redundant_store_elimination(new_prog)
        new_prog = apply_remat(new_prog, zp_slot_symbols=zp_slot_symbols)
        new_prog = apply_asm_dead_store(
            new_prog, zp_slot_symbols=zp_slot_symbols,
        )
        new_prog = apply_dead_a_arith_elimination(new_prog)
        new_prog = apply_branch_invert(new_prog)
        new_prog = apply_mem_const_prop(new_prog)
        new_prog = apply_const_arith_fold(new_prog)
        new_prog = apply_round_trip_load_drop(new_prog)
        new_prog = apply_and_sign_bit_branch(new_prog)
        new_prog = apply_self_store_drop(new_prog)
        new_prog = apply_adc_commute(new_prog)
        new_prog = apply_cmp_sbc_fusion(new_prog)
        new_prog = apply_dec_inc_branch_fold(new_prog)
        new_prog = apply_tail_call(new_prog)
        if new_prog == prog:
            return new_prog
        prog = new_prog
    raise AssertionError(
        f"asm peephole fixed-point loop didn't converge in "
        f"{_PEEPHOLE_FIXEDPOINT_CAP} iterations — a peephole pass "
        "is reporting changes without actually modifying the IR.",
    )


def _format_tokens(source: str) -> str:
    out: list[str] = []
    for tok in tokenize(source):
        out.append(f"{tok.line}:{tok.col}\t{tok.kind.value}\t{tok.value}\n")
    return "".join(out)


def _run_stage(
    stage: str, source: str, optimize: bool = False, unroll: bool = False,
) -> str:
    if stage == "lex":
        return _format_tokens(source)
    if stage == "parse":
        return pretty(parse(source)) + "\n"
    if stage == "resolve":
        return pretty(_resolved(source, unroll=unroll)) + "\n"
    if stage == "tac":
        # Thread the symbol + type tables from type_checking into
        # c99_to_tac so the latter can read function-linkage flags,
        # emit StaticVariable entries for static-storage objects,
        # and resolve struct/union sizes.
        prog, symbols, types = type_check_program(
            _resolved(source, unroll=unroll),
        )
        tac = translate_to_tac(prog, symbols, types)
        if optimize:
            tac = optimize_tac(tac, symbols)
        return pretty(tac) + "\n"
    if stage == "codegen":
        prog, symbols, types = type_check_program(
            _resolved(source, unroll=unroll),
        )
        # `replace_pseudoregisters` needs to recognize every static-
        # storage object — including extern references that don't
        # produce a StaticVariable definition here — to avoid
        # mistaking their Pseudos for locals. Any StaticAttr entry
        # in the symbol table is a static-storage object; pass the
        # full set as `extra_statics` so the asm pass picks up the
        # externs the asm program doesn't otherwise know about.
        statics = frozenset(
            name for name, sym in symbols.items()
            if isinstance(sym.attrs, StaticAttr)
        )
        tac = translate_to_tac(prog, symbols, types)
        if optimize:
            # Optimized pipeline: TAC opts (no regalloc) → tac_to_asm
            # in bare-exit mode (Pseudos preserved) → asm-level SSA
            # round-trip with byte-granular copy-prop / DCE / regalloc
            # → replace_pseudoregisters_bare_exit (consumes the asm
            # regalloc colorings AND the per-function ParamLayouts so
            # ZP-ABI params resolve to ZP operands) →
            # synthesize_prologue (collapses prologue/epilogue when
            # nothing needs spilling).
            tac = optimize_tac(tac, symbols)
            # Inline-switch dispatch for small const-pointer arrays.
            # Recognizes `arr[i][j]` where arr is a `static const T
            # * const[N]` and rewrites the indirect chain to a
            # CMP/BEQ dispatch on `i`. Runs post-from_ssa (inside
            # optimize_tac), so the dispatched cases can each
            # write to the same dst without Phi insertion.
            tac = dispatch_const_pointer_arrays(tac, symbols)
            abi = select_abi(tac, prog, types)
            abi, zp_slot_symbols = allocate_zp_slots(tac, abi)
            asm0 = translate_to_asm(
                tac, symbols, types, bare_exit=True, abi=abi,
            )
            # Preliminary optimizer pass with the default
            # caller/callee partition. Used only to size each
            # function's local-byte demand; the IR it produces is
            # discarded.
            asm_prelim, _ = asm_opt.optimize_program(
                asm0, extra_statics=statics, param_layouts=abi,
                symbols=symbols,
            )
            local_bytes = compute_local_bytes(asm_prelim)
            # Address-taken locals would otherwise spill to the
            # soft-stack frame (forcing a prologue/epilogue + an
            # LDY/(FP),Y indirect-read at every access). For
            # functions with a private local pool, we can reserve
            # extra ZP bytes in the pool and route each address-
            # taken local to a stable ZP byte — `&local` then
            # becomes a 2-byte immediate constant instead of a
            # 6-byte FP-relative compute. Add the byte demand here
            # so `allocate_function_locals` sizes the pool
            # appropriately.
            addr_taken_bytes = compute_address_taken_bytes(
                asm_prelim, symbols, types,
            )
            combined_local_bytes = {
                fn: local_bytes.get(fn, 0) + addr_taken_bytes.get(fn, 0)
                for fn in set(local_bytes) | set(addr_taken_bytes)
            }
            # Call-graph-disjoint private pools per function. The
            # asm regalloc draws from these for eligible functions;
            # ineligible functions (recursive, indirect-calling,
            # or with non-zp_abi extern callees) fall back to the
            # conservative pool.
            local_pools = allocate_function_locals(
                tac, abi, combined_local_bytes,
            )
            # Final optimizer pass with private pools threaded
            # through to the regalloc.
            asm0, asm_colorings = asm_opt.optimize_program(
                asm0, extra_statics=statics, param_layouts=abi,
                symbols=symbols, local_pools=local_pools,
            )
            # Compute per-function address-taken Pseudo → ZP-byte
            # assignments from the pool addresses the regalloc
            # didn't claim. Names that don't fit (no contiguous
            # run of the required width is free) get omitted and
            # fall back to the Frame path inside
            # `replace_pseudoregisters`.
            addr_taken_assignments = compute_address_taken_assignments(
                asm0, local_pools, asm_colorings, symbols, types,
            )
            addr_taken_symbols = {
                fn: _addr_taken_slot_symbols(
                    fn, assignments, symbols, types,
                )
                for fn, assignments in addr_taken_assignments.items()
            }
            asm1, dims_by_fn = replace_pseudoregs_bare_exit(
                asm0, extra_statics=statics, symbols=symbols,
                types=types, colorings=asm_colorings,
                param_layouts=abi, local_pools=local_pools,
                address_taken_assignments=addr_taken_assignments,
                address_taken_symbols=addr_taken_symbols,
            )
            asm2 = synthesize_prologue(asm1, dims_by_fn)
            # LICM-lite: hoist loop-invariant constant stores out
            # of natural loops (only fires when the loop body has
            # no Call — conservative to sidestep zp_abi clobber
            # questions).
            asm2 = apply_licm(asm2)
            # Build the full EQU table: zp_abi param slots plus the
            # per-function body-local slot symbols emitted by
            # apply_coloring. The asm IR references body locals as
            # `Data(__local_<fn>__<source>[_<byte>], 0)`, so the
            # emit needs an `EQU` binding for every symbol the IR
            # uses. We compute the per-pool-byte ordered name list
            # once and thread it into both the EQU builder and the
            # link-metadata builder so both share the same view.
            from passes.zp_slot_naming import compute_local_slot_names
            slot_names_by_fn = compute_local_slot_names(
                local_pools, asm_colorings,
                address_taken_assignments=addr_taken_assignments,
                address_taken_symbols=addr_taken_symbols,
            )
            local_slot_symbols = build_local_slot_symbols(
                local_pools, slot_names_by_fn=slot_names_by_fn,
            )
            all_slot_symbols = {**zp_slot_symbols, **local_slot_symbols}
            asm3 = _peephole_fixedpoint(asm2, zp_slot_symbols=all_slot_symbols)
            asm3 = apply_loop_counter_to_x(asm3)
            # Catch cases the X-pivot promotion couldn't reach: a
            # Pseudo that got X-colored upstream but kept an
            # `LDA M`-style read of its spill home M for arg
            # passing, which reads stale M after DEX. Rewrite those
            # LDA M to TXA. Runs before the post-promotion peephole
            # fixedpoint so the now-dead STX M / LDX M wraps get
            # cleaned up downstream.
            asm3 = apply_x_save_slot_load(asm3)
            # Second pass of peephole catches what the promotion
            # exposed (e.g., redundant LDX/STX pairs after the
            # Y-pivot embedded in loop_counter_to_x freed up X).
            asm3 = _peephole_fixedpoint(asm3, zp_slot_symbols=all_slot_symbols)
            # Y-promote a Data symbol that's loaded into X multiple
            # times when Y is otherwise unused — saves the
            # per-occurrence `LDX abs` reload by loading once into
            # Y at function entry. Re-run the peephole fixedpoint
            # so dead LDA/STA pairs around the dropped LDX get
            # collected.
            asm3 = apply_dual_index_promotion(asm3)
            asm3 = _peephole_fixedpoint(asm3, zp_slot_symbols=all_slot_symbols)
            asm4 = expand_long_branches(asm3)
            # Drop EQU entries for `__local_*` symbols that the
            # peephole catalog rendered unreferenced. Walks asm_ast
            # operands directly, so run BEFORE asm_to_asm2 (which
            # re-tags instructions at asm2_ast types). Cosmetic only —
            # the bytes are still reserved by the function's
            # local_bytes count.
            emit_slot_symbols = prune_unused_locals(
                asm4, all_slot_symbols,
            )
            asm5 = lower_to_asm2(asm4)
            link_meta = build_metadata(
                tac, abi, local_pools,
                slot_names_by_fn=slot_names_by_fn,
            )
            return emit_program(
                asm5, zp_slot_symbols=emit_slot_symbols,
                link_metadata_lines=format_metadata(link_meta),
            )
        asm0 = translate_to_asm(tac, symbols, types)
        asm1 = replace_pseudoregs(
            asm0, extra_statics=statics, symbols=symbols, types=types,
        )
        asm2 = _peephole_fixedpoint(asm1)
        asm3 = expand_long_branches(asm2)
        asm4 = lower_to_asm2(asm3)
        return emit_program(asm4)
    raise AssertionError(f"unknown stage: {stage!r}")


def main(argv: list[str]) -> int:
    # `--link` is the multi-TU linker mode: positional args are
    # one-or-more .asm files (each from a per-TU
    # `compile.py --codegen --optimize`) and the output is a
    # single .asm with all `__zpabi_*` / `__local_*` symbols
    # re-allocated globally. Eats a different argument shape
    # than the single-TU compile modes, so we detect it before
    # the main argparse setup.
    if "--link" in argv[1:]:
        return _main_link(argv)
    ap = argparse.ArgumentParser(prog="compile.py")
    ap.add_argument("input", help="C source file, or - for stdin")
    ap.add_argument("-o", dest="output",
                    help="output file (default: stdout)")
    stages = ap.add_mutually_exclusive_group(required=True)
    stages.add_argument("--lex", dest="stage", action="store_const",
                        const="lex", help="stop after tokenization")
    stages.add_argument("--parse", dest="stage", action="store_const",
                        const="parse", help="stop after parsing")
    stages.add_argument("--resolve", dest="stage", action="store_const",
                        const="resolve",
                        help="stop after variable + label resolution + "
                             "loop labeling")
    stages.add_argument("--tac", dest="stage", action="store_const",
                        const="tac", help="stop after TAC translation")
    stages.add_argument("--codegen", dest="stage", action="store_const",
                        const="codegen", help="emit 6502 assembly")
    ap.add_argument(
        "--optimize", dest="optimize", action="store_true",
        help="run the optimizer pipeline: TAC-level fixed-point opts "
             "(constant folding, strength reduction, comparison-"
             "against-zero / jump fold, UCE, copy propagation, dead-"
             "store elimination), then asm-level SSA round-trip "
             "(byte-granular forward + backward copy-prop, byte-DCE, "
             "byte-granular regalloc), then late prologue / epilogue "
             "synthesis. Also enables the `__attribute__((zp_abi))` "
             "calling-convention optimization. Applies to --tac and "
             "--codegen.",
    )
    ap.add_argument(
        "--unroll", dest="unroll", action="store_true",
        help="fully unroll every for-loop carrying `#pragma c6502 "
             "loop unroll(enable)`. Requires --optimize. Applies to "
             "--resolve, --tac, and --codegen.",
    )
    args, pcpp_args = ap.parse_known_args(argv[1:])

    if args.unroll and not args.optimize:
        print(
            "compile.py: --unroll requires --optimize",
            file=sys.stderr,
        )
        return 2

    if (args.stage == "codegen"
            and args.output is not None
            and not args.output.endswith(".asm")):
        print(
            f"compile.py: --codegen output must have .asm suffix: {args.output}",
            file=sys.stderr,
        )
        return 2

    if args.input == "-":
        source = sys.stdin.read()
    else:
        with open(args.input, "r", encoding="utf-8") as f:
            source = f.read()

    text = _run_stage(
        args.stage, preprocess(source, pcpp_args),
        optimize=args.optimize, unroll=args.unroll,
    )

    if args.output is not None:
        with open(args.output, "w", encoding="utf-8") as f:
            f.write(text)
    else:
        sys.stdout.write(text)
    return 0


def _main_link(argv: list[str]) -> int:
    """Argparse path for `--link`. Positional args are one or
    more `.asm` files; `-o` is the combined output."""
    ap = argparse.ArgumentParser(
        prog="compile.py --link",
        description=(
            "Re-allocate __zpabi_* and __local_* symbols across "
            "multiple .asm files and emit a single combined .asm "
            "with one global EQU block."
        ),
    )
    ap.add_argument(
        "inputs", nargs="+",
        help="input .asm files (each from `compile.py --codegen "
             "--optimize`)",
    )
    ap.add_argument("-o", dest="output", required=True,
                    help="combined output .asm")
    ap.add_argument(
        "--link", action="store_true",
        help=argparse.SUPPRESS,  # already detected by main
    )
    args = ap.parse_args(argv[1:])
    if not args.output.endswith(".asm"):
        print(
            f"compile.py: --link output must have .asm suffix: "
            f"{args.output}",
            file=sys.stderr,
        )
        return 2
    from passes.linker import LinkError, link_files
    try:
        link_files(args.inputs, args.output)
    except LinkError as e:
        print(f"compile.py: link error: {e}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
