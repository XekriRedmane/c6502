"""End-to-end harness for the chapter_15 valid corpus.

Each `tests/chapter_15/valid/<topic>/*.c` file is run through the full
pipeline (parse → resolve → tac → codegen). Files that exercise features
c6502 doesn't yet support are listed in `_EXPECTED_FAILURES_CODEGEN` —
each must currently raise; if any starts succeeding, the test fails so
we know to take it off the list. Files that can never pass under c6502's
narrow integer / soft-stack model live in `_INCOMPATIBLE`.

`big_array.c` and the kin in `_INCOMPATIBLE` use `long` literals beyond
c6502's 16-bit Long range or arrays larger than the 256-byte FP-relative
addressing window — chapter_15 was written against an 8-byte-long, large-
memory model. We skip these rather than expectedFailure them, because
there's no path to making them pass.
"""

import shutil
import unittest
from pathlib import Path

from compile import _run_stage
from preprocessor import preprocess


_TESTS_DIR = Path(__file__).parent
_C15_VALID = _TESTS_DIR / "chapter_15" / "valid"


# Permanently incompatible: literals out of c6502's 16-bit Long range,
# or frame too large for the soft stack's single-byte FP offsets.
# chapter_15 was written against an 8-byte-long, large-memory model;
# there's no path to making these compile under c6502's targets.
_INCOMPATIBLE = frozenset()


# Currently fail through `--codegen`. Each corresponds to a feature gap
# we plan to close. When a fix lands, drop the entry — the harness
# asserts each listed file still fails so it can't drift silently.
_EXPECTED_FAILURES_CODEGEN = frozenset()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter15Valid(unittest.TestCase):
    """Each chapter_15 valid file must compile through `--codegen`,
    except those listed in `_EXPECTED_FAILURES_CODEGEN` (must fail) or
    `_INCOMPATIBLE` (skipped)."""

    def test_codegen(self):
        files = sorted(_C15_VALID.rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_15 valid files found")

        for path in files:
            rel = str(path.relative_to(_C15_VALID))
            if rel in _INCOMPATIBLE:
                continue
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _EXPECTED_FAILURES_CODEGEN:
                    with self.assertRaises(
                        Exception,
                        msg=(
                            f"{rel} unexpectedly compiled — drop it from "
                            "_EXPECTED_FAILURES_CODEGEN"
                        ),
                    ):
                        _run_stage("codegen", source)
                else:
                    _run_stage("codegen", source)
