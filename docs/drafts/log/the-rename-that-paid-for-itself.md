# The rename that paid for itself

Two changes shipped this session on c6502 (the C99-to-6502 compiler
I've been hacking on). Both were prompted by the same thing: I
finally got around to renaming the optimizer's internal slot
symbols so the emitted asm reads like asm and not like a regalloc
dump. The rename was supposed to be cosmetic. It ended up paying
for itself within an hour by exposing a 16-byte, 120-cycle missed
optimization that had been sitting in the output for months.

## Before: numbered slots

The optimizer assigns body-local Pseudos to ZP bytes per-function.
Each byte got a symbol of the form `__local_<fn>_b<N>` where N is
just the position in the function's private pool. For zp_abi
parameters: `__zpabi_<fn>_p<N>` where N is the flat byte index
across all params.

Concretely, the asm for a four-line C function ended up looking
like this:

```asm
__zpabi_special_inactive_draw_p0    EQU $80
__zpabi_special_inactive_draw_p1    EQU $81
__zpabi_special_inactive_draw_p2    EQU $82
__zpabi_draw_sprite_p0              EQU $83
__zpabi_draw_sprite_p1              EQU $84
__zpabi_draw_sprite_p2              EQU $85
__zpabi_draw_sprite_p3              EQU $86
__zpabi_draw_sprite_p4              EQU $87
__zpabi_draw_sprite_p5              EQU $88
__zpabi_draw_sprite_p6              EQU $89
__local_special_inactive_draw_b0    EQU $8A
__local_special_inactive_draw_b1    EQU $8B
__local_special_inactive_draw_b2    EQU $8C

   LDX   __zpabi_special_inactive_draw_p1
   LDA   proj_screen_col,X
   STA   __local_special_inactive_draw_b2
   ...
```

Symbolic in the sense that dasm can resolve the references. But
the names tell you *nothing*. Is `_p1` the `width` arg or the
`height`? Is `_b2` the local `sprite_x`, or an SSA temp, or a
coalesced merge of several values? You have to flip back to the
function's signature in the source, count parameter bytes, and
mentally simulate the regalloc to map names to identities.

I was OK reading this. I wrote it. After three months of working
on the optimizer I had the byte-index-to-source-name map memorized
for the half-dozen examples I'd been profiling. But every time I
asked Claude (the coding assistant I'm pairing with) to look at
the output, it had to redo the same map from scratch. And every
time I came back to an example after a few weeks, *I* had to redo
the map.

So I asked: how hard would it be to make the slot names carry the
source-level identifier? Looked like a one-day project. (Spoiler:
it was a one-day project plus a bonus optimization payoff.)

## The design

There are three families of slots and three naming policies:

**zp_abi parameter slots** — easy. The function signature lists
the parameter names; the regalloc gives each parameter N bytes
starting at a known ZP address. Direct mapping:

- 1-byte param `width`: `__zpabi_draw_sprite__width`
- 2-byte param `tile_src`: `__zpabi_draw_sprite__tile_src_0`,
  `__zpabi_draw_sprite__tile_src_1` (low byte then high byte)

Double underscore between function and param name so things like
`draw_sprite_width` (a function whose name happens to end in
`_width`) don't get confused with `draw_sprite`'s param `width`.

**Body-local slots with source identity** — harder. After the
asm-SSA roundtrip, byte-versioning, and coalescing, the regalloc
assigns each colored Pseudo to a ZP byte. The Pseudo name is the
result of layering renames: identifier resolution turns C's
`sprite_x` into `@5.sprite_x`; TAC-level SSA turns it into
`@5.sprite_x.3` (per-def versioning); asm-level SSA wraps that as
`@5.sprite_x.3.b0.v1` (byte index + version). To recover the
original C spelling I had to strip in reverse: `.v1` (asm-SSA
version), then `.b0` (byte index), then `.3` (TAC-SSA version),
then the `@5.` prefix.

For a Pseudo that parses cleanly back to a source name, the slot
becomes `__local_<fn>__<source>[_<byte>]`. Byte suffix only when
the source variable was wider than 1 byte.

**Body-local slots with no source identity** — compiler-only
temps that the TAC translator minted as `%5` or similar. These
don't trace to a source-level name; they were intermediate
values produced by lowering a complex expression. Numbered
sequentially: `__local_<fn>__0`, `__local_<fn>__1`, ...

I asked the user (me, in this case, via Claude's AskUserQuestion
mechanism — yes, the LLM was driving the design conversation; I
was answering) about the multi-byte naming convention (`_0/_1`
vs `_lo/_hi`) and the coalescing policy ("first source name
wins" vs "fall back to temp on collision"). User picked
numeric byte suffixes and first-wins.

## The collision bug

The first cut of the naming algorithm parsed each Pseudo and
emitted the source-derived name. It immediately broke a chapter_8
test:

```c
int main(void) {
    int i = 100;
    int count = 0;
    while (i--) count++;
    if (count != 100) return 0;
    ...
}
```

After renaming, the test returned 0 instead of 1 — and the cycle
count was 81 instead of ~40000 (i.e. the loop didn't run). The
emitted asm initialized `i` to 0 instead of 100.

Took me longer than it should have to isolate. I read the asm,
formed theories about regalloc bugs, formed theories about my
SSA-name parser, formed theories about ordering of coalescing
projection. Then I `git stash`-ed the changes, ran the test on
main: it passed. Of course. The regression was mine. Compare the
two compiles of the same source.

The diff was immediate. The OLD asm initialized six ZP bytes
(b0..b5); the NEW asm initialized only four. Two stores were
*missing*.

The cause: `count` had two TAC-SSA versions (`count.2` from the
first loop, `count.5` from the second loop's re-init) and the
regalloc had assigned them to different ZP byte pairs because
their live ranges overlapped through the comparison. My naming
algorithm parsed both as `(source=count, byte=0 or 1)` and
emitted the same symbol — `__local_main__count_0`,
`__local_main__count_1` — for two distinct addresses. The asm IR
references the duplicate-named symbols at the duplicate addresses,
and `redundant_store` (a peephole that drops writes to a cell
that's already going to be overwritten) saw two consecutive
stores to "the same slot" and dropped one.

The peephole was correct, given what it could see. The IR violated
its precondition (operand names being 1:1 with storage cells).

The fix is mechanical: track the set of names already emitted,
and fall back to a numeric temp on collision. The second `count`
slot becomes `__local_main__0` instead of duplicating
`__local_main__count_0`. The asm reads slightly less helpfully
("which one is the real `count`?" — they both are, just at
different lifetime windows) but every name binds to exactly one
address, and downstream peepholes are sound again.

This is a project-level invariant I hadn't articulated before:
*the symbol you write into the asm must uniquely identify the
storage cell.* Naming by source identity is fine as long as the
identifier function is injective on storage addresses.

## The linker

There's also a multi-TU linker (`compile.py --link`) that takes
per-TU `.asm` outputs and re-allocates ZP addresses globally. It
reads a metadata block at the top of each input:

```
; @zp-link-meta-begin
; def step_pos param_bytes=2 local_bytes=0 indirect=false in_cycle=false
; @zp-link-meta-end
```

The OLD linker minted slot symbols from the metadata: it sees
`param_bytes=2` and synthesizes `__zpabi_step_pos_p0`,
`__zpabi_step_pos_p1`. That worked because the names were
deterministic functions of the byte count. The NEW naming depends
on source identifiers the linker has no way to recompute — the
asm-SSA colorings aren't in the metadata block.

Solution: embed the slot symbols themselves in the metadata. The
format goes from

```
; def step_pos param_bytes=2 local_bytes=0 ...
```

to

```
; def step_pos params=__zpabi_step_pos__slot_0,__zpabi_step_pos__slot_1 locals= ...
```

The linker reuses the strings verbatim. Per-TU outputs already
have the names baked into their bodies; the linker just re-binds
the EQU addresses.

## The bonus payoff

After the rename landed and all 2574 tests went green, I
recompiled `examples/special_inactive_draw.c` — the four-line C
function I'd been using as a test bed — and read the asm. Now
the body looked like this (one section, around the call to
`draw_sprite`):

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

And I stared at it for about ten seconds. Why am I staging a
known compile-time address through a ZP-pool slot? The address
of `special_peek_sprite` is a link-time constant. The four
instructions writing it to `__local_..._0` and `__local_..._1`,
then the four instructions reading those back and writing them
to `__zpabi_draw_sprite__tile_src_0/1`, should collapse to two
direct writes:

```asm
   LDA   #<special_peek_sprite
   STA   __zpabi_draw_sprite__tile_src_0
   LDA   #>special_peek_sprite
   STA   __zpabi_draw_sprite__tile_src_1
```

Pre-rename, the same asm — `STA __local_..._b0`, `LDA __local_..._b0`,
etc. — never triggered the same question, because I'd internalized
that body-local slots were just numeric scratch. Post-rename, the
slot has a name (`__0`) that says "no source identity, I'm a
compiler temp," and a compiler temp in the middle of a 2-byte
copy of a link-time constant raised an obvious red flag.

I traced through why the optimizer wasn't catching this:

- The TAC `GetAddress(static_var, dst)` lowers in `tac_to_asm` to
  a single `LoadAddress(Pseudo(static_var), Pseudo(dst))` atom.
  At emit time it expands to the four-line `LDA #< / STA / LDA #>
  / STA` sequence.
- Three asm-level passes treat `LoadAddress` as opaque:
  `mem_const_prop` lists it in "atoms that invalidate every
  tracked memory cell"; `ssa_construction._excluded_names`
  excludes its dst from byte-granular versioning because the
  byte-1 write is implicit; forward copy-prop only chases
  Pseudo-to-Pseudo Mov chains, not writes-through-compound-atoms.
- The IR already has `ImmLabelLow(name, offset)` and
  `ImmLabelHigh(name, offset)` operand variants — they're what
  the asm emitter uses internally when it expands `LoadAddress`.

The fix: lower static-storage `LoadAddress` at `tac_to_asm` time
to two atomic Movs with `ImmLabelLow/High` srcs. The compound
atom only survives for Frame-storage srcs (address-of an
automatic variable, where the runtime adds SSP+offset).

```python
def _translate_get_address(self, operand, dst):
    if self._is_static_storage(operand.name):
        return [
            Mov(ImmLabelLow(name=operand.name, offset=0),
                Pseudo(dst.name, offset=0)),
            Mov(ImmLabelHigh(name=operand.name, offset=0),
                Pseudo(dst.name, offset=1)),
        ]
    return [LoadAddress(...)]  # Frame case unchanged
```

Twenty lines of code including the static-storage check helper.
The asm-SSA byte trackers see the two atomic Movs as ordinary
defs, byte-version them, propagate the `ImmLabel*` operands
through to the eventual consumers (the `Mov(P[k], tile_src_k)`
copies), and `byte_dce` drops the now-dead staging local.

Small follow-on: two encoders — `Compare(Reg, ImmLabel*)` and
`Add/Sub/And/Or/Xor(ImmLabel*, A)` — didn't previously accept
the operand variant because the optimizer had never propagated
ImmLabel into those slots. Each needed two lines added: same
encoding shape as the `Imm` case, just resolving the label at
assembly time instead of taking the byte from a literal.
`const_static_fold` also needed updating: its "address-taken"
disqualification scan walks `LoadAddress.src` for candidate
names; with static-LoadAddress lowered, the address-taken
signal now lives in `ImmLabelLow/High` operands, and the scan
had to follow.

## Numbers

`special_inactive_draw` optimized, before / after:

```
unopt: 1811 bytes, 10755 cycles  (unchanged)
opt:    557 →  541 bytes  (-16 bytes)
       1846 → 1726 cycles  (-120 cycles, -6.5%)
```

The function's private ZP pool dropped from 3 bytes to 1 — the
old `__local_..._0/1` pair holding the staged address is gone.

I'm sure the same pattern fires elsewhere in the chapter corpus
— anywhere a static-array decayed pointer is passed to a zp_abi
callee, and anywhere `&static_var` is computed into a local
that's read once and written through. Haven't audited yet.

## The meta-point

The rename was supposed to be cosmetic. It's the kind of change
that's hard to justify in isolation — "the compiler still
produces correct code, the asm still assembles, what's the
point?" — and easy to defer. I deferred it for months.

The cost of the deferral was that I'd been reading my own
output through a lens of decoded indices and missing patterns
that would be obvious if the lens were gone. Tooling that
makes your output legible isn't a vanity feature. It's part of
the optimizer because *what you can read is what you can think
about.*

This isn't a new observation — it's the entire reason we have
syntax highlighting, indentation, autoformatters. The c6502
output going from `_b0/_b1/_b2` to `__sprite_x/__addr_0/__0`
is the same idea applied to a smaller, narrower form: the
named cell is the labeled axis on the chart. Once you can see
it, you can ask whether it should be there.

I'm going to look at the rest of the example corpus this week
and see what else I haven't been seeing.
