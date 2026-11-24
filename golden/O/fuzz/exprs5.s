SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v0_0	EQU	$80
__zpabi_check__v0_1	EQU	$81
__zpabi_check__v0_2	EQU	$82
__zpabi_check__v0_3	EQU	$83
__zpabi_check__v1_0	EQU	$84
__zpabi_check__v1_1	EQU	$85
__zpabi_check__v1_2	EQU	$86
__zpabi_check__v1_3	EQU	$87
__zpabi_check__v2_0	EQU	$88
__zpabi_check__v2_1	EQU	$89
__zpabi_check__v2_2	EQU	$8A
__zpabi_check__v2_3	EQU	$8B
__zpabi_check__v3_0	EQU	$8C
__zpabi_check__v3_1	EQU	$8D
__zpabi_check__v4_0	EQU	$8E
__zpabi_check__v4_1	EQU	$8F
__zpabi_check__v4_2	EQU	$90
__zpabi_check__v4_3	EQU	$91
__zpabi_check__v5	EQU	$92
__local_check__fail	EQU	$94
__local_check__0	EQU	$95
__local_check__1	EQU	$96
__local_check__2	EQU	$97

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5 locals=__local_check__fail,__local_check__0,__local_check__1,__local_check__2 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   ORA   __zpabi_check__v1_2
   ORA   __zpabi_check__v1_3
   BNE   .or_true@2
   LDA   #$00
   STA   __local_check__2
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   STA   __local_check__2
   LDA   #$00
.or_end@3:
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_check__fail
   ORA   __local_check__2
   ORA   __local_check__fail
   BEQ   .lnot_true@2
   LDA   #$00
   BEQ   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   STA   __local_check__fail
   LDA   #$00
   BNE   .jcmp_skip@4
   LDA   __local_check__fail
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@4:
   LDA   #$04
   STA   __local_check__fail
   LDX   #$00
.if_end@4:
   LDA   __zpabi_check__v2_0
   LSR
   TXA
   ROR
   STA   __local_check__2
   LDA   __zpabi_check__v2_0
   AND   __zpabi_check__v3_0
   STA   __local_check__1
   LDA   __zpabi_check__v2_1
   AND   __zpabi_check__v3_1
   STA   __local_check__0
   TXA
   SEC
   SBC   __local_check__1
   STA   __local_check__1
   LDA   __local_check__2
   SBC   __local_check__0
   ORA   __local_check__1
   BEQ   .if_end@6
   LDA   __local_check__fail
   ORA   #$10
   STA   __local_check__fail
.if_end@6:
   TXA
   CMP   __zpabi_check__v1_3
   BNE   .vthread@0
   CMP   __zpabi_check__v1_2
   BNE   .vthread@0
   CMP   __zpabi_check__v1_1
   BNE   .vthread@0
   LDA   #$07
   CMP   __zpabi_check__v1_0
   BEQ   .cmp_true@5
.vthread@0:
.cmp_true@5:
   SEC
   TXA
   SBC   #$80
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   TXA
   ROL
   STA   __local_check__2
   TXA
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   __local_check__0
   ORA   __local_check__2
   ORA   __local_check__0
   BEQ   .if_end@7
   LDA   __local_check__fail
   ORA   #$20
   STA   __local_check__fail
.if_end@7:
   LDA   __zpabi_check__v2_3
   CMP   #$EC
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v2_2
   CMP   #$1D
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v2_1
   CMP   #$7D
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v2_0
   CMP   #$A0
   BEQ   .if_end@9
.jcmp_skip@11:
   LDA   __local_check__fail
   ORA   #$80
   STA   __local_check__fail
.if_end@9:
   LDA   __local_check__fail
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail
   LDX   #$00
   JMP   .if_end@4

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   LDA   #$80
   STA   __zpabi_check__v0_3
   LDA   #$A0
   STA   __zpabi_check__v2_0
   LDA   #$7D
   STA   __zpabi_check__v2_1
   LDA   #$1D
   STA   __zpabi_check__v2_2
   LDA   #$EC
   STA   __zpabi_check__v2_3
   LDA   #$0F
   STA   __zpabi_check__v3_0
   LDA   #$0E
   STA   __zpabi_check__v4_0
   LDA   #$07
   STA   __zpabi_check__v5
   JMP   check
