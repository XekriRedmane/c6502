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
`sdivmod8`. The 16- and 32-bit divmods stay as Python hooks in
`sim/runtime.py` until they're written here (same shift-and-subtract
algorithm as udivmod8, scaled byte-widths plus a sign-correction
wrapper for sdivmod16 / sdivmod32).
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


# Names of the helpers we have real asm for. `sim/runtime.py` reads
# this list and does NOT install Python hooks for these names — the
# real asm runs instead, assembled into the program image alongside
# the user code.
ASM_IMPLEMENTED: tuple[str, ...] = (
    "mul8", "mul16", "mul32",
    "udivmod8", "sdivmod8",
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
    ]
