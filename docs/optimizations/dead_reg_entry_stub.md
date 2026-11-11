# Dead register-parameter entry stub

`apply_dead_reg_entry_stub_drop` in
[passes/dead_reg_entry_stub.py](../../passes/dead_reg_entry_stub.py)
deletes the store that copies a `register` parameter from its
register into its zero-page slot at function entry, when the body
never reads the slot. It runs once under `--optimize`, after the
third peephole fixed point and just before the
[compare-key promotion](loop_invariant_a.md). The unoptimized
pipeline doesn't run it.

`tac_to_asm` starts a function that has `register` parameters with an
entry stub such as `STX __zpabi_<fn>__<param>`, so that the body can
read the parameter from its slot like any other zp_abi byte. When the
register allocator keeps the parameter in its register, the body no
longer reads the slot and the stub is a dead store.
[Asm dead-store elimination](asm_dead_store.md) removes it when the
loop below reads a volatile global in place of `*port`. It can't
remove it when the body reads memory through a pointer, as in `CMP
(DPTR),Y`, because such a read might reach any byte. The slot can't be the target of that read, since
there's no way to write `&__zpabi_*` in C and taking the address of a
`register` parameter is a type error. So this pass drops the stub
whenever nothing in the function names the slot as a source. Each
stub is 2 bytes and 3 cycles.

## Example

```c
extern const volatile uint8_t *port;

void clicks(register uint8_t n __attribute__((in_x)))
{
    do {
        (void)*port;
    } while (--n != 0);
}
```

### Before

```
clicks:
   SUBROUTINE

   STX   __zpabi_clicks__n
   LDA   port
   STA   DPTR
   LDA   port+1
   STA   DPTR+1
.loop@0_start:
   LDY   #$00
   CMP   (DPTR),Y
   DEX
   BNE   .loop@0_start
   RTS
```

### After

```
clicks:
   SUBROUTINE

   LDA   port
   STA   DPTR
   LDA   port+1
   STA   DPTR+1
.loop@0_start:
   LDY   #$00
   CMP   (DPTR),Y
   DEX
   BNE   .loop@0_start
   RTS
```

## When it applies

- The function uses the zp_abi convention, and at least one of its
  parameter bytes arrives in a register (A, X, or Y), as the
  convention records for each `register` parameter.
- The stub is a non-volatile `Mov` from that same register into the
  byte's slot symbol at offset 0.
- No instruction in the function reads that slot symbol. A read is
  any operand position other than the destination of a move, a pull,
  or an address load, so an `INC` or an `ADC` into the slot counts as
  a read.

The pass doesn't check where the stub sits, or whether the body keeps
the parameter in its register. If the slot is never read, the stored
value is never used.

## What it leaves alone

- A parameter the body reads from its slot. In

  ```c
  uint8_t poll(register uint8_t mask)
  {
      uint8_t v;
      do { v = *port; } while ((v & mask) == 0);
      return v;
  }
  ```

  the loop tests `AND __zpabi_poll__mask`, so `STA
  __zpabi_poll__mask` stays.
- A store into the slot from a register other than the one the
  convention names for that byte, or from memory.
- Functions on the soft stack, and zp_abi functions with no
  `register` parameters, whose slots the caller writes.

## Related

[Asm dead-store elimination](asm_dead_store.md) is the general pass,
and [zp_abi](zp_abi.md) describes the slots and the register
convention.
