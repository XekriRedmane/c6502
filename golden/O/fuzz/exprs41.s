SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v4	EQU	$26
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__0	EQU	$83
__local_check__1	EQU	$84
__local_check__2	EQU	$85
__local_check__3	EQU	$86
__local_check__4	EQU	$87
__local_check__5	EQU	$88
__local_check__6	EQU	$89
__local_check__7	EQU	$8A
__local_check__8	EQU	$8B
__local_check__9	EQU	$8C
__local_check__10	EQU	$8D

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run1 section=.text local=true zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.bss local=false zero=true
; item v3 section=.bss local=false zero=true
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v4 size=1 at=auto def=true init=$0F weight=4
; runtime hargs_size=32 hargs_need=30
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v4
   CMP   #$0F
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v2
   CLC
   ADC   #$FF
   STA   __local_check__3
   LDA   v2+1
   ADC   #$FF
   STA   __local_check__2
   LDA   v2+2
   ADC   #$FF
   STA   __local_check__1
   LDA   v2+3
   ADC   #$3F
   AND   #$3F
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   #$7F
   LDA   __local_check__2
   SBC   #$00
   LDA   __local_check__1
   SBC   #$00
   LDA   __local_check__0
   SBC   #$00
   BCS   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__0
   LDA   #$FF
   CLC
   ADC   v3
   STA   __local_check__1
   LDA   #$FF
   ADC   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_check__1
   CMP   #$FF
   BEQ   .if_end@2
.jcmp_skip@2:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   v5+1
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__9
   STA   __local_check__8
   STA   __local_check__7
   AND   #$03
   STA   __local_check__6
   LDA   v2
   SEC
   SBC   v0
   STA   __local_check__5
   LDA   v2+1
   SBC   v0+1
   STA   __local_check__4
   LDA   v2+2
   SBC   v0+2
   STA   __local_check__3
   LDA   v2+3
   SBC   v0+3
   STA   __local_check__2
   LDA   #$00
   SBC   v0+4
   STA   __local_check__1
   LDA   #$00
   SBC   v0+5
   AND   #$03
   STA   __local_check__0
   LDA   __local_check__5
   ORA   __local_check__4
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .and_false@3
   LDA   #$01
   STA   __local_check__5
   LDA   #$00
   BEQ   .and_end@4
.and_false@3:
   LDA   #$00
   STA   __local_check__5
.and_end@4:
   ORA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   AND   #$03
   STA   __local_check__0
   LDA   v5
   SEC
   SBC   __local_check__5
   LDA   v5+1
   SBC   #$00
   LDA   __local_check__9
   SBC   __local_check__3
   LDA   __local_check__8
   SBC   __local_check__2
   LDA   __local_check__7
   SBC   __local_check__1
   LDA   __local_check__6
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@7
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   #$FF
   EOR   v1
   STA   __local_check__10
   LDA   #$FF
   EOR   v1+1
   STA   __local_check__9
   LDA   #$00
   EOR   v1+2
   STA   __local_check__8
   LDA   #$00
   EOR   v1+3
   STA   __local_check__7
   LDA   #$00
   EOR   v1+4
   STA   __local_check__6
   LDA   #$00
   EOR   v1+5
   STA   __local_check__5
   LDA   v4
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__2
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_check__1
   AND   #$3F
   CMP   v2+3
   BNE   .thread@0
   LDA   __local_check__1
   CMP   v2+2
   BNE   .thread@0
   LDA   __local_check__2
   CMP   v2+1
   BNE   .thread@0
   LDA   v4
   CMP   v2
   BEQ   .cmp_true@12
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   __local_check__4
   LDA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   LDX   __local_check__4
   STX   HARGS+8
   LDX   #$00
   JSR   check@run0
   LDA   __local_check__10
   STA   HARGS
   LDA   __local_check__9
   STA   HARGS+1
   LDA   __local_check__8
   STA   HARGS+2
   LDA   __local_check__7
   STA   HARGS+3
   LDA   __local_check__6
   STA   HARGS+4
   LDA   __local_check__5
   JSR   check@run1
   JSR   mul64
   LDA   HARGS+16
   ORA   HARGS+17
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   v4
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_check__4
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDX   v4
   STX   HARGS+8
   LDX   __local_check__4
   JSR   check@run0
   LDA   v1
   STA   HARGS
   LDA   v1+1
   STA   HARGS+1
   LDA   v1+2
   STA   HARGS+2
   LDA   v1+3
   STA   HARGS+3
   LDA   v1+4
   STA   HARGS+4
   LDA   v1+5
   JSR   check@run1
   JSR   udivmod64
   LDA   HARGS+29
   BNE   .jcmp_skip@21
   LDA   HARGS+28
   BNE   .jcmp_skip@21
   LDA   HARGS+27
   BNE   .jcmp_skip@21
   LDA   HARGS+26
   BNE   .jcmp_skip@21
   LDA   HARGS+25
   BNE   .jcmp_skip@21
   LDA   HARGS+24
   CMP   #$02
   BEQ   .if_end@7
.jcmp_skip@21:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   #$FF
   STA   __local_check__0
   STA   __local_check__1
   LDX   v2
   BEQ   .shift_done@23
.shift_loop@22:
   ASL   __local_check__0
   ROL   __local_check__1
   DEX
   BNE   .shift_loop@22
.shift_done@23:
   LDA   __local_check__0
   ORA   __local_check__1
   BEQ   .if_end@10
   LDA   v1
   ORA   v1+1
   BEQ   .if_end@10
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDA   v4
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_check__2
   LDA   v2
   SEC
   SBC   v2
   LDA   v2+1
   SBC   v2+1
   LDA   v2+2
   SBC   v2+2
   LDA   v2+3
   SBC   v2+3
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   v4
   EOR   __local_check__0
   STA   __local_check__1
   LDA   __local_check__2
   STA   __local_check__0
   SEC
   LDA   #$00
   SBC   __local_check__1
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@26
   LDA   __local_check__1
   CMP   #$F2
   BEQ   .if_end@11
.jcmp_skip@26:
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

check@run1:
   SUBROUTINE

   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+14
   STA   HARGS+15
   RTS

check@run0:
   SUBROUTINE

   STX   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   STA   HARGS+12
   AND   #$03
   STA   HARGS+13
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DC.B  $0F
   DS.B  5

v1:
   DS.B  5
   DC.B  $02

v2:
   DS.B  4

v3:
   DS.B  1

v5:
   DC.W  $8000

__zp_init:
   SUBROUTINE

   LDA   #$0F
   STA   v4
   RTS
