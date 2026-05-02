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


# -------- FP helpers --------


def _fp_unimplemented(name: str) -> Hook:
    def hook(mem: bytearray) -> None:
        raise NotImplementedError(
            f"FP helper {name!r} is not implemented in the simulator yet"
        )
    return hook


# IEEE 754 single / double (float / double) arithmetic helpers,
# implemented via Python's built-in float — i.e. via the host's
# native FP unit. These are stand-ins until the actual 6502 asm
# implementations land in `sim/runtime_helpers.py`. They give
# end-to-end FP correctness for the chapter tests today; the asm
# versions are what'll let real c6502 binaries run.
import struct as _struct


def _fp_arith(in_size: int, op: str) -> Hook:
    """Read two `in_size`-byte IEEE 754 operands from HARGS+0..N-1
    and HARGS+N..2N-1, compute `A op B` via Python float, and write
    the `in_size`-byte result to the matching FP-result slot
    (HARGS+8..11 for Float, HARGS+16..23 for Double — the same slot
    the FP-returning calling convention uses for its return).
    `op` is one of `+`, `-`, `*`, `/`."""
    fmt = "<f" if in_size == 4 else "<d"
    out_off = 8 if in_size == 4 else 16

    def hook(mem: bytearray) -> None:
        a = _struct.unpack(fmt, bytes(mem[HARGS:HARGS + in_size]))[0]
        b = _struct.unpack(
            fmt, bytes(mem[HARGS + in_size:HARGS + 2 * in_size])
        )[0]
        if op == "+":
            r = a + b
        elif op == "-":
            r = a - b
        elif op == "*":
            r = a * b
        elif op == "/":
            # Division by zero: produce IEEE 754 ±inf or NaN. Python's
            # plain `/` would raise ZeroDivisionError; emulate IEEE
            # behavior explicitly.
            if b == 0.0:
                if a == 0.0:
                    r = float("nan")
                elif a < 0.0:
                    r = float("-inf")
                else:
                    r = float("inf")
            else:
                r = a / b
        else:
            raise ValueError(f"unknown FP op {op!r}")
        packed = _struct.pack(fmt, r)
        mem[HARGS + out_off:HARGS + out_off + in_size] = packed
    return hook


def _int_to_fp(int_size: int, signed: bool, fp_size: int) -> Hook:
    """Read an `int_size`-byte integer (signed or unsigned per
    `signed`) at HARGS+0..int_size-1 and write its `fp_size`-byte
    FP representation to HARGS+int_size..int_size+fp_size-1.

    The output offset matches the helper's documented layout:
    output starts immediately after the input slot. e.g.
      i2f:  in HARGS+0,    out HARGS+1..4
      l2d:  in HARGS+0..1, out HARGS+2..9
      ll2f: in HARGS+0..3, out HARGS+4..7
    """
    fmt = "<f" if fp_size == 4 else "<d"

    def hook(mem: bytearray) -> None:
        raw = _read_int(mem, HARGS + 0, int_size)
        if signed:
            raw = _signed(raw, int_size)
        packed = _struct.pack(fmt, float(raw))
        mem[HARGS + int_size:HARGS + int_size + fp_size] = packed
    return hook


def _fp_to_int(fp_size: int, int_size: int, signed: bool) -> Hook:
    """Read an `fp_size`-byte FP value at HARGS+0..fp_size-1 and
    truncate to an `int_size`-byte integer at HARGS+fp_size..
    fp_size+int_size-1. Truncation is toward zero per C99 §6.3.1.4;
    NaN / Inf and out-of-range values are UB and produce 0 here."""
    fmt = "<f" if fp_size == 4 else "<d"
    mask = (1 << (int_size * 8)) - 1

    def hook(mem: bytearray) -> None:
        v = _struct.unpack(fmt, bytes(mem[HARGS:HARGS + fp_size]))[0]
        try:
            i = int(v)   # truncates toward zero
        except (OverflowError, ValueError):
            i = 0
        # Two's-complement wrap; signedness rides on how the caller
        # interprets the bytes. Bit pattern is identical for the two
        # cases since we mask to the integer's width.
        i &= mask
        _write_int(mem, HARGS + fp_size, i, int_size)
    return hook


def _f2d_hook() -> Hook:
    """Float (4B) → Double (8B). Input HARGS+0..3, output HARGS+4..11."""
    def hook(mem: bytearray) -> None:
        v = _struct.unpack("<f", bytes(mem[HARGS:HARGS + 4]))[0]
        packed = _struct.pack("<d", v)
        mem[HARGS + 4:HARGS + 12] = packed
    return hook


def _d2f_hook() -> Hook:
    """Double (8B) → Float (4B). Input HARGS+0..7, output HARGS+8..11."""
    def hook(mem: bytearray) -> None:
        v = _struct.unpack("<d", bytes(mem[HARGS:HARGS + 8]))[0]
        packed = _struct.pack("<f", v)
        mem[HARGS + 8:HARGS + 12] = packed
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
    # FP integer→float (output at HARGS + int_size, fp_size = 4)
    ("i2f",   _int_to_fp(int_size=1, signed=True,  fp_size=4)),
    ("u2f",   _int_to_fp(int_size=1, signed=False, fp_size=4)),
    ("l2f",   _int_to_fp(int_size=2, signed=True,  fp_size=4)),
    ("ul2f",  _int_to_fp(int_size=2, signed=False, fp_size=4)),
    ("ll2f",  _int_to_fp(int_size=4, signed=True,  fp_size=4)),
    ("ull2f", _int_to_fp(int_size=4, signed=False, fp_size=4)),
    # FP integer→double (output at HARGS + int_size, fp_size = 8)
    ("i2d",   _int_to_fp(int_size=1, signed=True,  fp_size=8)),
    ("u2d",   _int_to_fp(int_size=1, signed=False, fp_size=8)),
    ("l2d",   _int_to_fp(int_size=2, signed=True,  fp_size=8)),
    ("ul2d",  _int_to_fp(int_size=2, signed=False, fp_size=8)),
    ("ll2d",  _int_to_fp(int_size=4, signed=True,  fp_size=8)),
    ("ull2d", _int_to_fp(int_size=4, signed=False, fp_size=8)),
    # FP float→integer (input HARGS+0..3, output starts at HARGS+4)
    ("f2i",   _fp_to_int(fp_size=4, int_size=1, signed=True)),
    ("f2u",   _fp_to_int(fp_size=4, int_size=1, signed=False)),
    ("f2l",   _fp_to_int(fp_size=4, int_size=2, signed=True)),
    ("f2ul",  _fp_to_int(fp_size=4, int_size=2, signed=False)),
    ("f2ll",  _fp_to_int(fp_size=4, int_size=4, signed=True)),
    ("f2ull", _fp_to_int(fp_size=4, int_size=4, signed=False)),
    # FP double→integer (input HARGS+0..7, output starts at HARGS+8)
    ("d2i",   _fp_to_int(fp_size=8, int_size=1, signed=True)),
    ("d2u",   _fp_to_int(fp_size=8, int_size=1, signed=False)),
    ("d2l",   _fp_to_int(fp_size=8, int_size=2, signed=True)),
    ("d2ul",  _fp_to_int(fp_size=8, int_size=2, signed=False)),
    ("d2ll",  _fp_to_int(fp_size=8, int_size=4, signed=True)),
    ("d2ull", _fp_to_int(fp_size=8, int_size=4, signed=False)),
    # FP cross-precision
    ("f2d", _f2d_hook()),
    ("d2f", _d2f_hook()),
    # FP arithmetic
    ("fadd", _fp_arith(4, "+")),
    ("fsub", _fp_arith(4, "-")),
    ("fmul", _fp_arith(4, "*")),
    ("fdiv", _fp_arith(4, "/")),
    ("dadd", _fp_arith(8, "+")),
    ("dsub", _fp_arith(8, "-")),
    ("dmul", _fp_arith(8, "*")),
    ("ddiv", _fp_arith(8, "/")),
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
