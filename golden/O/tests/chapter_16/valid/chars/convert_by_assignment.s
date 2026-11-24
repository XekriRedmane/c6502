SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_check_char__expected	EQU	$80
__local_check_uchar__expected	EQU	$80
__local_return_extended_schar__sc	EQU	$80
__zpabi_check_char_on_stack__dummy1_0	EQU	$80
__zpabi_check_double__converted_0	EQU	$80
__zpabi_check_int__expected_0	EQU	$80
__zpabi_check_long__converted_0	EQU	$80
__zpabi_check_uint__expected_0	EQU	$80
__zpabi_check_ulong__converted_0	EQU	$80
__zpabi_return_truncated_long__l_0	EQU	$80
__zpabi_check_char_on_stack__dummy1_1	EQU	$81
__zpabi_check_double__converted_1	EQU	$81
__zpabi_check_int__expected_1	EQU	$81
__zpabi_check_long__converted_1	EQU	$81
__zpabi_check_uint__expected_1	EQU	$81
__zpabi_check_ulong__converted_1	EQU	$81
__zpabi_return_truncated_long__l_1	EQU	$81
__zpabi_check_char_on_stack__dummy2_0	EQU	$82
__zpabi_check_double__converted_2	EQU	$82
__zpabi_check_long__converted_2	EQU	$82
__zpabi_check_ulong__converted_2	EQU	$82
__zpabi_return_truncated_long__l_2	EQU	$82
__zpabi_check_char_on_stack__dummy2_1	EQU	$83
__zpabi_check_double__converted_3	EQU	$83
__zpabi_check_long__converted_3	EQU	$83
__zpabi_check_ulong__converted_3	EQU	$83
__zpabi_return_truncated_long__l_3	EQU	$83
__zpabi_check_char_on_stack__dummy3_0	EQU	$84
__zpabi_check_double__converted_4	EQU	$84
__zpabi_check_long__expected_0	EQU	$84
__zpabi_check_ulong__expected_0	EQU	$84
__zpabi_check_char_on_stack__dummy3_1	EQU	$85
__zpabi_check_double__converted_5	EQU	$85
__zpabi_check_long__expected_1	EQU	$85
__zpabi_check_ulong__expected_1	EQU	$85
__zpabi_check_char_on_stack__dummy4_0	EQU	$86
__zpabi_check_double__converted_6	EQU	$86
__zpabi_check_long__expected_2	EQU	$86
__zpabi_check_ulong__expected_2	EQU	$86
__zpabi_check_char_on_stack__dummy4_1	EQU	$87
__zpabi_check_double__converted_7	EQU	$87
__zpabi_check_long__expected_3	EQU	$87
__zpabi_check_ulong__expected_3	EQU	$87
__zpabi_check_char_on_stack__dummy5_0	EQU	$88
__zpabi_check_double__expected_0	EQU	$88
__zpabi_check_char_on_stack__dummy5_1	EQU	$89
__zpabi_check_double__expected_1	EQU	$89
__zpabi_check_char_on_stack__dummy6_0	EQU	$8A
__zpabi_check_double__expected_2	EQU	$8A
__zpabi_check_char_on_stack__dummy6_1	EQU	$8B
__zpabi_check_double__expected_3	EQU	$8B
__zpabi_check_char_on_stack__converted	EQU	$8C
__zpabi_check_double__expected_4	EQU	$8C
__local_check_char_on_stack__0	EQU	$8D
__zpabi_check_double__expected_5	EQU	$8D
__local_check_char_on_stack__1	EQU	$8E
__zpabi_check_double__expected_6	EQU	$8E
__local_check_char_on_stack__expected	EQU	$8F
__zpabi_check_double__expected_7	EQU	$8F
__local_main__uchar_array_1	EQU	$90
__local_main__uchar_array_2	EQU	$91

; @zp-link-meta-begin
; def check_char params=__zpabi_check_char__converted,__zpabi_check_char__expected locals=__local_check_char__expected,__local_check_char__converted indirect=false in_cycle=false param_regs=A,X
; def check_char_on_stack params=__zpabi_check_char_on_stack__expected,__zpabi_check_char_on_stack__dummy1_0,__zpabi_check_char_on_stack__dummy1_1,__zpabi_check_char_on_stack__dummy2_0,__zpabi_check_char_on_stack__dummy2_1,__zpabi_check_char_on_stack__dummy3_0,__zpabi_check_char_on_stack__dummy3_1,__zpabi_check_char_on_stack__dummy4_0,__zpabi_check_char_on_stack__dummy4_1,__zpabi_check_char_on_stack__dummy5_0,__zpabi_check_char_on_stack__dummy5_1,__zpabi_check_char_on_stack__dummy6_0,__zpabi_check_char_on_stack__dummy6_1,__zpabi_check_char_on_stack__converted locals=__local_check_char_on_stack__0,__local_check_char_on_stack__1,__local_check_char_on_stack__expected indirect=false in_cycle=false param_regs=A,,,,,,,,,,,,,
; def check_double params=__zpabi_check_double__converted_0,__zpabi_check_double__converted_1,__zpabi_check_double__converted_2,__zpabi_check_double__converted_3,__zpabi_check_double__converted_4,__zpabi_check_double__converted_5,__zpabi_check_double__converted_6,__zpabi_check_double__converted_7,__zpabi_check_double__expected_0,__zpabi_check_double__expected_1,__zpabi_check_double__expected_2,__zpabi_check_double__expected_3,__zpabi_check_double__expected_4,__zpabi_check_double__expected_5,__zpabi_check_double__expected_6,__zpabi_check_double__expected_7 locals=__local_check_double__0 indirect=false in_cycle=false
; def check_int params=__zpabi_check_int__converted_0,__zpabi_check_int__converted_1,__zpabi_check_int__expected_0,__zpabi_check_int__expected_1 locals=__local_check_int__converted_1,__local_check_int__converted_0 indirect=false in_cycle=false param_regs=A,X,,
; def check_long params=__zpabi_check_long__converted_0,__zpabi_check_long__converted_1,__zpabi_check_long__converted_2,__zpabi_check_long__converted_3,__zpabi_check_long__expected_0,__zpabi_check_long__expected_1,__zpabi_check_long__expected_2,__zpabi_check_long__expected_3 locals=__local_check_long__0 indirect=false in_cycle=false
; def check_uchar params=__zpabi_check_uchar__converted,__zpabi_check_uchar__expected locals=__local_check_uchar__expected,__local_check_uchar__converted indirect=false in_cycle=false param_regs=A,X
; def check_uint params=__zpabi_check_uint__converted_0,__zpabi_check_uint__converted_1,__zpabi_check_uint__expected_0,__zpabi_check_uint__expected_1 locals=__local_check_uint__converted_1,__local_check_uint__converted_0 indirect=false in_cycle=false param_regs=A,X,,
; def check_ulong params=__zpabi_check_ulong__converted_0,__zpabi_check_ulong__converted_1,__zpabi_check_ulong__converted_2,__zpabi_check_ulong__converted_3,__zpabi_check_ulong__expected_0,__zpabi_check_ulong__expected_1,__zpabi_check_ulong__expected_2,__zpabi_check_ulong__expected_3 locals=__local_check_ulong__0 indirect=false in_cycle=false
; def main params= locals=__local_main__uchar_array_1,__local_main__uchar_array_2,__local_main__uchar_array_0,__local_main__array_0,__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def return_extended_schar params=__zpabi_return_extended_schar__sc locals=__local_return_extended_schar__sc indirect=false in_cycle=false param_regs=A
; def return_extended_uchar params=__zpabi_return_extended_uchar__c locals=__local_return_extended_uchar__c indirect=false in_cycle=false param_regs=A
; def return_truncated_long params=__zpabi_return_truncated_long__l_0,__zpabi_return_truncated_long__l_1,__zpabi_return_truncated_long__l_2,__zpabi_return_truncated_long__l_3 locals= indirect=false in_cycle=false
; call main -> check_char
; call main -> check_char_on_stack
; call main -> check_double
; call main -> check_int
; call main -> check_long
; call main -> check_uchar
; call main -> check_uint
; call main -> check_ulong
; call main -> return_extended_schar
; call main -> return_extended_uchar
; call main -> return_truncated_long
; item check_int section=.text local=false zero=false
; item check_uint section=.text local=false zero=false
; item check_long section=.text local=false zero=false
; item check_ulong section=.text local=false zero=false
; item check_double section=.text local=false zero=false
; item check_char section=.text local=false zero=false
; item check_char@tail0 section=.text local=true zero=false
; item check_uchar section=.text local=false zero=false
; item check_char_on_stack section=.text local=false zero=false
; item return_extended_uchar section=.text local=false zero=false
; item return_extended_schar section=.text local=false zero=false
; item return_truncated_long section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @32.uc_static section=.bss local=true zero=true
; item @34.s_static section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

check_int:
   SUBROUTINE

   CPX   __zpabi_check_int__expected_1
   BNE   .thread@0
   CMP   __zpabi_check_int__expected_0
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

check_uint:
   SUBROUTINE

   CPX   __zpabi_check_uint__expected_1
   BNE   .thread@1
   CMP   __zpabi_check_uint__expected_0
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

check_long:
   SUBROUTINE

   LDA   __zpabi_check_long__converted_3
   CMP   __zpabi_check_long__expected_3
   BNE   .thread@2
   LDA   __zpabi_check_long__converted_2
   CMP   __zpabi_check_long__expected_2
   BNE   .thread@2
   LDA   __zpabi_check_long__converted_1
   CMP   __zpabi_check_long__expected_1
   BNE   .thread@2
   LDA   __zpabi_check_long__converted_0
   CMP   __zpabi_check_long__expected_0
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDX   #$00
   RTS

check_ulong:
   SUBROUTINE

   LDA   __zpabi_check_ulong__converted_3
   CMP   __zpabi_check_ulong__expected_3
   BNE   .thread@3
   LDA   __zpabi_check_ulong__converted_2
   CMP   __zpabi_check_ulong__expected_2
   BNE   .thread@3
   LDA   __zpabi_check_ulong__converted_1
   CMP   __zpabi_check_ulong__expected_1
   BNE   .thread@3
   LDA   __zpabi_check_ulong__converted_0
   CMP   __zpabi_check_ulong__expected_0
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDX   #$00
   RTS

check_double:
   SUBROUTINE

   LDA   __zpabi_check_double__converted_7
   CMP   __zpabi_check_double__expected_7
   BNE   .thread@4
   LDA   __zpabi_check_double__converted_6
   CMP   __zpabi_check_double__expected_6
   BNE   .thread@4
   LDA   __zpabi_check_double__converted_5
   CMP   __zpabi_check_double__expected_5
   BNE   .thread@4
   LDA   __zpabi_check_double__converted_4
   CMP   __zpabi_check_double__expected_4
   BNE   .thread@4
   LDA   __zpabi_check_double__converted_3
   CMP   __zpabi_check_double__expected_3
   BNE   .thread@4
   LDA   __zpabi_check_double__converted_2
   CMP   __zpabi_check_double__expected_2
   BNE   .thread@4
   LDA   __zpabi_check_double__converted_1
   CMP   __zpabi_check_double__expected_1
   BNE   .thread@4
   LDA   __zpabi_check_double__converted_0
   CMP   __zpabi_check_double__expected_0
   BEQ   .cmp_true@12
.thread@4:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDX   #$00
   RTS

check_char:
   SUBROUTINE

   STX   __local_check_char__expected
   CMP   __local_check_char__expected
   JMP   check_char@tail0

check_char@tail0:
   SUBROUTINE

   BEQ   .cmp_true@15
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDX   #$00
   RTS

check_uchar:
   SUBROUTINE

   STX   __local_check_uchar__expected
   CMP   __local_check_uchar__expected
   JMP   check_char@tail0

check_char_on_stack:
   SUBROUTINE

   STA   __local_check_char_on_stack__expected
   LDA   __zpabi_check_char_on_stack__converted
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_check_char_on_stack__1
   LDA   __local_check_char_on_stack__expected
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_check_char_on_stack__0
   LDA   __local_check_char_on_stack__1
   CMP   __local_check_char_on_stack__0
   BNE   .thread@5
   LDA   __zpabi_check_char_on_stack__converted
   CMP   __local_check_char_on_stack__expected
   BEQ   .cmp_true@23
.thread@5:
   LDA   #$00
   BEQ   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDX   #$00
   RTS

return_extended_uchar:
   SUBROUTINE

   LDX   #$00
   RTS

return_extended_schar:
   SUBROUTINE

   STA   __local_return_extended_schar__sc
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   HARGS+17
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   __local_return_extended_schar__sc
   STA   HARGS+16
   RTS

return_truncated_long:
   SUBROUTINE

   LDA   __zpabi_return_truncated_long__l_0
   RTS

main:
   SUBROUTINE

   LDA   #$F6
   STA   __zpabi_check_long__converted_0
   STA   __zpabi_check_long__expected_0
   LDA   #$FF
   STA   __zpabi_check_long__converted_1
   STA   __zpabi_check_long__converted_2
   STA   __zpabi_check_long__converted_3
   STA   __zpabi_check_long__expected_1
   STA   __zpabi_check_long__expected_2
   STA   __zpabi_check_long__expected_3
   JSR   check_long
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$F6
   STA   __zpabi_check_uint__expected_0
   LDA   #$FF
   STA   __zpabi_check_uint__expected_1
   LDX   #$FF
   LDA   #$F6
   JSR   check_uint
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$00
   STA   __zpabi_check_double__converted_0
   STA   __zpabi_check_double__converted_1
   STA   __zpabi_check_double__converted_2
   STA   __zpabi_check_double__converted_3
   STA   __zpabi_check_double__converted_4
   STA   __zpabi_check_double__converted_5
   STA   __zpabi_check_double__expected_0
   STA   __zpabi_check_double__expected_1
   STA   __zpabi_check_double__expected_2
   STA   __zpabi_check_double__expected_3
   STA   __zpabi_check_double__expected_4
   STA   __zpabi_check_double__expected_5
   LDA   #$24
   STA   __zpabi_check_double__converted_6
   STA   __zpabi_check_double__expected_6
   LDA   #$C0
   STA   __zpabi_check_double__converted_7
   STA   __zpabi_check_double__expected_7
   JSR   check_double
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$F6
   TXA
   JSR   check_uchar
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$F6
   TXA
   JSR   check_char
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$F6
   TXA
   JSR   check_char
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDX   #$F6
   TXA
   JSR   check_char
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   #$00
   STA   __zpabi_check_char_on_stack__dummy1_0
   STA   __zpabi_check_char_on_stack__dummy1_1
   STA   __zpabi_check_char_on_stack__dummy2_0
   STA   __zpabi_check_char_on_stack__dummy2_1
   STA   __zpabi_check_char_on_stack__dummy3_0
   STA   __zpabi_check_char_on_stack__dummy3_1
   STA   __zpabi_check_char_on_stack__dummy4_0
   STA   __zpabi_check_char_on_stack__dummy4_1
   STA   __zpabi_check_char_on_stack__dummy5_0
   STA   __zpabi_check_char_on_stack__dummy5_1
   STA   __zpabi_check_char_on_stack__dummy6_0
   STA   __zpabi_check_char_on_stack__dummy6_1
   LDA   #$F6
   STA   __zpabi_check_char_on_stack__converted
   JSR   check_char_on_stack
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   #$F6
   STA   __zpabi_check_int__expected_0
   LDA   #$00
   STA   __zpabi_check_int__expected_1
   LDX   #$00
   LDA   #$F6
   JSR   check_int
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   #$F6
   STA   __zpabi_check_ulong__converted_0
   STA   __zpabi_check_ulong__expected_0
   LDA   #$00
   STA   __zpabi_check_ulong__converted_1
   STA   __zpabi_check_ulong__converted_2
   STA   __zpabi_check_ulong__converted_3
   STA   __zpabi_check_ulong__expected_1
   STA   __zpabi_check_ulong__expected_2
   STA   __zpabi_check_ulong__expected_3
   JSR   check_ulong
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@9
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   LDX   #$F6
   TXA
   JSR   check_char
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@10
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   LDX   #$F6
   TXA
   JSR   check_uchar
   STX   __local_main__uchar_array_1
   ORA   __local_main__uchar_array_1
   BNE   .if_end@11
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   LDA   #$F6
   JSR   return_extended_uchar
   STA   __local_main__uchar_array_2
   TXA
   BNE   .jcmp_skip@28
   LDA   __local_main__uchar_array_2
   CMP   #$F6
   BEQ   .if_end@12
.jcmp_skip@28:
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@12:
   LDA   #$F6
   JSR   return_extended_schar
   LDA   HARGS+23
   CMP   #$FF
   BNE   .jcmp_skip@29
   LDA   HARGS+22
   CMP   #$FF
   BNE   .jcmp_skip@29
   LDA   HARGS+21
   CMP   #$FF
   BNE   .jcmp_skip@29
   LDA   HARGS+20
   CMP   #$FF
   BNE   .jcmp_skip@29
   LDA   HARGS+19
   CMP   #$FF
   BNE   .jcmp_skip@29
   LDA   HARGS+18
   CMP   #$FF
   BNE   .jcmp_skip@29
   LDA   HARGS+17
   CMP   #$FF
   BNE   .jcmp_skip@29
   LDA   HARGS+16
   CMP   #$F6
   BEQ   .if_end@13
.jcmp_skip@29:
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@13:
   LDA   #$F6
   STA   __zpabi_return_truncated_long__l_0
   LDA   #$00
   STA   __zpabi_return_truncated_long__l_1
   LDA   #$08
   STA   __zpabi_return_truncated_long__l_2
   LDA   #$40
   STA   __zpabi_return_truncated_long__l_3
   JSR   return_truncated_long
   CMP   #$F6
   BEQ   .if_end@14
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@14:
   LDA   #$00
   LDY   #$80
   ORA   #$00
   BNE   .main@jthread@1
   TYA
   BPL   .main@jthread@1
   CPY   #$80
   BEQ   .if_end@29
.main@jthread@1:
   LDX   #$00
   LDA   #$10
   RTS
.if_end@29:
   LDY   #$00
   BNE   .main@jthread@7
   BEQ   .if_end@34
.main@jthread@7:
   LDX   #$00
   LDA   #$13
   RTS
.if_end@34:
   TYA
   BNE   .main@jthread@9
   LDA   #$FA
   CMP   #$FA
   BEQ   .if_end@39
.main@jthread@9:
   LDX   #$00
   LDA   #$14
   RTS
.if_end@39:
   LDA   @32.uc_static
   BEQ   .if_end@40
   LDX   #$00
   LDA   #$15
   RTS
.if_end@40:
   LDA   @34.s_static+1
   BMI   .sx_neg@36
   TYA
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   STA   __local_main__uchar_array_2
   STA   __local_main__uchar_array_1
   LDA   @34.s_static
   ORA   @34.s_static+1
   ORA   __local_main__uchar_array_2
   ORA   __local_main__uchar_array_1
   BEQ   .if_end@41
   LDX   #$00
   LDA   #$16
   RTS
.if_end@41:
   LDX   #$00
   TXA
   RTS

@32.uc_static:
   DS.B  1

@34.s_static:
   DS.B  2
