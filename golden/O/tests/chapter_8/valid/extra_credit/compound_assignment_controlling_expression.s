SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__sum	EQU	$80
__local_main__i_1	EQU	$81
__local_main__i_0	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__sum,__local_main__i_1,__local_main__i_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$64
   STA   __local_main__i_0
   LDA   #$00
   STA   __local_main__i_1
   STA   __local_main__sum
   LDX   #$00
.loop@0_start:
   LDA   __local_main__sum
   CLC
   ADC   #$02
   STA   __local_main__sum
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   LDA   __local_main__i_0
   SEC
   SBC   #$01
   STA   __local_main__i_0
   BCS   .carry_inc@1
   DEC   __local_main__i_1
.carry_inc@1:
   ORA   __local_main__i_1
   BNE   .loop@0_start
   CPX   #$00
   BNE   .and_false@0
   LDA   __local_main__sum
   CMP   #$C8
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   LDA   #$00
   LDX   #$00
   RTS
