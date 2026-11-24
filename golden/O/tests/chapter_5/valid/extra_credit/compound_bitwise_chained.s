SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__2	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=1
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$6D
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   ADC   #$C8
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   STA   __local_main__2
   LDA   #$FA
   AND   HARGS
   CMP   #$28
   BNE   .and_false@0
   LDA   __local_main__2
   CMP   #$55
   BNE   .and_false@0
   LDA   HARGS
   CMP   #$28
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   LDA   #$00
   LDX   #$00
   RTS
