"""C preprocessor wrapper, adapted from pcpp/pcmd.py.

pcmd.py wraps pcpp.preprocessor.Preprocessor with an argparse front-end
and a bunch of passthru/include/timing options we don't use. This module
strips that down to the one operation compile.py needs: take a C source
string and return the preprocessed string with comments removed and no
#line directives in the output.

Behavior matches the shell command `pcpp - --line-directive`:
  - block and line comments are replaced with a single space (C99
    translation phase 3), via the default `on_comment` hook
  - `#line` markers are suppressed (`line_directive = None`)
  - macros and includes work as the underlying Preprocessor handles them

Errors during parsing or writing propagate as the underlying pcpp
exceptions; the caller decides whether to format them or re-raise.
"""

from __future__ import annotations

import io

from pcpp.preprocessor import Preprocessor


class CompilePreprocessor(Preprocessor):
    """Minimal pcpp configuration for our compiler driver."""

    def __init__(self) -> None:
        super().__init__()
        # Suppress `#line N "file"` markers in the output. Same as
        # passing `--line-directive` with no argument to the pcpp CLI.
        self.line_directive = None

    def run(self, source: str) -> str:
        self.parse(source)
        out = io.StringIO()
        self.write(out)
        return out.getvalue()


def preprocess(source: str) -> str:
    """One-shot preprocessing: strip comments, drop #line directives."""
    return CompilePreprocessor().run(source)
