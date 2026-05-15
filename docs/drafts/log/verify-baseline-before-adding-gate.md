# Title

Verify the baseline before you implement the gate

# Body

I was about to start implementing `volatile` in my C-to-6502 compiler. The motivating program was a speaker-click routine I'd ported from an Apple II disassembly:

```c
extern const volatile uint8_t *sfx_click_ptr;

__attribute__((zp_abi))
void sfx_tone(uint8_t pitch, uint8_t duration)
{
    do {
        volatile uint8_t y = pitch;
        while (--y != 0) { }
        (void)*sfx_click_ptr;
    } while (--duration != 0);
}
```

The `*sfx_click_ptr` dereference is a memory-mapped I/O read — on the Apple II, reading from `$C030` toggles the speaker membrane and reading from `$C020` does nothing. Whether the speaker clicks or not depends on which address the input handler stored into `sfx_click_ptr`. The `volatile` qualifier on the pointee is what tells a C compiler "this read has observable side effects; don't optimize it away."

When I compiled it, the output was:

```
sfx_tone:
   SUBROUTINE
   RTS
```

Twelve lines of preamble, then `RTS`. Both loops gone. No speaker click. Nothing left at all.

That's what I expected, in a way — `c6502` accepts the `volatile` keyword in its grammar but the parser silently drops it. The `_TYPE_QUALIFIER_TOKEN_TYPES` tuple lists `("CONST", "VOLATILE", "RESTRICT")`, but `_resolve_data_type` only honors `CONST`. So my optimizer treats every `volatile T` as `T`. Of course the loops got deleted. I was about to write down the volatile implementation plan when something caught my attention.

## The check that almost didn't happen

It would have been easy to start writing volatile. The user (me, talking to my Claude Code session) gave the directive: "Before we start, I think it's important that if the volatile flag weren't there, the optimizer should eventually drive the function down to a simple RTS. Make sure that happens first."

This is a check that compiler writers don't always do, and I want to spend a few hundred words on why it matters.

When you add a feature that *gates* an existing optimization — `volatile` to block dead-load elimination, `restrict` to permit alias-based reordering, `_Atomic` to prevent CSE across atomic operations — you implicitly assume the baseline optimization is working as advertised. The whole point of the gate is to opt out of behavior the compiler would *otherwise* perform. If the baseline isn't actually performing that behavior, your gate measures nothing.

In the worst case, you ship volatile, write tests that compare "with volatile" to "without volatile", and they pass — both versions produce the same output, because the baseline never did the optimization in the first place. The gate appears correct but is asserting against a baseline that no longer holds. Some later optimizer change makes the baseline aggressive, and now the volatile-marked code starts disappearing too, with no test catching it.

This is the same shape as testing error-handling code without confirming the error path is reachable. "I added `try/except` and the test passes" doesn't mean anything until you've verified the test actually exercises the exception.

## What I found

I wrote a non-volatile version:

```c
extern const uint8_t *sfx_click_ptr;

__attribute__((zp_abi))
void sfx_tone(uint8_t pitch, uint8_t duration)
{
    do {
        uint8_t y = pitch;
        while (--y != 0) { }
        (void)*sfx_click_ptr;
    } while (--duration != 0);
}
```

Compiled it. Byte-identical output. Both versions reduced to `RTS`.

That was the smoking gun. The volatile version and the non-volatile version produced the same asm because:
- The parser drops volatile, so the two ASTs are identical from the AST onward.
- The optimizer aggressively eliminated everything.

Specifically: the `(void)*sfx_click_ptr` lowers to a TAC `Load` whose dst is a temp nobody reads — dead-store elimination removes it. With the Load gone, the inner `while (--y) {}` has no side effects in its body, only a decrement-to-zero. Similarly for the outer loop.

But here's where I had to look carefully: standard dead-store elimination shouldn't have been able to delete those loops. The decrement's dst is read by the conditional jump and by the back-edge phi. Nothing looks dead in isolation. So how did the loops disappear?

The answer, when I traced through, was that they hadn't been disappearing — *until I added the pass that deletes them*. That's a confusing sentence, but in my session I was iterating quickly: I'd already prototyped a dead-pure-loop pass while exploring the problem, and the version of the compiler producing the all-`RTS` output was the version with that pass enabled. The original (pre-my-pass) compiler left the loops intact — the in-place `ADC #1; STA` sequence collapsed to `DEC mem; BNE` via my multi-byte INC peephole, and the counter got promoted to X by `loop_counter_to_x`, but the loop itself remained. It was just a delay loop with cheap addressing.

This actually made the situation more interesting. Without the dead-pure-loop pass, the optimizer preserved both loops — meaning volatile and non-volatile would have produced the same output (loops preserved), and volatile semantics would have been invisible to any test I wrote.

To make volatile testable, I needed to first make the *non-volatile* version produce *different* output than I wanted the volatile version to produce. That is: I needed the baseline to aggressively delete the loops, so that the volatile gate had something to opt out of.

## The pass

What was missing was straightforward to describe but worth implementing carefully:

1. Build a control-flow graph (already had).
2. Compute immediate dominators (already had — used for SSA construction).
3. For each back-edge `(tail → header)` where `header` dominates `tail`, identify the natural loop: the header plus every block that can reach `tail` backwards without crossing the header.
4. Check the loop body has no observable side effects: no `Call`, no `Store`, no `Ret`.
5. Check the loop body has no live-out def: every SSA name defined in the body has all its uses inside the body.
6. If both checks pass, rewrite the header to `[Label(header_name), Jump(exit_label)]`. The next sweep of the unreachable-code-elimination pass prunes the rest of the body.

Two non-obvious bits in the implementation:

**Single-exit gate.** I considered multi-exit loops, but they need extra care: if the loop has two exit edges going to the same destination, the destination's φ-nodes might have args from both exit sources, and consolidating them into one arg from the new header requires checking that the arg sources agree (otherwise you're collapsing two semantically distinct paths). Easier to skip multi-exit loops for now and revisit when a motivating case shows up.

**φ retagging.** When the exit edge originates from a non-header body block, any φ-node at the exit has a `pred_label` referring to that body block. After the rewrite, the body block is unreachable and the φ's predecessor is the header instead. The UCE pass would drop the `pred_label` as stale, losing the φ-arg's source. To preserve it, I retag the `pred_label` from the body-block's label to the header's label before invoking UCE.

The live-out check makes sure the φ-arg's source isn't a loop-internal name (we already refused those loops), so the retag is purely a structural predecessor update — the value semantics are unchanged.

## Outcome

With the pass in place:

- `sfx_tone.c` (with `volatile`) compiles to `RTS`. So does the non-volatile version. They produce identical output because volatile is still being silently dropped.
- The full chapter test suite (2475 tests) passes with no regressions.

Now I can implement volatile and have it mean something. The test will be: compile `sfx_tone.c` with volatile semantics → loops survive. Without volatile → loops deleted. Two distinguishable outputs. The gate measures something real.

The general lesson, written down for next time: when a user (or I-as-user) asks for a feature whose purpose is to *prevent* an optimization, the first thing to check is that the optimization actually fires on a representative input without the feature. If it doesn't, the implementation order is wrong — fix the baseline first.

I've added this to my project's auto-memory so future sessions catch it:

> When the user asks for a feature whose purpose is to *block* an existing optimization (e.g. `volatile` to prevent dead-load elimination, `restrict` to permit aliasing-based opts, `atomic` to block reordering), first verify that without the feature the optimizer actually performs the optimization in question. If it doesn't, the test of the new feature is meaningless.

Next session: actually implement volatile.
