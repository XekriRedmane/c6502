# sound.c

## What it translates

Every routine UNITS.md gives the unit, `main.asm` lines 15057-15311.

| Original | C |
|---|---|
| TUNE_PLAYER $795B, with TUNE_PLAYER_FETCH, the commands, and the note action | `tune_player(tune, strength)` |
| TUNE_ACTION_INDIRECT $7A4C | the `if` on TUNE_ACTION_VEC in `tune_player` |
| TUNE_ADVANCE $7A4F | the end of `tune_player`'s loop |
| TUNE_INC_PC $7A77 | `TUNE_PC++` |
| PLAY_HIGH_TONE $7A7F | the static `play_high_tone(freq, duration, first)` |
| PLAY_LOW_TONE $7A91 | the static `play_low_tone(freq, duration, first)` |

It defines `SOUND_ON` (header), initially true, and the static
`tone_padding`, pinned at the original's TONE_PADDING ($E3). The unit
moves no table, since the tunes and the variables of page $5A stay
where the header puts them. Every routine is translated, and none is
left incomplete.

## Departures

The file's opening comment lists them. In short:

- tune_player takes the tune number, and the original's X is that
  number times 2.
- TUNE_ACTION_VEC still holds $7A7F or $7A91, and the C compares it
  with $7A91 to choose the low tone, and plays the high tone for any
  other value.
- `border_anim_step()` runs after each note's tone while
  BORDER_ANIM_ON is true, in place of `JMP (DISPATCH_VEC)` at $7A49.
- With SOUND_ON false the tone is skipped, and STEP_PRNG still runs for
  each note.
- The first delay of a note starts from the original's X, which
  tune_player keeps in the local `x`. That is the tune number times 2
  before the first note, the previous note's frequency after a note
  that played, and 1 after the title tune's check of button 0. During
  INIT_GAME_STATE's title tune, the original's X after a note is what
  GAME_ACTION_HANDLER leaves (5, 3, or 1, from BORDER_LOOP_CTR), which
  init.c doesn't return, and the C uses the previous note's frequency.
  Only the length of the gap after the note's first click differs.
- The delay loops count down in Y, from a `register` local, where the
  original counts in X. See gap 2 below.

## Timing

Each click is a read of SPKR, 10 per unit of duration, 0 meaning 256,
and the number of passes of each delay loop is the original's. The
cycles, from the `-S -O` listing:

| | Pass | Gap between clicks | Extra on every tenth gap |
|---|---|---|---|
| PLAY_HIGH_TONE | 5 (DEX; BNE) | 5X+11 | 9 |
| play_high_tone | 5 (DEY; BNE) | 5X+17 | 12 |
| PLAY_LOW_TONE | 10 (ROL zp; DEX; BNE) | 10X+27 | 9 |
| play_low_tone | 10 (INC zp; DEY; BNE) | 10X+27 | 12 |

play_low_tone has two increments of tone_padding after its delay loop,
10 cycles, which stand for the original's ROL and ROR of TONE_PADDING,
and which bring its gap to the original's. The high tone's gap is 6
cycles longer than the original's, which no change to the C would
remove without changing the loop's code. The harness compares the order
of the clicks and not their timing.

The harness runs three shapes of delay loop in one step, `DEX; BNE`,
`DEY; BNE`, and `ROL zp; DEX; BNE`. The high tone's loop is the second.
The low tone's `INC zp; DEY; BNE` is none of them, so the harness runs
it an instruction at a time, which takes longer and gives the same
result. Adding `INC zp; DEY; BNE` to `Machine._delay_loop` would speed
up the C's tunes.

## Where the original's comments and code disagree

- PLAY_LOW_TONE's comment gives a frequency of about 510204 / (10X +
  31) Hz. The code's gap is 10X+27 cycles, plus 9 every tenth gap,
  which is about 10X+28 on average. PLAY_HIGH_TONE's 510204 / (5X + 12)
  agrees with its code (5X+11, plus 0.9 on average).
- The EQU names $C061 PADDL0. TUNE_ADVANCE reads it as button 0, and
  the C reads `BUTN[0]`.

## Header

No change is needed.

## Compiler bugs and gaps

1. **`keep` before a `for (;;)` with a `break` crashes the compiler**
   (`KeyError: '.loop@0_break'` in `cfg.build_cfg`):

   ```c
   #include <stdint.h>
   void f(uint8_t x) {
       [[c6502::keep]] for (;;) {
           if (--x == 0) break;
       }
   }
   ```

   The low tone's loop increments tone_padding and then counts down,
   which is that shape. The C writes the increment into the condition,
   `for (; tone_padding++, --x;)`, which compiles to `INC tone_padding;
   DEY; BNE`. The crash was fixed on 18 Nov 2026, and the loop keeps
   the form with the condition.

2. **A `keep` loop's count went to zero page when X was taken.** In the
   tone routines, with the count a parameter or an ordinary local, the
   register allocator put the duration of the outer loop in X and
   compiled the delay loop to `DEC zp; BNE`, 8 cycles a pass, and the
   low tone's to `INC; DEC zp; BNE`, entered by a JMP. On its own, the
   same loop compiles to `DEX; BNE`:

   ```c
   void delay(uint8_t n) { [[c6502::keep]] for (; --n;) {} }
   ```

   Declaring the count `register` puts it in Y, which gives `DEY; BNE`,
   5 cycles.

## Size

Code bytes of the `-S -O` output, from the object's symbol sizes,
against the original's from `main.lst`. `__zp_init` (9 bytes, which
stores SOUND_ON and tone_padding) isn't counted.

| Original | Bytes | C | Bytes |
|---|---|---|---|
| TUNE_PLAYER through TUNE_INC_PC ($795B-$7A7E) | 292 | tune_player | 373 |
| PLAY_HIGH_TONE | 18 | play_high_tone | 31 |
| PLAY_LOW_TONE | 27 | play_low_tone | 37 |
| **Total** | **337** | | **441** |

The C is 104 bytes (31%) larger. The choice of tone accounts for 30
bytes of it. The C compares both bytes of TUNE_ACTION_VEC (14 bytes)
and copies the arguments into the chosen routine's parameters (16
bytes), where the original's `JSR TUNE_ACTION_INDIRECT` and `JMP
(TUNE_ACTION_VEC)` take 6. PLAN.md gives the unit 710 bytes, but its
code is $795B-$7AAB, 337 bytes, as report_sound found.

## For the integrator

- `tone_padding` is pinned at $E3 with `[[c6502::zeropage(0xE3)]]`, so
  the link takes $E3 out of the compiler's zero-page pools. The original
  uses $E1-$E3 only in this unit.
- SOUND_ON and tone_padding have initial values that `__zp_init`
  stores. SOUND_ON must be true before the first tune.
- TUNE_ACTION_VEC holds garbage ($D2C1) in the image until a tune's
  first HIGH or LOW. Every tune sets it before its first note.
