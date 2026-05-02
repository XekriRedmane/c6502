"""Runtime stub for the simulator: zero-page reservations, boot stub,
reset vector, and Python-implemented hooks for the 6502 helpers
(`mul*` / `divmod*` / `asl*` / `asr*` / `lsr*`, plus FP slots).

Each helper is given a fixed trap address. The harness intercepts PC
at a trap address — running the Python implementation against the
simulator's memory and synthesizing an RTS — instead of executing
instructions there. So no bytes need to be installed at the trap
addresses; only the symbol → address binding has to be added to the
assembler's symbol table before the user program is assembled, so
`Call("mul16")` resolves to `JSR $E0XX`.

Memory map:
  $0000-$001F  zero page (SSP/FP/HARGS/DPTR)
  $0100-$01FF  6502 hardware stack
  $0600-$06FF  boot stub
  $0800-...    program code + statics (assembler.origin)
  $E000-$E1FF  helper trap region (one address per helper, 16 bytes apart)
  $FFFC-$FFFD  reset vector → boot stub

SSP starts at `$7FFF` and grows downward — well clear of both the
program region above $0800 and the trap region at $E000.

Boot stub at $0600:
    LDA #$FF         ; SSP low
    STA SSP
    LDA #$7F         ; SSP high
    STA SSP+1
    JSR main
    BRK              ; halts the simulator (the harness stops on BRK)

Helpers are mostly modeled on the documented HARGS layouts. Today we
implement unsigned multiply / divide / shift / arithmetic-shift
semantics for the integer family. Multiplication is bitwise-correct
for both signed and unsigned C operands at the *truncated* widths the
caller uses (low N bytes of the product), which matches the way
`tac_to_asm` consumes the result. Unsigned division is bitwise-correct
for unsigned C operands; signed division on negatives currently gives
wrong results — there's no signed/unsigned routing in `tac_to_asm`
today, so this is an open question for the eventual real runtime, not
a simulator-specific limitation.

FP helpers (`i2f`, `u2f`, `f2i`, `f2d`, etc.) are registered with trap
addresses for symbol resolution but their hooks raise
NotImplementedError if called — programs that use FP arithmetic will
fault loudly instead of silently producing garbage.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Callable


# Zero-page reservations.
SSP = 0x00       # SSP+1 = $01
FP = 0x02        # FP+1 = $03
HARGS = 0x04     # spans $04..$1B (24 bytes)
DPTR = 0x1C      # DPTR+1 = $1D

# Boot stub address and SSP initial value.
BOOT_ADDR = 0x0600
SSP_INIT = 0x7FFF
RESET_VECTOR = 0xFFFC

# Helper trap region.
TRAP_BASE = 0xE000
TRAP_STRIDE = 0x10


# Type alias: a hook reads / writes the simulator's flat memory array.
Hook = Callable[[bytearray], None]


@dataclass
class Runtime:
    """Bundles everything the harness needs to run a user program:
    helper symbol → address bindings (to merge into the assembler's
    symbol table), trap-address → hook bindings (to intercept PC for
    Python-implemented helpers), and the bytes / origin of the
    real-asm helpers (to lay down in the memory image)."""
    symbols: dict[str, int] = field(default_factory=dict)
    hooks: dict[int, Hook] = field(default_factory=dict)
    helper_image: bytes = b""
    helper_origin: int = 0
    boot_addr: int = BOOT_ADDR
    ssp_init: int = SSP_INIT


# -------- HARGS read/write helpers --------


def _read_int(mem: bytearray, addr: int, nbytes: int) -> int:
    """Read an `nbytes`-byte little-endian unsigned integer from
    `addr`. Used to read HARGS slots."""
    v = 0
    for i in range(nbytes):
        v |= mem[addr + i] << (i * 8)
    return v


def _write_int(mem: bytearray, addr: int, val: int, nbytes: int) -> None:
    """Write an `nbytes`-byte little-endian unsigned integer to
    `addr`. The low N bytes of `val` are kept; higher bits are
    discarded — matches how `tac_to_asm` consumes truncated helper
    results."""
    for i in range(nbytes):
        mem[addr + i] = (val >> (i * 8)) & 0xFF


def _signed(val: int, nbytes: int) -> int:
    """Sign-extend an `nbytes`-byte unsigned integer to a Python int."""
    bits = nbytes * 8
    sign = 1 << (bits - 1)
    if val & sign:
        return val - (1 << bits)
    return val


# -------- integer helpers --------


def _make_mul(in_size: int, out_size: int) -> Hook:
    """Multiply two `in_size`-byte unsigned values at HARGS+0 /
    HARGS+in_size, store the `out_size`-byte product at HARGS +
    2*in_size. The low `out_size` bytes of `a*b` are kept — matches
    the bit-pattern semantics for both signed and unsigned C
    multiplications at any width the caller truncates back to."""
    def hook(mem: bytearray) -> None:
        a = _read_int(mem, HARGS + 0, in_size)
        b = _read_int(mem, HARGS + in_size, in_size)
        _write_int(mem, HARGS + 2 * in_size, a * b, out_size)
    return hook


def _make_udivmod(in_size: int) -> Hook:
    """Unsigned divide of two `in_size`-byte values. Quotient at HARGS
    + 2*in_size, remainder at HARGS + 3*in_size. Division by zero
    raises (matches C99's UB but at least won't silently produce
    garbage)."""
    def hook(mem: bytearray) -> None:
        n = _read_int(mem, HARGS + 0, in_size)
        d = _read_int(mem, HARGS + in_size, in_size)
        if d == 0:
            raise RuntimeError(f"udivmod{in_size * 8}: division by zero")
        q = n // d
        r = n - q * d
        _write_int(mem, HARGS + 2 * in_size, q, in_size)
        _write_int(mem, HARGS + 3 * in_size, r, in_size)
    return hook


def _make_sdivmod(in_size: int) -> Hook:
    """Signed divide of two `in_size`-byte values per C99 §6.5.5.6:
    `/` truncates toward zero, and `%` returns a remainder with the
    same sign as the dividend (so `(a/b)*b + a%b == a` always
    holds). Quotient at HARGS+2*in_size, remainder at HARGS+3*in_size.
    Python's `//` floors, so we sign-correct: floor-div agrees with
    trunc-div when the result is non-negative; when negative we
    bump by 1 toward zero and adjust the remainder to match."""
    def hook(mem: bytearray) -> None:
        n_raw = _read_int(mem, HARGS + 0, in_size)
        d_raw = _read_int(mem, HARGS + in_size, in_size)
        if d_raw == 0:
            raise RuntimeError(f"sdivmod{in_size * 8}: division by zero")
        n = _signed(n_raw, in_size)
        d = _signed(d_raw, in_size)
        # C trunc-toward-zero: q = trunc(n / d), r = n - q * d.
        # Python's int division rounds toward negative infinity; the
        # `int` builtin on a float rounds toward zero, but for exact
        # arithmetic we use the explicit construction below to avoid
        # FP precision issues for wide types.
        q = abs(n) // abs(d)
        if (n < 0) ^ (d < 0):
            q = -q
        r = n - q * d
        _write_int(mem, HARGS + 2 * in_size, q, in_size)
        _write_int(mem, HARGS + 3 * in_size, r, in_size)
    return hook


def _make_asl(value_size: int) -> Hook:
    """Logical shift left. Value at HARGS+0..value_size-1, count at
    HARGS+value_size (1 byte), result at HARGS+value_size+1.
    Counts >= width-in-bits produce 0 (UB; we pick the most useful
    interpretation for testing)."""
    def hook(mem: bytearray) -> None:
        v = _read_int(mem, HARGS + 0, value_size)
        c = mem[HARGS + value_size]
        if c >= value_size * 8:
            r = 0
        else:
            r = (v << c) & ((1 << (value_size * 8)) - 1)
        _write_int(mem, HARGS + value_size + 1, r, value_size)
    return hook


def _make_lsr(value_size: int) -> Hook:
    """Logical shift right (zero-fill)."""
    def hook(mem: bytearray) -> None:
        v = _read_int(mem, HARGS + 0, value_size)
        c = mem[HARGS + value_size]
        if c >= value_size * 8:
            r = 0
        else:
            r = (v >> c) & ((1 << (value_size * 8)) - 1)
        _write_int(mem, HARGS + value_size + 1, r, value_size)
    return hook


def _make_asr(value_size: int) -> Hook:
    """Arithmetic shift right (sign-fill). The value's MSB is
    replicated into the high bits."""
    def hook(mem: bytearray) -> None:
        v = _read_int(mem, HARGS + 0, value_size)
        c = mem[HARGS + value_size]
        sv = _signed(v, value_size)
        if c >= value_size * 8:
            c = value_size * 8 - 1   # full sign-extend
        r = (sv >> c) & ((1 << (value_size * 8)) - 1)
        _write_int(mem, HARGS + value_size + 1, r, value_size)
    return hook


# -------- FP helpers (placeholders) --------


def _fp_unimplemented(name: str) -> Hook:
    def hook(mem: bytearray) -> None:
        raise NotImplementedError(
            f"FP helper {name!r} is not implemented in the simulator yet"
        )
    return hook


# -------- the helper table --------
#
# Each entry is (symbol_name, hook_factory_call). The order pins each
# helper to a stable trap address (TRAP_BASE + index * TRAP_STRIDE) so
# binary diffs of test outputs stay small as the table grows.

_HELPERS: list[tuple[str, Hook]] = [
    # Integer mul / divmod / shift helpers. Each `mul*` returns
    # only the low N bytes of the product (i.e. the result at the
    # operand width) — same modular-wrap argument as for mul8: C's
    # int-times-int wraps to int under §6.5.5.4 semantics, and
    # `tac_to_asm` only reads `output_size = N` bytes regardless.
    # The high half is freed (HARGS+3 for mul8, HARGS+6..7 for
    # mul16, HARGS+12..15 for mul32) for other uses.
    # 1-byte integer
    ("mul8",      _make_mul(1, 1)),
    ("udivmod8",  _make_udivmod(1)),
    ("sdivmod8",  _make_sdivmod(1)),
    ("asl8",      _make_asl(1)),
    ("asr8",      _make_asr(1)),
    ("lsr8",      _make_lsr(1)),
    # 2-byte integer
    ("mul16",     _make_mul(2, 2)),
    ("udivmod16", _make_udivmod(2)),
    ("sdivmod16", _make_sdivmod(2)),
    ("asl16",     _make_asl(2)),
    ("asr16",     _make_asr(2)),
    ("lsr16",     _make_lsr(2)),
    # 4-byte integer
    ("mul32",     _make_mul(4, 4)),
    ("udivmod32", _make_udivmod(4)),
    ("sdivmod32", _make_sdivmod(4)),
    ("asl32",     _make_asl(4)),
    ("asr32",     _make_asr(4)),
    ("lsr32",     _make_lsr(4)),
    # FP integer→float
    ("i2f",  _fp_unimplemented("i2f")),
    ("u2f",  _fp_unimplemented("u2f")),
    ("l2f",  _fp_unimplemented("l2f")),
    ("ul2f", _fp_unimplemented("ul2f")),
    ("ll2f", _fp_unimplemented("ll2f")),
    ("ull2f", _fp_unimplemented("ull2f")),
    # FP integer→double
    ("i2d",  _fp_unimplemented("i2d")),
    ("u2d",  _fp_unimplemented("u2d")),
    ("l2d",  _fp_unimplemented("l2d")),
    ("ul2d", _fp_unimplemented("ul2d")),
    ("ll2d", _fp_unimplemented("ll2d")),
    ("ull2d", _fp_unimplemented("ull2d")),
    # FP float→integer
    ("f2i",  _fp_unimplemented("f2i")),
    ("f2u",  _fp_unimplemented("f2u")),
    ("f2l",  _fp_unimplemented("f2l")),
    ("f2ul", _fp_unimplemented("f2ul")),
    ("f2ll", _fp_unimplemented("f2ll")),
    ("f2ull", _fp_unimplemented("f2ull")),
    # FP double→integer
    ("d2i",  _fp_unimplemented("d2i")),
    ("d2u",  _fp_unimplemented("d2u")),
    ("d2l",  _fp_unimplemented("d2l")),
    ("d2ul", _fp_unimplemented("d2ul")),
    ("d2ll", _fp_unimplemented("d2ll")),
    ("d2ull", _fp_unimplemented("d2ull")),
    # FP cross-precision
    ("f2d", _fp_unimplemented("f2d")),
    ("d2f", _fp_unimplemented("d2f")),
    # FP arithmetic
    ("fadd", _fp_unimplemented("fadd")),
    ("fsub", _fp_unimplemented("fsub")),
    ("fmul", _fp_unimplemented("fmul")),
    ("fdiv", _fp_unimplemented("fdiv")),
    ("dadd", _fp_unimplemented("dadd")),
    ("dsub", _fp_unimplemented("dsub")),
    ("dmul", _fp_unimplemented("dmul")),
    ("ddiv", _fp_unimplemented("ddiv")),
]


def build_runtime() -> Runtime:
    """Build a Runtime with helper bindings.

    Helpers split into two pools:
      - Real-asm helpers (`runtime_helpers.ASM_IMPLEMENTED`):
        assembled into `helper_image` at `TRAP_BASE` (= $E000), with
        their resolved addresses recorded in `symbols`. Real 6502
        code runs when the user program JSRs to one of these names.
      - Python-hook helpers (everything in `_HELPERS` not in the
        asm-implemented set): assigned trap addresses past the end of
        the assembled image, with `hooks` recording the Python
        implementation. The harness intercepts PC at a trap address
        and calls the hook in lieu of fetching opcodes.

    Both pools share the same `symbols` table, so user code links
    uniformly via the helper name regardless of which pool fulfills
    it. Adding a helper to `runtime_helpers.ASM_IMPLEMENTED` flips
    it from Python to real asm without any changes elsewhere."""
    from sim.runtime_helpers import ASM_IMPLEMENTED, all_helper_functions
    from sim.assembler import assemble
    import asm_ast

    rt = Runtime(helper_origin=TRAP_BASE)

    # Assemble the real-asm helpers into a contiguous chunk at
    # TRAP_BASE. The resulting symbol table maps each helper's name
    # to its real address.
    asm_set = set(ASM_IMPLEMENTED)
    if asm_set:
        prog = asm_ast.Program(top_level=list(all_helper_functions()))
        assembled = assemble(prog, origin=TRAP_BASE)
        for name in asm_set:
            if name not in assembled.symbols:
                raise RuntimeError(
                    f"helper {name!r} declared in ASM_IMPLEMENTED "
                    "but not present in assembled output"
                )
            rt.symbols[name] = assembled.symbols[name]
        # Slice exactly the helper region (origin → code_end) into
        # `helper_image`; `install_runtime` lays it down at
        # `helper_origin`.
        rt.helper_image = bytes(
            assembled.image[TRAP_BASE:assembled.code_end]
        )
        trap_cursor = assembled.code_end
    else:
        trap_cursor = TRAP_BASE

    # Python-trap addresses for the rest. Round trap_cursor up to the
    # next 16-byte boundary so trap addresses stay readable.
    trap_cursor = (trap_cursor + 0xF) & ~0xF
    for name, fn in _HELPERS:
        if name in asm_set:
            continue
        rt.symbols[name] = trap_cursor
        rt.hooks[trap_cursor] = fn
        trap_cursor += TRAP_STRIDE
    return rt


# -------- boot stub --------


def build_boot_stub(main_addr: int) -> bytes:
    """Assemble the boot stub at `BOOT_ADDR`:

        LDA  #<SSP_INIT     ; A9 FF
        STA  SSP            ; 85 00
        LDA  #>SSP_INIT     ; A9 7F
        STA  SSP+1          ; 85 01
        JSR  main           ; 20 <lo> <hi>
        BRK                 ; 00

    Total 12 bytes. The harness stops the simulator when PC points at
    a BRK opcode (so the BRK byte itself is what halts execution after
    `main` returns)."""
    if not 0 <= main_addr <= 0xFFFF:
        raise ValueError(f"main_addr {main_addr} out of range")
    lo = main_addr & 0xFF
    hi = (main_addr >> 8) & 0xFF
    return bytes([
        0xA9, SSP_INIT & 0xFF,         # LDA #<SSP_INIT
        0x85, SSP,                      # STA SSP
        0xA9, (SSP_INIT >> 8) & 0xFF,   # LDA #>SSP_INIT
        0x85, SSP + 1,                  # STA SSP+1
        0x20, lo, hi,                   # JSR main
        0x00,                           # BRK
    ])


def install_runtime(
    image: bytearray, runtime: Runtime, main_addr: int,
) -> None:
    """Lay down the boot stub, the reset vector, and the assembled
    helper image (if any). Trap addresses for Python-hook helpers
    don't get any bytes — the harness intercepts PC at those
    addresses before any opcode is fetched there."""
    boot = build_boot_stub(main_addr)
    image[runtime.boot_addr:runtime.boot_addr + len(boot)] = boot
    # Reset vector at $FFFC/$FFFD points at the boot stub. The 6502
    # fetches PC from this on power-on / RESET.
    image[RESET_VECTOR] = runtime.boot_addr & 0xFF
    image[RESET_VECTOR + 1] = (runtime.boot_addr >> 8) & 0xFF
    # Real-asm helpers (assembled into runtime.helper_image).
    if runtime.helper_image:
        end = runtime.helper_origin + len(runtime.helper_image)
        image[runtime.helper_origin:end] = runtime.helper_image
