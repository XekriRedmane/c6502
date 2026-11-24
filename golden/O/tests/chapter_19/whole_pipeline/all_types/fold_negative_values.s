SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_double_add params= locals= indirect=false in_cycle=false
; def target_double_div params= locals= indirect=false in_cycle=false
; def target_double_sub params= locals= indirect=false in_cycle=false
; def target_long_complement params= locals= indirect=false in_cycle=false
; def target_long_division params= locals= indirect=false in_cycle=false
; def target_long_subtraction params= locals= indirect=false in_cycle=false
; def target_remainder_test params= locals= indirect=false in_cycle=false
; call main -> target_double_add
; call main -> target_double_div
; call main -> target_double_sub
; call main -> target_long_complement
; call main -> target_long_division
; call main -> target_long_subtraction
; call main -> target_remainder_test
; item target_remainder_test section=.text local=false zero=false
; item target_remainder_test@tail0 section=.text local=true zero=false
; item target_long_subtraction section=.text local=false zero=false
; item target_long_division section=.text local=false zero=false
; item target_long_complement section=.text local=false zero=false
; item target_double_add section=.text local=false zero=false
; item target_double_sub section=.text local=false zero=false
; item target_double_div section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target_remainder_test:
   SUBROUTINE

   LDA   #$FB
   JMP   target_remainder_test@tail0

target_remainder_test@tail0:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$FF
   STA   HARGS+9
   LDY   #$FF
   STY   HARGS+10
   STA   HARGS+11
   RTS

target_long_subtraction:
   SUBROUTINE

   LDA   #$AC
   JMP   target_remainder_test@tail0

target_long_division:
   SUBROUTINE

   LDA   #$01
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   STA   HARGS+10
   LDA   #$FE
   STA   HARGS+11
   RTS

target_long_complement:
   SUBROUTINE

   LDA   #$FE
   JMP   target_remainder_test@tail0

target_double_add:
   SUBROUTINE

   LDA   #$6B
   STA   HARGS+16
   LDA   #$7B
   STA   HARGS+17
   LDA   #$53
   STA   HARGS+18
   LDA   #$AC
   STA   HARGS+19
   LDA   #$59
   STA   HARGS+20
   LDA   #$95
   STA   HARGS+21
   LDA   #$68
   STA   HARGS+22
   LDA   #$CC
   STA   HARGS+23
   RTS

target_double_sub:
   SUBROUTINE

   LDA   #$01
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STA   HARGS+22
   LDA   #$80
   STA   HARGS+23
   RTS

target_double_div:
   SUBROUTINE

   LDA   #$B1
   STA   HARGS+16
   LDA   #$EC
   STA   HARGS+18
   LDA   #$9E
   STA   HARGS+19
   LDA   #$3C
   STA   HARGS+20
   LDA   #$2C
   STA   HARGS+21
   LDA   #$CC
   STA   HARGS+22
   LDA   #$BF
   STA   HARGS+17
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   JSR   target_remainder_test
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+8
   CMP   #$FB
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   target_long_subtraction
   LDX   #$00
   LDA   #$02
   RTS
