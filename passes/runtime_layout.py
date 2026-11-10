"""Where the runtime's zero-page block goes: SSP, FP, HARGS, and DPTR.

The runtime keeps four things in zero page:

    SSP    2 bytes   soft stack pointer, an `(SSP),Y` base
    FP     2 bytes   frame pointer, an `(FP),Y` base
    HARGS  N bytes   the runtime-helper exchange block
    DPTR   2 bytes   the pointer scratch, an `(DPTR),Y` base

The compiled code names them only symbolically, and the output
binds each one with an `EQU` line. Their *classic* layout is SSP at
$00, FP at $02, a 32-byte HARGS at $04, and DPTR at $24, which ends
at $25.

Code that runs alongside system software can't always use those
bytes. On an Apple II, the Monitor keeps its text window in $20-$23
and its cursor in $24-$25, right on top of HARGS and DPTR. A program
says which bytes something else owns by declaring them, with
`__attribute__((address(...)))` or `zeropage(addr)`, and
`choose_layout` moves the block out of their way:

  * If no reserved byte is in the classic layout, the classic layout
    is used, so a program that doesn't reserve those bytes compiles
    exactly as before.
  * Otherwise HARGS is sized to what the program needs (`hargs_need`),
    and HARGS, SSP, FP, and DPTR take the lowest free runs of bytes
    below $80, in that order. The compiler's own pools are $80-$FF,
    so the block stays below them. When it doesn't fit, that's a
    compile error.

The layout has to be fixed before the asm passes run, since several
of them resolve runtime names to addresses to decide aliasing
(`asm_aliasing`, `memory_value_propagation`, `redundant_store`).
`current()` is the layout of the compile in progress, which the
driver sets with `set_current` and resets at the end.

# HARGS size

A helper reads its inputs from HARGS, writes its outputs there, and
may use more of it as scratch: `mul32` works in HARGS+12..19. So a
program needs the larger of its own highest HARGS offset and the
footprint of every helper it calls. The footprints are measured from
the 6502 helpers in `sim/runtime_helpers.py`, including the helpers
they call. A helper that's only a Python hook in the simulator, such
as the floating-point ones, uses no scratch, so the program's own
offsets cover it. A runtime written outside this repo has to keep
each helper within those footprints.
"""

from __future__ import annotations

import dataclasses
from dataclasses import dataclass
from functools import lru_cache

import asm_ast
from passes.type_checking import TypeCheckError


@dataclass(frozen=True)
class RuntimeLayout:
    """The address of each runtime zero-page block. `hargs_size` is
    how many bytes HARGS spans."""
    ssp: int = 0x00
    fp: int = 0x02
    hargs: int = 0x04
    hargs_size: int = 32
    dptr: int = 0x24

    def addrs(self) -> dict[str, int]:
        """Each runtime symbol's address, for the EQU block and the
        passes that resolve names to bytes."""
        return {
            "SSP": self.ssp, "FP": self.fp,
            "HARGS": self.hargs, "DPTR": self.dptr,
        }

    def bytes(self) -> frozenset[int]:
        """Every byte the block occupies."""
        out: set[int] = set()
        for base, n in (
            (self.ssp, 2), (self.fp, 2), (self.dptr, 2),
            (self.hargs, self.hargs_size),
        ):
            out.update(range(base, base + n))
        return frozenset(out)


CLASSIC = RuntimeLayout()
RUNTIME_NAMES = frozenset(CLASSIC.addrs())

_current = CLASSIC


def current() -> RuntimeLayout:
    """The layout of the compile in progress (CLASSIC outside one)."""
    return _current


def set_current(layout: RuntimeLayout) -> None:
    """Sets the layout for the compile in progress. The driver resets
    it to CLASSIC when the compile ends."""
    global _current
    _current = layout


class RuntimeLayoutError(TypeCheckError):
    """The runtime's zero-page block doesn't fit around the bytes the
    program reserves."""


def choose_layout(reserved, hargs_need) -> RuntimeLayout:
    """The layout for a program whose fixed zero-page objects occupy
    `reserved`. `hargs_need` is called, only when the block has to
    move, for the number of HARGS bytes the program needs."""
    reserved = frozenset(reserved)
    if not CLASSIC.bytes() & reserved:
        return CLASSIC
    # HARGS gets at least one byte, even in a program that never
    # touches it, so that no other object shares its address and the
    # emitter never names a pointer `(HARGS),Y`.
    size = max(hargs_need(), 1)
    used = set(reserved)

    def take(n: int, what: str) -> int:
        for addr in range(0, 0x80 - n + 1):
            if all(b not in used for b in range(addr, addr + n)):
                used.update(range(addr, addr + n))
                return addr
        raise RuntimeLayoutError(
            f"no room below $80 for the runtime's {what} ({n} bytes) "
            f"around the reserved zero-page bytes "
            f"{_ranges(sorted(reserved))}"
        )

    hargs = take(size, "HARGS block")
    ssp = take(2, "soft stack pointer SSP")
    fp = take(2, "frame pointer FP")
    dptr = take(2, "pointer scratch DPTR")
    return RuntimeLayout(
        ssp=ssp, fp=fp, hargs=hargs, hargs_size=size, dptr=dptr,
    )


def _ranges(addrs: list[int]) -> str:
    """`[0x20, 0x21, 0x22, 0x30]` as "$20-$22, $30"."""
    out: list[str] = []
    i = 0
    while i < len(addrs):
        j = i
        while j + 1 < len(addrs) and addrs[j + 1] == addrs[j] + 1:
            j += 1
        out.append(
            f"${addrs[i]:02X}" if i == j
            else f"${addrs[i]:02X}-${addrs[j]:02X}"
        )
        i = j + 1
    return ", ".join(out)


# ---------------------------------------------------------------------
# HARGS size
# ---------------------------------------------------------------------

def _hargs_offsets(instrs) -> set[int]:
    """The HARGS byte offsets that `instrs` reads or writes."""
    out: set[int] = set()
    for instr in instrs:
        for f in dataclasses.fields(instr):
            op = getattr(instr, f.name)
            if (isinstance(op, (asm_ast.Data, asm_ast.IndexedData))
                    and op.name == "HARGS"):
                out.add(op.offset)
    return out


@lru_cache(maxsize=1)
def helper_footprints() -> dict[str, int]:
    """For each 6502 helper in `sim/runtime_helpers.py`, the number
    of HARGS bytes it touches, counting the helpers it calls."""
    from sim.runtime_helpers import all_helper_functions
    fns = {fn.name: fn for fn in all_helper_functions()}
    own = {
        name: max(_hargs_offsets(fn.instructions), default=-1) + 1
        for name, fn in fns.items()
    }
    calls = {
        name: {
            i.name for i in fn.instructions
            if isinstance(i, asm_ast.Call) and i.name in fns
        }
        for name, fn in fns.items()
    }
    out: dict[str, int] = {}

    def size(name: str, seen: frozenset[str]) -> int:
        if name in out:
            return out[name]
        n = own[name]
        for callee in calls[name] - seen:
            n = max(n, size(callee, seen | {name}))
        out[name] = n
        return n

    for name in fns:
        size(name, frozenset())
    return out


def hargs_need(prog: asm_ast.Program) -> int:
    """How many HARGS bytes `prog` needs: its own highest offset, and
    the footprint of each helper it calls."""
    footprints = helper_footprints()
    need = 0
    for tl in prog.top_level:
        if not isinstance(tl, asm_ast.Function):
            continue
        need = max(need, max(_hargs_offsets(tl.instructions), default=-1) + 1)
        for i in tl.instructions:
            if isinstance(i, asm_ast.Call):
                need = max(need, footprints.get(i.name, 0))
    return need


def check_hargs(prog: asm_ast.Program, layout: RuntimeLayout) -> None:
    """Raises if the finished `prog` uses more of HARGS than `layout`
    gave it. The size is measured on the unoptimized code, so this
    would mean an optimizer pass called a helper with a larger
    footprint than any the unoptimized code called."""
    need = hargs_need(prog)
    if need > layout.hargs_size:
        raise RuntimeLayoutError(
            f"internal error: the program uses {need} HARGS bytes, but "
            f"the runtime layout was sized for {layout.hargs_size}"
        )
