# A fold I could see but hadn't written

In a wrap-up session for some bigger work I was looking through one of
the example outputs of my C-to-6502 compiler and stopped on a block of
code that just looked wrong:

```
entity_proximity:
   ...
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

The source is innocuous:

```c
if (!find_active_entity(hit_max, &entity_row)) return;
```

`find_active_entity` returns `bool`. The compiler uses a calling
convention (call-graph-disjoint ZP allocation, `__attribute__((zp_abi))`)
where 1-byte returns leave the value in A — and so the Z flag right
after the `JSR` already reflects the return value, because the last
flag-setting instruction inside the callee is a `LDA` of the return
value before `RTS`. The 8-line sequence then proceeds to:

1. Use that Z flag to branch into the LogicalNot's "the predicate was
   zero" arm,
2. materialize `0` or `1` in A,
3. ORA the result with itself (`ORA #$00` is a one-byte way to set Z
   from A),
4. branch on Z.

What I wanted was the obvious one-step substitution:

```
   JSR   find_active_entity
   BNE   .if_end@1
   RTS
.if_end@1:
   ...
```

Same semantics: "if the return is non-zero, skip the early return."
The post-JSR Z flag is already exactly the signal we want, just
inverted from what `JumpIfFalse(!ret)` claims to test.

So where had I gone wrong in the optimizer? I hadn't gone wrong
anywhere in particular — I'd just never written a fold for this
combination.

## What the compiler actually does

The first thing to understand is that the asm-level lowering of
`!cond` (`LogicalNot` in TAC) is already pretty smart about the post-
call case. It emits:

```
Mov(src, A)         ; would emit LDA, but if A already holds src, dropped
Branch(EQ, .true)
Mov(Imm(0), A)
Jump(.end)
.true:
Mov(Imm(1), A)
.end:
```

The first `Mov(src, A)` is dead-coded away by an asm-level redundant-
load peephole when `src` is already in A — which is exactly the case
for a value just returned by a `JSR`. That's why the asm starts with
`JSR; BEQ .lnot_true` and not `JSR; LDA ret; BEQ .lnot_true`.

So the JSR-Z-flag shortcut at the *front* of the sequence comes for
free. What doesn't come for free is *the rest*: the 0/1 materialize, a
no-op `ORA #$00`, and a branch. All of that exists only because the
TAC right after the LogicalNot is:

```
JumpIfFalse(%lnot_result, .if_end)
```

We compute the LogicalNot's result into a temp and then test it. The
asm side can't easily collapse this because by the time the asm
optimizer sees it, the materialization is already concrete (with
labels and a branch), and asm peepholes that touch labels are
expensive in fix-up cost.

## Where the fold belongs

This is a clean TAC-level pattern: producer and consumer are adjacent;
the consumer reads exactly the producer's dst; the producer's dst is
otherwise dead.

```
Unary(LogicalNot, src, %t)
JumpIf{True,False}(%t, target)
```

→

```
JumpIf{False,True}(src, target)
```

— sense flipped to absorb LogicalNot's semantic inversion.
Width-agnostic: the `JumpIf*` lowering for a multi-byte source ORs
the bytes together and branches once on the result, which is strictly
cheaper than materializing 0/1 first and then doing the same OR.

The soundness gate is single-use of `%t`. If anything *else* reads
the materialized 0/1 — say the negated value is also assigned to a
variable — then dropping the producer would silently change those
reads. In SSA TAC, single-def is automatic; we just need use-count ==
1.

## The implementation, with one wrong turn

I sat down to write this and immediately overcomplicated it.

In TAC the basic-block boundaries don't matter for adjacent-pair
folds — you walk the instruction list looking for `(i, i+1)` pairs
that match, and when one does, you emit a single replacement
instruction in place of both. My first cut used a module-level set to
remember which indices to "skip" on a second walk:

```python
_SKIP_IDX_SENTINEL: set[int] = set()
...
new_instrs.pop()  # back up over the just-appended Unary
new_instrs.append(rewrite)
_SKIP_IDX_SENTINEL.add(i + 1)
...
# second pass to actually drop the JumpIfs whose slot got replaced
```

Reading it back, the giveaway was the "second pass" — the rewrite
doesn't need a second pass; it needs a one-pass loop that knows to
skip the next instruction when the current one's rewrite consumed it.
And the sibling pass in the same directory, `cmp_zero_jump_fold`,
does exactly that:

```python
new_instrs: list[tac_ast.Type_instruction] = []
skip_next = False
for i, instr in enumerate(fn.instructions):
    if skip_next:
        skip_next = False
        continue
    rewrite = _try_fold(...)
    if rewrite is None:
        new_instrs.append(instr)
        continue
    new_instrs.append(rewrite)
    skip_next = True
```

Twelve lines, no module state, no second walk. I'd written the
overcomplicated version because I'd written the `_try_fold` helper
first and was thinking in terms of "what do I do when I find a
match" rather than "what's the iteration shape."

I rewrote in the `skip_next` shape, lifted the `_count_var_uses` and
`_vars_used_in` helpers verbatim from `cmp_zero_jump_fold` (the
`_vars_used_in` matcher needs to handle *every* TAC variant or
single-use counts would be wrong for fold-candidate temps — and the
sibling pass already had a complete enumeration), and the whole pass
came in at ~40 lines plus a docstring.

The takeaway, which I dropped into my memory file: before writing a
new TAC fold pass that has the same shape as an existing one
(adjacent pair + single-use temp + rewrite), read the existing pass
*first*. The iteration idiom is usually the part you'd reinvent badly
if you didn't.

## A meta-observation about flag mechanics

While discussing this with my collaborator, a useful framing came up.
The reason the simplest form of this optimization — collapsing the
whole block to a single `BNE` — works isn't *just* that the
LogicalNot can be folded away at the TAC level. It also depends on a
6502-specific fact: the calling convention's last flag-modifying
instruction is `LDA bool_val` inside the callee. So after a JSR to a
1-byte-bool-returning function, `Z` already reflects the return value
and there's no need to re-test.

If the function had returned a `uint16_t` instead, the optimization
would still be sound at the TAC level — the JumpIfTrue/False lowering
would just emit an ORA chain across the bytes — but the very tightest
form (no LDA at all between the JSR and the branch) only works
because of the 1-byte-return shape and the asm-level redundant-load-
after-call peephole. The fold doesn't *know* about that; it just
hands the rest of the pipeline a shape that's already in the
neighborhood of optimal.

This is the kind of thing that makes 6502 compilation interesting:
the calling convention and the addressing-mode quirks reach pretty
deep into what middle-end folds turn out to be worth writing. There
are TAC-level optimizations that produce identical bytecode on x86
or ARM and are worth implementing on those targets — but on the
6502, the asm-level cleanup needs to compose for the *whole* chain to
land at minimal code. Knowing the destination shape (post-JSR-Z still
reflects A → asm-opt drops the LDA before a branch) tells you which
TAC-level rewrites are worth chasing.

## Results

Two occurrences in the example file, both in the `if (!ok) return;`
shape near the top of a function. After the pass:

- Each occurrence drops 7 lines from the asm and replaces the 8th
  with a sense-flipped branch.
- The dead Unary is cleaned up by standard DSE in the same fixed-
  point round.
- No other peephole had to change; the shrinkage flowed naturally
  from `JumpIfTrue`'s lowering plus the existing
  redundant-LDA-after-call cleanup.
- 14 lines saved in `companion_update.asm` (about 1.6% of the
  function's emit), and the optimization is a strict subset of the
  fixed-point loop, so future code with the same idiom will pick
  it up for free.

The pass joined the optimizer in 4 files: a new `lnot_jump_fold.py`,
a one-line wire-up in `optimizer.py`, 12 unit tests (sense-flip
table, single-use / adjacency / op-shape gates, end-to-end sim
across uchar / uint16 / uint32 sources, and an asm-shape smoke
check), and the re-blessed snapshot for the example whose output
shrunk.

## What's left

I don't expect this exact pass to grow much more — the soundness
gate is tight and the rewrite is structural. The directional
question is whether the same shape applies to *other* unary
operations that the compiler computes and then immediately
branches on. `Negate` and `Complement` don't have the sense-
inversion semantics that make this fold sound; they'd give the
wrong answer for negative zero or after a complement of a value
whose low byte happens to be zero. So this pass is genuinely
LogicalNot-specific.

But it's a reminder of how much code-size win is sitting one
fold-level away in a compiler whose middle-end and back-end mostly
already know what to do — and how often the right way to find it
is to actually read the asm of a function you didn't expect to
look closely at.
