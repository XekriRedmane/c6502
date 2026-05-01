"""End-to-end harness for the chapter_19 corpus.

chapter_19 is upstream's optimization chapter. c6502 doesn't
implement TAC-level optimization yet, but the corpus is useful
*now* because each program has a deterministic expected return —
listed in upstream's `expected_results.json` (vendored as
`tests/chapter_19/expected_results.json`). Running each program
through parse → identifier_resolution → string_lifting →
label_resolution → loop_labeling → type_checking → c99_to_tac →
simulator must produce that same value. When an optimizer lands
and is plugged in just before the simulator stage, every file's
return must stay the same — divergence is a semantics-breaking
miscompile.

Because c6502's integers are narrower than the upstream reference
compiler's (`int` is 1 byte, `long` is 2 bytes, `long long` is 4
bytes vs upstream's 4 / 8 / 8), some programs need adaptation to
produce the same observable answer:

- Programs whose arithmetic relied on 32-bit `int` widths get
  their `int` declarations rewritten to `long long` (c6502's
  4-byte type); literals get `LL` suffixes where multi-operand
  expressions need to happen at 4-byte width and would otherwise
  promote to c6502's 2-byte `long`.
- A few programs need targeted rewrites — e.g.
  `whole_pipeline/all_types/integer_promotions.c` adds an
  explicit `(long long)` cast, because c6502 promotes `char` →
  1-byte `int` (per its narrow-integer model), so `c1 + c1`
  overflows where the upstream compiler's 4-byte `int` promotion
  doesn't.
- `copy_propagation/all_types/extra_credit/pointer_incr.c`
  replaces `ptr++` with `ptr = ptr + 1` to work around a c6502
  bug: postfix / prefix `++` on a pointer-to-array currently
  scales by `sizeof(elem)` instead of `sizeof(*ptr)`. The
  equivalent additive form `ptr + 1` goes through
  `translate_pointer_arithmetic` which scales correctly. A
  comment in the file flags the workaround.

Adaptations preserve the upstream-expected return value, so
once the bug is fixed and / or wider integer types are made
default, the workarounds can be unwound without touching the
expected-return table.

Files that can't be evaluated through the simulator are pinned
in two skip sets:

- `_PARSE_FAILURES`: oversized literals exceeding c6502's widest
  type (`unsigned long long`, 2^32 - 1). The parser rejects
  these at lex / parse time per C99 §6.4.4.1's "doesn't fit any
  supported type" rule.
- `_NEEDS_LIBC`: files calling into libc (`putchar`, `copysign`,
  `double_isnan`) or a multi-TU helper (`exit_wrapper`, defined
  in `helper_libs/exit.c`). The TAC simulator can't execute
  external code.

`helper_libs/exit.c` is skipped unconditionally — it has no
`main`, only the `exit_wrapper` helper consumed by other files.
"""
from __future__ import annotations

import json
import shutil
import unittest
from pathlib import Path

from c99_to_tac import translate_program as translate_to_tac
from parser import ParserError, parse
from passes.identifier_resolution import resolve_program as resolve_identifiers
from passes.label_resolution import resolve_program as resolve_labels
from passes.loop_labeling import label_program as label_loops
from passes.string_lifting import lift_program as lift_strings
from passes.type_checking import check_program as type_check_program
from preprocessor import preprocess
from tac_sim import Simulator


_TESTS_DIR = Path(__file__).parent
_C19 = _TESTS_DIR / "chapter_19"


# Vendored from upstream's `expected_results.json` (filtered to
# chapter_19 entries). Source of truth for the per-file
# expected `main()` return code; any optimization pass that
# changes one of these is a miscompile.
_UPSTREAM_EXPECTED: dict[str, int] = {
    rel: entry["return_code"]
    for rel, entry in json.loads(
        (_C19 / "expected_results.json").read_text()
    ).items()
    if "return_code" in entry
}


# Files whose oversized integer literals (> 2^32 - 1) the parser
# rejects per C99 §6.4.4.1: every type in the candidate list
# fails the value-fits check, so it raises "doesn't fit any
# supported type". c6502's widest integer is unsigned long long
# (4 bytes); upstream uses 8-byte `long` / `long long`, so any
# value > 4_294_967_295 lex-time rejects here.
_PARSE_FAILURES: frozenset[str] = frozenset({
    "constant_folding/all_types/extra_credit/fold_bitwise_long.c",
    "constant_folding/all_types/extra_credit/fold_bitwise_unsigned.c",
    "constant_folding/all_types/fold_cast_from_double.c",
    "constant_folding/all_types/fold_cast_to_double.c",
    "constant_folding/all_types/fold_extensions_and_copies.c",
    "constant_folding/all_types/fold_long.c",
    "constant_folding/all_types/fold_truncate.c",
    "constant_folding/all_types/fold_ulong.c",
    "copy_propagation/all_types/propagate_all_types.c",
    "dead_store_elimination/all_types/dont_elim/recognize_all_uses.c",
    "whole_pipeline/all_types/extra_credit/fold_compound_assign_all_types.c",
    "whole_pipeline/all_types/extra_credit/fold_compound_bitwise_assign_all_types.c",
    "whole_pipeline/all_types/extra_credit/fold_negative_long_bitshift.c",
    "whole_pipeline/all_types/fold_cast_from_double.c",
    "whole_pipeline/all_types/fold_cast_to_double.c",
    "whole_pipeline/all_types/fold_extension_and_truncation.c",
    "whole_pipeline/all_types/fold_negative_values.c",
    "whole_pipeline/all_types/signed_unsigned_conversion.c",
})


# Files that call into libc or a multi-TU helper. The TAC
# simulator can't execute external code, so these are pinned
# until either (a) the simulator gains stubs for the relevant
# functions, or (b) we wire multi-TU compilation and link the
# helper translation unit.
_NEEDS_LIBC: dict[str, str] = {
    "constant_folding/all_types/extra_credit/fold_nan.c":
        "calls libc double_isnan",
    "constant_folding/all_types/extra_credit/return_nan.c":
        "calls libc double_isnan",
    "copy_propagation/all_types/char_type_conversion.c":
        "calls libc putchar",
    "copy_propagation/all_types/extra_credit/redundant_nan_copy.c":
        "calls libc double_isnan",
    "copy_propagation/int_only/dont_propagate/source_killed_on_one_path.c":
        "calls libc putchar",
    "dead_store_elimination/int_only/dont_elim/add_all_to_worklist.c":
        "calls libc putchar",
    "dead_store_elimination/int_only/dont_elim/dont_remove_funcall.c":
        "calls libc putchar",
    "dead_store_elimination/int_only/dont_elim/nested_loops.c":
        "calls libc putchar",
    "dead_store_elimination/int_only/loop_dead_store.c":
        "calls libc putchar",
    "dead_store_elimination/int_only/static_not_always_live.c":
        "calls exit_wrapper from helper_libs/exit.c (multi-TU)",
    "unreachable_code_elimination/infinite_loop.c":
        "calls exit_wrapper from helper_libs/exit.c (multi-TU)",
    "whole_pipeline/all_types/alias_analysis_change.c":
        "calls libc putchar",
    "whole_pipeline/all_types/fold_infinity.c":
        "calls libc copysign",
    "whole_pipeline/all_types/fold_negative_zero.c":
        "calls libc copysign",
}


# helper_libs/exit.c is consumed by other files (multi-TU); it
# has no `main` and isn't a standalone test.
_NOT_A_TEST: frozenset[str] = frozenset({
    "helper_libs/exit.c",
})


def _run_program(source: str) -> int | None:
    preprocessed = preprocess(source)
    resolved = label_loops(resolve_labels(lift_strings(
        resolve_identifiers(parse(preprocessed)),
    )))
    prog, symbols, types = type_check_program(resolved)
    tac = translate_to_tac(prog, symbols, types)
    sim = Simulator(tac, symbols, types)
    return sim.call("main", [])


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestChapter19Sim(unittest.TestCase):
    """One subTest per file. The harness fails if a file is
    missing from every category — that catches new files added
    upstream and forces a triage decision."""

    def test_expected_returns(self):
        files = sorted(_C19.rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_19 files found")

        for path in files:
            rel = str(path.relative_to(_C19))
            if rel in _NOT_A_TEST:
                continue
            if rel in _NEEDS_LIBC:
                continue
            if rel in _PARSE_FAILURES:
                # Sanity-check the pin: make sure the parser does
                # actually reject. If it starts accepting the
                # literal, the pin is stale.
                with self.subTest(file=rel):
                    src = path.read_text()
                    with self.assertRaises(
                        ParserError,
                        msg=(
                            f"{rel} now parses — remove it from "
                            f"_PARSE_FAILURES"
                        ),
                    ):
                        _run_program(src)
                continue
            with self.subTest(file=rel):
                self.assertIn(
                    rel, _UPSTREAM_EXPECTED,
                    msg=(
                        f"{rel} has no upstream expected return — "
                        f"add an entry to expected_results.json or "
                        f"add the file to _PARSE_FAILURES / "
                        f"_NEEDS_LIBC"
                    ),
                )
                expected = _UPSTREAM_EXPECTED[rel]
                src = path.read_text()
                actual = _run_program(src)
                self.assertEqual(
                    actual, expected,
                    msg=(
                        f"{rel}: expected {expected}, got {actual} — "
                        f"either c6502 is miscompiling or the file "
                        f"needs a narrow-types adaptation"
                    ),
                )


if __name__ == "__main__":
    unittest.main()
