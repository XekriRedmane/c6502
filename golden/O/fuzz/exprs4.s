SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2	EQU	$82
__zpabi_check__v3_0	EQU	$83
__zpabi_check__v3_1	EQU	$84
__zpabi_check__v4	EQU	$85
__zpabi_check__v5_0	EQU	$86
__zpabi_check__v5_1	EQU	$87
__local_check__fail_1	EQU	$89
__local_check__fail_0	EQU	$8A
__local_check__0	EQU	$8B
__local_check__1	EQU	$8C
__local_check__3	EQU	$8E
__local_check__6	EQU	$91
__local_check__v0	EQU	$92

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__v0 indirect=false in_cycle=false param_regs=A,,,,,,,,
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
   LDA   __zpabi_check__v2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__6
   LDA   __zpabi_check__v5_1
   JSR   check@run0
   EOR   #$01
   STA   __local_check__fail_0
   LDA   __zpabi_check__v2
   EOR   __local_check__fail_0
   ORA   __local_check__6
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __local_check__v0
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v2
   LDA   __zpabi_check__v3_1
   SBC   __local_check__6
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   JSR   check@run0
   STA   __local_check__0
   LDA   __zpabi_check__v5_1
   BNE   .thread@0
   LDA   __zpabi_check__v5_0
   CMP   __local_check__0
   BEQ   .cmp_true@3
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+3
   LDA   __local_check__v0
   STA   HARGS+2
   JSR   mul16
   LDA   HARGS+4
   ORA   HARGS+5
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v2
   CMP   #$01
   BEQ   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   #$00
   CMP   __zpabi_check__v1_1
   BNE   .cmp_end@7
   LDA   __local_check__v0
   CMP   __zpabi_check__v1_0
   BEQ   .cmp_true@6
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   HARGS+4
   LDA   __zpabi_check__v1_0
   STA   HARGS
   LDA   __zpabi_check__v1_1
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   JSR   lsr32
   LDA   HARGS+5
   SEC
   SBC   __zpabi_check__v1_0
   LDA   HARGS+6
   SBC   __zpabi_check__v1_1
   LDA   HARGS+7
   SBC   #$00
   LDA   HARGS+8
   SBC   #$00
   BCS   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v3_0
   STA   HARGS
   LDA   __zpabi_check__v3_1
   STA   HARGS+1
   LDA   #$07
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   STA   HARGS
   LDA   HARGS+5
   STA   HARGS+1
   LDA   __zpabi_check__v1_0
   STA   HARGS+2
   LDA   __zpabi_check__v1_1
   STA   HARGS+3
   JSR   udivmod16
   LDA   __zpabi_check__v4
   ORA   __zpabi_check__v3_0
   STA   __local_check__3
   LDA   __local_check__v0
   EOR   __zpabi_check__v3_0
   STA   __local_check__1
   LDA   #$00
   EOR   __zpabi_check__v3_1
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   __local_check__1
   LDA   __zpabi_check__v3_1
   SBC   __local_check__0
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   JSR   check@run0
   ORA   HARGS+6
   STA   __local_check__1
   LDA   HARGS+7
   CMP   #$1D
   BNE   .jcmp_skip@10
   LDA   __local_check__1
   CMP   #$47
   BEQ   .if_end@6
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   __zpabi_check__v2
   STA   HARGS
   STA   HARGS+2
   LDA   __local_check__6
   STA   HARGS+1
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+5
   BMI   .jcmp_skip@13
   BNE   .jcmp_skip@13
   LDA   HARGS+4
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@7:
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

   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$97
   STA   __zpabi_check__v1_0
   LDA   #$7A
   STA   __zpabi_check__v1_1
   LDA   #$01
   STA   __zpabi_check__v2
   LDA   #$EA
   STA   __zpabi_check__v3_0
   LDA   #$CC
   STA   __zpabi_check__v3_1
   LDA   #$80
   STA   __zpabi_check__v4
   LDA   #$9D
   STA   __zpabi_check__v5_0
   LDA   #$C7
   STA   __zpabi_check__v5_1
   LDA   #$00
   JMP   check
