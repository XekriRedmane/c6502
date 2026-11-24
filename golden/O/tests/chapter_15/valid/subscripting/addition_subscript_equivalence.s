SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87
__local_main__8	EQU	$88
__local_main__9	EQU	$89
__local_main__j_1	EQU	$8A
__local_main__10	EQU	$8B
__local_main__j_0	EQU	$8C
__local_main__i_0	EQU	$8D

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__j_1,__local_main__10,__local_main__j_0,__local_main__i_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item x section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__j_0
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$64
   LDA   __local_main__j_0
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .lb_skip@7
   JMP   .loop@0_break
.lb_skip@7:
   LDA   #$00
   STA   __local_main__10
   LDY   #$00
   STY   __local_main__j_1
.loop@1_start:
   LDA   __local_main__10
   SEC
   SBC   #$05
   LDA   __local_main__j_1
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BMI   .lb_skip@6
   JMP   .loop@1_break
.lb_skip@6:
   LDA   __local_main__i_0
   ASL
   STA   __local_main__8
   LDA   __local_main__j_0
   ROL
   ASL   __local_main__8
   ROL
   STA   __local_main__9
   LDA   __local_main__8
   CLC
   ADC   __local_main__i_0
   STA   __local_main__1
   LDA   __local_main__9
   ADC   __local_main__j_0
   STA   __local_main__0
   LDA   __local_main__1
   CLC
   ADC   __local_main__10
   STA   __local_main__7
   LDA   __local_main__0
   ADC   __local_main__j_1
   STA   __local_main__6
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_main__5
   LDA   __local_main__8
   ASL
   STA   __local_main__2
   LDA   __local_main__9
   ROL
   ASL   __local_main__2
   ROL
   STA   __local_main__3
   LDA   __local_main__2
   CLC
   ADC   __local_main__8
   STA   __local_main__1
   LDA   __local_main__3
   ADC   __local_main__9
   STA   __local_main__0
   LDA   #<x
   CLC
   ADC   __local_main__1
   STA   __local_main__3
   LDA   #>x
   ADC   __local_main__0
   STA   __local_main__1
   LDA   __local_main__10
   ASL
   ASL
   CLC
   ADC   __local_main__3
   STA   __local_main__0
   BCC   .carry_inc@0
   INC   __local_main__1
.carry_inc@0:
   LDA   __local_main__7
   LDY   #$00
   STA   (__local_main__0),Y
   LDA   __local_main__6
   INY
   STA   (__local_main__0),Y
   LDA   __local_main__5
   INY
   STA   (__local_main__0),Y
   INY
   STA   (__local_main__0),Y
   INC   __local_main__10
   BEQ   .lb_skip@5
   JMP   .loop@1_start
.lb_skip@5:
   INC   __local_main__j_1
   JMP   .loop@1_start
.loop@1_break:
   INC   __local_main__i_0
   BEQ   .lb_skip@4
   JMP   .loop@0_start
.lb_skip@4:
   INC   __local_main__j_0
   JMP   .loop@0_start
.loop@0_break:
   LDA   x+412
   STA   __local_main__9
   STA   __local_main__3
   LDA   x+413
   STA   __local_main__8
   STA   __local_main__2
   LDA   x+415
   LDX   x+414
   STX   __local_main__7
   STX   __local_main__1
   CMP   x+415
   BNE   .jcmp_skip@4
   LDA   __local_main__7
   CMP   __local_main__1
   BNE   .jcmp_skip@4
   LDA   __local_main__8
   CMP   __local_main__2
   BNE   .jcmp_skip@4
   LDA   __local_main__9
   CMP   __local_main__3
   BEQ   .if_end@0
.jcmp_skip@4:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #>(x+1812)
   CMP   #>(x+1812)
   BNE   .jcmp_skip@5
   LDA   #<(x+1812)
   CMP   #<(x+1812)
   BEQ   .if_end@1
.jcmp_skip@5:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$00
   STA   __local_main__i_0
   LDX   #$00
.loop@2_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$64
   TXA
   SBC   #$00
   BVC   .jcmp_novf@6
   EOR   #$80
.jcmp_novf@6:
   BMI   .lb_skip@3
   JMP   .loop@2_break
.lb_skip@3:
   LDA   #$00
   STA   __local_main__j_0
   LDY   #$00
   STY   __local_main__10
.loop@3_start:
   LDA   __local_main__j_0
   SEC
   SBC   #$05
   LDA   __local_main__10
   SBC   #$00
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BMI   .lb_skip@2
   JMP   .loop@3_break
.lb_skip@2:
   LDA   __local_main__i_0
   ASL
   STA   __local_main__2
   STX   __local_main__3
   ROL   __local_main__3
   ASL   __local_main__2
   ROL   __local_main__3
   LDA   __local_main__2
   ASL
   STA   __local_main__0
   LDA   __local_main__3
   ROL
   ASL   __local_main__0
   ROL
   STA   __local_main__1
   LDA   __local_main__0
   CLC
   ADC   __local_main__2
   STA   __local_main__j_1
   LDA   __local_main__1
   ADC   __local_main__3
   STA   __local_main__5
   LDA   #<x
   CLC
   ADC   __local_main__j_1
   STA   __local_main__0
   LDA   #>x
   ADC   __local_main__5
   STA   __local_main__4
   LDA   __local_main__j_0
   ASL
   STA   __local_main__2
   LDA   __local_main__10
   ROL
   ASL   __local_main__2
   ROL
   STA   __local_main__3
   LDA   __local_main__0
   CLC
   ADC   __local_main__2
   STA   __local_main__0
   LDA   __local_main__4
   ADC   __local_main__3
   STA   __local_main__1
   LDY   #$00
   LDA   (__local_main__0),Y
   STA   __local_main__9
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__8
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__7
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__6
   LDA   #<x
   CLC
   ADC   __local_main__j_1
   STA   __local_main__1
   LDA   #>x
   ADC   __local_main__5
   STA   __local_main__5
   LDA   __local_main__j_0
   ASL
   ASL
   CLC
   ADC   __local_main__1
   STA   __local_main__4
   BCC   .carry_inc@1
   INC   __local_main__5
.carry_inc@1:
   LDY   #$00
   LDA   (__local_main__4),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__1
   LDA   __local_main__6
   INY
   CMP   (__local_main__4),Y
   BNE   .jcmp_skip@8
   LDA   __local_main__7
   CMP   __local_main__1
   BNE   .jcmp_skip@8
   LDA   __local_main__8
   CMP   __local_main__2
   BNE   .jcmp_skip@8
   LDA   __local_main__9
   CMP   __local_main__3
   BEQ   .if_end@2
.jcmp_skip@8:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   INC   __local_main__j_0
   BEQ   .lb_skip@1
   JMP   .loop@3_start
.lb_skip@1:
   INC   __local_main__10
   JMP   .loop@3_start
.loop@3_break:
   INC   __local_main__i_0
   BEQ   .lb_skip@0
   JMP   .loop@2_start
.lb_skip@0:
   INX
   JMP   .loop@2_start
.loop@2_break:
   LDA   #$F0
   STA   x+1516
   LDA   #$55
   STA   x+1517
   LDA   #$00
   STA   x+1518
   STA   x+1519
   BNE   .jcmp_skip@9
   LDA   #$55
   CMP   #$55
   BNE   .jcmp_skip@9
   LDA   #$F0
   CMP   #$F0
   BEQ   .if_end@3
.jcmp_skip@9:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

x:
   DS.B  2000
