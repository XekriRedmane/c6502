"""TAC unreachable-code elimination.

Drops any instruction sequence that can't be reached on any control-
flow path: code after `Ret` / `Jump` until the next `Label` (since
no fall-through reaches it), `Label`s with no incoming jump, and
empty regions that bridge two unreachable spans.

Currently a stub — returns its input unchanged. The actual pass will
land in a follow-up.
"""

from __future__ import annotations

import tac_ast


def eliminate_unreachable_code(fn: tac_ast.Function) -> tac_ast.Function:
    return fn
