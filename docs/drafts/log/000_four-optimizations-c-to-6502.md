# Closing the gap between a C compiler's output and hand-written 6502 assembly

I've been writing a C99 compiler that targets the MOS 6502 — c6502, in Python, ASDL-defined IRs, classic textbook structure. It's pre-runtime, so programs assemble cleanly but won't link yet. The interesting work right now is the optimization pipeline: I use real 6502 programs from the corpus (Drol, mainly) as targets to chase, taking the hand-written assembly as ground truth and treating any structural gap between my compiler's output and the hand-written reference as a backlog of optimizations to implement.

This is a write-up of one such gap-closing session. I started from a compiler bug and ended up implementing four optimizations that, composed, brought the compiler's output for a real Drol routine to within striking distance of the hand-written original.

## The target

The motivating function is a per-frame routine that draws four floor enemies. Each frame, it walks the four slots in reverse, skips inactive ones, projects each active enemy's column through two perspective tables, picks the right sprite-pointer pair for that slot, and dispatches a draw call:

```c
extern uint8_t enemy_flag[4];
extern uint8_t enemy_col[4];
extern uint8_t enemy_y[4];

static const uint8_t proj_screen_col[132] = { /* perspective table */ };
static const uint8_t proj_frame_idx[165]  = { /* walking-frame cycle */ };

static const uint8_t floor_enemy_spr_s0_lo[7] = { /* slot-0 frame addrs (lo bytes) */ };
static const uint8_t floor_enemy_spr_s0_hi[7] = { /* slot-0 frame addrs (hi bytes) */ };
/* ...s1, s2, s3 likewise... */

static const uint8_t *const floor_enemy_spr_lo[4] = {
    floor_enemy_spr_s0_lo, floor_enemy_spr_s1_lo,
    floor_enemy_spr_s2_lo, floor_enemy_spr_s3_lo,
};
static const uint8_t *const floor_enemy_spr_hi[4] = { /* same shape */ };

__attribute__((zp_abi))
void floor_enemy_draw(uint8_t page_flag) {
    for (int8_t slot = 3; slot >= 0; slot--) {
        if (enemy_flag[slot] == 0) continue;
        uint8_t col_idx    = enemy_col[slot];
        uint8_t screen_col = proj_screen_col[col_idx];
        uint8_t frame      = proj_frame_idx[col_idx];
        uint8_t lo = floor_enemy_spr_lo[slot][frame];
        uint8_t hi = floor_enemy_spr_hi[slot][frame];
        const uint8_t *src = (const uint8_t *)(((uint16_t)hi << 8) | lo);
        draw_sprite(0x01, 0x05, screen_col, enemy_y[slot], src, page_flag);
    }
}
```

What the hand-written original does:

```
        LDX #$03
.find_active:
        LDA ZP_ENEMY_FLAG,X         ; slot's flag
        BNE .active
.next_slot:
        DEX
        BPL .find_active
        RTS

.active:
        LDA ZP_ENEMY_Y,X
        STA ZP_SPRITE_Y
        LDA #$01
        STA ZP_SPRITE_W
        LDA #$05
        STA ZP_SPRITE_H
        LDA ZP_ENEMY_COL,X
        TAY
        LDA PROJ_SCREEN_COL,Y
        STA ZP_SPRITE_X
        LDA PROJ_FRAME_IDX,Y
        TAY                          ; Y' = frame

        CPX #$03                     ; slot dispatch
        BNE .not_s3
        LDA FLOOR_ENEMY_SPR_S3_LO,Y
        STA SMC_DS_SRC_LO            ; SMC: patch DRAW_SPRITE's operand
        LDA FLOOR_ENEMY_SPR_S3_HI,Y
        STA SMC_DS_SRC_HI
        STX ZP_SAVE_X
        JSR DRAW_SPRITE
        LDX ZP_SAVE_X
        JMP .next_slot
.not_s3:
        CPX #$02
        BNE .not_s2
        ... slot 2 ...
.not_s2:
        ... slot 1, then slot 0 fallthrough ...
```

Three structural choices stand out:

1. **`slot` lives in X for the whole loop.** Save/restore with a one-byte ZP cell around the `JSR`. No `LDX zp` reload at the top of every iteration.
2. **The slot dispatch is inlined.** A `CPX #k / BNE / LDA SPR_Sk_LO,Y` chain replaces what would otherwise be a pointer-table lookup with an indirect-Y deref.
3. **Self-modifying code patches `DRAW_SPRITE`'s `LDA` operand bytes** so the callee reads the sprite directly with `LDA $XXXX,...`, no separate pointer-fetch.

The 6502 SMC trick (#3) is fundamentally outside what a typed IR can express — code is data in 6502 assembly, but not in a compiler that treats code as immutable. So I'll never close that gap. But (#1) and (#2) are squarely on the optimizer's plate. Let's see where my compiler started.

## The starting point: a compiler bug

I tried to compile the function. It threw:

```
AssemblerError: unsupported Mov: IndexedData(name='enemy_col', offset=0, index=X())
              -> Reg(reg=X())
```

The asm-level optimizer had produced an instruction that the in-process assembler refused to encode — `LDX enemy_col,X`. For anyone who hasn't memorized the 6502 opcode table:

The 6502 has `LDX abs,Y` (0xBE) and `LDY abs,X` (0xBC), but it does NOT have `LDX abs,X` or `LDY abs,Y`. The asymmetric same-index combinations of indexed loads simply don't exist. And neither do any of the `STX abs,X|Y` or `STY abs,X|Y` forms. So an index register can't simultaneously be both the addressing-mode index and the load/store target of the same instruction.

That's a fundamental hardware constraint that I'd half-encoded in c6502: my emitter knew that `LDA arr,Y` was 3 bytes, but it didn't have an opinion on `LDX arr,X` because no pass had ever asked it to emit one. Until now.

The pass that produced the bad instruction was the asm-level HwReg coloring (`hwreg_eligibility`). It tracks which SSA Pseudos can be "pinned" to the X or Y register across their live range, sparing the cost of the `LDX / LDY` setup before each `IndexedData` access where the index is the pinned value.

A Pseudo `P` whose only def was `Mov(IndexedData(enemy_col, _, X), P)` — that is, "P holds the value loaded from `enemy_col` using X as the index" — was being considered eligible for X-pinning. The eligibility scan walked all the def/use sites and asked "can this be encoded if P is in X?" and the IndexedData-peer check returned `True` blindly.

But that's exactly the case where pinning to X would force `LDX enemy_col,X` — the impossible opcode.

## Optimization 1: per-HwReg eligibility

My initial reaction was the conservative fix: refuse to pin a Pseudo whose def or use involves an `IndexedData` peer. Done — one-line change, the bug goes away, and I committed it.

But that conservative fix throws away the two cross-cases where the load IS encodable:

* `Mov(IndexedData(...,X), Reg(Y))` is `LDY abs,X` — 3 bytes, exists.
* `Mov(IndexedData(...,Y), Reg(X))` is `LDX abs,Y` — 3 bytes, exists.

For our example: `col_idx = enemy_col[slot]` loads the byte using X as the index. If `col_idx` ends up pinned to Y, the load becomes `LDY enemy_col,X` — entirely fine. And the subsequent `proj_screen_col[col_idx]` and `proj_frame_idx[col_idx]` accesses, which would normally need an `LDA col_idx; TAX` chain to set up X, can now be `LDA proj_screen_col,Y` directly — Y is already loaded.

So the real fix wasn't to disqualify IndexedData peers entirely. It was to track eligibility per-HwReg: keep a separate `eligible_x` set and `eligible_y` set on the eligibility result. The asymmetric IndexedData case is the only one where they differ — for every other peer type, the symmetry of the instruction set means a Pseudo is either eligible for both or neither.

```python
@dataclass
class HwRegEligibility:
    eligible_x: set[str] = field(default_factory=set)
    eligible_y: set[str] = field(default_factory=set)
    hints_x:    set[str] = field(default_factory=set)
    hints_y:    set[str] = field(default_factory=set)
    use_count:  dict[str, int] = field(default_factory=dict)
```

The downstream regalloc's `_can_pin(name, reg)` now checks per-HwReg eligibility:

```python
def _can_pin(name: str, reg: str) -> bool:
    if reg == "X" and name not in eligibility.eligible_x:
        return False
    if reg == "Y" and name not in eligibility.eligible_y:
        return False
    # ... existing interference / cross-call / etc. checks
```

The hint-driven assignment ("prefer X" or "prefer Y") still uses the union for filtering — a hint is just a preference, the per-HwReg gate enforces feasibility.

I also taught the emitter and the in-process assembler the two valid cross-cases:

```python
# IndexedData → Reg(X) / Reg(Y): only the cross-index combinations exist.
if isinstance(src, asm_ast.IndexedData) and isinstance(dst, asm_ast.Reg):
    if (isinstance(dst.reg, asm_ast.X)
        and isinstance(src.index, asm_ast.Y)):
        return [_instr_line("LDX", _indexed_data_addr(src))]
    if (isinstance(dst.reg, asm_ast.Y)
        and isinstance(src.index, asm_ast.X)):
        return [_instr_line("LDY", _indexed_data_addr(src))]
```

The emitter rejects the same-index cases by falling through — the assembler then raises a clean error rather than silently emitting garbage. Defense in depth.

### A subtlety in the cross-transfer rewrite

The asm optimizer has a phase that recognizes a `Mov(P, A); Mov(A, X|Y)` index-setup chain after Y-pinning and rewrites the subsequent `LDA arr,X` accesses to use the actual pinned register. With more Pseudos becoming Y-eligible, this rewriter started seeing cases where the rewrite would produce an unencodable shape — e.g. converting `LDY proj_frame_idx,X` (which was the pre-rewrite consolidated form of `LDA proj_frame_idx,X; TAY` after Y-pinning the dst) into `LDY proj_frame_idx,Y` — same-index, invalid.

I gave the rewriter a fallback: when the rewrite would produce an unencodable Mov, *split* the consolidated load back into `LDA arr,index; TAR` instead of giving up. The split is one byte longer than the consolidated form would have been, but the transfer chain that precedes it (2 bytes) still drops, so the rewrite still nets one byte saved. And, critically, it leaves the same-index-register cases reachable instead of bailing.

## Optimization 2: keep the loop counter in X across the loop body

My compiler already had a `loop_counter_to_x` pass that detects the canonical "uchar counter initialized once, decremented at the bottom" loop shape and promotes the counter slot to live in X. It already supported wrapping `STX zp / JSR / LDX zp` around any `Call` inside the loop body — the hand-written code's exact pattern.

But the pass was failing to fire on this example. The disqualifier: the body had `LDA __local_slot` instances coming from the `slot * 2` arithmetic in the const-pointer-array indexing.

```
LDA __local_slot         ; load counter
STA __local_b0
ASL __local_b0           ; slot * 2
LDX __local_b0
LDA floor_enemy_spr_lo,X
...
```

The eligibility filter saw `LDA __local_slot` and bailed: "the slot has a use that isn't an `LDX M` or `DEC M`; can't promote." But that filter is too strict. After promotion, `X = M` is the invariant — so `LDA M` is value-equivalent to `TXA`. We can accept `LDA M` uses in the body and rewrite them at apply time.

That's a five-line change: add a new `lda` role to the eligibility classifier (alongside `init`, `ldx`, `dec`), collect the indices of `LDA M` Movs, and in the apply phase replace each with `Mov(Reg(X), Reg(A))` (TXA).

### A latent soundness bug

Extending Y-pinning surfaced a soundness bug I'd had for a while. The `loop_counter_to_x` pass has a sub-phase that creates "Y-pivot ranges" — short stretches within the loop body where a non-counter X-write happens, the pass rewrites the LDX to LDY and shifts all the indexed accesses in the range to use Y instead, leaving X free for the counter.

The pivot validator checked: "no Y reads or writes inside the range" — and explicitly looked for `IndexedData(..., index=Y)` operands as Y-reads.

But it didn't check `Indirect`, `IndirectY`, `IndirectZp`, or `IndirectZpY` operands. These all use Y for their addressing mode (`(zp),Y` and similar). With the per-HwReg refactor putting more values into Y around the pivot point, the pivot started overwriting live Y values, breaking observable behavior.

The fix is one extra check in the pivot validator:

```python
if isinstance(op, (asm_ast.Indirect, asm_ast.IndirectY,
                   asm_ast.IndirectZp, asm_ast.IndirectZpY)):
    return None
```

Caught by the sim differential test I'd written for this example. Unit tests never would have surfaced this — the bug needs the specific combination of indirect-Y access + pivot rewrite that only arises end-to-end.

## Optimization 3: inline switch dispatch for small `const T *const arr[N]`

This is the big one — the actual structural transformation that gets c6502's output close to the hand-written.

The TAC for `floor_enemy_spr_lo[slot][frame]` (where `slot` and `frame` are uchars and `floor_enemy_spr_lo` is a `static const uint8_t *const[4]`) looks like:

```
%scaled = Binary(LeftShift, slot, ConstInt(1), %scaled)   ; slot * 2
%ptr    = IndexedLoad(floor_enemy_spr_lo, %scaled, %ptr)  ; 2-byte pointer
%val    = IndirectIndexedLoad(%ptr, frame, %val)
```

That lowers to roughly:

```
LDA slot; ASL; TAX          ; X = slot * 2
LDA floor_enemy_spr_lo, X   ; ptr low byte
STA DPTR
LDA floor_enemy_spr_lo+1, X ; ptr high byte
STA DPTR+1
LDY frame
LDA (DPTR), Y               ; the value
```

That's three address-mode resources active at once (X is the abs,X index, Y is the (zp),Y offset, DPTR is the indirect base), plus the `slot * 2` scaling, plus the two separate `LDA` calls for the pointer bytes. And the same chain repeats for `floor_enemy_spr_hi`. Roughly 25 bytes per pointer-array access.

What if I recognize that `floor_enemy_spr_lo` is small (`N = 4`) and each of its entries is `AddressInit(some_named_static)` — a compile-time-known pointer? Then I can replace the indirect with a direct switch on the array index:

```
TAX (or already there): slot in X / A
CMP #0; BEQ case_0
CMP #1; BEQ case_1
CMP #2; BEQ case_2
; fallthrough is case_3
LDA floor_enemy_spr_s3_lo, Y    ; Y = frame
JMP end
case_0: LDA floor_enemy_spr_s0_lo, Y; JMP end
case_1: LDA floor_enemy_spr_s1_lo, Y; JMP end
case_2: LDA floor_enemy_spr_s2_lo, Y; JMP end
end:
```

Each case-arm is a single `LDA target_k, Y` — no DPTR, no `slot * 2`, and the X/Y conflict is gone (Y carries `frame` all the way through). The dispatch chain itself costs `(N-1) * 4` bytes for the CMP/BEQ checks, but the per-access savings dwarf that overhead at most useful array sizes.

The pass is a TAC-level transformation, running after the optimizer's main fixed-point and after SSA destruction:

```python
def dispatch_const_pointer_arrays(prog, symbols=None):
    pointer_arrays = _collect_pointer_arrays(prog)  # name → [target_0, ...]
    for fn in prog.functions:
        while True:
            chain = _find_one_chain(fn.instructions, pointer_arrays)
            if chain is None: break
            fn.instructions = _apply_dispatch(fn.instructions, chain, ...)
```

The recognition walks each `IndirectIndexedLoad`, finds the latest def of its pointer operand (must be an `IndexedLoad` from a known small pointer-array static), finds the def of THAT instruction's index (must be a `Multiply(_, 2)` or `LeftShift(_, 1)`), and verifies both intermediates are single-use. The transformation builds the dispatch and splices it in, removing the three chain instructions.

Running post-SSA-destruction is what makes the multi-case write to a shared dst tractable: in SSA proper, each case would need its own renamed dst and a Phi at the end label. After destruction, each case-arm just writes to the same `%val` and the downstream code reads it.

## Optimization 4: LICM-lite for loop-invariant constant stores

This is general-purpose loop-invariant code motion. It detects natural loops by back-edge identification, finds `Mov(Imm, Data|ZP)` and `LDA #c; STA M` pairs inside the body where the dst isn't otherwise written and no `Call` appears in the body, and hoists them to the preheader.

The no-`Call` constraint is what makes it not fire on our example: the loop calls `draw_sprite`. The conservative gate exists because a `Call` to a `zp_abi` callee writes to the callee's `__zpabi_<callee>_p<k>` slots, and the callee may further mutate them as locals. Hoisting `LDA #$01; STA __zpabi_draw_sprite_p0` out of the loop would leave whatever value draw_sprite last wrote there in place for the next iteration — wrong.

A smarter pass could narrow the analysis: if we know exactly which slots a callee touches, we can hoist writes to OTHER slots. But that requires interprocedural analysis I don't have yet, and the conservative version is already useful on no-call loops elsewhere.

## Composing them: the final output

With all four optimizations (the bug fix counts as "Opt 0"), c6502's output for `floor_enemy_draw` is now:

```
floor_enemy_draw:
   SUBROUTINE
   LDX   #$03                          ; slot pinned to X
.loop_start:
   LDA   enemy_flag,X
   BNE   .if_end
   JMP   .loop_continue
.if_end:
   LDY   enemy_col,X                   ; col_idx → Y (the new LDY abs,X opcode)
   LDA   proj_screen_col,Y             ; Y-pivot rewrite: was ,X
   STA   __local_b3
   LDA   proj_frame_idx,Y              ; was ,X
   TAY                                 ; Y = frame
   TXA
   CMP   #$00                          ; first dispatch — for spr_lo
   BEQ   .dispatch@0@case@0
   CMP   #$01
   BEQ   .dispatch@0@case@1
   CMP   #$02
   BEQ   .dispatch@0@case@2
   LDA   floor_enemy_spr_s3_lo,Y       ; fallthrough = slot 3
   STA   __local_b1
   JMP   .dispatch@0@end
.dispatch@0@case@0:
   LDA   floor_enemy_spr_s0_lo,Y
   STA   __local_b1
   JMP   .dispatch@0@end
   ; cases 1, 2 likewise
.dispatch@0@end:
   TXA
   CMP   #$00                          ; second dispatch — for spr_hi
   ; ... same shape ...
.dispatch@1@end:
   ; ... setup zp_abi args ...
   STX   __local_floor_enemy_draw_b4   ; save X across call
   JSR   draw_sprite
   LDX   __local_floor_enemy_draw_b4   ; restore X
.loop_continue:
   DEX                                 ; was: DEC __local_slot
   BPL   .loop_start
   RTS
```

This is structurally the hand-written code, with two non-structural deltas:

1. **Two separate dispatches** (one for `spr_lo`, one for `spr_hi`) instead of one combined dispatch. A pass could merge them via CSE on the predicate, but I haven't written it.
2. **No SMC.** The compiler stores the sprite address to a zp_abi param slot; the callee reads from there.

The first is a real optimization c6502 doesn't yet do. The second is fundamentally outside the compiler's model.

## Some thoughts on the process

A few patterns from this session that I want to write down for future me:

**Compiler bugs are often the visible end of a missing optimization.** The `LDX abs,X` bug was discovered as an assertion failure, but the real story was that my eligibility model was too coarse — it had one set where it needed two. The bug fix (disqualify IndexedData peers) was correct but lossy; the right fix was the refactor.

**The differential sim test was the lever.** I wrote a sim test that runs the compiled function under both `--optimize` and unoptimized pipelines and asserts they produce identical observable state for a battery of scenarios. Without that, the latent indirect-Y soundness bug in `loop_counter_to_x` would have escaped — it's the kind of thing that only manifests when one optimization's output happens to satisfy another optimization's input shape.

**Optimization composability is non-linear.** Each individual optimization here is modest. The per-HwReg eligibility frees Y for the col_idx Y-pivot. The Y-pivot frees X for the loop counter. The loop counter promotion only matters if the body doesn't have things that disqualify it — like the `LDA slot` for `slot * 2` — so the LDA-as-TXA rewrite matters. The const-pointer-array dispatch eliminates the `slot * 2` entirely AND eliminates the X-vs-Y conflict at the indirect deref. Each one alone is +5-10%; together they restructure the function.

**The SMC ceiling is real.** I can keep grinding to close the byte-count gap, but the structural ceiling — direct memory operands in the called function that the caller patches in place — isn't reachable without giving up on a typed code IR. That's fine: hobbyist 6502 programmers use SMC because their code is small and they have global perspective; my compiler can't, but it can get within ~6 bytes per call site of that without it.

## What's in the repo

The compiler is at https://github.com/XekriRedmane/c6502 — Python, MIT, single-TU compiles cleanly through dasm. Pre-runtime: programs assemble but don't link yet (no `mul16`, `divmod16`, FP helpers, etc.). The four optimizations from this session are commits `fcaae2d` through `dd076fd` on `main`.

The Drol example used here is `examples/floor_enemy_draw.c`. The sim differential test is `tests/test_floor_enemy_draw_sim.py`.
