# Renaming the slots made the missed optimization visible

A C99-to-6502 compiler I've been hacking on emits assembly that's, in
principle, dasm-readable. In practice the body locals were named
`__local_<fn>_b0`, `__local_<fn>_b1`, `__local_<fn>_b2`, ... — numeric
indices in pool order. The byte at `_b2` could be anything: a source
variable, an SSA temp, a coalesced merge of several values. Reading
the asm meant cross-referencing the index against a mental model of
the pool. I'd been doing it for months.

This week I renamed the slots to carry the source-level spelling:
`__local_<fn>__sprite_x`, `__local_<fn>__addr_lo`, etc., with numeric
fallbacks for compiler-only temps. Within an hour of reading the
first re-emitted asm I spotted a 16-byte, 120-cycle win that had
been hiding all along.

## The function

```c
__attribute__((zp_abi))
void special_inactive_draw(uint8_t special_row,
                           uint8_t special_pos_hi,
                           uint8_t page_flag)
{
    uint8_t sprite_x = proj_screen_col[special_pos_hi];
    draw_sprite(0x02, 0x06, sprite_x, special_row,
                special_peek_sprite, page_flag);
}
```

One table lookup, one call to `draw_sprite` with six args. `draw_sprite`
is also `zp_abi`, so its args sit in fixed ZP cells the caller writes
directly. The asm-SSA + byte-granular regalloc + per-function private
ZP pool combination is supposed to make calls like this collapse to
"compute each arg directly into its callee slot." No staging through
the soft stack.

Pre-rename, the call-marshal sequence around the pointer arg looked
like this — and I'd glance at it and move on:

```asm
   LDA   #<special_peek_sprite
   STA   __local_special_inactive_draw_b0
   LDA   #>special_peek_sprite
   STA   __local_special_inactive_draw_b0+1
   ...
   LDA   __local_special_inactive_draw_b0
   STA   __zpabi_draw_sprite_p4
   LDA   __local_special_inactive_draw_b1
   STA   __zpabi_draw_sprite_p5
```

Post-rename, same asm. Now legible:

```asm
   LDA   #<special_peek_sprite
   STA   __local_special_inactive_draw__0
   LDA   #>special_peek_sprite
   STA   __local_special_inactive_draw__0+1
   ...
   LDA   __local_special_inactive_draw__0
   STA   __zpabi_draw_sprite__tile_src_0
   LDA   __local_special_inactive_draw__1
   STA   __zpabi_draw_sprite__tile_src_1
```

Why am I staging a constant address through a ZP-pool slot? `special_peek_sprite`
is a link-time symbol. The four-line stage / four-line copy reads as
two redundant operations that, if the optimizer were doing its job,
should collapse to:

```asm
   LDA   #<special_peek_sprite
   STA   __zpabi_draw_sprite__tile_src_0
   LDA   #>special_peek_sprite
   STA   __zpabi_draw_sprite__tile_src_1
```

## Why it was hiding

The c6502 IR has a single compound atom `LoadAddress(src, dst)` for
"compute the address of `src` into the 2-byte `dst`." The asm emitter
expands it at print time into the four-line `LDA #< / STA / LDA #> / STA`
sequence. So at the IR level, before emit, the picture is:

```
LoadAddress(special_peek_sprite, P)    ; P is the staging temp
...
Mov(P[0], tile_src_0)
Mov(P[1], tile_src_1)
```

Three asm-level passes are supposed to collapse this:

- `mem_const_prop` tracks "after `STA M`, M holds the constant we just
  stored." A later `LDA M` would substitute the original constant
  back in. But `LoadAddress` is on the pass's *opaque atoms* list —
  it invalidates A and every tracked memory cell. The pass loses the
  fact that `P[0]` and `P[1]` hold known link-time bytes.
- Byte-granular SSA construction normally byte-versions every Pseudo
  so each byte gets its own copy-prop / DCE story. But `LoadAddress.dst`
  is in `_excluded_names` — the comment says "the instruction writes
  byte 1 implicitly; byte-versioning would leave the second write
  as an invisible side effect that regalloc could silently overlap
  with another SSA value." So the staging Pseudo `P` keeps its
  multi-byte coherence and stays opaque to the byte trackers.
- Forward copy-propagation operates on Pseudo-to-Pseudo Movs. The
  `LoadAddress → Mov(P[k], target_k)` pattern isn't a Mov chain —
  it's a write through a compound atom followed by a single-byte
  read. Copy-prop has nothing to chase.

Three nets, every one of them with `LoadAddress` cut out by a
specific rule, and each rule was correct in isolation. The pattern
slipped through the gaps because nothing knew to look for it.

## The fix: lower it earlier

The IR already had `ImmLabelLow(name, offset)` and `ImmLabelHigh(name, offset)`
operand variants. They've been there forever — the emitter uses
them internally when it expands `LoadAddress`. They just weren't
exposed to the optimizer.

The fix is small: when `tac_to_asm` sees `GetAddress(static_var, dst)`,
emit two single-byte atomic Movs instead of one compound LoadAddress:

```python
def _translate_get_address(self, operand, dst):
    if self._is_static_storage(operand.name):
        return [
            asm_ast.Mov(asm_ast.ImmLabelLow(name=operand.name, offset=0),
                        _byte_at(dst_op, 0)),
            asm_ast.Mov(asm_ast.ImmLabelHigh(name=operand.name, offset=0),
                        _byte_at(dst_op, 1)),
        ]
    # Frame-storage src — runtime FP+offset add, keep as LoadAddress.
    return [asm_ast.LoadAddress(...)]
```

Address-of an automatic variable still goes through `LoadAddress`
(the SSP+offset add has no compile-time analogue). But for
static-storage labels — the common case for array decay, address-of
file-scope vars, and so on — the two bytes become independent
single-byte writes the existing optimizer machinery understands.
Byte-granular SSA versions them. Copy-prop sees the chain. Byte-DCE
drops the dead intermediate. The four lines collapse to two, and
the staging ZP slot goes away with them.

There's a small follow-on. The optimizer now propagates
`ImmLabelLow/High` operands into more places — `Compare(Reg, ImmLabel)`,
`Add(ImmLabel, A)`, etc. — which the emitter and the in-process
assembler didn't previously encode. Two encoders needed extension
to accept the variant, both 2-byte instructions identical in shape
to their `Imm` counterparts.

## The numbers

`special_inactive_draw` optimized:

```
before: 557 bytes, 1846 cycles
after:  541 bytes, 1726 cycles    (-16 bytes, -120 cycles, -6.5%)
```

The intermediate ZP slot `__local_special_inactive_draw__0/1` is
gone — the function's private local pool shrank from 3 bytes to 1.
Eight asm instructions become four.

I don't think the win itself is the story, though. The story is
that I'd been looking at this output for months and never noticed,
because `__local_<fn>_b0` doesn't tell you what's in `_b0`. Naming
things in a way that surfaces intent is part of the optimizer.
What the compiler emits is what you read, and what you read is
what you can think about.

Are there other compound atoms in your IR that are opaque to your
per-byte trackers? `AllocateStack` / `Call` / function prologue /
epilogue all carry implicit side effects mine doesn't model
precisely. I'm starting to suspect the answer is "yes, several."
