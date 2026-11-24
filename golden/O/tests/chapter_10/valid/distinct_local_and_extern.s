SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def return_a params= locals= indirect=false in_cycle=false
; call main -> return_a
; item return_a section=.text local=false zero=false
; item main section=.text local=false zero=false
; item a section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

return_a:
   SUBROUTINE

   LDX   a+1
   LDA   a
   RTS

main:
   SUBROUTINE

   LDA   a+1
   BNE   .jcmp_skip@0
   LDA   a
   CMP   #$05
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$04
   STA   a
   LDA   #$00
   STA   a+1
   JSR   return_a
   STA   __local_main__0
   LDA   #$03
   CLC
   ADC   __local_main__0
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   RTS

a:
   DC.W  $0005
