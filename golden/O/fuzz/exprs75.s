SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_check__fail	EQU	$81
__local_check__r_3	EQU	$82
__local_check__r_2	EQU	$83
__local_check__r_1	EQU	$84
__local_check__r_0	EQU	$85
__local_check__0	EQU	$86
__local_check__1	EQU	$87
__local_check__2	EQU	$88

; @zp-link-meta-begin
; def check params= locals=__local_check__fail,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v0 section=.data local=false zero=false
; item v1 section=.data local=false zero=false
; item v2 section=.data local=false zero=false
; item v3 section=.bss local=false zero=true
; item v4 section=.bss local=false zero=true
; item v5 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v4+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__r_3
   STA   __local_check__fail
   LDA   v4+1
   ORA   v3+1
   STA   __local_check__r_2
   LDA   __local_check__r_3
   ORA   v3+2
   STA   __local_check__r_3
   LDA   __local_check__fail
   ORA   v3+3
   STA   __local_check__fail
   LDA   #$00
   STA   __local_check__0
   LDA   __local_check__r_2
   AND   #$80
   STA   __local_check__r_0
   LDA   __local_check__r_3
   STA   __local_check__r_1
   LDA   __local_check__fail
   STA   __local_check__r_2
   LDA   v2
   STA   HARGS
   LDA   v2+1
   STA   HARGS+1
   LDA   v0
   STA   HARGS+2
   LDA   v0+1
   STA   HARGS+3
   JSR   mul16
   LDA   __local_check__0
   STA   HARGS
   LDA   __local_check__r_0
   STA   HARGS+1
   LDA   __local_check__r_1
   STA   HARGS+2
   LDA   __local_check__r_2
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail
   LDX   #$00
.if_end@0:
   LDA   v1
   BMI   .sx_neg@2
   TXA
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__r_3
   LDA   v1
   CLC
   ADC   v5
   STA   __local_check__r_0
   LDA   __local_check__r_3
   ADC   v5+1
   STA   __local_check__r_1
   LDA   v4
   ORA   v4+1
   BNE   .or_true@1
   LDA   #$00
   STA   __local_check__r_2
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   __local_check__r_2
.or_end@2:
   LDA   __local_check__r_1
   BNE   .thread@0
   LDA   __local_check__r_0
   CMP   __local_check__r_2
   BEQ   .cmp_true@4
.thread@0:
   TXA
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   SEC
   SBC   v5
   TXA
   SBC   v5+1
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BPL   .if_end@3
   LDA   __local_check__fail
   ORA   #$02
   STA   __local_check__fail
.if_end@3:
   LDA   v3
   ORA   v3+1
   ORA   v3+2
   ORA   v3+3
   BEQ   .if_end@4
   LDA   __local_check__fail
   ORA   #$04
   STA   __local_check__fail
.if_end@4:
   LDA   v4
   ORA   v4+1
   BEQ   .if_end@7
   LDA   v3
   CLC
   SBC   v0
   LDA   v3+1
   SBC   v0+1
   LDA   v3+2
   SBC   #$00
   LDA   v3+3
   SBC   #$00
   TXA
   ROL
   STA   __local_check__2
   TXA
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_check__1
   STA   __local_check__0
   LDA   v4+1
   BMI   .sx_neg@10
   TXA
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   v2
   CLC
   ADC   v4
   STA   __local_check__r_0
   LDA   v2+1
   ADC   v4+1
   STA   __local_check__r_1
   LDA   v2+2
   ADC   __local_check__r_2
   STA   __local_check__r_2
   LDA   v2+3
   ADC   __local_check__r_3
   STA   __local_check__r_3
   LDA   __local_check__r_0
   SEC
   SBC   __local_check__2
   LDA   __local_check__r_1
   SBC   #$00
   LDA   __local_check__r_2
   SBC   __local_check__1
   LDA   __local_check__r_3
   SBC   __local_check__0
   BCC   .if_end@7
   LDA   __local_check__fail
   ORA   #$08
   STA   __local_check__fail
.if_end@7:
   LDA   v5+1
   ASL
   TXA
   ROL
   EOR   #$01
   STA   __local_check__r_1
   LDA   #$02
   SEC
   SBC   v3
   TXA
   SBC   v3+1
   TXA
   SBC   v3+2
   TXA
   SBC   v3+3
   TXA
   ROL
   STA   __local_check__r_3
   TXA
   BNE   .cmp_true@12
   LDA   __local_check__r_1
   CMP   __local_check__r_3
   BNE   .cmp_true@12
   TXA
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   __local_check__r_3
   TXA
   BNE   .jcmp_skip@15
   LDA   __local_check__r_3
   CMP   #$01
   BEQ   .if_end@8
.jcmp_skip@15:
   LDA   __local_check__fail
   ORA   #$10
   STA   __local_check__fail
.if_end@8:
   LDA   v4
   SEC
   SBC   #$FF
   LDA   v4+1
   SBC   #$00
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   TXA
   ROL
   STA   __local_check__2
   TXA
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   __local_check__1
   STA   __local_check__0
   LDA   v0
   SEC
   SBC   v2
   LDA   v0+1
   SBC   v2+1
   TXA
   SBC   v2+2
   TXA
   SBC   v2+3
   TXA
   ROL
   STA   __local_check__r_1
   TXA
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   __local_check__r_2
   STA   __local_check__r_3
   LDA   #$FF
   EOR   __local_check__r_1
   STA   __local_check__r_0
   LDA   #$FF
   EOR   __local_check__r_2
   STA   __local_check__r_2
   LDA   #$FF
   EOR   __local_check__r_3
   STA   __local_check__r_3
   LDA   __local_check__2
   SEC
   SBC   __local_check__r_0
   TXA
   SBC   #$FE
   LDA   __local_check__1
   SBC   __local_check__r_2
   LDA   __local_check__0
   SBC   __local_check__r_3
   BVC   .jcmp_novf@21
   EOR   #$80
.jcmp_novf@21:
   BPL   .if_end@9
   LDA   __local_check__fail
   ORA   #$20
   STA   __local_check__fail
.if_end@9:
   LDA   v0+1
   CMP   #$FF
   BNE   .jcmp_skip@22
   LDA   v0
   CMP   #$FF
   BEQ   .if_end@10
.jcmp_skip@22:
   LDA   __local_check__fail
   ORA   #$40
   STA   __local_check__fail
.if_end@10:
   LDA   v2+3
   CMP   #$FF
   BNE   .jcmp_skip@23
   LDA   v2+2
   CMP   #$FF
   BNE   .jcmp_skip@23
   LDA   v2+1
   CMP   #$FF
   BNE   .jcmp_skip@23
   LDA   v2
   CMP   #$FF
   BEQ   .if_end@11
.jcmp_skip@23:
   LDA   __local_check__fail
   ORA   #$80
   STA   __local_check__fail
.if_end@11:
   LDA   __local_check__fail
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail
   LDX   #$00
   JMP   .if_end@0

main:
   SUBROUTINE

   JMP   check

v0:
   DC.W  $FFFF

v1:
   DC.B  $01

v2:
   DC.L  $FFFFFFFF

v3:
   DS.B  4

v4:
   DS.B  2

v5:
   DC.W  $8000
