"""6502 assembly implementations of the runtime helpers, expressed as
`asm_ast.Function` objects. The simulator's `build_runtime` assembles
these alongside the user program and binds their symbols, so a `JSR
udivmod8` in user code lands on the real 6502 routine instead of a
Python trap.

Each helper follows the documented HARGS layout (see `tac_to_asm.py`'s
helper-constant section):
  - inputs at HARGS+0..N-1 (low offsets) and survive the call
  - outputs at HARGS + (k * width) for k = 2, 3, ... (high offsets)

Helpers below use the IR's `Data(name="HARGS", offset=k)` operands —
`asm_emit` and `sim.assembler` lower those to `<op> HARGS+k` (zp or
abs as the resolved address dictates). The shift / rotate / inc / dec
emit on `Data` operands — added alongside this module — is what
allows these to be expressed cleanly without going through A.

Currently implemented: `mul8`, `mul16`, `mul32`, `udivmod8`,
`sdivmod8`, plus the full shift family `asl{8,16,32}` /
`asr{8,16,32}` / `lsr{8,16,32}`. The 16- and 32-bit divmods stay
as Python hooks in `sim/runtime.py` until they're written here
(same shift-and-subtract algorithm as udivmod8, scaled byte-widths
plus a sign-correction wrapper for sdivmod16 / sdivmod32).
"""

from __future__ import annotations

import asm_ast as a


_REG_A = a.Reg(reg=a.A())
_REG_X = a.Reg(reg=a.X())


def _hargs(k: int) -> a.Data:
    return a.Data(name="HARGS", offset=k)


def _imm(v: int) -> a.Imm:
    return a.Imm(value=v)


def _negate_a() -> list[a.Type_instruction]:
    """Two's-complement negate of the byte in A: A = -A. EOR #$FF
    flips every bit (one's complement); CLC/ADC #$01 adds 1 to
    convert to two's complement. Three instructions, 6 bytes. The
    Xor IR node uses A as both src and dst (its `src1` and `src2`
    convention requires one to be A; the other becomes the
    addressing-mode carrier)."""
    return [
        a.Xor(src1=_REG_A, src2=_imm(0xFF), dst=_REG_A),
        a.ClearCarry(),
        a.Add(src=_imm(0x01), dst=_REG_A),
    ]


def _copy_n_via_a(src_off: int, dst_off: int, n: int) -> list[a.Type_instruction]:
    """Emit `n` LDA / STA pairs to copy `n` bytes from
    HARGS+src_off..src_off+n-1 to HARGS+dst_off..dst_off+n-1."""
    out: list[a.Type_instruction] = []
    for k in range(n):
        out += [
            a.Mov(src=_hargs(src_off + k), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(dst_off + k)),
        ]
    return out


def _negate_n_inplace(off: int, n: int) -> list[a.Type_instruction]:
    """Emit a multi-byte two's-complement negate of HARGS+off..off+n-1.

    Algorithm: invert each byte (EOR #$FF), then add 1 to the low
    byte and propagate carry through the higher bytes. The first
    byte uses CLC + ADC #$01; subsequent bytes use ADC #$00 with
    the carry from the previous chained add. EOR doesn't touch C,
    so the carry survives the LDA / EOR pairs between ADCs."""
    out: list[a.Type_instruction] = [
        a.Mov(src=_hargs(off), dst=_REG_A),
        a.Xor(src1=_REG_A, src2=_imm(0xFF), dst=_REG_A),
        a.ClearCarry(),
        a.Add(src=_imm(0x01), dst=_REG_A),
        a.Mov(src=_REG_A, dst=_hargs(off)),
    ]
    for k in range(1, n):
        out += [
            a.Mov(src=_hargs(off + k), dst=_REG_A),
            a.Xor(src1=_REG_A, src2=_imm(0xFF), dst=_REG_A),
            a.Add(src=_imm(0x00), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(off + k)),
        ]
    return out


def mul8_function() -> a.Function:
    """8-bit unsigned multiply, low-byte result via shift-and-add.

    Inputs at HARGS+0 (A), HARGS+1 (B); output at HARGS+2 (1 byte =
    low byte of A*B). Both inputs are preserved across the call.

    Algorithm — eight iterations:
      A_work := A; B_work := B
      result := 0
      for _ in 8:
          if low bit of A_work: result += B_work
          A_work >>= 1
          B_work <<= 1

    The `LSR HARGS+3 / BCC skip` peels off A's bits LSB-first; each
    set bit triggers `result += B_work`. Then `ASL HARGS+4` doubles
    B_work for the next bit. C's modular `int*int` semantics fall
    out naturally — the carry from the additions doesn't propagate
    past the result's single byte, so overflow just wraps. HARGS+3
    and HARGS+4 are scratch (A_work and B_work copies); HARGS+0..1
    are read once at the start and never written, so they survive
    the call without an explicit save/restore."""
    loop = ".mul8_loop"
    skip = ".mul8_skip"
    return a.Function(
        name="mul8", is_global=True, params=[],
        instructions=[
            # A_work := A; B_work := B (HARGS+3..4 scratch).
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(3)),
            a.Mov(src=_hargs(1), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(4)),
            # result := 0.
            a.Mov(src=_imm(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(2)),
            # X := 8 (loop count).
            a.Mov(src=_imm(8), dst=_REG_X),
            a.Label(name=loop),
            # A_work LSB → C; A_work >>= 1.
            a.LogicalShiftRight(dst=_hargs(3)),
            a.Branch(cond=a.CC(), target=skip),
            # result += B_work.
            a.Mov(src=_hargs(2), dst=_REG_A),
            a.ClearCarry(),
            a.Add(src=_hargs(4), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(2)),
            a.Label(name=skip),
            # B_work <<= 1 for next bit.
            a.ArithmeticShiftLeft(dst=_hargs(4)),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def mul16_function() -> a.Function:
    """16-bit unsigned multiply, low-2-byte result via shift-and-add.

    Inputs at HARGS+0..1 (A), HARGS+2..3 (B); output at HARGS+4..5
    (2 bytes = low half of A*B). Inputs preserved.

    Sixteen iterations of the same scheme as `mul8`, with multi-byte
    shifts (LSR / ROR for >>= 1; ASL / ROL for <<= 1) and a 2-byte
    add chain (CLC + 2 × ADC, carry threading between bytes). Scratch
    at HARGS+6..7 (A_work) and HARGS+8..9 (B_work); the result slot
    HARGS+4..5 is initialized to zero in place."""
    loop = ".mul16_loop"
    skip = ".mul16_skip"
    return a.Function(
        name="mul16", is_global=True, params=[],
        instructions=[
            # A_work (HARGS+6..7) := A
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(6)),
            a.Mov(src=_hargs(1), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(7)),
            # B_work (HARGS+8..9) := B
            a.Mov(src=_hargs(2), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(8)),
            a.Mov(src=_hargs(3), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(9)),
            # result (HARGS+4..5) := 0
            a.Mov(src=_imm(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(4)),
            a.Mov(src=_REG_A, dst=_hargs(5)),
            # X := 16
            a.Mov(src=_imm(16), dst=_REG_X),
            a.Label(name=loop),
            # A_work >>= 1: hi-byte first so its bit-0 chains into
            # the lo-byte's bit-7 via C; A_work LSB ends up in C.
            a.LogicalShiftRight(dst=_hargs(7)),
            a.RotateRight(dst=_hargs(6)),
            a.Branch(cond=a.CC(), target=skip),
            # result += B_work (16-bit, carry-chained).
            a.ClearCarry(),
            a.Mov(src=_hargs(4), dst=_REG_A),
            a.Add(src=_hargs(8), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(4)),
            a.Mov(src=_hargs(5), dst=_REG_A),
            a.Add(src=_hargs(9), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(5)),
            a.Label(name=skip),
            # B_work <<= 1 (lo-byte first; bit 7 → C → bit 0 of hi).
            a.ArithmeticShiftLeft(dst=_hargs(8)),
            a.RotateLeft(dst=_hargs(9)),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def mul32_function() -> a.Function:
    """32-bit unsigned multiply, low-4-byte result via shift-and-add.

    Inputs at HARGS+0..3 (A), HARGS+4..7 (B); output at HARGS+8..11
    (4 bytes = low half of A*B). Inputs preserved.

    Thirty-two iterations of shift-and-add. Scratch is in the back
    half of HARGS: A_work at HARGS+12..15 and B_work at HARGS+16..19
    (those slots are not used by any other call path while mul32 is
    running — see `sim/runtime.py`'s convention notes; HARGS is
    caller-saved across helper calls)."""
    loop = ".mul32_loop"
    skip = ".mul32_skip"
    init_a_work: list[a.Type_instruction] = []
    for k in range(4):
        init_a_work += [
            a.Mov(src=_hargs(k), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(12 + k)),
        ]
    init_b_work: list[a.Type_instruction] = []
    for k in range(4):
        init_b_work += [
            a.Mov(src=_hargs(4 + k), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(16 + k)),
        ]
    init_result: list[a.Type_instruction] = [
        a.Mov(src=_imm(0), dst=_REG_A),
    ] + [
        a.Mov(src=_REG_A, dst=_hargs(8 + k)) for k in range(4)
    ]
    add_chain: list[a.Type_instruction] = [a.ClearCarry()]
    for k in range(4):
        add_chain += [
            a.Mov(src=_hargs(8 + k), dst=_REG_A),
            a.Add(src=_hargs(16 + k), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(8 + k)),
        ]
    return a.Function(
        name="mul32", is_global=True, params=[],
        instructions=[
            *init_a_work,
            *init_b_work,
            *init_result,
            a.Mov(src=_imm(32), dst=_REG_X),
            a.Label(name=loop),
            # A_work >>= 1: top byte first, then ROR each lower byte.
            # The final ROR puts A_work's LSB into C.
            a.LogicalShiftRight(dst=_hargs(15)),
            a.RotateRight(dst=_hargs(14)),
            a.RotateRight(dst=_hargs(13)),
            a.RotateRight(dst=_hargs(12)),
            a.Branch(cond=a.CC(), target=skip),
            # result += B_work (4-byte, carry-chained).
            *add_chain,
            a.Label(name=skip),
            # B_work <<= 1: low byte first (ASL), then ROL each
            # higher byte.
            a.ArithmeticShiftLeft(dst=_hargs(16)),
            a.RotateLeft(dst=_hargs(17)),
            a.RotateLeft(dst=_hargs(18)),
            a.RotateLeft(dst=_hargs(19)),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def asl8_function() -> a.Function:
    """8-bit shift left.

    val=HARGS+0, count=HARGS+1, result=HARGS+2 (1 byte). Loops
    `count` times applying `ASL A`. Counts ≥ 8 are UB per C99
    §6.5.7.4 but produce the natural 0 result here (each ASL shifts
    a 0 in; after 8 iterations the value is 0 regardless of input).
    """
    loop = ".asl8_loop"
    done = ".asl8_done"
    return a.Function(
        name="asl8", is_global=True, params=[],
        instructions=[
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Mov(src=_hargs(1), dst=_REG_X),
            a.Branch(cond=a.EQ(), target=done),
            a.Label(name=loop),
            a.ArithmeticShiftLeft(dst=_REG_A),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Label(name=done),
            a.Mov(src=_REG_A, dst=_hargs(2)),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def lsr8_function() -> a.Function:
    """8-bit logical shift right (zero-fill). val=HARGS+0,
    count=HARGS+1, result=HARGS+2."""
    loop = ".lsr8_loop"
    done = ".lsr8_done"
    return a.Function(
        name="lsr8", is_global=True, params=[],
        instructions=[
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Mov(src=_hargs(1), dst=_REG_X),
            a.Branch(cond=a.EQ(), target=done),
            a.Label(name=loop),
            a.LogicalShiftRight(dst=_REG_A),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Label(name=done),
            a.Mov(src=_REG_A, dst=_hargs(2)),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def asr8_function() -> a.Function:
    """8-bit arithmetic shift right (sign-fill). val=HARGS+0,
    count=HARGS+1, result=HARGS+2.

    Each iteration: `CMP #$80` sets carry to A's bit 7 (true iff
    A >= $80, i.e. the sign bit), then `ROR A` rotates right with
    that carry filling bit 7 — preserving sign. Negative inputs
    saturate to `$FF` after enough iterations, positive inputs to
    `$00`. Same UB semantics for count ≥ 8 as the other 8-bit
    shifts, but the natural saturation matches what the Python
    hook does too."""
    loop = ".asr8_loop"
    done = ".asr8_done"
    return a.Function(
        name="asr8", is_global=True, params=[],
        instructions=[
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Mov(src=_hargs(1), dst=_REG_X),
            a.Branch(cond=a.EQ(), target=done),
            a.Label(name=loop),
            a.Compare(left=_REG_A, right=_imm(0x80)),
            a.RotateRight(dst=_REG_A),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Label(name=done),
            a.Mov(src=_REG_A, dst=_hargs(2)),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def asl16_function() -> a.Function:
    """16-bit shift left. val=HARGS+0..1, count=HARGS+2,
    result=HARGS+3..4. Each iteration: `ASL low ; ROL high`."""
    loop = ".asl16_loop"
    done = ".asl16_done"
    return a.Function(
        name="asl16", is_global=True, params=[],
        instructions=[
            # result := val
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(3)),
            a.Mov(src=_hargs(1), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(4)),
            # X = count
            a.Mov(src=_hargs(2), dst=_REG_X),
            a.Branch(cond=a.EQ(), target=done),
            a.Label(name=loop),
            a.ArithmeticShiftLeft(dst=_hargs(3)),
            a.RotateLeft(dst=_hargs(4)),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Label(name=done),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def lsr16_function() -> a.Function:
    """16-bit logical shift right (zero-fill). Each iteration:
    `LSR high ; ROR low` — the LSR shifts a 0 into bit 7 of high;
    the ROR rotates the carry (= bit 0 of high) into bit 7 of low."""
    loop = ".lsr16_loop"
    done = ".lsr16_done"
    return a.Function(
        name="lsr16", is_global=True, params=[],
        instructions=[
            # result := val
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(3)),
            a.Mov(src=_hargs(1), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(4)),
            a.Mov(src=_hargs(2), dst=_REG_X),
            a.Branch(cond=a.EQ(), target=done),
            a.Label(name=loop),
            a.LogicalShiftRight(dst=_hargs(4)),
            a.RotateRight(dst=_hargs(3)),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Label(name=done),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def asr16_function() -> a.Function:
    """16-bit arithmetic shift right (sign-fill). Each iteration:
    capture sign of high byte into C (`LDA high ; ASL A` puts bit 7
    in C; A is scratch), then `ROR high ; ROR low` chains the sign
    fill from C through to bit 7 of high, then bit 0 of high to
    bit 7 of low."""
    loop = ".asr16_loop"
    done = ".asr16_done"
    return a.Function(
        name="asr16", is_global=True, params=[],
        instructions=[
            # result := val (HARGS+3..4)
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(3)),
            a.Mov(src=_hargs(1), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(4)),
            a.Mov(src=_hargs(2), dst=_REG_X),
            a.Branch(cond=a.EQ(), target=done),
            a.Label(name=loop),
            # Capture sign: A := high byte, ASL puts bit 7 in C.
            a.Mov(src=_hargs(4), dst=_REG_A),
            a.ArithmeticShiftLeft(dst=_REG_A),
            # Sign-rotate: ROR high (C → bit 7), then ROR low.
            a.RotateRight(dst=_hargs(4)),
            a.RotateRight(dst=_hargs(3)),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Label(name=done),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def asl32_function() -> a.Function:
    """32-bit shift left. val=HARGS+0..3, count=HARGS+4,
    result=HARGS+5..8. Each iteration: ASL low; ROL each higher
    byte chained through C."""
    loop = ".asl32_loop"
    done = ".asl32_done"
    init: list[a.Type_instruction] = []
    for k in range(4):
        init += [
            a.Mov(src=_hargs(k), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(5 + k)),
        ]
    return a.Function(
        name="asl32", is_global=True, params=[],
        instructions=[
            *init,
            a.Mov(src=_hargs(4), dst=_REG_X),
            a.Branch(cond=a.EQ(), target=done),
            a.Label(name=loop),
            a.ArithmeticShiftLeft(dst=_hargs(5)),
            a.RotateLeft(dst=_hargs(6)),
            a.RotateLeft(dst=_hargs(7)),
            a.RotateLeft(dst=_hargs(8)),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Label(name=done),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def lsr32_function() -> a.Function:
    """32-bit logical shift right (zero-fill). Each iteration: LSR
    top byte; ROR each lower byte chained through C."""
    loop = ".lsr32_loop"
    done = ".lsr32_done"
    init: list[a.Type_instruction] = []
    for k in range(4):
        init += [
            a.Mov(src=_hargs(k), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(5 + k)),
        ]
    return a.Function(
        name="lsr32", is_global=True, params=[],
        instructions=[
            *init,
            a.Mov(src=_hargs(4), dst=_REG_X),
            a.Branch(cond=a.EQ(), target=done),
            a.Label(name=loop),
            a.LogicalShiftRight(dst=_hargs(8)),
            a.RotateRight(dst=_hargs(7)),
            a.RotateRight(dst=_hargs(6)),
            a.RotateRight(dst=_hargs(5)),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Label(name=done),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def asr32_function() -> a.Function:
    """32-bit arithmetic shift right (sign-fill). Each iteration:
    capture top-byte's bit 7 into C via `LDA top ; ASL A`, then ROR
    chain from top to low."""
    loop = ".asr32_loop"
    done = ".asr32_done"
    init: list[a.Type_instruction] = []
    for k in range(4):
        init += [
            a.Mov(src=_hargs(k), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(5 + k)),
        ]
    return a.Function(
        name="asr32", is_global=True, params=[],
        instructions=[
            *init,
            a.Mov(src=_hargs(4), dst=_REG_X),
            a.Branch(cond=a.EQ(), target=done),
            a.Label(name=loop),
            a.Mov(src=_hargs(8), dst=_REG_A),
            a.ArithmeticShiftLeft(dst=_REG_A),
            a.RotateRight(dst=_hargs(8)),
            a.RotateRight(dst=_hargs(7)),
            a.RotateRight(dst=_hargs(6)),
            a.RotateRight(dst=_hargs(5)),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Label(name=done),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def udivmod8_function() -> a.Function:
    """8-bit unsigned divmod via shift-and-subtract long division.

    Algorithm (eight iterations):
      quot := num                  ; dividend slot doubles as quotient
      rem := 0
      for _ in 8:
          quot := quot << 1        ; MSB of dividend → C
          rem := (rem << 1) | C    ; rotate C into rem's LSB
          if rem >= den:
              rem -= den
              quot bit 0 := 1      ; ASL just put 0 there, INC is fine

    The `INC HARGS+2` trick works because every iteration's `ASL
    HARGS+2` shifts a 0 into bit 0 of quot, so an `INC` cleanly sets
    that bit to 1 without rippling. Inputs at HARGS+0..1 survive
    the call (we read num once at the start; den is read but never
    written). Output: quot at HARGS+2, rem at HARGS+3. Clobbers A
    and X."""
    loop = ".udivmod8_loop"
    skip = ".udivmod8_skip"
    return a.Function(
        name="udivmod8", is_global=True, params=[],
        instructions=[
            # quot := num
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(2)),
            # rem := 0
            a.Mov(src=_imm(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(3)),
            # X := 8 (loop count)
            a.Mov(src=_imm(8), dst=_REG_X),
            a.Label(name=loop),
            # quot <<= 1, MSB → C
            a.ArithmeticShiftLeft(dst=_hargs(2)),
            # rem = (rem << 1) | C
            a.RotateLeft(dst=_hargs(3)),
            # if rem >= den, subtract and set quot bit 0
            a.Mov(src=_hargs(3), dst=_REG_A),
            a.Compare(left=_REG_A, right=_hargs(1)),
            a.Branch(cond=a.CC(), target=skip),
            # rem -= den (CMP set carry; SBC reuses it)
            a.Sub(src=_hargs(1), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(3)),
            a.Inc(dst=_hargs(2)),  # set quot LSB
            a.Label(name=skip),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def sdivmod8_function() -> a.Function:
    """8-bit signed divmod with C99 §6.5.5.6 truncate-toward-zero.

    Algorithm:
      sign_q = sign(n) XOR sign(d)        ; goes into the quotient
      |n|, |d| feed into udivmod8         ; always-positive division
      if sign(n) negative: rem := -rem    ; rem matches n's sign
      if sign_q negative:  quot := -quot

    We stash the original n and d in HARGS+4 / HARGS+5 (scratch slots
    not used by the 8-bit helpers' inputs/outputs) so we can recover
    them after udivmod8 runs and the inputs/outputs of HARGS+0..3
    have been overwritten. At exit we restore HARGS+0 / HARGS+1 from
    the scratch — the calling convention says inputs survive helper
    calls.

    Edge case: INT_MIN (-128). `-(-128)` overflows to -128 in 8 bits,
    but the algorithm still produces the right answer because the
    bit pattern of `-(-128)` is `$80` (which read as an unsigned 8-bit
    is 128), and udivmod8 of that works correctly. The final negate
    of the quotient also overflows back to $80, which is `-128` in
    8-bit signed — the right result for any non-zero divisor."""
    n_pos = ".sdivmod8_n_pos"
    d_pos = ".sdivmod8_d_pos"
    quot_pos = ".sdivmod8_quot_pos"
    rem_pos = ".sdivmod8_rem_pos"
    return a.Function(
        name="sdivmod8", is_global=True, params=[],
        instructions=[
            # Stash originals: HARGS+4 := n, HARGS+5 := d.
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(4)),
            a.Mov(src=_hargs(1), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(5)),
            # n := |n| (negate if MSB set).
            a.Mov(src=_hargs(0), dst=_REG_A),
            a.Branch(cond=a.PL(), target=n_pos),
            *_negate_a(),
            a.Mov(src=_REG_A, dst=_hargs(0)),
            a.Label(name=n_pos),
            # d := |d|.
            a.Mov(src=_hargs(1), dst=_REG_A),
            a.Branch(cond=a.PL(), target=d_pos),
            *_negate_a(),
            a.Mov(src=_REG_A, dst=_hargs(1)),
            a.Label(name=d_pos),
            # Unsigned divide on absolute values.
            a.Call(name="udivmod8"),
            # Sign-correct the quotient: negate if sign(n) XOR sign(d).
            a.Mov(src=_hargs(4), dst=_REG_A),
            a.Xor(src1=_REG_A, src2=_hargs(5), dst=_REG_A),
            a.Branch(cond=a.PL(), target=quot_pos),
            a.Mov(src=_hargs(2), dst=_REG_A),
            *_negate_a(),
            a.Mov(src=_REG_A, dst=_hargs(2)),
            a.Label(name=quot_pos),
            # Sign-correct the remainder: negate if n was negative
            # (rem takes dividend's sign per C99).
            a.Mov(src=_hargs(4), dst=_REG_A),
            a.Branch(cond=a.PL(), target=rem_pos),
            a.Mov(src=_hargs(3), dst=_REG_A),
            *_negate_a(),
            a.Mov(src=_REG_A, dst=_hargs(3)),
            a.Label(name=rem_pos),
            # Restore original inputs (caller convention).
            a.Mov(src=_hargs(4), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(0)),
            a.Mov(src=_hargs(5), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(1)),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def udivmod16_function() -> a.Function:
    """16-bit unsigned divmod via shift-and-subtract long division.

    Inputs at HARGS+0..1 (num) and HARGS+2..3 (den); outputs at
    HARGS+4..5 (quot) and HARGS+6..7 (rem). Scratch at HARGS+8 to
    hold the tentative rem.lo across the multi-byte SBC chain (we
    can't keep it in a register because the SBC of the high byte
    needs A as scratch).

    Algorithm: 16 iterations of "shift the combined 32-bit
    (rem:quot) value left by 1, then if rem >= den subtract den
    from rem and set quot's bit 0." The shift moves num's MSBs up
    through the combined value into rem one bit per iteration; the
    space left at quot's LSB by the ASL gets overwritten by INC
    quot.lo when we commit. Inputs at HARGS+0..3 are read once at
    the start (copied to HARGS+4..5 as the working dividend / future
    quot) and never written, so they survive the call."""
    loop = ".udivmod16_loop"
    skip = ".udivmod16_skip"
    return a.Function(
        name="udivmod16", is_global=True, params=[],
        instructions=[
            # quot := num
            *_copy_n_via_a(0, 4, 2),
            # rem := 0
            a.Mov(src=_imm(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(6)),
            a.Mov(src=_REG_A, dst=_hargs(7)),
            a.Mov(src=_imm(16), dst=_REG_X),
            a.Label(name=loop),
            # 32-bit shift left of (rem:quot).
            a.ArithmeticShiftLeft(dst=_hargs(4)),
            a.RotateLeft(dst=_hargs(5)),
            a.RotateLeft(dst=_hargs(6)),
            a.RotateLeft(dst=_hargs(7)),
            # Tentative rem - den.
            a.SetCarry(),
            a.Mov(src=_hargs(6), dst=_REG_A),
            a.Sub(src=_hargs(2), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(8)),  # tentative rem.lo
            a.Mov(src=_hargs(7), dst=_REG_A),
            a.Sub(src=_hargs(3), dst=_REG_A),
            a.Branch(cond=a.CC(), target=skip),
            # Commit: rem := tentative; A still has tentative rem.hi.
            a.Mov(src=_REG_A, dst=_hargs(7)),
            a.Mov(src=_hargs(8), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(6)),
            a.Inc(dst=_hargs(4)),  # set quot bit 0
            a.Label(name=skip),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def sdivmod16_function() -> a.Function:
    """16-bit signed divmod with C99 §6.5.5.6 trunc-toward-zero.

    Stashes original n.hi and d.hi at HARGS+9 and HARGS+10 (slots
    not touched by udivmod16), absolute-values both inputs in
    place, calls udivmod16, sign-corrects the quotient and remainder,
    and re-negates the inputs to restore them. Negation is its own
    inverse mod 2^16, so re-negating after the divide cleanly
    restores the originals."""
    n_pos = ".sdivmod16_n_pos"
    d_pos = ".sdivmod16_d_pos"
    quot_pos = ".sdivmod16_quot_pos"
    rem_pos = ".sdivmod16_rem_pos"
    n_done = ".sdivmod16_n_done"
    d_done = ".sdivmod16_d_done"
    return a.Function(
        name="sdivmod16", is_global=True, params=[],
        instructions=[
            # Stash original n.hi at HARGS+9 (sign-test slot).
            a.Mov(src=_hargs(1), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(9)),
            a.Branch(cond=a.PL(), target=n_pos),
            *_negate_n_inplace(0, 2),
            a.Label(name=n_pos),
            # Stash original d.hi at HARGS+10.
            a.Mov(src=_hargs(3), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(10)),
            a.Branch(cond=a.PL(), target=d_pos),
            *_negate_n_inplace(2, 2),
            a.Label(name=d_pos),
            # Unsigned divide on absolute values.
            a.Call(name="udivmod16"),
            # Sign-correct quot if sign(n) XOR sign(d) is negative.
            a.Mov(src=_hargs(9), dst=_REG_A),
            a.Xor(src1=_REG_A, src2=_hargs(10), dst=_REG_A),
            a.Branch(cond=a.PL(), target=quot_pos),
            *_negate_n_inplace(4, 2),
            a.Label(name=quot_pos),
            # Sign-correct rem if n was negative.
            a.Mov(src=_hargs(9), dst=_REG_A),
            a.Branch(cond=a.PL(), target=rem_pos),
            *_negate_n_inplace(6, 2),
            a.Label(name=rem_pos),
            # Restore original inputs by re-negating those that were
            # originally negative (negation is its own inverse).
            a.Mov(src=_hargs(9), dst=_REG_A),
            a.Branch(cond=a.PL(), target=n_done),
            *_negate_n_inplace(0, 2),
            a.Label(name=n_done),
            a.Mov(src=_hargs(10), dst=_REG_A),
            a.Branch(cond=a.PL(), target=d_done),
            *_negate_n_inplace(2, 2),
            a.Label(name=d_done),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def udivmod32_function() -> a.Function:
    """32-bit unsigned divmod via shift-and-subtract long division.

    Inputs at HARGS+0..3 (num) and HARGS+4..7 (den); outputs at
    HARGS+8..11 (quot) and HARGS+12..15 (rem). Scratch at
    HARGS+16..19 holds the tentative rem across the multi-byte SBC
    chain.

    Same algorithm as udivmod16 scaled to 4-byte values: 32
    iterations, 8-byte (rem:quot) shift left, 4-byte SBC for the
    tentative rem - den, conditional commit. Inputs at HARGS+0..7
    are read-only and survive the call."""
    loop = ".udivmod32_loop"
    skip = ".udivmod32_skip"

    sbc_chain: list[a.Type_instruction] = [a.SetCarry()]
    for k in range(4):
        sbc_chain += [
            a.Mov(src=_hargs(12 + k), dst=_REG_A),
            a.Sub(src=_hargs(4 + k), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(16 + k)),
        ]
    commit: list[a.Type_instruction] = []
    for k in range(4):
        commit += [
            a.Mov(src=_hargs(16 + k), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(12 + k)),
        ]
    commit.append(a.Inc(dst=_hargs(8)))

    return a.Function(
        name="udivmod32", is_global=True, params=[],
        instructions=[
            # quot := num (4 bytes)
            *_copy_n_via_a(0, 8, 4),
            # rem := 0 (4 bytes)
            a.Mov(src=_imm(0), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(12)),
            a.Mov(src=_REG_A, dst=_hargs(13)),
            a.Mov(src=_REG_A, dst=_hargs(14)),
            a.Mov(src=_REG_A, dst=_hargs(15)),
            a.Mov(src=_imm(32), dst=_REG_X),
            a.Label(name=loop),
            # 64-bit shift left of (rem:quot).
            a.ArithmeticShiftLeft(dst=_hargs(8)),
            a.RotateLeft(dst=_hargs(9)),
            a.RotateLeft(dst=_hargs(10)),
            a.RotateLeft(dst=_hargs(11)),
            a.RotateLeft(dst=_hargs(12)),
            a.RotateLeft(dst=_hargs(13)),
            a.RotateLeft(dst=_hargs(14)),
            a.RotateLeft(dst=_hargs(15)),
            *sbc_chain,
            a.Branch(cond=a.CC(), target=skip),
            *commit,
            a.Label(name=skip),
            a.Dec(dst=_REG_X),
            a.Branch(cond=a.NE(), target=loop),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


def sdivmod32_function() -> a.Function:
    """32-bit signed divmod with C99 §6.5.5.6 trunc-toward-zero.

    Same shape as sdivmod16: stash sign info (top byte of each
    input) at HARGS+20 / HARGS+21 — slots not touched by
    udivmod32's scratch at HARGS+16..19 — abs-value the inputs in
    place, JSR udivmod32, sign-correct outputs, and re-negate
    inputs to restore."""
    n_pos = ".sdivmod32_n_pos"
    d_pos = ".sdivmod32_d_pos"
    quot_pos = ".sdivmod32_quot_pos"
    rem_pos = ".sdivmod32_rem_pos"
    n_done = ".sdivmod32_n_done"
    d_done = ".sdivmod32_d_done"
    return a.Function(
        name="sdivmod32", is_global=True, params=[],
        instructions=[
            a.Mov(src=_hargs(3), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(20)),
            a.Branch(cond=a.PL(), target=n_pos),
            *_negate_n_inplace(0, 4),
            a.Label(name=n_pos),
            a.Mov(src=_hargs(7), dst=_REG_A),
            a.Mov(src=_REG_A, dst=_hargs(21)),
            a.Branch(cond=a.PL(), target=d_pos),
            *_negate_n_inplace(4, 4),
            a.Label(name=d_pos),
            a.Call(name="udivmod32"),
            a.Mov(src=_hargs(20), dst=_REG_A),
            a.Xor(src1=_REG_A, src2=_hargs(21), dst=_REG_A),
            a.Branch(cond=a.PL(), target=quot_pos),
            *_negate_n_inplace(8, 4),
            a.Label(name=quot_pos),
            a.Mov(src=_hargs(20), dst=_REG_A),
            a.Branch(cond=a.PL(), target=rem_pos),
            *_negate_n_inplace(12, 4),
            a.Label(name=rem_pos),
            a.Mov(src=_hargs(20), dst=_REG_A),
            a.Branch(cond=a.PL(), target=n_done),
            *_negate_n_inplace(0, 4),
            a.Label(name=n_done),
            a.Mov(src=_hargs(21), dst=_REG_A),
            a.Branch(cond=a.PL(), target=d_done),
            *_negate_n_inplace(4, 4),
            a.Label(name=d_done),
            a.Ret(arg_bytes=0, local_bytes=0, save_a=False),
        ],
    )


# Names of the helpers we have real asm for. `sim/runtime.py` reads
# this list and does NOT install Python hooks for these names — the
# real asm runs instead, assembled into the program image alongside
# the user code.
ASM_IMPLEMENTED: tuple[str, ...] = (
    "mul8", "mul16", "mul32",
    "udivmod8", "sdivmod8",
    "udivmod16", "sdivmod16",
    "udivmod32", "sdivmod32",
    "asl8", "asr8", "lsr8",
    "asl16", "asr16", "lsr16",
    "asl32", "asr32", "lsr32",
)


def all_helper_functions() -> list[a.Function]:
    """The full set of asm helpers to assemble into the program image.
    Add to this list as more helpers move from Python hooks to real
    asm."""
    return [
        mul8_function(),
        mul16_function(),
        mul32_function(),
        udivmod8_function(),
        sdivmod8_function(),
        udivmod16_function(),
        sdivmod16_function(),
        udivmod32_function(),
        sdivmod32_function(),
        asl8_function(),
        asr8_function(),
        lsr8_function(),
        asl16_function(),
        asr16_function(),
        lsr16_function(),
        asl32_function(),
        asr32_function(),
        lsr32_function(),
    ]
