SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2_0	EQU	$82
__zpabi_check__v2_1	EQU	$83
__zpabi_check__v3_0	EQU	$84
__zpabi_check__v3_1	EQU	$85
__zpabi_check__v4_0	EQU	$86
__zpabi_check__v4_1	EQU	$87
__zpabi_check__v5	EQU	$88
__local_check__fail_1	EQU	$8A
__local_check__fail_0	EQU	$8B
__local_check__0	EQU	$8C
__local_check__1	EQU	$8D
__local_check__2	EQU	$8E
__local_check__3	EQU	$8F
__local_check__4	EQU	$90
__local_check__5	EQU	$91
__local_check__7	EQU	$93
__local_check__v0	EQU	$94

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__v0,__local_check__8,__local_check__9 indirect=false in_cycle=false param_regs=A,,,,,,,,,
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
   LDA   __zpabi_check__v3_0
   SEC
   SBC   #$01
   STA   __local_check__fail_0
   LDA   __zpabi_check__v3_1
   SBC   #$00
   STA   __local_check__fail_1
   LDA   #$01
   SEC
   SBC   __local_check__fail_0
   LDA   #$00
   SBC   __local_check__fail_1
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   __zpabi_check__v3_1
   LSR
   STA   __local_check__1
   LDA   __zpabi_check__v3_0
   ROR
   LSR   __local_check__1
   ROR
   STA   __local_check__0
   SEC
   LDA   #$00
   SBC   __zpabi_check__v3_0
   STA   __local_check__fail_1
   LDA   #$00
   SBC   __zpabi_check__v3_1
   STA   __local_check__fail_0
   LDA   __local_check__0
   EOR   __local_check__fail_1
   STA   __local_check__fail_1
   LDA   __local_check__1
   EOR   __local_check__fail_0
   STA   __local_check__fail_0
   LDA   __local_check__2
   ORA   __local_check__fail_1
   STA   __local_check__fail_1
   LDA   __local_check__fail_0
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_check__fail_1
   CMP   #$FB
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@4:
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v1_0
   LDA   __zpabi_check__v3_1
   SBC   __zpabi_check__v1_1
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   __local_check__1
   LDA   __local_check__2
   EOR   #$07
   STA   __local_check__7
   LDA   __local_check__1
   STA   __local_check__5
   STA   __local_check__4
   LDA   __zpabi_check__v5
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__3
   BNE   .thread@0
   LDA   __zpabi_check__v5
   CMP   __local_check__v0
   BEQ   .cmp_true@5
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__7
   CLC
   SBC   __local_check__2
   LDA   #$00
   SBC   #$00
   LDA   __local_check__5
   SBC   __local_check__1
   LDA   __local_check__4
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@10
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@6:
   LDA   __zpabi_check__v4_1
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v4_0
   CMP   #$03
   BEQ   .if_end@7
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v5
   CMP   #$68
   BEQ   .vthread@0
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
   LDA   __zpabi_check__v5
.vthread@0:
   SEC
   SBC   __zpabi_check__v4_0
   LDA   __local_check__3
   SBC   __zpabi_check__v4_1
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   JSR   check@run0
   SEC
   SBC   #$FF
   LDA   #$00
   SBC   #$7F
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   JSR   check@run0
   STA   __local_check__0
   LDA   __zpabi_check__v4_0
   STA   HARGS
   LDA   __zpabi_check__v4_1
   STA   HARGS+1
   LDA   __zpabi_check__v2_0
   STA   HARGS+2
   LDA   __zpabi_check__v2_1
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   SEC
   SBC   __zpabi_check__v4_0
   LDA   HARGS+5
   SBC   __zpabi_check__v4_1
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   JSR   check@run0
   TAX
   LDA   #$00
   STA   __local_check__1
   CPX   #$00
   BEQ   .shift_done@16
.shift_loop@15:
   LDA   __local_check__1
   ASL
   ROR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@15
.shift_done@16:
   LDA   __local_check__0
   ORA   __local_check__1
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@9:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@4

check@run0:
   SUBROUTINE

   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v4_1
   LDA   #$06
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v3_0
   LDA   #$03
   STA   __zpabi_check__v4_0
   LDA   #$68
   STA   __zpabi_check__v5
   LDA   #$01
   JMP   check
