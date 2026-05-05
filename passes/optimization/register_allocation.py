"""Coloring data class + helpers shared with the asm-level register
allocator (`passes/optimization_asm/regalloc.py`).

The TAC-level register allocator that used to live here has been
removed — coloring decisions now live entirely in the asm-level
pipeline, which has byte-granular precision and operates on the
post-`tac_to_asm` IR. This module retains:

  * `Coloring` — the result type the asm regalloc returns and that
    `apply_coloring` / `replace_pseudoregisters_bare_exit` consume.
  * `_blocked_bytes` — given an interference-graph node and the
    current `assignments` map, returns the set of ZP byte addresses
    occupied by colored neighbors. The asm regalloc reuses this
    width-aware blocking math.
  * `_find_fit` — given a contiguous byte range and a width, finds
    the lowest base such that `[base, base+width)` is unblocked.
"""

from __future__ import annotations

from dataclasses import dataclass, field

from passes.optimization.interference import InterferenceGraph
from passes.optimization.pool import Pool


@dataclass
class Coloring:
    """Result of an asm-level register-allocation pass.

    `assignments` maps each successfully-colored Var name to its ZP
    base address (the lowest byte of its allocated `width`-byte slot).
    `spilled` lists names that were in the input graph but couldn't
    fit any pool. `pool` echoes the configuration used.

    `hwreg_assignments` maps an SSA name to a hardware register letter
    ("X" or "Y") when regalloc decides to pin the value into a 6502
    index register instead of a ZP byte. A name appearing here is NOT
    in `assignments` (the two are mutually exclusive). HwReg pinning
    is only chosen for 1-byte values that meet a specific eligibility
    predicate (see `passes.optimization_asm.hwreg_eligibility`):
    every def/use must be representable as an LDX/LDY/STX/STY/INX/
    DEX/INY/DEY/CPX/CPY-style operation, and the value must not be
    live across any `Call` (helpers clobber X and Y). The headline
    savings are: (a) eliminating the LDX/LDY setup before each
    `IndexedData` access where the index value is HwReg-pinned, and
    (b) collapsing per-byte ADC chains on a counter into INX/DEX
    when the counter is HwReg-pinned."""
    assignments: dict[str, int] = field(default_factory=dict)
    spilled: set[str] = field(default_factory=set)
    pool: Pool = field(default_factory=Pool)
    hwreg_assignments: dict[str, str] = field(default_factory=dict)


def _blocked_bytes(
    name: str,
    graph: InterferenceGraph,
    assignments: dict[str, int],
) -> set[int]:
    """Bytes occupied by every already-colored neighbor of `name`."""
    out: set[int] = set()
    for m in graph.neighbors(name):
        base = assignments.get(m)
        if base is None:
            # Neighbor uncolored or spilled — no constraint on us
            # (a spilled neighbor lives in the frame, not a ZP slot).
            continue
        w = graph.nodes[m].width
        out.update(range(base, base + w))
    return out


def _find_fit(
    byte_range: range, width: int, blocked: set[int],
) -> int | None:
    """Lowest base in `byte_range` such that `[base, base+width)` is
    fully inside the range and disjoint from `blocked`. Returns None
    if no such base exists."""
    lo, hi = byte_range.start, byte_range.stop
    if width <= 0 or hi - lo < width:
        return None
    for base in range(lo, hi - width + 1):
        if any(b in blocked for b in range(base, base + width)):
            continue
        return base
    return None
