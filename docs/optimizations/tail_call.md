# Tail call

`apply_tail_call` in [passes/tail_call.py](../../passes/tail_call.py)
turns a `JSR f` followed by a bare `RTS` into `JMP f`. It runs in the
peephole fixed point, which both pipelines run, but it can't fire in
the unoptimized pipeline. There every function exits through the
compound `Ret` atom, and the pass matches only the bare `Return`.

When a function's last act is a call, `JSR f; RTS` pushes a return
address that `f`'s `RTS` pops only to land on another `RTS`. `JMP f`
lets `f`'s `RTS` return straight to the original caller. The caller
gets back the same registers and flags, because nothing runs between the
two `RTS` instructions. The jump saves 1 byte (4 bytes become 3), 9
cycles (the `JSR` and the extra `RTS` take 6 each, the `JMP` takes
3), and 2 bytes of hardware stack.

## Example

```c
unsigned char mode;
void redraw(unsigned char m);
void set_mode(unsigned char m) {
    mode = m;
    redraw(m);
}
```

### Before

```
set_mode:
   LDA   __zpabi_set_mode__m
   STA   mode
   STA   __zpabi_redraw__m
   JSR   redraw
   RTS
```

### After

```
set_mode:
   LDA   __zpabi_set_mode__m
   STA   mode
   STA   __zpabi_redraw__m
   JMP   redraw
```

## When it applies

A `Call` is directly followed by a `Return`, anywhere in the
function. The new `Jump` keeps the call's `reg_args`, so the liveness
helpers still see that it reads the registers the callee takes its
arguments in.

`Return` is the bare exit that `tac_to_asm` emits in the optimized
pipeline, and `prologue_synthesis` keeps it only for a function with
no frame, no parameter bytes or locals on the soft stack, and no
callee-saved bytes. Any other function ends in a `Ret`, whose
teardown of the soft-stack frame has to run after the call returns.

## What it leaves alone

- A function with a soft-stack frame. When `set_mode`'s address is
  taken (`hook = set_mode;` inside a function), it falls back to the
  soft-stack convention, and its `JSR redraw` is followed by the frame
  teardown, so the pass reports no change.
- A call followed by anything other than the exit, such as a store of
  the result.
- Every function in the unoptimized pipeline.

## Related

[Prologue synthesis](prologue_synthesis.md) replaces each `Return`
with a `Ret` in a function with a frame, and
[Tail merge](tail_merge.md) shares repeated return sequences.
