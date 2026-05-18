# Putting address-taken locals in zero page on a 6502 C compiler

The 6502 is a 1MHz CPU with no hardware stack worth mentioning for
locals (256 bytes of page-1 you mostly want to leave for JSR return
addresses). My C compiler (c6502) uses a soft data stack at
`SSP`/`FP` for argument passing and locals. Soft-stack access is
indirect-indexed: every load is `LDY #off; LDA (FP),Y` — 4 cycles
plus a `Y` clobber. Functions with non-zero arg/local bytes also
need a prologue (`SSP -= N+M+2`, save caller FP) and epilogue.

That's expensive. The compiler tries hard to keep locals in ZP
instead — for functions on the call-graph-disjoint "private pool"
path, each eligible function gets a private range of ZP bytes,
guaranteed disjoint from every other function's coexisting code.
The body's regalloc colors directly into that pool.

But ONE class of local kept falling through to the soft stack:
**address-taken locals**. The classic shape:

```c
uint8_t entity_row;
if (!find_active_entity(&entity_row)) return;
if (entity_row != companion_row[slot]) return;
```

`&entity_row` is passed to `find_active_entity`, which writes
through the pointer. The local needs a stable, addressable
location — historically that forced a Frame slot, materializing
the soft-stack prologue/epilogue plus a 6-instruction `&local`
runtime compute and `LDY/(FP),Y` indirect read at every access.

In one example function (`entity_proximity`, three return paths)
that adds up to ~60 wasted instructions per call.

But the address-taken-ness doesn't fundamentally require Frame.
It requires "the local has a stable address the compiler can hand
out as a pointer." A ZP byte qualifies: `&entity_row` becomes
`#$8F` (the byte's ZP address) and `#$00` (high byte, since ZP
addresses are < 256). Two `LDA #imm; STA` pairs to load the
pointer, then `STA (ptr),Y` writes through it just like any other
address. The callee doesn't know or care that the pointer happens
to point into ZP.

## The fix

The asm-SSA regalloc excludes address-taken Pseudos from coloring
(they need stable identity, not SSA-renamed bytes). They reach
`replace_pseudoregisters` as unrenamed Pseudos that historically
got Frame slots.

The new pass `passes/address_taken_zp.py` runs after the final
regalloc. For each function on the private-pool path:

  1. Scan the IR for `LoadAddress.src = Pseudo`. Collect names.
  2. The regalloc colored some pool bytes (`coloring.assignments`).
     Expand each to include all bytes of multi-byte Pseudos
     (Pointer = 2 bytes, etc.). Subtract from the pool.
  3. For each address-taken local, find a contiguous run of free
     bytes of its size. Assign.
  4. Mint a slot symbol `__local_<fn>__<source_name>` and EQU it
     to the chosen ZP byte.

`replace_pseudoregisters` then routes the address-taken Pseudo to
`Data(slot_symbol, offset)` instead of `Frame(offset)`.
`asm_emit`'s LoadAddress lowering already handles
`src=Data(name)`: emits `LDA #<name; LDA #>name` as the address-
load immediate pair (dasm's `<` / `>` operators).

For the pool to have room for both colored bytes AND
address-taken bytes, the pool-size input passed to
`allocate_function_locals` is now `compute_local_bytes` (colored
demand from a preliminary pass) PLUS
`compute_address_taken_bytes` (new — sums the byte sizes of
candidates).

## The multi-byte trap

The first version passed all my tests, then the
`gold-output-snapshot` test caught a content mismatch even at the
same line count. Debugging it: the regalloc's `assignments` map
stored only the **first byte** of each colored Pseudo. A
`Pointer` Pseudo at `$82` actually occupies `$82` AND `$83` — but
naive `set(coloring.assignments.values())` only marked `$82` as
used. My address-taken allocator then put a 1-byte local at `$83`
— overlapping the pointer's high byte.

The fix: when computing the "used" set, expand each colored
Pseudo through its `size_of_name` width:

```python
used: set[int] = set()
for name, addr in coloring.assignments.items():
    size = size_of_name(name, symbols, types)
    for k in range(size):
        used.add(addr + k)
```

After this, the allocator correctly skips the multi-byte
continuation bytes.

## Size impact

On a real-world example from the game I'm porting
(`examples/companion_update.c`), the optimized output shrunk from
1132 to 986 lines — **13%** reduction, all from two
address-taken locals (`entity_proximity.entity_row` and
`companion_update.sprite_y`) being routed to ZP. Per-call cycle
savings on those functions are larger in relative terms because
the prologue/epilogue overhead vanishes entirely:
`entity_proximity` now emits as a bare body with three RTS
return paths, no SSP/FP setup at all.

Sim differential test: behavior byte-for-byte identical between
unopt and opt pipelines. Pure size win.

## Why this works only on private-pool functions

The private-pool model already guarantees that the function's ZP
range is disjoint from every coexisting function's storage (any
ancestor or descendant on the call stack). Putting an
address-taken local in that range inherits the same guarantee:
no callee can stomp on it during the call where the pointer is
live.

For functions OUTSIDE the private-pool path (recursive,
indirect-calling, or with non-zp_abi extern callees), the
fallback is unchanged — address-taken locals stay on the soft
stack. The pass simply doesn't run for those.

Repo: <https://github.com/XekriRedmane/c6502>
