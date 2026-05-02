# Findings from the asm-level simulator

The 6502 simulator (`sim/` + `tests/test_sim_*.py`) drives compiled C
through `tac_to_asm`, an in-process assembler, and py65's MPU until
the boot stub's BRK fires. Running it across the chapter corpus
surfaced the bugs and gaps below. Each one is also documented inline
in `tests/test_sim_asm.py`'s `SKIPS` table with a category string;
this file fleshes out the *why* and a sketch of the fix.

Numbers are at the time of the simulator's first run:

  402 chapter files pass through the asm sim
  115 in-Int-range chapter files skip with a categorized reason
   ~  out-of-Int-range files (separate count below) hit the
      Long-return bug and aren't yet routed through the sim

## 1. Long-return convention is broken (Long, ULong, Pointer) — FIXED via convention change

**Status:** the underlying problem (the epilogue's `TAX`/`STX`
clobbering X) was first patched by stashing the FP-restore's 1-byte
scratch through `PHA`/`PLA` on the HW stack — the X-clobber
finding's "concrete fix" path. That patch later got rolled back as
part of a larger change: **2-byte returns no longer ride in
registers at all.** Long / ULong / Pointer return values now live
in `HARGS+0..1`, matching the way 4-byte (LongLong / Float at
`HARGS+8..11`) and 8-byte (Double at `HARGS+16..23`) returns
already worked. The 1-byte (Int) path is unchanged.

The convention change makes the X register free to clobber in the
FP-restore (no return data lives there anymore), so the
`_emit_restore_fp_from_slot` is back to the simpler `TAX`/`STX`
form. The 10 chapter cases that the X-clobber fix had unblocked
stay unblocked under the new convention — pointer-typed temps in
the body now go through HARGS for return, which works the same
end-to-end. The asm-sim chapter pass count is 412 / 517 in-Int-range
cases.

The wider-than-Int (long-range) chapter cases that depend on a
working 2-byte return still don't all flow through cleanly because
of finding 7 below (`SignExtend` lowering tests `BMI` after the
STA's LDY clobbers N). The 2-byte return path itself is now correct;
verified directly via `sim/harness.py`'s `return_long()` helper
(reads `HARGS+0..1` from memory).

Original notes for context:

CLAUDE.md and `asm.asdl`'s `Ret` doc said a 2-byte return travelled
with the low byte in `A` and the high byte in `X`, and that
`X isn't touched by the SSP/FP arithmetic`. In practice, the
epilogue's FP-restore step used `TAX` as a 1-byte scratch —
destroying the return high byte. The body had loaded the return high
byte into X just before the epilogue began. `PHA`/`PLA` brackets
the SSP/FP arithmetic to preserve A, but nothing protected X. The
convention change side-stepped the fundamental tension: register-
based returns are tempting (no memory traffic) but fragile against
any later codegen pass that wants X as scratch. Memory-based
returns are uniform across all wider types and don't constrain the
register file.

## 2. Branch out of range (`branch_oor`, 40 chapter files)

**Status:** real codegen / encoding issue.

The 6502's `Bxx` opcodes carry a signed 8-bit displacement
(`-128..+127`). Functions large enough to push a label past 127 bytes
from a branch site can't encode the branch in one instruction. The
in-process assembler refuses with `branch to <label> out of range:
disp=<d>` — the same outcome dasm would give on the text output.

**Fix sketch.** In `tac_to_asm` (or a small post-pass on `asm_ast`),
when emitting a `Branch(cond, target)` whose displacement at lowering
time would exceed ±127 bytes, expand to:

```
B<inverted_cond>  +5            ; 2 bytes
JMP target                      ; 3 bytes
```

The expansion is 5 bytes vs. 2, and it changes addresses, so the
expansion has to iterate to a fixed point: identify too-far branches
under current sizing, expand them, recompute sizes, repeat. The
single-direction nature of the change (short → long, never the other
way) guarantees termination.

The simulator could install the same expansion in its assembler, but
that would mask the bug from the asm-text pipeline that targets
`dasm`. Better to fix it once in `tac_to_asm` so both targets benefit.

## 3. Signed `divmod` not implemented in the runtime helpers (~10
   `wrong_value` cases)

**Status:** real ambiguity; the c6502 pipeline doesn't distinguish
signed from unsigned divmod today.

`tac_to_asm` emits a single `divmod{8,16,32}` helper for both signed
and unsigned `/` and `%` (see `tac_to_asm.py:1273-1300`). The
simulator's hook does unsigned division; signed operands give wrong
results for negative numerators. The chapter file
`chapter_3/valid/div_neg.c` (`(-12)/5`) is the canonical case.

**Fix sketch.** Mirror the right-shift split (`asr*` / `lsr*`):
introduce `sdivmod{8,16,32}` and `udivmod{8,16,32}`, route from
`tac_to_asm`'s Divide / Modulo arms based on the operand's symbol-
table c99 type (the same `_is_unsigned_val` predicate already used
for ordering / right shift). The simulator's hook factory becomes two
families instead of one — both straightforward Python.

C99 §6.5.5.6 specifies signed `/` truncates toward zero and `a%b`
satisfies `(a/b)*b + a%b == a`. Python's `//` floors and `%` matches
(both round toward minus infinity), so the signed hook needs an
explicit sign-correction pass — fold the result toward zero, then
recompute the remainder.

## 4. Sign-extend / wider-than-int arithmetic (~50 `wrong_value` cases)

**Status:** likely related to the X-clobber and signed-divmod issues,
but worth bisecting case by case.

Most of the `wrong_value` skip-list under chapters 11–16 involves
`long` / `unsigned long` / `long long` arithmetic, sign-extending
casts, or character-type promotions. After fixing 1 and 3 above, the
expectation is that many of these flip to passing on their own.
Anything left should be triaged individually. A first pass:

  - `chapter_11/valid/explicit_casts/sign_extend.c` —
    `LongLong` equality returning 0 when `(long long)(-10) == -10ll`
    is expected. Probably the X-clobber affecting LongLong-via-HARGS
    return path's setup.

  - `chapter_14/valid/dereference/static_var_indirection.c` and the
    other Long-pointer tests — Pointer width is 2 bytes, same family
    as Long, so finding 1's fix probably applies.

  - `chapter_16/valid/chars/explicit_casts.c` — char-type promotion
    edge cases. Likely independent.

## 5. FP helpers not implemented (`fp_unimpl`, 11 cases)

**Status:** scope marker, not a bug.

The simulator registers trap addresses for the 26 FP conversion
helpers (`i2f`, `d2l`, etc.) and the 8 FP arithmetic helpers
(`fadd` ... `ddiv`), but each hook raises `NotImplementedError`. The
real 6502 helpers don't exist either — `tac_to_asm` emits
`JSR <helper>` calls in advance of the runtime header landing. Once
either side is implemented, the simulator's hook factory in
`sim/runtime.py:_HELPERS` is the place to fill in.

## 6. Externs we don't link (`extern_unresolved`, 3 cases)

**Status:** scope marker, not a bug.

Three chapter files reference functions like `exit` and helper
sidecars (`on_page_boundary`) that aren't in the same translation
unit. Real c6502 doesn't have a libc either, so these would fail at
the assembler level too. Worth a tiny libc stub eventually
(`exit`/`puts`/`putchar`/`malloc` would unlock ~30 more chapter files,
including some currently in `SKIPPED` upstream).

## 7. SignExtend lowering tests `BMI` after `LDY` clobbers `N` — FIXED

**Status:** fixed in `tac_to_asm._translate_sign_extend` by inserting
`Or(Imm(0), A)` immediately before the `Branch(MI, ...)`. `ORA #$00`
preserves A but updates N/Z from A's bit 7, refreshing the N flag
that the trailing `STA`'s `LDY #off` clobbered for soft-stack
operands. Costs 2 bytes per SignExtend; chosen over the alternative
(reorder so BMI runs before any STA, with the high-byte STA
duplicated in each branch arm) because it's a smaller diff and
keeps the byte-copy loop uniform.

The fix flipped 31 chapter cases from `wrong_value` to passing,
including most of chapter 11/12/14/15/16's compound-assignment,
explicit-cast, and char-arithmetic tests. The asm-sim chapter pass
count jumped 412 → 443 (517 - 74 still-skipping). What's left in
`wrong_value` (19 cases) concentrates in chapter 13 (FP) and a few
signed-div / pointer-diff / char-pointer edge cases that need
individual diagnosis.

Original notes for context:

`tac_to_asm._translate_sign_extend` lowered `SignExtend(src, dst)`
as: copy each source byte through A into the matching dst byte,
then `BMI sx_neg` to dispatch on the sign of the last (high) byte
loaded. The original comment claimed "the intervening STAs preserve
flags so the BMI below sees the right N." That's true for `STA abs`
but **not** for the indirect-Y form `LDY #off; STA (PTR),Y` that
`asm_emit` lowers soft-stack stores into — the `LDY` updates N/Z
based on its immediate. So when `dst` was a Frame / Stack /
Indirect operand, the `LDY` between the last source-byte LDA and
the BMI clobbered N, and the branch tested the LDY's immediate
(almost always positive for small offsets) instead of the source's
sign bit. Negative sources sign-extended to `$00...00` instead of
`$FF...FF`. Reproducer:
`long main(void) { long x = -1; return x; }` — used to return 255,
now returns -1.
