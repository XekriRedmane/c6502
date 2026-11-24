SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v2	EQU	$80
__zpabi_check__v3	EQU	$81
__zpabi_check__v4_0	EQU	$82
__zpabi_check__v4_1	EQU	$83
__zpabi_check__v5_0	EQU	$84
__zpabi_check__v5_1	EQU	$85
__local_check__fail	EQU	$87
__local_check__0	EQU	$88
__local_check__1	EQU	$89
__local_check__2	EQU	$8A
__local_check__3	EQU	$8B
__local_check__v1	EQU	$8C
__local_check__v0	EQU	$8D

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1,__zpabi_check__v2,__zpabi_check__v3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__v1,__local_check__v0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   STX   __local_check__v1
   CMP   #$FF
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail
   LDX   #$00
.if_end@0:
   LDA   __zpabi_check__v2
   CMP   #$01
   BEQ   .if_end@4
   LDA   __local_check__fail
   ORA   #$04
   STA   __local_check__fail
.if_end@4:
   LDA   __local_check__v1
   CMP   #$31
   BEQ   .if_end@5
   LDA   __local_check__fail
   ORA   #$08
   STA   __local_check__fail
.if_end@5:
   LDA   __zpabi_check__v3
   CMP   #$66
   BEQ   .if_end@6
   LDA   __local_check__fail
   ORA   #$10
   STA   __local_check__fail
.if_end@6:
   TXA
   SEC
   SBC   __local_check__v0
   STA   __local_check__1
   TXA
   SBC   #$00
   STA   __local_check__0
   TXA
   AND   __local_check__1
   STA   __local_check__3
   TXA
   AND   __local_check__0
   STA   __local_check__2
   BMI   .sx_neg@0
   TXA
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v4_0
   AND   __local_check__3
   STA   __local_check__3
   LDA   __zpabi_check__v4_1
   AND   __local_check__2
   STA   __local_check__2
   TXA
   AND   __local_check__1
   STA   __local_check__1
   TXA
   AND   __local_check__0
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@7
   LDA   __local_check__fail
   ORA   #$20
   STA   __local_check__fail
.if_end@7:
   LDA   #$01
   CLC
   SBC   #$FF
   TXA
   SBC   #$00
   TXA
   SBC   #$00
   TXA
   SBC   #$00
   TXA
   ROL
   STA   __local_check__0
   LDA   __zpabi_check__v4_1
   BNE   .thread@0
   LDA   __zpabi_check__v4_0
   CMP   __local_check__0
   BEQ   .cmp_true@2
.thread@0:
   TXA
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   STA   __local_check__0
   LDA   __local_check__v1
   EOR   __local_check__0
   STA   __local_check__1
   TXA
   BNE   .jcmp_skip@5
   LDA   __local_check__1
   CMP   #$31
   BEQ   .if_end@8
.jcmp_skip@5:
   LDA   __local_check__fail
   ORA   #$40
   STA   __local_check__fail
.if_end@8:
   LDA   __local_check__v0
   CMP   #$FF
   BEQ   .if_end@9
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
   JMP   .if_end@0

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_check__v2
   LDA   #$66
   STA   __zpabi_check__v3
   LDA   #$FF
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   LDX   #$31
   JMP   check
