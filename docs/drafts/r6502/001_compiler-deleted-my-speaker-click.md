# Title

My C-to-6502 compiler deleted my speaker-click loop. Then I realized it wasn't because I forgot `volatile`.

# Body

I'm writing a C99 compiler that targets the 6502 (project's `c6502`, Python). It's getting good enough that I'm porting actual Apple II code to it as a stress test. Yesterday I fed it this:

```c
extern const volatile uint8_t *sfx_click_ptr;

__attribute__((zp_abi))
void sfx_tone(uint8_t pitch, uint8_t duration)
{
    do {
        volatile uint8_t y = pitch;
        while (--y != 0) { }       /* inner delay: count y down to 0 */
        (void)*sfx_click_ptr;      /* volatile read = click or silent */
    } while (--duration != 0);
}
```

Classic 6502 speaker-tone idiom. The hardware click is a volatile read through `sfx_click_ptr` (pointed at either `$C030` or `$C020` depending on whether sound is muted). The inner `while (--y)` is a busy-wait that sets the pitch — larger `pitch`, slower toggle, lower frequency.

Output:

```
sfx_tone:
   SUBROUTINE
.sfx_tone@asm_ssa_block@0:
   RTS
```

Just RTS. Both loops gone. The speaker click gone. Everything gone.

OK, first thought: I never implemented `volatile`. My grammar accepts the keyword but the parser silently drops it. So neither the `volatile uint8_t y` nor the `const volatile uint8_t *sfx_click_ptr` carry any volatile-ness past parse. The optimizer sees normal types and does normal things.

Second thought, before I fix volatile: **wait, what optimization actually deleted those loops?** I'd been planning to implement volatile to *block* the dead-store / dead-load passes — but the way the function reduces to RTS, both loops have to be eliminated as a whole, not just their stores. That's not what any pass I'd written should do.

So I wrote a non-volatile version of the same code:

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

Compiled it. Byte-identical output. Same RTS. So `volatile` wasn't the issue — these loops *should* have survived even without it (since the parser drops the keyword), but they were being deleted anyway.

Reading the TAC, I found my optimizer was sort of cheating its way to RTS:

1. `(void)*sfx_click_ptr` lowers to a `Load`. The Load's destination is a temp. Nobody reads the temp. The TAC dead-store-elimination pass drops the Load.
2. With the Load gone, the inner `while (--y) {}` becomes purely a decrement-to-zero with no side effects. **But my DSE couldn't delete the loop** — the decrement's dst was read by both the conditional jump and the back-edge phi, so nothing looked dead in isolation.

Yet the output had no loops. How? Turns out my regalloc and peephole passes were collapsing the in-place ADC-#1 chain to a `DEC mem; BNE` (the multi-byte INC peephole), and then `loop_counter_to_x` was promoting the counter to X. But neither pass *deleted* a loop — they just made it cheaper. So there had to be something else.

Re-reading more carefully: the actual output (`12 lines including only RTS`) was wrong, but the TAC I dumped DID show the loops. The mismatch was that I was running the example with my new dead-loop pass that I'd already added between the time I started this debugging and the time I dumped the TAC. So the chronological story is: at the start the optimizer left the loops in place; I added the dead-pure-loop pass; THAT'S what eliminated them.

So this isn't really a debugging story — it's a story about asking "is my baseline doing the right thing" before adding a feature that *gates* the baseline. If I'd added volatile semantics first, both the volatile and non-volatile versions would have left the loops alone (because the baseline never deleted them), and I'd have no way to tell whether volatile was actually doing anything.

The new pass (`eliminate_dead_loops`) is straightforward:

1. Find natural loops via back-edge detection (using the dominator analysis I already had for SSA construction).
2. Check the body has no `Call`, no `Store`, no `Ret`.
3. Check every SSA def in the body has no use outside the body.
4. If both gates pass, rewrite the header to `[Label, Jump(exit)]` — UCE prunes the now-unreachable body on the next fixed-point sweep.

For `do { while(--y); } while(--d);` the inner loop deletes; then the outer loop's body becomes "init y = pitch", which DSE drops; then the outer body is empty and gets the same treatment. Down to RTS.

Now the volatile work has a meaningful test. Compile sfx_tone.c WITH volatile semantics → loops survive. Compile WITHOUT volatile → loops deleted. Two distinguishable outputs.

Has anyone else hit this ordering thing — adding the "opt-out" feature before verifying the "opt-in" baseline actually does what you think? It feels like a generic compiler-writing trap, but I can't tell if it's documented anywhere as a named anti-pattern.
