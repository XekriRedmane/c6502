SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_is_negative_zero__d_0	EQU	$80
__zpabi_is_positive_zero__d_0	EQU	$80
__zpabi_is_negative_zero__d_1	EQU	$81
__zpabi_is_positive_zero__d_1	EQU	$81
__zpabi_is_negative_zero__d_2	EQU	$82
__zpabi_is_positive_zero__d_2	EQU	$82
__zpabi_is_negative_zero__d_3	EQU	$83
__zpabi_is_positive_zero__d_3	EQU	$83
__zpabi_is_negative_zero__d_4	EQU	$84
__zpabi_is_positive_zero__d_4	EQU	$84
__zpabi_is_negative_zero__d_5	EQU	$85
__zpabi_is_positive_zero__d_5	EQU	$85
__zpabi_is_negative_zero__d_6	EQU	$86
__zpabi_is_positive_zero__d_6	EQU	$86
__zpabi_is_negative_zero__d_7	EQU	$87
__zpabi_is_positive_zero__d_7	EQU	$87
__zpabi_copysign__x_0	EQU	$88
__zpabi_copysign__x_1	EQU	$89
__zpabi_copysign__x_2	EQU	$8A
__zpabi_copysign__x_3	EQU	$8B
__zpabi_copysign__x_4	EQU	$8C
__zpabi_copysign__x_5	EQU	$8D
__zpabi_copysign__x_6	EQU	$8E
__zpabi_copysign__x_7	EQU	$8F
__zpabi_copysign__y_0	EQU	$90
__zpabi_copysign__y_1	EQU	$91
__zpabi_copysign__y_2	EQU	$92
__zpabi_copysign__y_3	EQU	$93
__zpabi_copysign__y_4	EQU	$94
__zpabi_copysign__y_5	EQU	$95
__zpabi_copysign__y_6	EQU	$96
__zpabi_copysign__y_7	EQU	$97
__local_main__0	EQU	$98
__local_main__1	EQU	$99
__local_main__8	EQU	$A0
__local_main__9	EQU	$A1
__local_main__10	EQU	$A2
__local_main__11	EQU	$A3
__local_main__12	EQU	$A4
__local_main__13	EQU	$A5
__local_main__14	EQU	$A6
__local_main__15	EQU	$A7
__local_main__16	EQU	$A8
__local_main__17	EQU	$A9
__local_main__18	EQU	$AA
__local_main__19	EQU	$AB
__local_main__20	EQU	$AC
__local_main__21	EQU	$AD
__local_main__22	EQU	$AE
__local_main__23	EQU	$AF

; @zp-link-meta-begin
; def is_negative_zero params=__zpabi_is_negative_zero__d_0,__zpabi_is_negative_zero__d_1,__zpabi_is_negative_zero__d_2,__zpabi_is_negative_zero__d_3,__zpabi_is_negative_zero__d_4,__zpabi_is_negative_zero__d_5,__zpabi_is_negative_zero__d_6,__zpabi_is_negative_zero__d_7 locals=__local_is_negative_zero__0,__local_is_negative_zero__1,__local_is_negative_zero__2,__local_is_negative_zero__3,__local_is_negative_zero__4,__local_is_negative_zero__5,__local_is_negative_zero__6,__local_is_negative_zero__7 indirect=false in_cycle=false
; def is_positive_zero params=__zpabi_is_positive_zero__d_0,__zpabi_is_positive_zero__d_1,__zpabi_is_positive_zero__d_2,__zpabi_is_positive_zero__d_3,__zpabi_is_positive_zero__d_4,__zpabi_is_positive_zero__d_5,__zpabi_is_positive_zero__d_6,__zpabi_is_positive_zero__d_7 locals=__local_is_positive_zero__0,__local_is_positive_zero__1,__local_is_positive_zero__2,__local_is_positive_zero__3,__local_is_positive_zero__4,__local_is_positive_zero__5,__local_is_positive_zero__6,__local_is_positive_zero__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23 indirect=false in_cycle=false
; def target_add_infinity params= locals= indirect=false in_cycle=false
; def target_add_overflow params= locals= indirect=false in_cycle=false
; def target_div_infinity_by_num params= locals= indirect=false in_cycle=false
; def target_div_overflow params= locals= indirect=false in_cycle=false
; def target_infinity_equal params= locals= indirect=false in_cycle=false
; def target_infinity_gt params= locals= indirect=false in_cycle=false
; def target_infinity_is_true params= locals= indirect=false in_cycle=false
; def target_mult_infinity params= locals= indirect=false in_cycle=false
; def target_mult_neg_infinity params= locals= indirect=false in_cycle=false
; def target_mult_neg_infinity_by_neg_number params= locals= indirect=false in_cycle=false
; def target_mult_overflow params= locals= indirect=false in_cycle=false
; def target_neg_infinity_lt params= locals= indirect=false in_cycle=false
; def target_negate_inf params= locals= indirect=false in_cycle=false
; def target_negative_zero_div_by_inf params= locals= indirect=false in_cycle=false
; def target_sub_infinity params= locals= indirect=false in_cycle=false
; def target_sub_overflow params= locals= indirect=false in_cycle=false
; def target_zero_div_by_inf params= locals= indirect=false in_cycle=false
; def target_zero_div_by_neg_inf params= locals= indirect=false in_cycle=false
; ext copysign params=__zpabi_copysign__x_0,__zpabi_copysign__x_1,__zpabi_copysign__x_2,__zpabi_copysign__x_3,__zpabi_copysign__x_4,__zpabi_copysign__x_5,__zpabi_copysign__x_6,__zpabi_copysign__x_7,__zpabi_copysign__y_0,__zpabi_copysign__y_1,__zpabi_copysign__y_2,__zpabi_copysign__y_3,__zpabi_copysign__y_4,__zpabi_copysign__y_5,__zpabi_copysign__y_6,__zpabi_copysign__y_7
; call is_negative_zero -> copysign
; call is_positive_zero -> copysign
; call main -> is_negative_zero
; call main -> is_positive_zero
; call main -> target_add_infinity
; call main -> target_add_overflow
; call main -> target_div_infinity_by_num
; call main -> target_div_overflow
; call main -> target_infinity_equal
; call main -> target_infinity_gt
; call main -> target_infinity_is_true
; call main -> target_mult_infinity
; call main -> target_mult_neg_infinity
; call main -> target_mult_neg_infinity_by_neg_number
; call main -> target_mult_overflow
; call main -> target_neg_infinity_lt
; call main -> target_negate_inf
; call main -> target_negative_zero_div_by_inf
; call main -> target_sub_infinity
; call main -> target_sub_overflow
; call main -> target_zero_div_by_inf
; call main -> target_zero_div_by_neg_inf
; item is_positive_zero section=.text local=false zero=false
; item is_positive_zero@run2 section=.text local=true zero=false
; item is_positive_zero@tail2 section=.text local=true zero=false
; item is_negative_zero section=.text local=false zero=false
; item target_infinity_equal section=.text local=false zero=false
; item target_infinity_equal@tail4 section=.text local=true zero=false
; item target_infinity_gt section=.text local=false zero=false
; item target_neg_infinity_lt section=.text local=false zero=false
; item target_add_infinity section=.text local=false zero=false
; item target_add_infinity@tail0 section=.text local=true zero=false
; item target_add_infinity@tail0@run3 section=.text local=true zero=false
; item target_add_infinity@tail0@run0 section=.text local=true zero=false
; item target_sub_infinity section=.text local=false zero=false
; item target_mult_infinity section=.text local=false zero=false
; item target_mult_neg_infinity section=.text local=false zero=false
; item target_mult_neg_infinity@tail1 section=.text local=true zero=false
; item target_mult_neg_infinity_by_neg_number section=.text local=false zero=false
; item target_div_infinity_by_num section=.text local=false zero=false
; item target_negate_inf section=.text local=false zero=false
; item target_mult_overflow section=.text local=false zero=false
; item target_div_overflow section=.text local=false zero=false
; item target_add_overflow section=.text local=false zero=false
; item target_sub_overflow section=.text local=false zero=false
; item target_zero_div_by_inf section=.text local=false zero=false
; item target_zero_div_by_neg_inf section=.text local=false zero=false
; item target_zero_div_by_neg_inf@tail3 section=.text local=true zero=false
; item target_negative_zero_div_by_inf section=.text local=false zero=false
; item target_infinity_is_true section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item zero section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

is_positive_zero:
   SUBROUTINE

   LDA   __zpabi_is_positive_zero__d_0
   ORA   __zpabi_is_positive_zero__d_1
   ORA   __zpabi_is_positive_zero__d_2
   ORA   __zpabi_is_positive_zero__d_3
   ORA   __zpabi_is_positive_zero__d_4
   ORA   __zpabi_is_positive_zero__d_5
   ORA   __zpabi_is_positive_zero__d_6
   ORA   __zpabi_is_positive_zero__d_7
   BEQ   .if_end@0
   LDX   #$00
   TXA
   RTS
.if_end@0:
   JSR   is_positive_zero@run2
   LDA   __zpabi_is_positive_zero__d_0
   STA   __zpabi_copysign__y_0
   LDA   __zpabi_is_positive_zero__d_1
   STA   __zpabi_copysign__y_1
   LDA   __zpabi_is_positive_zero__d_2
   STA   __zpabi_copysign__y_2
   LDA   __zpabi_is_positive_zero__d_3
   STA   __zpabi_copysign__y_3
   LDA   __zpabi_is_positive_zero__d_4
   STA   __zpabi_copysign__y_4
   LDA   __zpabi_is_positive_zero__d_5
   STA   __zpabi_copysign__y_5
   LDA   __zpabi_is_positive_zero__d_6
   STA   __zpabi_copysign__y_6
   LDA   __zpabi_is_positive_zero__d_7
   STA   __zpabi_copysign__y_7
   JSR   copysign
   LDA   HARGS+23
   CMP   #$40
   JMP   is_positive_zero@tail2

is_positive_zero@run2:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_copysign__x_0
   STA   __zpabi_copysign__x_1
   STA   __zpabi_copysign__x_2
   STA   __zpabi_copysign__x_3
   STA   __zpabi_copysign__x_4
   STA   __zpabi_copysign__x_5
   LDA   #$14
   STA   __zpabi_copysign__x_6
   LDA   #$40
   STA   __zpabi_copysign__x_7
   RTS

is_positive_zero@tail2:
   SUBROUTINE

   BNE   .thread@0
   LDA   HARGS+22
   CMP   #$14
   BNE   .thread@0
   LDA   HARGS+21
   BNE   .thread@0
   LDA   HARGS+20
   BNE   .thread@0
   LDA   HARGS+19
   BNE   .thread@0
   LDA   HARGS+18
   BNE   .thread@0
   LDA   HARGS+17
   BNE   .thread@0
   LDA   HARGS+16
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

is_negative_zero:
   SUBROUTINE

   LDA   __zpabi_is_negative_zero__d_0
   ORA   __zpabi_is_negative_zero__d_1
   ORA   __zpabi_is_negative_zero__d_2
   ORA   __zpabi_is_negative_zero__d_3
   ORA   __zpabi_is_negative_zero__d_4
   ORA   __zpabi_is_negative_zero__d_5
   ORA   __zpabi_is_negative_zero__d_6
   ORA   __zpabi_is_negative_zero__d_7
   BEQ   .if_end@1
   LDX   #$00
   TXA
   RTS
.if_end@1:
   JSR   is_positive_zero@run2
   LDA   __zpabi_is_negative_zero__d_0
   STA   __zpabi_copysign__y_0
   LDA   __zpabi_is_negative_zero__d_1
   STA   __zpabi_copysign__y_1
   LDA   __zpabi_is_negative_zero__d_2
   STA   __zpabi_copysign__y_2
   LDA   __zpabi_is_negative_zero__d_3
   STA   __zpabi_copysign__y_3
   LDA   __zpabi_is_negative_zero__d_4
   STA   __zpabi_copysign__y_4
   LDA   __zpabi_is_negative_zero__d_5
   STA   __zpabi_copysign__y_5
   LDA   __zpabi_is_negative_zero__d_6
   STA   __zpabi_copysign__y_6
   LDA   __zpabi_is_negative_zero__d_7
   STA   __zpabi_copysign__y_7
   JSR   copysign
   LDA   HARGS+23
   CMP   #$C0
   JMP   is_positive_zero@tail2

target_infinity_equal:
   SUBROUTINE

   JMP   target_infinity_equal@tail4

target_infinity_equal@tail4:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_infinity_gt:
   SUBROUTINE

   JMP   target_infinity_equal@tail4

target_neg_infinity_lt:
   SUBROUTINE

   JMP   target_infinity_equal@tail4

target_add_infinity:
   SUBROUTINE

   JMP   target_add_infinity@tail0

target_add_infinity@tail0:
   SUBROUTINE

   JSR   target_add_infinity@tail0@run3
   LDA   #$7F
   STA   HARGS+23
   RTS

target_add_infinity@tail0@run3:
   SUBROUTINE

   JSR   target_add_infinity@tail0@run0
   STA   HARGS+21
   LDA   #$F0
   STA   HARGS+22
   RTS

target_add_infinity@tail0@run0:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   RTS

target_sub_infinity:
   SUBROUTINE

   JMP   target_add_infinity@tail0

target_mult_infinity:
   SUBROUTINE

   JMP   target_add_infinity@tail0

target_mult_neg_infinity:
   SUBROUTINE

   JMP   target_mult_neg_infinity@tail1

target_mult_neg_infinity@tail1:
   SUBROUTINE

   JSR   target_add_infinity@tail0@run3
   LDA   #$FF
   STA   HARGS+23
   RTS

target_mult_neg_infinity_by_neg_number:
   SUBROUTINE

   JMP   target_add_infinity@tail0

target_div_infinity_by_num:
   SUBROUTINE

   JMP   target_add_infinity@tail0

target_negate_inf:
   SUBROUTINE

   JMP   target_mult_neg_infinity@tail1

target_mult_overflow:
   SUBROUTINE

   JMP   target_add_infinity@tail0

target_div_overflow:
   SUBROUTINE

   JMP   target_mult_neg_infinity@tail1

target_add_overflow:
   SUBROUTINE

   JMP   target_add_infinity@tail0

target_sub_overflow:
   SUBROUTINE

   JMP   target_mult_neg_infinity@tail1

target_zero_div_by_inf:
   SUBROUTINE

   JSR   target_add_infinity@tail0@run0
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

target_zero_div_by_neg_inf:
   SUBROUTINE

   JMP   target_zero_div_by_neg_inf@tail3

target_zero_div_by_neg_inf@tail3:
   SUBROUTINE

   JSR   target_add_infinity@tail0@run0
   STY   HARGS+21
   STA   HARGS+22
   LDA   #$80
   STA   HARGS+23
   RTS

target_negative_zero_div_by_inf:
   SUBROUTINE

   JMP   target_zero_div_by_neg_inf@tail3

target_infinity_is_true:
   SUBROUTINE

   JMP   target_infinity_equal@tail4

main:
   SUBROUTINE

   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$F0
   STA   HARGS+6
   LDA   #$3F
   STA   HARGS+7
   LDA   zero
   STA   HARGS+8
   LDA   zero+1
   STA   HARGS+9
   LDA   zero+2
   STA   HARGS+10
   LDA   zero+3
   STA   HARGS+11
   LDA   zero+4
   STA   HARGS+12
   LDA   zero+5
   STA   HARGS+13
   LDA   zero+6
   STA   HARGS+14
   LDA   zero+7
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+16
   STA   __local_main__23
   LDA   HARGS+17
   STA   __local_main__22
   LDA   HARGS+18
   STA   __local_main__21
   LDA   HARGS+19
   STA   __local_main__20
   LDA   HARGS+20
   STA   __local_main__19
   LDA   HARGS+21
   STA   __local_main__18
   LDA   HARGS+22
   STA   __local_main__17
   LDA   HARGS+23
   STA   __local_main__16
   JSR   target_infinity_equal
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   JMP   target_infinity_equal@tail4
.if_end@3:
   JSR   target_infinity_gt
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$02
   RTS
.if_end@4:
   JSR   target_neg_infinity_lt
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@5
   LDX   #$00
   LDA   #$03
   RTS
.if_end@5:
   JSR   target_add_infinity
   LDA   HARGS+23
   CMP   __local_main__16
   BNE   .jcmp_skip@6
   LDA   HARGS+22
   CMP   __local_main__17
   BNE   .jcmp_skip@6
   LDA   HARGS+21
   CMP   __local_main__18
   BNE   .jcmp_skip@6
   LDA   HARGS+20
   CMP   __local_main__19
   BNE   .jcmp_skip@6
   LDA   HARGS+19
   CMP   __local_main__20
   BNE   .jcmp_skip@6
   LDA   HARGS+18
   CMP   __local_main__21
   BNE   .jcmp_skip@6
   LDA   HARGS+17
   CMP   __local_main__22
   BNE   .jcmp_skip@6
   LDA   HARGS+16
   CMP   __local_main__23
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@6:
   JSR   target_sub_infinity
   LDA   HARGS+23
   CMP   __local_main__16
   BNE   .jcmp_skip@7
   LDA   HARGS+22
   CMP   __local_main__17
   BNE   .jcmp_skip@7
   LDA   HARGS+21
   CMP   __local_main__18
   BNE   .jcmp_skip@7
   LDA   HARGS+20
   CMP   __local_main__19
   BNE   .jcmp_skip@7
   LDA   HARGS+19
   CMP   __local_main__20
   BNE   .jcmp_skip@7
   LDA   HARGS+18
   CMP   __local_main__21
   BNE   .jcmp_skip@7
   LDA   HARGS+17
   CMP   __local_main__22
   BNE   .jcmp_skip@7
   LDA   HARGS+16
   CMP   __local_main__23
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@7:
   JSR   target_mult_infinity
   LDA   HARGS+23
   CMP   __local_main__16
   BNE   .jcmp_skip@8
   LDA   HARGS+22
   CMP   __local_main__17
   BNE   .jcmp_skip@8
   LDA   HARGS+21
   CMP   __local_main__18
   BNE   .jcmp_skip@8
   LDA   HARGS+20
   CMP   __local_main__19
   BNE   .jcmp_skip@8
   LDA   HARGS+19
   CMP   __local_main__20
   BNE   .jcmp_skip@8
   LDA   HARGS+18
   CMP   __local_main__21
   BNE   .jcmp_skip@8
   LDA   HARGS+17
   CMP   __local_main__22
   BNE   .jcmp_skip@8
   LDA   HARGS+16
   CMP   __local_main__23
   BEQ   .if_end@8
.jcmp_skip@8:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@8:
   JSR   target_mult_neg_infinity
   LDA   __local_main__23
   STA   __local_main__15
   LDA   __local_main__22
   STA   __local_main__14
   LDA   __local_main__21
   STA   __local_main__13
   LDA   __local_main__20
   STA   __local_main__12
   LDA   __local_main__19
   STA   __local_main__11
   LDA   __local_main__18
   STA   __local_main__10
   LDA   __local_main__17
   STA   __local_main__9
   LDA   __local_main__16
   EOR   #$80
   STA   __local_main__8
   LDA   HARGS+23
   CMP   __local_main__8
   BNE   .jcmp_skip@9
   LDA   HARGS+22
   CMP   __local_main__9
   BNE   .jcmp_skip@9
   LDA   HARGS+21
   CMP   __local_main__10
   BNE   .jcmp_skip@9
   LDA   HARGS+20
   CMP   __local_main__11
   BNE   .jcmp_skip@9
   LDA   HARGS+19
   CMP   __local_main__12
   BNE   .jcmp_skip@9
   LDA   HARGS+18
   CMP   __local_main__13
   BNE   .jcmp_skip@9
   LDA   HARGS+17
   CMP   __local_main__14
   BNE   .jcmp_skip@9
   LDA   HARGS+16
   CMP   __local_main__15
   BEQ   .if_end@9
.jcmp_skip@9:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@9:
   JSR   target_mult_neg_infinity_by_neg_number
   LDA   HARGS+23
   CMP   __local_main__16
   BNE   .jcmp_skip@10
   LDA   HARGS+22
   CMP   __local_main__17
   BNE   .jcmp_skip@10
   LDA   HARGS+21
   CMP   __local_main__18
   BNE   .jcmp_skip@10
   LDA   HARGS+20
   CMP   __local_main__19
   BNE   .jcmp_skip@10
   LDA   HARGS+19
   CMP   __local_main__20
   BNE   .jcmp_skip@10
   LDA   HARGS+18
   CMP   __local_main__21
   BNE   .jcmp_skip@10
   LDA   HARGS+17
   CMP   __local_main__22
   BNE   .jcmp_skip@10
   LDA   HARGS+16
   CMP   __local_main__23
   BEQ   .if_end@10
.jcmp_skip@10:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@10:
   JSR   target_div_infinity_by_num
   LDA   HARGS+23
   CMP   __local_main__16
   BNE   .jcmp_skip@11
   LDA   HARGS+22
   CMP   __local_main__17
   BNE   .jcmp_skip@11
   LDA   HARGS+21
   CMP   __local_main__18
   BNE   .jcmp_skip@11
   LDA   HARGS+20
   CMP   __local_main__19
   BNE   .jcmp_skip@11
   LDA   HARGS+19
   CMP   __local_main__20
   BNE   .jcmp_skip@11
   LDA   HARGS+18
   CMP   __local_main__21
   BNE   .jcmp_skip@11
   LDA   HARGS+17
   CMP   __local_main__22
   BNE   .jcmp_skip@11
   LDA   HARGS+16
   CMP   __local_main__23
   BEQ   .if_end@11
.jcmp_skip@11:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@11:
   JSR   target_negate_inf
   LDA   HARGS+23
   CMP   __local_main__8
   BNE   .jcmp_skip@12
   LDA   HARGS+22
   CMP   __local_main__9
   BNE   .jcmp_skip@12
   LDA   HARGS+21
   CMP   __local_main__10
   BNE   .jcmp_skip@12
   LDA   HARGS+20
   CMP   __local_main__11
   BNE   .jcmp_skip@12
   LDA   HARGS+19
   CMP   __local_main__12
   BNE   .jcmp_skip@12
   LDA   HARGS+18
   CMP   __local_main__13
   BNE   .jcmp_skip@12
   LDA   HARGS+17
   CMP   __local_main__14
   BNE   .jcmp_skip@12
   LDA   HARGS+16
   CMP   __local_main__15
   BEQ   .if_end@12
.jcmp_skip@12:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@12:
   JSR   target_mult_overflow
   LDA   HARGS+23
   CMP   __local_main__16
   BNE   .jcmp_skip@13
   LDA   HARGS+22
   CMP   __local_main__17
   BNE   .jcmp_skip@13
   LDA   HARGS+21
   CMP   __local_main__18
   BNE   .jcmp_skip@13
   LDA   HARGS+20
   CMP   __local_main__19
   BNE   .jcmp_skip@13
   LDA   HARGS+19
   CMP   __local_main__20
   BNE   .jcmp_skip@13
   LDA   HARGS+18
   CMP   __local_main__21
   BNE   .jcmp_skip@13
   LDA   HARGS+17
   CMP   __local_main__22
   BNE   .jcmp_skip@13
   LDA   HARGS+16
   CMP   __local_main__23
   BEQ   .if_end@13
.jcmp_skip@13:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@13:
   JSR   target_div_overflow
   LDA   HARGS+23
   CMP   __local_main__8
   BNE   .jcmp_skip@14
   LDA   HARGS+22
   CMP   __local_main__9
   BNE   .jcmp_skip@14
   LDA   HARGS+21
   CMP   __local_main__10
   BNE   .jcmp_skip@14
   LDA   HARGS+20
   CMP   __local_main__11
   BNE   .jcmp_skip@14
   LDA   HARGS+19
   CMP   __local_main__12
   BNE   .jcmp_skip@14
   LDA   HARGS+18
   CMP   __local_main__13
   BNE   .jcmp_skip@14
   LDA   HARGS+17
   CMP   __local_main__14
   BNE   .jcmp_skip@14
   LDA   HARGS+16
   CMP   __local_main__15
   BEQ   .if_end@14
.jcmp_skip@14:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@14:
   JSR   target_add_overflow
   LDA   HARGS+23
   CMP   __local_main__16
   BNE   .jcmp_skip@15
   LDA   HARGS+22
   CMP   __local_main__17
   BNE   .jcmp_skip@15
   LDA   HARGS+21
   CMP   __local_main__18
   BNE   .jcmp_skip@15
   LDA   HARGS+20
   CMP   __local_main__19
   BNE   .jcmp_skip@15
   LDA   HARGS+19
   CMP   __local_main__20
   BNE   .jcmp_skip@15
   LDA   HARGS+18
   CMP   __local_main__21
   BNE   .jcmp_skip@15
   LDA   HARGS+17
   CMP   __local_main__22
   BNE   .jcmp_skip@15
   LDA   HARGS+16
   CMP   __local_main__23
   BEQ   .if_end@15
.jcmp_skip@15:
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@15:
   JSR   target_sub_overflow
   LDA   HARGS+23
   CMP   __local_main__8
   BNE   .jcmp_skip@16
   LDA   HARGS+22
   CMP   __local_main__9
   BNE   .jcmp_skip@16
   LDA   HARGS+21
   CMP   __local_main__10
   BNE   .jcmp_skip@16
   LDA   HARGS+20
   CMP   __local_main__11
   BNE   .jcmp_skip@16
   LDA   HARGS+19
   CMP   __local_main__12
   BNE   .jcmp_skip@16
   LDA   HARGS+18
   CMP   __local_main__13
   BNE   .jcmp_skip@16
   LDA   HARGS+17
   CMP   __local_main__14
   BNE   .jcmp_skip@16
   LDA   HARGS+16
   CMP   __local_main__15
   BEQ   .if_end@16
.jcmp_skip@16:
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@16:
   JSR   target_zero_div_by_inf
   LDA   HARGS+16
   STA   __zpabi_is_positive_zero__d_0
   LDA   HARGS+17
   STA   __zpabi_is_positive_zero__d_1
   LDA   HARGS+18
   STA   __zpabi_is_positive_zero__d_2
   LDA   HARGS+19
   STA   __zpabi_is_positive_zero__d_3
   LDA   HARGS+20
   STA   __zpabi_is_positive_zero__d_4
   LDA   HARGS+21
   STA   __zpabi_is_positive_zero__d_5
   LDA   HARGS+22
   STA   __zpabi_is_positive_zero__d_6
   LDA   HARGS+23
   STA   __zpabi_is_positive_zero__d_7
   JSR   is_positive_zero
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@17
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@17:
   JSR   target_zero_div_by_neg_inf
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@18
   LDX   #$00
   LDA   #$10
   RTS
.if_end@18:
   JSR   target_negative_zero_div_by_inf
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@19
   LDX   #$00
   LDA   #$11
   RTS
.if_end@19:
   JSR   target_infinity_is_true
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@17
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@20
.jcmp_skip@17:
   LDX   #$00
   LDA   #$12
   RTS
.if_end@20:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   LDA   HARGS+16
   STA   __zpabi_is_negative_zero__d_0
   LDA   HARGS+17
   STA   __zpabi_is_negative_zero__d_1
   LDA   HARGS+18
   STA   __zpabi_is_negative_zero__d_2
   LDA   HARGS+19
   STA   __zpabi_is_negative_zero__d_3
   LDA   HARGS+20
   STA   __zpabi_is_negative_zero__d_4
   LDA   HARGS+21
   STA   __zpabi_is_negative_zero__d_5
   LDA   HARGS+22
   STA   __zpabi_is_negative_zero__d_6
   LDA   HARGS+23
   STA   __zpabi_is_negative_zero__d_7
   JMP   is_negative_zero

zero:
   DS.B  8
