# Adding `extern T name[];` to my C-to-6502 compiler — and the
# absolute,Y addressing fast path it unlocks

I've been writing a C99 compiler (c6502) that targets the MOS 6502 in
Python. Today I added support for the C99 incomplete-array form
`extern T name[];` — the shape used when one TU declares a lookup
table that another TU defines. Here's the change, and the 6502-side
codegen story that motivated it.

## The motivating example

A snippet I want to compile, lifted verbatim from a game-engine
sketch:

```c
#include <stdint.h>

extern uint8_t entity_active[20];
extern uint8_t rescue_dir[20];
extern uint8_t entity_floor_col[20];
extern uint8_t entity_floor_pos[20];

extern const uint8_t floor_thresh[];   /* per-floor row anchor */
extern uint8_t rescue_floor[20];       /* assigned floor index */

static void spawn_pos_dir(uint8_t slot)
{
    entity_active[slot]    = 0x01;
    rescue_dir[slot]       = 0x01;
    entity_floor_col[slot] = 0x3E;
    entity_floor_pos[slot] =
        (uint8_t)(floor_thresh[rescue_floor[slot]] - 0x07);
}
```

The sized arrays (`uint8_t name[20];`) were already accepted — they
carry an `Array(UChar, 20)` data type and the rest of the pipeline is
happy. The unsized form (`uint8_t floor_thresh[];`) was rejected at
parse time, which forced me to either pick a fake size or implement
the feature properly.

## The sentinel

I picked the second path. The change is small: the parser now maps
`[]` to `Array(elem, size=0)` — the "incomplete-array sentinel". The
size field was already a positive integer in every other context, so
0 carves out an unambiguous slot. The type checker enforces C99
§6.7.5.2's restriction that the sentinel is only legal at the
*outermost* type of an `extern` declaration: struct members, array
elements, sizeof targets, and non-extern objects all require a
complete type. That check rides on the existing
`require_complete=True` flag in `_check_well_formed_type`, so the
implementation is a four-line addition.

## Why size=0 doesn't break the IndexedLoad fast path

c6502 has an `IndexedLoad` recognizer that fires on subscript reads
into static-storage arrays. The eligibility gate is `_sizeof(arr) ≤
256` — i.e., "the byte offset always fits in Y's 0..255 range for any
in-bounds access." Sized arrays compute their byte size and check;
incomplete arrays evaluate `0 * sizeof(elem) = 0 ≤ 256`, which passes
trivially.

This is technically a "trust the user" contract: if a downstream TU
defines `floor_thresh` as a 300-byte array, `floor_thresh[260]` would
miscompile (Y wraps mod 256, the access lands at offset 4). For game
code where extern lookup tables live in zero page or are sized to fit
under 256 bytes, the contract is the same as the sized case — only
the size check moved from the compiler into the programmer's head.

## The codegen win

Here's the asm for the `floor_thresh[rescue_floor[slot]]` chained
subscript that `--optimize --unroll` produces:

```asm
LDY   #$03                ; slot is at FP+3
LDA   (FP),Y              ; A = slot
TAX                       ; X = slot
LDY   rescue_floor,X      ; Y = rescue_floor[slot]  (extern, sized [20])
LDA   floor_thresh,Y      ; A = floor_thresh[Y]     (extern, unsized [])
SEC
SBC   #$07
STA   __local_spawn_pos_dir_b2
```

Five instructions for the read chain — both subscript reads use
absolute,Y addressing on link-time symbols. Same shape regardless of
whether the array's size is known to the compiler.

## What's still on the floor

The store side is uglier. `entity_active[slot] = 0x01;` doesn't go
through a write-side absolute,Y fast path; instead it stages the
array's address into a DPTR pair and uses `(zp),Y` indirect-Y. The
`IndexedStore` recognizer in the TAC optimizer only fires on a
specific `ZeroExtend + Add(Constant, idx) + Store` pattern that
arises from const-static base folding (e.g. a `static T * const buf
= (T*)0x2000;` lvalue subscript), not from the c99→TAC lvalue-
subscript lowering. So writes through extern arrays look like

```asm
LDA   #<entity_active
STA   __local_spawn_pos_dir_b0
LDA   #>entity_active
STA   __local_spawn_pos_dir_b0+1
LDA   __local_spawn_pos_dir_b0
STA   DPTR
LDA   __local_spawn_pos_dir_b1
STA   DPTR+1
LDA   #$01
PHA
LDY   #$03
LDA   (FP),Y
TAY
PLA
STA   (__local_spawn_pos_dir_b0),Y
```

instead of a much tighter

```asm
LDY   #$03
LDA   (FP),Y
TAX
LDA   #$01
STA   entity_active,X
```

That's a one-screen optimizer change I haven't made yet — either
extend the IndexedStore recognizer to accept a `GetAddress(name)`
base alongside `Constant(C)`, or add a store-side mirror to the
c99→TAC subscript-lval lowering. Both are local and well-scoped, the
read side already shows the shape works.

## Discussion

Curious whether anyone else has hit this trust-the-user / sentinel
trick in their compilers. The size=0-as-incomplete approach felt
clean — no new ASDL variant, no parser-level "is this an extern?"
lookahead, just let the existing `Array` carry an out-of-band value
and have the type checker do the policing. But it does push the
"linker resolves the actual size" contract entirely onto the
programmer.

Is there a saner representation? An explicit
`IncompleteArray(element_type)` variant in the AST is the obvious
alternative — heavier (one more switch case in every Array-handling
site) but more honest. Curious how people doing similar small-target
compilers represent this.

If you're interested in the code: parser change is in `parser.py`
around `_array_size_from_suffix`; type-checker change is in
`passes/type_checking.py` in `_check_well_formed_type`. The full
emitted asm for the example is at `examples/spawn_pos_dir.asm`.
