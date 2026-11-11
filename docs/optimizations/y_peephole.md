# Redundant LDY setup

`apply_y_peephole` in [passes/y_peephole.py](../../passes/y_peephole.py)
removes an `LDY #n` when Y already holds `n`, and replaces it with
`INY` or `DEY` when Y holds `n - 1` or `n + 1`. It works on the
emitted assembly text. `asm_emit.emit_program` calls it on the
finished line list as its last step, so it runs once, after every IR
pass, in both the optimized and the unoptimized pipelines.

The emitter writes an `LDY #offset` in front of every access through
a pointer or the soft stack, because it addresses both with `(zp),Y`
and the offset goes in Y. Two accesses at the same offset get two
identical `LDY` instructions, and two at neighboring offsets get two
`LDY` instructions where the second could be a 1-byte `INY` or `DEY`.
The pass walks the lines in order and tracks the value Y is known to
hold. A dropped `LDY #n` saves 2 bytes and 2 cycles, and an `INY` or
`DEY` in its place saves 1 byte at the same cycle count. Both set N
and Z from the same value the `LDY` would have.

Since the pass reads text, `optdiff.py` can't capture it. The listings
below come from a short script that wraps `apply_y_peephole` and
prints the line list it receives and returns.

## Example

```c
struct pt { unsigned char x, y, z; };

void bump(struct pt *p)
{
    p->x = p->x + 1;
    p->y = p->y + 1;
}
```

### Before

```
bump:
   SUBROUTINE

   LDY   #$00
   LDA   (__zpabi_bump__p_0),Y
   STA   __local_bump__0
   INC   __local_bump__0
   LDA   __local_bump__0
   LDY   #$00
   STA   (__zpabi_bump__p_0),Y
   LDA   __zpabi_bump__p_0
   CLC
   ADC   #$01
   STA   __local_bump__1
   LDA   __zpabi_bump__p_1
   ADC   #$00
   STA   __local_bump__2
   LDY   #$00
   LDA   (__local_bump__1),Y
   STA   __local_bump__0
   INC   __local_bump__0
   LDA   __local_bump__0
   LDY   #$00
   STA   (__local_bump__1),Y
   RTS
```

### After

```
bump:
   SUBROUTINE

   LDY   #$00
   LDA   (__zpabi_bump__p_0),Y
   STA   __local_bump__0
   INC   __local_bump__0
   LDA   __local_bump__0
   STA   (__zpabi_bump__p_0),Y
   LDA   __zpabi_bump__p_0
   CLC
   ADC   #$01
   STA   __local_bump__1
   LDA   __zpabi_bump__p_1
   ADC   #$00
   STA   __local_bump__2
   LDA   (__local_bump__1),Y
   STA   __local_bump__0
   INC   __local_bump__0
   LDA   __local_bump__0
   STA   (__local_bump__1),Y
   RTS
```

Nothing between the first `LDY #$00` and the `RTS` writes Y, so the
other three are dropped, even though the second pair of accesses goes
through a different pointer.

The `INY` and `DEY` forms show up in soft-stack code. In the
unoptimized compile of `int add(int a, int b) { return a + b; }`, the
epilogue's `LDY #$03; LDA (FP),Y; TAX; LDY #$04; LDA (FP),Y` becomes
`LDY #$03; LDA (FP),Y; TAX; INY; LDA (FP),Y`, and a later `LDY #$01`
that follows an `LDY #$02` becomes `DEY`.

## When it applies

The pass tracks Y through each run of lines between labels.

- An `LDY #n` with a numeric operand sets the known value to `n`.
- `INY` and `DEY` step the known value, wrapping at 0xFF.
- `TAY`, `JSR`, an `LDY` from memory, and an `LDY #<label` or `LDY
  #>label` make Y unknown, as does every label line.
- Any other instruction leaves the known value as it is. On the NMOS
  6502 only `LDY`, `TAY`, `INY`, and `DEY` write Y, and a callee may
  write it, which the `JSR` rule covers.

An `LDY #n` is then dropped when Y is known to hold `n`, and replaced
with `INY` or `DEY` when Y is known to hold `n - 1` or `n + 1` modulo
256.

## What it leaves alone

- An `LDY` after a label, even when every path into the label leaves
  Y at the same value. In

  ```c
  void f(struct pt *p, unsigned char c)
  {
      unsigned char v = p->x;
      if (c)
          v = v + 3;
      p->x = v;
  }
  ```

  Y is 0 on both paths into `.if_end@0`, but the `LDY #$00` after it
  stays.
- An `LDY` after a `JSR`, including a call to a routine declared
  `preserves_y`.
- An `LDY #n` whose value differs from the known one by more than 1.

## Related

[DEC-branch indirect-Y fold](dec_branch_indirect_y_fold.md) removes
the `LDY #$00` setup in the IR when a `DEY; BNE` loop has just left Y
at 0.
