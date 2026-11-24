SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__autom_0	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83
__local_main__autom_3	EQU	$84
__local_main__autom_1	EQU	$85
__local_main__i	EQU	$86

; @zp-link-meta-begin
; def main params= locals=__local_main__autom_2,__local_main__autom_0,__local_main__0,__local_main__1,__local_main__autom_3,__local_main__autom_1,__local_main__i indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item @2.stat section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__i
   LDX   #$00
.loop@0_start:
   LDA   __local_main__i
   SEC
   SBC   #$0A
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   #$00
   STA   __local_main__autom_1
   LDY   #$02
   STA   __local_main__autom_3
   LDA   #$01
   CLC
   ADC   #$01
   STA   __local_main__1
   BCC   .carry_inc@0
   INC   __local_main__autom_1
.carry_inc@0:
   TYA
   CLC
   ADC   #$01
   STA   __local_main__0
   BCC   .carry_inc@1
   INC   __local_main__autom_3
.carry_inc@1:
   INC   @2.stat
   BNE   .inc_done@0
   INC   @2.stat+1
.inc_done@0:
   INC   @2.stat+2
   BNE   .inc_done@1
   INC   @2.stat+3
.inc_done@1:
   CPX   #$00
   BNE   .if_end@0
   LDA   __local_main__i
   CMP   #$09
   BNE   .if_end@0
   LDA   @2.stat
   STA   __local_main__autom_0
   LDA   @2.stat+1
   BNE   .main@jthread@0
   LDA   __local_main__autom_0
   CMP   #$0B
   BNE   .main@jthread@0
   LDA   @2.stat+2
   STA   __local_main__autom_0
   LDA   @2.stat+3
   BNE   .main@jthread@0
   LDA   __local_main__autom_0
   CMP   #$0C
   BEQ   .if_end@3
.main@jthread@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@3:
   LDA   __local_main__autom_1
   BNE   .main@jthread@1
   LDA   __local_main__1
   CMP   #$02
   BNE   .main@jthread@1
   LDA   __local_main__autom_3
   BNE   .main@jthread@1
   LDA   __local_main__0
   CMP   #$03
   BEQ   .if_end@6
.main@jthread@1:
   LDX   #$00
   TYA
   RTS
.if_end@6:
.if_end@0:
   INC   __local_main__i
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INX
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$00
   TXA
   RTS

@2.stat:
   DC.W  $0001
   DC.W  $0002
