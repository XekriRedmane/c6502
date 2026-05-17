# JSR followed by RTS is just JMP — but the optimizer didn't know that

Catch-up post on my C99-to-6502 compiler. Adding a tail-call peephole
should be three lines: match `JSR foo; RTS`, rewrite to `JMP foo`,
done. It was three lines. It also broke 80 tests, because the
dead-store-elimination pass quietly relied on `JSR` being opaque in a
way `JMP` wasn't.

## The peephole

The function I was looking at ends like every leaf-call sequence
should:

```asm
   LDA   __zpabi_step_pos_p0       ; marshal slot
   STA   __zpabi_apply_bobble_p0
   LDA   __local_step_pos_b3       ; marshal new_anim
   STA   __zpabi_apply_bobble_p1
   JSR   apply_bobble
   RTS
```

The `JSR; RTS` pair pushes a return address, runs `apply_bobble`,
pops it, then immediately pops the *outer* return address and goes
home. Equivalent to:

```asm
   ...
   JMP   apply_bobble               ; falls through to outer caller
                                    ; via apply_bobble's own RTS
```

Saves one push/pop pair: 7 cycles, 2 bytes, every time it fires.

The matcher in my compiler's asm IR works on two atoms:

```python
if (isinstance(instrs[i], asm_ast.Call)
        and isinstance(instrs[i + 1], asm_ast.Return)):
    out.append(asm_ast.Jump(target=instrs[i].name))
    i += 2
    continue
```

I only match `Return` (the bare-RTS atom), never `Ret(arg_bytes, …)`
which carries a non-trivial frame teardown sequence the function
needs to run before exit. The bare-RTS form gets emitted exactly when
the function has no frame to tear down — which is the common case
for the zp_abi leaves I care about.

Unit tests passed. I ran the full suite expecting maybe a gold file
update. Got 80 failures.

## The bug

After the peephole, the same function now ends:

```asm
   LDA   __zpabi_step_pos_p0       ; gone
   STA   __zpabi_apply_bobble_p0   ; gone
   LDA   __local_step_pos_b3       ; gone
   STA   __zpabi_apply_bobble_p1   ; gone
   JMP   apply_bobble
```

All four marshalling instructions, *gone*. The dead-store
elimination decided they were dead, because nothing in this function
reads `__zpabi_apply_bobble_p0` after the store.

Of course — nothing in *this function* does. The reader is
`apply_bobble`, on the other side of the call.

The DSE pass models `Call(name)` as opaque: it might read any memory,
so the walk that decides whether a store is dead bails when it hits
one. But after the peephole, the function ends with `Jump(name)`,
not `Call(name)`. The walk's `_successors(Jump)` does a label lookup,
and `apply_bobble` isn't a label in this function. Empty successor
list. Walk terminates. Store classified as dead.

The runtime symptom: `apply_bobble(slot, new_anim)` runs with whatever
stale bytes happened to be at `__zpabi_apply_bobble_p0` and `_p1`
from the last time someone called it. My sim-differential test
caught it instantly (the recorded `bobble_last_idx` was zero where
it should have been 7).

## The fix

One condition in `_is_dead_cfg`:

```python
if (isinstance(nxt, asm_ast.Jump)
        and nxt.target not in label_to_index):
    # Jump to a non-local label is a tail-call — opaque,
    # same reasoning as Call.
    return False
```

A `Jump` whose target isn't in the function's local label map IS a
tail-call (because every other `Jump` in this codegen goes to a loop
header, an `if`-end, etc., all minted as `.something@N` local
labels). Treat it like `Call` — opaque.

Suite back to green.

## What I'm taking from this

The DSE pass treated `Jump` as "just control flow" because, before
this peephole existed, every `Jump` *was* just control flow within
a function. Adding a tail-call peephole silently changed the
invariant — `Jump` could now leave the function. Same instruction
mnemonic in dasm output (just `JMP`), totally different liveness
semantics.

If you're adding an inter-procedural tail-call to a backend that's
been single-procedure-aware everywhere, audit every flow analysis
for "what if this transfer leaves the function?" — it's a
two-condition fix per pass, but you have to find them.

The full pass is 30 lines plus the docstring, in
`passes/tail_call.py` if anyone wants to lift it.
