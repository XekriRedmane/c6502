# 147 → 26 lines: a code-generation walkthrough on one game-engine helper

I've been writing a C99-to-6502 compiler in Python for a while now.
A recent batch of work tightened its output on a small game-engine
helper from 147 lines of 6502 assembly down to 26. The function
itself is twenty lines of C, but the compiler had three layers of
slack between "naive correct codegen" and "tight enough that I'd
hand-write it the same way." This is the walkthrough.

The C source is from a draft Apple II side-scroller — a rescue-NPC
spawner that initializes seven slots' worth of per-entity state.
The interesting thing about it for compiler purposes is that
**every line is a store to an extern array indexed by a function
parameter**, and the parameter is a single byte. That makes it a
useful target for the 6502's absolute-indexed addressing modes
(`STA arr,X`), but only if the compiler manages to choose them.

## The C source

```c
#include <stdint.h>

extern uint8_t entity_active[20];
extern uint8_t rescue_dir[20];
extern uint8_t entity_floor_col[20];
extern uint8_t entity_xoff_idx[20];
extern uint8_t entity_floor_pos[20];
extern uint8_t rescue_anim[20];
extern uint8_t rescue_floor[20];

extern const uint8_t floor_thresh[];   /* per-floor row anchor */

__attribute__((zp_abi))
static void spawn_pos_dir(uint8_t slot)
{
    entity_active[slot]    = 0x01;
    rescue_dir[slot]       = 0x01;
    entity_floor_col[slot] = 0x3E;
    entity_xoff_idx[slot]  = 0x00;
    rescue_anim[slot]      = 0x00;
    entity_floor_pos[slot] =
        (uint8_t)(floor_thresh[rescue_floor[slot]] - 0x07);
}
```

It's a function I'd love to keep small. Seven stores (six of them
constant, one a derived value), every one indexed by `slot`. Plus
one chained subscript read for the derived value.

Notable C99 details that affect codegen:

- `floor_thresh[]` is declared with unspecified size. This is the
  C99 incomplete-array form, valid only at the outermost type of
  an `extern` declaration. The defining TU supplies the size.
- `__attribute__((zp_abi))` is a c6502 extension that tells the
  compiler the function's parameter can live in a permanent
  zero-page slot rather than the soft-stack frame. Useful for
  leaf functions called frequently with small param counts.

When the work below started, my compiler couldn't parse the
unsized extern array form at all.

## Round 0: 147 lines

Initial state of the world: the parser rejected `extern T name[];`,
so the function didn't compile end-to-end. The error was
`array of unspecified size ("[]") is not supported`, which is true
— the parser had a `NotImplementedError` for the case. But sized
extern arrays (the seven `uint8_t name[20];` declarations) already
worked, so the gap was specifically the unsized form.

I added support for the form by treating `Array(elem, size=0)` as
the incomplete-array sentinel. The parser just stops rejecting
`[]` and emits `Array(elem, 0)`. The type checker enforces C99's
restriction that the sentinel is only legal at the outermost type
of an `extern` declaration — struct members, array elements,
sizeof targets, and non-extern variables all require a complete
type, gated on the existing `require_complete=True` flag in
`_check_well_formed_type`. SizeOfExp rejects an
incomplete-array-typed operand directly.

After the parser change, the function compiled. Output:

```asm
spawn_pos_dir:
   SUBROUTINE

   ; prologue: 1 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.spawn_pos_dir@asm_ssa_block@0:
   LDA   #<entity_active
   STA   __local_spawn_pos_dir_b0
   LDA   #>entity_active
   STA   __local_spawn_pos_dir_b0+1
   LDA   __local_spawn_pos_dir_b0
   STA   DPTR
   LDA   __local_spawn_pos_dir_b1
   STA   DPTR+1
   LDA   #$01
   PHA
   LDY   #$03
   LDA   (FP),Y
   TAY
   PLA
   STA   (__local_spawn_pos_dir_b0),Y
   /* … five more blocks like this … */

   ; epilogue
   CLC
   LDA   FP
   ADC   #$03
   STA   SSP
   /* … */
   RTS
```

147 lines. Three categories of waste, top to bottom:

1. **Prologue / epilogue churn.** The function has one byte of
   parameter and zero locals. The compiler allocated a soft-stack
   frame anyway, saved/restored FP, advanced SSP, etc. — about 30
   lines of bookkeeping for storage that holds one byte.
2. **PHA/PLA save-restore around every store.** Each subscript
   write goes through `LDA val; PHA; LDA idx; TAY; PLA; STA
   (ptr),Y` — save A on the hardware stack so the LDA-into-A step
   for the index doesn't clobber it. Six lines per store, seven
   stores.
3. **DPTR staging for every store.** Each store loads the array's
   address into a zero-page pointer, then does `STA (zp),Y`. Four
   lines of address-staging per store, seven stores.

Each one is a separate optimization story.

## Round 1: 62 lines

The first cleanup was straightforward — the user (me) already had
`__attribute__((zp_abi))` infrastructure for moving small param
values into a permanent ZP slot, I just hadn't applied it to this
function. Adding the annotation eliminated the prologue/epilogue
entirely: `slot` lives at `__zpabi_spawn_pos_dir_p0` (a 1-byte ZP
slot), no soft-stack frame needed, no FP saved or restored. The
function becomes a bare body plus `RTS`.

This also eliminated the body's `LDY #$03; LDA (FP),Y` to read
`slot` from the frame — every reference to `slot` becomes a direct
read of `__zpabi_spawn_pos_dir_p0`.

What about the PHA/PLA? After the zp_abi change, the store body
shape becomes

```asm
LDA   #$01
PHA
LDY   __zpabi_spawn_pos_dir_p0     ; <-- direct LDY from ZP, no A clobber!
PLA
STA   (ptr),Y
```

The body between PHA and PLA is now a single `LDY` from a ZP
location. `LDY` doesn't touch A. The save/restore is dead.

This was a documented deferred peephole in the compiler — the
`tac_to_asm._translate_indirect_indexed_store` function had a
comment saying

> "An asm-level peephole could collapse the save/restore when both
> operands prove to be ZP-resident post-regalloc — deferred."

I implemented `apply_dead_pha_pla`: match `Push(Reg(A)); body;
Pop(Reg(A))` where the body preserves A (no read, no write, no
nested push, no call, no control flow). Soundness gate: the PLA's
N/Z flag effect must be dead at +1. The existing
`apply_direct_index_load` peephole was what turned `LDA idx; TAY`
into a flag-preserving `LDY idx` — exposing the precondition for
the new peephole.

After this round, each store became

```asm
LDA   #<entity_active
STA   __local_spawn_pos_dir_b0
LDA   #>entity_active
STA   __local_spawn_pos_dir_b0+1
LDA   #$01
LDY   __zpabi_spawn_pos_dir_p0
STA   (__local_spawn_pos_dir_b0),Y
```

62 lines total. Still wasteful, still per-store DPTR staging, but
the inner save/restore was gone.

## Round 2: 26 lines

The remaining waste was structural: every store was using indirect
addressing through a runtime pointer, even though the pointer's
value was a compile-time-known link-time symbol. The 6502 has
absolute-indexed addressing — `STA arr,X` — that's literally what
this code wants.

The READ side was already producing this shape. The chained
subscript `floor_thresh[rescue_floor[slot]]` lowered to

```asm
LDX   __zpabi_spawn_pos_dir_p0
LDY   rescue_floor,X
LDA   floor_thresh,Y
```

via a c99→TAC fast path called `_try_indexed_load_subscript` that
recognizes `arr[i]` where `arr` has static storage and total byte
size ≤ 256, and emits a new TAC instruction `IndexedLoad(name,
index, dst)` directly. `tac_to_asm` lowers that as the
absolute-indexed read.

There was no mirror on the store side. The Subscript lvalue path
in c99→TAC went straight to pointer arithmetic + Store, and the
existing TAC `recognize_indexed_store` pass only fires on patterns
where the base is a compile-time `Constant` (produced by
const-static folding of `static T * const`-shaped sources, not
named arrays).

I added the mirror. A new TAC variant
`IndexedSymbolStore(identifier name, val index, val src, bool
is_volatile)` paired with a `_try_indexed_store_subscript` in
c99→TAC that runs at the same eligibility and emits directly
during translation. `tac_to_asm` lowers as:

```asm
LDA   idx
TAX
LDA   src[k]            ; per byte k of src (for multi-byte stores)
STA   name+k,X
```

The existing `direct_index_load` peephole collapses `LDA idx; TAX`
into `LDX idx` when `idx` is Imm/Data/ZP, giving the final shape
`LDX idx; LDA src; STA name,X` per byte.

The asm-level optimizer rounds then noticed that successive stores
share the X register (idx didn't change between stores) and the A
register (when the value matched the previous one). The asm-level
const-propagator pulled the per-store LDX out, and the per-value
LDA folded across constant-valued stores. Final output for the
example:

```asm
__zpabi_spawn_pos_dir_p0	EQU	$80
__local_spawn_pos_dir_b0	EQU	$81

; @zp-link-meta-begin
; def spawn_pos_dir param_bytes=1 local_bytes=1 indirect=false in_cycle=false
; @zp-link-meta-end

spawn_pos_dir:
   SUBROUTINE

.spawn_pos_dir@asm_ssa_block@0:
   LDX   __zpabi_spawn_pos_dir_p0
   LDA   #$01
   STA   entity_active,X
   STA   rescue_dir,X
   LDA   #$3E
   STA   entity_floor_col,X
   LDA   #$00
   STA   entity_xoff_idx,X
   STA   rescue_anim,X
   LDY   rescue_floor,X
   LDA   floor_thresh,Y
   SEC
   SBC   #$07
   STA   entity_floor_pos,X
   RTS
```

26 lines. 82% reduction from where we started.

## The bureaucratic part

Adding a TAC variant turned out to be the most labor-intensive
piece. The new `IndexedSymbolStore` variant required new cases in
eight separate optimizer passes. Each one does some kind of "walk
the instruction tree" thing — collecting uses, substituting copies,
rewriting in SSA construction, classifying side effects. Miss any
one and the optimizer doesn't fall over with an exception; it just
produces beautifully-wrong code (dead-store elimination drops the
constants because `uses_in` reports no uses for the new variant's
`src`).

The first round I added cases iteratively — trial-compile, see
breakage, patch one more pass, repeat. The second iteration I
did `grep -rn IndexedStore passes/optimization/` first and patched
them all in one go. The grep takes a second. The
trial-compile-fix-trial-compile loop takes minutes per iteration
because each one runs the full test suite.

A small memory note for future me went into the project's
auto-memory file: "When adding a TAC variant, grep for the
nearest-existing variant first."

## What this all looks like in context

Each round was a few hundred lines of compiler code:

- Round 0 (extern unsized arrays): ~10 lines in the parser, ~20 in
  the type checker, plus tests.
- Round 1 (dead-PHA/PLA peephole): ~50 lines in a new always-on
  asm peephole pass, plus tests.
- Round 2 (IndexedSymbolStore): ~110 lines spread across the new
  TAC variant, the c99→TAC fast path, the tac→asm lowering, eight
  optimizer-pass cases, and the TAC simulator case. Plus tests.

Cumulative effect on the motivating example: 147 → 62 → 26 lines.
The function now compiles to what I'd write by hand.

Most of this was already-there infrastructure being used a little
more aggressively, not deep theory. The compiler already had:

- absolute,Y / absolute,X addressing in its asm IR
- a peephole fixed-point loop that runs always-on
- a forward A-liveness analyzer (`a_dead_at` in `asm_liveness.py`)
- a forward flag-liveness analyzer (`flags_dead_at`)
- the `__attribute__((zp_abi))` infrastructure for frame
  elimination
- a TAC interpreter for differential testing

The work was mostly noticing — the dead-PHA/PLA peephole had been
flagged in a code comment as deferrable, the read fast path
existed without a write mirror, the parser's unsupported-array
form was a missing branch rather than a deep limitation. Three
sessions of "let me read this output more carefully and ask why
it's that shape" later, the function shrunk to a quarter of its
original size.

Source is at github.com/XekriRedmane/c6502 if anyone wants to
follow the commits. The function in question is in
`examples/spawn_pos_dir.c`, and the matching `.asm` is checked in
as a snapshot test target — so every diff to the codegen layer
shows up immediately.
