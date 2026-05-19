"""Shared operand-aliasing predicate for asm-level peepholes.

Two passes (`redundant_load`, `asm_dead_store`) need to decide
whether a read from operand `A` could be reading the same byte that
a write to operand `B` just modified. A third pass (`redundant_
store`) does the same thing but with a slightly different keyed-by-
address representation. This module centralizes the predicate so
the rules stay consistent across passes.

# Operand kinds and their addressing

c6502's asm operand vocabulary covers six memory-shaped kinds:

  * `Imm(value)` — immediate constant baked into the instruction.
    Never refers to memory.
  * `ZP(addr, offset)` — the byte at `addr + offset`, in `[0, 0xFF]`.
  * `Data(name, offset)` — the byte at the link-time absolute
    address `name + offset`. For c6502's runtime-installed symbols
    (`SSP` / `FP` / `HARGS` / `DPTR`), `name` resolves into ZP. For
    user statics, `name` resolves to a fixed address in the data
    segment (≥ ORIGIN = $0800 by default).
  * `IndexedData(name, offset, index_reg)` — the byte at
    `name + offset + index_reg_value`. Spans the 256-byte range
    `[name+offset, name+offset+255]` at runtime.
  * `Indirect(off)` / `IndirectY()` — the byte at `(DPTR),Y`. DPTR
    holds a runtime 16-bit pointer; the actual address is opaque
    at compile time. `Indirect(off)` adds a compile-time `off` to
    Y; `IndirectY()` uses whatever Y already holds.
  * `Frame(off)` / `Stack(off)` — the byte at `(FP),Y` / `(SSP),Y`
    with a compile-time `off`. FP and SSP are stable runtime
    pointers into the soft-stack region.

# Aliasing rules

Two operands "may alias" iff we can't prove they refer to disjoint
memory cells. The rules below are deliberately conservative — when
in doubt, return True. The interesting refinements over a fully
naive "anything-indirect-may-alias-anything" model:

  * **Frame / Stack vs ZP**: never aliases. `FP` and `SSP` point
    into the soft stack — a region of main RAM well above ZP
    ($0800+ by c6502 convention). Soft-stack offsets are 0..255;
    no `(FP)+off` reaches into `[0, 0xFF]`.

  * **Indirect / IndirectY vs ZP in the regalloc pool**: never
    aliases. DPTR holds a user-supplied pointer. By c6502's
    invariant, the asm-level regalloc pool (`[Pool.start, 0xFF]`,
    default `[$80, $FF]`) is reserved for compiler-managed scratch
    storage for non-address-taken locals. Address-taken locals
    spill to `Frame`, never ZP — so no source-level construct can
    form a pointer into the pool. Pathological literal casts
    (`*(uint8_t*)0x84`) are out of scope.

  * **ZP vs Data / IndexedData**: never aliases. The asm IR uses
    distinct namespaces — `Data(symbol_name, off)` accesses through
    the symbol name (resolved at link time), `ZP(addr, off)` is
    a literal byte address. c6502's emission convention ensures the
    two namespaces don't both name the same byte: ZP references
    are emitted only by regalloc (slots inside the pool) and the
    runtime header symbols (`DPTR`, `HARGS`, etc.) are referenced
    by name as Data, never as raw ZP literals.

  * Frame vs Frame: alias iff same offset (Frame's `(FP),Y`
    semantics — different offsets touch different bytes of the same
    frame).
  * Stack vs Stack: alias iff same offset.
  * Frame vs Stack: no alias. FP and SSP point to different stack
    frames (SSP=top of current arg pack, FP=bottom of frame); even
    if their ranges adjoin, distinct offsets within each map to
    distinct frames.
  * Indirect vs Indirect: alias iff same offset (both via DPTR).
  * Indirect vs IndirectY / vice versa: aliases (both access
    DPTR-indexed bytes; Y could match `off`).
  * Indirect / IndirectY vs Frame / Stack: aliases (user might
    pass `&local`, making the DPTR pointer point into the frame).
  * Anything else: conservative True.

The "regalloc pool" range below comes from `passes.optimization.
pool.Pool`'s default of `start=0x80`. Functions running with a
non-default Pool can pass an explicit range; the default is
correct for everything c6502 ships today.
"""

from __future__ import annotations

import asm_ast


# Default asm-level regalloc pool range. Bytes here are
# function-local scratch storage; no user pointer can refer to them
# under c6502's address-taken-goes-to-Frame invariant.
DEFAULT_POOL_LO = 0x80
DEFAULT_POOL_HI = 0x100


def may_alias(
    a: asm_ast.Type_operand,
    b: asm_ast.Type_operand,
    *,
    pool_lo: int = DEFAULT_POOL_LO,
    pool_hi: int = DEFAULT_POOL_HI,
    zp_slot_symbols: dict[str, int] | None = None,
) -> bool:
    """True iff we can't prove the two operands refer to disjoint
    memory cells. Conservative — see module docstring for the rule
    list. `pool_lo` / `pool_hi` define the half-open range
    `[pool_lo, pool_hi)` of asm-level regalloc-pool ZP addresses
    (i.e. addresses for which a `(DPTR),Y` read is provably
    non-aliasing under the c6502 convention).

    `zp_slot_symbols` is the optional slot-name → ZP-address map
    (e.g. `__local_foo__bar` → `$8A`). When provided, the ZP-vs-Data
    rule resolves the Data name through it and compares addresses
    — so `Data("__local_..._1")` at $8A aliases `ZP($8A)` exactly,
    catching the case where an `IndirectZp(addr=$8A)` reads the
    pointer pair that an `STA __local_..._1` initialized. Without
    the map, the rule falls back to conservative aliasing for any
    Data name that *looks* like a ZP slot symbol (per
    `_is_zp_symbol`) — sound but pessimistic; callers that have
    the map should always pass it."""
    # Imm / ImmLabelLow / ImmLabelHigh are values, not memory —
    # they never alias a memory operand. `LDA #<label` loads the
    # immediate low byte of the resolved address; it doesn't read
    # the memory the label names. Same byte-emit shape as `LDA
    # #imm`, same aliasing behaviour.
    _imm_kinds = (asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh)
    if isinstance(a, _imm_kinds) or isinstance(b, _imm_kinds):
        return False
    # Normalize so the same-kind cases each get one branch by
    # checking both orderings against canonical type-pair tests.
    # ZP vs ZP: alias iff same absolute address.
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return (a.address + a.offset) == (b.address + b.offset)
    # Data vs Data: alias iff same name+offset.
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    # ZP vs Data: by c6502's emission convention these are distinct
    # namespaces, EXCEPT for `__local_*` / `__zpabi_*` / runtime
    # (`SSP` / `FP` / `HARGS` / `DPTR`) symbols which resolve to
    # ZP byte addresses via EQU bindings. For those, compare
    # resolved addresses when the slot map is available; without
    # the map, fall back to conservative (may alias) for any
    # ZP-symbol Data — sound but pessimistic.
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.Data):
        return _zp_data_alias(a, b, zp_slot_symbols)
    if isinstance(b, asm_ast.ZP) and isinstance(a, asm_ast.Data):
        return _zp_data_alias(b, a, zp_slot_symbols)
    # ZP vs IndexedData: distinct namespaces (IndexedData operands
    # name link-time statics — own namespace — or raw absolute
    # addresses ≥ $0100 whose offset exceeds the ZP range).
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.IndexedData):
        return False
    if isinstance(b, asm_ast.ZP) and isinstance(a, asm_ast.IndexedData):
        return False
    # ZP-symbol Data (`__local_*`, `__zpabi_*`) vs IndexedData:
    # these symbols resolve to ZP byte addresses via EQU
    # bindings at link time. IndexedData operands either target
    # link-time-named statics (own namespace) or raw absolute
    # addresses ≥ $0100 (their offset field exceeds the ZP
    # range). In either case they can't alias a ZP-resolved
    # symbol — same reasoning as the literal `ZP` operand
    # branch above.
    if isinstance(a, asm_ast.Data) and isinstance(
        b, asm_ast.IndexedData,
    ) and _is_zp_symbol(a.name):
        return False
    if isinstance(b, asm_ast.Data) and isinstance(
        a, asm_ast.IndexedData,
    ) and _is_zp_symbol(b.name):
        return False
    # Frame / Stack vs ZP: FP / SSP point into the soft stack
    # (main RAM, ≥ $0800); offsets 0..255 don't reach ZP.
    if isinstance(a, (asm_ast.Frame, asm_ast.Stack)) and isinstance(
        b, asm_ast.ZP,
    ):
        return False
    if isinstance(b, (asm_ast.Frame, asm_ast.Stack)) and isinstance(
        a, asm_ast.ZP,
    ):
        return False
    # Indirect-Y family (Indirect / IndirectY / IndirectZp /
    # IndirectZpY) vs ZP-in-pool: user pointers don't point into
    # the regalloc pool under c6502's address-taken-goes-to-Frame
    # invariant.
    _indy_indirect_kinds = (
        asm_ast.Indirect, asm_ast.IndirectY,
        asm_ast.IndirectZp, asm_ast.IndirectZpY,
    )
    if isinstance(a, _indy_indirect_kinds) and isinstance(b, asm_ast.ZP):
        addr = b.address + b.offset
        if pool_lo <= addr < pool_hi:
            return False
    if isinstance(b, _indy_indirect_kinds) and isinstance(a, asm_ast.ZP):
        addr = a.address + a.offset
        if pool_lo <= addr < pool_hi:
            return False
    # IndirectZp / IndirectZpY vs Data(runtime symbol): the
    # IndirectZp family uses an explicit ZP base (NOT DPTR), so it
    # neither aliases DPTR (the pointer source) nor any other
    # runtime ZP symbol (user pointers don't point into runtime
    # infrastructure by c6502 convention).
    #
    # Note: `Indirect` / `IndirectY` themselves DO reference DPTR
    # (it's their pointer source) and so MUST be allowed to alias
    # `Data("DPTR", _)` — otherwise DSE would drop a live STA DPTR
    # that subsequent LDA (DPTR),Y reads observe. The rule below
    # is intentionally scoped to the IndirectZp* kinds only.
    _zp_indirect_kinds = (asm_ast.IndirectZp, asm_ast.IndirectZpY)
    if isinstance(a, _zp_indirect_kinds) and isinstance(b, asm_ast.Data):
        if b.name in _RUNTIME_ZP_NAMES:
            return False
    if isinstance(b, _zp_indirect_kinds) and isinstance(a, asm_ast.Data):
        if a.name in _RUNTIME_ZP_NAMES:
            return False
    # Same-kind same-offset cases for the indirect-Y family.
    if isinstance(a, asm_ast.Frame) and isinstance(b, asm_ast.Frame):
        return a.offset == b.offset
    if isinstance(a, asm_ast.Stack) and isinstance(b, asm_ast.Stack):
        return a.offset == b.offset
    if isinstance(a, asm_ast.Indirect) and isinstance(b, asm_ast.Indirect):
        return a.offset == b.offset
    if isinstance(a, asm_ast.IndirectZp) and isinstance(b, asm_ast.IndirectZp):
        # Different ZP bases ⇒ different runtime pointers ⇒ no
        # alias. Same base + same offset ⇒ same byte.
        if a.address != b.address:
            return False
        return a.offset == b.offset
    if isinstance(a, asm_ast.IndirectZpY) and isinstance(b, asm_ast.IndirectZpY):
        if a.address != b.address:
            return False
        return True  # same base, both depend on the same external Y
    # Frame vs Stack: separate stack pointers; no alias.
    if (
        isinstance(a, asm_ast.Frame) and isinstance(b, asm_ast.Stack)
        or isinstance(a, asm_ast.Stack) and isinstance(b, asm_ast.Frame)
    ):
        return False
    # Default: conservative.
    return True


# Names of runtime-installed ZP symbols (matches `sim.assembler.
# DEFAULT_ZP_SYMBOLS`). Used by the indirect-Y aliasing rules to
# tell "Data(known runtime symbol)" apart from "Data(user
# static)" — only the user-static side could possibly be reached
# by a user pointer.
_RUNTIME_ZP_NAMES = frozenset({"SSP", "FP", "HARGS", "DPTR"})

# Fixed base addresses for the runtime ZP symbols. These are
# pinned by the c6502 ABI (`sim/runtime.py:57-62`) and never
# move, so `_zp_data_alias` can resolve them precisely without a
# caller-supplied slot map. The address layout is documented in
# the project root `CLAUDE.md` under "Function stack frame".
_RUNTIME_ZP_ADDRS: dict[str, int] = {
    "SSP": 0x00,
    "FP": 0x02,
    "HARGS": 0x04,
    "DPTR": 0x24,
}


def _zp_data_alias(
    zp: asm_ast.ZP,
    data: asm_ast.Data,
    zp_slot_symbols: dict[str, int] | None,
) -> bool:
    """Aliasing decision for `ZP(addr)` vs `Data(name, off)`.
    Three cases:

    1. `data.name` isn't a known ZP-resolving symbol (`_is_zp_symbol`
       false) — it names a non-ZP static, addresses ≥ $0100. Can't
       alias a ZP byte.
    2. `data.name` IS a ZP symbol AND `zp_slot_symbols` resolves it
       — compare resolved addresses (the exact alias check).
    3. `data.name` IS a ZP symbol but the slot map isn't supplied
       (or doesn't contain this name) — fall back to conservative
       MAY-alias. Sound; callers with the map should always pass
       it to get the precise answer."""
    if not _is_zp_symbol(data.name):
        return False
    # Runtime symbols are pinned at fixed addresses; no slot map
    # needed.
    base = _RUNTIME_ZP_ADDRS.get(data.name)
    if base is None and zp_slot_symbols is not None:
        base = zp_slot_symbols.get(data.name)
    if base is not None:
        return (zp.address + zp.offset) == (base + data.offset)
    return True


def _is_zp_symbol(name: str) -> bool:
    """True iff `name` is one of c6502's known ZP-resolving
    symbols: runtime infrastructure (`SSP`/`FP`/`HARGS`/`DPTR`),
    a zp_abi param slot (`__zpabi_<fn>_p<k>`), or a body-local
    slot (`__local_<fn>_b<k>`). Each of these resolves to a ZP
    byte address via an EQU binding, so the same "distinct
    namespace" rule that applies to literal `ZP` operands
    applies to Data references against these symbols too."""
    return (
        name in _RUNTIME_ZP_NAMES
        or name.startswith("__zpabi_")
        or name.startswith("__local_")
    )
