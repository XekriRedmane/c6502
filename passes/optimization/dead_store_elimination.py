"""TAC dead-store elimination.

Removes assignments to temporaries / locals whose result is never
read on any subsequent path. Pairs with copy propagation: once a
Copy's destination has had every use rewritten to the source, the
Copy itself becomes a dead store.

Currently a stub — returns its input unchanged. The actual pass will
land in a follow-up: a backward liveness analysis on the function's
instruction list, treating params and address-taken locals
conservatively as live across pointer-writing instructions.
"""

from __future__ import annotations

import tac_ast


def eliminate_dead_stores(fn: tac_ast.Function) -> tac_ast.Function:
    return fn
