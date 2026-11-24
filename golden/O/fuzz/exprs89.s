SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
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

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v0 section=.bss local=false zero=true
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.bss local=false zero=true
; item v4 section=.data local=false zero=false
; item v5 section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=29
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v0
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   v1
   ORA   v1+1
   ORA   v1+2
   ORA   v1+3
   BEQ   .lnot_true@0
   LDA   #$00
   BEQ   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   CLC
   ADC   v2
   STA   __local_check__1
   LDA   #$00
   ADC   v2+1
   STA   __local_check__0
   LDA   __local_check__1
   SEC
   SBC   v0
   LDA   __local_check__0
   SBC   #$00
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@3
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@3:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   v2
   EOR   #$FF
   STA   __local_check__5
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   LDA   __local_check__5
   SEC
   SBC   v3
   STA   __local_check__1
   LDA   __local_check__4
   SBC   v3+1
   STA   __local_check__0
   LDA   __local_check__3
   SBC   #$00
   STA   __local_check__3
   BCS   .carry_inc@0
   DEC   __local_check__2
.carry_inc@0:
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __local_check__3
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __local_check__1
   CMP   #$FF
   BEQ   .if_end@3
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   JSR   check@run0
   LDA   #$00
   STA   HARGS+4
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$02
   STY   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   STA   __local_check__8
   LDA   HARGS+25
   STA   __local_check__7
   LDA   HARGS+26
   STA   __local_check__6
   LDA   HARGS+27
   STA   __local_check__5
   LDA   HARGS+28
   STA   __local_check__4
   JSR   check@run0
   LDA   #$07
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   __local_check__8
   SEC
   SBC   HARGS+12
   STA   __local_check__3
   LDA   __local_check__7
   SBC   HARGS+13
   STA   __local_check__2
   LDA   __local_check__6
   SBC   HARGS+14
   STA   __local_check__1
   LDA   __local_check__5
   SBC   HARGS+15
   STA   __local_check__0
   BCS   .carry_inc@1
   DEC   __local_check__4
.carry_inc@1:
   LDA   __local_check__3
   STA   __local_check__9
   LDA   __local_check__2
   STA   __local_check__8
   LDA   __local_check__1
   STA   __local_check__7
   LDA   __local_check__0
   STA   __local_check__6
   LDA   __local_check__4
   AND   #$01
   STA   __local_check__5
   LDA   v2+1
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_check__2
   STA   __local_check__1
   AND   #$01
   STA   __local_check__0
   LDA   v2
   SEC
   SBC   __local_check__9
   LDA   v2+1
   SBC   __local_check__8
   LDA   __local_check__2
   SBC   __local_check__7
   LDA   __local_check__1
   SBC   __local_check__6
   LDA   __local_check__0
   SBC   __local_check__5
   BCC   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   #$00
   SEC
   SBC   v3
   STA   __local_check__4
   LDA   #$00
   SBC   v3+1
   STA   __local_check__3
   LDA   #$00
   SBC   #$00
   STA   __local_check__2
   LDA   #$80
   SBC   #$00
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   AND   #$01
   STA   __local_check__0
   LDA   __local_check__4
   SEC
   SBC   v4
   LDA   __local_check__3
   SBC   v4+1
   LDA   __local_check__2
   SBC   v4+2
   LDA   __local_check__1
   SBC   v4+3
   LDA   __local_check__0
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@9
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   v1
   CLC
   SBC   v4
   LDA   v1+1
   SBC   v4+1
   LDA   v1+2
   SBC   v4+2
   LDA   v1+3
   SBC   v4+3
   LDA   #$00
   ROL
   STA   HARGS+2
   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   STA   HARGS+3
   JSR   sdivmod16
   LDA   v4
   CLC
   SBC   #$01
   LDA   v4+1
   SBC   #$00
   LDA   v4+2
   SBC   #$00
   LDA   v4+3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   HARGS+5
   BNE   .thread@0
   LDA   HARGS+4
   CMP   __local_check__0
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@13
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@8
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   v1
   CMP   #$36
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

check@run0:
   SUBROUTINE

   LDA   v4
   STA   HARGS
   LDA   v4+1
   STA   HARGS+1
   LDA   v4+2
   STA   HARGS+2
   LDA   v4+3
   STA   HARGS+3
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DS.B  1

v1:
   DC.L  $23FD7E36

v2:
   DC.W  $8000

v3:
   DS.B  2

v4:
   DC.L  $00000009

v5:
   DS.B  4
