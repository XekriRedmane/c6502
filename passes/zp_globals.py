"""Zero-page globals: `__attribute__((zeropage))`.

A static-storage object declared with `__attribute__((zeropage))`
lives in zero page instead of the data segment, so every access is a
2-byte zero-page instruction (`LDA zp`, 3 cycles) where an ordinary
global takes a 3-byte absolute one (`LDA abs`, 4 cycles). A pointer
in zero page can also serve directly as the base of an `(zp),Y`
access, where an ordinary pointer global is first copied to DPTR.

    uint8_t num_entries __attribute__((zeropage));            // $26
    __attribute__((zeropage(0xBA))) uint8_t *z_header_addr;   // $BA-$BB

The address form puts the object's first (low) byte at that
address, anywhere in `$00`-`$FF`, so code ported from hand-written
assembly can keep its documented addresses. A fixed address in
`$80`-`$FF` is taken out of the compiler's pool: `reserved` is passed
to the zp_abi slot allocator, the private-pool allocator, and
register allocation, so nothing else is placed on those bytes. Two
zero-page objects may not overlap, and an object must fit below
`$100`.

The runtime's own block (SSP, FP, HARGS, DPTR) is placed after the
fixed objects, by `runtime_layout.choose_layout`: at `$00`-`$25` when
no fixed object is there, and around them otherwise. The bare form
then takes the lowest free bytes below `$80` (`AUTO_HI`), which is
`$26` up in the classic layout. The compiler's own pool is
`$80`-`$FF`.

# Initial values

Zero page isn't part of the loaded program image, so a zero-page
object's initial value (its initializer, or zero, as C requires for
static storage) is stored at run time. `place_zp_globals` drops the
object's `StaticVariable` and adds a function `__zp_init` that stores
the initial bytes of every zero-page object defined in the
translation unit. The boot code calls `__zp_init` before `main`; the
simulator's boot stub does (`sim/runtime.py`). An `extern`
declaration with a fixed address and no definition in the unit
names a byte something else owns (another unit, or hand-written
code), so it's reserved and addressed but not initialized. An
`extern` declaration without an address and without a definition
names an object another unit defines. This unit doesn't place it
(`ZpGlobals.external`); its references stay `Data(name, k)`, and
`compile.py --link` gives the name its address.

# How references reach zero page

Every reference to a zero-page object stays `Data(name, k)` in the
asm IR, the same as any other static. The name is bound with an
`EQU` in the output, and dasm picks zero-page addressing for it. The
peepholes that track zero-page cells (`memory_value_propagation`)
are given the addresses, so the DPTR staging of a zero-page pointer
becomes `(name),Y` directly. `asm_aliasing` is told the addresses for
the duration of a compile (`set_user_zp_globals`), so a numeric
`ZP(addr)` byte, such as the pointer of an `IndirectZp` base, is known
to be the same byte as the object. These names are kept out of the
compiler's own slot-symbol map, because several passes treat a slot
symbol in `$80`-`$FF` as scratch that's dead when the function
returns, and a global's value isn't.

# Linking

Since every reference is by name, the code doesn't depend on where a
bare `zeropage` object ends up. Under `--optimize` each unit lists its
zero-page objects in the link metadata (`link_records`): the size, the
fixed address if it has one, whether the unit defines it, and its
initial bytes. `compile.py --link` keeps the fixed addresses, places
the rest for the whole program, keeps every object's bytes out of the
slot allocators, and replaces each unit's `__zp_init` with one that
stores every object's initial bytes (`passes/linker.py`).

# Promotion under `--optimize`

With `--optimize`, globals nobody asked for are moved into zero page
too, when they're used enough to pay for it (`_promotion_order`). A
candidate is a scalar (an integer, a floating-point number, or a
pointer) with static storage that the unit defines, that isn't
`const`, and that isn't already placed. An array or a struct gains
little, since `LDA arr,Y` has no zero-page form and taking an address
costs the same anywhere. Each candidate's weight is its references in
the unoptimized TAC, other than taking its address, with a reference
inside a loop counting 8 times per loop around it, up to 3 deep
(`access_weights`). A candidate is promoted when its weight is at
least 3 per byte, since storing its initial value in `__zp_init`
costs about 3 cycles a byte that loading the program image didn't;
the densest go first, into whatever bytes below `$80` are left after
the declared objects. A promoted object is then a zero-page object
like a declared one (`ZpGlobals.promoted` keeps the weights), and the
link metadata marks it so that the linker can put it back in the data
segment if the linked program has no room for it.

# Fixed addresses outside zero page

`__attribute__((address(addr)))` binds a function or an `extern`
object to an address something outside the program owns:

    __attribute__((zp_abi, address(0xFDED)))
    extern void COUT(register uint8_t c);              // ROM routine
    extern volatile uint8_t KBD __attribute__((address(0xC000)));

An object at an address below $100 is a zero-page object like any
other, reserved and not initialized. Every other name goes in
`ZpGlobals.fixed`, which only adds an `EQU` line: a call stays `JSR
COUT`, and an access stays `Data(name, k)`, which dasm assembles with
absolute addressing. Two fixed objects may not overlap, because the
aliasing passes take two different `Data` names to be different
bytes. A fixed object isn't checked against the program's own code
and data, since their addresses are only known once dasm has
assembled the program.
"""

from __future__ import annotations

from contextlib import contextmanager
from dataclasses import dataclass, field

import asm_ast
from passes.runtime_layout import CLASSIC, RuntimeLayout, choose_layout
from passes.type_checking import TypeCheckError

# A bare `zeropage` object is placed below this address, in bytes
# neither a fixed object nor the runtime's block uses.
AUTO_HI = 0x80

INIT_FUNCTION = "__zp_init"


class ZpGlobalError(TypeCheckError):
    """A zero-page object that can't be placed."""


@dataclass
class ZpGlobals:
    """The zero-page objects of a translation unit.

    `addrs` maps each object's name to the address of its first byte.
    `reserved` is every byte they occupy, for the allocators to avoid.
    `owned` names the objects this unit defines, whose initial values
    `__zp_init` stores. `fixed` maps each function and object placed
    with `address(addr)` at $100 or above to its address. Those only
    need an `EQU` line, so they're kept out of `addrs`, which the
    zero-page peepholes read. `layout` is where the runtime's
    zero-page block goes (`passes/runtime_layout.py`). `sizes` gives
    each object's size, `pinned` names the objects declared with an
    address, and `external` maps each `extern` object with neither an
    address nor a definition here to its size; another unit defines
    it and the linker places it. `promoted` maps each object
    `--optimize` moved into zero page to its weight (see "Promotion
    under `--optimize`")."""
    addrs: dict[str, int] = field(default_factory=dict)
    reserved: frozenset[int] = frozenset()
    owned: frozenset[str] = frozenset()
    fixed: dict[str, int] = field(default_factory=dict)
    layout: RuntimeLayout = CLASSIC
    sizes: dict[str, int] = field(default_factory=dict)
    pinned: frozenset[str] = frozenset()
    external: dict[str, int] = field(default_factory=dict)
    promoted: dict[str, int] = field(default_factory=dict)

    def equs(self) -> dict[str, int]:
        """Every name this unit binds with an `EQU` line, the runtime's
        SSP, FP, HARGS, and DPTR included."""
        return {**self.layout.addrs(), **self.addrs, **self.fixed}


def allocate_zp_globals(
    symbols, types=None, hargs_need=None, weights=None,
) -> ZpGlobals:
    """Gives every `zeropage` object in `symbols` an address: the one
    it was declared with, or the lowest free run below `$80`. Fixed
    addresses are placed first, in declaration order, then the
    runtime's block (`runtime_layout.choose_layout`, which calls
    `hargs_need` for the number of HARGS bytes the program needs if
    the block has to move), then the rest. An `extern` with no address
    and no definition goes in `external` instead. With `weights` (the
    `access_weights` of the unit), globals that pay for it are then
    promoted into the bytes left (see "Promotion under `--optimize`").
    Raises
    `ZpGlobalError` for an address out of range, an overlap, or a full
    range, and `RuntimeLayoutError` when the runtime's block doesn't
    fit.

    Also collects the `address(addr)` functions and objects outside
    zero page into `fixed` (see "Fixed addresses outside zero page"
    above)."""
    from passes.replace_pseudoregisters import sizeof
    from passes.type_checking import FunAttr, NoInitializer, StaticAttr

    fixed: list[tuple[str, int, int]] = []
    auto: list[tuple[str, int]] = []
    owned: set[str] = set()
    sizes: dict[str, int] = {}
    external: dict[str, int] = {}
    elsewhere = _fixed_elsewhere(symbols, types, sizeof)
    for name, sym in symbols.items():
        attrs = sym.attrs
        if isinstance(attrs, FunAttr) and attrs.address is not None:
            elsewhere[name] = attrs.address
        if not (isinstance(attrs, StaticAttr) and attrs.zeropage):
            continue
        size = sizeof(sym.type, types)
        defined = not isinstance(attrs.initial_value, NoInitializer)
        if defined:
            owned.add(name)
        if attrs.zp_address is None:
            if not defined:
                external[name] = size
                continue
            auto.append((name, size))
        else:
            fixed.append((name, attrs.zp_address, size))
        sizes[name] = size

    used: dict[int, str] = {}
    addrs: dict[str, int] = {}

    def claim(name: str, addr: int, size: int) -> None:
        for b in range(addr, addr + size):
            if b in used:
                raise ZpGlobalError(
                    f"zero-page object {name!r} at ${addr:02X} overlaps "
                    f"{used[b]!r} at byte ${b:02X}"
                )
        for b in range(addr, addr + size):
            used[b] = name
        addrs[name] = addr

    for name, addr, size in fixed:
        if addr + size > 0x100:
            raise ZpGlobalError(
                f"zero-page object {name!r} ({size} bytes at "
                f"${addr:02X}) runs past $FF"
            )
        claim(name, addr, size)
    layout = choose_layout(
        frozenset(used), hargs_need or (lambda: CLASSIC.hargs_size),
    )
    reserved = frozenset(used)
    taken = set(used) | layout.bytes()
    for name, size in auto:
        addr = _first_free(taken, size)
        if addr is None:
            raise ZpGlobalError(
                f"no room for zero-page object {name!r} ({size} bytes) "
                f"below ${AUTO_HI:02X}, after the runtime's block and "
                f"the fixed zero-page objects"
            )
        claim(name, addr, size)
        taken.update(range(addr, addr + size))
        reserved = frozenset(used)
    promoted: dict[str, int] = {}
    for name, weight, size in _promotion_order(symbols, types, weights):
        addr = _first_free(taken, size)
        if addr is None:
            continue
        claim(name, addr, size)
        taken.update(range(addr, addr + size))
        owned.add(name)
        sizes[name] = size
        promoted[name] = weight
    reserved = frozenset(used)
    return ZpGlobals(
        addrs=addrs, reserved=reserved, owned=frozenset(owned),
        fixed=elsewhere, layout=layout, sizes=sizes,
        pinned=frozenset(name for name, _, _ in fixed), external=external,
        promoted=promoted,
    )


# A candidate needs this much weight per byte to be promoted.
_MIN_WEIGHT_PER_BYTE = 3


def _promotion_order(symbols, types, weights) -> list[tuple[str, int, int]]:
    """`(name, weight, size)` of each global worth promoting, densest
    first (weight per byte, then name). Empty without `weights`."""
    from c99_ast import (
        Array, Const, FunType, Structure, Union, Volatile,
    )
    from passes.replace_pseudoregisters import sizeof
    from passes.type_checking import NoInitializer, StaticAttr

    if not weights:
        return []
    out = []
    for name, sym in symbols.items():
        attrs = sym.attrs
        if not isinstance(attrs, StaticAttr) or attrs.zeropage:
            continue
        if attrs.address is not None:
            continue
        if isinstance(attrs.initial_value, NoInitializer):
            continue
        t = sym.type
        while isinstance(t, Volatile):
            t = t.referenced_type
        if isinstance(t, (Const, Array, Structure, Union, FunType)):
            continue
        size = sizeof(sym.type, types)
        weight = weights.get(name, 0)
        if size == 0 or weight < _MIN_WEIGHT_PER_BYTE * size:
            continue
        out.append((name, weight, size))
    out.sort(key=lambda c: (-c[1] / c[2], c[0]))
    return out


def access_weights(tac) -> dict[str, int]:
    """Each name's weighted reference count in the TAC program `tac`:
    every mention in an instruction other than `GetAddress`, `Label`,
    or a jump's target, times 8 per loop around it, up to 3 deep. A
    loop is the span from a label to a jump back to it."""
    import dataclasses
    import tac_ast

    out: dict[str, int] = {}

    def names(node, found: list[str]) -> None:
        if isinstance(node, str):
            found.append(node)
        elif isinstance(node, list):
            for x in node:
                names(x, found)
        elif dataclasses.is_dataclass(node):
            for f in dataclasses.fields(node):
                if f.name != "target":
                    names(getattr(node, f.name), found)

    for fn in tac.top_level:
        if not isinstance(fn, tac_ast.Function):
            continue
        instrs = fn.instructions
        label_at = {
            ins.name: k for k, ins in enumerate(instrs)
            if isinstance(ins, tac_ast.Label)
        }
        depth = [0] * (len(instrs) + 1)
        for k, ins in enumerate(instrs):
            start = label_at.get(getattr(ins, "target", None))
            if start is not None and start < k:
                depth[start] += 1
                depth[k + 1] -= 1
        level = 0
        for k, ins in enumerate(instrs):
            level += depth[k]
            if isinstance(ins, (tac_ast.Label, tac_ast.GetAddress)):
                continue
            found: list[str] = []
            names(ins, found)
            for name in found:
                out[name] = out.get(name, 0) + 8 ** min(level, 3)
    return out


def _fixed_elsewhere(symbols, types, sizeof) -> dict[str, int]:
    """The objects placed with `address(addr)` at $100 or above,
    mapped to their addresses. Raises `ZpGlobalError` when one runs
    past $FFFF or two overlap."""
    from passes.type_checking import StaticAttr

    used: dict[int, str] = {}
    out: dict[str, int] = {}
    for name, sym in symbols.items():
        attrs = sym.attrs
        if not (isinstance(attrs, StaticAttr) and attrs.address is not None
                and attrs.address >= 0x100):
            continue
        addr = attrs.address
        size = sizeof(sym.type, types)
        if addr + size > 0x10000:
            raise ZpGlobalError(
                f"object {name!r} ({size} bytes at ${addr:04X}) runs "
                f"past $FFFF"
            )
        for b in range(addr, addr + size):
            if b in used:
                raise ZpGlobalError(
                    f"object {name!r} at ${addr:04X} overlaps "
                    f"{used[b]!r} at byte ${b:04X}"
                )
            used[b] = name
        out[name] = addr
    return out


def plan_zero_page(tac, symbols, types, promote: bool = False) -> ZpGlobals:
    """`allocate_zp_globals` for a translation unit whose unoptimized
    TAC is `tac`. If the runtime's block has to move, HARGS is sized
    by translating `tac` to asm and measuring it
    (`runtime_layout.hargs_need`). `promote` (under `--optimize`)
    also promotes globals into zero page."""
    def need() -> int:
        from passes.runtime_layout import hargs_need
        from tac_to_asm import translate_program
        return hargs_need(translate_program(tac, symbols, types))
    return allocate_zp_globals(
        symbols, types, hargs_need=need,
        weights=access_weights(tac) if promote else None,
    )


@contextmanager
def zero_page_in_effect(zp: ZpGlobals):
    """Makes `zp`'s zero-page objects and runtime layout the ones the
    asm passes see, for the duration of a compile."""
    from passes.asm_aliasing import set_user_zp_globals
    from passes.runtime_layout import set_current
    set_user_zp_globals(zp.addrs, zp.reserved)
    set_current(zp.layout)
    try:
        yield
    finally:
        set_user_zp_globals({})
        set_current(CLASSIC)


def _first_free(taken, size: int) -> int | None:
    for addr in range(0, AUTO_HI - size + 1):
        if all(b not in taken for b in range(addr, addr + size)):
            return addr
    return None


def place_zp_globals(
    prog: asm_ast.Program, zp: ZpGlobals,
) -> asm_ast.Program:
    """Drops the `StaticVariable` of every zero-page object and adds
    `__zp_init`, which stores their initial bytes. Stores of the same
    value share one `LDA`, zero first. A program with no zero-page
    object comes back unchanged. An object whose `StaticVariable` an
    earlier pass dropped (nothing referenced it) isn't initialized.

    Also checks that `prog` fits the HARGS size its runtime layout
    was given (`runtime_layout.check_hargs`)."""
    from passes.runtime_layout import check_hargs
    check_hargs(prog, zp.layout)
    if not zp.addrs:
        return prog
    stores: dict[str, list[asm_ast.Type_operand]] = {}
    order: list[str] = []
    top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.StaticVariable) and tl.name in zp.addrs:
            if tl.name in zp.owned:
                for k, src in enumerate(_init_operands(tl.init)):
                    key = repr(src)
                    if key not in stores:
                        stores[key] = [src]
                        order.append(key)
                    stores[key].append(
                        asm_ast.Data(name=tl.name, offset=k),
                    )
            continue
        top.append(tl)
    if not order:
        return asm_ast.Program(top_level=top)
    zero = repr(asm_ast.Imm(value=0))
    order.sort(key=lambda k: k != zero)
    a = asm_ast.Reg(reg=asm_ast.A())
    body: list[asm_ast.Type_instruction] = []
    for key in order:
        src, *dsts = stores[key]
        body.append(asm_ast.Mov(src=src, dst=a, is_volatile=False))
        for d in dsts:
            body.append(asm_ast.Mov(src=a, dst=d, is_volatile=False))
    body.append(asm_ast.Return(save_a=False, save_x=False))
    top.append(asm_ast.Function(
        name=INIT_FUNCTION, is_global=True, params=[], instructions=body,
    ))
    return asm_ast.Program(top_level=top)


def link_records(prog: asm_ast.Program, zp: ZpGlobals) -> list:
    """The link metadata's `zp` records for the zero-page objects of
    `prog` (before `place_zp_globals`), in address order then the
    external ones by name. A defined object's initial bytes are
    rendered as `asm_emit` writes them (`$05`, `<buf`, `>(buf+2)`), so
    the linker can store them without knowing the init kinds. An
    object whose `StaticVariable` an earlier pass dropped has none."""
    from passes.zp_link_metadata import ZpObjectMeta

    inits = {
        tl.name: tl.init for tl in prog.top_level
        if isinstance(tl, asm_ast.StaticVariable)
    }
    internal = {
        tl.name for tl in prog.top_level
        if isinstance(tl, asm_ast.StaticVariable) and not tl.is_global
    }

    def text(op) -> str:
        if isinstance(op, asm_ast.Imm):
            return f"${op.value & 0xFF:02X}"
        half = "<" if isinstance(op, asm_ast.ImmLabelLow) else ">"
        if op.offset == 0:
            return f"{half}{op.name}"
        return f"{half}({op.name}+{op.offset})"

    out = []
    for name, addr in sorted(zp.addrs.items(), key=lambda kv: (kv[1], kv[0])):
        defined = name in zp.owned
        init = inits.get(name) if defined else None
        out.append(ZpObjectMeta(
            name=name, size=zp.sizes[name],
            at=addr if name in zp.pinned else None, defined=defined,
            init=[] if init is None else [text(o) for o in _init_operands(init)],
            weight=zp.promoted.get(name),
            local=name in internal,
        ))
    for name in sorted(zp.external):
        out.append(ZpObjectMeta(
            name=name, size=zp.external[name], at=None, defined=False,
        ))
    return out


def _init_operands(init) -> list[asm_ast.Type_operand]:
    """The source operand of each initial byte: an `Imm`, or for an
    address initializer the `ImmLabelLow` / `ImmLabelHigh` of the
    label."""
    out: list[asm_ast.Type_operand] = []

    def num(value: int, n: int) -> None:
        value &= (1 << (8 * n)) - 1
        out.extend(
            asm_ast.Imm(value=(value >> (8 * k)) & 0xFF) for k in range(n)
        )

    for item in init:
        match item:
            case asm_ast.CharInit(value=v):
                num(v, 1)
            case asm_ast.IntInit(value=v):
                num(v, 2)
            case asm_ast.LongInit(value=v) | asm_ast.FloatInit(bits=v):
                num(v, 4)
            case asm_ast.LongLongInit(value=v) | asm_ast.DoubleInit(bits=v):
                num(v, 8)
            case asm_ast.AddressInit(name=n, offset=off):
                out.append(asm_ast.ImmLabelLow(name=n, offset=off))
                out.append(asm_ast.ImmLabelHigh(name=n, offset=off))
            case asm_ast.StringInit(str=s, bytes=n):
                out.extend(asm_ast.Imm(value=ord(c) & 0xFF) for c in s)
                out.extend(asm_ast.Imm(value=0) for _ in range(n - len(s)))
            case asm_ast.ZeroInit(bytes=n):
                out.extend(asm_ast.Imm(value=0) for _ in range(n))
            case _:
                raise TypeError(f"unexpected static_init: {item!r}")
    return out
