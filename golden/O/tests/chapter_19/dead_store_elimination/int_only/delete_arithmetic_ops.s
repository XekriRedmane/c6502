SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals= indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item a section=.data local=false zero=false
; item b section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   LDX   #$00
   LDA   #$05
   RTS

main:
   SUBROUTINE

   JMP   target

a:
   DC.W  $0001

b:
   DC.W  $0002
