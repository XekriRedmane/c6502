SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v3	EQU	$26
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_0	EQU	$83
__local_check__0	EQU	$84
__local_check__1	EQU	$85
__local_check__2	EQU	$86
__local_check__3	EQU	$87
__local_check__4	EQU	$88
__local_check__5	EQU	$89
__local_check__6	EQU	$8A
__local_check__7	EQU	$8B
__local_check__8	EQU	$8C
__local_check__9	EQU	$8D

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.bss local=false zero=true
; item v2 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v3 size=1 at=auto def=true init=$80 weight=4
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v0+3
   BNE   .jcmp_skip@0
   LDA   v0+2
   BNE   .jcmp_skip@0
   LDA   v0+1
   BNE   .jcmp_skip@0
   LDA   v0
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v0
   STA   __local_check__9
   LDA   v0+1
   STA   __local_check__8
   LDA   v0+2
   STA   __local_check__7
   LDA   v0+3
   STA   __local_check__6
   LDA   #$02
   SEC
   SBC   v4
   LDA   #$00
   SBC   v4+1
   LDA   #$00
   SBC   v4+2
   LDA   #$00
   SBC   v4+3
   LDA   #$00
   ROL
   STA   __local_check__5
   LDA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   __local_check__4
   STA   __local_check__3
   LDA   v0
   STA   HARGS
   LDA   v0+1
   STA   HARGS+1
   LDA   v0+2
   STA   HARGS+2
   LDA   v0+3
   STA   HARGS+3
   LDA   v4
   STA   HARGS+4
   LDA   v4+1
   STA   HARGS+5
   LDA   v4+2
   STA   HARGS+6
   LDA   v4+3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   HARGS+4
   LDA   HARGS+9
   STA   HARGS+5
   LDA   HARGS+10
   STA   HARGS+6
   LDA   HARGS+11
   STA   HARGS+7
   LDA   __local_check__5
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_check__4
   STA   HARGS+2
   LDA   __local_check__3
   STA   HARGS+3
   JSR   udivmod32
   LDA   __local_check__9
   CLC
   ADC   HARGS+8
   STA   __local_check__2
   LDA   __local_check__8
   ADC   HARGS+9
   STA   __local_check__1
   LDA   __local_check__7
   ADC   HARGS+10
   STA   __local_check__0
   LDA   __local_check__6
   ADC   HARGS+11
   BNE   .jcmp_skip@3
   LDA   __local_check__0
   BNE   .jcmp_skip@3
   LDA   __local_check__1
   BNE   .jcmp_skip@3
   LDA   __local_check__2
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@3:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v2+1
   BNE   .jcmp_skip@4
   LDA   v2
   CMP   #$08
   BEQ   .if_end@3
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   v5
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__r_0
   LDA   v2
   SEC
   SBC   v5
   LDA   v2+1
   SBC   __local_check__r_0
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__0
   LDA   v3
   CLC
   SBC   #$45
   LDA   #$00
   ROL
   CLC
   ADC   __local_check__0
   STA   __local_check__r_0
   LDA   #$00
   ADC   #$00
   STA   __local_check__0
   SEC
   LDA   #$00
   SBC   #$80
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__2
   LDA   #$00
   SEC
   SBC   v0
   LDA   #$00
   SBC   v0+1
   LDA   #$00
   SBC   v0+2
   LDA   #$80
   SBC   v0+3
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   __local_check__2
   EOR   __local_check__1
   TAX
   BEQ   .shift_done@11
.shift_loop@10:
   ASL   __local_check__r_0
   ROL   __local_check__0
   DEX
   BNE   .shift_loop@10
.shift_done@11:
   LDA   __local_check__0
   BNE   .jcmp_skip@12
   LDA   __local_check__r_0
   CMP   #$02
   BEQ   .if_end@4
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   v3
   CMP   #$80
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   v1
   ORA   v1+1
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   v3
   CMP   #$80
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
   JMP   .if_end@1

main:
   SUBROUTINE

   JMP   check

v0:
   DC.L  $00000001

v1:
   DS.B  2

v2:
   DC.W  $0008

v4:
   DC.L  $2BF3F8EA

v5:
   DC.B  $01

__zp_init:
   SUBROUTINE

   LDA   #$80
   STA   v3
   RTS
