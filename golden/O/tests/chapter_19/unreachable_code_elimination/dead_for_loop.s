SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def callee params= locals=__local_callee__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals= indirect=false in_cycle=false
; call main -> target
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

callee:
   SUBROUTINE

   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDX   HARGS+5
   RTS

target:
   SUBROUTINE

   LDX   #$00
   LDA   #$0A
   RTS

main:
   SUBROUTINE

   JMP   target
