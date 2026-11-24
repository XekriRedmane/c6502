SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_add__b_0	EQU	$80
__zpabi_add__b_1	EQU	$81
__local_add__0	EQU	$8A
__local_add__1	EQU	$8B
__local_add__2	EQU	$8C
__local_add__3	EQU	$8D
__local_add__4	EQU	$8E
__local_add__5	EQU	$8F
__local_add__6	EQU	$90
__local_add__7	EQU	$91
__local_add__8	EQU	$92
__local_add__9	EQU	$93
__local_add__10	EQU	$94
__local_add__11	EQU	$95
__local_add__a_0	EQU	$97

; @zp-link-meta-begin
; def add params=__zpabi_add__a_0,__zpabi_add__a_1,__zpabi_add__b_0,__zpabi_add__b_1 locals=__local_add__0,__local_add__1,__local_add__2,__local_add__3,__local_add__4,__local_add__5,__local_add__6,__local_add__7,__local_add__8,__local_add__9,__local_add__10,__local_add__11,__local_add__a_1,__local_add__a_0 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; call main -> add
; item add section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

add:
   SUBROUTINE

   STA   __local_add__a_0
   TXA
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_add__11
   STA   __local_add__10
   STA   __local_add__9
   STA   __local_add__8
   STA   __local_add__7
   STA   __local_add__6
   LDA   __zpabi_add__b_1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_add__5
   STA   __local_add__4
   STA   __local_add__3
   STA   __local_add__2
   STA   __local_add__1
   STA   __local_add__0
   LDA   __local_add__a_0
   CLC
   ADC   __zpabi_add__b_0
   STA   HARGS+16
   TXA
   ADC   __zpabi_add__b_1
   STA   HARGS+17
   LDA   __local_add__11
   ADC   __local_add__5
   STA   HARGS+18
   LDA   __local_add__10
   ADC   __local_add__4
   STA   HARGS+19
   LDA   __local_add__9
   ADC   __local_add__3
   STA   HARGS+20
   LDA   __local_add__8
   ADC   __local_add__2
   STA   HARGS+21
   LDA   __local_add__7
   ADC   __local_add__1
   STA   HARGS+22
   LDA   __local_add__6
   ADC   __local_add__0
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   LDA   #$7D
   STA   __zpabi_add__b_0
   LDA   #$00
   STA   __zpabi_add__b_1
   LDX   #$00
   LDA   #$7D
   JSR   add
   LDA   HARGS+23
   BNE   .if_end@0
   LDA   HARGS+22
   BNE   .if_end@0
   LDA   HARGS+21
   BNE   .if_end@0
   LDA   HARGS+20
   BNE   .if_end@0
   LDA   HARGS+19
   BNE   .if_end@0
   LDA   HARGS+18
   BNE   .if_end@0
   LDA   HARGS+17
   BNE   .if_end@0
   LDA   HARGS+16
   CMP   #$FA
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS
