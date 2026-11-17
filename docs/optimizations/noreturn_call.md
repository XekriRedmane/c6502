# A call that doesn't return

`noreturn_calls` in
[passes/optimization/noreturn_call.py](../../passes/optimization/noreturn_call.py)
ends the path at a call to a function declared `[[noreturn]]`. It puts
a `Ret` right after the call and drops the instructions after it, up
to the next label. It runs once per function in the TAC optimizer,
before SSA construction, under `-O` only.

C23 §6.7.13.7 makes it undefined for a `[[noreturn]]` function to
return, so nothing after such a call runs. Without the `Ret`, the code
after the call is the code after the `if` around it, and every value
that code reads has to survive the call, in memory, since the call
may change every register. With it, those values only have to survive
the other way into that code. The `Ret` has no value, even in a
function that returns one, since it never runs, so the call and the
`Ret` are `JSR f; RTS`, which [tail call](tail_call.md) makes `JMP
f`.

## Example

```c
#include <stdint.h>
uint8_t table[8];
[[noreturn]] void fail(void);
uint8_t sum(uint8_t i) {
    uint8_t v = table[i];
    if (v == 0)
        fail();
    return v + table[i + 1];
}
```

### Before

Compiled with the attribute left off:

```
sum:
   LDX   __zpabi_sum__i
   LDA   table,X
   STA   __local_sum__1
   BNE   .if_end@0
   JSR   fail
.if_end@0:
   LDX   __zpabi_sum__i
   LDA   __local_sum__1
   CLC
   ADC   table+1,X
   RTS
```

### After

```
sum:
   LDX   __zpabi_sum__i
   LDA   table,X
   BNE   .if_end@0
   JMP   fail
.if_end@0:
   CLC
   ADC   table+1,X
   RTS
```

`v` stays in A and `i` in X, since the only way to `.if_end@0` is
the branch. Zork's `do_2op` (`examples/zork/interpreter/decode.c`)
reads its operand `a` straight from `OPERANDS` now, where it had
copied it on entry because `internal_error()` came between the copy
and the reads.

## When it applies

- A `FunctionCall` to a function that any of its declarations marks
  `[[noreturn]]` or `[[_Noreturn]]`. The parser records the attribute
  on the declaration, and the type checker sets `FunAttr.noreturn` if
  any declaration has it.
- The instructions after the call up to the next label go, and so
  does a `Ret` among them, which the new `Ret` replaces.

## What it leaves alone

- A call through a function pointer, since the attribute applies to a
  function and not to a pointer's type.
- The function's own body. A `[[noreturn]]` function that returns
  anyway, which C23 leaves undefined, returns as it's written.
- The unoptimized pipeline, which compiles the code after the call as
  it is.

## The call graph

A call to a `[[noreturn]]` function that passes no arguments isn't an
edge of the call graph, in either pipeline
([passes/call_edges.py](../../passes/call_edges.py)). Once the call is
made, nothing reads the caller's parameters or locals again, nor those
of any function the caller was called from, so the callee's bytes may
overlap theirs, and the callee may call the caller again without
making a cycle. `c99_to_tac` sets `noreturn` on the TAC
`FunctionCall`, and every pass that walks the call graph takes its
edges from `call_edges.graph_calls`. These are zp_abi eligibility and
the soft-stack demotion (`abi_selection`), the parameter slots
(`zp_slot_allocation`), the private pools (`zp_local_allocation`), the
shared slots (`slot_sharing.reach_sets`), the hardware-stack cycles
(`recursion_saves`), and the link metadata, which has no `call` line
for the call, so the linker's reallocation leaves it out too.

Lode Runner's deep routines start the game over with `JMP RESET_GAME`,
which in C is a call to `[[noreturn]] void reset_game(void)`. Before,
every function between `reset_game` and those calls was on a cycle and
used the soft stack, a function with a `register` parameter couldn't
call one of them, and the link failed with "cycle detected in zp_abi
call graph". Now none of them uses the soft stack. In the program
below, `deep`, `middle`, and `reset_game` used to be on a cycle, and
now `deep` reads its parameter and keeps its local in zero-page bytes
of its own.

```c
#include <stdint.h>
uint8_t steps, resets, out;
[[noreturn]] void reset_game(void);
uint8_t deep(uint8_t n) {
    uint8_t v = n + 1;
    if (++steps == 3) {
        steps = 0;
        reset_game();
    }
    return v;
}
uint8_t middle(uint8_t n) { return deep(n) + deep(n + 1); }
void reset_game(void) {
    for (;;)
        out = middle(resets);
}
```

```
deep:
   LDA   __zpabi_deep__n
   CLC
   ADC   #$01
   STA   __local_deep__1
   INC   steps
   LDA   steps
   CMP   #$03
   BNE   .if_end@0
   LDA   #$00
   STA   steps
   JMP   reset_game
.if_end@0:
   LDA   __local_deep__1
   RTS
```

A call with arguments stays an edge. The caller writes the arguments
into the callee's parameter slots while its own bytes are still in
use, and when the callee is on a cycle with the caller, those slots
are the ones an earlier activation of the callee reads its arguments
from, so one argument could overwrite another before it's passed on.
A function that can return into itself through ordinary calls is on a
cycle whatever else it calls.

C keeps the caller's objects alive while the callee runs, since the
caller's block never ends, but c6502 doesn't. A pointer to a local of
the caller, or of a function it was called from, must not be used
after the call, and nothing checks this. Each trip around a cycle
through the call leaves the return addresses of the `JSR`s that led
to it on the hardware stack, which wraps around within page 1, so
later trips overwrite them. A function that still uses the soft stack
leaves its frame behind as well, and a cycle through one runs out of
soft stack after enough trips.

`#pragma c6502 call save(hardware_stack)` on such a call is an error,
since nothing of the caller's is needed after it.

## Related

[Tail call](tail_call.md) turns the `JSR f; RTS` into `JMP f`.
