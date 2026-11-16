# A call that doesn't return

`noreturn_calls` in
[passes/optimization/noreturn_call.py](../../passes/optimization/noreturn_call.py)
ends the path at a call to a function declared `[[noreturn]]`. It puts
a `Ret` right after the call and drops the instructions after it, up
to the next label. It runs once per function in the TAC optimizer,
before SSA construction, under `--optimize` only.

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

## Related

[Tail call](tail_call.md) turns the `JSR f; RTS` into `JMP f`.
