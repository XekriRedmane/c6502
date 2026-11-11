# Commuted ADC, AND, and ORA

`apply_adc_commute` in
[passes/adc_commute.py](../../passes/adc_commute.py) rewrites `STA
temp; ...; LDA mem; CLC; ADC temp; STA mem` so that the `ADC` reads
`mem` and the `LDA mem` goes away. It runs in the peephole fixed
point, which both pipelines run, but no program in the test corpus
makes it fire in the unoptimized pipeline, where the spill slot is
on the soft stack and the pass accepts only a `Data` or `ZP` one.

A compound assignment `mem += v`, with `v` just computed into A,
spills A to a temporary, loads `mem`, and adds the temporary. A
still holds `v` when the `LDA mem` runs, and addition is
commutative, so `ADC mem` computes the same sum without the load.
`AND` and `ORA` are commutative too and get the same rewrite. The
pass leaves the `STA temp` in place, and `asm_dead_store` deletes it
on a later sweep when nothing else reads `temp`. For an absolute
`mem` and a zero-page `temp`, the rewrite saves 2 bytes and 3 cycles,
and the store's removal saves 2 bytes and 3 more.

## Example

```c
unsigned char total;
unsigned char sample(void);
void accumulate(void) {
    total += sample();
}
```

### Before

```
accumulate:
   JSR   sample
   STA   __local_accumulate__0
   LDA   total
   CLC
   ADC   __local_accumulate__0
   STA   total
   RTS
```

### After

```
accumulate:
   JSR   sample
   STA   __local_accumulate__0
   CLC
   ADC   total
   STA   total
   RTS
```

The final output is `JSR sample; CLC; ADC total; STA total; RTS`.

## When it applies

- The window opens with a non-volatile `STA temp`, where `temp` is a
  `Data` or `ZP` operand.
- Any number of instructions may follow before the load, as long as
  each is a `Mov`, `INC`, `DEC`, `CLC`, `SEC`, `CMP`, or `BIT`, isn't
  a `Mov` into A, and has no operand that `may_alias` says could
  address `temp`. This band admits the `LDX` or `LDY` that sets up an
  indexed `mem`.
- Next is a non-volatile `LDA mem`, where `mem` is a `Data`, `ZP`, or
  `abs,X` / `abs,Y` operand, optionally followed by `CLC` or `SEC`.
- Then comes an `ADC`, `AND`, or `ORA` of `temp` into A, and right
  after it a non-volatile `STA mem` to the same operand as the load.

The band check rejects a `Mov` only when its destination is A. A
`Mov` that stores an immediate or copies memory to memory has a
memory destination but emits through A (`LDA #imm; STA dst`), so it
passes the check and overwrites `v`. With the declarations above
and an `unsigned char other`, this miscompiles

```c
void accumulate(void) {
    unsigned char v = sample();
    other = 7;
    total += v;
}
```

into `JSR sample; LDA #$07; STA other; CLC; ADC total; STA total`,
which adds 7 instead of `v`. With `sample` returning 2 and `total`
starting at 10, the simulator ends with 17 where the unoptimized
build gives 12. This is a bug in the current code.

## What it leaves alone

- A subtract. `total -= sample();` keeps `LDA total; SEC; SBC
  __local_accumulate__0`, because `SBC` isn't commutative.
- An `EOR`, whose asm atom has a different shape.
- A band holding a call, a push or pull, a branch, a label, or any
  other instruction outside the list above.
- A store after the operation that goes somewhere other than `mem`,
  and any volatile load or store in the window.

## Related

[Asm dead store](asm_dead_store.md) deletes the spill that this pass
leaves unread.
