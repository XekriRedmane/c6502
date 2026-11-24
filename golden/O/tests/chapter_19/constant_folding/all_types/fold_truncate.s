SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83

; @zp-link-meta-begin
; def implicit params= locals= indirect=false in_cycle=false
; def int_to_char params= locals= indirect=false in_cycle=false
; def int_to_schar params= locals= indirect=false in_cycle=false
; def int_to_uchar params= locals= indirect=false in_cycle=false
; def long_to_char params= locals= indirect=false in_cycle=false
; def long_to_int params= locals= indirect=false in_cycle=false
; def long_to_schar params= locals= indirect=false in_cycle=false
; def long_to_uchar params= locals= indirect=false in_cycle=false
; def long_to_uint params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def uint_to_char params= locals= indirect=false in_cycle=false
; def uint_to_schar params= locals= indirect=false in_cycle=false
; def uint_to_uchar params= locals= indirect=false in_cycle=false
; def ulong_to_char params= locals= indirect=false in_cycle=false
; def ulong_to_int params= locals= indirect=false in_cycle=false
; def ulong_to_schar params= locals= indirect=false in_cycle=false
; def ulong_to_uchar params= locals= indirect=false in_cycle=false
; def ulong_to_uint params= locals= indirect=false in_cycle=false
; call main -> implicit
; call main -> int_to_char
; call main -> int_to_schar
; call main -> int_to_uchar
; call main -> long_to_char
; call main -> long_to_int
; call main -> long_to_schar
; call main -> long_to_uchar
; call main -> long_to_uint
; call main -> uint_to_char
; call main -> uint_to_schar
; call main -> uint_to_uchar
; call main -> ulong_to_char
; call main -> ulong_to_int
; call main -> ulong_to_schar
; call main -> ulong_to_uchar
; call main -> ulong_to_uint
; item long_to_int section=.text local=false zero=false
; item long_to_uint section=.text local=false zero=false
; item long_to_char section=.text local=false zero=false
; item long_to_schar section=.text local=false zero=false
; item long_to_uchar section=.text local=false zero=false
; item ulong_to_int section=.text local=false zero=false
; item ulong_to_uint section=.text local=false zero=false
; item ulong_to_char section=.text local=false zero=false
; item ulong_to_schar section=.text local=false zero=false
; item ulong_to_uchar section=.text local=false zero=false
; item int_to_char section=.text local=false zero=false
; item int_to_schar section=.text local=false zero=false
; item int_to_uchar section=.text local=false zero=false
; item uint_to_char section=.text local=false zero=false
; item uint_to_schar section=.text local=false zero=false
; item uint_to_uchar section=.text local=false zero=false
; item implicit section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item one section=.data local=false zero=false
; item six section=.data local=false zero=false
; item three section=.data local=false zero=false
; item one_twenty_eight section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

long_to_int:
   SUBROUTINE

   LDX   #$04
   LDA   #$D2
   RTS

long_to_uint:
   SUBROUTINE

   LDX   #$04
   LDA   #$D2
   RTS

long_to_char:
   SUBROUTINE

   LDA   #$FF
   RTS

long_to_schar:
   SUBROUTINE

   LDA   #$80
   RTS

long_to_uchar:
   SUBROUTINE

   LDA   #$FF
   RTS

ulong_to_int:
   SUBROUTINE

   LDX   #$FF
   TXA
   RTS

ulong_to_uint:
   SUBROUTINE

   LDX   #$FF
   TXA
   RTS

ulong_to_char:
   SUBROUTINE

   LDA   #$FF
   RTS

ulong_to_schar:
   SUBROUTINE

   LDA   #$80
   RTS

ulong_to_uchar:
   SUBROUTINE

   LDA   #$FF
   RTS

int_to_char:
   SUBROUTINE

   LDA   #$FA
   RTS

int_to_schar:
   SUBROUTINE

   LDA   #$FF
   RTS

int_to_uchar:
   SUBROUTINE

   LDA   #$FA
   RTS

uint_to_char:
   SUBROUTINE

   LDA   #$FD
   RTS

uint_to_schar:
   SUBROUTINE

   LDA   #$0C
   RTS

uint_to_uchar:
   SUBROUTINE

   LDA   #$FD
   RTS

implicit:
   SUBROUTINE

   LDA   #$FD
   RTS

main:
   SUBROUTINE

   JSR   long_to_int
   CPX   #$04
   BNE   .jcmp_skip@0
   CMP   #$D2
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   long_to_uint
   CPX   #$04
   BNE   .jcmp_skip@1
   CMP   #$D2
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   long_to_char
   STA   __local_main__2
   JSR   main@run0
   STA   __local_main__1
   JSR   main@run1
   STA   __local_main__0
   LDA   #$00
   CMP   __local_main__0
   BNE   .jcmp_skip@2
   LDA   __local_main__2
   CMP   __local_main__1
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   long_to_schar
   STA   __local_main__3
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_main__2
   SEC
   LDA   #$00
   SBC   one_twenty_eight
   STA   __local_main__1
   LDA   #$00
   SBC   one_twenty_eight+1
   STA   __local_main__0
   LDA   __local_main__2
   CMP   __local_main__0
   BNE   .jcmp_skip@5
   LDA   __local_main__3
   CMP   __local_main__1
   BEQ   .if_end@3
.jcmp_skip@5:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   long_to_uchar
   CMP   #$FF
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   JSR   ulong_to_int
   STA   __local_main__3
   JSR   main@run0
   STA   __local_main__1
   JSR   main@run1
   STA   __local_main__0
   CPX   __local_main__0
   BNE   .jcmp_skip@6
   LDA   __local_main__3
   CMP   __local_main__1
   BEQ   .if_end@5
.jcmp_skip@6:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   JSR   ulong_to_uint
   STA   __local_main__1
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@7
   CPX   #$FF
   BNE   .jcmp_skip@7
   LDA   __local_main__1
   CMP   #$FF
   BEQ   .if_end@6
.jcmp_skip@7:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   JSR   ulong_to_char
   STA   __local_main__2
   JSR   main@run0
   STA   __local_main__1
   JSR   main@run1
   STA   __local_main__0
   LDA   #$00
   CMP   __local_main__0
   BNE   .jcmp_skip@8
   LDA   __local_main__2
   CMP   __local_main__1
   BEQ   .if_end@7
.jcmp_skip@8:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   JSR   ulong_to_schar
   STA   __local_main__3
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_main__2
   SEC
   LDA   #$00
   SBC   one_twenty_eight
   STA   __local_main__1
   LDA   #$00
   SBC   one_twenty_eight+1
   STA   __local_main__0
   LDA   __local_main__2
   CMP   __local_main__0
   BNE   .jcmp_skip@11
   LDA   __local_main__3
   CMP   __local_main__1
   BEQ   .if_end@8
.jcmp_skip@11:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   JSR   ulong_to_uchar
   CMP   #$FF
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   JSR   int_to_char
   STA   __local_main__2
   SEC
   LDA   #$00
   SBC   six
   STA   __local_main__1
   LDA   #$00
   SBC   six+1
   STA   __local_main__0
   LDA   #$00
   CMP   __local_main__0
   BNE   .jcmp_skip@12
   LDA   __local_main__2
   CMP   __local_main__1
   BEQ   .if_end@10
.jcmp_skip@12:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   JSR   int_to_schar
   STA   __local_main__3
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_main__2
   JSR   main@run0
   STA   __local_main__1
   JSR   main@run1
   STA   __local_main__0
   LDA   __local_main__2
   CMP   __local_main__0
   BNE   .jcmp_skip@15
   LDA   __local_main__3
   CMP   __local_main__1
   BEQ   .if_end@11
.jcmp_skip@15:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   JSR   int_to_uchar
   CMP   #$FA
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@12:
   JSR   uint_to_char
   STA   __local_main__2
   SEC
   LDA   #$00
   SBC   three
   STA   __local_main__1
   LDA   #$00
   SBC   three+1
   STA   __local_main__0
   LDA   #$00
   CMP   __local_main__0
   BNE   .jcmp_skip@16
   LDA   __local_main__2
   CMP   __local_main__1
   BEQ   .if_end@13
.jcmp_skip@16:
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@13:
   JSR   uint_to_schar
   CMP   #$0C
   BEQ   .if_end@14
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@14:
   JSR   uint_to_uchar
   CMP   #$FD
   BEQ   .if_end@15
   LDX   #$00
   LDA   #$10
   RTS
.if_end@15:
   JSR   implicit
   CMP   #$FD
   BEQ   .if_end@16
   LDX   #$00
   LDA   #$11
   RTS
.if_end@16:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   LDA   #$00
   SBC   one+1
   RTS

main@run0:
   SUBROUTINE

   SEC
   LDA   #$00
   SBC   one
   RTS

one:
   DC.W  $0001

six:
   DC.W  $0006

three:
   DC.W  $0003

one_twenty_eight:
   DC.W  $0080
