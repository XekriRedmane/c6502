SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_multiply__0	EQU	$80
__local_tern_double_result__flag_1	EQU	$80
__zpabi_lt__d_0	EQU	$80
__zpabi_tern_double_flag__flag_0	EQU	$80
__local_multiply__1	EQU	$81
__zpabi_lt__d_1	EQU	$81
__zpabi_tern_double_flag__flag_1	EQU	$81
__zpabi_lt__d_2	EQU	$82
__zpabi_tern_double_flag__flag_2	EQU	$82
__zpabi_lt__d_3	EQU	$83
__zpabi_tern_double_flag__flag_3	EQU	$83
__zpabi_lt__d_4	EQU	$84
__zpabi_tern_double_flag__flag_4	EQU	$84
__zpabi_lt__d_5	EQU	$85
__zpabi_tern_double_flag__flag_5	EQU	$85
__zpabi_lt__d_6	EQU	$86
__zpabi_tern_double_flag__flag_6	EQU	$86
__zpabi_lt__d_7	EQU	$87
__zpabi_tern_double_flag__flag_7	EQU	$87
__zpabi_lt__l_0	EQU	$88
__zpabi_lt__l_1	EQU	$89
__zpabi_lt__l_2	EQU	$8A
__zpabi_lt__l_3	EQU	$8B
__zpabi_lt__l_4	EQU	$8C
__zpabi_lt__l_5	EQU	$8D
__zpabi_lt__l_6	EQU	$8E
__zpabi_lt__l_7	EQU	$8F
__local_main__0	EQU	$90

; @zp-link-meta-begin
; def lt params=__zpabi_lt__d_0,__zpabi_lt__d_1,__zpabi_lt__d_2,__zpabi_lt__d_3,__zpabi_lt__d_4,__zpabi_lt__d_5,__zpabi_lt__d_6,__zpabi_lt__d_7,__zpabi_lt__l_0,__zpabi_lt__l_1,__zpabi_lt__l_2,__zpabi_lt__l_3,__zpabi_lt__l_4,__zpabi_lt__l_5,__zpabi_lt__l_6,__zpabi_lt__l_7 locals=__local_lt__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def multiply params= locals=__local_multiply__0,__local_multiply__1 indirect=false in_cycle=false
; def tern_double_flag params=__zpabi_tern_double_flag__flag_0,__zpabi_tern_double_flag__flag_1,__zpabi_tern_double_flag__flag_2,__zpabi_tern_double_flag__flag_3,__zpabi_tern_double_flag__flag_4,__zpabi_tern_double_flag__flag_5,__zpabi_tern_double_flag__flag_6,__zpabi_tern_double_flag__flag_7 locals=__local_tern_double_flag__0,__local_tern_double_flag__1,__local_tern_double_flag__2,__local_tern_double_flag__3 indirect=false in_cycle=false
; def tern_double_result params=__zpabi_tern_double_result__flag_0,__zpabi_tern_double_result__flag_1 locals=__local_tern_double_result__flag_1,__local_tern_double_result__flag_0,__local_tern_double_result__0,__local_tern_double_result__1,__local_tern_double_result__2,__local_tern_double_result__3,__local_tern_double_result__4,__local_tern_double_result__5 indirect=false in_cycle=false param_regs=A,X
; call main -> lt
; call main -> multiply
; call main -> tern_double_flag
; call main -> tern_double_result
; item lt section=.text local=false zero=false
; item tern_double_flag section=.text local=false zero=false
; item tern_double_result section=.text local=false zero=false
; item multiply section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; item ten section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

lt:
   SUBROUTINE

   LDA   __zpabi_lt__l_0
   STA   HARGS
   LDA   __zpabi_lt__l_1
   STA   HARGS+1
   LDA   __zpabi_lt__l_2
   STA   HARGS+2
   LDA   __zpabi_lt__l_3
   STA   HARGS+3
   LDA   __zpabi_lt__l_4
   STA   HARGS+4
   LDA   __zpabi_lt__l_5
   STA   HARGS+5
   LDA   __zpabi_lt__l_6
   STA   HARGS+6
   LDA   __zpabi_lt__l_7
   STA   HARGS+7
   JSR   ll2d
   LDA   __zpabi_lt__d_0
   STA   HARGS
   LDA   __zpabi_lt__d_1
   STA   HARGS+1
   LDA   __zpabi_lt__d_2
   STA   HARGS+2
   LDA   __zpabi_lt__d_3
   STA   HARGS+3
   LDA   __zpabi_lt__d_4
   STA   HARGS+4
   LDA   __zpabi_lt__d_5
   STA   HARGS+5
   LDA   __zpabi_lt__d_6
   STA   HARGS+6
   LDA   __zpabi_lt__d_7
   STA   HARGS+7
   JSR   dlt
   LDA   HARGS+16
   LDX   #$00
   RTS

tern_double_flag:
   SUBROUTINE

   LDA   __zpabi_tern_double_flag__flag_0
   ORA   __zpabi_tern_double_flag__flag_1
   ORA   __zpabi_tern_double_flag__flag_2
   ORA   __zpabi_tern_double_flag__flag_3
   ORA   __zpabi_tern_double_flag__flag_4
   ORA   __zpabi_tern_double_flag__flag_5
   ORA   __zpabi_tern_double_flag__flag_6
   ORA   __zpabi_tern_double_flag__flag_7
   BEQ   .cond_else@0
   LDA   #$E2
   STA   HARGS
   LDA   #$FF
   BNE   .cond_end@1
.cond_else@0:
   LDA   #$0A
   STA   HARGS
   LDA   #$00
.cond_end@1:
   STA   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   JSR   ul2d
   LDA   HARGS+4
   STA   HARGS+16
   LDA   HARGS+5
   STA   HARGS+17
   LDA   HARGS+6
   STA   HARGS+18
   LDA   HARGS+7
   STA   HARGS+19
   LDA   HARGS+8
   STA   HARGS+20
   LDA   HARGS+9
   STA   HARGS+21
   LDA   HARGS+10
   STA   HARGS+22
   LDA   HARGS+11
   STA   HARGS+23
   RTS

tern_double_result:
   SUBROUTINE

   STX   __local_tern_double_result__flag_1
   ORA   __local_tern_double_result__flag_1
   BEQ   .cond_else@2
   LDA   #$00
   STA   HARGS+20
   STA   HARGS+21
   LDA   #$14
   STA   HARGS+22
   LDA   #$40
   STA   HARGS+23
   JMP   .cond_end@3
.cond_else@2:
   LDA   #$65
   STA   HARGS+20
   LDA   #$CD
   STA   HARGS+21
   LDA   #$ED
   STA   HARGS+22
   LDA   #$41
   STA   HARGS+23
.cond_end@3:
   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STA   HARGS+19
   RTS

multiply:
   SUBROUTINE

   LDA   ten
   STA   HARGS
   LDA   ten+1
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   STA   __local_multiply__1
   LDA   HARGS+3
   STA   __local_multiply__0
   LDA   HARGS+4
   STA   HARGS+10
   LDA   HARGS+5
   STA   HARGS+11
   LDA   HARGS+6
   STA   HARGS+12
   LDA   HARGS+7
   STA   HARGS+13
   LDA   HARGS+8
   STA   HARGS+14
   LDA   HARGS+9
   STA   HARGS+15
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STA   HARGS+4
   LDA   #$80
   STA   HARGS+5
   LDA   #$25
   STA   HARGS+6
   LDA   #$40
   STA   HARGS+7
   LDA   __local_multiply__1
   STA   HARGS+8
   LDA   __local_multiply__0
   STA   HARGS+9
   JSR   dmul
   LDA   HARGS+16
   STA   HARGS
   LDA   HARGS+17
   STA   HARGS+1
   LDA   HARGS+18
   STA   HARGS+2
   LDA   HARGS+19
   STA   HARGS+3
   LDA   HARGS+20
   STA   HARGS+4
   LDA   HARGS+21
   STA   HARGS+5
   LDA   HARGS+22
   STA   HARGS+6
   LDA   HARGS+23
   STA   HARGS+7
   JSR   d2i
   LDA   HARGS+9
   BNE   .thread@0
   LDA   HARGS+8
   CMP   #$6B
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

   LDA   #$00
   STA   __zpabi_lt__d_0
   STA   __zpabi_lt__d_1
   STA   __zpabi_lt__d_2
   STA   __zpabi_lt__d_3
   STA   __zpabi_lt__d_4
   LDA   #$40
   STA   __zpabi_lt__d_5
   LDA   #$8F
   STA   __zpabi_lt__d_6
   LDA   #$C0
   STA   __zpabi_lt__d_7
   LDA   #$18
   STA   __zpabi_lt__l_0
   LDA   #$FC
   STA   __zpabi_lt__l_1
   LDA   #$FF
   STA   __zpabi_lt__l_2
   STA   __zpabi_lt__l_3
   STA   __zpabi_lt__l_4
   STA   __zpabi_lt__l_5
   STA   __zpabi_lt__l_6
   STA   __zpabi_lt__l_7
   JSR   lt
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$01
   RTS
.if_end@4:
   JSR   main@run0
   LDA   #$34
   STA   __zpabi_tern_double_flag__flag_6
   LDA   #$40
   STA   __zpabi_tern_double_flag__flag_7
   JSR   tern_double_flag
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@3
   LDA   HARGS+22
   CMP   #$EF
   BNE   .jcmp_skip@3
   LDA   HARGS+21
   CMP   #$FC
   BNE   .jcmp_skip@3
   LDA   HARGS+20
   CMP   #$40
   BNE   .jcmp_skip@3
   LDA   HARGS+19
   BNE   .jcmp_skip@3
   LDA   HARGS+18
   BNE   .jcmp_skip@3
   LDA   HARGS+17
   BNE   .jcmp_skip@3
   LDA   HARGS+16
   BEQ   .if_end@5
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@5:
   JSR   main@run0
   STA   __zpabi_tern_double_flag__flag_6
   STA   __zpabi_tern_double_flag__flag_7
   JSR   tern_double_flag
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@4
   LDA   HARGS+22
   CMP   #$24
   BNE   .jcmp_skip@4
   LDA   HARGS+21
   BNE   .jcmp_skip@4
   LDA   HARGS+20
   BNE   .jcmp_skip@4
   LDA   HARGS+19
   BNE   .jcmp_skip@4
   LDA   HARGS+18
   BNE   .jcmp_skip@4
   LDA   HARGS+17
   BNE   .jcmp_skip@4
   LDA   HARGS+16
   BEQ   .if_end@6
.jcmp_skip@4:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@6:
   LDX   #$00
   LDA   #$01
   JSR   tern_double_result
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@5
   LDA   HARGS+22
   CMP   #$14
   BNE   .jcmp_skip@5
   LDA   HARGS+21
   BNE   .jcmp_skip@5
   LDA   HARGS+20
   BNE   .jcmp_skip@5
   LDA   HARGS+19
   BNE   .jcmp_skip@5
   LDA   HARGS+18
   BNE   .jcmp_skip@5
   LDA   HARGS+17
   BNE   .jcmp_skip@5
   LDA   HARGS+16
   BEQ   .if_end@7
.jcmp_skip@5:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@7:
   LDX   #$00
   TXA
   JSR   tern_double_result
   LDA   HARGS+23
   CMP   #$41
   BNE   .jcmp_skip@6
   LDA   HARGS+22
   CMP   #$ED
   BNE   .jcmp_skip@6
   LDA   HARGS+21
   CMP   #$CD
   BNE   .jcmp_skip@6
   LDA   HARGS+20
   CMP   #$65
   BNE   .jcmp_skip@6
   LDA   HARGS+19
   BNE   .jcmp_skip@6
   LDA   HARGS+18
   BNE   .jcmp_skip@6
   LDA   HARGS+17
   BNE   .jcmp_skip@6
   LDA   HARGS+16
   BEQ   .if_end@8
.jcmp_skip@6:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@8:
   JSR   multiply
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@9
   LDX   #$00
   LDA   #$06
   RTS
.if_end@9:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_tern_double_flag__flag_0
   STA   __zpabi_tern_double_flag__flag_1
   STA   __zpabi_tern_double_flag__flag_2
   STA   __zpabi_tern_double_flag__flag_3
   STA   __zpabi_tern_double_flag__flag_4
   STA   __zpabi_tern_double_flag__flag_5
   RTS

ten:
   DC.W  $000A
