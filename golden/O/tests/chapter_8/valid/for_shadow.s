SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__shadow_1	EQU	$80
__local_main__shadow_0	EQU	$81
__local_main__acc_1	EQU	$82
__local_main__acc_0	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__shadow_1,__local_main__shadow_0,__local_main__acc_1,__local_main__acc_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__acc_0
   STA   __local_main__acc_1
   STA   __local_main__shadow_0
   LDY   #$00
   STY   __local_main__shadow_1
.loop@0_start:
   LDA   __local_main__shadow_0
   SEC
   SBC   #$0A
   LDA   __local_main__shadow_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__acc_0
   CLC
   ADC   __local_main__shadow_0
   STA   __local_main__acc_0
   LDA   __local_main__acc_1
   ADC   __local_main__shadow_1
   STA   __local_main__acc_1
   INC   __local_main__shadow_0
   BNE   .loop@0_start
   INC   __local_main__shadow_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_main__acc_1
   BNE   .and_false@0
   LDA   __local_main__acc_0
   CMP   #$2D
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   TYA
   LDX   #$00
   RTS
