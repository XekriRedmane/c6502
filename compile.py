"""Top-level c6502 compiler driver.

Runs the preprocessor (preprocessor.preprocess, our wrapper around the
pcpp library), then continues the pipeline up to the stage requested by
exactly one of:

  --lex      stop after tokenization; one `line:col<tab>kind<tab>value`
             line per token
  --parse    stop after parsing; pretty-print the c99_ast tree
  --resolve  stop after name resolution (variable resolution, label
             resolution, then loop labeling); pretty-print the
             rewritten c99_ast (user variables -> `@N.orig`, labels ->
             `@<funcname>.<orig>`, loops -> `.loop@N`)
  --tac      stop after TAC translation; pretty-print the tac_ast tree
  --codegen  go all the way to 6502 assembly text

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

from passes.allocate_stack import allocate_program as allocate_stack
from asm_emit import emit_program
from tac_to_asm import translate_program as translate_to_asm
from lexer import tokenize
from parser import parse
from preprocessor import preprocess
from pretty import pretty
from passes.label_resolution import resolve_program as resolve_labels
from passes.loop_labeling import label_program as label_loops
from passes.replace_pseudoregisters import replace_program as replace_pseudoregs
from passes.variable_resolution import resolve_program as resolve_variables
from c99_to_tac import translate_program as translate_to_tac


def _format_tokens(source: str) -> str:
    out: list[str] = []
    for tok in tokenize(source):
        out.append(f"{tok.line}:{tok.col}\t{tok.kind.value}\t{tok.value}\n")
    return "".join(out)


def _run_stage(stage: str, source: str) -> str:
    if stage == "lex":
        return _format_tokens(source)
    if stage == "parse":
        return pretty(parse(source)) + "\n"
    if stage == "resolve":
        return pretty(label_loops(
            resolve_labels(resolve_variables(parse(source)))
        )) + "\n"
    if stage == "tac":
        return pretty(translate_to_tac(label_loops(
            resolve_labels(resolve_variables(parse(source)))
        ))) + "\n"
    if stage == "codegen":
        return emit_program(allocate_stack(replace_pseudoregs(
            translate_to_asm(translate_to_tac(label_loops(
                resolve_labels(resolve_variables(parse(source)))
            )))
        )))
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

    text = _run_stage(args.stage, preprocess(source, pcpp_args))

    if args.output is not None:
        with open(args.output, "w", encoding="utf-8") as f:
            f.write(text)
    else:
        sys.stdout.write(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
