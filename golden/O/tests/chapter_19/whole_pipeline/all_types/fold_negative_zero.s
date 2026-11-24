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

; @zp-link-meta-begin
; def is_negative_zero params=__zpabi_is_negative_zero__d_0,__zpabi_is_negative_zero__d_1,__zpabi_is_negative_zero__d_2,__zpabi_is_negative_zero__d_3,__zpabi_is_negative_zero__d_4,__zpabi_is_negative_zero__d_5,__zpabi_is_negative_zero__d_6,__zpabi_is_negative_zero__d_7 locals=__local_is_negative_zero__0,__local_is_negative_zero__1,__local_is_negative_zero__2,__local_is_negative_zero__3,__local_is_negative_zero__4,__local_is_negative_zero__5,__local_is_negative_zero__6,__local_is_negative_zero__7 indirect=false in_cycle=false
; def is_positive_zero params=__zpabi_is_positive_zero__d_0,__zpabi_is_positive_zero__d_1,__zpabi_is_positive_zero__d_2,__zpabi_is_positive_zero__d_3,__zpabi_is_positive_zero__d_4,__zpabi_is_positive_zero__d_5,__zpabi_is_positive_zero__d_6,__zpabi_is_positive_zero__d_7 locals=__local_is_positive_zero__0,__local_is_positive_zero__1,__local_is_positive_zero__2,__local_is_positive_zero__3,__local_is_positive_zero__4,__local_is_positive_zero__5,__local_is_positive_zero__6,__local_is_positive_zero__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_add_negative_nonzero_to_negative_zero params= locals= indirect=false in_cycle=false
; def target_add_negative_to_positive_zero params= locals= indirect=false in_cycle=false
; def target_add_negative_zero_to_self params= locals= indirect=false in_cycle=false
; def target_add_positive_nonzero_to_negative_zero params= locals= indirect=false in_cycle=false
; def target_add_positive_to_negative_zero params= locals= indirect=false in_cycle=false
; def target_div_neg_nonzero_by_neg_zero params= locals= indirect=false in_cycle=false
; def target_div_neg_nonzero_by_zero params= locals= indirect=false in_cycle=false
; def target_div_neg_zero_by_neg_nonzero params= locals= indirect=false in_cycle=false
; def target_div_neg_zero_by_pos_nonzero params= locals= indirect=false in_cycle=false
; def target_div_negative_underflow params= locals= indirect=false in_cycle=false
; def target_div_pos_non_zero_by_neg_zero params= locals= indirect=false in_cycle=false
; def target_div_pos_zero_by_neg_nonzero params= locals= indirect=false in_cycle=false
; def target_mult_underflow params= locals= indirect=false in_cycle=false
; def target_neg_zero_and params= locals= indirect=false in_cycle=false
; def target_neg_zero_branch params= locals= indirect=false in_cycle=false
; def target_neg_zero_mult_zero params= locals= indirect=false in_cycle=false
; def target_negate_calculated_zero params= locals= indirect=false in_cycle=false
; def target_negate_neg_zero params= locals= indirect=false in_cycle=false
; def target_negative_zero_eq params= locals= indirect=false in_cycle=false
; def target_negative_zero_ge params= locals= indirect=false in_cycle=false
; def target_negative_zero_gt params= locals= indirect=false in_cycle=false
; def target_negative_zero_le params= locals= indirect=false in_cycle=false
; def target_negative_zero_lt params= locals= indirect=false in_cycle=false
; def target_negative_zero_mult params= locals= indirect=false in_cycle=false
; def target_negative_zero_mult_negative params= locals= indirect=false in_cycle=false
; def target_negative_zero_neq params= locals= indirect=false in_cycle=false
; def target_negative_zero_squared params= locals= indirect=false in_cycle=false
; def target_not_neg_zero params= locals= indirect=false in_cycle=false
; def target_subtract_neg_zero_from_pos_zero params= locals= indirect=false in_cycle=false
; def target_subtract_neg_zero_from_self params= locals= indirect=false in_cycle=false
; def target_subtract_pos_nonzero_from_neg_zero params= locals= indirect=false in_cycle=false
; def target_subtract_pos_zero_from_neg_zero params= locals= indirect=false in_cycle=false
; ext copysign params=__zpabi_copysign__x_0,__zpabi_copysign__x_1,__zpabi_copysign__x_2,__zpabi_copysign__x_3,__zpabi_copysign__x_4,__zpabi_copysign__x_5,__zpabi_copysign__x_6,__zpabi_copysign__x_7,__zpabi_copysign__y_0,__zpabi_copysign__y_1,__zpabi_copysign__y_2,__zpabi_copysign__y_3,__zpabi_copysign__y_4,__zpabi_copysign__y_5,__zpabi_copysign__y_6,__zpabi_copysign__y_7
; call is_negative_zero -> copysign
; call is_positive_zero -> copysign
; call main -> is_negative_zero
; call main -> is_positive_zero
; call main -> target_add_negative_nonzero_to_negative_zero
; call main -> target_add_negative_to_positive_zero
; call main -> target_add_negative_zero_to_self
; call main -> target_add_positive_nonzero_to_negative_zero
; call main -> target_add_positive_to_negative_zero
; call main -> target_div_neg_nonzero_by_neg_zero
; call main -> target_div_neg_nonzero_by_zero
; call main -> target_div_neg_zero_by_neg_nonzero
; call main -> target_div_neg_zero_by_pos_nonzero
; call main -> target_div_negative_underflow
; call main -> target_div_pos_non_zero_by_neg_zero
; call main -> target_div_pos_zero_by_neg_nonzero
; call main -> target_mult_underflow
; call main -> target_neg_zero_and
; call main -> target_neg_zero_branch
; call main -> target_neg_zero_mult_zero
; call main -> target_negate_calculated_zero
; call main -> target_negate_neg_zero
; call main -> target_negative_zero_eq
; call main -> target_negative_zero_ge
; call main -> target_negative_zero_gt
; call main -> target_negative_zero_le
; call main -> target_negative_zero_lt
; call main -> target_negative_zero_mult
; call main -> target_negative_zero_mult_negative
; call main -> target_negative_zero_neq
; call main -> target_negative_zero_squared
; call main -> target_not_neg_zero
; call main -> target_subtract_neg_zero_from_pos_zero
; call main -> target_subtract_neg_zero_from_self
; call main -> target_subtract_pos_nonzero_from_neg_zero
; call main -> target_subtract_pos_zero_from_neg_zero
; item is_positive_zero section=.text local=false zero=false
; item is_positive_zero@run5 section=.text local=true zero=false
; item is_positive_zero@tail5 section=.text local=true zero=false
; item is_positive_zero@tail2 section=.text local=true zero=false
; item is_negative_zero section=.text local=false zero=false
; item target_negative_zero_eq section=.text local=false zero=false
; item target_negative_zero_eq@tail4 section=.text local=true zero=false
; item target_negative_zero_neq section=.text local=false zero=false
; item target_negative_zero_lt section=.text local=false zero=false
; item target_negative_zero_gt section=.text local=false zero=false
; item target_negative_zero_ge section=.text local=false zero=false
; item target_negative_zero_le section=.text local=false zero=false
; item target_neg_zero_branch section=.text local=false zero=false
; item target_not_neg_zero section=.text local=false zero=false
; item target_neg_zero_and section=.text local=false zero=false
; item target_add_negative_zero_to_self section=.text local=false zero=false
; item target_add_negative_zero_to_self@tail0 section=.text local=true zero=false
; item target_add_negative_zero_to_self@tail0@run2 section=.text local=true zero=false
; item target_add_positive_to_negative_zero section=.text local=false zero=false
; item target_add_positive_to_negative_zero@tail1 section=.text local=true zero=false
; item target_add_negative_to_positive_zero section=.text local=false zero=false
; item target_add_negative_nonzero_to_negative_zero section=.text local=false zero=false
; item target_add_negative_nonzero_to_negative_zero@run6 section=.text local=true zero=false
; item target_add_negative_nonzero_to_negative_zero@tail6 section=.text local=true zero=false
; item target_add_positive_nonzero_to_negative_zero section=.text local=false zero=false
; item target_subtract_neg_zero_from_self section=.text local=false zero=false
; item target_subtract_pos_zero_from_neg_zero section=.text local=false zero=false
; item target_subtract_neg_zero_from_pos_zero section=.text local=false zero=false
; item target_subtract_pos_nonzero_from_neg_zero section=.text local=false zero=false
; item target_negative_zero_mult section=.text local=false zero=false
; item target_negative_zero_mult_negative section=.text local=false zero=false
; item target_negative_zero_squared section=.text local=false zero=false
; item target_neg_zero_mult_zero section=.text local=false zero=false
; item target_mult_underflow section=.text local=false zero=false
; item target_div_neg_zero_by_pos_nonzero section=.text local=false zero=false
; item target_div_pos_zero_by_neg_nonzero section=.text local=false zero=false
; item target_div_neg_zero_by_neg_nonzero section=.text local=false zero=false
; item target_div_negative_underflow section=.text local=false zero=false
; item target_div_pos_non_zero_by_neg_zero section=.text local=false zero=false
; item target_div_pos_non_zero_by_neg_zero@tail3 section=.text local=true zero=false
; item target_div_neg_nonzero_by_zero section=.text local=false zero=false
; item target_div_neg_nonzero_by_neg_zero section=.text local=false zero=false
; item target_negate_neg_zero section=.text local=false zero=false
; item target_negate_calculated_zero section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run4 section=.text local=true zero=false
; item main@run3 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
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
   JMP   is_positive_zero@tail5
.if_end@0:
   JSR   is_positive_zero@run5
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

is_positive_zero@run5:
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

is_positive_zero@tail5:
   SUBROUTINE

   LDX   #$00
   TXA
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
   JMP   is_positive_zero@tail5
.if_end@1:
   JSR   is_positive_zero@run5
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

target_negative_zero_eq:
   SUBROUTINE

   JMP   target_negative_zero_eq@tail4

target_negative_zero_eq@tail4:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_negative_zero_neq:
   SUBROUTINE

   JMP   is_positive_zero@tail5

target_negative_zero_lt:
   SUBROUTINE

   JMP   is_positive_zero@tail5

target_negative_zero_gt:
   SUBROUTINE

   JMP   is_positive_zero@tail5

target_negative_zero_ge:
   SUBROUTINE

   JMP   target_negative_zero_eq@tail4

target_negative_zero_le:
   SUBROUTINE

   JMP   target_negative_zero_eq@tail4

target_neg_zero_branch:
   SUBROUTINE

   JMP   is_positive_zero@tail5

target_not_neg_zero:
   SUBROUTINE

   JMP   target_negative_zero_eq@tail4

target_neg_zero_and:
   SUBROUTINE

   JMP   is_positive_zero@tail5

target_add_negative_zero_to_self:
   SUBROUTINE

   JMP   target_add_negative_zero_to_self@tail0

target_add_negative_zero_to_self@tail0:
   SUBROUTINE

   JSR   target_add_negative_zero_to_self@tail0@run2
   STY   HARGS+21
   STA   HARGS+22
   LDA   #$80
   STA   HARGS+23
   RTS

target_add_negative_zero_to_self@tail0@run2:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   RTS

target_add_positive_to_negative_zero:
   SUBROUTINE

   JMP   target_add_positive_to_negative_zero@tail1

target_add_positive_to_negative_zero@tail1:
   SUBROUTINE

   JSR   target_add_negative_zero_to_self@tail0@run2
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

target_add_negative_to_positive_zero:
   SUBROUTINE

   JMP   target_add_positive_to_negative_zero@tail1

target_add_negative_nonzero_to_negative_zero:
   SUBROUTINE

   JSR   target_add_negative_nonzero_to_negative_zero@run6
   LDA   #$14
   JMP   target_add_negative_nonzero_to_negative_zero@tail6

target_add_negative_nonzero_to_negative_zero@run6:
   SUBROUTINE

   JSR   target_add_negative_zero_to_self@tail0@run2
   STA   HARGS+21
   RTS

target_add_negative_nonzero_to_negative_zero@tail6:
   SUBROUTINE

   STA   HARGS+22
   LDA   #$C0
   STA   HARGS+23
   RTS

target_add_positive_nonzero_to_negative_zero:
   SUBROUTINE

   JSR   target_add_negative_nonzero_to_negative_zero@run6
   LDA   #$24
   STA   HARGS+22
   LDA   #$40
   STA   HARGS+23
   RTS

target_subtract_neg_zero_from_self:
   SUBROUTINE

   JMP   target_add_positive_to_negative_zero@tail1

target_subtract_pos_zero_from_neg_zero:
   SUBROUTINE

   JMP   target_add_negative_zero_to_self@tail0

target_subtract_neg_zero_from_pos_zero:
   SUBROUTINE

   JMP   target_add_positive_to_negative_zero@tail1

target_subtract_pos_nonzero_from_neg_zero:
   SUBROUTINE

   JSR   target_add_negative_nonzero_to_negative_zero@run6
   LDA   #$24
   JMP   target_add_negative_nonzero_to_negative_zero@tail6

target_negative_zero_mult:
   SUBROUTINE

   JMP   target_add_negative_zero_to_self@tail0

target_negative_zero_mult_negative:
   SUBROUTINE

   JMP   target_add_positive_to_negative_zero@tail1

target_negative_zero_squared:
   SUBROUTINE

   JMP   target_add_positive_to_negative_zero@tail1

target_neg_zero_mult_zero:
   SUBROUTINE

   JMP   target_add_negative_zero_to_self@tail0

target_mult_underflow:
   SUBROUTINE

   JMP   target_add_negative_zero_to_self@tail0

target_div_neg_zero_by_pos_nonzero:
   SUBROUTINE

   JMP   target_add_negative_zero_to_self@tail0

target_div_pos_zero_by_neg_nonzero:
   SUBROUTINE

   JMP   target_add_negative_zero_to_self@tail0

target_div_neg_zero_by_neg_nonzero:
   SUBROUTINE

   JMP   target_add_positive_to_negative_zero@tail1

target_div_negative_underflow:
   SUBROUTINE

   JMP   target_add_negative_zero_to_self@tail0

target_div_pos_non_zero_by_neg_zero:
   SUBROUTINE

   JMP   target_div_pos_non_zero_by_neg_zero@tail3

target_div_pos_non_zero_by_neg_zero@tail3:
   SUBROUTINE

   JSR   target_add_negative_nonzero_to_negative_zero@run6
   LDA   #$F0
   STA   HARGS+22
   LDA   #$FF
   STA   HARGS+23
   RTS

target_div_neg_nonzero_by_zero:
   SUBROUTINE

   JMP   target_div_pos_non_zero_by_neg_zero@tail3

target_div_neg_nonzero_by_neg_zero:
   SUBROUTINE

   JSR   target_add_negative_nonzero_to_negative_zero@run6
   LDA   #$F0
   STA   HARGS+22
   LDA   #$7F
   STA   HARGS+23
   RTS

target_negate_neg_zero:
   SUBROUTINE

   JMP   target_add_positive_to_negative_zero@tail1

target_negate_calculated_zero:
   SUBROUTINE

   JMP   target_add_negative_zero_to_self@tail0

main:
   SUBROUTINE

   JSR   target_negative_zero_eq
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@6:
   JMP   target_negative_zero_eq@tail4
.if_end@5:
   JSR   target_negative_zero_neq
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$02
   RTS
.if_end@6:
   JSR   target_negative_zero_lt
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$03
   RTS
.if_end@7:
   JSR   target_negative_zero_gt
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$04
   RTS
.if_end@8:
   JSR   target_negative_zero_ge
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@7
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@7:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@9:
   JSR   target_negative_zero_le
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@8
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@10
.jcmp_skip@8:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@10:
   JSR   target_neg_zero_branch
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$07
   RTS
.if_end@11:
   JSR   target_not_neg_zero
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@9
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@12
.jcmp_skip@9:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@12:
   JSR   target_neg_zero_and
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@13
   LDX   #$00
   LDA   #$09
   RTS
.if_end@13:
   JSR   target_add_negative_zero_to_self
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@14
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@14:
   JSR   target_add_positive_to_negative_zero
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@15
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@15:
   JSR   target_add_negative_to_positive_zero
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@16
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@16:
   JSR   target_add_negative_nonzero_to_negative_zero
   LDA   HARGS+23
   CMP   #$C0
   BNE   .jcmp_skip@10
   LDA   HARGS+22
   CMP   #$14
   BNE   .jcmp_skip@10
   LDA   HARGS+21
   BNE   .jcmp_skip@10
   LDA   HARGS+20
   BNE   .jcmp_skip@10
   LDA   HARGS+19
   BNE   .jcmp_skip@10
   LDA   HARGS+18
   BNE   .jcmp_skip@10
   LDA   HARGS+17
   BNE   .jcmp_skip@10
   LDA   HARGS+16
   BEQ   .if_end@17
.jcmp_skip@10:
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@17:
   JSR   target_add_positive_nonzero_to_negative_zero
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@11
   LDA   HARGS+22
   CMP   #$24
   BNE   .jcmp_skip@11
   LDA   HARGS+21
   BNE   .jcmp_skip@11
   LDA   HARGS+20
   BNE   .jcmp_skip@11
   LDA   HARGS+19
   BNE   .jcmp_skip@11
   LDA   HARGS+18
   BNE   .jcmp_skip@11
   LDA   HARGS+17
   BNE   .jcmp_skip@11
   LDA   HARGS+16
   BEQ   .if_end@18
.jcmp_skip@11:
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@18:
   JSR   target_subtract_neg_zero_from_self
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@19
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@19:
   JSR   target_subtract_pos_zero_from_neg_zero
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@20
   LDX   #$00
   LDA   #$10
   RTS
.if_end@20:
   JSR   target_subtract_neg_zero_from_pos_zero
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@21
   LDX   #$00
   LDA   #$11
   RTS
.if_end@21:
   JSR   target_subtract_pos_nonzero_from_neg_zero
   LDA   HARGS+23
   CMP   #$C0
   BNE   .jcmp_skip@12
   LDA   HARGS+22
   CMP   #$24
   BNE   .jcmp_skip@12
   LDA   HARGS+21
   BNE   .jcmp_skip@12
   LDA   HARGS+20
   BNE   .jcmp_skip@12
   LDA   HARGS+19
   BNE   .jcmp_skip@12
   LDA   HARGS+18
   BNE   .jcmp_skip@12
   LDA   HARGS+17
   BNE   .jcmp_skip@12
   LDA   HARGS+16
   BEQ   .if_end@22
.jcmp_skip@12:
   LDX   #$00
   LDA   #$12
   RTS
.if_end@22:
   JSR   target_negative_zero_mult
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@23
   LDX   #$00
   LDA   #$13
   RTS
.if_end@23:
   JSR   target_negative_zero_mult_negative
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@24
   LDX   #$00
   LDA   #$14
   RTS
.if_end@24:
   JSR   target_negative_zero_squared
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@25
   LDX   #$00
   LDA   #$15
   RTS
.if_end@25:
   JSR   target_neg_zero_mult_zero
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@26
   LDX   #$00
   LDA   #$16
   RTS
.if_end@26:
   JSR   target_mult_underflow
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@27
   LDX   #$00
   LDA   #$17
   RTS
.if_end@27:
   JSR   target_div_neg_zero_by_pos_nonzero
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@28
   LDX   #$00
   LDA   #$18
   RTS
.if_end@28:
   JSR   target_div_pos_zero_by_neg_nonzero
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@29
   LDX   #$00
   LDA   #$19
   RTS
.if_end@29:
   JSR   target_div_neg_zero_by_neg_nonzero
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@30
   LDX   #$00
   LDA   #$1A
   RTS
.if_end@30:
   JSR   target_div_pos_non_zero_by_neg_zero
   JSR   main@run4
   BNE   .if_end@31
   LDX   #$00
   LDA   #$1B
   RTS
.if_end@31:
   JSR   target_div_neg_nonzero_by_zero
   JSR   main@run4
   BNE   .if_end@32
   LDX   #$00
   LDA   #$1C
   RTS
.if_end@32:
   JSR   target_div_neg_nonzero_by_neg_zero
   LDA   HARGS+16
   STA   HARGS+8
   LDA   HARGS+17
   STA   HARGS+9
   LDA   HARGS+18
   STA   HARGS+10
   LDA   HARGS+19
   STA   HARGS+11
   LDA   HARGS+20
   STA   HARGS+12
   LDA   HARGS+21
   STA   HARGS+13
   LDA   HARGS+22
   STA   HARGS+14
   LDA   HARGS+23
   STA   HARGS+15
   LDA   #$99
   STA   HARGS
   LDA   #$BB
   STA   HARGS+1
   LDA   #$AD
   STA   HARGS+2
   LDA   #$58
   STA   HARGS+3
   LDA   #$F1
   STA   HARGS+4
   LDA   #$DC
   STA   HARGS+5
   LDA   #$EF
   STA   HARGS+6
   LDA   #$7F
   STA   HARGS+7
   JSR   dlt
   LDA   HARGS+16
   BNE   .if_end@33
   LDX   #$00
   LDA   #$1D
   RTS
.if_end@33:
   JSR   target_div_negative_underflow
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@34
   LDX   #$00
   LDA   #$1E
   RTS
.if_end@34:
   JSR   target_negate_neg_zero
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@35
   LDX   #$00
   LDA   #$1F
   RTS
.if_end@35:
   JSR   target_negate_calculated_zero
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@36
   LDX   #$00
   LDA   #$20
   RTS
.if_end@36:
   JMP   is_positive_zero@tail5

main@run4:
   SUBROUTINE

   LDA   HARGS+16
   STA   HARGS
   LDA   HARGS+17
   STA   HARGS+1
   LDA   HARGS+18
   STA   HARGS+2
   LDA   HARGS+19
   STA   HARGS+3
   LDA   HARGS+20
   JSR   main@run3
   LDA   HARGS+16
   RTS

main@run3:
   SUBROUTINE

   STA   HARGS+4
   LDA   HARGS+21
   STA   HARGS+5
   LDA   HARGS+22
   STA   HARGS+6
   LDA   HARGS+23
   STA   HARGS+7
   LDA   #$99
   STA   HARGS+8
   LDA   #$BB
   STA   HARGS+9
   LDA   #$AD
   STA   HARGS+10
   LDA   #$58
   STA   HARGS+11
   LDA   #$F1
   STA   HARGS+12
   LDA   #$DC
   STA   HARGS+13
   LDA   #$EF
   STA   HARGS+14
   LDA   #$FF
   STA   HARGS+15
   JMP   dlt

main@run1:
   SUBROUTINE

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
   JMP   is_positive_zero

main@run0:
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
