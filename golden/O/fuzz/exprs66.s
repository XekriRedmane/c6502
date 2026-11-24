SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v2	EQU	$26
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_1	EQU	$83
__local_check__r_0	EQU	$84
__local_check__0	EQU	$85
__local_check__1	EQU	$86
__local_check__2	EQU	$87
__local_check__3	EQU	$88
__local_check__4	EQU	$89
__local_check__7	EQU	$8C
__local_check__8	EQU	$8D
__local_check__9	EQU	$8E
__local_check__11	EQU	$90

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.bss local=false zero=true
; item v5 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp v2 size=1 at=auto def=true init=$FF weight=3
; runtime hargs_size=32 hargs_need=21
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v4
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v3+1
   BNE   .jcmp_skip@0
   LDA   v3
   CMP   #$08
   BEQ   .if_end@1
.jcmp_skip@0:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v5
   ORA   v5+1
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v3
   ORA   v3+1
   BEQ   .and_false@4
   LDA   v4
   BEQ   .and_false@4
   LDA   #$01
   STA   __local_check__1
   LDA   #$00
   BEQ   .and_end@5
.and_false@4:
   LDA   #$00
   STA   __local_check__1
.and_end@5:
   ORA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   __local_check__r_0
   STA   __local_check__r_1
   LDA   #$00
   AND   __local_check__1
   STA   __local_check__2
   LDA   #$00
   AND   __local_check__r_0
   STA   __local_check__0
   LDA   #$00
   AND   __local_check__r_1
   STA   __local_check__r_0
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDX   __local_check__2
   STX   __local_check__11
   LDX   __local_check__0
   STX   __local_check__9
   LDX   __local_check__r_0
   STX   __local_check__8
   AND   #$7F
   STA   __local_check__7
   LDA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   __local_check__4
   STA   __local_check__3
   LDA   v2
   AND   v1
   STA   HARGS+8
   LDA   #$00
   AND   v1+1
   STA   HARGS+9
   LDA   __local_check__4
   AND   v1+2
   STA   HARGS+10
   LDA   __local_check__3
   AND   v1+3
   STA   HARGS+11
   LDA   __local_check__11
   STA   HARGS
   LDA   __local_check__9
   STA   HARGS+2
   LDA   __local_check__8
   STA   HARGS+3
   LDA   __local_check__7
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+12
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   v2
   SEC
   SBC   v2
   LDA   #$00
   ROL
   STA   __local_check__r_1
   LDA   #$00
   BNE   .jcmp_skip@7
   LDA   __local_check__r_1
   CMP   #$01
   BEQ   .if_end@8
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_check__0
   STA   __local_check__r_0
   AND   #$7F
   STA   __local_check__r_1
   LDA   v0
   CLC
   SBC   #$FF
   LDA   #$00
   SBC   #$FF
   LDA   __local_check__0
   SBC   #$FF
   LDA   __local_check__r_0
   SBC   #$7F
   LDA   __local_check__r_1
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__r_1
   LDA   #$00
   SEC
   SBC   __local_check__r_1
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   TAX
   LDA   #$02
   STA   __local_check__r_1
   CPX   #$00
   BEQ   .shift_done@12
.shift_loop@11:
   LSR   __local_check__r_1
   DEX
   BNE   .shift_loop@11
.shift_done@12:
   LDA   __local_check__r_1
   CMP   #$02
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
   JMP   .if_end@0

main:
   SUBROUTINE

   JMP   check

v0:
   DC.B  $06

v1:
   DC.L  $00000001

v3:
   DC.W  $0008

v4:
   DS.B  1

v5:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$FF
   STA   v2
   RTS
