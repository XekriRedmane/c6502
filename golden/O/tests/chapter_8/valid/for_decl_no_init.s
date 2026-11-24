SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__x	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__x indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$04
   STA   __local_main__x
   LDX   #$00
   BEQ   .loop@0_start
.main@rot@0:
   LDA   __local_main__1
   CLC
   ADC   #$01
   STA   __local_main__x
   LDA   __local_main__0
   ADC   #$00
   TAX
.loop@0_start:
   LDA   __local_main__x
   SEC
   SBC   #$02
   STA   __local_main__1
   TXA
   SBC   #$00
   STA   __local_main__0
   ORA   __local_main__1
   BNE   .main@rot@0
   LDA   __local_main__x
   RTS
