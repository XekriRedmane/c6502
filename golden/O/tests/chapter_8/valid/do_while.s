SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__a_0	EQU	$80
__local_main__a_1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__a_0,__local_main__a_1 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__a_0
   LDA   #$00
   STA   __local_main__a_1
.loop@0_start:
   ASL   __local_main__a_0
   ROL   __local_main__a_1
   LDA   __local_main__a_0
   SEC
   SBC   #$0B
   LDA   __local_main__a_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .loop@0_start
   LDX   __local_main__a_1
   LDA   __local_main__a_0
   RTS
