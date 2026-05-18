# Address-taken without frames: another oracle-driven optimizer pass

Continuing the "compare to hand-written 6502 and close the gap"
methodology from the last few posts. Today's gap: how c6502 handled
address-taken local variables, and why the soft-stack overhead
that resulted was avoidable on the private-pool path.

## The function

```c
__attribute__((zp_abi))
static void entity_proximity(uint8_t slot, uint8_t screen_x,
                             uint8_t hit_max)
{
    uint8_t entity_row;
    if (!find_active_entity(hit_max, &entity_row)) return;
    if (entity_row != companion_row[slot]) return;
    /* ... three more branches ... */
}
```

`find_active_entity` scans the hit-entity table and, on success,
writes the row through the `out_row` pointer. The caller takes the
address of a local `entity_row`, passes it in, and reads
`entity_row` after the call.

This is one of the most common C idioms for "find or return false."
Predates `std::optional` by half a century.

## The cost in c6502's output

Pre-optimization output for `entity_proximity` (selected lines):

```asm
entity_proximity:
   SUBROUTINE
   ; prologue: 0 arg bytes, 1 local bytes
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$02
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
.body:
   CLC
   LDA   FP
   ADC   #$01
   STA   __local_entity_proximity__0
   LDA   FP+1
   ADC   #$00
   STA   __local_entity_proximity__0+1
   ; → __local_0 now holds &entity_row, a 2-byte pointer
   LDA   __zpabi_entity_proximity__hit_max
   STA   __zpabi_find_active_entity__hit_max
   LDA   __local_entity_proximity__0
   STA   __zpabi_find_active_entity__out_row_0
   LDA   __local_entity_proximity__1
   STA   __zpabi_find_active_entity__out_row_1
   JSR   find_active_entity
   ...
.read_entity_row:
   LDY   #$01           ; offset within frame
   LDA   (FP),Y         ; entity_row at FP+1
   ; ... three RTS paths, each running the full epilogue ...
```

Decomposing the overhead:

- **Prologue (16 instructions / ~40 cycles)**: subtract 3 from
  SSP, save caller FP into the new frame, capture SSP into FP.
- **`&entity_row` runtime compute (6 instructions / ~20 cycles)**:
  CLC; LDA FP; ADC #1; STA dst_lo; LDA FP+1; ADC #0; STA dst_hi.
  Because FP isn't constant at compile time, the address has to
  be computed every time the function runs.
- **Indirect-Y read of `entity_row` (2 instructions / ~7 cycles)**:
  LDY #1; LDA (FP),Y. Every access goes through Y; Y can't hold
  anything else across the read.
- **Epilogue (~14 instructions per return path × 3 paths = 42
  instructions)**: restore caller FP, rewind SSP, RTS.

Total overhead per call: 16 + 6 + (~2 per access) + 42 = around
70 instructions. The body's actual logic is maybe 30
instructions. So roughly 70% of the function's footprint is
soft-stack overhead.

## What's actually required

`&entity_row` needs a stable address the compiler can hand out as
a pointer. The callee uses indirect addressing (`LDY #0; STA
(DPTR),Y`) to write through it; it doesn't care what kind of
address it is. ZP, page-1, page-2, anywhere in the 64K — same
opcodes, same cycles (the indirect-Y opcode handles them all).

So a ZP byte qualifies as the storage. If `entity_row` lives at,
say, `$8F`:

- `&entity_row` is the constant `$008F`. Two `LDA #imm; STA`
  pairs to load the pointer into a callee's slots. No FP+offset
  compute, no `CLC`.
- Reading `entity_row` is `LDA $8F` (or via the EQU'd symbol). 3
  cycles, no Y clobber.
- The function needs no Frame slot for it, so the prologue and
  epilogue collapse to nothing — bare body + RTS.

The only constraint: the ZP byte has to stay alive across the
call where the pointer is dereferenced. The function isn't
allowed to share that byte with a callee that might write to it.

## The private-pool guarantee

c6502 already has a call-graph-disjoint local-pool allocator. For
every function on a viable subset of the call graph (no
recursion, no indirect calls, no non-zp_abi extern callees), it
allocates a private ZP range that's guaranteed disjoint from
every ancestor's range AND every descendant's range. Bytes in
that range are "safe across calls within this clique."

This is exactly the property we need for address-taken locals.
If `entity_proximity` is on the private-pool path (it is), and
`find_active_entity` is too (also yes — it's a leaf), then any
byte in `entity_proximity`'s pool is safe to use as
`entity_row`'s storage during the JSR — `find_active_entity`'s
own pool is disjoint by construction.

So the optimization is: **for each address-taken local in a
private-pool function, find a free byte in that function's
pool**, route the local there, and let the rest of the pipeline
handle the bookkeeping.

## Implementation

A new pass, `passes/address_taken_zp.py`, runs after the asm
regalloc. Per function:

  1. Scan the IR for `LoadAddress.src = Pseudo`. Collect the names.
  2. The regalloc colored some pool bytes via
     `coloring.assignments`. Expand each entry to include all bytes
     of multi-byte Pseudos (a Pointer at `$82` occupies `$82`
     AND `$83`). Subtract from the pool to get the "free" set.
  3. For each address-taken local, find a contiguous run of free
     bytes of its size. Assign the local to the first byte. Mint
     a slot symbol `__local_<fn>__<source_name>` and bind it via
     EQU.

`replace_pseudoregisters_bare_exit` then routes the Pseudo to
`Data(slot_symbol, offset)` (instead of `Frame(offset)`).
`asm_emit`'s LoadAddress-on-Data path already handles this case:
it emits `LDA #<name; STA dst.lo; LDA #>name; STA dst.hi` — a
2-byte immediate pair, no runtime compute.

The pool-sizing step also gets a small tweak:
`compute_address_taken_bytes` sums each function's address-taken
byte demand, and that's added to the colored-byte count before
the pool allocator runs. So the pool always has room for the
address-taken bytes.

## The multi-byte trap

First-pass implementation passed unit tests, the sim
differential, and most of the chapter corpus. Then the
gold-output snapshot test caught a problem: same line count, but
the content differed. Inspecting the output revealed two slot
symbols at the same address:

```
__local_caller__1   EQU  $83
__local_caller__x   EQU  $83   ; ← collision!
```

`__local_caller__1` was a pointer's high byte. `__local_caller__x`
was the address-taken local I'd allocated. Both at `$83`. The
regalloc had stored only the pointer's first-byte address (`$82`)
in `coloring.assignments`; my "used" set therefore missed `$83`.

The fix:

```python
used: set[int] = set()
for name, addr in coloring.assignments.items():
    size = size_of_name(name, symbols, types)
    for k in range(size):
        used.add(addr + k)
```

A two-line change. Multi-byte values now correctly mark all their
bytes as occupied, and the address-taken allocator finds the
genuinely-free addresses past the end of the pointer.

The gold-output snapshot test was the only signal that this was
wrong, because the **sim test still passed**. The compiled
program happened to compute the right answer in this particular
test program — the pointer's high byte got written before the
address-taken local was read, so the overlap didn't manifest.
That's the kind of bug that the differential test alone wouldn't
catch but the snapshot test would, if you're paying attention to
the diff.

Lesson learned: sim differential tests verify semantics; gold-
output snapshot tests verify generated code shape. Both matter,
and they catch different classes of bugs. A snapshot test that's
allowed to drift "because the output got shorter" loses its
ability to detect aliasing bugs that happen not to manifest in
the test inputs.

## Results

`examples/companion_update.asm` — the per-frame tick for the
two-slot companion-walker in the game I'm porting — shrunk from
1132 to 986 lines. 13% reduction. Two address-taken locals got
routed to ZP:

  - `__local_entity_proximity__entity_row`: the case I started
    with. `entity_proximity` now emits as bare body + 3 RTS.
    No prologue, no epilogue, no `&entity_row` runtime compute,
    no `(FP),Y` reads.

  - `__local_companion_update__sprite_y`: a uchar local that
    `companion_update`'s drift-path takes the address of when
    calling `drift_step(&sprite_y)`. Same shape, same fix.
    `companion_update`'s prologue/epilogue overhead drops too,
    though it had several other locals on the soft stack so
    the function as a whole still has a frame.

Sim differential test: opt vs unopt byte-for-byte identical
across 8 scenarios.

## Reflecting on the methodology

This is the third change in a row driven by the
hand-written-vs-compiler-output diff. Each followed roughly the
same shape:

  1. Notice a specific opcode sequence where c6502 emits
     N instructions and the hand-written equivalent emits M
     < N.
  2. Trace what passes contributed to the N-instruction
     sequence. Read the IR at each stage.
  3. Find the pass whose decision is responsible for the gap.
  4. Either tighten an existing pass's eligibility or add a
     new pass that runs after it.
  5. Run the sim differential and the snapshot tests; iterate
     until both pass.

What I notice: the optimizer architecture is composable in a
way that lets each gap be filled by adding ~100-200 LOC of
focused work, not by restructuring. The asm-SSA pipeline +
peephole catalog + post-coloring fixup passes have enough hooks
that "do exactly this rewrite at exactly this point" usually
maps onto a single new pass.

The hand-written reference keeps producing concrete targets.
I'm not running out of gaps to close.

Repo: <https://github.com/XekriRedmane/c6502>
