"""Zero-page globals: `__attribute__((zeropage))`.

A static-storage object declared with `__attribute__((zeropage))`
lives in zero page instead of the data segment, so every access is a
2-byte zero-page instruction (`LDA zp`, 3 cycles) where an ordinary
global takes a 3-byte absolute one (`LDA abs`, 4 cycles). A pointer
in zero page can also serve directly as the base of an `(zp),Y`
access, where an ordinary pointer global is first copied to DPTR.

    uint8_t num_entries __attribute__((zeropage));            // $26
    __attribute__((zeropage(0xBA))) uint8_t *z_header_addr;   // $BA-$BB

The bare form takes the lowest free bytes in `$26`-`$7F` (`AUTO_LO`
.. `AUTO_HI`), the range between the runtime's reserved bytes
(`$00`-`$25`: SSP, FP, HARGS, DPTR) and the compiler's own zero-page
pool (`$80`-`$FF`). The address form puts the object's first (low)
byte at that address, anywhere in `$26`-`$FF`, so code ported from
hand-written assembly can
keep its documented addresses. A fixed address in `$80`-`$FF` is taken
out of the compiler's pool: `reserved` is passed to the zp_abi slot
allocator, the private-pool allocator, and register allocation, so
nothing else is placed on those bytes. Two zero-page objects may not
overlap, and an object must fit below `$100`.

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
`extern` declaration without an address has to be defined in the
unit, since only the definition decides where it goes.

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

Linking several translation units (`compile.py --link`) isn't
supported yet for programs with zero-page globals; the linker
rejects them (see TODO.md).

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

from dataclasses import dataclass, field

import asm_ast
from passes.type_checking import TypeCheckError

# The range a bare `zeropage` object is placed in.
AUTO_LO = 0x26
AUTO_HI = 0x80
# The lowest address a fixed `zeropage(addr)` may use. Below it are
# the runtime's SSP, FP, HARGS, and DPTR.
FIXED_LO = 0x26

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
    zero-page peepholes read."""
    addrs: dict[str, int] = field(default_factory=dict)
    reserved: frozenset[int] = frozenset()
    owned: frozenset[str] = frozenset()
    fixed: dict[str, int] = field(default_factory=dict)

    def equs(self) -> dict[str, int]:
        """Every name this unit binds with an `EQU` line."""
        return {**self.addrs, **self.fixed}


def allocate_zp_globals(symbols, types=None) -> ZpGlobals:
    """Gives every `zeropage` object in `symbols` an address: the one
    it was declared with, or the lowest free run in `$26`-`$7F`. Fixed
    addresses are placed first, in declaration order, then the rest.
    Raises `ZpGlobalError` for an address out of range, an overlap, an
    `extern` with no address and no definition, or a full range.

    Also collects the `address(addr)` functions and objects outside
    zero page into `fixed` (see "Fixed addresses outside zero page"
    above)."""
    from passes.replace_pseudoregisters import sizeof
    from passes.type_checking import FunAttr, NoInitializer, StaticAttr

    fixed: list[tuple[str, int, int]] = []
    auto: list[tuple[str, int]] = []
    owned: set[str] = set()
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
                raise ZpGlobalError(
                    f"`extern` zero-page object {name!r} needs an "
                    f"address (`zeropage(0x..)`) or a definition in "
                    f"this translation unit"
                )
            auto.append((name, size))
        else:
            fixed.append((name, attrs.zp_address, size))

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
        if addr < FIXED_LO:
            raise ZpGlobalError(
                f"zero-page object {name!r} at ${addr:02X} is in the "
                f"runtime's reserved bytes $00-${FIXED_LO - 1:02X}"
            )
        if addr + size > 0x100:
            raise ZpGlobalError(
                f"zero-page object {name!r} ({size} bytes at "
                f"${addr:02X}) runs past $FF"
            )
        claim(name, addr, size)
    for name, size in auto:
        addr = _first_free(used, size)
        if addr is None:
            raise ZpGlobalError(
                f"no room for zero-page object {name!r} ({size} bytes) "
                f"in ${AUTO_LO:02X}-${AUTO_HI - 1:02X}"
            )
        claim(name, addr, size)
    return ZpGlobals(
        addrs=addrs, reserved=frozenset(used), owned=frozenset(owned),
        fixed=elsewhere,
    )


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


def _first_free(used: dict[int, str], size: int) -> int | None:
    for addr in range(AUTO_LO, AUTO_HI - size + 1):
        if all(b not in used for b in range(addr, addr + size)):
            return addr
    return None


def place_zp_globals(
    prog: asm_ast.Program, zp: ZpGlobals,
) -> asm_ast.Program:
    """Drops the `StaticVariable` of every zero-page object and adds
    `__zp_init`, which stores their initial bytes. Stores of the same
    value share one `LDA`, zero first. A program with no zero-page
    object comes back unchanged. An object whose `StaticVariable` an
    earlier pass dropped (nothing referenced it) isn't initialized."""
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
