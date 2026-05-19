# Generalizing the fold

A couple of weeks ago I shipped a small TAC-level optimization in
c6502: when the compiler had emitted

```
Unary(LogicalNot, x, %t)
JumpIfFalse(%t, T)
```

— that is, "compute `!x` into a temp, then branch on the temp" —
the fold rewrote it to

```
JumpIfTrue(x, T)
```

with the sense flipped to absorb the LogicalNot's semantic
inversion. The win was clean: the asm-level lowering of
LogicalNot is a `LDA #0` / `LDA #1` materialize sequence, and the
JumpIf consumer then re-tests it with `ORA #$00; BEQ`. By the time
the asm optimizer sees that pattern there are labels in the
middle of it and the rewrite is expensive in label fix-up cost;
doing it at the TAC level (one fold-eligible pair, one
sense-flipped JumpIf in its place) is much cheaper. I wrote that
up at the time. The retrospective is log/012 in this repo, the
public-facing version is r6502/014.

Yesterday I was reading another example file's output and noticed
the same shape on the `&&` and `||` lowerings:

```
   LDA   __zpabi_entity_proximity__screen_x
   CMP   #$40
   BCC   .and_false@3
   CMP   #$47
   BCS   .and_false@3
   LDA   #$01
   STA   __local_entity_proximity__1
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   STA   __local_entity_proximity__1
.and_end@4:
   LDA   __local_entity_proximity__1
   BEQ   .if_end@5
```

The two `BCC`/`BCS` are the chain. Everything between
`.and_false@3` and `.if_end@5` is a materialize-and-re-test. The
chain ALREADY controls flow exactly the same way the consumer's
`BEQ` will — every short-circuit branch in the chain means "the
overall `&&` is false", which is what the consumer is testing
for. There's no reason to round-trip through a Boolean.

So I sat down to generalize the LogicalNot fold to handle this.

## What the chain actually looks like in TAC

The c6502 short-circuit lowering (in `c99_to_tac.translate_short_
circuit`) emits the same 5-instruction "tail" after every `&&` or
`||` chain:

```
Copy(C_ft, %t)         # fallthrough value
Jump(end_label)
Label(branch_label)
Copy(C_sc, %t)         # short-circuit value
Label(end_label)
```

where `(C_ft, C_sc)` is `(1, 0)` for `&&` (any operand false →
overall is 0) and `(0, 1)` for `||` (any operand true → overall is
1). The chain of conditional jumps preceding this tail all
target `branch_label`. The consumer is the immediately-following
`JumpIf{True,False}(%t, T)`.

When you draw out the four `(C_ft, C_sc) × consumer_kind`
combinations and figure out where flow actually goes, two things
become clear. First, the dst of the fold is *always* one of:

  - T (the consumer's target), or
  - "next" (the instruction immediately after the consumer in the
    function).

There are only two destinations because the consumer is a single
conditional jump. Second, two distinct sub-cases drop out:

  - The **natural** sub-case is the one where the chain's "fired"
    destination (D_sc) equals T. In that case the rewrite is a
    plain retarget of every short-circuit chain jump to T,
    followed by deleting the 6-instruction tail+consumer. Nothing
    new emitted.
  - The **flipped** sub-case is the one where the chain's "fired"
    destination equals "next" — i.e. flow should fall PAST the
    consumer when a chain jump fires. There's no label for "next"
    in TAC, so the rewrite mints a fresh
    `.<funcname>@scfold@<N>`, retargets the chain to it, and
    replaces the tail+consumer with `Jump(T); Label(.scfold@N)` —
    the `Jump(T)` routes the *fall-through* (no chain fired) path
    to T, and the synthetic label catches the chain jumps so flow
    naturally falls through to whatever was after the original
    consumer.

`&& consumed by JumpIfFalse` is natural; `|| consumed by
JumpIfTrue` is natural. The other two combinations are flipped.
`c99_to_tac` produces a mix in practice, depending on whether the
surrounding `if` directly tests the result or whether there's a
negation in the middle.

## The wrong turn

I almost overcomplicated the pattern matcher. My first cut
identified the chain by scanning *backwards* from the consumer:
find the JumpIf{True,False}, then walk backwards through the
5-instr tail, then walk backwards collecting chain jumps that
target the branch_label. That works, but it's awkward because the
"backwards" walk can hit non-chain instructions (the optimizer's
SSA construction sometimes inserts a `Label(.<fn>@ssa_block@N)`
between the chain's last jump and the tail; older code paths can
have any number of unrelated instructions in there).

The clean version drops the backwards-walk entirely. It scans
forward looking for the 5-instr tail + 1-instr consumer (6
instructions total, structurally rigid) and retargets every jump
in the function that targets `branch_label`. The chain doesn't
need to be IDENTIFIED — anything that happens to jump to
`branch_label` is, by semantic equivalence, going to flow through
`Copy(C_sc, %t); fall through to consumer; consumer routes per
C_sc`. Replacing each such jump with one to D_sc preserves
exactly that flow.

That's actually a much stronger guarantee than I'd assumed when I
sat down. The rewrite is sound even if the "chain" includes weird
shapes like a `Jump(branch_label)` from elsewhere in the function
— anything that happens to target the soon-to-be-deleted label
gets correctly retargeted to wherever flow would have ended up.

So the soundness gates are just:

  - `%t` used exactly once across the function (the consumer).
  - `end_label` jumped to exactly once (by the in-tail `Jump`).
  - The 5-instr tail's structural shape matches.
  - `{C_ft, C_sc} == {0, 1}`.
  - The consumer is a direct `JumpIf{True,False}` on `%t`.

The pattern matcher is ~50 lines. The retarget step is two passes
over `new_instrs`: one to delete the 6-instr blocks (and emit the
flipped-case `Jump`/`Label` pair when needed), another to apply
the retarget map to every jump.

## The nested-chain gotcha

I shipped my first version, ran it on the example file that had
motivated the work, and got a `KeyError: '.or_true@33'` from the
asm-level CFG builder. The error happened during `to_ssa` on the
asm IR — meaning I'd produced TAC that translated to asm with a
jump to a non-existent label. Definitely my fault.

The function that died is `drift_step`. Its TAC has

```c
if (row == 0x63 || row == 0x8B || row == 0xB3) { ... }
```

which `c99_to_tac` nests as `(row == 0x63 || row == 0x8B) || row
== 0xB3` and lowers as two foldable short-circuit patterns
stacked on top of each other:

```
JumpIfCmp(==, row, 0x63, .or_true@35)    # inner chain
JumpIfCmp(==, row, 0x8B, .or_true@35)    # inner chain
Copy(0, %t1)                              # inner tail
Jump(.or_end@36)
Label(.or_true@35)
Copy(1, %t1)
Label(.or_end@36)
JumpIfTrue(%t1, .or_true@33)              # inner consumer / outer chain
JumpIfCmp(==, row, 0xB3, .or_true@33)    # outer chain
Copy(0, %t2)                              # outer tail
Jump(.or_end@34)
Label(.or_true@33)
Copy(1, %t2)
Label(.or_end@34)
JumpIfFalse(%t2, .if_else@38)             # outer consumer
```

The inner fold runs first. The inner is OR consumed by
JumpIfTrue — natural case — and records
`retarget_map[.or_true@35] = .or_true@33`. The outer is OR
consumed by JumpIfFalse — flipped case — and records
`retarget_map[.or_true@33] = .drift_step@scfold@0`.

Then the pass applied the retarget map. For each instruction with
a target field, look up the target in the map and substitute if
found. The inner chain's `JumpIfCmp(==, row, 0x63, .or_true@35)`
became `JumpIfCmp(==, row, 0x63, .or_true@33)` — but
`Label(.or_true@33)` was about to be deleted by the outer's
rewrite. The asm CFG builder caught the dangling reference.

The fix is one extra pass of transitive closure on
`retarget_map`. For each key, follow `new = retarget_map[new]`
until `new not in retarget_map`. After that pass:

```
retarget_map[.or_true@35] = .drift_step@scfold@0   # chained through .or_true@33
retarget_map[.or_true@33] = .drift_step@scfold@0
```

Now the inner chain jumps land at `.drift_step@scfold@0`, which
the outer's rewrite emits. Same destination the inner would have
reached by going through `.or_true@33` originally — and the same
destination it would reach if the outer's rewrite weren't
deleting that label out from under it.

I added a memory note for myself titled "Retarget maps need
transitive closure for nested patterns" — the kind of detail
that's easy to forget until the next adjacent-pair fold pass
needs it.

## Composition with branch_invert

The flipped case's rewrite emits `Jump(T); Label(.scfold@N)` and
retargets the chain to `.scfold@N`. Naively that's a couple of
extra TAC instructions compared to the natural case. In asm it's
something like

```
   BCC   .scfold@0
   CMP   #$50
   BCS   .scfold@0
   JMP   .if_end@22
.scfold@0:
   RTS
.if_end@22:
   ...
```

But there's an asm-level peephole called `branch_invert` whose
job is exactly to collapse `Branch(cond, L); Jump(target); L:`
into `Branch(inverted_cond, target)` when L is the immediately
following instruction. So the `BCS .scfold@0; JMP .if_end@22;
.scfold@0:` triplet collapses to `BCC .if_end@22`, and the asm
becomes

```
   LDA   screen_x
   CMP   #$40
   BCC   .scfold@0
   CMP   #$50
   BCC   .if_end@22
.scfold@0:
   RTS
.if_end@22:
   ...
```

— five-line return-early shape from the original ~14 lines. The
TAC pass didn't have to be clever about this; the asm-level
infrastructure that was already there picked up the win.

That's a nice property of c6502's middle-end / back-end split:
the TAC fold gets the shape into the neighborhood of optimal,
and the asm-level peepholes close the remaining distance without
the TAC pass having to know about them. The LogicalNot fold had
the same dynamic with `redundant_load_after_call` (drops the
`LDA` between a JSR and a `BEQ` when A already holds the return
value).

## Where it lives in the pipeline

I tried briefly running the new fold inside the main fixed-point
loop, alongside `cmp_zero_jump_fold` and `lnot_jump_fold`. The
issue is that pre-SSA-destruction the 5-instruction tail is
*not* 5 instructions — it's 5 + a `Phi` at the merge that
collapses the two SSA-renamed `%t` defs. The pattern is
structurally 9 instructions long and the single-use check on
`Phi.dst` doesn't compose cleanly with the two separate Copy
defs writing to two different SSA names. Doable, but messy.

The clean home is *post*-from_ssa. After SSA destruction +
`fold_copies` runs (the post-destruction pass that fuses the
per-arm Copy chains into a single Copy per arm), the canonical
5-instr tail is exactly what the pattern matcher expects. I
added the fold there, in its own small fixed-point loop
(nested short-circuits need multiple sweeps to fully collapse —
each pass folds the inner first, exposing the outer next).

The same observation probably applies to the Conditional
expression (`?:`) lowering, which has the same 5-instruction
tail shape with whatever Constants the user wrote on the
true/false arms. I haven't generalized to that yet — the
current pass restricts to `{0, 1}` to stay focused on the
short-circuit shape — but `cond ? 1 : 0` style idioms would
benefit identically. A future pass.

## Results

Across the one example file that motivated the work, 77 lines
of 821 dropped — about 9.4%. Five short-circuit blocks across
five functions, three `&&` and two `||`. Some natural, some
flipped. All folded cleanly; the asm-level `branch_invert`
peephole picked up the flipped-case composition without any
help from the TAC pass.

The new pass is ~120 lines of code (including the rigorous
docstring), ~280 lines of tests (sense-flip and constants table,
chain-jump variants — `Jump`/`JumpIf{True,False,Cmp,Masked}` —
nested chains via transitive closure, multi-use / multi-target
soundness guards, end-to-end sim across `&&` / `||` /
nested-`||` / `uint16_t` operands, and asm shape smoke checks).
It joined the optimizer in 4 files plus the re-blessed
snapshot.

What I take from this round: when you've already written one
fold for an instance of a pattern, the next instance is mostly a
matter of working out the truth table for the new shape and
trusting the existing infrastructure to handle the rest.
LogicalNot was a 2-instruction adjacent pair; short-circuit is a
6-instruction adjacent pair. The truth table is bigger (4 cases
instead of 2, two of which mint fresh labels), but the
*structure* is identical: pattern-match the producer + adjacent
consumer, retarget, delete the producer, let DSE and the
asm-level peepholes clean up downstream.

Two folds in two weeks, both prompted by reading the asm output
of a function I hadn't been planning to touch. The "find folds
by reading what the compiler actually emits" technique keeps
paying out. I'd bet there's one more fold-shaped wins in this
example file alone — I just haven't read closely enough yet.
