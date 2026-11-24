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
; item l section=.data local=false zero=false
; item i section=.data local=false zero=false
; item u section=.data local=false zero=false
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

l:
   DC.L  $00000001

i:
   DC.W  $0002

u:
   DC.W  $001E
