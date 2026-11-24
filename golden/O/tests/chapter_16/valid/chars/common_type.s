SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_char_lt_uchar__0	EQU	$80
__local_multiply__0	EQU	$80
__local_signed_char_le_char__0	EQU	$80
__zpabi_char_lt_int__i_0	EQU	$80
__zpabi_ternary__c	EQU	$80
__zpabi_uchar_gt_long__l_0	EQU	$80
__local_char_lt_uchar__u	EQU	$81
__local_multiply__1	EQU	$81
__zpabi_char_lt_int__i_1	EQU	$81
__zpabi_uchar_gt_long__l_1	EQU	$81
__local_char_lt_uchar__c	EQU	$82
__local_signed_char_le_char__s	EQU	$82
__zpabi_uchar_gt_long__l_2	EQU	$82
__zpabi_uchar_gt_long__l_3	EQU	$83
__local_main__0	EQU	$84
__local_ternary__flag_1	EQU	$8C
__local_uchar_gt_long__uc	EQU	$8C
__local_ternary__0	EQU	$8E

; @zp-link-meta-begin
; def char_lt_int params=__zpabi_char_lt_int__c,__zpabi_char_lt_int__i_0,__zpabi_char_lt_int__i_1 locals=__local_char_lt_int__c indirect=false in_cycle=false param_regs=A,,
; def char_lt_uchar params=__zpabi_char_lt_uchar__c,__zpabi_char_lt_uchar__u locals=__local_char_lt_uchar__0,__local_char_lt_uchar__u,__local_char_lt_uchar__c indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def multiply params= locals=__local_multiply__0,__local_multiply__1 indirect=false in_cycle=false
; def signed_char_le_char params=__zpabi_signed_char_le_char__s,__zpabi_signed_char_le_char__c locals=__local_signed_char_le_char__0,__local_signed_char_le_char__c,__local_signed_char_le_char__s indirect=false in_cycle=false param_regs=A,X
; def ternary params=__zpabi_ternary__flag_0,__zpabi_ternary__flag_1,__zpabi_ternary__c locals=__local_ternary__flag_1,__local_ternary__flag_0,__local_ternary__0,__local_ternary__1 indirect=false in_cycle=false param_regs=A,X,
; def uchar_gt_long params=__zpabi_uchar_gt_long__uc,__zpabi_uchar_gt_long__l_0,__zpabi_uchar_gt_long__l_1,__zpabi_uchar_gt_long__l_2,__zpabi_uchar_gt_long__l_3 locals=__local_uchar_gt_long__uc indirect=false in_cycle=false param_regs=A,,,,
; call main -> char_lt_int
; call main -> char_lt_uchar
; call main -> multiply
; call main -> signed_char_le_char
; call main -> ternary
; call main -> uchar_gt_long
; item ternary section=.text local=false zero=false
; item char_lt_int section=.text local=false zero=false
; item char_lt_int@tail0 section=.text local=true zero=false
; item uchar_gt_long section=.text local=false zero=false
; item char_lt_uchar section=.text local=false zero=false
; item signed_char_le_char section=.text local=false zero=false
; item multiply section=.text local=false zero=false
; item main section=.text local=false zero=false
; item ten section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

ternary:
   SUBROUTINE

   STX   __local_ternary__flag_1
   ORA   __local_ternary__flag_1
   BEQ   .cond_else@0
   LDA   __zpabi_ternary__c
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_ternary__0
   LDA   __zpabi_ternary__c
   STA   HARGS+16
   JMP   .cond_end@1
.cond_else@0:
   LDA   #$01
   STA   HARGS+16
   LDA   #$00
   STA   __local_ternary__0
.cond_end@1:
   LDA   __local_ternary__0
   STA   HARGS+17
   STA   HARGS+18
   STA   HARGS+19
   LDA   #$00
   STA   HARGS+20
   LDY   #$00
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

char_lt_int:
   SUBROUTINE

   SEC
   SBC   __zpabi_char_lt_int__i_0
   LDA   #$00
   SBC   __zpabi_char_lt_int__i_1
   JMP   char_lt_int@tail0

char_lt_int@tail0:
   SUBROUTINE

   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   LDX   #$00
   RTS

uchar_gt_long:
   SUBROUTINE

   STA   __local_uchar_gt_long__uc
   LDA   __zpabi_uchar_gt_long__l_0
   SEC
   SBC   __local_uchar_gt_long__uc
   LDA   __zpabi_uchar_gt_long__l_1
   SBC   #$00
   LDA   __zpabi_uchar_gt_long__l_2
   SBC   #$00
   LDA   __zpabi_uchar_gt_long__l_3
   SBC   #$00
   JMP   char_lt_int@tail0

char_lt_uchar:
   SUBROUTINE

   STA   __local_char_lt_uchar__c
   STX   __local_char_lt_uchar__u
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_char_lt_uchar__0
   LDA   __local_char_lt_uchar__c
   SEC
   SBC   __local_char_lt_uchar__u
   LDA   __local_char_lt_uchar__0
   SBC   #$00
   JMP   char_lt_int@tail0

signed_char_le_char:
   SUBROUTINE

   STA   __local_signed_char_le_char__s
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_signed_char_le_char__0
   TXA
   SEC
   SBC   __local_signed_char_le_char__s
   LDA   #$00
   SBC   __local_signed_char_le_char__0
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDX   #$00
   RTS

multiply:
   SUBROUTINE

   LDA   ten
   STA   HARGS
   LDA   #$00
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
   JSR   d2u
   LDA   HARGS+8
   CMP   #$6B
   BEQ   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$F6
   STA   __zpabi_ternary__c
   LDX   #$00
   LDA   #$01
   JSR   ternary
   LDA   HARGS+23
   BNE   .jcmp_skip@12
   LDA   HARGS+22
   BNE   .jcmp_skip@12
   LDA   HARGS+21
   BNE   .jcmp_skip@12
   LDA   HARGS+20
   BNE   .jcmp_skip@12
   LDA   HARGS+19
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   HARGS+18
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   HARGS+17
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   HARGS+16
   CMP   #$F6
   BEQ   .if_end@2
.jcmp_skip@12:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDA   #$00
   STA   __zpabi_char_lt_int__i_0
   LDA   #$01
   STA   __zpabi_char_lt_int__i_1
   JSR   char_lt_int
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDA   #$FE
   STA   __zpabi_uchar_gt_long__l_0
   LDA   #$FF
   STA   __zpabi_uchar_gt_long__l_1
   STA   __zpabi_uchar_gt_long__l_2
   STA   __zpabi_uchar_gt_long__l_3
   LDA   #$64
   JSR   uchar_gt_long
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$03
   RTS
.if_end@4:
   LDX   #$02
   LDA   #$FF
   JSR   char_lt_uchar
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@5
   LDX   #$00
   LDA   #$04
   RTS
.if_end@5:
   LDX   #$FF
   TXA
   JSR   signed_char_le_char
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@6
   LDX   #$00
   LDA   #$05
   RTS
.if_end@6:
   JSR   multiply
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@7
   LDX   #$00
   LDA   #$06
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS

ten:
   DC.B  $0A
