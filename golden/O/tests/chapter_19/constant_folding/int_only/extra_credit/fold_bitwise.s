SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_and params= locals= indirect=false in_cycle=false
; def target_or params= locals= indirect=false in_cycle=false
; def target_shift_left params= locals= indirect=false in_cycle=false
; def target_shift_right params= locals= indirect=false in_cycle=false
; def target_xor params= locals= indirect=false in_cycle=false
; call main -> target_and
; call main -> target_or
; call main -> target_shift_left
; call main -> target_shift_right
; call main -> target_xor
; item target_and section=.text local=false zero=false
; item target_and@tail2 section=.text local=true zero=false
; item target_and@tail0 section=.text local=true zero=false
; item target_and@tail0@tail3 section=.text local=true zero=false
; item target_and@tail0@tail3@tail4 section=.text local=true zero=false
; item target_and@tail0@tail3@tail4@tail5 section=.text local=true zero=false
; item target_and@tail0@tail3@tail4@tail5@tail6 section=.text local=true zero=false
; item target_and@tail0@tail3@tail4@tail5@tail6@tail7 section=.text local=true zero=false
; item target_or section=.text local=false zero=false
; item target_or@tail1 section=.text local=true zero=false
; item target_or@tail1@tail8 section=.text local=true zero=false
; item target_xor section=.text local=false zero=false
; item target_shift_left section=.text local=false zero=false
; item target_shift_right section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target_and:
   SUBROUTINE

   LDA   #$0F
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STA   HARGS+18
   TYA
   STA   HARGS+19
   JMP   target_and@tail2

target_and@tail2:
   SUBROUTINE

   STY   HARGS+20
   JMP   target_and@tail0

target_and@tail0:
   SUBROUTINE

   JMP   target_and@tail0@tail3

target_and@tail0@tail3:
   SUBROUTINE

   JMP   target_and@tail0@tail3@tail4

target_and@tail0@tail3@tail4:
   SUBROUTINE

   JMP   target_and@tail0@tail3@tail4@tail5

target_and@tail0@tail3@tail4@tail5:
   SUBROUTINE

   JMP   target_and@tail0@tail3@tail4@tail5@tail6

target_and@tail0@tail3@tail4@tail5@tail6:
   SUBROUTINE

   JMP   target_and@tail0@tail3@tail4@tail5@tail6@tail7

target_and@tail0@tail3@tail4@tail5@tail6@tail7:
   SUBROUTINE

   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

target_or:
   SUBROUTINE

   LDA   #$FF
   JMP   target_or@tail1

target_or@tail1:
   SUBROUTINE

   STA   HARGS+16
   LDY   #$0F
   STY   HARGS+17
   STA   HARGS+18
   STY   HARGS+19
   LDA   #$00
   JMP   target_or@tail1@tail8

target_or@tail1@tail8:
   SUBROUTINE

   STA   HARGS+20
   LDY   #$00
   JMP   target_and@tail0

target_xor:
   SUBROUTINE

   LDA   #$F0
   JMP   target_or@tail1

target_shift_left:
   SUBROUTINE

   LDA   #$8C
   STA   HARGS+18
   LDA   #$04
   STA   HARGS+19
   LDA   #$00
   STA   HARGS+16
   STA   HARGS+17
   JMP   target_or@tail1@tail8

target_shift_right:
   SUBROUTINE

   LDA   #$87
   STA   HARGS+16
   STA   HARGS+17
   LDA   #$07
   STA   HARGS+18
   LDA   #$00
   STA   HARGS+19
   LDY   #$00
   JMP   target_and@tail2

main:
   SUBROUTINE

   JSR   target_and
   LDA   HARGS+23
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   BNE   .jcmp_skip@0
   LDA   HARGS+21
   BNE   .jcmp_skip@0
   LDA   HARGS+20
   BNE   .jcmp_skip@0
   LDA   HARGS+19
   BNE   .jcmp_skip@0
   LDA   HARGS+18
   CMP   #$0F
   BNE   .jcmp_skip@0
   LDA   HARGS+17
   BNE   .jcmp_skip@0
   LDA   HARGS+16
   CMP   #$0F
   BEQ   .if_end@0
.jcmp_skip@0:
   LDA   #$01
   JSR   main@run0
   JMP   target_and@tail2
.if_end@0:
   JSR   target_or
   LDA   HARGS+23
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   BNE   .jcmp_skip@1
   LDA   HARGS+21
   BNE   .jcmp_skip@1
   LDA   HARGS+20
   BNE   .jcmp_skip@1
   LDA   HARGS+19
   CMP   #$0F
   BNE   .jcmp_skip@1
   LDA   HARGS+18
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   HARGS+17
   CMP   #$0F
   BNE   .jcmp_skip@1
   LDA   HARGS+16
   CMP   #$FF
   BEQ   .if_end@1
.jcmp_skip@1:
   LDA   #$02
   JSR   main@run1
   JMP   target_and@tail0@tail3
.if_end@1:
   JSR   target_xor
   LDA   HARGS+23
   BNE   .jcmp_skip@2
   LDA   HARGS+22
   BNE   .jcmp_skip@2
   LDA   HARGS+21
   BNE   .jcmp_skip@2
   LDA   HARGS+20
   BNE   .jcmp_skip@2
   LDA   HARGS+19
   CMP   #$0F
   BNE   .jcmp_skip@2
   LDA   HARGS+18
   CMP   #$F0
   BNE   .jcmp_skip@2
   LDA   HARGS+17
   CMP   #$0F
   BNE   .jcmp_skip@2
   LDA   HARGS+16
   CMP   #$F0
   BEQ   .if_end@2
.jcmp_skip@2:
   LDA   #$03
   JSR   main@run1
   JMP   target_and@tail0@tail3@tail4
.if_end@2:
   JSR   target_shift_left
   LDA   HARGS+23
   BNE   .jcmp_skip@3
   LDA   HARGS+22
   BNE   .jcmp_skip@3
   LDA   HARGS+21
   BNE   .jcmp_skip@3
   LDA   HARGS+20
   BNE   .jcmp_skip@3
   LDA   HARGS+19
   CMP   #$04
   BNE   .jcmp_skip@3
   LDA   HARGS+18
   CMP   #$8C
   BNE   .jcmp_skip@3
   LDA   HARGS+17
   BNE   .jcmp_skip@3
   LDA   HARGS+16
   BEQ   .if_end@3
.jcmp_skip@3:
   LDA   #$04
   JSR   main@run1
   JMP   target_and@tail0@tail3@tail4@tail5
.if_end@3:
   JSR   target_shift_right
   LDA   HARGS+23
   BNE   .jcmp_skip@4
   LDA   HARGS+22
   BNE   .jcmp_skip@4
   LDA   HARGS+21
   BNE   .jcmp_skip@4
   LDA   HARGS+20
   BNE   .jcmp_skip@4
   LDA   HARGS+19
   BNE   .jcmp_skip@4
   LDA   HARGS+18
   CMP   #$07
   BNE   .jcmp_skip@4
   LDA   HARGS+17
   CMP   #$87
   BNE   .jcmp_skip@4
   LDA   HARGS+16
   CMP   #$87
   BEQ   .if_end@4
.jcmp_skip@4:
   LDA   #$05
   JSR   main@run1
   JMP   target_and@tail0@tail3@tail4@tail5@tail6
.if_end@4:
   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   JMP   target_and@tail0@tail3@tail4@tail5@tail6@tail7

main@run1:
   SUBROUTINE

   JSR   main@run0
   STY   HARGS+20
   RTS

main@run0:
   SUBROUTINE

   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   STY   HARGS+18
   STY   HARGS+19
   RTS
