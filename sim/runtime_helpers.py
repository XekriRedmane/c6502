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

Currently implemented: `udivmod8`, `sdivmod8`. The 16- and 32-bit
variants stay as Python hooks in `sim/runtime.py` until they're
written here (same algorithm, scaled byte-widths).
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
ASM_IMPLEMENTED: tuple[str, ...] = ("udivmod8", "sdivmod8")


def all_helper_functions() -> list[a.Function]:
    """The full set of asm helpers to assemble into the program image.
    Add to this list as more helpers move from Python hooks to real
    asm."""
    return [udivmod8_function(), sdivmod8_function()]
