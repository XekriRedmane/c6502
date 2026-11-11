# Reads of X's save slot

`apply_x_save_slot_load` in
[passes/x_save_slot_load.py](../../passes/x_save_slot_load.py)
rewrites a load from the memory byte that X is saved to as a read of X
itself. It runs once under `--optimize`, after the
[count-down counter promotion](counter_to_index_reg.md) and before the
second peephole fixed point, which removes the `STX` / `LDX` pairs the
rewrite leaves dead. The unoptimized pipeline doesn't run it.

When a value lives in X, the code that keeps it there across a call
saves it to a memory byte `M` with `STX M; JSR f; LDX M`. X is the
live copy and `M` is only its spill home. If something also reads the
value with `LDA M`, that load returns whatever X held at the last
`STX M`, and an `INX` or `DEX` since then leaves `M` one step behind.
The pass reads X instead. `LDA M` becomes `TXA`, and a memory-to-memory
copy `Mov(M, D)`, which emits as `LDA M; STA D`, becomes `STX D`.
`TXA` takes 1 byte and 2 cycles against 2 bytes and 3 cycles for `LDA
zp`, and `STX D` saves the whole `LDA`.

None of the 657 C files under `tests/chapter_*/valid/` and
`examples/` makes this pass change anything. The listing below
is the input of the unit test `test_loop_with_dex_buggy_pattern` in
`tests/test_x_save_slot_load.py`, with the slot names `M` and
`zpabi_callee_slot` renamed to look like compiler output.

## Example

### Before

```
f:
   SUBROUTINE

   LDA   #$01
   STA   __local_f__i
   TAX
.loop:
   LDA   __local_f__i
   STA   __zpabi_callee__v
   STX   __local_f__i
   JSR   callee
   LDX   __local_f__i
   DEX
   BPL   .loop
   RTS
```

### After

```
f:
   SUBROUTINE

   LDA   #$01
   STA   __local_f__i
   TAX
.loop:
   STX   __zpabi_callee__v
   STX   __local_f__i
   JSR   callee
   LDX   __local_f__i
   DEX
   BPL   .loop
   RTS
```

On the second iteration the original `LDA __local_f__i` reads the
counter from before the `DEX`, so the callee receives 1 twice. After
the rewrite it receives 1 and then 0, which is the value X holds.

The other rewrite turns `STX M; JSR f; LDX M; DEX; LDA M` into `STX
M; JSR f; LDX M; DEX; TXA`.

## When it applies

- Exactly one memory byte `M` in the function is written by an `STX`.
  If X is stored to two different bytes, it holds more than one value
  over the function's life, and a `TXA` could read the wrong one.
- Every other write to `M` is a store followed, after labels and
  moves that write neither A nor X, by `TAX`. That's the
  initialization shape `LDA c; STA M; TAX`, which leaves A, X, and
  `M` equal. An `STY M`, an `INC M` or `DEC M`, a shift or rotate of
  `M`, an arithmetic or logic operation whose destination is `M`, or
  a pull into `M` disqualifies it.
- Every instruction that writes X leaves X equal to `M`, or one step
  from it. That covers `INX` and `DEX`, `LDX M`, a `TAX` directly
  after a store to `M`, and a call directly followed by `LDX M`. Any
  other load of X, a call without that reload, and a soft-stack
  allocation disqualify the function.

The rewrite applies to each `LDA M` and each copy from `M` to a
`Data` or `ZP` byte that every path from the function's entry reaches
through an `STX M`, an `LDX M`, or the initialization `TAX`. Until
one of those runs, X holds whatever the caller left in it and `M` is
the only copy of the value.

## What it leaves alone

- A function where X holds some other value at a read of `M`. In
  `STX color; LDX x_pixel; LDA color` the `LDA` has to read memory,
  and the `LDX x_pixel` disqualifies the function.
- A call that returns a value in X. After `STX p; JSR f`, X holds f's
  result, and a later `LDA p` must keep reading `p`.
- A read of `M` that comes before X is given its value on some path.
  In `LDA p; STA out; LDX p; DEX; STX p`, with `p` a global, the
  `LDA p` keeps reading memory.
- A copy from `M` to a soft-stack byte, an indirect operand, or an
  indexed byte, since `STX` has no `(zp),Y` or `abs,X` mode.
- A load of `M` into X, which is already the right register.

## Related

[Loop counter in X](loop_counter_to_x.md) does the same `LDA M` to
`TXA` rewrite for the counters it promotes, and
[TXA-STA fold](via_a_store_fold.md) turns a `TXA; STA M` pair into
`STX M`.
