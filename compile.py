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

`--optimize` (orthogonal to the stage flag) runs the TAC-level
optimizer to a fixed point between TAC translation and the next
stage. Effective for `--tac` and `--codegen`; ignored otherwise.

`--optimize-asm` (mutually exclusive with `--optimize`) selects the
alternate optimization pipeline that runs TAC-level fixed-point
opts (the same four passes — constant folding, UCE, copy prop,
DSE) but defers register allocation to the asm-level. Phase 9
emits a bare `Return(value)` exit atom and skips
`FunctionPrologue` entirely; an asm-level SSA optimizer runs
byte-granular passes (DCE, etc.); regalloc colors byte-wide
nodes; a post-regalloc synthesis pass inserts the prologue /
epilogue based on what actually spilled. Step-by-step build —
intermediate steps may delegate part of the path to the existing
pipeline.

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
from passes.label_resolution import resolve_program as resolve_labels
from passes.long_branches import expand_program as expand_long_branches
from passes.loop_labeling import label_program as label_loops
from passes.optimization import optimize_program as optimize_tac
from passes.optimization_asm import optimizer as asm_opt
from passes.prologue_synthesis import synthesize_program as synthesize_prologue
from passes.replace_pseudoregisters import (
    replace_program as replace_pseudoregs,
    replace_program_bare_exit as replace_pseudoregs_bare_exit,
)
from passes.identifier_resolution import resolve_program as resolve_identifiers
from passes.string_lifting import lift_program as lift_strings
from passes.type_checking import (
    StaticAttr,
    check_program as type_check_program,
)
from c99_to_tac import translate_program as translate_to_tac


def _resolved(source: str):
    """Run parse + name resolution + string lifting. Order matters:
      1. parse — c99 AST
      2. identifier_resolution — user names get unique
         `@N.<orig>` rewrites; string literals pass through.
      3. string_lifting — every non-direct-array-init String
         becomes a `Var(.str@N)` referring to a fresh file-scope
         static (prepended to the program's declaration list).
         Runs AFTER identifier_resolution so the lifted names use
         a disjoint character (`.`) and don't get re-renamed.
      4. label_resolution — user `goto` labels mangle to
         `.<funcname>@<orig>`.
      5. loop_labeling — iteration / switch / case / default
         labels get `.loop@<N>` etc.
    """
    return label_loops(resolve_labels(lift_strings(
        resolve_identifiers(parse(source)),
    )))


def _format_tokens(source: str) -> str:
    out: list[str] = []
    for tok in tokenize(source):
        out.append(f"{tok.line}:{tok.col}\t{tok.kind.value}\t{tok.value}\n")
    return "".join(out)


def _run_stage(
    stage: str, source: str, optimize: bool = False,
    optimize_asm: bool = False,
) -> str:
    if stage == "lex":
        return _format_tokens(source)
    if stage == "parse":
        return pretty(parse(source)) + "\n"
    if stage == "resolve":
        return pretty(_resolved(source)) + "\n"
    if stage == "tac":
        # Thread the symbol + type tables from type_checking into
        # c99_to_tac so the latter can read function-linkage flags,
        # emit StaticVariable entries for static-storage objects,
        # and resolve struct/union sizes.
        prog, symbols, types = type_check_program(_resolved(source))
        tac = translate_to_tac(prog, symbols, types)
        if optimize or optimize_asm:
            # optimize_tac returns (prog, colorings); we're stopping
            # before codegen here so the colorings are discarded.
            tac, _ = optimize_tac(tac, symbols)
        return pretty(tac) + "\n"
    if stage == "codegen":
        prog, symbols, types = type_check_program(_resolved(source))
        # `replace_pseudoregisters` needs to recognize every static-
        # storage object — including extern references that don't
        # produce a StaticVariable definition here — to avoid
        # mistaking their Pseudos for locals. Any StaticAttr entry in
        # the symbol table is a static-storage object; pass the full
        # set as `extra_statics` so the asm pass picks up the externs
        # the asm program doesn't otherwise know about.
        statics = frozenset(
            name for name, sym in symbols.items()
            if isinstance(sym.attrs, StaticAttr)
        )
        tac = translate_to_tac(prog, symbols, types)
        # `colorings` is the per-function register-allocation result
        # produced by the optimizer (empty when --optimize is off).
        # `replace_pseudoregisters` consumes it to lower colored
        # Pseudos to ZP operands; uncolored / spilled / address-
        # taken names continue to flow through the existing Frame
        # path.
        colorings: dict = {}
        if optimize:
            tac, colorings = optimize_tac(tac, symbols)
        if optimize_asm:
            # Asm-level pipeline: TAC opts WITHOUT regalloc, then
            # tac_to_asm in bare-exit mode (Pseudos preserved), then
            # asm-level SSA round-trip (step 5e: no opts between
            # to_ssa and from_ssa yet — steps 6 and 7 will populate),
            # then replace_pseudoregisters_bare_exit (no colorings —
            # asm-level regalloc isn't done yet, so all Pseudos go
            # to Frame), then synthesize_prologue.
            tac, _ = optimize_tac(tac, symbols, do_regalloc=False)
            asm0 = translate_to_asm(
                tac, symbols, types, bare_exit=True,
            )
            asm0 = asm_opt.optimize_program(
                asm0, extra_statics=statics,
            )
            asm1, dims_by_fn = replace_pseudoregs_bare_exit(
                asm0, extra_statics=statics, symbols=symbols,
                types=types, colorings={},
            )
            asm2 = synthesize_prologue(asm1, dims_by_fn)
            return emit_program(lower_to_asm2(
                expand_long_branches(asm2),
            ))
        return emit_program(lower_to_asm2(expand_long_branches(replace_pseudoregs(
            translate_to_asm(tac, symbols, types),
            extra_statics=statics,
            symbols=symbols,
            types=types,
            colorings=colorings,
        ))))
    raise AssertionError(f"unknown stage: {stage!r}")


def main(argv: list[str]) -> int:
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
    opt_group = ap.add_mutually_exclusive_group()
    opt_group.add_argument(
        "--optimize", dest="optimize", action="store_true",
        help="run TAC-level optimization passes (constant "
             "folding, unreachable-code elimination, copy "
             "propagation, dead-store elimination) to a "
             "fixed point, then SSA-bracketed register "
             "allocation. Applies to --tac and --codegen.",
    )
    opt_group.add_argument(
        "--optimize-asm", dest="optimize_asm", action="store_true",
        help="alternate pipeline: TAC fixed-point opts, then "
             "asm-level SSA opts (byte-granular DCE, peepholes, "
             "etc.), regalloc on byte-wide nodes, and late "
             "prologue / epilogue synthesis driven by what "
             "actually spilled. Mutually exclusive with "
             "--optimize. Applies to --tac and --codegen.",
    )
    args, pcpp_args = ap.parse_known_args(argv[1:])

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
        optimize=args.optimize,
        optimize_asm=args.optimize_asm,
    )

    if args.output is not None:
        with open(args.output, "w", encoding="utf-8") as f:
            f.write(text)
    else:
        sys.stdout.write(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
