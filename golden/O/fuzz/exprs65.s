SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1	EQU	$80
__zpabi_check__v2_0	EQU	$81
__zpabi_check__v2_1	EQU	$82
__zpabi_check__v3_0	EQU	$83
__zpabi_check__v3_1	EQU	$84
__zpabi_check__v3_2	EQU	$85
__zpabi_check__v3_3	EQU	$86
__zpabi_check__v4	EQU	$87
__zpabi_check__v5_0	EQU	$88
__zpabi_check__v5_1	EQU	$89
__local_check__fail_1	EQU	$8B
__local_check__fail_0	EQU	$8C
__local_check__0	EQU	$8D
__local_check__1	EQU	$8E
__local_check__2	EQU	$8F
__local_check__3	EQU	$90
__local_check__v0_1	EQU	$92
__local_check__v0_0	EQU	$93

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__v0_1,__local_check__v0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   LDA   __zpabi_check__v1
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDY   #$FF
   STY   HARGS
   INY
   STY   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   LDA   __local_check__v0_0
   STA   HARGS+4
   STX   HARGS+5
   TYA
   STA   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   __zpabi_check__v2_0
   SEC
   SBC   #$02
   LDA   __zpabi_check__v2_1
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   HARGS+6
   STA   HARGS+7
   LDA   HARGS+12
   STA   HARGS
   LDA   HARGS+13
   STA   HARGS+1
   LDA   HARGS+14
   STA   HARGS+2
   LDA   HARGS+15
   STA   HARGS+3
   LDA   __local_check__0
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   JSR   mul32
   LDA   HARGS+8
   EOR   #$FF
   STA   __local_check__3
   LDA   HARGS+9
   EOR   #$FF
   STA   __local_check__2
   LDA   HARGS+10
   EOR   #$FF
   STA   __local_check__1
   LDA   HARGS+11
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_check__3
   CMP   #$FF
   BEQ   .if_end@1
.jcmp_skip@2:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   #$07
   ORA   __zpabi_check__v2_0
   EOR   #$FF
   STA   __local_check__1
   LDA   __zpabi_check__v2_1
   EOR   #$FF
   STA   __local_check__0
   LDA   #$FF
   CLC
   ADC   __local_check__1
   STA   __local_check__3
   LDA   #$7F
   ADC   __local_check__0
   STA   __local_check__2
   LDA   #$00
   ADC   #$00
   STA   __local_check__1
   LDA   #$00
   ADC   #$00
   BNE   .jcmp_skip@3
   LDA   __local_check__1
   BNE   .jcmp_skip@3
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   __local_check__3
   CMP   #$F7
   BEQ   .if_end@2
.jcmp_skip@3:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v4
   CMP   #$FF
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v5_1
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __zpabi_check__v5_0
   CMP   #$FF
   BEQ   .vthread@0
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
   LDA   __zpabi_check__v5_0
.vthread@0:
   AND   __local_check__v0_0
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   AND   __local_check__v0_1
   STA   __local_check__0
   LDA   __local_check__1
   SEC
   SBC   #$FF
   LDA   __local_check__0
   SBC   #$FF
   LDA   #$00
   ROL
   TAX
   LDA   #$07
   STA   __local_check__0
   CPX   #$00
   BEQ   .shift_done@6
.shift_loop@5:
   LSR   __local_check__0
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   __local_check__0
   CMP   #$07
   BEQ   .if_end@7
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

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_check__v1
   LDA   #$00
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   LDA   #$80
   STA   __zpabi_check__v2_1
   LDA   #$0E
   STA   __zpabi_check__v3_0
   LDA   #$FF
   STA   __zpabi_check__v4
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   LDX   #$00
   LDA   #$01
   JMP   check
