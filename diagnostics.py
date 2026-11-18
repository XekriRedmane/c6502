"""Warnings, which report a problem in the program being compiled
without stopping the compile.

A pass that finds something to warn about calls `warn` with the place
and the message. The warning goes to the innermost `collecting()`
block that is open, and is dropped when none is, so a caller that
opens no such block, such as a test helper or the simulator harness,
never receives them. compile.py collects the warnings of each C
input and prints them to stderr as `<file>: warning: line N: <message>`,
the form its errors take.

    with diagnostics.collecting() as found:
        text = preprocess(source)
    for w in found:
        print(f"x.c: warning: {w}")

The only warning so far is the preprocessor's, for each `_Bool`
(`preprocessor.ALT_BOOL_WARNING`).
"""

from __future__ import annotations

import contextlib
from contextvars import ContextVar
from dataclasses import dataclass
from typing import Iterator


@dataclass(frozen=True)
class CompileWarning:
    """One warning. `where` names the line, as "line 3" in the file
    being compiled or "line 3 of foo.h" in a file it includes, the way
    the preprocessor's errors name it. `str()` gives the text after
    `warning: `."""
    where: str
    message: str

    def __str__(self) -> str:
        return f"{self.where}: {self.message}"


# The list the innermost open `collecting()` block appends to, or None
# outside every such block.
_SINK: ContextVar[list[CompileWarning] | None] = ContextVar(
    "c6502_warnings", default=None,
)


def warn(where: str, message: str) -> None:
    """Records a warning at `where` (see `CompileWarning`) in the
    innermost open `collecting()` block, or drops it outside one."""
    sink = _SINK.get()
    if sink is not None:
        sink.append(CompileWarning(where, message))


@contextlib.contextmanager
def collecting() -> Iterator[list[CompileWarning]]:
    """Collects the warnings issued inside the block, in the order they
    were issued, into the list it yields. A block nested inside another
    takes its warnings for itself, and the outer list doesn't get
    them."""
    found: list[CompileWarning] = []
    token = _SINK.set(found)
    try:
        yield found
    finally:
        _SINK.reset(token)
