"""The bit-precise integer types, `_BitInt(N)` and `unsigned
_BitInt(N)` (C23 §6.2.5), for the fuzzers that model C's integer
rules (exprs.py and stmts.py).

Each fuzzer keeps a table of its types, key -> (C spelling, width in
bits, signed), and a rank for each standard type left after the
integer promotions. A seed that uses bit-precise types adds a few to
the table with `add`, under keys that `is_bit_precise` recognizes.
Those types aren't promoted (C23 §6.3.1.1p2), and `common` gives the
usual arithmetic conversions of two promoted types when either is one
of them (§6.3.1.8)."""

from __future__ import annotations

import random


def add(types: dict, width: int, signed: bool) -> str:
    """The key of `_BitInt(width)` or `unsigned _BitInt(width)`, added
    to `types`."""
    key = f"{'' if signed else 'u'}bi{width}"
    types[key] = (
        f"{'' if signed else 'unsigned '}_BitInt({width})", width, signed,
    )
    return key


def is_bit_precise(key: str) -> bool:
    return "bi" in key


def choose(types: dict, seed: int, count: int) -> list[str]:
    """The keys of the bit-precise types seed `seed` uses, added to
    `types`: none in half the seeds, and `count` of random widths in
    the others. The choice has a random stream of its own, so a seed
    without them is the program it was before they existed."""
    rng = random.Random(f"bitint{seed}")
    if rng.random() < 0.5:
        return []
    keys = []
    for _ in range(count):
        signed = rng.random() < 0.5
        keys.append(add(types, rng.randint(2 if signed else 1, 64), signed))
    return keys


def _rank_key(types: dict, rank: dict, t: str) -> tuple[int, int]:
    """Orders the promoted types by conversion rank (C23 §6.3.1.1p1):
    by width, with a standard type above a bit-precise one of the same
    width."""
    _, w, _ = types[t]
    return (w, 0) if is_bit_precise(t) else (w, rank[t])


def common(types: dict, rank: dict, unsigned_of: dict, a: str, b: str) -> str:
    """C23 §6.3.1.8 for the promoted types `a` and `b`, at least one of
    them bit-precise. `unsigned_of` maps each standard signed type to
    its unsigned counterpart."""
    if a == b:
        return a
    sa, sb = types[a][2], types[b][2]
    if sa == sb:
        return a if _rank_key(types, rank, a) >= _rank_key(types, rank, b) else b
    u, s = (a, b) if not sa else (b, a)
    if _rank_key(types, rank, u) >= _rank_key(types, rank, s):
        return u
    if types[s][1] - 1 >= types[u][1]:
        return s
    if is_bit_precise(s):
        return add(types, types[s][1], False)
    return unsigned_of[s]


def literal(types: dict, v: int, t: str) -> str:
    """A constant expression of the bit-precise type `t` with value `v`:
    a cast of its bit pattern, which c99_to_tac folds."""
    spelling, w, _ = types[t]
    return f"(({spelling})0x{v & ((1 << w) - 1):X}ULL)"
