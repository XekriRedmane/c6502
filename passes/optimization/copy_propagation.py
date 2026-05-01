"""TAC copy propagation.

Where a `Copy(src, dst)` lets us replace later reads of `dst` with
`src` directly — until either is rewritten — substitute the source
value into those uses. Sets up dead-store elimination to remove
the now-unused Copy on the next cycle.

Currently a stub — returns its input unchanged. The actual pass will
land in a follow-up: a forward dataflow analysis tracking known
copies per program point, invalidated on any write to either side
of a tracked pair (and on every instruction that writes through a
pointer, since a Store may alias an address-taken local).
"""

from __future__ import annotations

import tac_ast


def copy_propagate(fn: tac_ast.Function) -> tac_ast.Function:
    return fn
