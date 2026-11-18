# Link-time address arithmetic

`apply_label_offset_fold` in
[passes/label_offset_fold.py](../../passes/label_offset_fold.py) turns
a 2-byte add of a label's address and a constant into one immediate
address, `#<(name+k)` and `#>(name+k)`. It runs in the peephole fixed
point, which both pipelines run.

`&r[2].a` on a static array, or the `&prompts + 15` that [indexed
access to a static array of structs](static_indexed_access.md) leaves
for a member's pointer, reaches the asm as an add of the label's low
and high halves and a constant, one byte at a time. The sum is known
when the program is linked, and dasm computes `name+k` as a full
address, so the high half carries out of the low as the `ADC` did.

## Example

```c
struct rec { char t[5]; unsigned int w; char a; };
struct rec r[4];
unsigned char deref(const unsigned char *p);
int main(void) { return deref(&r[2].a); }
```

### Before

```
   LDA   #$17
   CLC
   ADC   #<r
   STA   __zpabi_deref__p_0
   LDA   #$00
   ADC   #>r
   STA   __zpabi_deref__p_1
```

### After

```
   LDA   #<(r+23)
   STA   __zpabi_deref__p_0
   LDA   #>(r+23)
   STA   __zpabi_deref__p_1
```

That's 4 instructions instead of 7.

The low byte alone folds too. `(uint8_t)(uint16_t)&LOCS[1]`, on an
array of 9-byte records at a fixed address, reaches the asm as `LDA
#<LOCS; CLC; ADC #$09; STA t` with nothing after it that reads the
carry. The low byte of the sum doesn't depend on the carry out, so
that becomes `LDA #<(LOCS+9); STA t`, and memory value propagation
then compares with the immediate, `CMP #<(LOCS+9)`.

## When it applies

- The low byte is a load into A, a `CLC` before or after it, an `ADC`,
  and a store; the high byte follows at once as a load, an `ADC`, and
  a store. One operand of each `ADC` (either one) is the label's half,
  `ImmLabelLow` and then `ImmLabelHigh` of the same name and offset,
  and the other is an immediate.
- The stores go to `Data` or `ZP` operands, or to X or Y (`TAX`,
  `TAY`), and nothing is volatile. A function returning `T + 7` for an
  array `T` at a fixed address puts the high byte in X, and the fold
  leaves `LDX #>(T+7); LDA #<(T+7); RTS` once later passes have
  shortened it.
- C and V are dead after the last store, since the `ADC`s set them and
  the loads don't. A and N/Z come out the same either way.
- When no high byte follows, the low byte folds alone if C and V are
  dead after its store.

## What it leaves alone

- An add of a label and a variable, such as `prompts + offset`.
- A high byte of an address on its own, since it depends on the
  carry out of the low byte.

## Related

[Constant AND and ORA fold](const_arith_fold.md) does the same for two
immediates.
