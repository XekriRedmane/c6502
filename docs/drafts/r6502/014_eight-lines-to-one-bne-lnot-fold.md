# Eight lines collapsed to BNE: a TAC fold for `if (!fn())` in my 6502 C compiler

Looking at a recently-compiled function in my hobby C-to-6502 compiler I
spotted this prologue for `entity_proximity`, which starts with
`if (!find_active_entity(...)) return;`:

```
   JSR   find_active_entity
   BEQ   .lnot_true@0
   LDA   #$00
   JMP   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   ORA   #$00
   BEQ   .if_end@1
   RTS
.if_end@1:
   ...
```

Eight instructions to do what should be one branch. `find_active_entity`
returns a `bool` in A (the compiler uses a zp_abi calling convention
that returns 1-byte values in the accumulator). So when control reaches
the first `BEQ`, A already holds the return value AND the Z flag
already reflects it (`LDA` was the last flag-setter inside the callee,
just before its `RTS`). The whole sequence after the JSR exists only to
turn that bool into a *negated* bool in A, and then test *that*. The
final shape we want is:

```
   JSR   find_active_entity
   BNE   .if_end@1
   RTS
.if_end@1:
   ...
```

## Why it isn't already that

The 8-instruction sequence is the literal lowering of two separate
operations from the compiler's middle IR (TAC):

```
%t = Unary(LogicalNot, %ret_of_call)
JumpIfFalse(%t, .if_end)
```

`LogicalNot` lowers to a "materialize 0 or 1 in A" idiom: test the
source, branch over a `LDA #0`, otherwise fall through to `LDA #1`.
Then `JumpIfFalse` is a separate translation that does `LDA t; BEQ
target` — except the first `LDA` is dropped by an existing asm-level
peephole when A already holds `%t`. That's where the post-JSR Z-flag
shortcut at the *start* of the sequence comes from, too: the
LogicalNot's first `Mov(src, A)` got dropped because A held the call's
return.

But once you've materialized 0 or 1 in A only to test it on the next
instruction, the post-JSR shortcut buys you nothing. You've already
discarded the post-call Z and replaced it with the Z of a constant.

## The fold

`Unary(LogicalNot, src, %t)` followed *immediately* by
`JumpIf{True,False}(%t, L)`, with `%t` used exactly once across the
function, rewrites to `JumpIf{False,True}(src, L)` — sense flipped to
absorb the LogicalNot's semantic inversion. The Unary becomes dead and
DSE reaps it.

After that, `JumpIfTrue(%ret, .if_end)` lowers as `LDA %ret; BNE
.if_end`. And once again, A already holds `%ret` post-JSR, so the same
asm-level peephole that dropped the leading LDA inside the LogicalNot
now drops it before the BNE — leaving a bare `JSR / BNE / RTS`.

Width doesn't enter the soundness argument. `Unary(LogicalNot, src)`
inverts a predicate; `JumpIf*` tests a predicate; the composition is a
sense-flipped test of the original. For a multi-byte source the
`JumpIf*` lowering ORs the bytes together with an `ORA` chain and
branches once on the result — strictly cheaper than materializing the
0/1 and then ORing-and-branching anyway.

## Soundness gate

The single-use check on the LogicalNot's `%t` is the only correctness
condition. If anything *else* reads `%t` — e.g. the negated value is
assigned to a variable AND tested — dropping the Unary would lose
those reads. SSA single-def + use-count == 1 is enough.

Strict adjacency keeps the implementation small; the fixed-point loop
collapses any intervening Copy/DSE so non-adjacent variants converge
in later rounds.

## Result in practice

The compiler's example corpus has two occurrences of this idiom in one
~800-line file (`companion_update.c`), both `if (!some_check())
return;` near the top of a function. After the fold:

- Each occurrence drops 7 lines from the asm and replaces the 8th
  with a sense-flipped branch.
- The dead Unary is cleaned up by DSE the same round.
- No other downstream peephole has to change; the shrinkage flows
  naturally from `JumpIfTrue`'s existing lowering plus the
  redundant-LDA-after-JSR cleanup that was already there.

Implementation is a ~40-line pass that mirrors the structure of the
existing `cmp_zero_jump_fold` (the same single-use, strict-adjacency,
sense-flipping shape, but for unary `!` instead of `==/!= 0`). I drop
it into the TAC fixed-point loop next to its sibling.

Curious if other 6502 compilers have this fold — it's a clean win on
the `if (!ok) return;` idiom that's all over hand-written game code.
