# Dispatch on a small pointer table

`dispatch_const_pointer_arrays` in
[passes/optimization/dispatch_pointer_array.py](../../passes/optimization/dispatch_pointer_array.py)
rewrites `arr[i][j]`, where `arr` is a small file-scope table of
pointers to other statics, as a compare-and-branch on `i` that reads
`target_k[j]` directly in each arm. `compile.optimized_asm` calls it
on the whole program right after the TAC optimizer returns, so its
input is the TAC after SSA destruction. It doesn't run in the
unoptimized pipeline.

The generic lowering loads the 2-byte pointer `arr[i]` with
`ASL A; TAX; LDA arr,X; ...; LDA arr+1,X`, stores it in a zero-page
pair, and reads through the pair with `LDY j; LDA (ptr),Y`. That
takes X, Y, and two zero-page bytes at the same time. After the
rewrite, each arm is `LDX j; LDA target_k,X`, and Y and the
zero-page pair are free. A loop counter that lives in X or Y can then
stay there across the access. In the example below the function
body grows from 18 bytes to 19, but nothing reads the 4-byte `frames`
table any more, so the table is removed. The read takes 15 or 17
cycles where it took 29.

## Example

```c
#include <stdint.h>
static const uint8_t walk[4] = {1, 2, 3, 4};
static const uint8_t jump[4] = {5, 6, 7, 8};
static const uint8_t * const frames[2] = {walk, jump};
uint8_t frame(uint8_t anim, uint8_t step) { return frames[anim][step]; }
```

### Before

```
Binary(<<, @0.anim, 1, %2.1)
IndexedLoad(frames, %2.1, %3.1)
IndirectIndexedLoad(%3.1, @1.step, %6.1)
Ret(%6.1)
```

### After

```
JumpIfCmp(==, @0.anim, 0c, .dispatch@0@case@0)
IndexedLoad(jump, @1.step, %6.1)
Jump(.dispatch@0@end)
Label(.dispatch@0@case@0)
IndexedLoad(walk, @1.step, %6.1)
Jump(.dispatch@0@end)
Label(.dispatch@0@end)
Ret(%6.1)
```

The last element is the fall-through case, so a table of N pointers
takes N - 1 compares. The final assembly is
`LDA __zpabi_frame__anim; CMP #$00; BEQ .dispatch@0@case@0;
LDX __zpabi_frame__step; LDA jump,X; JMP .dispatch@0@end`, followed
by `LDX __zpabi_frame__step; LDA walk,X` for case 0.

## When it applies

The pass first collects the tables it can dispatch on. A table
qualifies when it's a `StaticVariable` without external linkage, and
its initializer is a list of at most 8 entries, each the address of
another named static with no offset.

In each function it then looks for this chain.

- An `IndirectIndexedLoad(%P, j, %V)` that isn't volatile, with a
  variable `%V`. The index `j` can be any value.
- `%P` is read only by that load, and its latest definition before
  the load is a non-volatile `IndexedLoad(arr, %S, %P)` on one of the
  collected tables.
- `%S` is read only by that `IndexedLoad`, and its latest definition
  before it is `i << 1` or `i * 2` for a variable `i`.

The three instructions don't have to be adjacent. The pass removes
them and puts the dispatch where the `IndirectIndexedLoad` was. It
repeats until no chain is left in the function. Each arm writes the
same `%V`, which is why the pass runs after SSA destruction, where no
`Phi` is needed at the join.

The pass doesn't check that the table or its elements are `const`,
although the module docstring says the table is a
`static const T * const[N]`. A table that the program writes at run
time is dispatched on its initial contents. With
`static uint8_t *frames[2] = {walk, jump};` and a function that
stores `other` into `frames[0]`, the final assembly of `frame` still
reads `walk,X` for `anim == 0`.

## What it leaves alone

- A table with more than 8 entries, where the compare chain would
  cost more than the indirect load.
- A table with external linkage, or one with a null entry, an entry
  that isn't an address, or an address with an offset such as
  `&buf[2]`.
- A volatile access, through either the table or the pointer. One
  volatile read would become a read in each arm, which changes the
  sequence of volatile accesses.
- A pointer read from the table that's also used for something else.
- A load wider than a byte through the pointer, which never becomes
  an `IndirectIndexedLoad`.

## Related

[Indirect-indexed access](recognize_indirect_indexed.md) produces
the `IndirectIndexedLoad` that this pass matches.
