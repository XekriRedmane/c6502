"""Checks that every program of the chapters' `invalid_*` buckets that
c6502 rejects is rejected with an error about the program.

The chapter harnesses accept any exception as a rejection, so a program
the type checker let through and a later pass crashed on passed there.
On 20 Nov 2026 fourteen did, among them `10 & ptr`, `ptr /= q`, `buff +=
3` on a `void *`, `foo(s)` with a struct for an `int` parameter, and
`*x = foo()` through a `void *`. Here each program must compile (the
harnesses pin the ones c6502 accepts) or raise one of the types
`compile._source_errors` lists, which `compile.py` reports as a one-line
error. There is a test method per chapter, so the workers of `pytest -n`
can share them.
"""
from __future__ import annotations

import traceback
import unittest
from pathlib import Path

from compile import _run_source, _source_errors
from tests.cpp_build import needs_c6502cpp

_TESTS_DIR = Path(__file__).parent


def _chapters() -> list[Path]:
    return sorted(
        d for d in _TESTS_DIR.glob("chapter_*")
        if any(d.glob("invalid_*/**/*.c"))
    )


@needs_c6502cpp
class TestInvalidProgramsReportErrors(unittest.TestCase):
    def _check_chapter(self, chapter: Path) -> None:
        errors = _source_errors()
        for path in sorted(chapter.glob("invalid_*/**/*.c")):
            rel = path.relative_to(_TESTS_DIR).as_posix()
            if "/libraries/" in rel:
                continue
            with self.subTest(file=rel):
                try:
                    _run_source("codegen", path.read_text())
                except errors:
                    pass
                except Exception:  # noqa: BLE001 - the failure under test
                    self.fail(
                        f"{rel} raised an internal error:\n"
                        f"{traceback.format_exc(limit=3)}"
                    )


def _make_test(chapter: Path):
    def test(self):
        self._check_chapter(chapter)
    return test


for _chapter in _chapters():
    setattr(
        TestInvalidProgramsReportErrors, f"test_{_chapter.name}",
        _make_test(_chapter),
    )


if __name__ == "__main__":
    unittest.main()
