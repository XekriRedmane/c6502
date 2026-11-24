SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_schar_to_double__s	EQU	$80
__local_schar_to_int__s	EQU	$80
__local_schar_to_long__s	EQU	$80
__local_schar_to_uint__s	EQU	$80
__local_schar_to_ulong__s	EQU	$80
__zpabi_double_to_char__d_0	EQU	$80
__zpabi_double_to_uchar__d_0	EQU	$80
__zpabi_long_to_schar__l_0	EQU	$80
__zpabi_long_to_uchar__l_0	EQU	$80
__zpabi_ulong_to_schar__l_0	EQU	$80
__zpabi_ulong_to_uchar__ul_0	EQU	$80
__zpabi_double_to_char__d_1	EQU	$81
__zpabi_double_to_uchar__d_1	EQU	$81
__zpabi_long_to_schar__l_1	EQU	$81
__zpabi_long_to_uchar__l_1	EQU	$81
__zpabi_ulong_to_schar__l_1	EQU	$81
__zpabi_ulong_to_uchar__ul_1	EQU	$81
__zpabi_double_to_char__d_2	EQU	$82
__zpabi_double_to_uchar__d_2	EQU	$82
__zpabi_long_to_schar__l_2	EQU	$82
__zpabi_long_to_uchar__l_2	EQU	$82
__zpabi_ulong_to_schar__l_2	EQU	$82
__zpabi_ulong_to_uchar__ul_2	EQU	$82
__zpabi_double_to_char__d_3	EQU	$83
__zpabi_double_to_uchar__d_3	EQU	$83
__zpabi_long_to_schar__l_3	EQU	$83
__zpabi_long_to_uchar__l_3	EQU	$83
__zpabi_ulong_to_schar__l_3	EQU	$83
__zpabi_ulong_to_uchar__ul_3	EQU	$83
__zpabi_double_to_char__d_4	EQU	$84
__zpabi_double_to_uchar__d_4	EQU	$84
__zpabi_double_to_char__d_5	EQU	$85
__zpabi_double_to_uchar__d_5	EQU	$85
__zpabi_double_to_char__d_6	EQU	$86
__zpabi_double_to_uchar__d_6	EQU	$86
__zpabi_double_to_char__d_7	EQU	$87
__zpabi_double_to_uchar__d_7	EQU	$87
__local_main__1	EQU	$89

; @zp-link-meta-begin
; def char_to_double params=__zpabi_char_to_double__c locals= indirect=false in_cycle=false param_regs=A
; def char_to_int params=__zpabi_char_to_int__c locals=__local_char_to_int__c indirect=false in_cycle=false param_regs=A
; def char_to_long params=__zpabi_char_to_long__c locals= indirect=false in_cycle=false param_regs=A
; def char_to_schar params=__zpabi_char_to_schar__c locals=__local_char_to_schar__c indirect=false in_cycle=false param_regs=A
; def char_to_uchar params=__zpabi_char_to_uchar__c locals=__local_char_to_uchar__c indirect=false in_cycle=false param_regs=A
; def char_to_uint params=__zpabi_char_to_uint__c locals=__local_char_to_uint__c indirect=false in_cycle=false param_regs=A
; def char_to_ulong params=__zpabi_char_to_ulong__c locals= indirect=false in_cycle=false param_regs=A
; def double_to_char params=__zpabi_double_to_char__d_0,__zpabi_double_to_char__d_1,__zpabi_double_to_char__d_2,__zpabi_double_to_char__d_3,__zpabi_double_to_char__d_4,__zpabi_double_to_char__d_5,__zpabi_double_to_char__d_6,__zpabi_double_to_char__d_7 locals=__local_double_to_char__0 indirect=false in_cycle=false
; def double_to_uchar params=__zpabi_double_to_uchar__d_0,__zpabi_double_to_uchar__d_1,__zpabi_double_to_uchar__d_2,__zpabi_double_to_uchar__d_3,__zpabi_double_to_uchar__d_4,__zpabi_double_to_uchar__d_5,__zpabi_double_to_uchar__d_6,__zpabi_double_to_uchar__d_7 locals=__local_double_to_uchar__0 indirect=false in_cycle=false
; def int_to_char params=__zpabi_int_to_char__i_0,__zpabi_int_to_char__i_1 locals=__local_int_to_char__i indirect=false in_cycle=false param_regs=A,X
; def int_to_uchar params=__zpabi_int_to_uchar__i_0,__zpabi_int_to_uchar__i_1 locals=__local_int_to_uchar__i indirect=false in_cycle=false param_regs=A,X
; def long_to_schar params=__zpabi_long_to_schar__l_0,__zpabi_long_to_schar__l_1,__zpabi_long_to_schar__l_2,__zpabi_long_to_schar__l_3 locals= indirect=false in_cycle=false
; def long_to_uchar params=__zpabi_long_to_uchar__l_0,__zpabi_long_to_uchar__l_1,__zpabi_long_to_uchar__l_2,__zpabi_long_to_uchar__l_3 locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def schar_to_char params=__zpabi_schar_to_char__u locals=__local_schar_to_char__u indirect=false in_cycle=false param_regs=A
; def schar_to_double params=__zpabi_schar_to_double__s locals=__local_schar_to_double__s indirect=false in_cycle=false param_regs=A
; def schar_to_int params=__zpabi_schar_to_int__s locals=__local_schar_to_int__s indirect=false in_cycle=false param_regs=A
; def schar_to_long params=__zpabi_schar_to_long__s locals=__local_schar_to_long__s indirect=false in_cycle=false param_regs=A
; def schar_to_uchar params=__zpabi_schar_to_uchar__u locals=__local_schar_to_uchar__u indirect=false in_cycle=false param_regs=A
; def schar_to_uint params=__zpabi_schar_to_uint__s locals=__local_schar_to_uint__s indirect=false in_cycle=false param_regs=A
; def schar_to_ulong params=__zpabi_schar_to_ulong__s locals=__local_schar_to_ulong__s indirect=false in_cycle=false param_regs=A
; def uchar_to_char params=__zpabi_uchar_to_char__u locals=__local_uchar_to_char__u indirect=false in_cycle=false param_regs=A
; def uchar_to_double params=__zpabi_uchar_to_double__u locals= indirect=false in_cycle=false param_regs=A
; def uchar_to_int params=__zpabi_uchar_to_int__u locals=__local_uchar_to_int__u indirect=false in_cycle=false param_regs=A
; def uchar_to_long params=__zpabi_uchar_to_long__u locals= indirect=false in_cycle=false param_regs=A
; def uchar_to_schar params=__zpabi_uchar_to_schar__u locals=__local_uchar_to_schar__u indirect=false in_cycle=false param_regs=A
; def uchar_to_uint params=__zpabi_uchar_to_uint__u locals=__local_uchar_to_uint__u indirect=false in_cycle=false param_regs=A
; def uchar_to_ulong params=__zpabi_uchar_to_ulong__u locals= indirect=false in_cycle=false param_regs=A
; def uint_to_char params=__zpabi_uint_to_char__u_0,__zpabi_uint_to_char__u_1 locals=__local_uint_to_char__u indirect=false in_cycle=false param_regs=A,X
; def uint_to_uchar params=__zpabi_uint_to_uchar__ui_0,__zpabi_uint_to_uchar__ui_1 locals=__local_uint_to_uchar__ui indirect=false in_cycle=false param_regs=A,X
; def ulong_to_schar params=__zpabi_ulong_to_schar__l_0,__zpabi_ulong_to_schar__l_1,__zpabi_ulong_to_schar__l_2,__zpabi_ulong_to_schar__l_3 locals= indirect=false in_cycle=false
; def ulong_to_uchar params=__zpabi_ulong_to_uchar__ul_0,__zpabi_ulong_to_uchar__ul_1,__zpabi_ulong_to_uchar__ul_2,__zpabi_ulong_to_uchar__ul_3 locals= indirect=false in_cycle=false
; call main -> char_to_int
; call main -> char_to_uchar
; call main -> char_to_ulong
; call main -> double_to_char
; call main -> double_to_uchar
; call main -> int_to_char
; call main -> int_to_uchar
; call main -> long_to_schar
; call main -> long_to_uchar
; call main -> schar_to_double
; call main -> schar_to_long
; call main -> schar_to_uchar
; call main -> schar_to_uint
; call main -> uchar_to_char
; call main -> uchar_to_double
; call main -> uchar_to_int
; call main -> uchar_to_long
; call main -> uchar_to_schar
; call main -> uchar_to_uint
; call main -> uchar_to_ulong
; call main -> uint_to_char
; call main -> uint_to_uchar
; call main -> ulong_to_schar
; call main -> ulong_to_uchar
; item char_to_uchar section=.text local=false zero=false
; item char_to_schar section=.text local=false zero=false
; item uchar_to_char section=.text local=false zero=false
; item schar_to_char section=.text local=false zero=false
; item uchar_to_schar section=.text local=false zero=false
; item schar_to_uchar section=.text local=false zero=false
; item char_to_int section=.text local=false zero=false
; item char_to_uint section=.text local=false zero=false
; item char_to_long section=.text local=false zero=false
; item char_to_long@tail1 section=.text local=true zero=false
; item char_to_ulong section=.text local=false zero=false
; item char_to_double section=.text local=false zero=false
; item char_to_double@tail2 section=.text local=true zero=false
; item char_to_double@tail0 section=.text local=true zero=false
; item schar_to_int section=.text local=false zero=false
; item schar_to_uint section=.text local=false zero=false
; item schar_to_uint@run0 section=.text local=true zero=false
; item schar_to_long section=.text local=false zero=false
; item schar_to_ulong section=.text local=false zero=false
; item schar_to_double section=.text local=false zero=false
; item uchar_to_int section=.text local=false zero=false
; item uchar_to_uint section=.text local=false zero=false
; item uchar_to_long section=.text local=false zero=false
; item uchar_to_ulong section=.text local=false zero=false
; item uchar_to_double section=.text local=false zero=false
; item int_to_char section=.text local=false zero=false
; item uint_to_char section=.text local=false zero=false
; item double_to_char section=.text local=false zero=false
; item double_to_char@tail3 section=.text local=true zero=false
; item long_to_schar section=.text local=false zero=false
; item ulong_to_schar section=.text local=false zero=false
; item int_to_uchar section=.text local=false zero=false
; item uint_to_uchar section=.text local=false zero=false
; item long_to_uchar section=.text local=false zero=false
; item ulong_to_uchar section=.text local=false zero=false
; item double_to_uchar section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @34.null_ptr section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

char_to_uchar:
   SUBROUTINE

   RTS

char_to_schar:
   SUBROUTINE

   RTS

uchar_to_char:
   SUBROUTINE

   RTS

schar_to_char:
   SUBROUTINE

   RTS

uchar_to_schar:
   SUBROUTINE

   RTS

schar_to_uchar:
   SUBROUTINE

   RTS

char_to_int:
   SUBROUTINE

   LDX   #$00
   RTS

char_to_uint:
   SUBROUTINE

   LDX   #$00
   RTS

char_to_long:
   SUBROUTINE

   JMP   char_to_long@tail1

char_to_long@tail1:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

char_to_ulong:
   SUBROUTINE

   JMP   char_to_long@tail1

char_to_double:
   SUBROUTINE

   JMP   char_to_double@tail2

char_to_double@tail2:
   SUBROUTINE

   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   u2d
   JMP   char_to_double@tail0

char_to_double@tail0:
   SUBROUTINE

   LDA   HARGS+2
   STA   HARGS+16
   LDA   HARGS+3
   STA   HARGS+17
   LDA   HARGS+4
   STA   HARGS+18
   LDA   HARGS+5
   STA   HARGS+19
   LDA   HARGS+6
   STA   HARGS+20
   LDA   HARGS+7
   STA   HARGS+21
   LDA   HARGS+8
   STA   HARGS+22
   LDA   HARGS+9
   STA   HARGS+23
   RTS

schar_to_int:
   SUBROUTINE

   STA   __local_schar_to_int__s
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   TAX
   LDA   __local_schar_to_int__s
   RTS

schar_to_uint:
   SUBROUTINE

   STA   __local_schar_to_uint__s
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   schar_to_uint@run0
   LDA   __local_schar_to_uint__s
   STA   HARGS+8
   RTS

schar_to_uint@run0:
   SUBROUTINE

   STA   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   RTS

schar_to_long:
   SUBROUTINE

   STA   __local_schar_to_long__s
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   schar_to_uint@run0
   LDA   __local_schar_to_long__s
   STA   HARGS+8
   RTS

schar_to_ulong:
   SUBROUTINE

   STA   __local_schar_to_ulong__s
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   schar_to_uint@run0
   LDA   __local_schar_to_ulong__s
   STA   HARGS+8
   RTS

schar_to_double:
   SUBROUTINE

   STA   __local_schar_to_double__s
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   HARGS+1
   LDA   __local_schar_to_double__s
   STA   HARGS
   JSR   i2d
   JMP   char_to_double@tail0

uchar_to_int:
   SUBROUTINE

   LDX   #$00
   RTS

uchar_to_uint:
   SUBROUTINE

   LDX   #$00
   RTS

uchar_to_long:
   SUBROUTINE

   JMP   char_to_long@tail1

uchar_to_ulong:
   SUBROUTINE

   JMP   char_to_long@tail1

uchar_to_double:
   SUBROUTINE

   JMP   char_to_double@tail2

int_to_char:
   SUBROUTINE

   RTS

uint_to_char:
   SUBROUTINE

   RTS

double_to_char:
   SUBROUTINE

   LDA   __zpabi_double_to_char__d_0
   STA   HARGS
   LDA   __zpabi_double_to_char__d_1
   STA   HARGS+1
   LDA   __zpabi_double_to_char__d_2
   STA   HARGS+2
   LDA   __zpabi_double_to_char__d_3
   STA   HARGS+3
   LDA   __zpabi_double_to_char__d_4
   STA   HARGS+4
   LDA   __zpabi_double_to_char__d_5
   STA   HARGS+5
   LDA   __zpabi_double_to_char__d_6
   STA   HARGS+6
   LDA   __zpabi_double_to_char__d_7
   JMP   double_to_char@tail3

double_to_char@tail3:
   SUBROUTINE

   STA   HARGS+7
   JSR   d2u
   LDA   HARGS+8
   RTS

long_to_schar:
   SUBROUTINE

   LDA   __zpabi_long_to_schar__l_0
   RTS

ulong_to_schar:
   SUBROUTINE

   LDA   __zpabi_ulong_to_schar__l_0
   RTS

int_to_uchar:
   SUBROUTINE

   RTS

uint_to_uchar:
   SUBROUTINE

   RTS

long_to_uchar:
   SUBROUTINE

   LDA   __zpabi_long_to_uchar__l_0
   RTS

ulong_to_uchar:
   SUBROUTINE

   LDA   __zpabi_ulong_to_uchar__ul_0
   RTS

double_to_uchar:
   SUBROUTINE

   LDA   __zpabi_double_to_uchar__d_0
   STA   HARGS
   LDA   __zpabi_double_to_uchar__d_1
   STA   HARGS+1
   LDA   __zpabi_double_to_uchar__d_2
   STA   HARGS+2
   LDA   __zpabi_double_to_uchar__d_3
   STA   HARGS+3
   LDA   __zpabi_double_to_uchar__d_4
   STA   HARGS+4
   LDA   __zpabi_double_to_uchar__d_5
   STA   HARGS+5
   LDA   __zpabi_double_to_uchar__d_6
   STA   HARGS+6
   LDA   __zpabi_double_to_uchar__d_7
   JMP   double_to_char@tail3

main:
   SUBROUTINE

   LDA   #$7F
   JSR   char_to_uchar
   CMP   #$7F
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$7F
   JSR   char_to_int
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@10
   LDA   __local_main__1
   CMP   #$7F
   BEQ   .if_end@1
.jcmp_skip@10:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$7F
   JSR   char_to_ulong
   LDA   HARGS+11
   BNE   .jcmp_skip@11
   LDA   HARGS+10
   BNE   .jcmp_skip@11
   LDA   HARGS+9
   BNE   .jcmp_skip@11
   LDA   HARGS+8
   CMP   #$7F
   BEQ   .if_end@2
.jcmp_skip@11:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$F6
   JSR   schar_to_uchar
   CMP   #$F6
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #$F6
   JSR   schar_to_long
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   HARGS+8
   CMP   #$F6
   BEQ   .if_end@4
.jcmp_skip@12:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$F6
   JSR   schar_to_uint
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@13
   LDA   HARGS+8
   CMP   #$F6
   BEQ   .if_end@5
.jcmp_skip@13:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #$F6
   JSR   schar_to_double
   LDA   HARGS+23
   CMP   #$C0
   BNE   .jcmp_skip@14
   LDA   HARGS+22
   CMP   #$24
   BNE   .jcmp_skip@14
   LDA   HARGS+21
   BNE   .jcmp_skip@14
   LDA   HARGS+20
   BNE   .jcmp_skip@14
   LDA   HARGS+19
   BNE   .jcmp_skip@14
   LDA   HARGS+18
   BNE   .jcmp_skip@14
   LDA   HARGS+17
   BNE   .jcmp_skip@14
   LDA   HARGS+16
   BEQ   .if_end@6
.jcmp_skip@14:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   #$FA
   JSR   uchar_to_int
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@15
   LDA   __local_main__1
   CMP   #$FA
   BEQ   .if_end@7
.jcmp_skip@15:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   #$FA
   JSR   uchar_to_long
   LDA   HARGS+11
   BNE   .jcmp_skip@16
   LDA   HARGS+10
   BNE   .jcmp_skip@16
   LDA   HARGS+9
   BNE   .jcmp_skip@16
   LDA   HARGS+8
   CMP   #$FA
   BEQ   .if_end@8
.jcmp_skip@16:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   #$FA
   JSR   uchar_to_uint
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@17
   LDA   __local_main__1
   CMP   #$FA
   BEQ   .if_end@9
.jcmp_skip@17:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   LDA   #$FA
   JSR   uchar_to_ulong
   LDA   HARGS+11
   BNE   .jcmp_skip@18
   LDA   HARGS+10
   BNE   .jcmp_skip@18
   LDA   HARGS+9
   BNE   .jcmp_skip@18
   LDA   HARGS+8
   CMP   #$FA
   BEQ   .if_end@10
.jcmp_skip@18:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   LDA   #$FA
   JSR   uchar_to_double
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@19
   LDA   HARGS+22
   CMP   #$6F
   BNE   .jcmp_skip@19
   LDA   HARGS+21
   CMP   #$40
   BNE   .jcmp_skip@19
   LDA   HARGS+20
   BNE   .jcmp_skip@19
   LDA   HARGS+19
   BNE   .jcmp_skip@19
   LDA   HARGS+18
   BNE   .jcmp_skip@19
   LDA   HARGS+17
   BNE   .jcmp_skip@19
   LDA   HARGS+16
   BEQ   .if_end@11
.jcmp_skip@19:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   LDA   #$FA
   JSR   uchar_to_schar
   STA   __local_main__1
   ORA   #$00
   BPL   .jcmp_skip@22
   LDA   __local_main__1
   CMP   #$FA
   BEQ   .if_end@12
.jcmp_skip@22:
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@12:
   LDA   #$FA
   JSR   uchar_to_char
   STA   __local_main__1
   ORA   #$00
   BPL   .jcmp_skip@25
   LDA   __local_main__1
   CMP   #$FA
   BEQ   .if_end@13
.jcmp_skip@25:
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@13:
   LDX   #$00
   LDA   #$80
   JSR   int_to_char
   CMP   #$80
   BEQ   .if_end@14
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@14:
   LDX   #$00
   LDA   #$FA
   JSR   uint_to_char
   CMP   #$FA
   BEQ   .if_end@15
   LDX   #$00
   LDA   #$10
   RTS
.if_end@15:
   LDA   #$CD
   STA   __zpabi_double_to_char__d_0
   LDA   #$CC
   STA   __zpabi_double_to_char__d_1
   STA   __zpabi_double_to_char__d_2
   STA   __zpabi_double_to_char__d_3
   STA   __zpabi_double_to_char__d_4
   STA   __zpabi_double_to_char__d_5
   LDA   #$04
   STA   __zpabi_double_to_char__d_6
   LDA   #$C0
   STA   __zpabi_double_to_char__d_7
   JSR   double_to_char
   CMP   #$FE
   BEQ   .if_end@16
   LDX   #$00
   LDA   #$11
   RTS
.if_end@16:
   LDA   #$00
   STA   __zpabi_long_to_schar__l_0
   STA   __zpabi_long_to_schar__l_1
   STA   __zpabi_long_to_schar__l_2
   STA   __zpabi_long_to_schar__l_3
   JSR   long_to_schar
   ORA   #$00
   BEQ   .if_end@17
   LDX   #$00
   LDA   #$12
   RTS
.if_end@17:
   LDA   #$82
   STA   __zpabi_ulong_to_schar__l_0
   LDA   #$00
   STA   __zpabi_ulong_to_schar__l_1
   STA   __zpabi_ulong_to_schar__l_2
   STA   __zpabi_ulong_to_schar__l_3
   JSR   ulong_to_schar
   CMP   #$82
   BEQ   .if_end@18
   LDX   #$00
   LDA   #$13
   RTS
.if_end@18:
   LDX   #$29
   LDA   #$C8
   JSR   int_to_uchar
   CMP   #$C8
   BEQ   .if_end@19
   LDX   #$00
   LDA   #$14
   RTS
.if_end@19:
   LDX   #$29
   LDA   #$C8
   JSR   uint_to_uchar
   CMP   #$C8
   BEQ   .if_end@20
   LDX   #$00
   LDA   #$15
   RTS
.if_end@20:
   LDA   #$C8
   STA   __zpabi_long_to_uchar__l_0
   LDA   #$29
   STA   __zpabi_long_to_uchar__l_1
   LDA   #$ED
   STA   __zpabi_long_to_uchar__l_2
   LDA   #$FF
   STA   __zpabi_long_to_uchar__l_3
   JSR   long_to_uchar
   CMP   #$C8
   BEQ   .if_end@21
   LDX   #$00
   LDA   #$16
   RTS
.if_end@21:
   LDA   #$C8
   STA   __zpabi_ulong_to_uchar__ul_0
   LDA   #$00
   STA   __zpabi_ulong_to_uchar__ul_1
   STA   __zpabi_ulong_to_uchar__ul_2
   STA   __zpabi_ulong_to_uchar__ul_3
   JSR   ulong_to_uchar
   CMP   #$C8
   BEQ   .if_end@22
   LDX   #$00
   LDA   #$17
   RTS
.if_end@22:
   LDA   #$48
   STA   __zpabi_double_to_uchar__d_0
   LDA   #$E1
   STA   __zpabi_double_to_uchar__d_1
   LDA   #$7A
   STA   __zpabi_double_to_uchar__d_2
   LDA   #$14
   STA   __zpabi_double_to_uchar__d_3
   LDA   #$AE
   STA   __zpabi_double_to_uchar__d_4
   LDA   #$1F
   STA   __zpabi_double_to_uchar__d_5
   LDA   #$69
   STA   __zpabi_double_to_uchar__d_6
   LDA   #$40
   STA   __zpabi_double_to_uchar__d_7
   JSR   double_to_uchar
   CMP   #$C8
   BEQ   .if_end@23
   LDX   #$00
   LDA   #$18
   RTS
.if_end@23:
   LDA   @34.null_ptr
   BEQ   .if_end@25
   LDX   #$00
   LDA   #$19
   RTS
.if_end@25:
   LDX   #$00
   TXA
   RTS

@34.null_ptr:
   DS.B  2
