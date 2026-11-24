SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals=__local_target__my_struct,__local_target__0 indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item glob_struct section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #$04
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$02
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod16
   LDA   glob_struct
   LDX   glob_struct+1
   RTS

main:
   SUBROUTINE

   JMP   target

glob_struct:
   DC.W  $000F
