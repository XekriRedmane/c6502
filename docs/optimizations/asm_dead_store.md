# Asm dead-store elimination

`apply_asm_dead_store` in
[passes/asm_dead_store.py](../../passes/asm_dead_store.py) removes a
store to a byte of memory when no instruction reachable from the
store reads that byte before it is overwritten or the function
returns. It runs in the peephole fixed point, in both the optimized
and the unoptimized pipelines. The unoptimized pipeline passes no
slot-symbol map, so there the pass can't treat a zero-page slot
symbol as dead at return, but it still fires on stores that are
overwritten, such as the first `STA g` in `g = 1; g = 2;`.

Many of the dead stores come from earlier peepholes. Once [memory
value propagation](memory_value_propagation.md) rewrites a
`(DPTR),Y` access to read the pointer from its own zero-page pair, the
stores that staged the pointer in `DPTR` have no reader. Once
[rematerialization](asm_remat.md) or a redundant-load rewrite drops
the last read of a temporary, its store has none either. A store to
an absolute address is 3 bytes and 4 cycles, and a store to zero page
is 2 bytes and 3 cycles. The load that fed a dropped store is often
dead as well, and `apply_dead_a_arith_elimination` removes it later in
the fixed point.

The walk is a forward search over the function's control-flow graph,
so a store can be dead because every branch overwrites it, which a
scan of one block can't show.

## Example

```c
uint8_t g;
void f(uint8_t a) { g = 1; if (a) g = 2; else g = 3; }
```

### Before

```
   LDA   #$01
   STA   g
   LDA   __zpabi_f__a
   BEQ   .if_else@1
.f@asm_ssa_block@1:
   LDA   #$02
   STA   g
   JMP   .if_end@0
.if_else@1:
   LDA   #$03
   STA   g
.if_end@0:
   RTS
```

### After

```
   LDA   #$01
   LDA   __zpabi_f__a
   BEQ   .if_else@1
.f@asm_ssa_block@1:
   LDA   #$02
   STA   g
   JMP   .if_end@0
.if_else@1:
   LDA   #$03
   STA   g
.if_end@0:
   RTS
```

The pass has two other rewrites. For `void clear(uint8_t *p) { *p =
0; }` it drops both `STA DPTR` and `STA DPTR+1` after memory value
propagation has changed the store to `STA ($80),Y`, because `DPTR` is
scratch that is dead at return. For `return (uint8_t)(w << 1);` with
a global `uint16_t w`, the shift of a copy of `w` into two slots ends
in `ROL __local_lo2__1`, and the pass drops the `ROL` because nothing
reads the high byte or the carry it sets.

## When it applies

The pass considers three kinds of instruction.

- A store from A, X, or Y into a `Data` or `ZP` operand that isn't
  volatile. When it's dead, the pass drops it.
- A memory-to-memory `Mov` into a `Data` or `ZP` operand that isn't
  volatile. When the store half is dead, the pass turns the `Mov`
  into a load into A, since A's new value or the flags may still be
  read. Most of these were already split into a load and a store at
  the top of the sweep, so this case covers one created earlier in
  the same sweep.
- An `INC`, `DEC`, `ASL`, `LSR`, `ROL`, or `ROR` of one of the
  compiler's `__local_*` slots. When the new value is dead, the pass
  drops it if the flags it sets are dead too. Those are N and Z for
  `INC` and `DEC`, and all flags for a shift or rotate, since those
  write C.

From the store, the pass follows every path forward. A path is done
when it reaches an instruction that writes exactly the same byte,
meaning the same name and offset or the same zero-page address. The
store is live if any path reaches an instruction that may read the
byte, using the aliasing rules of `passes/asm_aliasing.py`. An
operand addressed through a pointer also reads the two bytes of the
pointer, so a store into `DPTR` is live while a `(DPTR),Y` access
follows. A `Call`, a `FunctionPrologue`, or an `AllocateStack` makes
the store live, with two exceptions for a direct call (any callee
other than the `icall` trampoline). It doesn't read or write a byte of
this function's own `__local_<fn>__*` slots, as long as the function
never builds the slot's address with `ImmLabelLow`, `ImmLabelHigh`, or
a `LoadAddress`. The call-graph-disjoint allocator gives each function
slots that no callee uses, and such a slot can't be reached through a
pointer either, so an indirect access reads only the pointer's bytes
as far as that slot goes. And it ends the life of a store to `DPTR`:
DPTR is caller-saved scratch, which no callee reads on entry and no
pass expects to survive a call. Only `icall`'s `JMP (DPTR)` reads it.
So a loop that stages a pointer in `DPTR`, reads through `(ptr),Y`
directly, and calls a routine for each byte loses the staging.

A path that reaches a return makes the store live unless the byte is
dead at return. The bytes that are dead at return are zero-page
addresses `$80` to `$FF`, `DPTR`, every `__local_*` slot (the
function's own locals, in zero page or spilled above it), and slot
symbols whose address in the slot-symbol map lies in `$80` to `$FF`. A path that jumps to a
label outside the function, which is a tail call, makes the store
live unless the byte is a `__local_*` slot.

## What it leaves alone

- A volatile store, such as `port = a; port = a + 1;` with a
  `volatile uint8_t port`. Every store to a volatile object is a side
  effect under C99 §6.7.3.
- A store to a static with a call before the next store, as in `g =
  a; tick(); g = a + 1;`, because the callee may read `g`.
- The last store to a static, or to `HARGS`, `SSP`, or `FP`, before a
  return. Other functions can read a static, and `HARGS` carries a
  wide return value.
- A store to a callee's zp_abi argument slot before a tail-call
  `JMP`, since the callee reads it.
- A store into a `Frame`, `Stack`, or indirect operand, because an
  access through a pointer elsewhere might read it. In the unoptimized pipeline every local is on the
  soft stack, so there the pass drops only stores to statics and
  runtime cells.
- A store to the high byte of a pair that a later `LoadAddress`
  overwrites. The pass counts only the low byte of a `LoadAddress` as
  a kill, so that store stays.
- A dead `INC`, `DEC`, or shift whose flags a later branch reads.

## Related

[Dead store elimination](dead_store_elimination.md) is the TAC-level
pass. [Redundant store elimination](redundant_store.md) handles
memory-to-memory repeats in one block, and [dead A
arithmetic](dead_a_arith.md) drops the loads this pass leaves behind.
