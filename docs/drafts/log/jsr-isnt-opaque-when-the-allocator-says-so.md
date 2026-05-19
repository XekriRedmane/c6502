# JSR isn't opaque when the allocator says so

A short optimization story from c6502. The fix is small. The
reasoning chain behind it took me longer to write down than to
implement, and that's the part I want to capture.

## The symptom

I compile my example programs to 6502 assembly with full
optimization, then run a sim differential to make sure the
optimized and unoptimized outputs match observable state. The
`companion_update` example — a per-frame tick + draw routine for
a two-slot "walker" entity in a side-scroller — was clean and
shrinking nicely after each round of optimizer work. But one of
its callees, `entity_proximity`, kept emitting four dead stores at
the top:

```asm
entity_proximity:
   SUBROUTINE
   LDA   #<__local_entity_proximity__entity_row
   STA   __local_entity_proximity__0           ; ← here
   LDA   #>__local_entity_proximity__entity_row
   STA   __local_entity_proximity__0+1         ; ← here
   LDA   __zpabi_entity_proximity__hit_max
   STA   __zpabi_find_active_entity__hit_max
   LDA   #<__local_entity_proximity__entity_row
   STA   __zpabi_find_active_entity__out_row_0
   LDA   #>__local_entity_proximity__entity_row
   STA   __zpabi_find_active_entity__out_row_1
   JSR   find_active_entity
   ...
```

`__local_entity_proximity__0` is a 2-byte compiler-temp slot in
zero page (`$8D`/`$8E`). The two stores to it are dead: a few
basic blocks down the function, `__local__0` gets overwritten with
something completely unrelated (`companion_row[X]`) before anyone
reads it. The C source has no `entity_row` value that should
survive past the call. So why is the compiler bothering?

## Why those stores existed in the first place

The C looks like:

```c
uint8_t entity_row;
if (!find_active_entity(hit_max, &entity_row)) return;
if (entity_row != companion_row[slot]) return;
...
```

Translated naively, that produces, in the asm IR:

```
LoadAddress(entity_row, %temp)         ; %temp ← &entity_row
Mov(%temp_lo, __zpabi_callee__out_row_0)
Mov(%temp_hi, __zpabi_callee__out_row_1)
Mov(hit_max, __zpabi_callee__hit_max)
Call find_active_entity
```

A subsequent pass (`lower_data_load_address`) splits the
`LoadAddress(Data, _)` atom into two `Mov(ImmLabel*, _)` atoms so
later byte-granular passes can see the byte writes individually.
After byte-granular register allocation, `%temp` lands on
`__local__0` (a slot in the function's private pool).

Then `apply_remat`, a small peephole that I wrote a few sessions
ago, notices:

> "You stage `<recomputable_src>` through `__local__0`, then later
> reload from `__local__0` to write somewhere else. The
> reload-from-zp is 2 bytes / 3 cycles. The original
> `<recomputable_src>` is `ImmLabelLow(entity_row)` — also 2
> bytes / 2 cycles. Just rewrite the use site to recompute."

That's what produces the duplicated `LDA #<entity_row` you see at
lines 3 and 9 of the dump. `apply_remat`'s docstring says, in so
many words: "the original `STA __local__0` is now dead; DSE will
clean it up on the next fixedpoint iteration." That's the
expectation it ships with.

It just wasn't true for this case.

## Why DSE bailed

`apply_asm_dead_store` walks the CFG forward from each candidate
STA looking for a same-address overwrite before any read. If
every path forward either re-overwrites the byte or terminates at
function exit with the byte dead-at-exit, the STA is dead and
drops.

The walk has an "opaque instructions" list: instructions that the
walker assumes may read or write any memory at all. `Call` is on
that list. The reasoning is straightforward — the callee might
read this byte through a pointer; the callee might overwrite it.
Without more information, the safe answer is LIVE.

Walking from `STA __local_entity_proximity__0`, the next few
instructions don't touch `__local__0`. Then comes `JSR
find_active_entity` — opaque — and the walker bails. LIVE.

## What the allocator guarantees

c6502 has a calling-convention optimization called `zp_abi`:
functions whose direct callees are all "eligible" (no indirect
calls, not in a call-graph cycle, no non-zp_abi extern callees)
get their parameters AND body locals placed in zero page, with
the allocator giving each function a private byte range disjoint
from every transitively-reachable callee's range. The whole
optimization rides on that disjointness: caller storage and
callee storage simply don't overlap, so there's nothing to save
across the call.

`__local_entity_proximity__0` is in `entity_proximity`'s private
slice. `find_active_entity` is one of its callees, so by
construction `find_active_entity` and everything `find_active_entity`
transitively calls cannot directly read or write that byte.

The only way `find_active_entity` (or its transitive callees)
could touch `__local__0` is if the caller leaked the byte's
address somehow — handed it across as a pointer parameter.

## Catching the pointer leak

Address construction in the asm IR has exactly one shape:
`ImmLabelLow(<slot_name>)` and `ImmLabelHigh(<slot_name>)`. The
numeric address of a slot isn't known until link time, so the
compiler always builds addresses symbolically and lets the
assembler resolve them. There's no path where a literal
`Imm(0x8D)` ends up in the IR referring to `__local__0`.

That makes the leak check trivial: scan the function's instruction
list for every `ImmLabelLow` / `ImmLabelHigh` operand, collect
their `name` fields into a set. Any `__local_<curfn>__*` slot
whose name is NOT in the set has never had its address constructed
in this function, so no callee can have received a pointer to it.

That gave me the safety argument I needed. A direct (non-icall)
JSR is transparent for any DSE target byte that's both:

1. In the current function's `__local_<curfn>__*` private pool
   (prefix check on the slot symbol name), AND
2. Not in the leaked-names set (`ImmLabel*` scan).

## The patch

```python
def _call_cannot_touch(target, local_prefix, address_taken_names):
    if not isinstance(target, asm_ast.Data):
        return False
    if not target.name.startswith(local_prefix):
        return False
    if target.name in address_taken_names:
        return False
    return True
```

In the DSE walker, when about to bail on an opaque instruction, I
check whether it's specifically a non-icall `Call` and whether
`_call_cannot_touch` returns True. If both, I continue past it
instead of returning LIVE:

```python
if (
    isinstance(nxt, asm_ast.Call)
    and nxt.name != "icall"
    and local_prefix is not None
    and address_taken_names is not None
    and _call_cannot_touch(target, local_prefix, address_taken_names)
):
    stack.extend(_successors(instrs, j, label_to_index))
    continue
if isinstance(nxt, _OPAQUE_TYPES):
    return False
```

`icall` (the runtime trampoline for indirect calls) is excluded
because its eventual target is unknown — the allocator can't
reason about a function pointer's destination. (Functions
containing `IndirectCall` aren't eligible for the private-pool
treatment in the first place, so by construction a function whose
DSE candidate is `__local_<fn>__*` doesn't contain `icall` calls,
but pinning that with the explicit check costs nothing.)

## Tests first

Before touching the implementation, I wrote six unit tests
covering the surface area:

- Compiler-temp slot, JSR sandwiched between dead STA and kill —
  should drop after fix.
- Same shape but the slot's address IS constructed elsewhere
  (address-taken) — should stay live.
- Source-named-but-not-address-taken slot — same as
  compiler-temp; should drop.
- `__zpabi_callee__*` slot — never in our local prefix; should
  stay live.
- A slot belonging to a *different* function's private pool —
  outside our local prefix; should stay live.
- `icall` between STA and kill — even for our own pool, should
  stay live (target unknown).

Two of the six failed before the fix. All passed after. The
"address-taken stays live" and "other-function's pool stays live"
tests are the ones I care most about — they encode the soundness
boundary.

## The result

`entity_proximity` post-fix:

```asm
entity_proximity:
   SUBROUTINE
   LDA   __zpabi_entity_proximity__hit_max
   STA   __zpabi_find_active_entity__hit_max
   LDA   #<__local_entity_proximity__entity_row
   STA   __zpabi_find_active_entity__out_row_0
   LDA   #>__local_entity_proximity__entity_row
   STA   __zpabi_find_active_entity__out_row_1
   JSR   find_active_entity
   ...
```

The two dead STA pairs (4 instructions, 8 bytes, 12 cycles) are
gone. Same pattern in `companion_update` itself, around a call to
`drift_step`, also fixed.

Whole-example numbers (a sim harness that batters the function
under a variety of scenarios):

- Before: 3524 bytes assembled, 17654 cycles total in the
  harness, 2405 of those in `companion_update` itself.
- After: 3504 bytes, 17404 cycles total, 2176 in
  `companion_update`. The function-under-test slice is the one
  that matters — the rest is test instrumentation, constant
  across runs. So: −9.5% on the optimization-relevant
  denominator.

Modest absolute. But this is a wedge: every staging-through-temp
shape that `apply_remat` collapses leaves dead stores behind, and
previously the nearest JSR pinned them. Now they vanish on the
next fixedpoint iteration.

## What I'm taking away

Two things worth holding onto.

First, "opaque" rules in dataflow / peephole passes are often
worth a second look once your compiler has structural guarantees
the rules don't know about. The call-graph-disjoint allocator
gives me a much stronger invariant than "Call may touch any
memory" — it gives me "Call cannot touch *these specific* bytes
unless I leaked their address." The right move was to let the DSE
walker exploit that, not to weaken the allocator's invariant or
restructure the IR.

Second, when an optimizer pass like `apply_remat` ships with a
docstring saying "downstream pass X will clean up the residue,"
it's worth occasionally checking whether X actually does. The
coupling isn't obvious in the code — `apply_remat` doesn't call
DSE, doesn't even mention DSE by name in its module signature —
and so a regression that prevents X from cleaning up doesn't
surface as a test failure in either pass's unit tests. It surfaces
as code that looks slightly worse than it should, in an example
nobody's looking at carefully.

I caught this one because I was looking carefully. The next one
I'll catch the same way, or not at all. Worth thinking about how
to make these coupling assumptions more checkable.
