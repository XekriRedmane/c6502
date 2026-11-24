SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__a_1	EQU	$80
__local_main__a_0	EQU	$81
__local_main__b	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__a_1,__local_main__a_0,__local_main__b indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$0A
   STA   __local_main__a_0
   LDA   #$00
   STA   __local_main__a_1
   LDA   #$EC
   STA   __local_main__b
   LDX   #$FF
.main@rot@0:
   LDA   __local_main__a_0
   SEC
   SBC   #$01
   STA   __local_main__a_0
   BCS   .carry_inc@0
   DEC   __local_main__a_1
.carry_inc@0:
   LDA   __local_main__a_1
   BMI   .loop@0_break
   ORA   __local_main__a_0
   BEQ   .loop@0_break
   INC   __local_main__b
   BNE   .loop@0_start
   INX
.loop@0_start:
   TXA
   BMI   .main@rot@0
.loop@0_break:
   LDA   __local_main__a_0
   ORA   __local_main__a_1
   BNE   .and_false@1
   CPX   #$FF
   BNE   .and_false@1
   LDA   __local_main__b
   CMP   #$F5
   BNE   .and_false@1
   LDA   #$01
   LDX   #$00
   RTS
.and_false@1:
   LDA   #$00
   LDX   #$00
   RTS
