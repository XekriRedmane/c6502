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
__zpabi_check__v1	EQU	$86
__zpabi_check__v2	EQU	$87
__zpabi_check__v3_0	EQU	$88
__zpabi_check__v3_1	EQU	$89
__zpabi_check__v3_2	EQU	$8A
__zpabi_check__v3_3	EQU	$8B
__zpabi_check__v3_4	EQU	$8C
__zpabi_check__v3_5	EQU	$8D
__zpabi_check__v4_0	EQU	$8E
__zpabi_check__v4_1	EQU	$8F
__zpabi_check__v5_0	EQU	$90
__zpabi_check__v5_1	EQU	$91
__zpabi_check__v5_2	EQU	$92
__zpabi_check__v5_3	EQU	$93
__zpabi_check__v5_4	EQU	$94
__zpabi_check__v5_5	EQU	$95
__local_check__fail_1	EQU	$97
__local_check__fail_0	EQU	$98
__local_check__0	EQU	$99
__local_check__1	EQU	$9A
__local_check__2	EQU	$9B
__local_check__3	EQU	$9C
__local_check__4	EQU	$9D

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v0_4,__zpabi_check__v0_5,__zpabi_check__v1,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v3_4,__zpabi_check__v3_5,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3,__zpabi_check__v5_4,__zpabi_check__v5_5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v1
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_check__3
   LDA   #$00
   BEQ   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_check__3
.and_end@1:
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_check__0
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   ORA   __local_check__3
   ORA   __local_check__0
   ORA   __local_check__fail_0
   ORA   __local_check__fail_1
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   __zpabi_check__v1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__4
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v1
   LDA   __zpabi_check__v3_1
   SBC   __local_check__4
   LDA   __zpabi_check__v3_2
   SBC   __local_check__3
   LDA   __zpabi_check__v3_3
   SBC   __local_check__2
   LDA   __zpabi_check__v3_4
   SBC   __local_check__1
   LDA   __zpabi_check__v3_5
   SBC   __local_check__0
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__2
   LDA   __zpabi_check__v1
   STA   HARGS
   LDA   __local_check__4
   STA   HARGS+1
   LDA   __zpabi_check__v4_0
   STA   HARGS+2
   LDA   __zpabi_check__v4_1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   SEC
   SBC   __local_check__2
   LDA   HARGS+5
   SBC   #$00
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_check__0
   ORA   __local_check__2
   ORA   __local_check__0
   BEQ   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v1
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v4_1
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   __zpabi_check__v4_0
   CMP   #$FF
   BEQ   .if_end@5
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   #$80
   AND   __zpabi_check__v4_0
   STA   __local_check__1
   LDA   __zpabi_check__v4_1
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__1
   CMP   #$80
   BEQ   .if_end@6
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@2

main:
   SUBROUTINE

   LDA   #$05
   STA   __zpabi_check__v0_0
   LDA   #$9A
   STA   __zpabi_check__v3_0
   LDA   #$91
   STA   __zpabi_check__v3_1
   LDA   #$B0
   STA   __zpabi_check__v3_2
   LDA   #$22
   STA   __zpabi_check__v3_3
   LDA   #$2B
   STA   __zpabi_check__v3_4
   LDA   #$07
   STA   __zpabi_check__v3_5
   LDA   #$FF
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   LDA   #$06
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v0_4
   STA   __zpabi_check__v0_5
   STA   __zpabi_check__v1
   STA   __zpabi_check__v2
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   STA   __zpabi_check__v5_4
   STA   __zpabi_check__v5_5
   JMP   check
