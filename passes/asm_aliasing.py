"""Shared operand-aliasing predicate for asm-level peepholes.

Two passes (`redundant_load`, `asm_dead_store`) need to decide
whether a read from operand `A` could be reading the same byte that
a write to operand `B` just modified. A third pass (`redundant_
store`) does the same thing but with a slightly different keyed-by-
address representation. This module centralizes the predicate so
the rules stay consistent across passes.

# Operand kinds and their addressing

c6502's asm operand vocabulary covers seven memory-shaped kinds:

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
  * `IndirectZp(addr, off)` / `IndirectZpY(addr)` — the same through
    the pointer held in the zero-page pair at `addr` (a pointer
    variable kept in zero page) instead of DPTR. `may_alias` compares
    the byte accessed, not the pointer bytes: a write to `addr`
    doesn't alias `(addr),Y`, though it changes where the next one
    reads. A pass that remembers a value read through the pointer
    must drop it when the pointer changes, as `asm_dead_store` does
    with `_ptr_source_reads`.
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
  * Indirect family vs a `Data` name in `unreachable_by_pointer`:
    no alias. The set is empty unless a pass installs one with
    `pointer_proof` (see `unreachable_by_pointer` below).
  * Indirect vs Indirect: alias iff same offset (both via DPTR).
  * IndirectZp vs IndirectZp: alias iff same base and same offset.
    Through different bases they may alias, since two pointer
    variables can hold the same address (`p = q`, or both set to one
    buffer). Nothing here knows the pointers' values, so it can't
    prove them different.
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

import dataclasses

import asm_ast
from passes.dataclass_fields import field_names


# Default asm-level regalloc pool range. Bytes here are
# function-local scratch storage; no user pointer can refer to them
# under c6502's address-taken-goes-to-Frame invariant.
DEFAULT_POOL_LO = 0x80
DEFAULT_POOL_HI = 0x100

# User globals placed in zero page (`[[c6502::zeropage]]`, see
# passes/zp_globals.py) for the compile in progress: name -> address,
# and every byte they occupy. Such a global is a `Data(name)` operand
# whose bytes may also appear as numeric `ZP(addr)` operands (the
# pointer bytes of an `IndirectZp` base), and its address may be
# taken, so the rules below that call a ZP byte compiler-private
# don't apply to its bytes. The driver sets this at the start of each
# compile (`set_user_zp_globals`) and clears it at the end, the way
# `asm_emit` keeps its per-emit symbol table.
_USER_ZP_GLOBALS: dict[str, int] = {}
_USER_ZP_BYTES: frozenset[int] = frozenset()

# Names of bytes no pointer can point to, which `pointer_proof`
# installs for the program a pass is rewriting. Empty otherwise.
_POINTER_PROOF: frozenset[str] = frozenset()


def set_user_zp_globals(
    addrs: dict[str, int], reserved: frozenset[int] = frozenset(),
) -> None:
    """Records the zero-page globals of the compile in progress.
    Pass `{}` to clear."""
    global _USER_ZP_GLOBALS, _USER_ZP_BYTES
    _USER_ZP_GLOBALS = dict(addrs)
    _USER_ZP_BYTES = frozenset(reserved)


def is_user_zp_byte(addr: int) -> bool:
    """True iff byte `addr` belongs to a user zero-page global."""
    return addr in _USER_ZP_BYTES


def is_user_zp_global(name: str) -> bool:
    """True iff `name` is a user zero-page global of the compile in
    progress."""
    return name in _USER_ZP_GLOBALS


def unreachable_by_pointer(prog: asm_ast.Program) -> frozenset[str]:
    """The `Data` names in `prog` that no pointer can point to, since
    nothing in the unit takes their address: no `LoadAddress` of
    them, no `#<name` or `#>name`, and no static initialized with
    their address. Two kinds qualify.

    - A `static` object (not `is_global`). Other units can't name it.
    - A zero-page slot of a function `F` defined in the unit,
      `__local_F__*` or `__zpabi_F__*`. Only `F`'s body can take the
      address of its own local or parameter. A slot that might
      belong to another function, whose name `F`'s prefix also
      begins (a function named `F__x`), is left out.

    `address_taken_zp` puts an address-taken local in a `__local_*`
    slot, so the slots aren't all out of reach. The address-taking
    of such a local is in this unit, which is what the check reads.
    A pointer to some other function's slot that shares the byte
    could only be followed while that function is active, and
    functions active together have disjoint bytes."""
    defined = {
        tl.name for tl in prog.top_level if isinstance(tl, asm_ast.Function)
    }
    named = set(defined)
    statics: set[str] = set()
    taken: set[str] = set()
    seen: set[str] = set()
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.StaticVariable):
            if not tl.is_global:
                statics.add(tl.name)
            for init in tl.init:
                if isinstance(init, (
                    asm_ast.AddressInit, asm_ast.AddressByteInit,
                )):
                    taken.add(init.name)
        elif isinstance(tl, asm_ast.Function):
            for ins in tl.instructions:
                if isinstance(ins, asm_ast.Call):
                    named.add(ins.name)
                if isinstance(ins, asm_ast.LoadAddress):
                    name = getattr(ins.src, "name", None)
                    if name is not None:
                        taken.add(name)
                for op in _instruction_operands(ins):
                    if isinstance(op, (asm_ast.ImmLabelLow,
                                       asm_ast.ImmLabelHigh)):
                        taken.add(op.name)
                    elif isinstance(op, asm_ast.Data):
                        seen.add(op.name)

    def own_slot(name: str) -> bool:
        owners = [
            f for f in named
            for prefix in (f"__local_{f}__", f"__zpabi_{f}__")
            if name.startswith(prefix)
        ]
        return len(owners) == 1 and owners[0] in defined

    slots = {n for n in seen if own_slot(n)}
    return frozenset((statics | slots) - taken)


# Classes of field values that are neither an operand nor a dataclass,
# which `_instruction_operands` passes over without calling
# `dataclasses.is_dataclass`.
_PLAIN = frozenset({str, int, bool, type(None)})


def _instruction_operands(ins):
    """Every operand in `ins`, in any field, including those inside a
    list (a `Phi`'s arguments)."""
    for name in field_names(ins):
        value = getattr(ins, name)
        for v in value if isinstance(value, list) else (value,):
            if isinstance(v, asm_ast.Type_operand):
                yield v
            elif type(v) not in _PLAIN and dataclasses.is_dataclass(v):
                yield from _instruction_operands(v)


def through_slot_pointer(ins, slot_addrs) -> bool:
    """Whether `ins` reads or writes through a `(zp),Y` pointer
    (`IndirectZp`, `IndirectZpY`) with a byte at one of `slot_addrs`,
    the addresses of the unit's `__zpabi_*` and `__local_*` slots.

    The operand carries the pointer's address, and `asm_emit` names it
    by the slot that the function it's emitted in keeps there. Two
    functions that are never on the call stack together can have
    slots at the same bytes in one unit, and the linker may place
    those slots apart, so such an instruction can't be moved into
    another function."""
    for op in _instruction_operands(ins):
        if (isinstance(op, (asm_ast.IndirectZp, asm_ast.IndirectZpY))
                and (op.address in slot_addrs
                     or op.address + 1 in slot_addrs)):
            return True
    return False


def pointer_byte_addresses(instrs) -> frozenset[int]:
    """The addresses of both bytes of every `(zp),Y` pointer
    (`IndirectZp`, `IndirectZpY`) that an instruction in `instrs`
    reads or writes through."""
    out: set[int] = set()
    for ins in instrs:
        for op in _instruction_operands(ins):
            if isinstance(op, (asm_ast.IndirectZp, asm_ast.IndirectZpY)):
                out.update((op.address, op.address + 1))
    return frozenset(out)


def is_pointer_byte(op, pointer_addrs, zp_symbol_addrs) -> bool:
    """Whether the `Data` or `ZP` byte `op` is one of `pointer_addrs`
    (from `pointer_byte_addresses`). A `(zp),Y` operand carries the
    pointer's address, and a `Data` operand a symbol, which
    `zp_symbol_addrs` resolves. A compiler slot or runtime byte it
    doesn't resolve counts as a pointer byte whenever the function
    has a pointer, since it may be one."""
    if not pointer_addrs:
        return False
    if isinstance(op, asm_ast.ZP):
        return op.address + op.offset in pointer_addrs
    if isinstance(op, asm_ast.Data):
        addr = (zp_symbol_addrs or {}).get(op.name)
        if addr is None:
            return _is_zp_symbol(op.name)
        return addr + op.offset in pointer_addrs
    return False


class pointer_proof:
    """Installs `names` (from `unreachable_by_pointer`) as the bytes
    `may_alias` treats as out of reach of the indirect operands, for
    the duration of a `with` block."""

    def __init__(self, names: frozenset[str]):
        self.names = names

    def __enter__(self):
        global _POINTER_PROOF
        self.saved = _POINTER_PROOF
        _POINTER_PROOF = self.names
        return self

    def __exit__(self, *exc):
        global _POINTER_PROOF
        _POINTER_PROOF = self.saved
        return False


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
    # addresses ≥ $0100 whose offset exceeds the ZP range). An
    # indexed access to a user zero-page array does reach zero page.
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.IndexedData):
        return b.name in _USER_ZP_GLOBALS or _is_zp_symbol(b.name)
    if isinstance(b, asm_ast.ZP) and isinstance(a, asm_ast.IndexedData):
        return a.name in _USER_ZP_GLOBALS or _is_zp_symbol(a.name)
    # An indexed access to a zero-page slot's object, a local array of
    # a function with a private pool, which `memory_value_propagation`
    # spells `name,Y` when a pointer holds its address, may reach any
    # of its bytes.
    if (isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.IndexedData)
            and a.name == b.name):
        return True
    if (isinstance(b, asm_ast.Data) and isinstance(a, asm_ast.IndexedData)
            and a.name == b.name):
        return True
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
    # A user zero-page object named by a `Data` operand, such as Lode
    # Runner's `GAME_COLNUM` at $85, against an indexed access to an
    # object outside zero page: the same reasoning as the literal `ZP`
    # operand above, since the name stands for its zero-page address.
    for d, x in ((a, b), (b, a)):
        if (isinstance(d, asm_ast.Data) and isinstance(x, asm_ast.IndexedData)
                and d.name in _USER_ZP_GLOBALS
                and d.name != x.name
                and x.name not in _USER_ZP_GLOBALS
                and not _is_zp_symbol(x.name)):
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
    if (isinstance(a, _indy_indirect_kinds) and isinstance(b, asm_ast.ZP)
            and not is_user_zp_byte(b.address + b.offset)):
        addr = b.address + b.offset
        if pool_lo <= addr < pool_hi:
            return False
    if (isinstance(b, _indy_indirect_kinds) and isinstance(a, asm_ast.ZP)
            and not is_user_zp_byte(a.address + a.offset)):
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
    if (isinstance(a, _indy_indirect_kinds) and isinstance(b, asm_ast.Data)
            and b.name in _POINTER_PROOF):
        return False
    if (isinstance(b, _indy_indirect_kinds) and isinstance(a, asm_ast.Data)
            and a.name in _POINTER_PROOF):
        return False
    # The same holds for an indexed access to such an object, a
    # `static` array whose address the unit never takes, such as a
    # table of row addresses: no pointer reaches any of its bytes.
    if (isinstance(a, _indy_indirect_kinds)
            and isinstance(b, asm_ast.IndexedData)
            and b.name in _POINTER_PROOF):
        return False
    if (isinstance(b, _indy_indirect_kinds)
            and isinstance(a, asm_ast.IndexedData)
            and a.name in _POINTER_PROOF):
        return False
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
        # Same base: the same pointer, so the offsets decide. Different
        # bases: two pointers can hold the same address, so different
        # bases may alias.
        if a.address != b.address:
            return True
        return a.offset == b.offset
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

def runtime_zp_addrs() -> dict[str, int]:
    """Base address of each runtime ZP symbol in the layout of the
    compile in progress (`passes/runtime_layout.py`), so
    `_zp_data_alias` can resolve them precisely without a
    caller-supplied slot map."""
    from passes.runtime_layout import current
    return current().addrs()


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
       it to get the precise answer.

    A user zero-page global (`_USER_ZP_GLOBALS`) is compared by its
    address."""
    user = _USER_ZP_GLOBALS.get(data.name)
    if user is not None:
        return (zp.address + zp.offset) == (user + data.offset)
    if not _is_zp_symbol(data.name):
        return False
    # Runtime symbols are pinned at fixed addresses; no slot map
    # needed.
    base = runtime_zp_addrs().get(data.name)
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
