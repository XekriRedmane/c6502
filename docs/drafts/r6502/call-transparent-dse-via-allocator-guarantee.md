# When JSR isn't really opaque: refining DSE with a call-graph guarantee

Quick optimization story from c6502 (the C99 compiler I'm writing
for the 6502).

A function called `entity_proximity` in one of my example programs
was emitting four dead stores right before a `JSR`:

```asm
entity_proximity:
   SUBROUTINE
   LDA   #<__local_entity_proximity__entity_row
   STA   __local_entity_proximity__0           ; dead
   LDA   #>__local_entity_proximity__entity_row
   STA   __local_entity_proximity__0+1         ; dead
   LDA   __zpabi_entity_proximity__hit_max
   STA   __zpabi_find_active_entity__hit_max
   LDA   #<__local_entity_proximity__entity_row
   STA   __zpabi_find_active_entity__out_row_0
   LDA   #>__local_entity_proximity__entity_row
   STA   __zpabi_find_active_entity__out_row_1
   JSR   find_active_entity
   ...
```

Lines 2 and 4 of that block store the address of a local
(`entity_row`) into `__local__0`. But `__local__0` gets overwritten
later in the function before anyone reads it. Pure dead code. The
optimizer's residue from rewriting an earlier "load from staged
local" into "load the immediate again."

## Why my dead-store elimination wouldn't kill it

`asm_dead_store` walks the CFG forward from each candidate STA
looking for a same-address overwrite before any read. If the walk
hits an opaque instruction (`Call`, `FunctionPrologue`,
`AllocateStack`), it bails and treats the store as LIVE — the
callee might read or overwrite this byte through who-knows-what
pointer.

That's the safe default for a JSR. But it's overkill here, and
"here" turns out to be a pattern with structural guarantees I can
exploit.

## The call-graph-disjoint allocator

In c6502, leaf-ish functions ("eligible" ones — no indirect calls,
not in a call-graph cycle, every direct callee also eligible or a
zp_abi extern) get a private slice of zero page for their locals.
The allocator hands each function a byte range disjoint from every
transitively-reachable callee's range. That's what makes
`__attribute__((zp_abi))` work without prologues/epilogues: caller
storage and callee storage simply don't overlap, so there's
nothing to save.

`__local_entity_proximity__0` (at `$8D`) is in
`entity_proximity`'s private slice. By construction, neither
`find_active_entity` nor anything it calls owns `$8D`.

So the only way `find_active_entity` could touch `$8D` is via a
pointer the caller handed it. The caller controls those pointers,
and the IR makes them visible:

```asm
LDA #<__local_entity_proximity__entity_row
STA __zpabi_find_active_entity__out_row_0
```

`ImmLabelLow(name)` / `ImmLabelHigh(name)` is the only way the
compiler builds a slot's address (the actual numeric ZP address
isn't known until link time, so an absolute `Imm(0x8D)` doesn't
appear in the IR). If I scan every `ImmLabelLow` / `ImmLabelHigh`
operand in the function and ask "is `__local__0`'s name in that
set?" — the answer's no. No pointer to `$8D` exists. The callee
cannot touch it.

## The refinement

In the DSE walker, when I hit `Call(name=<callee>)`:

1. If `name == "icall"` (indirect-call trampoline; target
   unknown): conservative, treat as opaque.
2. Otherwise, if the DSE target is `Data(__local_<curfn>__<x>, _)`
   AND `<x>` doesn't appear in the function's leaked-names set,
   the call cannot touch this byte. Continue walking the CFG past
   the JSR instead of bailing.

The leaked-names set is computed once per function as a single
forward pass scanning ImmLabel* operands. Cheap.

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

The DSE walk now sees the post-JSR overwrite that kills
`__local__0`, drops both dead STAs, and `entity_proximity` shrinks
to:

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

## Numbers

On the example that surfaced this (a 2-slot companion-walker for
a side-scroller):

- Before: 3524 bytes, 17654 cycles in the test harness; the
  function `companion_update` alone took 2405 cycles.
- After: 3504 bytes, 17404 cycles; `companion_update` down to
  2176 cycles (−9.5% just on that function).

The same pattern hit two other examples (one row removed each).
Modest win in bytes, but the DSE relaxation is the kind of thing
that compounds: every staging-through-a-temp that `apply_remat`
later optimizes leaves dead stores behind, and previously the
nearest JSR pinned them.

## Question for you

What's the most aggressive *sound* refinement you've made to a
"call is opaque" rule in your own peephole / dataflow passes? The
allocator-guarantee angle is nice because it makes the safety
argument structural rather than per-callee, but I bet there are
more axes (caller-saved-only zones, callee-signature-driven
read-set bounds, …) I'm leaving on the floor.
