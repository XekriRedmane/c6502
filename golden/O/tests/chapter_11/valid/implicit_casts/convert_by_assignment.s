SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_return_truncated_long__l_0	EQU	$80
__zpabi_truncate_on_assignment__l_0	EQU	$80
__local_return_extended_int__i_0	EQU	$81
__zpabi_return_truncated_long__l_1	EQU	$81
__zpabi_truncate_on_assignment__l_1	EQU	$81
__zpabi_return_truncated_long__l_2	EQU	$82
__zpabi_truncate_on_assignment__l_2	EQU	$82
__zpabi_return_truncated_long__l_3	EQU	$83
__zpabi_truncate_on_assignment__l_3	EQU	$83
__zpabi_return_truncated_long__l_4	EQU	$84
__zpabi_truncate_on_assignment__l_4	EQU	$84
__zpabi_return_truncated_long__l_5	EQU	$85
__zpabi_truncate_on_assignment__l_5	EQU	$85
__zpabi_return_truncated_long__l_6	EQU	$86
__zpabi_truncate_on_assignment__l_6	EQU	$86
__zpabi_return_truncated_long__l_7	EQU	$87
__zpabi_truncate_on_assignment__l_7	EQU	$87
__zpabi_truncate_on_assignment__expected_0	EQU	$88
__zpabi_truncate_on_assignment__expected_1	EQU	$89
__local_main__7	EQU	$8F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def return_extended_int params=__zpabi_return_extended_int__i_0,__zpabi_return_extended_int__i_1 locals=__local_return_extended_int__i_1,__local_return_extended_int__i_0 indirect=false in_cycle=false param_regs=A,X
; def return_truncated_long params=__zpabi_return_truncated_long__l_0,__zpabi_return_truncated_long__l_1,__zpabi_return_truncated_long__l_2,__zpabi_return_truncated_long__l_3,__zpabi_return_truncated_long__l_4,__zpabi_return_truncated_long__l_5,__zpabi_return_truncated_long__l_6,__zpabi_return_truncated_long__l_7 locals= indirect=false in_cycle=false
; def truncate_on_assignment params=__zpabi_truncate_on_assignment__l_0,__zpabi_truncate_on_assignment__l_1,__zpabi_truncate_on_assignment__l_2,__zpabi_truncate_on_assignment__l_3,__zpabi_truncate_on_assignment__l_4,__zpabi_truncate_on_assignment__l_5,__zpabi_truncate_on_assignment__l_6,__zpabi_truncate_on_assignment__l_7,__zpabi_truncate_on_assignment__expected_0,__zpabi_truncate_on_assignment__expected_1 locals=__local_truncate_on_assignment__0 indirect=false in_cycle=false
; call main -> return_extended_int
; call main -> return_truncated_long
; item return_truncated_long section=.text local=false zero=false
; item return_extended_int section=.text local=false zero=false
; item truncate_on_assignment section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

return_truncated_long:
   SUBROUTINE

   LDX   __zpabi_return_truncated_long__l_1
   LDA   __zpabi_return_truncated_long__l_0
   RTS

return_extended_int:
   SUBROUTINE

   STA   __local_return_extended_int__i_0
   TXA
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   __local_return_extended_int__i_0
   STA   HARGS+16
   STX   HARGS+17
   RTS

truncate_on_assignment:
   SUBROUTINE

   LDA   __zpabi_truncate_on_assignment__l_1
   CMP   __zpabi_truncate_on_assignment__expected_1
   BNE   .thread@0
   LDA   __zpabi_truncate_on_assignment__l_0
   CMP   __zpabi_truncate_on_assignment__expected_0
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __zpabi_return_truncated_long__l_0
   LDA   #$01
   STA   __zpabi_return_truncated_long__l_1
   LDA   #$00
   STA   __zpabi_return_truncated_long__l_2
   STA   __zpabi_return_truncated_long__l_3
   STA   __zpabi_return_truncated_long__l_4
   STA   __zpabi_return_truncated_long__l_5
   STA   __zpabi_return_truncated_long__l_6
   STA   __zpabi_return_truncated_long__l_7
   JSR   return_truncated_long
   STA   __local_main__7
   TXA
   BMI   .jcmp_skip@7
   BNE   .jcmp_skip@7
   LDA   __local_main__7
   CMP   #$02
   BEQ   .if_end@0
.jcmp_skip@7:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$FF
   LDA   #$F6
   JSR   return_extended_int
   LDA   HARGS+23
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+22
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+21
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+20
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+19
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+18
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+17
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+16
   CMP   #$F6
   BEQ   .if_end@1
.jcmp_skip@8:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   LDA   #$03
   RTS
