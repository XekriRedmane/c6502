# Short-circuit jump fold: collapsing 0-or-1 materialize patterns at the TAC level

In an earlier round I wrote a TAC-level fold for `!cond`
immediately followed by a `JumpIf` — `Unary(LogicalNot, x, %t);
JumpIfFalse(%t, T)` becomes `JumpIfTrue(x, T)`, dropping the
0/1 materialize. While reading another generated example file
I noticed the same materialize-then-test shape on `&&` and `||`
and figured I'd generalize.

Before, for `if (screen_x >= 0x40 && screen_x < 0x47) ...`:

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

The two `BCC`/`BCS` are the short-circuit chain. Everything after
is a 0-or-1 materialize that the very next instruction re-tests
with `BEQ`. After the fold:

```
   LDA   __zpabi_entity_proximity__screen_x
   CMP   #$40
   BCC   .if_end@5
   CMP   #$47
   BCS   .if_end@5
```

The chain's short-circuit targets are retargeted directly to the
consumer's destination; the 0/1 materialize, the inner unconditional
`JMP`, both labels, and the consumer `LDA / BEQ` are all gone.

## What the TAC pattern looks like

`c99_to_tac.translate_short_circuit` emits the same 5-instruction
"tail" for every `&&` / `||`:

```
Copy(C_ft, %t)         # fallthrough value (chain didn't fire)
Jump(end_label)
Label(branch_label)
Copy(C_sc, %t)         # short-circuit value (chain fired)
Label(end_label)
```

where `(C_ft, C_sc) = (1, 0)` for `&&` and `(0, 1)` for `||`. The
chain of short-circuit jumps (one per operand) targets
`branch_label`; the consumer immediately follows and is one of
`JumpIfTrue(%t, T)` / `JumpIfFalse(%t, T)`.

There are four `(C_ft, C_sc) × consumer_kind` combinations. For each,
two destinations matter:

  - D_sc — where flow ends up when the chain fired (`%t = C_sc`).
  - D_ft — where flow ends up when no jump fired (`%t = C_ft`).

Each is either T (the consumer's target) or "next" (the instruction
after the consumer). Two sub-cases drop out:

  - **Natural** (D_sc == T): retarget every chain jump to T;
    delete the 6-instruction tail+consumer.
  - **Flipped** (D_sc == "next"): mint a fresh
    `.<funcname>@scfold@<N>` label; retarget the chain to it;
    replace the 6 instructions with `Jump(T); Label(.scfold@N)`.

The flipped sub-case looks bigger but composes nicely with the
asm-level `branch_invert` peephole — the trailing `BCS .scfold;
JMP .if_end; .scfold:` shape is exactly its target, so one of
the chain jumps inverts to a direct branch. A `if (a < 0x40 ||
a >= 0x50) return;` ends up as:

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

Five lines for the whole `||`-with-early-return shape.

## The transitive-closure gotcha

The fold processes patterns left-to-right and records
`retarget_map[branch_label] = new_target`. For nested chains like
`(a && b) || c`, the inner fold runs first and records
`retarget_map[.BR1] = .BR2` (the inner's flipped-case emits
`Jump(.BR2)`, which the outer's pattern hasn't been rewritten yet);
the outer fold then records `retarget_map[.BR2] = .scfold@1`. A
naive single-substitution pass would leave the inner chain's
jumps pointing at `.BR2` — a label about to be deleted. The asm-
level CFG builder then dies with
`KeyError: '.or_true@33'`.

The fix is one pass of transitive closure: for each key, follow
the chain `new = retarget_map[new]` until `new not in retarget_map`,
then substitute. Three-operand `||` (which `c99_to_tac` nests as
`(a||b)||c`) was the case that surfaced it — `drift_step`'s row-
anchor check (`row == 0x63 || row == 0x8B || row == 0xB3`) folds
to a clean three-jump chain.

## Results on one file

A companion-update example, ~820 lines of asm, three `&&` and
two `||` blocks scattered through five functions. Re-blessing the
example:

```
examples/companion_update.asm | 121 ++++++++--------------------
1 file changed, 22 insertions(+), 99 deletions(-)
```

77 lines saved end-to-end (the example shrinks from 821 to 744
lines, ~9.4%). Each `&&` block drops the 7-line materialize +
re-test sequence (12 asm bytes / ~17 cycles per occurrence on
average); each `||` block drops the same plus picks up an inverted
direct branch via `branch_invert` composition. The optimizer fold
runs as a strict subset of the post-from_ssa fixed-point loop, so
any future code with the same idiom picks it up for free.

The fold's soundness gates are: `%t` used exactly once,
`end_label` jumped to exactly once (only by the inner `Jump`),
both Copies write the same `%t`, and `{C_ft, C_sc} = {0, 1}`
specifically (`ConstInt` — keeps the pass focused on the
short-circuit shape rather than firing on `cond ? 5 : 0`
Conditional shapes, which would also be sound but probably
deserve their own pass).

Pattern detection runs post-from_ssa rather than inside the main
SSA-time fixedpoint, because pre-destruction the 5-instr tail is
split across two SSA-renamed `%t` defs merged by a Phi — the
pattern is ~9 instructions long instead of 5. `fold_copies` runs
right before this pass and consolidates the per-arm Copy chains
into the canonical 5-instr tail.
