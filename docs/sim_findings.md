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

## 2. Branch out of range (`branch_oor`) — FIXED

**Status:** fixed by adding `passes/long_branches.py`, which runs after
`replace_pseudoregisters` and before either `asm_emit.emit_program`
or `sim.assembler.assemble`. The pass walks each function, computes
label addresses (using the public `instruction_size` helper added to
`sim/assembler.py`), identifies any `Branch(cond, target)` whose
displacement exceeds ±127 bytes, and rewrites each as:

```
Branch(inverted_cond, .lb_skip@N)   ; 2 bytes
Jump(target)                         ; 3 bytes
Label(.lb_skip@N)
```

Iterates to fixed point per function — each expansion grows the
function by 3 bytes, which can push other still-short branches over
their windows. Termination is guaranteed because expansion only ever
increases distances. Wired into both `compile.py`'s codegen path
(for the dasm-text output) and `sim/harness.py`'s `compile_to_asm`
(for the simulator), so both targets see the same expanded program.

The fix unblocked 27 chapter cases (every `branch_oor` entry that
wasn't actually a separate frame-too-large issue — see finding 8).
Asm-sim chapter SKIPS: 74 → 47.

`tests/test_long_branches.py` covers the basic shapes: short
branches pass through unchanged, over-long forward and backward
branches get rewritten, the iteration converges, and unknown targets
raise.

Original notes for context:

The 6502's `Bxx` opcodes carry a signed 8-bit displacement
(`-128..+127`). Functions large enough to push a label past 127
bytes from a branch site couldn't encode the branch in one
instruction; the in-process assembler refused with `branch to
<label> out of range: disp=<d>`, the same outcome dasm would give
on the text output. The expansion is +3 bytes per long branch, so
short branches are still 2 bytes — only the over-long ones pay the
cost.

## 8. Frame too large (`frame_too_large`, 2 chapter files)

**Status:** real codegen issue, surfaced after the long-branch fix.

The prologue / epilogue address the saved-FP slot via `LDY #(M+1)`
followed by `LDA (SSP),Y` / `STA (SSP),Y`. The `LDY` immediate is a
single byte, so `M+1 ≤ 255`, i.e. `local_bytes ≤ 254`. `asm_emit`
checks this with `_check_local_bytes(m)` and raises
`local_bytes <m> out of range (expected 0..253)`.

Hits two struct-heavy chapter-18 tests:
`compound_assign_struct_members.c` (657 local bytes) and
`scalar_member_access/nested_struct.c` (449 local bytes).

**Fix sketches:**

  (a) Use a 16-bit indirect addressing for the saved-FP slot —
      stage `SSP+M+1` into a zero-page pointer pair, then
      `LDA (PTR),Y` with `Y=0`. Costs ~6 bytes per prologue and
      epilogue but unblocks any frame size up to 64KB.

  (b) Split large frames: allocate the first 254 bytes via the
      existing FP machinery and the rest via a secondary frame
      pointer (or via a chunk addressed through DPTR). More work,
      but fits the existing addressing-mode envelope.

  (c) Reject at compile time with a clear error and ask the user
      to refactor — until c6502 has structs that real programs
      regularly exceed 254 local bytes with, this might be the
      pragmatic choice.

## 3. Signed `divmod` not implemented in the runtime helpers — FIXED

**Status:** fixed in two phases. (1) `tac_to_asm`'s Divide / Modulo
arms now dispatch by operand signedness, mirroring the existing
`asr*` / `lsr*` split — `sdivmod{8,16,32}` for signed, `udivmod{8,16,32}`
for unsigned, both gated by `_is_unsigned_val(src1)`. (2) `udivmod8`
and `sdivmod8` exist as real 6502 assembly in
`sim/runtime_helpers.py`; the simulator assembles them into the
program image at `$E000+` and the user's `JSR udivmod8` lands on the
real 6502 routine instead of a Python trap. The 16- and 32-bit
variants stay as Python hooks for now (same algorithm, scaled
byte-widths — they'll move to real asm when needed).

The split unblocked 6 chapter cases (`chapter_3/valid/div_neg.c`,
`chapter_5/valid/exp_then_declaration.c`, plus 4 others in
chapter 11/12/15 that depend on signed `/` or `%`). Asm-sim SKIPS:
47 → 41.

`udivmod8` is a 30-byte shift-and-subtract long-division routine;
`sdivmod8` is a 70-byte wrapper that absolute-values its inputs,
calls `udivmod8`, and sign-corrects the quotient and remainder per
C99 §6.5.5.6 (trunc-toward-zero, remainder takes dividend's sign).

`tests/test_runtime_helpers.py` covers each helper with hand-built
C programs, including the C99 round-trip identity
`(a/b)*b + (a%b) == a` across all four sign combinations. The
8-bit signed `INT_MIN / -1` overflow case is also covered.

To implement memory-mode helpers cleanly, this commit also extended
the asm IR's `ASL` / `LSR` / `ROL` / `ROR` and `Inc` / `Dec` to
accept `Data` operands (zp / abs addressing on the 6502); the only
mode the IR doesn't support is indirect-Y, which the 6502 itself
doesn't have for the shift family. Soft-stack `Stack` / `Frame`
operands still need the load-shift-store pattern.

Original notes for context:

`tac_to_asm` originally emitted a single `divmod{8,16,32}` helper for
both signed and unsigned `/` and `%` (see `tac_to_asm.py:1273-1300`).
The simulator's hook did unsigned division; signed operands gave
wrong results for negative numerators. C99 §6.5.5.6 specifies signed
`/` truncates toward zero and `a%b` satisfies `(a/b)*b + a%b == a`,
which Python's floor-div doesn't match for negatives — the new
`sdivmod*` hook does the explicit sign-correction.

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
