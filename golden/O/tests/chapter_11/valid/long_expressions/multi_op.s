SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__a_0	EQU	$80
__zpabi_target__a_1	EQU	$81
__zpabi_target__a_2	EQU	$82
__zpabi_target__a_3	EQU	$83
__zpabi_target__a_4	EQU	$84
__zpabi_target__a_5	EQU	$85
__zpabi_target__a_6	EQU	$86
__zpabi_target__a_7	EQU	$87
__local_target__0	EQU	$89
__local_target__1	EQU	$8A
__local_target__2	EQU	$8B
__local_target__3	EQU	$8C
__local_target__4	EQU	$8D
__local_target__5	EQU	$8E
__local_target__6	EQU	$8F
__local_target__7	EQU	$90

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params=__zpabi_target__a_0,__zpabi_target__a_1,__zpabi_target__a_2,__zpabi_target__a_3,__zpabi_target__a_4,__zpabi_target__a_5,__zpabi_target__a_6,__zpabi_target__a_7 locals=__local_target__0,__local_target__1,__local_target__2,__local_target__3,__local_target__4,__local_target__5,__local_target__6,__local_target__7 indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   __zpabi_target__a_0
   STA   HARGS
   LDA   __zpabi_target__a_1
   STA   HARGS+1
   LDA   __zpabi_target__a_2
   STA   HARGS+2
   LDA   __zpabi_target__a_3
   STA   HARGS+3
   LDA   __zpabi_target__a_4
   STA   HARGS+4
   LDA   __zpabi_target__a_5
   STA   HARGS+5
   LDA   __zpabi_target__a_6
   STA   HARGS+6
   LDA   __zpabi_target__a_7
   STA   HARGS+7
   LDA   #$05
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+23
   STA   __local_target__1
   LDA   HARGS+16
   SEC
   SBC   #$0A
   STA   __local_target__0
   LDA   HARGS+17
   SBC   #$00
   STA   __local_target__7
   LDA   HARGS+18
   SBC   #$00
   STA   __local_target__6
   LDA   HARGS+19
   SBC   #$00
   STA   __local_target__5
   LDA   HARGS+20
   SBC   #$00
   STA   __local_target__4
   LDA   HARGS+21
   SBC   #$00
   STA   __local_target__3
   LDA   HARGS+22
   SBC   #$00
   STA   __local_target__2
   BCS   .carry_inc@0
   DEC   __local_target__1
.carry_inc@0:
   LDA   __local_target__1
   BNE   .if_end@0
   LDA   __local_target__2
   BNE   .if_end@0
   LDA   __local_target__3
   BNE   .if_end@0
   LDA   __local_target__4
   BNE   .if_end@0
   LDA   __local_target__5
   BNE   .if_end@0
   LDA   __local_target__6
   CMP   #$07
   BNE   .if_end@0
   LDA   __local_target__7
   CMP   #$A1
   BNE   .if_end@0
   LDA   __local_target__0
   CMP   #$16
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$A0
   STA   __zpabi_target__a_0
   LDA   #$86
   STA   __zpabi_target__a_1
   LDA   #$01
   STA   __zpabi_target__a_2
   LDA   #$00
   STA   __zpabi_target__a_3
   STA   __zpabi_target__a_4
   STA   __zpabi_target__a_5
   STA   __zpabi_target__a_6
   STA   __zpabi_target__a_7
   JMP   target
