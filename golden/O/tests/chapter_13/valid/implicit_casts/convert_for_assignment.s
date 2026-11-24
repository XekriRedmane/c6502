SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check_args__l_0	EQU	$80
__zpabi_check_assignment__arg_0	EQU	$80
__zpabi_check_args__l_1	EQU	$81
__zpabi_check_assignment__arg_1	EQU	$81
__zpabi_check_args__l_2	EQU	$82
__zpabi_check_assignment__arg_2	EQU	$82
__zpabi_check_args__l_3	EQU	$83
__zpabi_check_assignment__arg_3	EQU	$83
__zpabi_check_args__l_4	EQU	$84
__zpabi_check_assignment__arg_4	EQU	$84
__zpabi_check_args__l_5	EQU	$85
__zpabi_check_assignment__arg_5	EQU	$85
__zpabi_check_args__l_6	EQU	$86
__zpabi_check_assignment__arg_6	EQU	$86
__zpabi_check_args__l_7	EQU	$87
__zpabi_check_assignment__arg_7	EQU	$87
__zpabi_check_args__d_0	EQU	$88
__zpabi_check_args__d_1	EQU	$89
__zpabi_check_args__d_2	EQU	$8A
__zpabi_check_args__d_3	EQU	$8B
__zpabi_check_args__d_4	EQU	$8C
__zpabi_check_args__d_5	EQU	$8D
__zpabi_check_args__d_6	EQU	$8E
__zpabi_check_args__d_7	EQU	$8F
__local_main__0	EQU	$90

; @zp-link-meta-begin
; def check_args params=__zpabi_check_args__l_0,__zpabi_check_args__l_1,__zpabi_check_args__l_2,__zpabi_check_args__l_3,__zpabi_check_args__l_4,__zpabi_check_args__l_5,__zpabi_check_args__l_6,__zpabi_check_args__l_7,__zpabi_check_args__d_0,__zpabi_check_args__d_1,__zpabi_check_args__d_2,__zpabi_check_args__d_3,__zpabi_check_args__d_4,__zpabi_check_args__d_5,__zpabi_check_args__d_6,__zpabi_check_args__d_7 locals=__local_check_args__0 indirect=false in_cycle=false
; def check_assignment params=__zpabi_check_assignment__arg_0,__zpabi_check_assignment__arg_1,__zpabi_check_assignment__arg_2,__zpabi_check_assignment__arg_3,__zpabi_check_assignment__arg_4,__zpabi_check_assignment__arg_5,__zpabi_check_assignment__arg_6,__zpabi_check_assignment__arg_7 locals=__local_check_assignment__0,__local_check_assignment__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def return_double params= locals= indirect=false in_cycle=false
; call main -> check_args
; call main -> check_assignment
; call main -> return_double
; item check_args section=.text local=false zero=false
; item return_double section=.text local=false zero=false
; item check_assignment section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

check_args:
   SUBROUTINE

   LDA   __zpabi_check_args__l_7
   BNE   .and_false@0
   LDA   __zpabi_check_args__l_6
   BNE   .and_false@0
   LDA   __zpabi_check_args__l_5
   BNE   .and_false@0
   LDA   __zpabi_check_args__l_4
   BNE   .and_false@0
   LDA   __zpabi_check_args__l_3
   BNE   .and_false@0
   LDA   __zpabi_check_args__l_2
   BNE   .and_false@0
   LDA   __zpabi_check_args__l_1
   BNE   .and_false@0
   LDA   __zpabi_check_args__l_0
   CMP   #$02
   BNE   .and_false@0
   LDA   __zpabi_check_args__d_7
   CMP   #$C0
   BNE   .and_false@0
   LDA   __zpabi_check_args__d_6
   CMP   #$18
   BNE   .and_false@0
   LDA   __zpabi_check_args__d_5
   BNE   .and_false@0
   LDA   __zpabi_check_args__d_4
   BNE   .and_false@0
   LDA   __zpabi_check_args__d_3
   BNE   .and_false@0
   LDA   __zpabi_check_args__d_2
   BNE   .and_false@0
   LDA   __zpabi_check_args__d_1
   BNE   .and_false@0
   LDA   __zpabi_check_args__d_0
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   LDA   #$00
   LDX   #$00
   RTS

return_double:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STA   HARGS+19
   LDA   #$65
   STA   HARGS+20
   LDA   #$CD
   STA   HARGS+21
   LDA   #$ED
   STA   HARGS+22
   LDA   #$41
   STA   HARGS+23
   RTS

check_assignment:
   SUBROUTINE

   LDA   __zpabi_check_assignment__arg_0
   STA   HARGS
   LDA   __zpabi_check_assignment__arg_1
   STA   HARGS+1
   LDA   __zpabi_check_assignment__arg_2
   STA   HARGS+2
   LDA   __zpabi_check_assignment__arg_3
   STA   HARGS+3
   LDA   __zpabi_check_assignment__arg_4
   STA   HARGS+4
   LDA   __zpabi_check_assignment__arg_5
   STA   HARGS+5
   LDA   __zpabi_check_assignment__arg_6
   STA   HARGS+6
   LDA   __zpabi_check_assignment__arg_7
   STA   HARGS+7
   JSR   d2i
   LDA   HARGS+9
   BNE   .thread@0
   LDA   HARGS+8
   CMP   #$04
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __zpabi_check_args__l_0
   LDA   #$00
   STA   __zpabi_check_args__l_1
   STA   __zpabi_check_args__l_2
   STA   __zpabi_check_args__l_3
   STA   __zpabi_check_args__l_4
   STA   __zpabi_check_args__l_5
   STA   __zpabi_check_args__l_6
   STA   __zpabi_check_args__l_7
   STA   __zpabi_check_args__d_0
   STA   __zpabi_check_args__d_1
   STA   __zpabi_check_args__d_2
   STA   __zpabi_check_args__d_3
   STA   __zpabi_check_args__d_4
   STA   __zpabi_check_args__d_5
   LDA   #$18
   STA   __zpabi_check_args__d_6
   LDA   #$C0
   STA   __zpabi_check_args__d_7
   JSR   check_args
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   JSR   return_double
   LDA   HARGS+23
   CMP   #$41
   BNE   .jcmp_skip@3
   LDA   HARGS+22
   CMP   #$ED
   BNE   .jcmp_skip@3
   LDA   HARGS+21
   CMP   #$CD
   BNE   .jcmp_skip@3
   LDA   HARGS+20
   CMP   #$65
   BNE   .jcmp_skip@3
   LDA   HARGS+19
   BNE   .jcmp_skip@3
   LDA   HARGS+18
   BNE   .jcmp_skip@3
   LDA   HARGS+17
   BNE   .jcmp_skip@3
   LDA   HARGS+16
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDA   #$9A
   STA   __zpabi_check_assignment__arg_0
   LDA   #$99
   STA   __zpabi_check_assignment__arg_1
   STA   __zpabi_check_assignment__arg_2
   STA   __zpabi_check_assignment__arg_3
   STA   __zpabi_check_assignment__arg_4
   STA   __zpabi_check_assignment__arg_5
   LDA   #$13
   STA   __zpabi_check_assignment__arg_6
   LDA   #$40
   STA   __zpabi_check_assignment__arg_7
   JSR   check_assignment
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$03
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS
