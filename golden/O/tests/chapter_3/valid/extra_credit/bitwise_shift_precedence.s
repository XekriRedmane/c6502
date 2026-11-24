SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$28
   STA   __local_main__0
   LDA   #$00
   STA   __local_main__1
   LDX   #$10
.shift_loop@0:
   ASL   __local_main__0
   ROL   __local_main__1
   DEX
   BNE   .shift_loop@0
   LDA   __local_main__1
   ASL
   ROR   __local_main__1
   ROR   __local_main__0
   LDX   __local_main__1
   LDA   __local_main__0
   RTS
