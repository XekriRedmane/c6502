SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_copysign__x_0	EQU	$80
__zpabi_copysign__x_1	EQU	$81
__zpabi_copysign__x_2	EQU	$82
__zpabi_copysign__x_3	EQU	$83
__zpabi_copysign__x_4	EQU	$84
__zpabi_copysign__x_5	EQU	$85
__zpabi_copysign__x_6	EQU	$86
__zpabi_copysign__x_7	EQU	$87
__zpabi_copysign__y_0	EQU	$88
__zpabi_copysign__y_1	EQU	$89
__zpabi_copysign__y_2	EQU	$8A
__zpabi_copysign__y_3	EQU	$8B
__zpabi_copysign__y_4	EQU	$8C
__zpabi_copysign__y_5	EQU	$8D
__zpabi_copysign__y_6	EQU	$8E
__zpabi_copysign__y_7	EQU	$8F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_from_char params= locals= indirect=false in_cycle=false
; def target_from_neg_int params= locals= indirect=false in_cycle=false
; def target_from_neg_long params= locals= indirect=false in_cycle=false
; def target_from_negated_int_zero params= locals= indirect=false in_cycle=false
; def target_from_schar params= locals= indirect=false in_cycle=false
; def target_from_truncated_char params= locals= indirect=false in_cycle=false
; def target_from_truncated_uchar params= locals= indirect=false in_cycle=false
; def target_from_uchar params= locals= indirect=false in_cycle=false
; ext copysign params=__zpabi_copysign__x_0,__zpabi_copysign__x_1,__zpabi_copysign__x_2,__zpabi_copysign__x_3,__zpabi_copysign__x_4,__zpabi_copysign__x_5,__zpabi_copysign__x_6,__zpabi_copysign__x_7,__zpabi_copysign__y_0,__zpabi_copysign__y_1,__zpabi_copysign__y_2,__zpabi_copysign__y_3,__zpabi_copysign__y_4,__zpabi_copysign__y_5,__zpabi_copysign__y_6,__zpabi_copysign__y_7
; call main -> copysign
; call main -> target_from_char
; call main -> target_from_neg_int
; call main -> target_from_neg_long
; call main -> target_from_negated_int_zero
; call main -> target_from_schar
; call main -> target_from_truncated_char
; call main -> target_from_truncated_uchar
; call main -> target_from_uchar
; item target_from_neg_int section=.text local=false zero=false
; item target_from_neg_long section=.text local=false zero=false
; item target_from_neg_long@run0 section=.text local=true zero=false
; item target_from_char section=.text local=false zero=false
; item target_from_char@tail0 section=.text local=true zero=false
; item target_from_char@tail0@tail1 section=.text local=true zero=false
; item target_from_schar section=.text local=false zero=false
; item target_from_uchar section=.text local=false zero=false
; item target_from_truncated_char section=.text local=false zero=false
; item target_from_truncated_uchar section=.text local=false zero=false
; item target_from_negated_int_zero section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target_from_neg_int:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   STA   HARGS+17
   LDA   #$C0
   STA   HARGS+18
   LDA   #$FF
   STA   HARGS+19
   LDY   #$FF
   STY   HARGS+20
   STA   HARGS+21
   LDA   #$DF
   STA   HARGS+22
   LDA   #$C1
   STA   HARGS+23
   RTS

target_from_neg_long:
   SUBROUTINE

   JSR   target_from_neg_long@run0
   STY   HARGS+20
   STA   HARGS+21
   LDA   #$D0
   STA   HARGS+22
   LDA   #$C3
   STA   HARGS+23
   RTS

target_from_neg_long@run0:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   RTS

target_from_char:
   SUBROUTINE

   JMP   target_from_char@tail0

target_from_char@tail0:
   SUBROUTINE

   JSR   target_from_neg_long@run0
   STA   HARGS+20
   LDA   #$C0
   STA   HARGS+21
   LDA   #$5F
   JMP   target_from_char@tail0@tail1

target_from_char@tail0@tail1:
   SUBROUTINE

   STA   HARGS+22
   LDA   #$40
   STA   HARGS+23
   RTS

target_from_schar:
   SUBROUTINE

   JSR   target_from_neg_long@run0
   STA   HARGS+20
   LDA   #$20
   STA   HARGS+21
   LDA   #$60
   JMP   target_from_char@tail0@tail1

target_from_uchar:
   SUBROUTINE

   JSR   target_from_neg_long@run0
   STA   HARGS+20
   LDA   #$E0
   STA   HARGS+21
   LDA   #$6F
   JMP   target_from_char@tail0@tail1

target_from_truncated_char:
   SUBROUTINE

   JMP   target_from_char@tail0

target_from_truncated_uchar:
   SUBROUTINE

   JSR   target_from_neg_long@run0
   STY   HARGS+20
   STA   HARGS+21
   LDA   #$6D
   JMP   target_from_char@tail0@tail1

target_from_negated_int_zero:
   SUBROUTINE

   JSR   target_from_neg_long@run0
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   JSR   target_from_neg_int
   LDA   HARGS+23
   CMP   #$C1
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   CMP   #$DF
   BNE   .jcmp_skip@0
   LDA   HARGS+21
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+20
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+19
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+18
   CMP   #$C0
   BNE   .jcmp_skip@0
   LDA   HARGS+17
   BNE   .jcmp_skip@0
   LDA   HARGS+16
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   target_from_neg_long
   LDA   HARGS+23
   CMP   #$C3
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   CMP   #$D0
   BNE   .jcmp_skip@1
   LDA   HARGS+21
   BNE   .jcmp_skip@1
   LDA   HARGS+20
   BNE   .jcmp_skip@1
   LDA   HARGS+19
   BNE   .jcmp_skip@1
   LDA   HARGS+18
   BNE   .jcmp_skip@1
   LDA   HARGS+17
   BNE   .jcmp_skip@1
   LDA   HARGS+16
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   target_from_char
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@2
   LDA   HARGS+22
   CMP   #$5F
   BNE   .jcmp_skip@2
   LDA   HARGS+21
   CMP   #$C0
   BNE   .jcmp_skip@2
   LDA   HARGS+20
   BNE   .jcmp_skip@2
   LDA   HARGS+19
   BNE   .jcmp_skip@2
   LDA   HARGS+18
   BNE   .jcmp_skip@2
   LDA   HARGS+17
   BNE   .jcmp_skip@2
   LDA   HARGS+16
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   target_from_schar
   LDA   HARGS+23
   CMP   #$C0
   BNE   .jcmp_skip@3
   LDA   HARGS+22
   CMP   #$5F
   BNE   .jcmp_skip@3
   LDA   HARGS+21
   CMP   #$C0
   BNE   .jcmp_skip@3
   LDA   HARGS+20
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
   LDA   #$04
   RTS
.if_end@3:
   JSR   target_from_uchar
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@4
   LDA   HARGS+22
   CMP   #$6F
   BNE   .jcmp_skip@4
   LDA   HARGS+21
   CMP   #$E0
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
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   JSR   target_from_truncated_char
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@5
   LDA   HARGS+22
   CMP   #$5F
   BNE   .jcmp_skip@5
   LDA   HARGS+21
   CMP   #$C0
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
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   JSR   target_from_truncated_uchar
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@6
   LDA   HARGS+22
   CMP   #$6D
   BNE   .jcmp_skip@6
   LDA   HARGS+21
   BNE   .jcmp_skip@6
   LDA   HARGS+20
   BNE   .jcmp_skip@6
   LDA   HARGS+19
   BNE   .jcmp_skip@6
   LDA   HARGS+18
   BNE   .jcmp_skip@6
   LDA   HARGS+17
   BNE   .jcmp_skip@6
   LDA   HARGS+16
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   JSR   target_from_negated_int_zero
   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   ORA   HARGS+21
   ORA   HARGS+22
   ORA   HARGS+23
   BNE   .main@jthread@0
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
   LDA   HARGS+16
   STA   __zpabi_copysign__y_0
   LDA   HARGS+17
   STA   __zpabi_copysign__y_1
   LDA   HARGS+18
   STA   __zpabi_copysign__y_2
   LDA   HARGS+19
   STA   __zpabi_copysign__y_3
   LDA   HARGS+20
   STA   __zpabi_copysign__y_4
   LDA   HARGS+21
   STA   __zpabi_copysign__y_5
   LDA   HARGS+22
   STA   __zpabi_copysign__y_6
   LDA   HARGS+23
   STA   __zpabi_copysign__y_7
   JSR   copysign
   LDA   HARGS+23
   CMP   #$40
   BNE   .main@jthread@0
   LDA   HARGS+22
   CMP   #$14
   BNE   .main@jthread@0
   LDA   HARGS+21
   BNE   .main@jthread@0
   LDA   HARGS+20
   BNE   .main@jthread@0
   LDA   HARGS+19
   BNE   .main@jthread@0
   LDA   HARGS+18
   BNE   .main@jthread@0
   LDA   HARGS+17
   BNE   .main@jthread@0
   LDA   HARGS+16
   BEQ   .if_end@9
.main@jthread@0:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@9:
   LDX   #$00
   TXA
   RTS
