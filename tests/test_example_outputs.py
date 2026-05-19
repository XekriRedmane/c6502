"""Gold-output regression tests for examples/.

Each `examples/<name>.c` file ships with a checked-in
`examples/<name>.asm` showing its blessed `--optimize` output.
These tests recompile each example and assert that the fresh
output is byte-identical to the committed asm. The committed
files are the "gold" output — any pipeline change that alters them
must be deliberate and re-committed.

Why this matters: an optimization pass that's optimal in isolation
can interact badly with the rest of the pipeline — e.g., firing
too early and preempting a cheaper downstream lowering. Pure unit
tests of the pass-in-isolation can't catch that; only end-to-end
output checks can.

Concrete regression caught (commit 9137831): the
`recognize_indirect_indexed` pass was firing inside the TAC fixed-
point loop and preempting `recognize_indexed_store` on chains
whose pointer side was about to fold to a Constant. On
`paint_hud_strip_p1`, this regressed the output from 60 lines to
284 lines — a 4.7× blowup that pass-local unit tests didn't see.

Failure handling: when a test fails because an optimization
improved output, regenerate the committed file:

    uv run python compile.py examples/<name>.c \\
        --codegen --optimize -o examples/<name>.asm

and inspect the diff before committing the new gold output.
"""
from __future__ import annotations

import os
import shutil
import unittest

from compile import _run_stage
from preprocessor import preprocess


_EXAMPLES_DIR = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
    "examples",
)


def _examples() -> list[str]:
    """Names of all examples (without the .c suffix) that have
    both a .c source and a committed .asm output."""
    out: list[str] = []
    for entry in sorted(os.listdir(_EXAMPLES_DIR)):
        if not entry.endswith(".c"):
            continue
        base = entry[:-2]
        asm = os.path.join(_EXAMPLES_DIR, base + ".asm")
        if os.path.exists(asm):
            out.append(base)
    return out


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestExampleOutputs(unittest.TestCase):
    """Recompile each example with --optimize and compare against
    the committed .asm file. A failure means the pipeline produces
    different output than the gold output — either an improvement
    that needs to be blessed, or a regression."""

    def test_gold_outputs(self) -> None:
        examples = _examples()
        if not examples:
            self.skipTest("no examples found")
        for name in examples:
            with self.subTest(example=name):
                src = os.path.join(_EXAMPLES_DIR, name + ".c")
                gold = os.path.join(_EXAMPLES_DIR, name + ".asm")
                with open(src) as f:
                    source = f.read()
                actual = _run_stage(
                    "codegen", preprocess(source),
                    optimize=True,
                )
                with open(gold) as f:
                    expected = f.read()
                if actual != expected:
                    actual_lines = actual.count("\n")
                    expected_lines = expected.count("\n")
                    self.fail(
                        f"Gold output mismatch for {name} "
                        f"(expected {expected_lines} lines, got "
                        f"{actual_lines} lines). To bless the new "
                        f"output:\n  uv run python compile.py "
                        f"examples/{name}.c --codegen --optimize "
                        f"-o examples/{name}.asm"
                    )
