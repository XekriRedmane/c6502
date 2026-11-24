SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__i_0	EQU	$80
__zpabi_target__i_1	EQU	$81
__zpabi_target__i_2	EQU	$82
__zpabi_target__i_3	EQU	$83
__zpabi_target__i_4	EQU	$84
__zpabi_target__i_5	EQU	$85
__zpabi_target__i_6	EQU	$86
__zpabi_target__i_7	EQU	$87
__local_main__0	EQU	$88

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target params=__zpabi_target__i_0,__zpabi_target__i_1,__zpabi_target__i_2,__zpabi_target__i_3,__zpabi_target__i_4,__zpabi_target__i_5,__zpabi_target__i_6,__zpabi_target__i_7 locals= indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   __zpabi_target__i_0
   STA   HARGS
   LDA   __zpabi_target__i_1
   STA   HARGS+1
   LDA   __zpabi_target__i_2
   STA   HARGS+2
   LDA   __zpabi_target__i_3
   STA   HARGS+3
   LDA   __zpabi_target__i_4
   STA   HARGS+4
   LDA   __zpabi_target__i_5
   STA   HARGS+5
   LDA   __zpabi_target__i_6
   STA   HARGS+6
   LDA   __zpabi_target__i_7
   STA   HARGS+7
   LDA   #$64
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
   JMP   udivmod64

main:
   SUBROUTINE

   LDA   #$38
   STA   __zpabi_target__i_0
   LDA   #$FF
   STA   __zpabi_target__i_1
   STA   __zpabi_target__i_2
   STA   __zpabi_target__i_3
   STA   __zpabi_target__i_4
   STA   __zpabi_target__i_5
   STA   __zpabi_target__i_6
   STA   __zpabi_target__i_7
   JSR   target
   LDA   HARGS+23
   BNE   .thread@0
   LDA   HARGS+22
   BNE   .thread@0
   LDA   HARGS+21
   BNE   .thread@0
   LDA   HARGS+20
   BNE   .thread@0
   LDA   HARGS+19
   CMP   #$02
   BNE   .thread@0
   LDA   HARGS+18
   CMP   #$8F
   BNE   .thread@0
   LDA   HARGS+17
   CMP   #$5C
   BNE   .thread@0
   LDA   HARGS+16
   CMP   #$26
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   STA   __local_main__0
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   __local_main__0
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   RTS
