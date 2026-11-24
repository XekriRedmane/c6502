SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_check__fail_1	EQU	$81
__local_check__fail_0	EQU	$82
__local_check__r_1	EQU	$83
__local_check__r_0	EQU	$84
__local_check__0	EQU	$85
__local_check__1	EQU	$86
__local_check__2	EQU	$87
__local_check__3	EQU	$88
__local_check__4	EQU	$89
__local_check__5	EQU	$8A
__local_check__6	EQU	$8B
__local_check__7	EQU	$8C
__local_check__9	EQU	$8E

; @zp-link-meta-begin
; def check params= locals=__local_check__fail_1,__local_check__fail_0,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run1 section=.text local=true zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item v0 section=.bss local=false zero=true
; item v1 section=.bss local=false zero=true
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   JSR   check@run0
   BEQ   .and_false@0
   LDA   v4+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   v4
   ORA   v4+1
   ORA   __local_check__fail_0
   ORA   __local_check__fail_1
   BEQ   .and_false@0
   LDA   #$01
   STA   HARGS
   LDA   #$00
   BEQ   .and_end@1
.and_false@0:
   LDA   #$00
   STA   HARGS
.and_end@1:
   STA   HARGS+1
   LDA   v2
   STA   HARGS+2
   LDA   v2+1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+5
   BNE   .jcmp_skip@2
   LDA   HARGS+4
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   v3
   STA   HARGS
   STA   HARGS+4
   LDA   v3+1
   STA   HARGS+1
   STA   HARGS+5
   LDA   v3+2
   STA   HARGS+2
   STA   HARGS+6
   LDA   v3+3
   STA   HARGS+3
   STA   HARGS+7
   JSR   mul32
   LDA   #$00
   AND   HARGS+8
   STA   __local_check__1
   LDA   #$00
   AND   HARGS+9
   STA   __local_check__0
   LDA   #$00
   AND   HARGS+10
   STA   __local_check__r_0
   LDA   #$00
   AND   HARGS+11
   STA   __local_check__r_1
   LDA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__r_0
   ORA   __local_check__r_1
   BEQ   .if_end@3
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@3:
   LDA   v4+1
   CMP   #$80
   BNE   .jcmp_skip@3
   LDA   v4
   BEQ   .if_end@4
.jcmp_skip@3:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   v3+3
   CMP   v3+3
   BNE   .cmp_true@4
   LDA   v3+2
   CMP   v3+2
   BNE   .cmp_true@4
   LDA   v3+1
   CMP   v3+1
   BNE   .cmp_true@4
   LDA   v3
   CMP   v3
   BNE   .cmp_true@4
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   STA   __local_check__4
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __local_check__3
   STA   __local_check__2
   SEC
   LDA   #$00
   SBC   v5
   STA   __local_check__1
   LDA   #$00
   SBC   v5+1
   STA   __local_check__0
   LDA   #$00
   SBC   v5+2
   STA   __local_check__r_0
   LDA   #$00
   SBC   v5+3
   STA   __local_check__r_1
   LDA   __local_check__4
   SEC
   SBC   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   LDA   __local_check__3
   SBC   __local_check__r_0
   LDA   __local_check__2
   SBC   __local_check__r_1
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   STA   HARGS+4
   LDA   v0
   STA   HARGS
   LDA   v0+1
   STA   HARGS+1
   LDA   v0+2
   STA   HARGS+2
   LDA   v0+3
   STA   HARGS+3
   JSR   asl32
   LDA   HARGS+5
   ORA   HARGS+6
   ORA   HARGS+7
   ORA   HARGS+8
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   v1
   BEQ   .if_end@8
   LDA   v3
   BEQ   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   LDA   v0
   ORA   v0+1
   ORA   v0+2
   ORA   v0+3
   BEQ   .and_false@11
   JSR   check@run1
   BEQ   .and_false@11
   LDA   #$01
   BNE   .and_end@12
.and_false@11:
   LDA   #$00
.and_end@12:
   SEC
   SBC   #$77
   LDA   #$00
   SBC   #$CC
   BVC   .jcmp_novf@10
   EOR   #$80
.jcmp_novf@10:
   BMI   .vthread@0
   LDA   v3+3
   CMP   v5+3
   BNE   .thread@0
   LDA   v3+2
   CMP   v5+2
   BNE   .thread@0
   LDA   v3+1
   CMP   v5+1
   BNE   .thread@0
   LDA   v3
   CMP   v5
   BEQ   .cmp_true@11
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   __local_check__r_0
   STA   __local_check__r_1
   LDA   v5
   SEC
   SBC   __local_check__0
   LDA   v5+1
   SBC   #$00
   LDA   v5+2
   SBC   __local_check__r_0
   LDA   v5+3
   SBC   __local_check__r_1
   BVC   .jcmp_novf@16
   EOR   #$80
.jcmp_novf@16:
   BMI   .if_end@13
.vthread@0:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@13:
   LDA   v1
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_check__0
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_check__r_0
   STA   __local_check__r_1
   LDA   v1
   SEC
   SBC   v5
   LDA   __local_check__0
   SBC   v5+1
   LDA   __local_check__r_0
   SBC   v5+2
   LDA   __local_check__r_1
   SBC   v5+3
   BVC   .jcmp_novf@21
   EOR   #$80
.jcmp_novf@21:
   BPL   .or_true@17
   JSR   check@run0
   BEQ   .check@ssa_block@21
   JSR   check@run1
   BNE   .or_true@17
.check@ssa_block@21:
   LDA   #$00
   STA   __local_check__9
   JMP   .or_end@18
.or_true@17:
   LDA   #$01
   STA   __local_check__9
   LDA   #$00
.or_end@18:
   ORA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   __local_check__7
   STA   __local_check__6
   LDA   v0
   CLC
   ADC   v3
   STA   __local_check__5
   LDA   v0+1
   ADC   v3+1
   STA   __local_check__4
   LDA   v0+2
   ADC   v3+2
   STA   __local_check__3
   LDA   v0+3
   ADC   v3+3
   STA   __local_check__2
   LDA   v4
   ORA   v4+1
   BEQ   .and_false@21
   LDA   v2
   ORA   v2+1
   BEQ   .and_false@21
   LDA   #$01
   STA   __local_check__1
   LDA   #$00
   BEQ   .and_end@22
.and_false@21:
   LDA   #$00
   STA   __local_check__1
.and_end@22:
   ORA   #$00
   BPL   .sx_done@25
   LDA   #$FF
.sx_done@25:
   STA   __local_check__r_0
   STA   __local_check__r_1
   LDA   __local_check__5
   SEC
   SBC   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   SBC   #$00
   STA   __local_check__0
   LDA   __local_check__3
   SBC   __local_check__r_0
   STA   __local_check__r_0
   LDA   __local_check__2
   SBC   __local_check__r_1
   STA   __local_check__r_1
   LDA   __local_check__9
   CLC
   ADC   __local_check__1
   STA   __local_check__1
   LDA   #$00
   ADC   __local_check__0
   STA   __local_check__0
   LDA   __local_check__7
   ADC   __local_check__r_0
   STA   __local_check__r_0
   LDA   __local_check__6
   ADC   __local_check__r_1
   CMP   #$80
   BNE   .jcmp_skip@26
   LDA   __local_check__r_0
   BNE   .jcmp_skip@26
   LDA   __local_check__0
   BNE   .jcmp_skip@26
   LDA   __local_check__1
   BEQ   .if_end@23
.jcmp_skip@26:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@23:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@2

check@run1:
   SUBROUTINE

   LDA   v5
   ORA   v5+1
   ORA   v5+2
   ORA   v5+3
   RTS

check@run0:
   SUBROUTINE

   LDA   v3
   ORA   v3+1
   ORA   v3+2
   ORA   v3+3
   RTS

main:
   SUBROUTINE

   JMP   check

v0:
   DS.B  4

v1:
   DS.B  1

v2:
   DC.W  $0001

v3:
   DC.L  $80000000

v4:
   DC.W  $8000

v5:
   DC.L  $FFFFFFFF
