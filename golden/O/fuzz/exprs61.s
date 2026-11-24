SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
v0	EQU	$26
__local_check__fail	EQU	$81
__local_check__r_6	EQU	$82
__local_check__r_5	EQU	$83
__local_check__r_4	EQU	$84
__local_check__r_3	EQU	$85
__local_check__r_2	EQU	$86
__local_check__r_1	EQU	$87
__local_check__r_0	EQU	$88
__local_check__0	EQU	$89
__local_check__1	EQU	$8A
__local_check__2	EQU	$8B
__local_check__3	EQU	$8C
__local_check__4	EQU	$8D
__local_check__5	EQU	$8E
__local_check__6	EQU	$8F

; @zp-link-meta-begin
; def check params= locals=__local_check__fail,__local_check__r_6,__local_check__r_5,__local_check__r_4,__local_check__r_3,__local_check__r_2,__local_check__r_1,__local_check__r_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; item v1 section=.bss local=false zero=true
; item v2 section=.data local=false zero=false
; item v3 section=.data local=false zero=false
; item v4 section=.data local=false zero=false
; item v5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp v0 size=1 at=auto def=true init=$00 weight=3
; runtime hargs_size=32 hargs_need=16
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   v5+1
   STA   HARGS+1
   LDA   v5+2
   STA   HARGS+2
   LDA   v5+3
   STA   HARGS+3
   LDA   v5+4
   STA   HARGS+4
   LDA   v5+5
   STA   HARGS+5
   LDA   v5+6
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   LDA   v5
   STA   HARGS
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+15
   AND   #$7F
   STA   __local_check__r_0
   LDA   v4
   ORA   v4+1
   BNE   .or_true@0
   LDA   v0
   BNE   .or_true@0
   LDA   #$00
   STA   __local_check__r_1
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_check__r_1
   LDA   #$00
.or_end@1:
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_check__r_3
   STA   __local_check__r_4
   STA   __local_check__r_5
   STA   __local_check__r_6
   AND   #$7F
   STA   __local_check__fail
   LDA   __local_check__r_1
   SEC
   SBC   HARGS+9
   LDA   #$00
   SBC   HARGS+10
   LDA   __local_check__r_3
   SBC   HARGS+11
   LDA   __local_check__r_4
   SBC   HARGS+12
   LDA   __local_check__r_5
   SBC   HARGS+13
   LDA   __local_check__r_6
   SBC   HARGS+14
   LDA   __local_check__fail
   SBC   __local_check__r_0
   LDA   #$00
   ROL
   STA   __local_check__r_5
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   __local_check__r_1
   STA   __local_check__r_2
   LSR
   LDA   __local_check__r_1
   ROR
   STA   __local_check__r_3
   LDA   #$00
   ROR
   STA   __local_check__r_6
   LDA   __local_check__r_5
   ROR
   STA   __local_check__r_4
   LDA   #$00
   ROR
   SEC
   SBC   __local_check__r_5
   STA   __local_check__r_5
   LDA   __local_check__r_4
   SBC   #$00
   STA   __local_check__r_4
   LDA   __local_check__r_6
   SBC   __local_check__r_1
   STA   __local_check__r_6
   LDA   __local_check__r_3
   SBC   __local_check__r_2
   STA   __local_check__fail
   LDA   __local_check__r_5
   ORA   __local_check__r_4
   ORA   __local_check__r_6
   ORA   __local_check__fail
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail
   LDX   #$00
.if_end@2:
   LDA   v4+1
   BMI   .sx_neg@4
   TXA
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_check__r_2
   STA   __local_check__r_3
   TXA
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_check__r_5
   STA   __local_check__r_6
   LDA   v3
   SEC
   SBC   v4
   TXA
   SBC   v4+1
   LDA   __local_check__r_5
   SBC   __local_check__r_2
   LDA   __local_check__r_6
   SBC   __local_check__r_3
   BCC   .if_end@3
   LDA   __local_check__fail
   ORA   #$02
   STA   __local_check__fail
.if_end@3:
   LDA   v3
   CMP   #$80
   BEQ   .if_end@4
   LDA   __local_check__fail
   ORA   #$04
   STA   __local_check__fail
.if_end@4:
   LDA   v5+6
   BNE   .jcmp_skip@8
   LDA   v5+5
   BNE   .jcmp_skip@8
   LDA   v5+4
   BNE   .jcmp_skip@8
   LDA   v5+3
   BNE   .jcmp_skip@8
   LDA   v5+2
   BNE   .jcmp_skip@8
   LDA   v5+1
   BNE   .jcmp_skip@8
   LDA   v5
   CMP   #$0A
   BEQ   .if_end@5
.jcmp_skip@8:
   LDA   __local_check__fail
   ORA   #$08
   STA   __local_check__fail
.if_end@5:
   LDA   v0
   BMI   .sx_neg@9
   TXA
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_check__r_6
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   AND   #$7F
   STA   __local_check__1
   LDA   #$01
   ORA   __local_check__r_6
   STA   __local_check__0
   LDA   v5
   EOR   #$FF
   STA   __local_check__r_6
   LDA   v5+1
   EOR   #$FF
   STA   __local_check__r_0
   LDA   v5+2
   EOR   #$FF
   STA   __local_check__r_1
   LDA   v5+3
   EOR   #$FF
   STA   __local_check__r_2
   LDA   v5+4
   EOR   #$FF
   STA   __local_check__r_3
   LDA   v5+5
   EOR   #$FF
   STA   __local_check__r_4
   LDA   v5+6
   EOR   #$FF
   AND   #$7F
   STA   __local_check__r_5
   LDA   v0
   EOR   __local_check__r_6
   STA   __local_check__r_6
   LDA   __local_check__0
   EOR   __local_check__r_0
   STA   __local_check__r_0
   LDA   __local_check__5
   EOR   __local_check__r_1
   STA   __local_check__r_1
   LDA   __local_check__4
   EOR   __local_check__r_2
   STA   __local_check__r_2
   LDA   __local_check__3
   EOR   __local_check__r_3
   STA   __local_check__r_3
   LDA   __local_check__2
   EOR   __local_check__r_4
   STA   __local_check__r_4
   LDA   __local_check__1
   EOR   __local_check__r_5
   STA   __local_check__r_5
   LDA   #$80
   ORA   __local_check__r_6
   STA   __local_check__r_6
   LDA   __local_check__r_5
   CMP   #$7F
   BNE   .jcmp_skip@11
   LDA   __local_check__r_4
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__r_3
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__r_2
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__r_1
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__r_0
   CMP   #$FE
   BNE   .jcmp_skip@11
   LDA   __local_check__r_6
   CMP   #$F5
   BEQ   .if_end@6
.jcmp_skip@11:
   LDA   __local_check__fail
   ORA   #$10
   STA   __local_check__fail
.if_end@6:
   TXA
   CMP   v1+3
   BNE   .cmp_true@12
   CMP   v1+2
   BNE   .cmp_true@12
   CMP   v1+1
   BNE   .cmp_true@12
   LDA   #$02
   CMP   v1
   BNE   .cmp_true@12
   TXA
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   __local_check__r_6
   TXA
   BNE   .jcmp_skip@15
   LDA   __local_check__r_6
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@15:
   LDA   __local_check__fail
   ORA   #$20
   STA   __local_check__fail
.if_end@7:
   LDA   v2
   BMI   .sx_neg@16
   TXA
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_check__r_6
   LDA   v2
   SEC
   SBC   v4
   LDA   __local_check__r_6
   SBC   v4+1
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   TXA
   ROL
   STA   __local_check__r_4
   LDA   v2
   BMI   .sx_neg@19
   TXA
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_check__r_6
   LDA   v2
   SEC
   SBC   __local_check__r_4
   LDA   __local_check__r_6
   SBC   #$00
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   TXA
   ROL
   EOR   #$01
   CLC
   ADC   v4
   STA   __local_check__r_5
   LDA   v4+1
   ADC   #$00
   CMP   #$55
   BNE   .jcmp_skip@22
   LDA   __local_check__r_5
   CMP   #$AB
   BEQ   .if_end@8
.jcmp_skip@22:
   LDA   __local_check__fail
   ORA   #$40
   STA   __local_check__fail
.if_end@8:
   LDA   v5
   SEC
   SBC   v1
   STA   __local_check__r_0
   LDA   v5+1
   SBC   v1+1
   STA   __local_check__r_1
   LDA   v5+2
   SBC   v1+2
   STA   __local_check__r_2
   LDA   v5+3
   SBC   v1+3
   STA   __local_check__r_3
   LDA   v5+4
   SBC   #$00
   STA   __local_check__r_4
   LDA   v5+5
   SBC   #$00
   STA   __local_check__r_5
   LDA   v5+6
   SBC   #$00
   LDY   __local_check__r_0
   STY   __local_check__6
   LDY   __local_check__r_1
   STY   __local_check__5
   LDY   __local_check__r_2
   STY   __local_check__4
   LDY   __local_check__r_3
   STY   __local_check__3
   LDY   __local_check__r_4
   STY   __local_check__2
   LDY   __local_check__r_5
   STY   __local_check__1
   AND   #$7F
   STA   __local_check__0
   LDA   v0
   BMI   .sx_neg@23
   TXA
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_check__r_6
   LDA   v4
   SEC
   SBC   v0
   LDA   v4+1
   SBC   __local_check__r_6
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
   ASL
   TXA
   ROL
   EOR   #$01
   STA   __local_check__r_1
   TXA
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   STA   __local_check__r_2
   STA   __local_check__r_3
   STA   __local_check__r_4
   STA   __local_check__r_5
   LDA   __local_check__r_1
   STA   __local_check__r_0
   LDA   __local_check__r_2
   AND   #$7F
   STA   __local_check__r_6
   LDA   __local_check__6
   SEC
   SBC   __local_check__r_0
   LDA   __local_check__5
   SBC   #$00
   LDA   __local_check__4
   SBC   __local_check__r_2
   LDA   __local_check__3
   SBC   __local_check__r_3
   LDA   __local_check__2
   SBC   __local_check__r_4
   LDA   __local_check__1
   SBC   __local_check__r_5
   LDA   __local_check__0
   SBC   __local_check__r_6
   BCC   .check@ssa_block@12
   LDA   v1
   ORA   v1+1
   ORA   v1+2
   ORA   v1+3
   BEQ   .if_end@11
.check@ssa_block@12:
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
   JMP   .if_end@2

main:
   SUBROUTINE

   JMP   check

v1:
   DS.B  4

v2:
   DC.B  $80

v3:
   DC.B  $80

v4:
   DC.W  $55AB

v5:
   DC.B  $0A
   DS.B  6

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   v0
   RTS
