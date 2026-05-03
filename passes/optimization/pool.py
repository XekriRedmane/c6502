"""Zero-page byte pool configuration for register allocation.

The 6502 has 256 zero-page bytes ($00-$FF). The runtime header
reserves the low end ($00-$1B today: SSP/FP/HARGS/DPTR), so the
register allocator draws from the high end. A `Pool` describes the
allocator's available range and how it's split into caller-saved
and callee-saved halves.

  * `start` — lowest available ZP address. **Must be even**, so that
    the available range `[start, 0xFF]` (length `0x100 - start`,
    which is then also even) splits exactly in half.
  * `mid = 0x80 + start // 2` — midpoint of `[start, 0xFF]`.
  * **Caller-saved pool** = `[start, mid - 1]`. Values colored here
    don't survive function calls; the caller would have to spill
    them around each call. Not promised to survive across the call.
  * **Callee-saved pool** = `[mid, 0xFF]`. The callee promises to
    preserve these values across its own body; using one costs
    extra prologue/epilogue save+restore pairs but means a value
    can stay in the same slot across calls.

Each half has `(0x100 - start) // 2` bytes. Default `start=0x80`
gives 64 bytes per half. The 6502 has no alignment requirement on
multi-byte values, so a 2/4/8-byte slot may start at any byte
within a pool's range as long as all of its bytes fit.
"""

from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class Pool:
    start: int = 0x80

    def __post_init__(self) -> None:
        self.validate()

    def validate(self) -> None:
        if self.start < 0 or self.start > 0xFF:
            raise ValueError(
                f"Pool.start out of range [0, 0xFF]: 0x{self.start:02X}",
            )
        if self.start % 2 != 0:
            raise ValueError(
                f"Pool.start must be even: 0x{self.start:02X}",
            )

    @property
    def mid(self) -> int:
        return 0x80 + self.start // 2

    def caller_saved(self) -> range:
        """Caller-saved bytes `[start, mid)`."""
        return range(self.start, self.mid)

    def callee_saved(self) -> range:
        """Callee-saved bytes `[mid, 0x100)`."""
        return range(self.mid, 0x100)
