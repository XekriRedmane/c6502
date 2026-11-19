# Inlining small functions

`inline_functions` in
[passes/optimization/inline_functions.py](../../passes/optimization/inline_functions.py)
replaces the calls of a `static` function with copies of its body,
when the copies are estimated to take no more bytes than the calls
and the function did. It runs once over the whole program, on the TAC
from `c99_to_tac`, before every other `-O` pass, so the TAC optimizer
works on each copy with the caller's own values. It doesn't run
without `-O`.

A call costs the caller the moves of its arguments into the callee's
parameter bytes, a `JSR`, and the moves of the result back, and the
function costs an `RTS`. Inside the function, a pointer parameter has
to be copied into a zero-page pair before the function can read
through it, and the function's code has to handle every argument and
every use of its result, where a call may pass a constant or only
branch on the result. A copy of the body in place of the call has
none of these costs, and the optimizer folds the caller's values into
it. C programmers write small helpers as function-like macros for
this reason, and under `-O` the compiler does the same for them.

## Example

```c
#include <stdint.h>

uint8_t *ENTITY_PTR [[c6502::zeropage]];
void hit(void);

static uint8_t defense(const uint8_t *m) { return (m[3] >> 1) & 0x0F; }

void attack(uint8_t power) {
    if (power > defense(ENTITY_PTR))
        hit();
    if (power > defense(ENTITY_PTR + 8) + 2)
        hit();
}
```

### Before

```
defense:
   STA   __local_defense__m
   STX   __local_defense__0
   LDY   #$03
   LDA   (__local_defense__m),Y
   LSR
   AND   #$0F
   RTS

attack:
   STA   __local_attack__power
   LDX   ENTITY_PTR+1
   LDA   ENTITY_PTR
   JSR   defense
   CMP   __local_attack__power
   BCS   .if_end@0
   JSR   hit
.if_end@0:
   LDA   ENTITY_PTR
   CLC
   ADC   #$08
   STA   __local_attack__0
   LDA   ENTITY_PTR+1
   ADC   #$00
   TAX
   LDA   __local_attack__0
   JSR   defense
   CLC
   ADC   #$02
   ...
```

### After

```
attack:
   STA   __local_attack__power
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   LSR
   AND   #$0F
   CMP   __local_attack__power
   BCS   .if_end@0
   JSR   hit
.if_end@0:
   LDY   #$0B
   LDA   (ENTITY_PTR),Y
   LSR
   AND   #$0F
   CLC
   ADC   #$02
   ...
```

These are two complete compiles, with the pass turned off and on, and
`...` marks the lines they share. Each copy reads `(ENTITY_PTR),Y`
where the function copied its parameter into a pair of its own, and
the second copy folds `+ 8` into the offset, `LDY #$0B`. The unit
goes from 71 bytes to 49. `defense` in Ali Baba's `combat.c` is this
function, and its three calls are inlined.

## How it works

In TAC, the call `FunctionCall(f, [a1, ..., an], d)` becomes a copy
of each argument into a new local for its parameter, then the body
with each of its locals and labels renamed, then an end label. Each
`Ret(v)` in the body becomes `Copy(v, d)` and a `Jump` to the end
label, which is left out after the body's last instruction. When the
caller returns the call's result at once, or returns nothing after
the call, the copy replaces the caller's `Ret` too, and each `Ret` in
the body stays a `Ret`. The arguments were computed before the call,
in the order `c99_to_tac` gives them, and each copy reads each of
them once, where the call read them, so the side effects and the
volatile accesses happen as they did. Copy propagation then removes
the parameter copies, where the argument isn't written again before
the parameter is read.

A renamed local is `@N.name` or `%N` with a new N, with the symbol
of the local it copies, so its type, `volatile` included, carries
over, and the zero-page slot it gets is still named after the C
variable. The k-th copy's labels are `.ik_` followed by the original
label without its `.`, which keeps the `_start`, `_continue`, and
`_break` endings loop rotation reads and the `_keep` mark dead-loop
elimination reads. Its end label is `.ik@end`.

The functions are taken leaves first, so a function that calls
another candidate is costed and copied with that call already
inlined. When every call of a function has been inlined, the function
is dropped before ABI selection, the call graph, the carry-return
choice, zero-page promotion, and the link metadata read the program,
so none of them name it.

## Choosing the calls

Each function's calls are all inlined, or none are. They are when
the bytes inlining adds are estimated to be at most the bytes it
removes:

```
added   = (n - 1) * BYTES_PER_COST * cost
        + EARLY_RETURN_BYTES * early * (calls not in tail position)
removed = n * (CALL_BYTES + ARGUMENT_BYTE_COST * a + RESULT_BYTE_COST * r) + 1
```

Here n is the number of calls, `a` the bytes of the arguments, `r` the
bytes of the result, and the 1 is the function's `RTS`. `cost` is
`inline_cost` of the function after the TAC optimizer has run on a
copy of it, which counts each instruction by the width of its widest
operand. `early` counts the returns before the end of the body, each
of which becomes a store and a `JMP` in a copy where the function had
an `RTS`. With one call the copy adds no body, so such a function is
inlined unless it has several early returns. With several calls the
body has to be about as small as a call.

The constants come from compiling every unit of the example programs
(Zork, Lode Runner, Ali Baba, and the single-file examples) with each
of the 264 candidates inlined alone at every call, and comparing the
unit's code bytes with no inlining.

- Of the 111 functions with one call, 88 made their unit smaller, 9
  left it the same, and 14 made it larger. Together they saved 286
  bytes. Ten of the 14 have two or more early returns, as Ali Baba's
  `event_penalty` has five (24 bytes larger) and Lode Runner's
  `guard_is_supported` six (37 bytes).
- Of the 153 functions with several calls, 21 made their unit smaller
  with every call inlined, 7 left it the same, and 125 made it larger,
  by 7322 bytes in all. A call in
  c6502 is cheap under `-O`: the arguments arrive in A, X, or the
  callee's own zero-page bytes, and a pointer result comes back in a
  zero-page pointer the caller reads through, so a copy of even a
  14-byte body at each of three calls loses. Lode Runner's
  `inc_anim_state`, with three calls, made `player.c` 26 bytes larger.
- `BYTES_PER_COST` is 3 because a candidate's compiled size was about
  3 bytes for each unit of its optimized cost (`next_string`, 8 units,
  is 25 bytes, and `inc_anim_state`, 5 units, 14).

`CALL_BYTES` is the 3 bytes of a `JSR`. The other three were chosen
from a grid (0 to 2 for `ARGUMENT_BYTE_COST`, 0 to 4 for the other
two) as the values whose choices saved the most bytes over the
measured candidates, added up, without making a program larger. They
came out as 2 bytes for an argument byte, which the caller stores and
the callee reads, 4 for a result byte, which the callee stores and the
caller reads back, and 4 for an early return's store and `JMP`.

## Results

| Program | Before | After | Saved |
|---|---|---|---|
| Zork | 6414 | 6377 | 37 |
| Lode Runner | 10399 | 10310 | 89 |
| Ali Baba | 15213 | 14994 | 219 |
| The single-file examples | 2982 | 2982 | 0 |

The bytes are the sums of each unit's function sizes (`nm.py -S` of
its `-c -O` object, without `__zp_init`). 105 functions are inlined,
13 of them with several calls: Ali Baba's `next_string` (text.c),
`is_player_character` (util.c), `pending_xp` and `current_slot`
(ai.c), `defense`, `get_hostility`, and `apply_damage_to_mob`
(combat.c), and `char_select_patch` (input.c), and Lode Runner's
`stop_digging` (player.c), `paddle_timer_running` (input.c), and
`consider_move`, `guard_half_col`, and `guard_screen_row` (guards.c).
The single-file examples have no candidates.

Two units grow. Lode Runner's `timers.c` is 10 bytes larger, because
`fill_hole` inlined into the loop of `handle_timers` makes the loop
too long for its branches and for [loop test at the
bottom](rotate_loop_test.md), so it gets `JMP`s back and around.
Ali Baba's `hrcg.c` is 3 bytes larger, with ten functions of one call
inlined into `hrcg_cout`, where each of them alone saved bytes. The
estimate leaves out the length of the code around a call.

## When it applies

- The function is defined in the unit with internal linkage, and its
  address isn't taken, so every call of it is a direct call in the
  unit.
- No chain of calls leads from the function back to itself.
- Its declaration has none of `[[c6502::zp_abi]]`, `section`,
  `address`, `return_bool_in_carry`, `[[noreturn]]`, a `register`,
  `in_*`, or `zeropage(addr)` parameter, or a `register` local, which
  set how it's called or where it goes.
- Its parameters and its result are scalars, which one `Copy` moves.
- Its body has no indirect call, no call marked
  `[[c6502::save_hardware_stack]]`, no local whose address is taken,
  and no block-scope `static` object.
- Every call can be inlined: the caller has no `register` local, which
  is pinned to Y, the call isn't marked
  `[[c6502::save_hardware_stack]]`, and each argument has its
  parameter's width and kind.

## What it leaves alone

- A function with external linkage, which another unit may call.
- A function with several calls whose body is larger than about a
  call, and a function with one call and several early returns, by
  the estimate above.
- A block-scope `static` object would be shared correctly by the
  copies, since its name doesn't change, but such a function is left
  alone to keep the pass simple.

## Related

[A pointer returned in zero page](return_in_zero_page.md) and
[carry-flag return without the annotation](carry_return_choice.md)
make the calls of the `static` functions that stay cheaper, and run
after this pass.
