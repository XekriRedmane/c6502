# Rematerialization of staged values

`apply_remat` in [passes/asm_remat.py](../../passes/asm_remat.py)
replaces a read of a zero-page staging slot with a fresh load from
the place the value came from, which leaves the store into the slot
with no reader. It runs in the peephole fixed point. The fixed point
runs in both pipelines, but the pass can fire only in the optimized
one, because the staging slots it rewrites are the `__local_<fn>__*`
body-local slots that the asm register allocator hands out. The
unoptimized pipeline keeps locals on the soft stack, and it passes no
slot-symbol map, which the pass's second step requires.

After register allocation, a value that has to live across code that
clobbers A is often parked in a body-local slot with `STA` and read
back later with `LDA`. When the value was loaded from an immediate, a
static, or a static indexed by X or Y, and nothing between the store
and the read can change that source, loading the source again gives
the same byte. The pass rewrites the read to load the source. A
second step in the same call then finds every store into a
`__local_*` slot that nothing in the function reads. A store from A,
X, or Y is dropped, and a memory-to-memory store becomes a plain load
into A. The load that fed the dropped store is usually dead too, and
a later pass in the fixed point removes it. In the example below, the
whole change saves 4 bytes and 6 cycles, counted from the listings,
and the function no longer uses its slot.

## Example

```c
static const uint8_t glyph[8] = {1, 2, 4, 8, 16, 32, 64, 128};
uint8_t plot(uint8_t i, uint8_t *p) {
    uint8_t bits = glyph[i];
    *p = 0;
    return bits;
}
```

### Before

```
   LDX   __zpabi_plot__i
   LDA   glyph,X
   STA   __local_plot__0
   LDA   __zpabi_plot__p_0
   STA   DPTR
   LDA   __zpabi_plot__p_1
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   ($81),Y
   LDA   __local_plot__0
   RTS
```

### After

```
   LDX   __zpabi_plot__i
   LDA   glyph,X
   LDA   __zpabi_plot__p_0
   STA   DPTR
   LDA   __zpabi_plot__p_1
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   ($81),Y
   LDA   glyph,X
   RTS
```

[Memory value propagation](memory_value_propagation.md) makes the
same kind of rewrite across the whole CFG, and it runs earlier in each
sweep of the fixed point. It discards every fact about slot contents
at a store through a pointer, though, so after the `*p = 0` here the
read is left for this pass. Afterward, [asm dead-store elimination](asm_dead_store.md)
drops the two `DPTR` stores, and `apply_dead_a_arith_elimination`
drops the first `LDA glyph,X` along with the two loads of `p`.

## When it applies

The rewrite starts from a staging store, a `Mov` that isn't volatile
into a `__local_*` slot at offset 0. The value stored is either the
source of a memory-to-memory `Mov`, or the source of the nearest
earlier load into A in the same block. The backward search for that
load stops at any other instruction that writes A, such as an `ADC`,
an `AND`, a shift of A, or a `PLA`. The source must be one of these.

- An `Imm`, `ImmLabelLow`, or `ImmLabelHigh` operand, which no
  instruction can change.
- A `Data` operand whose name is not the destination of any
  instruction in the function, when nothing between the load and the
  read stores through a pointer that can reach it.
- An `IndexedData` operand under the same conditions on its name, when
  nothing between the load and the read writes the index register.
  An operand lowered with an `LDY #off` setup (`Frame`, `Stack`,
  `Indirect`, `IndirectZp`) counts as a write to Y.

From the staging store, the pass scans forward in the same block and
rewrites every `Mov` that reads the slot and isn't volatile. The scan
stops at a label, a jump or branch, a call, a return, a
`FunctionPrologue`, an `AllocateStack`, a `LoadAddress`, or any
instruction that writes the slot.

A store through a pointer (`(DPTR),Y` or `(zp),Y`) or an indexed
store to a raw address, between the load and the read, counts as a
possible write to the static. The exception is a static with internal
linkage whose address the program never forms, by an `ImmLabelLow` or
`ImmLabelHigh` operand, a `LoadAddress`, or an `AddressInit` in some
static's initializer. No pointer can point into such a static. The
example above is rewritten because `glyph` is `static` and only ever
read as `glyph,X`. With a global `g`, the function `uint8_t f(uint8_t
*p) { uint8_t t = g; *p = 7; return t; }` keeps its slot, since `g`
can be reached from another translation unit, and `f(&g)` after `g =
3` returns 3. A store to another static indexed by X or Y doesn't
count, because in a valid program it stays inside that static. The
pass has no access to `const` qualifiers, so a global `const` table
loses the rewrite across a pointer store as well.

The second step drops or shortens a non-volatile `Mov` into a
`__local_*` slot at offset 0 when no instruction in the function reads
the slot's address. The addresses are compared through the slot-symbol
map, so a slot that forms one byte of a pointer pair for an
`(zp),Y` access counts as read. A slot whose address is built
anywhere in the function, by an `ImmLabelLow`, an `ImmLabelHigh`, or a
`LoadAddress`, is kept, since a pointer can read it without naming it.

A `Mov` of a constant or a memory byte into memory, `Mov(#k, M)` or
`Mov(M, N)`, emits as `LDA src; STA dst`, so it counts as the
producer of A, with `src` its value. The search used to pass over it
and take an earlier load for A's value, which rematerialized
`Mov(#$80, l19); STA t` as `#$FF` (fuzz.stmts seed 25347).

## What it leaves alone

- A slot updated in place after the store. `glyph[i] + 1` compiles to
  `STA __local_plot__0; INC __local_plot__0`, and the `INC` ends the
  scan, so the later `LDA __local_plot__0` stays.
- A read with a call between it and the store, such as `bits =
  glyph[i]; tick(); return bits;`. The callee can write the static,
  and it clobbers X and Y.
- A read with a store through a pointer between it and the store,
  when the static is global or its address is taken anywhere in the
  program, as with `t = g; *p = 7; return t;`.
- A read in a different block from the store, since the scan is
  block-local. Memory value propagation covers some of these.
- A value loaded from the soft stack or through a pointer (`Frame`,
  `Stack`, or any indirect operand), because the frame pointer or the
  pointer bytes may change before the read.
- A value produced by arithmetic in A, because no single operand
  reproduces it.
- A volatile load or store at either end.

## Related

[Memory value propagation](memory_value_propagation.md) does the
CFG-wide version of the rewrite. [Asm dead-store
elimination](asm_dead_store.md) and [dead A
arithmetic](dead_a_arith.md) clean up after it.
