# Title

Four optimizations to get my C-to-6502 compiler within striking distance of hand-written asm

# Body

I've been writing a C99 compiler that targets the 6502 (project's name is c6502, in Python). One of my test cases is a function from Drol — a 4-slot per-frame sprite-draw routine that dispatches through an array of sprite-pointer tables. The hand-written version is the kind of code real 6502 programmers write, so I treat the gap between my compiler's output and the hand-written reference as a backlog of optimizations to implement.

Here's the C:

```c
static const uint8_t *const floor_enemy_spr_lo[4] = {
    floor_enemy_spr_s0_lo, floor_enemy_spr_s1_lo,
    floor_enemy_spr_s2_lo, floor_enemy_spr_s3_lo,
};
static const uint8_t *const floor_enemy_spr_hi[4] = { /* same */ };

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

The hand-written reference does the `slot ∈ {0,1,2,3}` dispatch inline with `CPX / BNE` chains, keeps `slot` pinned to X across the whole loop, and patches `draw_sprite`'s `LDA` operand bytes with SMC. My compiler was generating ~50 bytes of DPTR-staged `(zp),Y` indirection per slot, with X getting reloaded from a ZP byte on every iteration.

Bug first, then four optimizations:

### Bug: `LDX abs,X`

The compile failed outright with `AssemblerError: unsupported Mov: IndexedData(name='enemy_col', offset=0, index=X()) -> Reg(reg=X())`. My HwReg-eligibility scan let a Pseudo whose def was `Mov(IndexedData(enemy_col, _, X), P)` get pinned to X, which would mean `LDX enemy_col,X` — which doesn't exist on the 6502.

For anyone who hasn't memorized the opcode table: the 6502 has `LDX abs,Y` (0xBE) and `LDY abs,X` (0xBC), but NOT `LDX abs,X` or `LDY abs,Y`. The same-index combinations of indexed loads don't exist, and neither do any `STX abs,X|Y` / `STY abs,X|Y` forms. So an index register can't be both the address index and the load/store target of the same instruction.

### Opt 1: per-HwReg eligibility (X and Y are independent)

The bug fix grew into a refactor. Previously the eligibility scan had a single `eligible` set — a Pseudo was either pinnable to X-or-Y or not. The asymmetric opcodes need a finer split: a Pseudo whose def is `Mov(IndexedData(...,X), P)` is Y-eligible (since `LDY abs,X` exists) but not X-eligible. Vice versa for IndexedData(...,Y). I split the set in two: `eligible_x` and `eligible_y` independently.

The downstream cross-transfer rewrite (which collapses `Mov(P, A); Mov(A, X)` chains followed by `LDA arr,X` into `LDA arr,Y` after Y-pinning P) gained a split fallback: when the rewrite would produce a same-index `Mov(IndexedData(...,X), Reg(X))`, split into `LDA arr,X; TAX` — one byte longer than the consolidated form, but the chain still drops, netting one byte saved.

### Opt 2: loop-counter promotion through `LDA M` uses

The counter-to-X pass already supported `STX M / JSR / LDX M` wrapping around calls inside a counter loop — that's how `slot` could be kept in X across `draw_sprite`. But the pass also bailed when the body had `LDA M` (e.g. for computing `slot * 2` as a byte offset into the const-pointer arrays). I extended the eligibility to accept `LDA M` and rewrite it to `TXA` at apply time, since `X = M` is the promotion invariant.

(Also fixed a latent soundness bug: the Y-pivot ranges inside the pass didn't check `Indirect` / `(zp),Y` operands, which read Y for their addressing mode. The pivot's `LDX → LDY` rewrite would silently clobber Y mid-range. With per-HwReg eligibility putting more values into Y, this started biting.)

### Opt 3: inline-switch dispatch for `static const T *const arr[N]`

This is the big one. The TAC pattern for `arr[slot][frame]` lowered to:

```
%scaled = LeftShift(slot, 1)            ; slot * 2
%ptr    = IndexedLoad(arr, %scaled)     ; 2-byte pointer load
%val    = IndirectIndexedLoad(%ptr, frame)
```

When `arr` is small (N ≤ 8) and every element is `&named_static`, I rewrite this to a CMP/BEQ dispatch on `slot`:

```
JumpIfCmp(Equal, slot, 0, case_0)
JumpIfCmp(Equal, slot, 1, case_1)
JumpIfCmp(Equal, slot, 2, case_2)
; fallthrough is case_3:
IndexedLoad(target_3, frame, %val)
Jump(end)
case_0: IndexedLoad(target_0, frame, %val); Jump(end)
case_1: IndexedLoad(target_1, frame, %val); Jump(end)
case_2: IndexedLoad(target_2, frame, %val); Jump(end)
end:
```

Runs after SSA destruction so the multiple case-arms can share the dst without Phi insertion. Each arm is a single `LDA target_k,Y` — no DPTR staging, no indirect-Y, no `slot * 2` scaling. The conflict between X (abs,X index for the table) and Y ((zp),Y offset for the deref) disappears.

### Opt 4: LICM-lite for loop-invariant constant stores

General-purpose: hoists `Mov(Imm, Data|ZP)` and `LDA #c; STA M` pairs out of natural loops when the dst isn't otherwise written in the body and no `Call` appears in the loop. Doesn't fire on this example (the loop calls `draw_sprite`), but it's a clean win on no-call loops elsewhere.

### Before and after

The original c6502 output for one slot:

```
LDA __local_b4
STA __local_b0
ASL __local_b0          ; slot * 2
LDX __local_b0
LDA floor_enemy_spr_lo,X
STA __local_b0          ; pointer low byte → DPTR
LDA floor_enemy_spr_lo+1,X
STA __local_b1          ; pointer high byte → DPTR+1
LDA (__local_b0),Y      ; (DPTR),Y where Y = frame
STA __local_b2          ; got the byte
; ... same dance for floor_enemy_spr_hi ...
```

The new output:

```
TXA                     ; slot is in X (counter promotion)
CMP #$00
BEQ case_0
CMP #$01
BEQ case_1
CMP #$02
BEQ case_2
; fallthrough: case_3
LDA floor_enemy_spr_s3_lo,Y     ; Y = frame
STA __local_b1
JMP end
case_0: LDA floor_enemy_spr_s0_lo,Y; STA b1; JMP end
case_1: LDA floor_enemy_spr_s1_lo,Y; STA b1; JMP end
case_2: LDA floor_enemy_spr_s2_lo,Y; STA b1; JMP end
end:
```

Plus the same dispatch for `floor_enemy_spr_hi`, and `STX zp / JSR draw_sprite / LDX zp` wrapping the call so `slot` survives in X. Structurally identical to the hand-written code, except for one thing:

### The one trick the compiler still can't do: SMC

The hand-written version patches the `LDA` operand bytes of the called `DRAW_SPRITE` routine in place (the `SMC_DS_SRC_LO / SMC_DS_SRC_HI` stores). The compiler can't do that — code is immutable in the IR. The compiler instead stores the sprite pointer to a zp_abi param slot that the callee reads. ~6 bytes' worth of overhead per call vs the SMC version.

If anyone has ideas on how to model SMC in a typed IR I'd love to hear them.

(Compiler is at https://github.com/XekriRedmane/c6502 — Python, MIT, ~30k loc, still pre-runtime so programs assemble but don't link yet.)
