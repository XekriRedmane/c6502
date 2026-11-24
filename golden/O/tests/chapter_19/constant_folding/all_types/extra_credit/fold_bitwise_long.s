SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_and params= locals= indirect=false in_cycle=false
; def target_or params= locals= indirect=false in_cycle=false
; def target_shift_left params= locals=__local_target_shift_left__0 indirect=false in_cycle=false
; def target_shift_right params= locals= indirect=false in_cycle=false
; def target_xor params= locals= indirect=false in_cycle=false
; call main -> target_and
; call main -> target_or
; call main -> target_shift_left
; call main -> target_shift_right
; call main -> target_xor
; item target_and section=.text local=false zero=false
; item target_and@tail0 section=.text local=true zero=false
; item target_or section=.text local=false zero=false
; item target_or@tail1 section=.text local=true zero=false
; item target_xor section=.text local=false zero=false
; item target_shift_left section=.text local=false zero=false
; item target_shift_right section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

target_and:
   SUBROUTINE

   LDA   #$0F
   STA   HARGS+8
   LDY   #$00
   JMP   target_and@tail0

target_and@tail0:
   SUBROUTINE

   STY   HARGS+9
   STA   HARGS+10
   STY   HARGS+11
   RTS

target_or:
   SUBROUTINE

   LDA   #$FF
   JMP   target_or@tail1

target_or@tail1:
   SUBROUTINE

   STA   HARGS+8
   LDY   #$0F
   JMP   target_and@tail0

target_xor:
   SUBROUTINE

   LDA   #$F0
   JMP   target_or@tail1

target_shift_left:
   SUBROUTINE

   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDA   #$3E
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+6
   STA   HARGS+9
   LDA   HARGS+7
   STA   HARGS+10
   LDA   HARGS+8
   STA   HARGS+11
   LDA   HARGS+5
   STA   HARGS+8
   RTS

target_shift_right:
   SUBROUTINE

   LDA   #$FF
   STA   HARGS+8
   STA   HARGS+9
   LDA   #$1F
   STA   HARGS+10
   LDA   #$00
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   JSR   target_and
   LDX   #$00
   LDA   #$01
   RTS
