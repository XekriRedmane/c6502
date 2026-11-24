SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_sign_extend__expected_0	EQU	$80
__zpabi_sign_extend__expected_1	EQU	$81
__zpabi_sign_extend__expected_2	EQU	$82
__zpabi_sign_extend__expected_3	EQU	$83
__zpabi_sign_extend__expected_4	EQU	$84
__zpabi_sign_extend__expected_5	EQU	$85
__zpabi_sign_extend__expected_6	EQU	$86
__zpabi_sign_extend__expected_7	EQU	$87
__local_sign_extend__0	EQU	$90
__local_sign_extend__i_0	EQU	$97

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def sign_extend params=__zpabi_sign_extend__i_0,__zpabi_sign_extend__i_1,__zpabi_sign_extend__expected_0,__zpabi_sign_extend__expected_1,__zpabi_sign_extend__expected_2,__zpabi_sign_extend__expected_3,__zpabi_sign_extend__expected_4,__zpabi_sign_extend__expected_5,__zpabi_sign_extend__expected_6,__zpabi_sign_extend__expected_7 locals=__local_sign_extend__0,__local_sign_extend__1,__local_sign_extend__2,__local_sign_extend__3,__local_sign_extend__4,__local_sign_extend__5,__local_sign_extend__i_1,__local_sign_extend__i_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,
; call main -> sign_extend
; item sign_extend section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

sign_extend:
   SUBROUTINE

   STA   __local_sign_extend__i_0
   TXA
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   CMP   __zpabi_sign_extend__expected_7
   BNE   .thread@0
   CMP   __zpabi_sign_extend__expected_6
   BNE   .thread@0
   CMP   __zpabi_sign_extend__expected_5
   BNE   .thread@0
   CMP   __zpabi_sign_extend__expected_4
   BNE   .thread@0
   CMP   __zpabi_sign_extend__expected_3
   BNE   .thread@0
   CMP   __zpabi_sign_extend__expected_2
   BNE   .thread@0
   CPX   __zpabi_sign_extend__expected_1
   BNE   .thread@0
   LDA   __local_sign_extend__i_0
   CMP   __zpabi_sign_extend__expected_0
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   STA   __local_sign_extend__0
   LDA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   __local_sign_extend__0
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   RTS

main:
   SUBROUTINE

   LDA   #$0A
   STA   __zpabi_sign_extend__expected_0
   LDA   #$00
   STA   __zpabi_sign_extend__expected_1
   JSR   main@run1
   LDX   #$00
   LDA   #$0A
   JSR   sign_extend
   JSR   main@run0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$F6
   STA   __zpabi_sign_extend__expected_0
   LDA   #$FF
   STA   __zpabi_sign_extend__expected_1
   JSR   main@run1
   LDX   #$FF
   LDA   #$F6
   JSR   sign_extend
   JSR   main@run0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   STA   __zpabi_sign_extend__expected_2
   STA   __zpabi_sign_extend__expected_3
   STA   __zpabi_sign_extend__expected_4
   STA   __zpabi_sign_extend__expected_5
   STA   __zpabi_sign_extend__expected_6
   STA   __zpabi_sign_extend__expected_7
   RTS

main@run0:
   SUBROUTINE

   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   ORA   HARGS+21
   ORA   HARGS+22
   ORA   HARGS+23
   RTS
