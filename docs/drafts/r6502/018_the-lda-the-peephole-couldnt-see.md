# The LDA the peephole couldn't see

Here is a snippet from a 6502 function emitted by my C99 compiler,
[c6502](https://github.com/XekriRedmane/c6502). The compiler is
under `--optimize`, every peephole I've written is active, and yet
something is plainly wrong:

```asm
    SEC
    SBC   #$04
    STA   __local_do_ascend__col
    LDA   __local_do_ascend__col   ; ← redundant
    STA   player_col
```

That third instruction is doing nothing. `STA` doesn't touch any
flag, so `A` still holds the value `SBC #$04` produced, and the
flags still reflect it. The `LDA __local_do_ascend__col` rereads
the byte we *just* wrote — same value into the same register,
same N/Z that the SBC already set.

The pass that's supposed to catch this exists. It's called
`round_trip_load_drop`, and its docstring describes exactly this
pattern:

> Drop adjacent `STA M; LDA M` round-trip when A's value already
> reflects M, AND the preceding A-writer already set the flags to
> A's value.

So why didn't it fire?

## The peephole is correct. The IR lies.

The IR before emit, dumped at the input to `round_trip_load_drop`,
looks like this:

```
13: Sub(src=Imm(4), dst=Reg(A))
14: Mov(src=Reg(A), dst=Data('__local_do_ascend__col'))
15: Mov(src=Data('__local_do_ascend__col'),
        dst=Data('player_col'))
```

Three atoms. Five 6502 instructions in the emitted asm. The
mismatch is atom 15: a `Mov` whose source AND destination are
both memory operands. There is no `MOV mem, mem` opcode on the
6502 — `asm_emit` lowers `Mov(mem, mem)` as `LDA src; STA dst`,
using A as the staging register. One IR atom, two opcodes.

`round_trip_load_drop`'s 3-instruction window is `flag-effect
writer; STA M; LDA M`. It scans for an explicit `Mov(M, Reg(A))`
atom. Atom 15 is a `Mov(M, Data)`, not a `Mov(M, Reg(A))` — the
implicit LDA inside the mem-to-mem lowering is invisible to a
peephole that matches at the IR level. The pass walks right past
it.

This is the same opacity I posted about in
[r6502/009](https://reddit.com/r/6502) — that time it caused a
*correctness* bug (a cross-block A-tracker thought A held a value
it didn't, because a mem-to-mem Mov silently clobbered it). Here
it's only costing an optimization, but the underlying IR mismatch
is identical.

## The fix

The peephole's existing rewrite is "drop the LDA":

```
[i-1]   flag-effect A-writer        ; A := V, flags := N/Z(V)
[i]     Mov(A, M)                   ; STA M, A unchanged, flags unchanged
[i+1]   Mov(M, A)                   ; LDA M, redundant
```

→ drop `[i+1]`.

The mem-to-mem variant uses the same setup but a different third
atom — and asks for a *rewrite*, not a drop:

```
[i-1]   flag-effect A-writer        ; A := V, flags := N/Z(V)
[i]     Mov(A, M)                   ; STA M
[i+1]   Mov(M, dst_mem)             ; emit: LDA M; STA dst
```

→ rewrite `[i+1]` to `Mov(Reg(A), dst_mem)`, which emits as a
plain `STA dst`. The hidden LDA collapses to nothing.

Flag soundness is the same argument. After `[i-1]`, N/Z reflect
V. `STA M` doesn't touch flags. In the OLD shape, the implicit
`LDA M` sets N/Z = N/Z(M); but M == V at that point, so the
flags it sets are identical to what `[i-1]` left. In the NEW
shape, the implicit LDA is gone, and `STA dst` doesn't touch
flags, so they're still N/Z(V). Same exit state. Safe.

About 30 lines of pass code, six unit tests covering both
patterns plus the volatile / address-mismatch / no-flag-effect
skip cases.

## What it bought

The example program is `do_ascend`, a per-frame ascent step out
of the game engine corpus. After the fix:

```
binary:     660 → 654 bytes        (-6)
do_ascend:  404 → 395 cycles       (-9 per call across the test)
```

Six bytes is not much, and the cycle saving is small. But the
fix isn't really about this one function — it's a general
peephole, and any time the optimizer produces `flag-A-writer;
STA M; mem-to-mem-from-M`, the LDA hidden inside that
mem-to-mem now goes away. That shape comes up wherever a value
is computed in A, stashed to a local, and immediately propagated
out to a global or another local: a common emission shape for
plain C assignments like `player_col = col;` after `col -= 4;`.

## The broader thing

There are exactly two ways to fix this kind of bug. One is to
extend every peephole (and every dataflow tracker) to understand
that mem-to-mem Mov implicitly clobbers A and reads from / writes
to specific addresses. That's what I did here, and what the
post-mem-to-mem-correctness-fix in r6502/009 also did. The other
is to lower mem-to-mem Mov into `Mov(src, Reg(A)); Mov(Reg(A),
dst)` earlier in the pipeline, so every downstream pass sees the
two opcodes that actually get emitted.

I keep choosing the first option, narrowly, one pass at a time.
Each time I do, the entry in my "things that are 1:1 with a 6502
instruction except for these exceptions" notebook gets a little
longer. At some point I'll do the splitting.
