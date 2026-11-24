SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v0_0	EQU	$80
__zpabi_check__v0_1	EQU	$81
__zpabi_check__v0_2	EQU	$82
__zpabi_check__v0_3	EQU	$83
__zpabi_check__v0_4	EQU	$84
__zpabi_check__v0_5	EQU	$85
__zpabi_check__v0_6	EQU	$86
__zpabi_check__v1	EQU	$87
__zpabi_check__v2	EQU	$88
__zpabi_check__v3_0	EQU	$89
__zpabi_check__v3_1	EQU	$8A
__zpabi_check__v3_2	EQU	$8B
__zpabi_check__v3_3	EQU	$8C
__zpabi_check__v4_0	EQU	$8D
__zpabi_check__v4_1	EQU	$8E
__zpabi_check__v5	EQU	$8F
__local_check__fail_1	EQU	$91
__local_check__fail_0	EQU	$92
__local_check__0	EQU	$93
__local_check__1	EQU	$94
__local_check__2	EQU	$95
__local_check__3	EQU	$96
__local_check__4	EQU	$97
__local_check__5	EQU	$98
__local_check__6	EQU	$99
__local_check__7	EQU	$9A
__local_check__8	EQU	$9B
__local_check__9	EQU	$9C
__local_check__10	EQU	$9D
__local_check__11	EQU	$9E

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v0_4,__zpabi_check__v0_5,__zpabi_check__v0_6,__zpabi_check__v1,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v2
   CMP   #$80
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__11
   LDA   __zpabi_check__v1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__10
   STA   __local_check__9
   STA   __local_check__7
   LDA   __zpabi_check__v3_0
   AND   __zpabi_check__v1
   STA   HARGS
   LDA   __zpabi_check__v3_1
   AND   __local_check__10
   STA   HARGS+1
   LDA   __zpabi_check__v3_2
   AND   __local_check__9
   STA   HARGS+2
   LDA   __zpabi_check__v3_3
   AND   __local_check__7
   STA   HARGS+3
   LDA   __zpabi_check__v0_0
   STA   HARGS+4
   JSR   lsr32
   LDA   #$00
   SEC
   SBC   HARGS+5
   LDA   #$00
   SBC   HARGS+6
   LDA   #$00
   SBC   HARGS+7
   LDA   #$80
   SBC   HARGS+8
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@4
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __zpabi_check__v5
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__8
   LDA   __zpabi_check__v5
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_check__6
   STA   __local_check__5
   STA   __local_check__4
   STA   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v5
   STA   HARGS
   JSR   check@run0
   JSR   mul32
   LDA   __zpabi_check__v3_0
   CLC
   SBC   HARGS+8
   LDA   __zpabi_check__v3_1
   SBC   HARGS+9
   LDA   __zpabi_check__v3_2
   SBC   HARGS+10
   LDA   __zpabi_check__v3_3
   SBC   HARGS+11
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v3_0
   CLC
   SBC   __local_check__2
   LDA   __zpabi_check__v3_1
   SBC   #$00
   LDA   __zpabi_check__v3_2
   SBC   __local_check__1
   LDA   __zpabi_check__v3_3
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@11
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v5
   STA   HARGS
   LDA   __local_check__6
   STA   HARGS+1
   LDA   __local_check__5
   STA   HARGS+2
   LDA   __local_check__4
   STA   HARGS+3
   JSR   check@run0
   JSR   udivmod32
   LDA   HARGS+12
   STA   HARGS
   LDA   HARGS+13
   STA   HARGS+1
   LDA   HARGS+14
   STA   HARGS+2
   LDA   HARGS+15
   STA   HARGS+3
   LDA   __zpabi_check__v1
   STA   HARGS+4
   LDA   __local_check__10
   STA   HARGS+5
   LDA   __local_check__9
   STA   HARGS+6
   LDA   __local_check__7
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   EOR   #$FF
   STA   __local_check__3
   LDA   HARGS+13
   EOR   #$FF
   STA   __local_check__2
   LDA   HARGS+14
   EOR   #$FF
   STA   __local_check__1
   LDA   HARGS+15
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   __local_check__3
   CMP   #$FF
   BEQ   .if_end@6
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   __zpabi_check__v0_6
   ASL
   LDA   #$00
   ROL
   TAX
   LDA   __zpabi_check__v5
   STA   __local_check__0
   LDA   __local_check__8
   STA   __local_check__1
   CPX   #$00
   BEQ   .shift_done@14
.shift_loop@13:
   LDA   __local_check__1
   ASL
   ROR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@13
.shift_done@14:
   LDA   __local_check__1
   BNE   .jcmp_skip@15
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   #$FF
   CMP   __zpabi_check__v0_6
   BNE   .thread@0
   CMP   __zpabi_check__v0_5
   BNE   .thread@0
   CMP   __zpabi_check__v0_4
   BNE   .thread@0
   CMP   __zpabi_check__v0_3
   BNE   .thread@0
   CMP   __zpabi_check__v0_2
   BNE   .thread@0
   CMP   __zpabi_check__v0_1
   BNE   .thread@0
   CMP   __zpabi_check__v0_0
   BEQ   .cmp_true@16
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   __local_check__1
   LDA   __zpabi_check__v1
   CLC
   SBC   __zpabi_check__v3_0
   LDA   __local_check__10
   SBC   __zpabi_check__v3_1
   LDA   __local_check__9
   SBC   __zpabi_check__v3_2
   LDA   __local_check__7
   SBC   __zpabi_check__v3_3
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __local_check__1
   BNE   .if_end@8
   LDA   #$00
   BNE   .if_end@8
   LDA   __local_check__3
   CMP   __local_check__0
   BNE   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   STA   __local_check__4
   STA   __local_check__3
   LDA   __zpabi_check__v0_0
   EOR   __zpabi_check__v2
   STA   __local_check__7
   LDA   __zpabi_check__v0_1
   STA   __local_check__6
   LDA   __zpabi_check__v0_2
   EOR   __local_check__4
   STA   __local_check__5
   LDA   __zpabi_check__v0_3
   EOR   __local_check__3
   STA   __local_check__4
   LDA   __zpabi_check__v2
   BEQ   .and_false@9
   LDA   #$01
   STA   __local_check__3
   LDA   #$00
   STA   __local_check__2
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   STA   __local_check__3
   STA   __local_check__2
.and_end@10:
   LDA   __zpabi_check__v1
   STA   HARGS
   LDA   __local_check__11
   STA   HARGS+1
   LDA   __zpabi_check__v5
   STA   HARGS+2
   LDA   __local_check__8
   STA   HARGS+3
   JSR   sdivmod16
   LDA   __local_check__3
   EOR   HARGS+6
   STA   __local_check__3
   LDA   __local_check__2
   EOR   HARGS+7
   STA   __local_check__2
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__7
   CLC
   SBC   __local_check__3
   LDA   __local_check__6
   SBC   __local_check__2
   LDA   __local_check__5
   SBC   __local_check__1
   LDA   __local_check__4
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@25
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@11
.jcmp_skip@25:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@11:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@0

check@run0:
   SUBROUTINE

   LDA   __zpabi_check__v3_0
   STA   HARGS+4
   LDA   __zpabi_check__v3_1
   STA   HARGS+5
   LDA   __zpabi_check__v3_2
   STA   HARGS+6
   LDA   __zpabi_check__v3_3
   STA   HARGS+7
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v0_4
   STA   __zpabi_check__v0_5
   STA   __zpabi_check__v0_6
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   STA   __zpabi_check__v4_0
   LDA   #$80
   STA   __zpabi_check__v1
   STA   __zpabi_check__v2
   STA   __zpabi_check__v4_1
   LDA   #$01
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v5
   JMP   check
