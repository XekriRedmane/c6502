SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2	EQU	$82
__zpabi_check__v3	EQU	$83
__zpabi_check__v4	EQU	$84
__zpabi_check__v5_0	EQU	$85
__zpabi_check__v5_1	EQU	$86
__local_check__fail_1	EQU	$88
__local_check__fail_0	EQU	$89
__local_check__0	EQU	$8A
__local_check__1	EQU	$8B
__local_check__3	EQU	$8D
__local_check__5	EQU	$8F
__local_check__6	EQU	$90
__local_check__v0	EQU	$91

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__v0 indirect=false in_cycle=false param_regs=A,,,,,,, changes=__zpabi_check__v2
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   LDA   __zpabi_check__v4
   CMP   #$03
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   SEC
   SBC   __zpabi_check__v2
   STA   HARGS
   JSR   check@run0
   STA   HARGS+1
   LDA   #$FF
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   __local_check__v0
   CLC
   SBC   __zpabi_check__v5_0
   LDA   #$00
   SBC   __zpabi_check__v5_1
   JSR   check@run0
   JSR   check@run0
   JSR   check@run0
   JSR   check@run0
   LDA   #$00
   ROL
   ORA   HARGS+4
   STA   __local_check__1
   LDA   HARGS+5
   BNE   .jcmp_skip@0
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@0:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __local_check__v0
   CMP   #$80
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v4
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_check__0
   LDA   __zpabi_check__v4
   EOR   __zpabi_check__v4
   STA   __local_check__1
   LDA   __local_check__0
   EOR   __local_check__0
   STA   __local_check__0
   LDA   __zpabi_check__v5_0
   AND   __local_check__1
   STA   __local_check__6
   LDA   __zpabi_check__v5_1
   AND   __local_check__0
   STA   __local_check__5
   SEC
   LDA   #$01
   SBC   __zpabi_check__v2
   JSR   check@run0
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__0
   LDA   __local_check__5
   BNE   .if_end@3
   LDA   __local_check__6
   CMP   __local_check__0
   BNE   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v5_1
   BNE   .jcmp_skip@9
   LDA   __zpabi_check__v5_0
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v3
   BEQ   .and_false@5
   LDA   #$02
   STA   __local_check__3
   JMP   .and_end@6
.and_false@5:
   LDA   #$01
   STA   __local_check__3
.and_end@6:
   LDA   __zpabi_check__v1_0
   STA   __local_check__0
   LDA   __zpabi_check__v1_1
   STA   __local_check__1
   LDX   __zpabi_check__v5_0
   BEQ   .shift_done@11
.shift_loop@10:
   ASL   __local_check__0
   ROL   __local_check__1
   DEX
   BNE   .shift_loop@10
.shift_done@11:
   LDA   __local_check__0
   ORA   __local_check__1
   BEQ   .and_false@7
   LDA   #$01
   STA   __local_check__1
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   STA   __local_check__1
.and_end@8:
   LDA   __local_check__3
   AND   __local_check__1
   LDY   #$00
   ORA   #$00
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BEQ   .lnot_true@12
   TYA
   JMP   .lnot_end@13
.lnot_true@12:
   LDA   #$01
.lnot_end@13:
   STA   __local_check__1
   LDA   #$00
   STA   __local_check__0
   LDX   __zpabi_check__v2
   BEQ   .shift_done@15
.shift_loop@14:
   LSR   __local_check__0
   DEX
   BNE   .shift_loop@14
.shift_done@15:
   LDA   __local_check__1
   EOR   __local_check__0
   CMP   #$01
   BEQ   .if_end@10
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDA   __local_check__v0
   CMP   #$80
   BEQ   .if_end@11
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

   LDA   #$00
   SBC   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_check__v2
   STA   __zpabi_check__v5_0
   LDA   #$97
   STA   __zpabi_check__v3
   LDA   #$03
   STA   __zpabi_check__v4
   LDA   #$00
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v5_1
   LDA   #$80
   JMP   check
