# JSR followed by RTS is just JMP — but the optimizer didn't know that

Catch-up post on my C99-to-6502 compiler. Adding a tail-call peephole
should be three lines: match `JSR foo; RTS`, rewrite to `JMP foo`,
done. It was three lines. It also broke 80 tests, because the
dead-store-elimination pass treated `JSR` and `JMP` differently in a
way that quietly relied on `JSR` being the only way to leave a
function.

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

To explain why DSE got confused, here's what it actually does. For
each candidate store `STA $XX`, the pass walks forward through the
control-flow graph from the store, following every successor edge,
asking at each instruction: "is `$XX` read here?" If it ever finds
a read on any path, the store is **live** — keep it. If every path
either (a) overwrites `$XX` before reading it, or (b) reaches a
function exit without anyone having read it, the store is **dead** —
drop it.

The pass also has to handle instructions whose effect it can't model
precisely. When the walker hits one of those, it gives up on that
path and conservatively assumes the store is live. The pass calls
this **opaque**.

`Call(name)` (= `JSR`) is opaque. The callee can read any memory the
caller cares about — we don't try to inspect the callee's body to
prove otherwise.

`Jump(name)` (= `JMP`) is *not* opaque. It's plain control flow:
look up the target label in the function's local label map, push
that index as the next instruction to visit, continue the walk.
That's exactly right for the `JMP` to a loop header, or the `JMP`
out of an `if`-branch to a join — every `Jump` this codegen produced
before the peephole stayed inside the function.

The peephole introduced a new shape: `Jump(apply_bobble)`. The
target isn't a local label — it's the name of an *external*
function. The walker did its label lookup, found nothing,
returned an empty successor list, and treated the empty list as
"this path exits cleanly with no reader found." Walk terminates.
Store classified as dead. Apply this to all four marshalling
stores. Drop them all.

Then `apply_bobble` runs with whatever stale bytes were already in
its param slots from the last unrelated call.

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
a function. The "opaque vs transparent" split between `Call` and
`Jump` wasn't a value judgment about the instructions — it was an
artifact of where they showed up. `Call` was the only way to
transfer to code we couldn't see; `Jump` always went somewhere
inside the function whose CFG we owned.

The peephole quietly violated that invariant. Same instruction
mnemonic in the dasm output (just `JMP`), totally different
liveness semantics: a `JMP` to a local label preserves the
"within this function" assumption the DSE walker depends on; a
`JMP` to an external function name doesn't, and the walker had
no way to tell the difference because it never had to before.

If you're adding an inter-procedural tail-call to a backend
that's been single-procedure-aware everywhere, audit every flow
analysis for "what if this transfer leaves the function?" — it's
a two-condition fix per pass (check whether the target resolves
inside the current function; if not, treat as opaque), but you
have to find them.

The full pass is 30 lines plus the docstring, in
`passes/tail_call.py` if anyone wants to lift it.
